# QMK / Vial Firmware for the YMDK ID75 (RP2040)

This project extends the YMDK ID75 firmware with two custom RGB Matrix effects:

* **Dynamic Lighting** – keycode-aware RGB lighting that automatically adapts to your Vial layout.
* **Audio Visualizer** – real-time audio spectrum visualization via Raw HID and a host application.

Unlike traditional per-key RGB setups, Dynamic Lighting assigns colors based on key function rather than physical LED position. Colors automatically follow key remaps made in Vial without requiring manual LED configuration.

## Demo Videos

### Dynamic Lighting

[![Dynamic Lighting Demo](https://img.youtube.com/vi/74F3iRf9jFE/maxresdefault.jpg)](https://www.youtube.com/watch?v=74F3iRf9jFE)

### Audio Visualizer

[![Audio Visualizer Demo](https://img.youtube.com/vi/4OLN6erbn6I/maxresdefault.jpg)](https://youtu.be/4OLN6erbn6I)

## Features

### Dynamic Lighting

* Keycode-aware RGB lighting
* Automatic color mapping for common key functions
* Layer-specific lighting rules
* Mod-Tap and Layer-Tap support
* Home-row mod highlighting
* Caps Lock and Num Lock indicators
* Configurable color palette
* Animated Startup Comet effect
* Automatic Vial keymap change detection
* Optimized lighting cache
* Full Vial compatibility

### Audio Visualizer

* 12-band FFT spectrum analyzer
* Multiple render modes

  * Bars
  * Dot
  * Center
  * KITT
  * Waterdrop
* Multiple color palettes

  * WinAmp
  * Frost
  * Fire
  * Purple Rain
* Peak Hold
* Band Decay
* Automatic Gain Control (AGC)
* Raw HID communication
* Linux support (PipeWire / PulseAudio)
* Windows support (WASAPI Loopback)
* Multi-keyboard device selection
* Support for split and non-split keyboard implementations

Designed for users who frequently customize their layouts and want lighting effects that automatically adapt to their keymap while remaining fully configurable through Vial.

## Quick Start

1. Flash the firmware
2. Open Vial
3. Select **Custom Lighting** or **Audio Visualizer**
4. Enjoy

### Dynamic Lighting

> [!IMPORTANT]
> Dynamic Lighting only runs while the RGB Matrix effect **Custom Lighting** is active.
>
> In Vial:
>
> `Lighting → Effects → Custom Lighting`

### Audio Visualizer

> [!IMPORTANT]
> Audio Visualizer only runs while the RGB Matrix effect **Audio Visualizer** is active.
>
> In Vial:
>
> `Lighting → Effects → Audio Visualizer`

The Audio Visualizer additionally requires the host application described in:

* `docs/audio_visualizer_host_setup.md`

## Documentation

* `docs/build.md`
* `docs/flashing.md`
* `docs/development.md`
* `docs/audio_visualizer.md`
* `docs/audio_visualizer_host_setup.md`
* `docs/known-limitations.md`
* `docs/porting_guide.md`

See the documentation directory for build instructions, flashing procedures, host setup, development notes, and porting information.
