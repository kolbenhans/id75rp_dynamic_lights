# Audio Visualizer Host Setup

This guide describes how to set up and run the Audio Visualizer host application on supported operating systems.

## Table of Contents

* [Overview](#overview)
* [Requirements](#requirements)
* [Linux Setup](#linux-setup)
* [Windows Setup](#windows-setup)
* [Device Selection](#device-selection)
* [Troubleshooting](#troubleshooting)
* [Supported Platforms](#supported-platforms)

---

## Overview

The Audio Visualizer consists of two components:

### Host Application

The host application performs:

* Audio capture
* FFT analysis
* Frequency band calculation
* Peak calculation
* Raw HID communication

### Keyboard Firmware

The firmware performs:

* Visualizer rendering
* Palette handling
* LED output
* Optional split synchronization

Communication between host and keyboard is performed via Raw HID.

---

## Requirements

### Common Requirements

* Python 3
* NumPy
* hidapi

Install dependencies:

```bash
python -m pip install numpy hidapi
```

---

## Linux Setup

### Supported Audio Systems

The following audio systems are supported:

* PipeWire (recommended)
* PulseAudio

### Verify Audio Tools

Check that the required audio tools are available:

```bash
which pactl
which parec
```

### Install Dependencies

```bash
python -m pip install numpy hidapi
```

### Run

```bash
python sound_grabber.py
```

The application automatically detects the default audio output device and captures its monitor stream.

---

## Windows Setup

### Additional Requirement

Windows audio capture requires:

* soundcard

### Install Dependencies

```powershell
py -m pip install --upgrade pip
py -m pip install numpy hidapi soundcard
```

### Run

```powershell
py sound_grabber.py
```

The Windows implementation uses WASAPI loopback capture via the Python `soundcard` package.

Audio is captured directly from the default playback device.

---

## Device Selection

At startup the application automatically searches for compatible Raw HID devices.

If multiple compatible keyboards are connected, a selection menu is displayed.

Example:

```text
Compatible Raw HID devices found:

[0] PandaKB Sofle
[1] YMDK ID75

Select device [0]:
```

Press Enter to select the default device.

This allows a single host application to work with multiple supported keyboards without modifying Vendor IDs or Product IDs in the source code.

---

## Troubleshooting

### No Compatible Device Found

Verify that:

* The keyboard firmware includes Raw HID support
* The keyboard is connected and recognized by the operating system
* The keyboard is running firmware that includes the Audio Visualizer
* No other application is exclusively using the Raw HID interface

### Linux: Audio Capture Not Working

Verify that the required audio tools are available:

```bash
which pactl
which parec
```

Check audio server status:

```bash
pactl info
```

Ensure audio is actively playing through the selected output device.

### Windows: Audio Capture Not Working

Ensure that:

* Audio is currently playing
* The correct playback device is selected as the default device
* The Python dependency `soundcard` is installed

Verify installation:

```powershell
py -m pip show soundcard
```

### Visualizer Not Reacting

Verify that:

* The Audio Visualizer RGB Matrix effect is active
* The host application is running
* The correct keyboard was selected from the device list

---

## Supported Platforms

Currently tested:

* Linux (PipeWire)
* Linux (PulseAudio)
* Windows 10
* Windows 11

Additional platforms may work but are currently untested.

---

## Related Documentation

* [Audio Visualizer](audio_visualizer.md)
* [Build Guide](build.md)
* [Flashing Guide](flashing.md)
* [Porting Guide](porting_guide.md)
