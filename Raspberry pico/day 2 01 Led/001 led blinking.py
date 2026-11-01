from machine import Pin
import time

led = Pin(25, Pin.OUT)

while True:
    led.value(1)   # ON
    time.sleep(0.5)
    led.value(0)   # OFF
    time.sleep(0.5)