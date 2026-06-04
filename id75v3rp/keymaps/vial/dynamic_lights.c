#include QMK_KEYBOARD_H
#include "dynamic_lights.h"

#if defined(RGB_MATRIX_ENABLE)

#define KEY_ROWS                  5
#define KEY_COLS                  15
#define KEY_LED_COUNT             75

#define STARTUP_STEP_MS           50
#define STARTUP_TAIL              7
#define STARTUP_DELAY_MS          1000
#define STARTUP_SWITCH_STEP_MS    25

#define KEYMAP_CHECK_INTERVAL_MS  250
#define CACHE_INVALID_COLOR       0xFF

#define LAYER_ALL                 0xFFFFFFFFUL
#define LAYER_MASK(n)             (1UL << (n))

#define L_ALL        LAYER_ALL
#define L(n)         LAYER_MASK(n)
#define L_RANGE(a,b) (((1UL << ((b) - (a) + 1)) - 1) << (a))

#define BLINK_1HZ(color) (((timer_read32() / 600) % 2) ? (color) : CLR_OFF)
#define BLINK_2HZ(color) (((timer_read32() / 200) % 2) ? (color) : CLR_OFF)
#define BLINK_CHECK_INTERVAL_MS 100

// replacing Direct Control with our key --> color effect
#define MY_CUSTOM_RGB_MODE        45

enum { // color ids
    CLR_OFF = 0,
    CLR_RED,
    CLR_LIGHTRED,
    CLR_YELLOW,
    CLR_PINK,
    CLR_GREEN,
    CLR_DARKGREEN,
    CLR_LIGHTGREEN,
    CLR_LEMONGREEN,
    CLR_BLUE,
    CLR_LIGHTBLUE,
    CLR_CYAN,
    CLR_LIGHTCYAN,
    CLR_PURPLE,
    CLR_ROSE,
    CLR_WHITE,
    CLR_ORANGE,
    CLR_LIGHTORANGE,
    CLR_DARKORANGE,
    CLR_GREY
};

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb_color_t;

static const rgb_color_t color_palette[] = {
    [CLR_OFF]         = {   0,   0,   0 },
    [CLR_RED]         = { 255,   0,   0 },
    [CLR_LIGHTRED]    = { 255, 102, 102 },
    [CLR_YELLOW]      = { 255, 210,   0 },
    [CLR_PINK]        = { 255,   0, 128 },
    [CLR_GREEN]       = {   0, 255,   0 },
    [CLR_DARKGREEN]   = {   0,  51,  25 },
    [CLR_LIGHTGREEN]  = {  51, 255, 153 },
    [CLR_LEMONGREEN]  = { 153, 255,  51 },
    [CLR_BLUE]        = {   0,  80, 255 },
    [CLR_LIGHTBLUE]   = { 153, 204, 255 },
    [CLR_CYAN]        = {   0, 255, 255 },
    [CLR_LIGHTCYAN]   = { 204, 255, 255 },
    [CLR_PURPLE]      = {  76,   0, 153 },
    [CLR_ROSE]        = { 153,   0,  76 },
    [CLR_WHITE]       = { 255, 255, 255 },
    [CLR_ORANGE]      = { 255, 100,   0 },
    [CLR_LIGHTORANGE] = { 255, 204, 153 },
    [CLR_DARKORANGE]  = {  63,  25,   0 },
    [CLR_GREY]        = {  24,  24,  24 },
};

typedef struct {
    uint16_t keycode;
    uint8_t color_id;
    uint32_t layer_mask;
} key_color_rule_t;

// Layer helpers:
//
// L_ALL              All layers
// L(0)               Layer 0 only
// L(1)               Layer 1 only
// L(2) | L(4)        Layers 2 and 4
// L_RANGE(1, 3)      Layers 1 through 3
// L_RANGE(1, 3) | L(5)
//                    Layers 1 through 3 and layer 5

