# Build Guide

This document describes how to build the firmware and integrate the keyboard source into a local `vial-qmk` checkout.

## Table of Contents

* [Verified Setup](#verified-setup)
* [Requirements](#requirements)
* [Clone this Repository](#clone-this-repository)
* [Clone vial-qmk](#clone-vial-qmk)
* [Link the Keyboard Source](#link-the-keyboard-source)
* [Build Firmware](#build-firmware)
* [Build and Flash Directly](#build-and-flash-directly)
* [Dynamic Lighting](#dynamic-lighting)
* [Audio Visualizer](#audio-visualizer)
* [Notes](#notes)
* [Related Documentation](#related-documentation)

## Verified Setup

Repository:

`id75rp_dynamic_lights`

Tested with:

* `vial-kb/vial-qmk`
* Standard `vial` branch
* RP2040 / UF2 bootloader
* RGB Matrix
* Vial

Verified:

* Builds successfully
* UF2 generated
* Vial detected
* QK_BOOT functional
* Matrix test successful
* Dynamic Lighting functional
* Audio Visualizer functional
* Startup Comet functional
* Linux host application functional
* Windows host application functional

---

## Requirements

This repository is intended to be built inside a working `vial-qmk` checkout.

The keyboard source is linked into the Vial tree via a symbolic link.

---

## Clone this Repository

```bash
git clone git@github.com:kolbenhans/id75rp_dynamic_lights.git
cd id75rp_dynamic_lights
```

---

## Clone vial-qmk

```bash
git clone https://github.com/vial-kb/vial-qmk ~/projects/vial-qmk
cd ~/projects/vial-qmk

git submodule update --init --recursive --depth 1
```

---

## Link the Keyboard Source

```bash
ln -s /path/to/id75rp_dynamic_lights/id75v3rp \
      ~/projects/vial-qmk/keyboards/ymdk/id75v3rp
```

Verify:

```bash
ls -ld ~/projects/vial-qmk/keyboards/ymdk/id75v3rp
```

---

## Build Firmware

```bash
cd ~/projects/vial-qmk

qmk clean
qmk compile -kb ymdk/id75v3rp -km vial
```

Output:

```text
.build/ymdk_id75v3rp_vial.uf2
```

---

## Build and Flash Directly

If the keyboard is already in bootloader mode, the firmware can be built and flashed in a single step:

```bash
cd ~/projects/vial-qmk

qmk clean
qmk flash -kb ymdk/id75v3rp -km vial
```

---

## Dynamic Lighting

Features:

* Keycode-aware RGB lighting
* Automatic color assignment
* Layer-specific color rules
* Home-row-mod highlighting
* Caps Lock and Num Lock indicators
* Automatic Vial keymap change detection
* Startup Comet animation
* Dedicated RGB Matrix effect

Activated through:

```text
USER06
```

---

## Audio Visualizer

Features:

* 12-band FFT spectrum analyzer
* Peak Hold
* Band Decay
* Automatic Gain Control (AGC)
* Multiple render modes
* Multiple color palettes
* Linux host application support
* Windows host application support

Activated through:

```text
USER07
```

Available render modes:

* Bars
* Dot
* Center
* KITT
* Waterdrop

Available color palettes:

* WinAmp
* Frost
* Fire
* Purple Rain

The host application performs:

* Audio capture
* FFT analysis
* Band calculation
* Peak calculation

The keyboard firmware performs:

* Effect rendering
* Palette handling
* RGB Matrix output

Communication between host and keyboard is performed via Raw HID.

For detailed information see:

* [Audio Visualizer](audio_visualizer.md)

---

## Notes

For compatibility with modern vial-qmk builds:

* `matrix_size` is defined in `keyboard.json`
* `processor` is defined in `keyboard.json`
* `bootloader` is defined in `keyboard.json`
* RGB Matrix LED count is defined in `keyboard.json`
* Legacy matrix definitions in `config.h` were cleaned up

The firmware uses dedicated RGB Matrix effects for both Dynamic Lighting and Audio Visualizer.

---

## Related Documentation

* [Audio Visualizer](audio_visualizer.md)
* [Host Setup](audio_visualizer_host_setup.md)
* [Flashing Guide](flashing.md)
* [Development Notes](development.md)
* [Known Limitations](known-limitations.md)
* [Porting Guide](porting_guide.md)
