# ID75 with RP2040 MPU

I've received from YDMK a zip file with code to compile a VIAL QMK firmware. It didn't compile without making some small changes.

Since there seem to exist several versions of this keyboard (with original MPU, with STM32F103 and with RP2040 at least), I've renamed the keyboard name to "ID75v3rp".

## Installation instructions

This firmware compile source folder (with all included files) for the YMDK RP2040 ID75 variant needs to be copied into an up-to-date VIAL QMK source tree under the keyboard/ymdk folder.

Then, follow instructions given for the YMDK ID75 keyboard at /keyboard/ymdk/id75.

## Compile the firmware

```s
cd [...]/vial-qmk
make git-submodule
make ymdk/id75v3rp:vial
python3 util/vial_generate_keyboard_uid.py
```

## Some measurements

Keyboard USB current with all leds on at full brightness is 0,85A. Keyboard USB current with all leds off: 0,06A  (Scroll Lock not lit in this scenario)

## Pending

- The provided code had some custom code for RGB lighting for the CAPS LOCK indicator. I didn't dedicate time to fix this, so if somebody wants to dig into it, the caps lock code in keymaps/vial/keymap.c file needs to be uncommented.

## Changes
- info.json renamed to keyboard.json to avoid this Warning: ymdk/id75v3rp: Build marker "keyboard.json" not found.
- Moved LAYOUT macro definition from id75*.h to info.json/keyboard.json to remove compilation Error: ymdk/id75v3rp: LAYOUT_ortho_5x15: Layout macro should not be defined within ".h" files. [See](https://github.com/qmk/qmk_firmware/issues/18486)

## Forked by Kolbenhans

Forked from:
- https://github.com/mgsg/id75rp

### keyboard.json
- Changed `manufacturer` to `YMDK`
- Changed `maintainer` to `Kolbenhans`

### keymaps/vial
#### dynamic lighting
- Added `dynamic_lights.c`
- Added `dynamic_lights.h`
- Added dynamic keycode-aware RGB lighting that automatically adapts to the current key layout

#### keymap.c
- Removed the original Caps Lock indicator
- Added dynamic lighting integration

#### rules.mk
- Added dynamic lighting source files

### Layer configuration
#### keymaps/vial/keymap.c
- Added an additional layer (Layer 4)

#### config.h
- Changed `DYNAMIC_KEYMAP_LAYER_COUNT` from `4` to `5`

#### keymaps/vial/config.h
- Changed `DYNAMIC_KEYMAP_LAYER_COUNT` from `4` to `5`

### RGB configuration
#### keymaps/vial/config.h
- Added:
  ```c
  #define RGB_MATRIX_TIMEOUT 300000
  ```
- Automatically turns off keyboard lighting after 5 minutes of inactivity