static const key_color_rule_t key_color_rules[] = {
    // Examples:
    // { KC_ESC,  CLR_CYAN,   L_ALL },
    // { KC_TAB,  CLR_BLUE,   L(1) },
    // { KC_LEFT, CLR_RED,    L(1) | L(4) },
    // { KC_F1,   CLR_GREEN,  L_RANGE(1, 3) },
    // { KC_F3,   CLR_RED,    L_RANGE(2, 4) },
    // { KC_F2,   CLR_YELLOW, L_RANGE(1, 3) | L(5) },

    { KC_ENT,       CLR_RED,        L_ALL },
    { KC_SPC,       CLR_YELLOW,     L_ALL },
    { KC_BSPC,      CLR_PINK,       L_ALL },
    { KC_DELETE,    CLR_BLUE,       L_ALL },

    { KC_LEFT,      CLR_RED,        L_RANGE(1, 4) },
    { KC_DOWN,      CLR_BLUE,       L_RANGE(1, 4) },
    { KC_UP,        CLR_GREEN,      L_RANGE(1, 4) },
    { KC_RGHT,      CLR_YELLOW,     L_RANGE(1, 4) },

    { KC_LSFT,      CLR_ORANGE,     L_RANGE(1, 4) },
    { KC_RSFT,      CLR_ORANGE,     L_RANGE(1, 4) },
    { KC_LCTL,      CLR_PINK,       L_RANGE(1, 4) },
    { KC_RCTL,      CLR_PINK,       L_RANGE(1, 4) },
    { KC_LGUI,      CLR_BLUE,       L_RANGE(1, 4) },
    { KC_RGUI,      CLR_BLUE,       L_RANGE(1, 4) },
    { KC_LALT,      CLR_PURPLE,     L_RANGE(1, 4) },
    { KC_RALT,      CLR_PURPLE,     L_RANGE(1, 4) },
    { KC_TAB,       CLR_LIGHTCYAN,  L_RANGE(1, 4) },

    { C(S(KC_TAB)), CLR_PURPLE,     L(1) },
    { C(KC_TAB),    CLR_ORANGE,     L(1) },

    { KC_P0,        CLR_BLUE,       L_ALL },
    { KC_P1,        CLR_BLUE,       L_ALL },
    { KC_P2,        CLR_BLUE,       L_ALL },
    { KC_P3,        CLR_BLUE,       L_ALL },
    { KC_P4,        CLR_BLUE,       L_ALL },
    { KC_P5,        CLR_ORANGE,     L_ALL },
    { KC_P6,        CLR_BLUE,       L_ALL },
    { KC_P7,        CLR_BLUE,       L_ALL },
    { KC_P8,        CLR_BLUE,       L_ALL },
    { KC_P9,        CLR_BLUE,       L_ALL },
    { KC_PPLS,      CLR_GREEN,      L_ALL },
    { KC_PAST,      CLR_GREEN,      L_ALL },
    { KC_PMNS,      CLR_RED,        L_ALL },
    { KC_PSLS,      CLR_RED,        L_ALL }
};

static uint32_t startup_timer = 0;
static uint32_t startup_delay_timer = 0;
static bool startup_done = false;

static uint32_t keymap_check_timer = 0;
static bool lighting_cache_initialized = false;

static uint8_t cached_color_ids[KEY_LED_COUNT];
static layer_state_t cached_layer_state = 0;
static uint8_t cached_rgb_value = 0;
static uint8_t cached_led_state_raw = 0;
static bool startup_from_mode_switch = false;
static uint8_t last_rgb_mode = 0;

static uint8_t get_led_index(uint8_t row, uint8_t col) {
    uint8_t inverted_row = (KEY_ROWS - 1) - row;
    uint8_t inverted_col = (KEY_COLS - 1) - col;
    return (inverted_row * KEY_COLS) + inverted_col;
}

static uint8_t color_for_keycode(uint16_t keycode, uint8_t layer) {
    if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) ||
        (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX)) {

        uint8_t base_key = keycode & 0xFF;

        switch (base_key) {
            case KC_A:
            case KC_SCLN:
                return CLR_YELLOW;

            case KC_S:
            case KC_L:
                return CLR_GREEN;

            case KC_D:
            case KC_K:
                return CLR_BLUE;

            case KC_F:
            case KC_J:
                return CLR_RED;

            default:
                keycode = base_key;
                break;
        }
    }

    if (keycode == MO(1)) return CLR_GREY;
    if (keycode == MO(2)) return CLR_GREY;
    if (keycode == MO(3)) return CLR_GREY;
    if (keycode == MO(4)) return CLR_GREY;

    if (keycode == TG(1)) return CLR_GREY;
    if (keycode == TG(2)) return CLR_GREY;
    if (keycode == TG(3)) return CLR_GREY;
    if (keycode == TG(4)) return CLR_GREY;

    if (keycode == TO(1)) return CLR_GREY;
    if (keycode == TO(2)) return CLR_GREY;
    if (keycode == TO(3)) return CLR_GREY;
    if (keycode == TO(4)) return CLR_GREY;

    if (keycode == OSL(1)) return CLR_GREY;
    if (keycode == OSL(2)) return CLR_GREY;
    if (keycode == OSL(3)) return CLR_GREY;
    if (keycode == OSL(4)) return CLR_GREY;

    if (keycode == TT(1)) return CLR_GREY;
    if (keycode == TT(2)) return CLR_GREY;
    if (keycode == TT(3)) return CLR_GREY;
    if (keycode == TT(4)) return CLR_GREY;

    if (keycode == TD(0)) return CLR_GREY;
    if (keycode == TD(1)) return CLR_GREY;
    if (keycode == TD(2)) return CLR_GREY;
    if (keycode == TD(3)) return CLR_GREY;

