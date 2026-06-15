RGB_MATRIX_EFFECT(custom_lighting)
RGB_MATRIX_EFFECT(audio_visualizer)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#include "audio_visualizer.h"

#define VISUALIZER_BRIGHTNESS_LIMIT 150
#define WATERDROP_COUNT 6 //6
#define WATERDROP_SPEED 6
#define WATERDROP_BASS_THRESHOLD 187
#define WATERDROP_RETRIGGER_GAP 3  //8
#define WATERDROP_MAX_RADIUS 260
#define AUDIO_ENTRY_WAVE_DURATION_MS ((15 + AUDIO_ENTRY_WAVE_WIDTH) * AUDIO_ENTRY_WAVE_STEP_MS)
#define AUDIO_ENTRY_WAVE_STEP_MS     35
#define AUDIO_ENTRY_WAVE_WIDTH       5
#define AUDIO_ENTRY_WAVE_HEIGHT      4

typedef struct {
    uint16_t radius;
    uint8_t age;
    bool active;
    uint8_t color_index;
} waterdrop_t;

static waterdrop_t waterdrops[WATERDROP_COUNT] = {0};
static uint8_t waterdrop_next_color = 0;
static uint8_t waterdrop_cooldown = 0;

static const uint8_t waterdrop_colors[5] = { 0, 1, 2, 3, 4 };

static uint8_t vis_scale(uint8_t value) {
    return ((uint16_t)value * VISUALIZER_BRIGHTNESS_LIMIT) / 255;
}

static uint8_t absdiff_u8(uint8_t a, uint8_t b) {
    return a > b ? a - b : b - a;
}

static void get_visualizer_bounds(uint8_t *min_x, uint8_t *max_x, uint8_t *min_y, uint8_t *max_y) {
    *min_x = 255;
    *max_x = 0;
    *min_y = 255;
    *max_y = 0;

    for (uint8_t led = 0; led < RGB_MATRIX_LED_COUNT; led++) {
        uint8_t x = g_led_config.point[led].x;
        uint8_t y = g_led_config.point[led].y;

        if (x < *min_x) *min_x = x;
        if (x > *max_x) *max_x = x;
        if (y < *min_y) *min_y = y;
        if (y > *max_y) *max_y = y;
    }
}

static uint8_t visualizer_max_distance(uint8_t center_x, uint8_t min_x, uint8_t max_x) {
    uint8_t left  = center_x - min_x;
    uint8_t right = max_x - center_x;
    return left > right ? left : right;
}

