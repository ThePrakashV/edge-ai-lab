from machine import Pin
import utime
ir_sensor = Pin(15, Pin.IN)
led = Pin(14, Pin.OUT)

while True:
    if ir_sensor.value() == 0:
        led.value(0)
        print("IR Sensor Detected")
        utime.sleep(1)
    else:
        led.value(1)
        print("IR Sensor Not Detected")