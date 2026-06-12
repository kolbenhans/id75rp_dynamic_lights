# Flashing Guide

## Table of Contents

* [Build Firmware](#build-firmware)
* [Flash Firmware](#flash-firmware)
* [Entering Bootloader Mode](#entering-bootloader-mode)
* [Recovery Recommendations](#recovery-recommendations)
* [Verification Checklist](#verification-checklist)
* [Related Documentation](#related-documentation)

---

## Build Firmware

Build the firmware inside your `vial-qmk` working tree:

```bash
qmk clean
qmk compile -kb ymdk/id75v3rp -km vial
```

Generated firmware:

```text
.build/ymdk_id75v3rp_vial.uf2
```

---

## Flash Firmware

Copy:

```text
.build/ymdk_id75v3rp_vial.uf2
```

to the keyboard UF2 drive.

The RP2040 bootloader will automatically flash the firmware and reboot the keyboard.

---

## Entering Bootloader Mode

### Option 1: Hardware Reset

Double-press the reset button on the PCB.

The keyboard should enter the UF2 bootloader and appear as a removable USB storage device.

### Option 2: QK_BOOT

This firmware supports the standard QMK `QK_BOOT` keycode.

If a key is assigned to `QK_BOOT` through Vial, pressing that key immediately reboots the keyboard into the UF2 bootloader.

---

## Recovery Recommendations

Before flashing experimental firmware:

* Keep a known-good firmware backup available.
* Ensure that a `QK_BOOT` key is assigned.
* Verify that Vial access is working.
* Verify that the keyboard can successfully enter bootloader mode.

Known-good recovery firmware can be stored in:

```text
firmware/working/
```

When making large firmware changes, verify that:

* Bootloader access remains functional.
* Vial access remains functional.
* RGB Matrix effects remain functional.
* Dynamic Lighting remains functional.
* Audio Visualizer remains functional.

---

## Verification Checklist

After flashing new firmware:

### Basic Functionality

* Keyboard is detected by the operating system
* All keys function correctly
* Vial detects the keyboard
* Layer switching works as expected

### Dynamic Lighting

* USER06 activates Dynamic Lighting
* Startup Comet animation runs correctly
* Layer-dependent colors update correctly
* Caps Lock and Num Lock indicators work correctly

### Audio Visualizer

* USER07 activates Audio Visualizer
* Host application connects successfully
* Visualizer reacts to audio playback
* Palette switching works correctly
* Render mode switching works correctly

---

## Related Documentation

* [Build Guide](build.md)
* [Development Notes](development.md)
* [Known Limitations](known-limitations.md)
* [Audio Visualizer](audio_visualizer.md)
* [Host Setup](audio_visualizer_host_setup.md)
