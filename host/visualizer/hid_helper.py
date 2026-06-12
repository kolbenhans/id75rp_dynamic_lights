import hid

USAGE_PAGE = 65376
USAGE = 97


def find_raw_hid_devices():
    devices = []

    for d in hid.enumerate():
        if d.get("usage_page") == USAGE_PAGE and d.get("usage") == USAGE:
            devices.append(d)

    return devices


def select_device(devices):
    if not devices:
        raise RuntimeError("No compatible Raw HID device found")

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


def open_raw_hid():
    device_info = select_device(find_raw_hid_devices())
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