//    if (keycode == EE_CLR) {
//        return BLINK_2HZ(CLR_RED);
//    }
//
//    if (keycode == QK_BOOT) {
//        return BLINK_1HZ(CLR_ORANGE);
//    }

    if (keycode == EE_CLR) {
        //fast: (timer_read32() / 400) % 2
        //slower: (timer_read32() / 600) % 2
        //even slower: (timer_read32() / 800) % 2
        return (timer_read32() / 800) % 2
            ? CLR_RED
            : CLR_DARKORANGE;
    }

    if (keycode == QK_BOOT) {
        return (timer_read32() / 800) % 2
            ? CLR_ORANGE
            : CLR_DARKORANGE;
    }

    if (keycode == QK_REBOOT) {
        return (timer_read32() / 800) % 2
            ? CLR_GREEN
            : CLR_DARKGREEN;
    }

    if (keycode == KC_CAPS) {
        return host_keyboard_led_state().caps_lock ? CLR_WHITE : CLR_OFF;
    }

    if (keycode == KC_NUM) {
        return host_keyboard_led_state().num_lock ? CLR_WHITE : CLR_OFF;
    }

    for (uint8_t i = 0; i < ARRAY_SIZE(key_color_rules); i++) {
        if (key_color_rules[i].keycode == keycode &&
            (key_color_rules[i].layer_mask == LAYER_ALL ||
             (key_color_rules[i].layer_mask & LAYER_MASK(layer)))) {
            return key_color_rules[i].color_id;
        }
    }

    return CLR_OFF;
}

static void set_color_id(uint8_t led, uint8_t color_id) {
    if (led == NO_LED) {
        return;
    }

    if (color_id == CLR_OFF || color_id >= ARRAY_SIZE(color_palette)) {
        rgb_matrix_set_color(led, 0, 0, 0);
        return;
    }

    uint8_t global_val = rgb_matrix_config.hsv.v;
    rgb_color_t color = color_palette[color_id];

    rgb_matrix_set_color(
        led,
        (uint16_t)color.r * global_val / 255,
        (uint16_t)color.g * global_val / 255,
        (uint16_t)color.b * global_val / 255
    );
}

static void clear_all_leds(void) {
    for (uint8_t i = 0; i < KEY_LED_COUNT; i++) {
        rgb_matrix_set_color(i, 0, 0, 0);
    }
}

static void render_keycode_lighting_full(void) {
    uint8_t current_layer = get_highest_layer(layer_state);

    clear_all_leds();

    for (uint8_t i = 0; i < KEY_LED_COUNT; i++) {
        cached_color_ids[i] = CACHE_INVALID_COLOR;
    }

    for (uint8_t row = 0; row < KEY_ROWS; row++) {
        for (uint8_t col = 0; col < KEY_COLS; col++) {
            uint16_t keycode = dynamic_keymap_get_keycode(current_layer, row, col);
            uint8_t led = get_led_index(row, col);
            uint8_t color_id = color_for_keycode(keycode, current_layer);

            set_color_id(led, color_id);

            if (led != NO_LED) {
                cached_color_ids[led] = color_id;
            }
        }
    }

    cached_layer_state = layer_state;
    cached_rgb_value = rgb_matrix_config.hsv.v;
    cached_led_state_raw = host_keyboard_led_state().raw;
    lighting_cache_initialized = true;
    keymap_check_timer = timer_read32();
}

