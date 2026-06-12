import argparse
from pathlib import Path

import numpy as np
from PIL import Image


PALETTE_SIZE = 5


def load_image(path):
    return Image.open(path).convert("RGB")


def image_to_pixels(image, size=120):
    image = image.copy()
    image.thumbnail((size, size))

    pixels = np.asarray(image, dtype=np.uint8).reshape(-1, 3)
    return pixels


def filter_pixels(pixels):
    pixels_f = pixels.astype(np.float32)

    brightness = pixels_f.mean(axis=1)
    saturation = pixels_f.max(axis=1) - pixels_f.min(axis=1)

    mask = (
        (brightness > 30) &
        (brightness < 210) &
        (saturation > 20)
    )

    filtered = pixels[mask]

    if filtered.size == 0:
        return pixels

    return filtered


def quantize_pixels(pixels, step=32):
    return (pixels // step) * step


def extract_palette(pixels, count=PALETTE_SIZE):
    quantized = quantize_pixels(pixels, step=16)

    colors, counts = np.unique(
        quantized,
        axis=0,
        return_counts=True,
    )

    colors_f = colors.astype(np.float32)

    r = colors_f[:, 0]
    g = colors_f[:, 1]
    b = colors_f[:, 2]

    brightness = colors_f.mean(axis=1)
    saturation = colors_f.max(axis=1) - colors_f.min(axis=1)

    valid = (
        (brightness > 25) &
        (brightness < 245) &
        (saturation > 35)
    )

    colors = colors[valid]
    counts = counts[valid]
    colors_f = colors.astype(np.float32)

    r = colors_f[:, 0]
    g = colors_f[:, 1]
    b = colors_f[:, 2]

    brightness = colors_f.mean(axis=1)
    saturation = colors_f.max(axis=1) - colors_f.min(axis=1)

    families = [
        ("yellow", (r > 160) & (g > 140) & (b < 100)),
        ("pink",   (r > 160) & (b > 80)  & (g < 120)),
        ("cyan",   (g > 140) & (b > 120) & (r < 140)),
        ("dark", (brightness < 90) & (saturation > 20) & (b > 30)),
        ("accent", (saturation > 80) & ~((r > 140) & (g > 140) & (b < 120))),
    ]

    selected = []

    for _, mask in families:
        if len(selected) >= count:
            break

        idxs = np.where(mask)[0]
        if len(idxs) == 0:
            continue

        local_score = counts[idxs] * ((saturation[idxs] + 1) ** 1.3)
        best = idxs[np.argmax(local_score)]
        color = colors[best]

        if selected:
            distances = [
                np.linalg.norm(color.astype(np.int16) - c.astype(np.int16))
                for c in selected
            ]
            if min(distances) < 55:
                continue

        selected.append(color)

    score = counts * ((saturation + 1) ** 1.3)
    order = np.argsort(score)[::-1]

    for idx in order:
        if len(selected) >= count:
            break

        color = colors[idx]

        distances = [
            np.linalg.norm(color.astype(np.int16) - c.astype(np.int16))
            for c in selected
        ]

        if min(distances) > 70:
            selected.append(color)

    while len(selected) < count:
        selected.append(np.array([80, 80, 80], dtype=np.uint8))

    return np.array(selected[:count], dtype=np.uint8)

def print_palette(palette):
    print("Detected palette:\n")

    for i, color in enumerate(palette):
        r, g, b = [int(v) for v in color]
        print(f"{i}: ({r:3}, {g:3}, {b:3})  #{r:02X}{g:02X}{b:02X}")


def main():
    parser = argparse.ArgumentParser(
        description="Extract a visualizer palette from an image.",
    )

    parser.add_argument(
        "image",
        type=Path,
        help="Path to an image file",
    )

    args = parser.parse_args()

    image = load_image(args.image)
    pixels = image_to_pixels(image)
    pixels = filter_pixels(pixels)

    palette = extract_palette(pixels)
    print_palette(palette)


if __name__ == "__main__":
    main()
