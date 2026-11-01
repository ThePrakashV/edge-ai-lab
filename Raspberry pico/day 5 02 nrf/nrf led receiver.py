# receiver.py
from machine import Pin, SPI
import utime
from nrf24l01 import NRF24L01

spi = SPI(0, sck=Pin(18), mosi=Pin(19), miso=Pin(16))
csn = Pin(17)
ce = Pin(20)

led = Pin(25, Pin.OUT)   # Pico onboard LED, external LED na GP14 use pannalam

address = b'1Node'

nrf = NRF24L01(spi, csn, ce, payload_size=1)
nrf.open_rx_pipe(0, address)
nrf.start_listening()

while True:
    if nrf.any():
        buf = nrf.recv()
        if buf[0] == 1:
            led.value(1)
            print("LED ON - object detected")
        else:
            led.value(0)
            print("LED OFF - no detection")
    utime.sleep_ms(50)