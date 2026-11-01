from machine import Pin
import utime as time
from dht import DHT11, InvalidChecksum

while True:
    pin = Pin(28, Pin.OUT, Pin.PULL_DOWN)
    sensor = DHT11(pin)
    try:
        t = sensor.temperature
        h = sensor.humidity
        print("Temperature: {} *C".format(t))
        print("Humidity: {} %".format(h))
    except InvalidChecksum:
        print("Checksum error, retrying...")
    time.sleep(2)