#include "audio_visualizer.h"
#ifdef SPLIT_KEYBOARD
#    include "transactions.h"
#endif
#include <string.h>

#ifdef RAW_ENABLE

#define VISUALIZER_SYNC_SIZE ((VISUALIZER_BAND_COUNT * 2) + 3)

static uint8_t visualizer_bands[VISUALIZER_BAND_COUNT] = {0};
static uint8_t visualizer_peaks[VISUALIZER_BAND_COUNT] = {0};
static bool visualizer_active = false;
static bool visualizer_underglow_enabled = false;
static uint8_t visualizer_mode = 0;
static uint8_t visualizer_render_mode = 0;

void audio_visualizer_next_mode(void) {
    visualizer_mode++;
    visualizer_mode %= 4;
}

void audio_visualizer_next_render_mode(void) {
    visualizer_render_mode++;
    visualizer_render_mode %= 5;
}

uint8_t audio_visualizer_get_mode(void) {
    return visualizer_mode;
}

uint8_t audio_visualizer_get_render_mode(void) {
    return visualizer_render_mode;
}

void audio_visualizer_toggle_underglow(void) {
    visualizer_underglow_enabled = !visualizer_underglow_enabled;
}

uint8_t audio_visualizer_underglow_enabled(void) {
    return visualizer_underglow_enabled ? 1 : 0;
}

#ifdef SPLIT_KEYBOARD

static uint8_t visualizer_sync_data[VISUALIZER_SYNC_SIZE];

static void visualizer_sync_handler(uint8_t in_size, const void *in_data,
                                    uint8_t out_size, void *out_data) {
    (void)out_size;
    (void)out_data;

    if (in_size != VISUALIZER_SYNC_SIZE || in_data == NULL) return;

    memcpy(visualizer_bands, in_data, VISUALIZER_BAND_COUNT);
    memcpy(
        visualizer_peaks,
        ((const uint8_t *)in_data) + VISUALIZER_BAND_COUNT,
        VISUALIZER_BAND_COUNT
    );
    visualizer_underglow_enabled =
        (((const uint8_t *)in_data)[VISUALIZER_BAND_COUNT * 2] != 0);

    visualizer_mode =
        ((const uint8_t *)in_data)[(VISUALIZER_BAND_COUNT * 2) + 1];
    
    visualizer_render_mode =
        ((const uint8_t *)in_data)[(VISUALIZER_BAND_COUNT * 2) + 2];

    visualizer_active = true;
}

void audio_visualizer_register_rpc(void) {
    transaction_register_rpc(USER_SYNC_VISUALIZER, visualizer_sync_handler);
}

void audio_visualizer_sync_to_slave(void) {
    if (!is_keyboard_master()) return;

    memcpy(&visualizer_sync_data[0], visualizer_bands, VISUALIZER_BAND_COUNT);
    memcpy(
        &visualizer_sync_data[VISUALIZER_BAND_COUNT],
        visualizer_peaks,
        VISUALIZER_BAND_COUNT
    );

    visualizer_sync_data[VISUALIZER_BAND_COUNT * 2] =
        visualizer_underglow_enabled ? 1 : 0;

    visualizer_sync_data[(VISUALIZER_BAND_COUNT * 2) + 1] =
        visualizer_mode;

    visualizer_sync_data[(VISUALIZER_BAND_COUNT * 2) + 2] =
        visualizer_render_mode;

    transaction_rpc_send(
        USER_SYNC_VISUALIZER,
        VISUALIZER_SYNC_SIZE,
        visualizer_sync_data

    );
}
#else
void audio_visualizer_register_rpc(void) {}
void audio_visualizer_sync_to_slave(void) {}
#endif

