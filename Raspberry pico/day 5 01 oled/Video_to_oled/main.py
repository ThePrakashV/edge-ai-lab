from machine import Pin, I2C
import ssd1306
import time

# --- Wiring (Pico I2C0): GP4 = SDA, GP5 = SCL, 3V3, GND ---
i2c = I2C(0, sda=Pin(4), scl=Pin(5), freq=400_000)

WIDTH, HEIGHT = 128, 64
oled = ssd1306.SSD1306_I2C(WIDTH, HEIGHT, i2c, addr=0x3C)

FRAME_SIZE = WIDTH * HEIGHT // 8   # 1024 bytes per frame
FRAME_MS = 100                     # delay per frame from animation.h (100 ms = 10 fps)

with open("frames.bin", "rb") as f:
    next_tick = time.ticks_ms()
    while True:
        # frames.bin is already in SSD1306 page format -> read straight into the display buffer
        if f.readinto(oled.buffer) != FRAME_SIZE:
            f.seek(0)              # end of file -> loop the animation
            continue
        oled.show()

        # keep a steady frame rate (compensates for I2C transfer time)
        next_tick = time.ticks_add(next_tick, FRAME_MS)
        wait = time.ticks_diff(next_tick, time.ticks_ms())
        if wait > 0:
            time.sleep_ms(wait)
        else:
            next_tick = time.ticks_ms()   # running behind, resync
