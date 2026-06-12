/* SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

#define VIAL_KEYBOARD_UID {0x81, 0x3E, 0x5D, 0xE8, 0x4E, 0x57, 0x75, 0xB6}

#define VIAL_UNLOCK_COMBO_ROWS {0, 1}
#define VIAL_UNLOCK_COMBO_COLS {0, 1}

#define DYNAMIC_KEYMAP_MACRO_COUNT 32
#define VIAL_COMBO_ENTRIES 32               // number of combos used, each entry take 10 byte in EEPROM
#define VIAL_TAP_DANCE_ENTRIES 48           // number of tap dances used, each entry take 10 byte in EEPROM
#define VIAL_KEY_OVERRIDE_ENTRIES 8         // number of key override used, each entry take 10 byte in EEPROM
#define RGB_MATRIX_FRAMEBUFFER_EFFECTS
#define RGB_MATRIX_KEYPRESSES

#define RGB_MATRIX_TIMEOUT 1800000           // turn off keyboard lightning after 5 minutes witout usage

#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_CUSTOM_custom_lighting
