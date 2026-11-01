from machine import Pin, ADC
import utime

ldr = ADC(Pin(26))
led = Pin(14, Pin.OUT)

while True:
    value = ldr.read_u16()
    print("LDR value:", value)

    if value < 30000:
        led.high()
        print("Dark - LED ON")
    else:
        led.low()
        print("Light - LED OFF")

    utime.sleep_ms(500)