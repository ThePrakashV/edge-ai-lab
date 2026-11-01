from machine import Pin
import time


r1 = Pin(2, Pin.OUT)
y1 = Pin(3, Pin.OUT)
g1 = Pin(4, Pin.OUT)


r2 = Pin(5, Pin.OUT)
y2 = Pin(6, Pin.OUT)
g2 = Pin(7, Pin.OUT)


r3 = Pin(8, Pin.OUT)
y3 = Pin(9, Pin.OUT)
g3 = Pin(10, Pin.OUT)


r1.value(1)
r2.value(1)
r3.value(1)

while True:
    
    r1.value(0)
    g1.value(1)
    time.sleep(3)

    g1.value(0)
    y1.value(1)
    time.sleep(1)

    y1.value(0)
    r1.value(1)

    r2.value(0)
    g2.value(1)
    time.sleep(3)

    g2.value(0)
    y2.value(1)
    time.sleep(1)

    y2.value(0)
    r2.value(1)

    r3.value(0)
    g3.value(1)
    time.sleep(3)

    g3.value(0)
    y3.value(1)
    time.sleep(1)

    y3.value(0)
    r3.value(1)