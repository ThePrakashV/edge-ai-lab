from machine import Pin, PWM
import time

led = PWM(Pin(2))
led.freq(1000)

while True:
    for duty in range(0, 65535, 500):
        led.duty_u16(duty)
        time.sleep(0.1)
    for duty in range(65535, 0, -500):
        led.duty_u16(duty)
        time.sleep(0.1)