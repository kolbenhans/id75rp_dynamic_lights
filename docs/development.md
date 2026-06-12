# Development Notes

## Table of Contents

* [Project Structure](#project-structure)
* [Dynamic Lighting](#dynamic-lighting)
* [Audio Visualizer](#audio-visualizer)
* [RGB Matrix Effects](#rgb-matrix-effects)
* [EEPROM Behavior](#eeprom-behavior)
* [Startup Animation](#startup-animation)
* [Recovery Firmware](#recovery-firmware)
* [Related Documentation](#related-documentation)

---

## Project Structure

Main implementation files:

```text
id75v3rp/keymaps/vial/

├── dynamic_lights.c
├── dynamic_lights.h
├── audio_visualizer.c
├── audio_visualizer.h
├── rgb_matrix_user.inc
├── keymap.c
└── rules.mk
```

---

## Dynamic Lighting

Dynamic Lighting is implemented as a dedicated RGB Matrix effect.

Activated through:

```text
USER06
```

Features:

* Keycode-aware RGB lighting
* Layer-aware color rules
* Home-row-mod highlighting
* Caps Lock and Num Lock indicators
* Automatic Vial keymap change detection
* Startup Comet animation

The active Vial keymap is used as the color source.

Changes made through Vial are detected automatically and reflected without requiring firmware modifications.

---

## Audio Visualizer

The Audio Visualizer is implemented as a dedicated RGB Matrix effect.

Activated through:

```text
USER07
```

Host responsibilities:

* Audio capture
* FFT processing
* Frequency band generation
* Peak calculation

Keyboard responsibilities:

* Effect rendering
* Palette handling
* RGB Matrix output

Communication is performed through Raw HID.

See:

* [Audio Visualizer](audio_visualizer.md)
* [Host Setup](audio_visualizer_host_setup.md)

---

## RGB Matrix Effects

The firmware currently provides two custom RGB Matrix effects:

### Custom Lighting

Activated through:

```text
USER06
```

Provides automatic keycode-based color assignment and layout-aware lighting.

### Audio Visualizer

Activated through:

```text
USER07
```

Provides real-time audio spectrum visualization driven by the host application.

Both effects are implemented as native RGB Matrix effects and can be selected through Vial.

---

## EEPROM Behavior

Vial stores:

* Keymaps
* Lighting settings
* User configuration

Changes made through Vial are preserved across reboots.

The Dynamic Lighting engine automatically detects keymap changes and refreshes its internal cache when necessary.

---

## Startup Animation

A startup Comet animation is displayed when entering the Dynamic Lighting effect.

The animation also runs after power-up when Dynamic Lighting is configured as the active effect.

The animation is implemented in:

```text
dynamic_lights.c
```

---

## Recovery Firmware

Keep at least one known-good UF2 firmware available before making major source changes.

A tested firmware image can be used to recover from failed experiments or configuration mistakes.

Recommended locations:

```text
firmware/reference/
firmware/working/
```

---

## Related Documentation

* [Build Guide](build.md)
* [Audio Visualizer](audio_visualizer.md)
* [Host Setup](audio_visualizer_host_setup.md)
* [Known Limitations](known-limitations.md)
* [Porting Guide](porting_guide.md)
