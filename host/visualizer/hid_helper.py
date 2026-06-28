import hid
import re

USAGE_PAGE = 65376
USAGE = 97

def device_search_text(device):
    parts = [
        device.get("manufacturer_string") or "",
        device.get("product_string") or "",
        str(device.get("path") or ""),
    ]

    return " ".join(parts)

def find_raw_hid_devices():
    devices = []

    for d in hid.enumerate():
        if d.get("usage_page") == USAGE_PAGE and d.get("usage") == USAGE:
            devices.append(d)

    return devices

def select_device(devices, selector=None):
    if not devices:
        raise RuntimeError("No compatible Raw HID device found")

    if selector:
        if selector.isdigit():
            index = int(selector)

            if 0 <= index < len(devices):
                return devices[index]

            raise RuntimeError(f"Raw HID device index out of range: {selector}")

    if selector:
        pattern = re.compile(selector, re.IGNORECASE)

        matches = [
            d for d in devices
            if pattern.search(device_search_text(d))
        ]

        if len(matches) == 1:
            return matches[0]

        if len(matches) > 1:
            print(f"Multiple Raw HID devices match selector '{selector}':\n")
            devices = matches
        else:
            raise RuntimeError(f"No Raw HID device matches selector: {selector}")

    if len(devices) == 1:
        return devices[0]

    print("Compatible Raw HID devices found:\n")

    for i, d in enumerate(devices):
        manufacturer = d.get("manufacturer_string") or "Unknown manufacturer"
        product = d.get("product_string") or "Unknown product"
        path = d.get("path")

        print(f"[{i}] {manufacturer} {product} path={path}")

    while True:
        choice = input("\nSelect device [0]: ").strip()

        if choice == "":
            return devices[0]

        try:
            index = int(choice)
            if 0 <= index < len(devices):
                return devices[index]
        except ValueError:
            pass

        print("Invalid selection")


def open_raw_hid(selector=None):
    device_info = select_device(find_raw_hid_devices(), selector=selector)
    path = device_info["path"]

    if hasattr(hid, "device"):
        dev = hid.device()
        dev.open_path(path)
    else:
        dev = hid.Device(path=path)

    manufacturer = device_info.get("manufacturer_string") or "Unknown manufacturer"
    product = device_info.get("product_string") or "Unknown product"

    print(f"Opened Raw HID device: {manufacturer} {product}")
    return dev


def hid_write(dev, packet):
    if hasattr(hid, "device"):
        dev.write(bytes([0x00] + packet))
    else:
        dev.write(bytes(packet))
