# Known Limitations

## Table of Contents

* [Current Status](#current-status)
* [Power Consumption](#power-consumption)
* [Audio Visualizer Requirements](#audio-visualizer-requirements)
* [Wireless Keyboards](#wireless-keyboards)
* [Related Documentation](#related-documentation)

---

## Current Status

At the time of writing, no known functional limitations have been identified.

The firmware has been tested successfully with:

* Dynamic Lighting
* Audio Visualizer
* Vial
* RP2040 UF2 Bootloader
* Linux host application
* Windows host application

---

## Power Consumption

The Audio Visualizer and Dynamic Lighting systems can significantly increase RGB power consumption.

At high brightness levels:

* USB power limits may be reached on some systems.
* Stability may depend on the available power supply.
* Maximum brightness may vary depending on the USB port and cable quality.

The firmware itself has not shown stability issues related to RGB rendering when sufficient power is available.

---

## Audio Visualizer Requirements

The Audio Visualizer requires:

* A compatible host application
* Raw HID communication
* Active audio playback

Without the host application running, the Audio Visualizer effect remains active but receives no audio data.

This is expected behavior and not considered a firmware defect.

---

## Wireless Keyboards

Wireless keyboards are currently not a primary target platform.

Reasons:

* Increased power consumption
* Continuous HID communication
* Continuous RGB activity

The firmware may work on wireless platforms, but wired keyboards are strongly recommended.

---

## Related Documentation

* [Build Guide](build.md)
* [Flashing Guide](flashing.md)
* [Audio Visualizer](audio_visualizer.md)
* [Host Setup](audio_visualizer_host_setup.md)
* [Porting Guide](porting_guide.md)
