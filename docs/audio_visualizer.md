# Audio Visualizer

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Color Palettes](#color-palettes)
- [Controls](#controls)
- [Host Application](#host-application)
- [Architecture](#architecture)
- [Split Keyboard Support](#split-keyboard-support)
- [Known Limitations](#known-limitations)
- [Related Documentation](#related-documentation)

## Overview

The audio visualizer runs directly on the keyboard.

The host application performs:

- Audio capture
- FFT analysis
- Band calculation
- Peak calculation

The keyboard firmware performs:

- Effect rendering
- Palette handling
- Split synchronization
- LED output

Communication is performed via Raw HID.

---

## Features

### Audio Processing

- 12 frequency bands
- Peak hold
- Band decay
- Automatic Gain Control (AGC)

### Render Modes

#### Bars

Vertical spectrum analyzer.

#### Dot

Peak indicator rendered per band.

#### Center

Spectrum rendered from the center outward.

#### KITT

Symmetrical center-out animation.

#### Waterdrop

Bass-triggered expanding wave effect using the lowest frequency bands.

---

## Color Palettes

Available palettes:

- WinAmp
- Frost
- Fire
- Purple Rain

USER09 cycles through palettes.

---

## Controls

| Key    | Function                  |
|--------|---------------------------|
| USER06 | Activate Custom Lighting  |
| USER07 | Activate Audio Visualizer |
| USER08 | Toggle Underglow          |
| USER09 | Next Palette              |
| USER10 | Next Render Mode          |

---

## Host Application

Supported platforms:

- Linux (PipeWire)
- Linux (PulseAudio)
- Windows (WASAPI Loopback)

See:

- [Host Setup](audio_visualizer_host_setup.md)

---

## Architecture

Host:

Audio → FFT → Bands → Raw HID

Keyboard:

Raw HID → Visualizer Engine → RGB Matrix

---

## Known Limitations

- High brightness levels may increase power consumption significantly
- Wireless keyboards are not recommended

---

## Related Documentation

- [Host Setup](audio_visualizer_host_setup.md)
- [Build Guide](build.md)
- [Flashing Guide](flashing.md)
- [Porting Guide](porting_guide.md)
