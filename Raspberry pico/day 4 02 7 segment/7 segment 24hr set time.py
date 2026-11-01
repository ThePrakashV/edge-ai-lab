from machine import Pin
from time import sleep, ticks_ms, ticks_diff

CLK = Pin(5, Pin.OUT)
DIO = Pin(4, Pin.OUT)

digits = [0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F]

def start():
    DIO.value(1)
    CLK.value(1)
    DIO.value(0)

def stop():
    CLK.value(0)
    DIO.value(0)
    CLK.value(1)
    DIO.value(1)

def write_byte(data):
    for _ in range(8):
        CLK.value(0)
        DIO.value(data & 1)
        data >>= 1
        CLK.value(1)

    CLK.value(0)
    DIO.init(Pin.IN)
    CLK.value(1)
    CLK.value(0)
    DIO.init(Pin.OUT)

def display(hour, minute, colon=True):
    data = [
        digits[hour // 10],
        digits[hour % 10] | (0x80 if colon else 0),
        digits[minute // 10],
        digits[minute % 10]
    ]

    start()
    write_byte(0x40)
    stop()

    start()
    write_byte(0xC0)
    for d in data:
        write_byte(d)
    stop()

    start()
    write_byte(0x8F)
    stop()

# -------- SET START TIME HERE --------
hour = 1
minute = 25
# ------------------------------------

last = ticks_ms()
colon = True

while True:
    display(hour, minute, colon)
    colon = not colon

    if ticks_diff(ticks_ms(), last) >= 60000:
        last += 60000
        minute += 1
        if minute == 60:
            minute = 0
            hour += 1
            if hour == 24:
                hour = 0

    sleep(0.5)