static void render_visualizer_waterdrop_mode(const visualizer_palette_t *palette) {
    uint8_t bass0 = audio_visualizer_get_band(0);
    uint8_t bass1 = audio_visualizer_get_band(1);
    uint8_t bass2 = audio_visualizer_get_band(2);

    uint8_t bass = bass0;
    if (bass1 > bass) bass = bass1;
    if (bass2 > bass) bass = bass2;

    if (waterdrop_cooldown > 0) {
        waterdrop_cooldown--;
    }

    if (bass > WATERDROP_BASS_THRESHOLD && waterdrop_cooldown == 0) {
        for (uint8_t i = 0; i < WATERDROP_COUNT; i++) {
            if (!waterdrops[i].active) {
                waterdrops[i].radius = 0;
                waterdrops[i].age = 255;
                waterdrops[i].active = true;
                waterdrops[i].color_index = waterdrop_next_color;

                waterdrop_next_color++;
                waterdrop_next_color %= 5;

                waterdrop_cooldown = WATERDROP_RETRIGGER_GAP;
                break;
            }
        }
    }

#ifdef SPLIT_KEYBOARD
    const uint8_t left_center_x = 56;
    const uint8_t right_center_x = 168;
    const uint8_t center_y = 32;
#else
    uint8_t min_x, max_x, min_y, max_y;
    get_visualizer_bounds(&min_x, &max_x, &min_y, &max_y);

    uint8_t center_x = min_x + ((max_x - min_x) / 2);
    uint8_t center_y = min_y + ((max_y - min_y) / 2);
#endif

    for (uint8_t led = 0; led < RGB_MATRIX_LED_COUNT; led++) {
        uint8_t x = g_led_config.point[led].x;
        uint8_t y = g_led_config.point[led].y;

#ifdef SPLIT_KEYBOARD
        uint8_t cx = x < 112 ? left_center_x : right_center_x;
        uint8_t dx = absdiff_u8(x, cx);
#else
        uint8_t dx = absdiff_u8(x, center_x);
#endif

        uint8_t dy = absdiff_u8(y, center_y);
        uint8_t dist = sqrt16((uint16_t)dx * dx + (uint16_t)dy * dy);

        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;

        for (uint8_t i = 0; i < WATERDROP_COUNT; i++) {
            if (!waterdrops[i].active) continue;

            uint16_t effect = waterdrops[i].radius - dist;
            if (effect > 255) effect = 255;

            uint8_t brightness = 255 - effect;

            if (brightness > 0) {
                uint8_t delta = absdiff_u8(dist, waterdrops[i].radius);

                uint8_t color;

                if (delta < 12) {
                    color = 4;
                } else if (delta < 28) {
                    color = 3;
                } else if (delta < 44) {
                    color = 2;
                } else if (delta < 64) {
                    color = 1;
                } else {
                    color = 0;
                }

                uint8_t fade = 255;

                if (delta > 0) {
                    fade = qsub8(255, delta * 4);
                }

                uint8_t final_brightness =
                    scale8(brightness, fade);

                final_brightness =
                    scale8(final_brightness, waterdrops[i].age);

                r = qadd8(
                    r,
                    scale8(
                        vis_scale(palette->level_r[color]),
                        final_brightness
                    )
                );

                g = qadd8(
                    g,
                    scale8(
                        vis_scale(palette->level_g[color]),
                        final_brightness
                    )
                );

                b = qadd8(
                    b,
                    scale8(
                        vis_scale(palette->level_b[color]),
                        final_brightness
                    )
                );
            }
        }

        rgb_matrix_set_color(led, r, g, b);
    }

    for (uint8_t i = 0; i < WATERDROP_COUNT; i++) {
        if (!waterdrops[i].active) continue;

        waterdrops[i].radius += WATERDROP_SPEED;

        if (waterdrops[i].age > 8) {
            waterdrops[i].age -= 8;
        } else {
            waterdrops[i].active = false;
        }
    }
}

static bool custom_lighting(effect_params_t *params) {
    return false;
}

static bool visualizer_level_is_active(uint8_t render_mode, uint8_t level, uint8_t height) {
    if (height == 0) return false;

    switch (render_mode) {
        default:
        case 0:
            return level < height;

        case 1:
            return level == height - 1;
    }
}

static void render_visualizer_geometry_bars_mode(const visualizer_palette_t *palette, uint8_t render_mode) {
    for (uint8_t led = 0; led < RGB_MATRIX_LED_COUNT; led++) {
        uint8_t x = g_led_config.point[led].x;
        uint8_t y = g_led_config.point[led].y;

        uint8_t band = ((uint16_t)x * VISUALIZER_BAND_COUNT) / 224;
        uint8_t level = ((uint16_t)y * VISUALIZER_LEVEL_COUNT) / 64;

        if (band >= VISUALIZER_BAND_COUNT) {
            band = VISUALIZER_BAND_COUNT - 1;
        }

        if (level >= VISUALIZER_LEVEL_COUNT) {
            level = VISUALIZER_LEVEL_COUNT - 1;
        }

        level = (VISUALIZER_LEVEL_COUNT - 1) - level;

        uint8_t value = audio_visualizer_get_band(band);
        uint8_t height = ((uint16_t)value * (VISUALIZER_LEVEL_COUNT + 1)) / 256;

        uint8_t peak_value = audio_visualizer_get_peak(band);
        uint8_t peak_height = ((uint16_t)peak_value * (VISUALIZER_LEVEL_COUNT + 1)) / 256;

        if (height > VISUALIZER_LEVEL_COUNT) {
            height = VISUALIZER_LEVEL_COUNT;
        }

        if (peak_height > VISUALIZER_LEVEL_COUNT) {
            peak_height = VISUALIZER_LEVEL_COUNT;
        }

        if (peak_height > height && peak_height > 0 && level == peak_height - 1) {
            rgb_matrix_set_color(
                led,
                vis_scale(palette->peak_r),
                vis_scale(palette->peak_g),
                vis_scale(palette->peak_b)
            );
        } else if (visualizer_level_is_active(render_mode, level, height)) {
            rgb_matrix_set_color(
                led,
                vis_scale(palette->level_r[level]),
                vis_scale(palette->level_g[level]),
                vis_scale(palette->level_b[level])
            );
        } else {
            rgb_matrix_set_color(led, 0, 0, 0);
        }
    }
}