static void apply_cached_lighting_to_buffer(void) {
    for (uint8_t led = 0; led < KEY_LED_COUNT; led++) {
        if (cached_color_ids[led] == CACHE_INVALID_COLOR) {
            continue;
        }

        set_color_id(led, cached_color_ids[led]);
    }
}

static void render_keycode_lighting_cached(void) {

    uint8_t current_led_state_raw = host_keyboard_led_state().raw;

    bool needs_full_refresh =
        !lighting_cache_initialized ||
        cached_layer_state != layer_state ||
        cached_rgb_value != rgb_matrix_config.hsv.v ||
        cached_led_state_raw != current_led_state_raw;

    if (needs_full_refresh) {
        render_keycode_lighting_full();
        return;
    }

    if (timer_elapsed32(keymap_check_timer) >= BLINK_CHECK_INTERVAL_MS) {
        
        keymap_check_timer = timer_read32();

        uint8_t current_layer = get_highest_layer(layer_state);

        for (uint8_t row = 0; row < KEY_ROWS; row++) {
            for (uint8_t col = 0; col < KEY_COLS; col++) {
                uint16_t keycode = dynamic_keymap_get_keycode(current_layer, row, col);
                uint8_t led = get_led_index(row, col);

                if (led == NO_LED) {
                    continue;
                }

                uint8_t color_id = color_for_keycode(keycode, current_layer);

                if (cached_color_ids[led] != color_id) {
                    cached_color_ids[led] = color_id;
                }
            }
        }
    }

    apply_cached_lighting_to_buffer();
}

void keyboard_post_init_user(void) {
    startup_delay_timer = timer_read32();
}

static uint8_t startup_snake_led(uint8_t pos) {
    uint8_t row = pos / KEY_COLS;
    uint8_t col = pos % KEY_COLS;

    if (row >= KEY_ROWS) {
        return NO_LED;
    }

    if (row % 2 == 0) {
        return KEY_LED_COUNT - 1 - pos;
    }

    return KEY_LED_COUNT - KEY_COLS - (row * KEY_COLS) + col;
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t current_rgb_mode = rgb_matrix_get_mode();

    if (current_rgb_mode != MY_CUSTOM_RGB_MODE) {
        last_rgb_mode = current_rgb_mode;
        return false;
    }

    if (last_rgb_mode != MY_CUSTOM_RGB_MODE && last_rgb_mode != 0) {
        startup_done = false;
        startup_timer = 0;
        startup_delay_timer = timer_read32();
        startup_from_mode_switch = true;
        lighting_cache_initialized = false;
    }

    last_rgb_mode = current_rgb_mode;

    if (!startup_done) {
        uint16_t startup_delay = startup_from_mode_switch ? 0 : STARTUP_DELAY_MS;

        if (timer_elapsed32(startup_delay_timer) < startup_delay) {
            return false;
        }

        if (startup_timer == 0) {
            startup_timer = timer_read32();
        }

        uint32_t elapsed = timer_elapsed32(startup_timer);

        uint16_t startup_step =
            startup_from_mode_switch
                ? STARTUP_SWITCH_STEP_MS
                : STARTUP_STEP_MS;

        uint8_t head = elapsed / startup_step;

        if (head > KEY_LED_COUNT + STARTUP_TAIL) {
            startup_done = true;
            startup_from_mode_switch = false;

            clear_all_leds();
            lighting_cache_initialized = false;
            return false;
        }

        clear_all_leds();

        for (uint8_t tail = 0; tail < STARTUP_TAIL; tail++) {
            int16_t pos = head - tail;

            if (pos < 0 || pos >= KEY_LED_COUNT) {
                continue;
            }

            uint8_t led = startup_snake_led(pos);

            if (led == NO_LED) {
                continue;
            }

            uint8_t hue = (elapsed / 8) + (pos * 10);
            uint8_t value = 255 - ((uint16_t)tail * 120 / STARTUP_TAIL);

            HSV hsv = { hue, 255, value };
            RGB rgb = hsv_to_rgb(hsv);

            rgb_matrix_set_color(led, rgb.r, rgb.g, rgb.b);
        }

        return false;
    }

    render_keycode_lighting_cached();
    // rgb_matrix_set_color(0, 255, 0, 0);
    // rgb_matrix_set_color(1, 0, 255, 0);
    // rgb_matrix_set_color(2, 0, 0, 255);

    return false;
}

#endif