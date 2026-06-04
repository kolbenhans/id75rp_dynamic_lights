# Dynamic Lighting Guide

This fork uses dynamic keycode-aware RGB lighting.

Instead of assigning colors to fixed LED positions, colors are assigned to keycodes. This means that when you remap a key in Vial, the color follows the assigned function automatically.

## Enabling Dynamic Lighting

> [!IMPORTANT]
> The Dynamic Lighting engine only runs while the RGB Matrix effect
> **Direct Control** is active.

To enable it in Vial:

1. Open Vial
2. Navigate to **Lighting**
3. Select **Direct Control** as the active RGB Matrix effect

All other RGB effects (Rainbow, Splash, Solid Color, etc.) will override the Dynamic Lighting engine.

## Adding a new color

Colors are defined in `keymaps/vial/dynamic_lights.c`.

First, add a new color ID to the color enum:

```c
enum {
    CLR_OFF = 0,
    CLR_RED,
    CLR_YELLOW,
    CLR_BLUE,
    CLR_MY_CUSTOM_COLOR
};
```

Then add the RGB value to `color_palette[]`:

```c
[CLR_MY_CUSTOM_COLOR] = { 128, 64, 255 },
```

## Assigning a color to a keycode

Key colors are configured in `key_color_rules[]`:

```c
{ KC_ENT, CLR_RED, L_ALL },
```

This means:

- `KC_ENT` = Enter key
- `CLR_RED` = assigned color
- `L_ALL` = active on all layers

## Layer selection

Layer helpers are provided to make the configuration easier to read:

```c
L_ALL
L(1)
L(2) | L(4)
L_RANGE(1, 3)
L_RANGE(1, 3) | L(5)
```

Examples:

```c
{ KC_ESC,  CLR_CYAN,   L_ALL },
{ KC_TAB,  CLR_BLUE,   L(1) },
{ KC_LEFT, CLR_RED,    L_RANGE(1, 4) },
{ KC_F1,   CLR_GREEN,  L_RANGE(1, 3) | L(5) },
```

## Common examples

### Navigation keys on layers 1 to 4

```c
{ KC_LEFT, CLR_RED,    L_RANGE(1, 4) },
{ KC_DOWN, CLR_BLUE,   L_RANGE(1, 4) },
{ KC_UP,   CLR_GREEN,  L_RANGE(1, 4) },
{ KC_RGHT, CLR_YELLOW, L_RANGE(1, 4) },
```

### Browser tab shortcuts on layer 1

```c
{ C(S(KC_TAB)), CLR_PURPLE, L(1) },
{ C(KC_TAB),    CLR_ORANGE, L(1) },
```

### Numpad keys on all layers

```c
{ KC_P0,   CLR_BLUE,  L_ALL },
{ KC_P1,   CLR_BLUE,  L_ALL },
{ KC_PPLS, CLR_GREEN, L_ALL },
{ KC_PMNS, CLR_RED,   L_ALL },
```

## Notes

After changing the source code, the firmware must be rebuilt and flashed again.

Changes made in Vial to the keymap are detected automatically by the dynamic lighting engine, but changes to colors or lighting rules require recompiling the firmware.


