# transmitter.py
from machine import Pin, SPI
import utime
from nrf24l01 import NRF24L01

spi = SPI(0, sck=Pin(18), mosi=Pin(19), miso=Pin(16))
csn = Pin(17)
ce = Pin(20)

sensor = Pin(15, Pin.IN, Pin.PULL_DOWN)   # sensor OUT pin GP15 la connect pannunga

address = b'1Node'

nrf = NRF24L01(spi, csn, ce, payload_size=1)
nrf.open_tx_pipe(address)
nrf.stop_listening()

while True:
    detected = sensor.value()
    try:
        nrf.send(bytes([1 if detected else 0]))
        print("Sent:", "DETECTED" if detected else "NOT detected")
    except OSError:
        print("send failed")
    utime.sleep_ms(200)