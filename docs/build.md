# Build Guide

This document describes how to build the firmware and integrate the keyboard source into a local `vial-qmk` checkout.

## Table of Contents

* [Platform](#platform)
* [Verified Setup](#verified-setup)
* [Directory Layout](#directory-layout)
* [Step 1 — Clone vial-qmk](#step-1--clone-vial-qmk)
* [Step 2 — Clone this repository](#step-2--clone-this-repository)
* [Step 3 — Link the keyboard source](#step-3--link-the-keyboard-source)
* [Step 4 — Build firmware](#step-4--build-firmware)
* [Dynamic Lighting](#dynamic-lighting)
* [Audio Visualizer](#audio-visualizer)
* [Notes](#notes)
* [Related Documentation](#related-documentation)

---

## Platform

All shell commands in this guide use bash syntax.

| Platform | Recommended environment |
|---|---|
| Linux | Standard terminal |
| macOS | Standard terminal |
| Windows | [QMK MSYS](https://msys.qmk.fm/) or WSL (Windows Subsystem for Linux) |

In **QMK MSYS** and **WSL**, all commands in this guide work as written.  
Native Windows Command Prompt and PowerShell alternatives are noted where they differ.

---

## Verified Setup

Repository: `id75rp_dynamic_lights`

Tested with:

* `vial-kb/vial-qmk`, standard `vial` branch
* RP2040 / UF2 bootloader
* RGB Matrix, Vial

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

## Directory Layout

This guide uses the following structure. You can choose any base directory — just replace `~/projects` with your preferred path consistently throughout.

```
~/projects/
├── vial-qmk/               ← vial-qmk checkout
└── id75rp_dynamic_lights/  ← this repository
```

---

## Step 1 — Clone vial-qmk

```bash
git clone https://github.com/vial-kb/vial-qmk ~/projects/vial-qmk
cd ~/projects/vial-qmk
git submodule update --init --recursive --depth 1
```

---

## Step 2 — Clone this repository

```bash
git clone git@github.com:kolbenhans/id75rp_dynamic_lights.git ~/projects/id75rp_dynamic_lights
```

---

## Step 3 — Link the keyboard source into vial-qmk

**Linux / macOS / QMK MSYS / WSL:**

```bash
ln -s ~/projects/id75rp_dynamic_lights/id75v3rp \
      ~/projects/vial-qmk/keyboards/ymdk/id75v3rp
```

**Windows (Command Prompt, without QMK MSYS):**

```cmd
mklink /D "%USERPROFILE%\projects\vial-qmk\keyboards\ymdk\id75v3rp" ^
          "%USERPROFILE%\projects\id75rp_dynamic_lights\id75v3rp"
```

> [!NOTE]
> `mklink /D` requires administrator privileges on Windows.  
> Using QMK MSYS avoids this — `ln -s` works there without elevation.

Verify the link:

```bash
ls -ld ~/projects/vial-qmk/keyboards/ymdk/id75v3rp
```

---

## Step 4 — Build firmware

```bash
cd ~/projects/vial-qmk
qmk clean
qmk compile -kb ymdk/id75v3rp -km vial
```

Output:

```
.build/ymdk_id75v3rp_vial.uf2
```

### Build and flash directly

If the keyboard is already in bootloader mode:

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

```
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

```
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
