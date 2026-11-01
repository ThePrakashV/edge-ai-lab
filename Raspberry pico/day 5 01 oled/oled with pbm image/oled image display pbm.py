from machine import Pin, SoftI2C
import ssd1306
import framebuf
import time

i2c = SoftI2C(scl=Pin(5), sda=Pin(4))

oled = ssd1306.SSD1306_I2C(128, 64, i2c)

def show_pbm(filename):
    with open(filename, "rb") as f:
        if f.readline().strip() != b"P4":
            raise ValueError("PBM file must be P4 format")

        line = f.readline()
        while line.startswith(b"#"):
            line = f.readline()

        width, height = map(int, line.split())

        data = bytearray(f.read())

        fb = framebuf.FrameBuffer(data, width, height, framebuf.MONO_HLSB)

        oled.fill(0)
        oled.blit(fb, 0, 0)
        oled.show()

show_pbm("123.pbm")

while True:
    time.sleep(1)