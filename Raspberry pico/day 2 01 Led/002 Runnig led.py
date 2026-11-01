from machine import Pin
from time import sleep

leds = [2,3,4,5,6]

while True:
   for led in leds:
        led_pin = Pin(led,Pin.OUT)
        led_pin.value(1)
        sleep(2)
        led_pin.value(0)
        sleep(0.5)