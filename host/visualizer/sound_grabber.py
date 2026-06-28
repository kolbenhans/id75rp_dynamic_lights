import argparse
import os
import subprocess
import time
from hid_helper import open_raw_hid, hid_write
import numpy as np

# HID protocol
PACKET_SIZE = 32
PACKET_COMMAND = 0x02
PACKET_SUBCOMMAND = 0xA1

# Audio Settings
RATE = 48000
CHANNELS = 2
BAND_COUNT = 12

# Visualizer refresh rate
FPS = 20

# Bar falloff speed
ENABLE_DECAY = True
DECAY = 15

# Peak marker behavior
PEAK_DECAY = 5
PEAK_HOLD_FRAMES = 16
ENABLE_PEAK_HOLD = False

# Per-band automatic normalization
ENABLE_AUTO_GAIN = True
AUTO_GAIN_DECAY = 0.995
AUTO_GAIN_FLOOR = 80.0
AUTO_GAIN_LEVEL = 0.85

CHUNK_BYTES = int(RATE * CHANNELS * 2 / FPS)

# Used only when ENABLE_AUTO_GAIN = False
SCALE = 2

running_max = np.ones(BAND_COUNT, dtype=np.float32) * AUTO_GAIN_FLOOR
BAND_EDGES = np.geomspace(60, 12000, BAND_COUNT + 1)

display_bands = np.zeros(BAND_COUNT, dtype=np.float32)
peak_bands = np.zeros(BAND_COUNT, dtype=np.float32)
peak_hold = np.zeros(BAND_COUNT, dtype=np.int16)

GAIN = np.array([
    0.45, 0.65, 0.85, 1.00,
    1.15, 1.30, 1.45, 1.60,
    1.80, 2.00, 2.20, 2.40,
])

def get_default_monitor():
    result = subprocess.run(
        ["pactl", "info"],
        capture_output=True,
        text=True,
        check=True,
    )

    for line in result.stdout.splitlines():
        if line.startswith("Default Sink:"):
            sink = line.split(":", 1)[1].strip()
            return f"{sink}.monitor"

    raise RuntimeError("Could not determine default PulseAudio/PipeWire sink")

def start_audio_capture(monitor=None):
    if os.name == "nt":
        return start_windows_audio_capture().start()

    return subprocess.Popen(
        [
            "parec",
            "-d", monitor,
            "--format=s16le",
            f"--rate={RATE}",
            f"--channels={CHANNELS}",
            "--latency-msec=20",
        ],
        stdout=subprocess.PIPE,
    )

def read_audio_samples(proc):
    if os.name == "nt":
        return proc.read()

    data = proc.stdout.read(CHUNK_BYTES)

    if not data:
        return None

    samples = np.frombuffer(data, dtype=np.int16)

    if samples.size == 0:
        return None

    return samples.reshape(-1, CHANNELS).mean(axis=1)

def calculate_band_values(samples):
    windowed = samples * np.hanning(samples.size)

    fft = np.abs(np.fft.rfft(windowed))
    freqs = np.fft.rfftfreq(windowed.size, d=1 / RATE)

    values = []

    for low, high in zip(BAND_EDGES[:-1], BAND_EDGES[1:]):
        mask = (freqs >= low) & (freqs < high)

        if not np.any(mask):
            values.append(0)
            continue

        value = np.sqrt(np.mean(fft[mask]))
        values.append(value)

    return np.array(values, dtype=np.float32)

def normalize_bands(values):
    if ENABLE_AUTO_GAIN:
        running_max[:] = np.maximum(values, running_max * AUTO_GAIN_DECAY)
        normalized = values / running_max
        return np.clip(normalized * 255 * AUTO_GAIN_LEVEL, 0, 255)

    return np.array([
        min(255, int(v * g / SCALE))
        for v, g in zip(values, GAIN)
    ], dtype=np.float32)

def apply_decay(raw_bands):
    if ENABLE_DECAY:
        display_bands[:] = np.maximum(raw_bands, display_bands - DECAY)
    else:
        display_bands[:] = raw_bands
    return display_bands

def update_peak_hold():
    if not ENABLE_PEAK_HOLD:
        peak_bands[:] = display_bands
        return peak_bands

    for i in range(BAND_COUNT):
        if display_bands[i] >= peak_bands[i]:
            peak_bands[i] = display_bands[i]
            peak_hold[i] = PEAK_HOLD_FRAMES
        else:
            if peak_hold[i] > 0:
                peak_hold[i] -= 1
            else:
                peak_bands[i] = max(display_bands[i], peak_bands[i] - PEAK_DECAY)

    return peak_bands

def build_packet(levels, peaks):
    packet = [PACKET_COMMAND, PACKET_SUBCOMMAND] + levels + peaks
    packet += [0x00] * (PACKET_SIZE - len(packet))

    if len(packet) != PACKET_SIZE:
        print("bad packet length", len(packet))
        packet = packet[:PACKET_SIZE]

    return packet

def open_hid(selector=None):
    while True:
        try:
            return open_raw_hid(selector=selector)
        except Exception as e:
            print("waiting for HID:", e)
            time.sleep(1)

def write_packet(dev, packet, selector=None):
    try:
        hid_write(dev, packet)
        return dev
    except Exception as e:
        print("HID write failed:", e)
        try:
            dev.close()
        except Exception:
            pass
        time.sleep(0.5)
        return open_hid(selector=selector)

def start_windows_audio_capture():
    import soundcard as sc

    speaker = sc.default_speaker()
    print("using Windows loopback:", speaker.name)

    mic = sc.get_microphone(
        id=speaker.name,
        include_loopback=True,
    )

    return WindowsLoopbackCapture(mic)

class WindowsLoopbackCapture:
    def __init__(self, mic):
        self.mic = mic
        self.ctx = None
        self.recorder = None

    def start(self):
        self.ctx = self.mic.recorder(
            samplerate=RATE,
            channels=CHANNELS,
        )
        self.recorder = self.ctx.__enter__()
        return self

    def read(self):
        frames = int(RATE / FPS)
        data = self.recorder.record(numframes=frames)

        if data is None or data.size == 0:
            return None

        # SoundCard returns float audio in range -1.0 .. 1.0.
        # Convert to roughly the same scale as Linux int16 capture.
        mono = data.mean(axis=1) * 32767.0
        return mono.astype(np.float32)

    def terminate(self):
        if self.ctx is not None:
            self.ctx.__exit__(None, None, None)

def main():
    parser = argparse.ArgumentParser(
        description="Send audio visualizer data to keyboard via Raw HID.",
    )
    parser.add_argument(
        "--select",
        type=str,
        help="Raw HID device index or regex",
    )
    args = parser.parse_args()

    proc = None
    dev = None

    if os.name == "nt":
        MONITOR = None
    else:
        MONITOR = get_default_monitor()
        print("using monitor", MONITOR)

    dev = open_hid(selector=args.select)
    proc = start_audio_capture(MONITOR)

    try:
        while True:
            samples = read_audio_samples(proc)
            if samples is None:
                continue

            values_np = calculate_band_values(samples)
            raw_bands = normalize_bands(values_np)
            display = apply_decay(raw_bands)
            peaks_display = update_peak_hold()

            levels = [min(255, int(v)) for v in display]
            peaks  = [min(255, int(v)) for v in peaks_display]

            packet = build_packet(levels, peaks)
            dev = write_packet(dev, packet, selector=args.select)

    except KeyboardInterrupt:
        pass
    finally:
        if proc is not None:
            proc.terminate()
        if dev is not None:
            dev.close()

if __name__ == "__main__":
    main()