// WinAmp Mode
static const visualizer_palette_t palette_winamp = {
    .peak_r = 255, .peak_g = 255, .peak_b = 255,

    .level_r = { 0, 160, 255, 255, 255 },
    .level_g = { 255, 255, 220, 80, 0 },
    .level_b = { 0, 0, 0, 0, 0 },

    .underglow_r = 255,
    .underglow_g = 90,
    .underglow_b = 0,
};
//Purple rain
static const visualizer_palette_t palette_purple = {
    .peak_r = 255, .peak_g = 60, .peak_b = 0,

    .level_r = { 76, 180, 0, 0, 255 },
    .level_g = { 0, 0, 0, 255, 255 },
    .level_b = { 153, 255, 255, 255, 255 },

    .underglow_r = 76,
    .underglow_g = 0,
    .underglow_b = 153,
};
// Ice Queen
static const visualizer_palette_t palette_ice = {
    .peak_r = 255, .peak_g = 255, .peak_b = 255,

    .level_r = { 0, 0, 0, 120, 255 },
    .level_g = { 80, 180, 255, 255, 255 },
    .level_b = { 255, 255, 255, 255, 255 },

    .underglow_r = 0,
    .underglow_g = 80,
    .underglow_b = 255,
};
// Fire Bird
static const visualizer_palette_t palette_fire = {
    .peak_r = 255, .peak_g = 255, .peak_b = 255,

    .level_r = { 120, 255, 255, 255, 255 },
    .level_g = { 0, 0, 80, 180, 255 },
    .level_b = { 0, 0, 0, 0, 0 },

    .underglow_r = 255,
    .underglow_g = 0,
    .underglow_b = 0,
};
//Color Getter
const visualizer_palette_t *audio_visualizer_get_palette(void) {
    switch (visualizer_mode) {
        default:
        case 0: return &palette_winamp;
        case 1: return &palette_purple;
        case 2: return &palette_ice;
        case 3: return &palette_fire;
    }
}

uint8_t audio_visualizer_is_active(void) {
    return visualizer_active ? 1 : 0;
}

uint8_t audio_visualizer_get_band(uint8_t index) {
    if (index >= VISUALIZER_BAND_COUNT) return 0;
    return visualizer_bands[index];
}

uint8_t audio_visualizer_get_peak(uint8_t index) {
    if (index >= VISUALIZER_BAND_COUNT) return 0;
    return visualizer_peaks[index];
}

void raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    static uint32_t raw_debug_timer = 0;

    if (timer_elapsed32(raw_debug_timer) > 1000) {
        raw_debug_timer = timer_read32();
        // uprintf("RAW KB len=%u cmd=%u sub=%u\n", length, data[0], data[1]);
    }

    if (length < 26) return;
    if (data[0] != 0x02) return;
    if (data[1] != 0xA1) return;

    for (uint8_t i = 0; i < VISUALIZER_BAND_COUNT; i++) {
        visualizer_bands[i] = data[i + 2];
        visualizer_peaks[i] = data[i + 2 + VISUALIZER_BAND_COUNT];
    }

    visualizer_active = true;

    static uint32_t visualizer_sync_timer = 0;

    if (timer_elapsed32(visualizer_sync_timer) > 50) {
        visualizer_sync_timer = timer_read32();
        audio_visualizer_sync_to_slave();
    }
}

#else

uint8_t audio_visualizer_is_active(void) { return 0; }
uint8_t audio_visualizer_get_band(uint8_t index) { return 0; }
uint8_t audio_visualizer_get_peak(uint8_t index) { return 0; }
void audio_visualizer_toggle_underglow(void) {}
uint8_t audio_visualizer_underglow_enabled(void) { return 0; }
uint8_t audio_visualizer_get_mode(void) { return 0; }
void audio_visualizer_next_mode(void) {}
const visualizer_palette_t *audio_visualizer_get_palette(void) { return NULL; }
void audio_visualizer_next_render_mode(void) {}
uint8_t audio_visualizer_get_render_mode(void) { return 0; }

void raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    (void)data;
    (void)length;
}

#endif