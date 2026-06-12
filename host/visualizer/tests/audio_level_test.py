import subprocess
import numpy as np
import time

MONITOR = "alsa_output.pci-0000_00_1f.3.iec958-stereo.monitor"

proc = subprocess.Popen(
    [
        "parec",
        "-d", MONITOR,
        "--format=s16le",
        "--rate=48000",
        "--channels=2",
    ],
    stdout=subprocess.PIPE,
)

last_print = 0

try:
    while True:
        data = proc.stdout.read(8192)

        if not data:
            continue

        samples = np.frombuffer(data, dtype=np.int16)

        if samples.size == 0:
            continue

        # Stereo -> Mono
        samples = samples.reshape(-1, 2).mean(axis=1)

        # FFT
        fft = np.abs(np.fft.rfft(samples))

        # Frequenzbereich in 12 Gruppen aufteilen
        bands = np.array_split(fft, 12)

        values = []

        for band in bands:
            value = int(np.sqrt(np.mean(band)))
            values.append(value)

        now = time.time()

        if now - last_print > 0.2:
            print(values)
            last_print = now

except KeyboardInterrupt:
    proc.terminate()
