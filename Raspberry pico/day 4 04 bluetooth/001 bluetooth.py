from machine import UART, Pin
from time import sleep

uart = UART(0, baudrate=9600, tx=Pin(0), rx=Pin(1))
led = Pin(15, Pin.OUT)

while True:
    if uart.any():
        msg = uart.read().decode().strip()

        if msg == "ON":
            led.value(1)
            uart.write("LED ON\n")

        elif msg == "OFF":
            led.value(0)
            uart.write("LED OFF\n")

    sleep(0.1)