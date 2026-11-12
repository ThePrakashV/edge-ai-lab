"""
MP4 (or any video) -> animation.h   for 128x64 SSD1306 OLED displays

Install once:   pip install opencv-python pillow

Two ways to set the video file:
  1) Edit VIDEO_PATH below, then run:   python mp4_to_animation_h.py
  2) Pass it on the command line:       python mp4_to_animation_h.py video.mp4

Usage examples:
    python mp4_to_animation_h.py video.mp4
    python mp4_to_animation_h.py video.mp4 animation.h --name Radhima --fps 10
    python mp4_to_animation_h.py video.mp4 --start 5 --duration 20 --threshold
    python mp4_to_animation_h.py video.mp4 --stretch --invert

Output format:
    NAME_frames[N][1024]  (row by row, MSB first = Adafruit drawBitmap layout)
    NAME_delays[N]        (ms per frame)
    AnimatedGIF struct + NAME_gif
"""
import argparse
import sys

import cv2
from PIL import Image, ImageOps

# ------------------------------------------------------------------
# PUT YOUR VIDEO FILE HERE (used when no path is given on the command line)
VIDEO_PATH = "C:/Users/PrakashV/Downloads/videoplayback.mp4"
# ------------------------------------------------------------------

W, H = 128, 64
FRAME_BYTES = W * H // 8  # 1024


def make_frame(bgr, stretch, invert, threshold, contrast):
    """One video frame -> 1024 bytes (1 bit per pixel, row by row, MSB first)."""
    img = Image.fromarray(cv2.cvtColor(bgr, cv2.COLOR_BGR2RGB))

    if stretch:                                   # fill the whole screen
        img = img.resize((W, H), Image.LANCZOS)
    else:                                         # keep aspect ratio, black bars
        img = ImageOps.contain(img, (W, H), Image.LANCZOS)
        canvas = Image.new("RGB", (W, H), (0, 0, 0))
        canvas.paste(img, ((W - img.width) // 2, (H - img.height) // 2))
        img = canvas

    gray = img.convert("L")
    if contrast:
        gray = ImageOps.autocontrast(gray)
    if invert:
        gray = ImageOps.invert(gray)

    if threshold is not None:                     # hard black/white
        bw = gray.point(lambda p: 255 if p >= threshold else 0).convert("1", dither=Image.NONE)
    else:                                         # Floyd-Steinberg dithering (looks smoother)
        bw = gray.convert("1")
    return bw.tobytes()                           # 16 bytes per row, MSB first


def main():
    ap = argparse.ArgumentParser(description="Video -> animation.h for 128x64 OLED")
    ap.add_argument("video", nargs="?", default=VIDEO_PATH,
                    help=f"video file (default: {VIDEO_PATH})")
    ap.add_argument("output", nargs="?", default="animation.h")
    ap.add_argument("--name", default="Animation", help="name used in the C identifiers")
    ap.add_argument("--fps", type=float, default=10, help="output frames per second (default 10)")
    ap.add_argument("--start", type=float, default=0, help="start time in seconds")
    ap.add_argument("--duration", type=float, default=None, help="seconds to convert")
    ap.add_argument("--max-frames", type=int, default=255, help="limit (struct uses uint8_t, max 255)")
    ap.add_argument("--threshold", type=int, nargs="?", const=128, default=None,
                    help="plain black/white instead of dithering (optional value 0-255, default 128)")
    ap.add_argument("--no-autocontrast", action="store_true", help="disable automatic contrast")
    ap.add_argument("--invert", action="store_true", help="swap black and white")
    ap.add_argument("--stretch", action="store_true", help="stretch to 128x64 (no black bars)")
    args = ap.parse_args()

    if args.max_frames > 255:
        sys.exit("max-frames cannot be more than 255 (frame_count is uint8_t in the struct)")

    cap = cv2.VideoCapture(args.video)
    if not cap.isOpened():
        sys.exit(f"Cannot open video: {args.video}")
    src_fps = cap.get(cv2.CAP_PROP_FPS) or 30
    step = 1.0 / args.fps

    frames = []
    index = 0                       # source frame number
    next_time = args.start          # time of the next frame we want
    end_time = args.start + args.duration if args.duration else float("inf")

    while len(frames) < args.max_frames:
        ok, bgr = cap.read()
        if not ok:
            break
        t = index / src_fps
        index += 1
        if t < next_time:
            continue
        if t >= end_time:
            break
        frames.append(make_frame(bgr, args.stretch, args.invert, args.threshold,
                                 not args.no_autocontrast))
        next_time += step
    cap.release()

    if not frames:
        sys.exit("No frames were read - check --start / --duration.")

    delay = round(1000 / args.fps)
    name = "".join(c if c.isalnum() else "_" for c in args.name)
    up = name.upper()
    n = len(frames)

    header = f"""#ifndef {up}_H
#define {up}_H

#include <stdint.h>
#include <pgmspace.h>

#ifndef ANIMATED_GIF_DEFINED
#define ANIMATED_GIF_DEFINED
typedef struct {{
    const uint8_t frame_count;
    const uint16_t width;
    const uint16_t height;
    const uint16_t* delays;
    const uint8_t (* frames)[1024];
}} AnimatedGIF;
#endif // ANIMATED_GIF_DEFINED

#define {up}_FRAME_COUNT {n}
#define {up}_WIDTH {W}
#define {up}_HEIGHT {H}

const uint16_t {name}_delays[{up}_FRAME_COUNT] = {{{", ".join([str(delay)] * n)}}};

PROGMEM const uint8_t {name}_frames[{up}_FRAME_COUNT][1024] = {{
"""

    blocks = []
    for fr in frames:
        lines = []
        for i in range(0, FRAME_BYTES, 16):
            lines.append("    " + ", ".join(f"0x{b:02x}" for b in fr[i:i + 16]))
        blocks.append("  {\n" + ",\n".join(lines) + "\n  }")
    body = ",\n".join(blocks)

    footer = f"""
}};

const AnimatedGIF {name}_gif = {{
    .frame_count = {up}_FRAME_COUNT,
    .width = {up}_WIDTH,
    .height = {up}_HEIGHT,
    .delays = {name}_delays,
    .frames = {name}_frames
}};

// playGIF(&{name}_gif);

#endif
"""

    with open(args.output, "w") as f:
        f.write(header + body + footer)

    kb = n * FRAME_BYTES / 1024
    print(f"Done: {n} frames ({n * delay / 1000:.1f} s at {args.fps:g} fps) -> {args.output}")
    print(f"Animation data size: {kb:.0f} KB (check that it fits in your board's flash)")
    if n == args.max_frames:
        print("Note: frame limit reached - use --fps lower, or --start/--duration to pick a part.")


if __name__ == "__main__":
    main()
