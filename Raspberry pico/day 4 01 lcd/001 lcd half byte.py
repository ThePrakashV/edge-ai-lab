from machine import Pin
from time import sleep_ms

class LCD:
    def __init__(self, rs, e, d4, d5, d6, d7):
        self.rs = Pin(rs, Pin.OUT)
        self.e = Pin(e, Pin.OUT)
        self.data = [
            Pin(d4, Pin.OUT),
            Pin(d5, Pin.OUT),
            Pin(d6, Pin.OUT),
            Pin(d7, Pin.OUT)
        ]

        sleep_ms(20)
        self.command(0x33)
        self.command(0x32)
        self.command(0x28)  # 4-bit, 2-line
        self.command(0x0C)  # Display ON
        self.command(0x06)  # Entry mode
        self.command(0x01)  # Clear
        sleep_ms(2)

    def pulse(self):
        self.e.value(1)
        sleep_ms(1)
        self.e.value(0)
        sleep_ms(1)

    def write4(self, value):
        for i in range(4):
            self.data[i].value((value >> i) & 1)
        self.pulse()

    def send(self, value, mode):
        self.rs.value(mode)
        self.write4(value >> 4)
        self.write4(value & 0x0F)

    def command(self, cmd):
        self.send(cmd, 0)

    def write(self, char):
        self.send(ord(char), 1)

    def print(self, text):
        for c in text:
            self.write(c)

    def move_to(self, col, row):
        addr = 0x80 + col if row == 0 else 0xC0 + col
        self.command(addr)

lcd = LCD(2, 3, 4, 5, 6, 7)

lcd.move_to(0, 0)
lcd.print("Hello World")

lcd.move_to(0, 1)
lcd.print("Raspberry Pico")