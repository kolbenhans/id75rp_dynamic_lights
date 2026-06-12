# Preconfigured Test Firmware

This UF2 file contains a preconfigured demonstration setup for testing the Dynamic Lighting feature without modifying the source code.

The firmware highlights a variety of commonly used keys and functions, making it easy to verify that Dynamic Lighting is working correctly and to understand how keycode-based color assignments behave across different layers.

## Included Color Assignments

### Global Keys (All Layers)

| Key       | Color       |
| --------- | ----------- |
| Enter     | Dark Orange |
| Space     | Dark Green  |
| Backspace | Purple      |
| Delete    | Pink        |

### Navigation Keys (Layers 1–4)

| Key         | Color  |
| ----------- | ------ |
| Left Arrow  | Red    |
| Down Arrow  | Blue   |
| Up Arrow    | Green  |
| Right Arrow | Yellow |

### Modifier Keys (Layers 1–4)

| Key               | Color        |
| ----------------- | ------------ |
| Shift             | Orange       |
| Ctrl              | Dark Orange  |
| GUI / Windows Key | Light Orange |
| Alt               | Light Red    |
| Tab               | Light Blue   |

### Browser Tab Shortcuts (Layer 1)

| Shortcut           | Color  |
| ------------------ | ------ |
| Ctrl + Tab         | Orange |
| Ctrl + Shift + Tab | Purple |

### Numpad Keys

| Key          | Color     |
| ------------ | --------- |
| Numbers 0–9  | Blue      |
| Numpad 5     | Orange    |
| Plus (+)     | Mint      |
| Multiply (*) | Mint      |
| Minus (-)    | Light Red |
| Divide (/)   | Light Red |

## Important

> [!IMPORTANT]
> Dynamic Lighting only works while the RGB Matrix effect **Custom Lighting** is selected and active.

In Vial:

```text
Lighting → Effects → Custom Lighting
```

After flashing the firmware, simply enable **Custom Lighting** to see the preconfigured lighting profile in action.

## Purpose

This firmware is intended as:

* A quick demonstration of Dynamic Lighting
* A reference configuration for custom color assignments
* A simple way to verify that Dynamic Lighting is working correctly after flashing

## Related Documentation

* `README.md`
* `docs/build.md`
* `docs/flashing.md`
* `docs/development.md`
