SRC += dynamic_lights.c
SRC += audio_visualizer.c
BOOTLOADER = rp2040

VIA_ENABLE = yes
VIAL_ENABLE = yes
LTO_ENABLE = yes # reduce firmware size
VIAL_INSECURE = yes
VIALRGB_ENABLE = yes

RAW_ENABLE = yes
RGB_MATRIX_CUSTOM_USER = yes

# Debug
CONSOLE_ENABLE = no