# Audio Visualizer Host

Host application for the PandaKB Sofle Audio Visualizer.

The host application performs:

* Audio capture
* FFT processing
* Band calculation
* Peak calculation
* Raw HID communication

All visual rendering is performed by the keyboard firmware.

---

## Requirements

* Python 3.11+
* hidapi
* numpy

Audio backend:

### Linux

Requires:

* PipeWire
  or
* PulseAudio

### Windows

Uses:

* WASAPI Loopback
* soundcard

---

## Installation

See:

* ../../docs/audio_visualizer_host_setup.md

---

## Usage

Linux:

```bash
python sound_grabber.py
```

Windows:

```powershell
py sound_grabber.py
```

The application automatically searches for a compatible PandaKB Sofle Raw HID device.

---

## Keyboard Controls

| Key    | Function         |
| ------ | ---------------- |
| USER06 | Custom Lighting  |
| USER07 | Audio Visualizer |
| USER08 | Underglow        |
| USER09 | Palette          |
| USER10 | Render Mode      |

---

## Related Documentation

* ../../docs/audio_visualizer.md
* ../../docs/audio_visualizer_host_setup.md
* ../../docs/porting_guide.md

## Developer Tools

### send_visualizer_test.py

Sends synthetic visualizer data directly to the keyboard.

Useful for:

- HID testing
- Visualizer development
- Render mode testing

### audio_level_test.py

Displays audio levels and FFT data without requiring a keyboard.

Useful for:

- Audio backend testing
- FFT tuning
- AGC tuning
