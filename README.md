# QMK / Vial firmware source for the YMDK ID75 ortholinear keyboard (RP2040).

This fork adds dynamic keycode-based RGB lighting with automatic color assignment based on the current keymap configuration. Unlike traditional per-key RGB setups, key colors follow the assigned keycodes and update automatically when keys are remapped through Vial.

Features:
- Dynamic keycode-aware RGB lighting
- Automatic color mapping for common key functions
- Layer-specific lighting rules
- Mod-Tap and Layer-Tap support
- Caps Lock and Num Lock and home-row-mod indicators
- Configurable color palette
- Animated startup "Comet" effect ![Startup Comet](images/comet.gif))
- Optimized lighting cache for reduced CPU usage
- Full Vial compatibility

Designed for users who frequently customize their layouts and want visual feedback that adapts automatically without manually configuring individual LEDs.

## Dynamic keycode-aware RGB lighting

This fork includes a custom dynamic lighting engine. Key colors are assigned by keycode, not by physical LED position, so lighting automatically follows your Vial layout.

See [Dynamic Lighting Guide](docs/dynamic-lighting.md) for color and key rule customization.
