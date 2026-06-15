#include QMK_KEYBOARD_H
#include "dynamic_lights.h"
#include "audio_visualizer.h"

#define KEYBIND_USER06 0x7E06
#define KEYBIND_USER07 0x7E07
#define KEYBIND_USER08 0x7E08
#define KEYBIND_USER09 0x7E09
#define KEYBIND_USER10 0x7E0A

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [0] = LAYOUT_ortho_5x15(KC_ESC, KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_HOME, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, 
                            KC_TAB, KC_NO, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_PGUP, KC_Z, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, 
                            KC_CAPS, KC_NO, KC_A, KC_S, KC_D, KC_F, KC_G, KC_PGDN, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT, 
                            KC_LSFT, KC_BSLS, KC_Y, KC_X, KC_C, KC_V, KC_B, KC_UP, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_INT1, KC_RSFT, 
                            KC_LCTL, KC_LGUI, KC_LALT, MO(2), MO(1), KC_SPC, KC_LEFT, KC_DOWN, KC_RGHT, TD(3), KC_BSPC, KC_DEL, KC_RALT, KC_RGUI, KC_RCTL),

    [1] = LAYOUT_ortho_5x15(KC_ESC, KC_TRNS, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_TRNS, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, 
                            KC_TAB, KC_NO, LAG(KC_LEFT), LGUI(KC_LCTL), LAG(KC_RGHT), KC_NO, KC_NO, KC_HOME, KC_PGDN, KC_PGUP, KC_END, RALT(KC_MINS), KC_NO, KC_NO, KC_NO, 
                            RALT(KC_7), LGUI(KC_SLSH), LGUI(KC_N), LCTL(LSFT(KC_LGUI)), LGUI(KC_LSFT), LCTL(LSFT(KC_TAB)), KC_WBAK, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, LSFT(KC_8), RALT(KC_8), KC_NO, KC_NO, 
                            RALT(KC_0), RALT(KC_LBRC), RALT(KC_RBRC), LSFT(KC_EQL), KC_EQL, LCTL(KC_TAB), KC_WFWD, KC_NUBS, LCTL(KC_LEFT), LCTL(KC_RGHT), LSFT(KC_INT1), LSFT(KC_9), RALT(KC_9), KC_NO, KC_NO, 
                            RM_TOGG, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_LGUI, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO),

    [2] = LAYOUT_ortho_5x15(LM_TOGG, KC_NO, KC_NO, KC_NO, KC_NO, LGUI(KC_L), KC_SCRL, KC_NO, KC_NO, KC_NUM, KEYBIND_USER06, KEYBIND_USER07, KEYBIND_USER08, KEYBIND_USER09, KEYBIND_USER10, 
                            UG_TOGG, KC_NO, KC_NO, KC_NO, RALT(KC_E), KC_NO, KC_PAUS, KC_NO, KC_PPLS, KC_P7, KC_P8, KC_P9, KC_PAST, KC_NO, KC_NO, 
                            RM_NEXT, RM_HUEU, RM_SATU, RM_VALU, RM_SPDU, KC_NO, KC_PSCR, LSFT(KC_TAB), KC_PMNS, KC_P4, KC_P5, KC_P6, KC_PSLS, KC_NO, KC_NO, 
                            RM_PREV, RM_HUED, RM_SATD, RM_VALD, RM_SPDD, KC_NO, LCA(KC_NO), KC_TAB, KC_NO, KC_P1, KC_P2, KC_P3, KC_NO, KC_NO, KC_NO, 
                            RM_TOGG, KC_NO, KC_NO, KC_TRNS, TG(3), KC_NO, KC_NO, KC_NO, KC_NO, KC_P0, KC_PDOT, KC_PENT, KC_NO, KC_NO, KC_NO),

    [3] = LAYOUT_ortho_5x15(KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, 
                            KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_MUTE, KC_VOLD, KC_VOLU, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, 
                            KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_MPLY, KC_MRWD, KC_MNXT, KC_MPRV, KC_MFFD, KC_NO, KC_NO, EE_CLR, 
                            KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, QK_RBT, 
                            KC_NO, KC_NO, KC_NO, KC_NO, TG(3), KC_SPC, KC_NO, KC_NO, KC_NO, KC_ENT, KC_BSPC, KC_DEL, KC_NO, KC_NO, QK_BOOT),

    [4] = LAYOUT_ortho_5x15(KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, 
                            KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, 
                            KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, 
                            KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, 
                            KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO) 

};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KEYBIND_USER06:
            if (record->event.pressed) {
                uprintf("USER06 pressed -> custom lighting mode\n");
                rgb_matrix_mode(RGB_MATRIX_CUSTOM_custom_lighting);
            }
            return false;

        case KEYBIND_USER07:
            if (record->event.pressed) {
                uprintf("USER07 pressed -> audio visualizer mode\n");
                rgb_matrix_mode(RGB_MATRIX_CUSTOM_audio_visualizer);
                audio_visualizer_start_entry_wave();
            }
            return false;

        case KEYBIND_USER08:
            if (record->event.pressed) {
                audio_visualizer_toggle_underglow();
                uprintf("USER08 pressed -> toggle visualizer underglow\n");
            }
            return false;

        case KEYBIND_USER09:
            if (record->event.pressed) {
                audio_visualizer_next_mode();
                uprintf("USER09 pressed -> next visualizer palette\n");
            }
            return false;

        case KEYBIND_USER10:
            if (record->event.pressed) {
                audio_visualizer_next_render_mode();
                uprintf("USER10 pressed -> next render mode\n");
            }
            return false;
    }

    return true;
}