static void render_visualizer_center_mode(const visualizer_palette_t *palette) {
#ifdef SPLIT_KEYBOARD
    const uint8_t center_x = 112;
    const uint8_t max_distance = 112;
#else
    uint8_t min_x, max_x, min_y, max_y;
    get_visualizer_bounds(&min_x, &max_x, &min_y, &max_y);

    uint8_t center_x = min_x + ((max_x - min_x) / 2);
    uint8_t max_distance = visualizer_max_distance(center_x, min_x, max_x);
#endif

    uint8_t values[VISUALIZER_LEVEL_COUNT] = {
        audio_visualizer_get_band(0),
        audio_visualizer_get_band(2),
        audio_visualizer_get_band(4),
        audio_visualizer_get_band(6),
        audio_visualizer_get_band(8),
    };

    for (uint8_t led = 0; led < RGB_MATRIX_LED_COUNT; led++) {
        uint8_t x = g_led_config.point[led].x;
        uint8_t y = g_led_config.point[led].y;

        uint8_t row = ((uint16_t)y * VISUALIZER_LEVEL_COUNT) / 64;

        if (row >= VISUALIZER_LEVEL_COUNT) {
            row = VISUALIZER_LEVEL_COUNT - 1;
        }

        uint8_t value = values[row];
        uint8_t width = ((uint16_t)value * max_distance) / 210;

        if (value == 0 || width == 0) {
            rgb_matrix_set_color(led, 0, 0, 0);
            continue;
        }

        uint8_t peak_value = audio_visualizer_get_peak(row * 2);
        uint8_t peak_width = ((uint16_t)peak_value * max_distance) / 210;

        uint8_t distance = x > center_x ? x - center_x : center_x - x;

        if (distance <= width) {
            uint8_t color_index =
                ((uint16_t)distance * VISUALIZER_LEVEL_COUNT) / max_distance;

            if (color_index >= VISUALIZER_LEVEL_COUNT) {
                color_index = VISUALIZER_LEVEL_COUNT - 1;
            }

            rgb_matrix_set_color(
                led,
                vis_scale(palette->level_r[color_index]),
                vis_scale(palette->level_g[color_index]),
                vis_scale(palette->level_b[color_index])
            );
        } else if (peak_width > width && distance <= peak_width && distance + 8 >= peak_width) {
            rgb_matrix_set_color(
                led,
                vis_scale(palette->peak_r),
                vis_scale(palette->peak_g),
                vis_scale(palette->peak_b)
            );
        } else {
            rgb_matrix_set_color(led, 0, 0, 0);
        }
    }
}

static void render_visualizer_geometry_kitt_mode(const visualizer_palette_t *palette) {
#ifdef SPLIT_KEYBOARD
    const uint8_t center_x = 112;
    const uint8_t max_distance = 112;
#else
    uint8_t min_x, max_x, min_y, max_y;
    get_visualizer_bounds(&min_x, &max_x, &min_y, &max_y);

    uint8_t center_x = min_x + ((max_x - min_x) / 2);
    uint8_t max_distance = visualizer_max_distance(center_x, min_x, max_x);
#endif

    uint8_t values[VISUALIZER_LEVEL_COUNT] = {
        audio_visualizer_get_band(0),
        audio_visualizer_get_band(2),
        audio_visualizer_get_band(4),
        audio_visualizer_get_band(6),
        audio_visualizer_get_band(8),
    };

    for (uint8_t led = 0; led < RGB_MATRIX_LED_COUNT; led++) {
        uint8_t x = g_led_config.point[led].x;
        uint8_t y = g_led_config.point[led].y;

        uint8_t row = ((uint16_t)y * VISUALIZER_LEVEL_COUNT) / 64;

        if (row >= VISUALIZER_LEVEL_COUNT) {
            row = VISUALIZER_LEVEL_COUNT - 1;
        }

        uint8_t value = values[row];
        uint8_t width = ((uint16_t)value * max_distance) / 210;

        if (value == 0 || width == 0) {
            rgb_matrix_set_color(led, 0, 0, 0);
            continue;
        }

        uint8_t peak_value = audio_visualizer_get_peak(row * 2);
        uint8_t peak_width = ((uint16_t)peak_value * max_distance) / 210;

        uint8_t distance = x > center_x ? x - center_x : center_x - x;

        if (distance <= width) {
            rgb_matrix_set_color(
                led,
                vis_scale(palette->level_r[row]),
                vis_scale(palette->level_g[row]),
                vis_scale(palette->level_b[row])
            );
        } else if (peak_width > width && distance <= peak_width && distance + 8 >= peak_width) {
            rgb_matrix_set_color(
                led,
                vis_scale(palette->peak_r),
                vis_scale(palette->peak_g),
                vis_scale(palette->peak_b)
            );
        } else {
            rgb_matrix_set_color(led, 0, 0, 0);
        }
    }
}

