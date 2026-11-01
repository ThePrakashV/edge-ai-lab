from machine import Pin
import utime

ir_sensor = Pin(15, Pin.IN)
led = Pin(14, Pin.OUT)

detect_count = 0

while True:
    if ir_sensor.value() == 1:
        detect_count = detect_count + 1
        print("Detected! count =", detect_count)

        if detect_count >= 2:
            led.value(1)
            print("LED ON")
            utime.sleep(1)
            led.value(0)
            detect_count = 0
        else:
            led.value(0)

        utime.sleep(1)
    else:
        led.value(0)