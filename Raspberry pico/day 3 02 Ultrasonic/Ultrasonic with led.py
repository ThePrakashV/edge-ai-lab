from machine import Pin
import utime

trigger = Pin(9, Pin.OUT)
echo = Pin(10, Pin.IN)
led = Pin(13, Pin.OUT)

def ultra():
    trigger.low()
    utime.sleep_us(2)
    trigger.high()
    utime.sleep_us(5)
    trigger.low()

    while echo.value() == 0:
        signaloff = utime.ticks_us()
    while echo.value() == 1:
        signalon = utime.ticks_us()

    timepassed = signalon - signaloff
    distance = (timepassed * 0.0343) / 2
    return distance

while True:
    distance = ultra()
    print("Distance:", distance, "cm")

    if distance < 20:
        led.high()
        print("Obstacle near! Stop")
    else:
        led.low()
        print("Path clear, moving")

    utime.sleep_ms(500)