static bool render_audio_entry_wave(void) {
    if (!audio_visualizer_entry_wave_running()) {
        return false;
    }

    uint32_t elapsed = audio_visualizer_entry_wave_elapsed();

    if (elapsed > AUDIO_ENTRY_WAVE_DURATION_MS) {
        audio_visualizer_stop_entry_wave();
        return false;
    }

    int16_t head_col = elapsed / AUDIO_ENTRY_WAVE_STEP_MS;

    for (uint8_t led = 0; led < DRIVER_LED_TOTAL; led++) {
        rgb_matrix_set_color(led, 0, 0, 0);
    }

    for (uint8_t row = 0; row < 5; row++) {
        for (uint8_t col = 0; col < 15; col++) {
            int16_t distance = head_col - col;

            if (distance < 0 || distance >= AUDIO_ENTRY_WAVE_WIDTH) {
                continue;
            }

            uint8_t height = AUDIO_ENTRY_WAVE_HEIGHT - distance;

            if (row < 5 - height) {
                continue;
            }

            uint8_t inverted_row = 4 - row;
            uint8_t inverted_col = 14 - col;
            uint8_t led = (inverted_row * 15) + inverted_col;

            if (led >= DRIVER_LED_TOTAL) {
                continue;
            }

            uint8_t hue = 128 + (col * 8) + (row * 18);
            RGB rgb = hsv_to_rgb((HSV){ hue, 255, 255 });

            rgb_matrix_set_color(led, rgb.r, rgb.g, rgb.b);
        }
    }

    return true;
}

static bool audio_visualizer(effect_params_t *params) {

    if (render_audio_entry_wave()) {
        return false;
    }

    if (!audio_visualizer_is_active()) {
        return false;
    }

    uint8_t render_mode =
        audio_visualizer_get_render_mode();

    const visualizer_palette_t *palette =
        audio_visualizer_get_palette();

    if (render_mode == 4) {
        render_visualizer_waterdrop_mode(palette);
    } else if (render_mode == 3) {
        render_visualizer_geometry_kitt_mode(palette);
    } else if (render_mode == 2) {
        render_visualizer_center_mode(palette);
    } else {
        render_visualizer_geometry_bars_mode(palette, render_mode);
    }

#ifdef SPLIT_KEYBOARD
    if (audio_visualizer_underglow_enabled()) {
        static const uint8_t bass_left[]  = { 1, 2, 3, 4, 5, 6 };
        static const uint8_t bass_right[] = { 37, 38, 39, 40, 41, 42 };

        uint8_t bass0 = audio_visualizer_get_band(0);
        uint8_t bass1 = audio_visualizer_get_band(1);
        uint8_t bass  = bass0 > bass1 ? bass0 : bass1;

        if (bass < 25) {
            bass = 0;
        }

        uint8_t brightness = ((uint16_t)bass * bass) / 255;

        uint8_t r = ((uint16_t)palette->underglow_r * brightness) / 255;
        uint8_t g = ((uint16_t)palette->underglow_g * brightness) / 255;
        uint8_t b = ((uint16_t)palette->underglow_b * brightness) / 255;

        for (uint8_t i = 0; i < 6; i++) {
            rgb_matrix_set_color(bass_left[i],  vis_scale(r), vis_scale(g), vis_scale(b));
            rgb_matrix_set_color(bass_right[i], vis_scale(r), vis_scale(g), vis_scale(b));
        }
    } else {
        static const uint8_t bass_left[]  = { 1, 2, 3, 4, 5, 6 };
        static const uint8_t bass_right[] = { 37, 38, 39, 40, 41, 42 };

        for (uint8_t i = 0; i < 6; i++) {
            rgb_matrix_set_color(bass_left[i],  0, 0, 0);
            rgb_matrix_set_color(bass_right[i], 0, 0, 0);
        }
    }
#endif

    return false;
}

#endif