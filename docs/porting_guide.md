# Porting Guide

This document describes the requirements for adapting the Audio Visualizer to other QMK keyboards.

## Table of Contents

* [Reference Implementations](#reference-implementations)
* [Requirements](#requirements)
* [Required Features](#required-features)
* [LED Geometry](#led-geometry)
* [Split Keyboards](#split-keyboards)
* [Keyboard-Specific Configuration](#keyboard-specific-configuration)
* [Porting Checklist](#porting-checklist)
* [Power Consumption](#power-consumption)
* [Related Documentation](#related-documentation)

---

## Reference Implementations

Current reference implementations:

* PandaKB Sofle (split keyboard)
* YMDK ID75 (non-split keyboard)

These implementations demonstrate both supported architectures:

* Split keyboards
* Non-split keyboards

---

## Requirements

The keyboard must support:

* QMK RGB Matrix
* Raw HID
* Custom RGB Matrix effects

Recommended:

* Vial support

For split keyboards:

* QMK transaction support

---

## Required Features

### RGB Matrix

The visualizer uses the QMK RGB Matrix subsystem.

Required:

```make
RGB_MATRIX_ENABLE = yes
```

### Raw HID

Communication between host application and keyboard uses Raw HID.

Required:

```make
RAW_ENABLE = yes
```

### Custom RGB Matrix Effects

The visualizer is implemented as a native RGB Matrix effect.

Required:

```make
RGB_MATRIX_CUSTOM_USER = yes
```

---

## LED Geometry

The visualizer uses the LED coordinates provided by:

```c
g_led_config
```

Ensure that all LEDs have valid X/Y coordinates.

Most visualizer modes use LED geometry directly and do not require keyboard-specific LED mappings.

The following render modes depend heavily on geometry information:

* Bars
* Dot
* Center
* KITT
* Waterdrop

Accurate LED coordinates improve visual quality.

---

## Split Keyboards

Split keyboards require additional synchronization logic.

Typical requirements:

* QMK transactions
* Master/slave communication
* Shared visualizer state

The reference Sofle implementation uses transaction-based synchronization for:

* Band values
* Peak values
* Visualizer state

Non-split keyboards do not require any synchronization layer.

---

## Keyboard-Specific Configuration

The following components may require adjustment when porting:

### Startup Animation

The startup Comet animation uses a keyboard-specific LED traversal order.

Review:

```text
startup_snake_led()
```

### Waterdrop Effect

The Waterdrop effect may require tuning for unusual keyboard layouts.

Review:

```text
render_visualizer_waterdrop_mode()
```

### Underglow

Bass-reactive underglow may require keyboard-specific LED selection.

Review:

```text
render_visualizer_underglow()
```

### LED Count

Verify:

```c
RGB_MATRIX_LED_COUNT
```

and ensure all internal loops operate on the correct LED range.

---

## Porting Checklist

Before testing:

* Raw HID communication functional
* RGB Matrix functional
* Custom RGB Matrix effects enabled
* LED coordinates verified
* Visualizer compiles successfully

After flashing:

* Audio data received correctly
* Bars mode renders correctly
* Dot mode renders correctly
* Center mode renders correctly
* KITT mode renders correctly
* Waterdrop mode renders correctly
* Palette switching functional
* Render mode switching functional

For split keyboards:

* Synchronization functional
* Both halves render identically

---

## Power Consumption

The Audio Visualizer can significantly increase RGB power consumption.

Recommendations:

* Use wired keyboards
* Verify USB power availability
* Reduce brightness if stability issues occur

For split keyboards:

* Dual USB connections may help on power-constrained systems

Wireless keyboards are generally not recommended.

---

## Related Documentation

* [Audio Visualizer](audio_visualizer.md)
* [Host Setup](audio_visualizer_host_setup.md)
* [Build Guide](build.md)
* [Development Notes](development.md)
