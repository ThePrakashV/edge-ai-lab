from machine import ADC, PWM, Pin
from time import sleep
ldr = ADC(26)
led = PWM(Pin(14))
led.freq(1000)
while True:
    ldr_value = ldr.read_u16()
    brightness = 65535 - ldr_value
    led.duty_u16(brightness)
    print("LDR:", ldr_value, "Brightness:", brightness)
    sleep(0.1)