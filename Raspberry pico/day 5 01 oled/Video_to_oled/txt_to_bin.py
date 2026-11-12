"""
Convert an OLED animation C/C++ file (.h / .txt / .ino) to frames.bin for the Pico.

Supported formats:
  1) animation.h style : PROGMEM const uint8_t NAME_frames[N][1024] = { {..}, {..} }
                         + const uint16_t NAME_delays[N] = {100, ...}
  2) Animation Maker   : const uint8_t PROGMEM frame0[1024] = {...}; frame1 ...

Usage:  python txt_to_bin.py [input] [output.bin]
Output: 1024 bytes per frame in SSD1306 page format (MONO_VLSB).
If frame delays differ, a delays.bin (uint16 little-endian, ms) is also written.
"""
import re
import struct
import sys

W, H = 128, 64
FRAME_BYTES = W * H // 8

src_path = sys.argv[1] if len(sys.argv) > 1 else "animation.h"
dst_path = sys.argv[2] if len(sys.argv) > 2 else "frames.bin"
text = open(src_path, encoding="utf-8", errors="ignore").read()


def hex_bytes(s):
    return bytes(int(h, 16) for h in re.findall(r"0x([0-9a-fA-F]{2})", s))


frames, delays = [], []

# --- format 1: one 2D array  name_frames[N][1024] = { { ... }, ... } ---
m = re.search(r"(\w+)\s*\[\s*[\w]*\s*\]\s*\[\s*1024\s*\]\s*=\s*\{(.*?)\n\};", text, re.S)
if m:
    data = hex_bytes(m.group(2))
    frames = [data[i:i + FRAME_BYTES] for i in range(0, len(data), FRAME_BYTES)]
    d = re.search(r"_delays\s*\[[^\]]*\]\s*=\s*\{(.*?)\}", text, re.S)
    if d:
        delays = [int(x) for x in re.findall(r"\d+", d.group(1))]

# --- format 2: separate frameN[1024] arrays ---
if not frames:
    arrays = {}
    for m in re.finditer(r"PROGMEM\s+frame(\d+)\s*\[\d*\]\s*=\s*\{(.*?)\};", text, re.S):
        arrays[int(m.group(1))] = hex_bytes(m.group(2))
    t = re.search(r"frames\s*\[\s*\]\s*=\s*\{(.*?)\};", text, re.S)
    order = [int(n) for n in re.findall(r"frame(\d+)", t.group(1))] if t else sorted(arrays)
    frames = [arrays[n] for n in order]
    dm = re.search(r"millis\(\)\s*-\s*lastMs\s*>=\s*(\d+)", text)
    delays = [int(dm.group(1))] * len(frames) if dm else []

if not frames:
    sys.exit("No frames found in this file.")
if any(len(f) != FRAME_BYTES for f in frames):
    sys.exit("A frame is not 1024 bytes - is the display 128x64?")


def convert(frame):
    """row-by-row bitmap (Adafruit drawBitmap) -> SSD1306 page format"""
    out = bytearray(FRAME_BYTES)
    for page in range(H // 8):
        for x in range(W):
            value, mask = 0, 0x80 >> (x & 7)
            for bit in range(8):
                if frame[(page * 8 + bit) * (W // 8) + (x >> 3)] & mask:
                    value |= 1 << bit
            out[page * W + x] = value
    return bytes(out)


with open(dst_path, "wb") as f:
    for fr in frames:
        f.write(convert(fr))
print(f"Done: {len(frames)} frames -> {dst_path} ({len(frames) * FRAME_BYTES} bytes)")

if delays and len(set(delays)) == 1:
    print(f"All frames use the same delay: {delays[0]} ms  -> set FRAME_MS = {delays[0]} in main.py")
elif delays:
    with open("delays.bin", "wb") as f:
        f.write(struct.pack(f"<{len(delays)}H", *delays))
    print("Delays differ per frame -> delays.bin written (copy it to the Pico too)")
