from machine import Pin
from time import sleep_us, sleep

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
    for i in range(8):
        CLK.value(0)
        DIO.value(data & 0x01)
        data >>= 1
        sleep_us(5)
        CLK.value(1)
        sleep_us(5)

    CLK.value(0)
    DIO.init(Pin.IN)
    CLK.value(1)
    sleep_us(5)
    CLK.value(0)
    DIO.init(Pin.OUT)

def display(num):
    t = [
        digits[(num//1000)%10],
        digits[(num//100)%10],
        digits[(num//10)%10],
        digits[num%10]
    ]

    start()
    write_byte(0x40)
    stop()

    start()
    write_byte(0xC0)
    for i in t:
        write_byte(i)
    stop()

    start()
    write_byte(0x88 | 7)
    stop()

count = 0

while True:
    display(count)
    count += 1
    if count > 9999:
        count = 0
    sleep(1)