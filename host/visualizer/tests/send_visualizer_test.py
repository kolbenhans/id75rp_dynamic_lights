import math
import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from hid_helper import open_raw_hid, hid_write

PACKET_SIZE = 32
PACKET_COMMAND = 0x02
PACKET_SUBCOMMAND = 0xA1
BAND_COUNT = 12
FPS = 30


def build_packet(bands, peaks):
    packet = [PACKET_COMMAND, PACKET_SUBCOMMAND] + bands + peaks
    packet += [0x00] * (PACKET_SIZE - len(packet))
    return packet[:PACKET_SIZE]


dev = open_raw_hid()
t = 0.0

try:
    while True:
        bands = []

        for i in range(BAND_COUNT):
            v = int((math.sin(t + i * 0.5) * 0.5 + 0.5) * 255)
            bands.append(v)

        peaks = bands[:]

        packet = build_packet(bands, peaks)
        hid_write(dev, packet)

        t += 0.15
        time.sleep(1 / FPS)

except KeyboardInterrupt:
    pass
finally:
    dev.close()
