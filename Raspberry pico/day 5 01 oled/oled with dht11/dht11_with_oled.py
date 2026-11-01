import machine
from machine import Pin, SoftI2C
import ssd1306
import utime
from dht import DHT11, InvalidChecksum

i2c = machine.SoftI2C(scl=machine.Pin(5), sda=machine.Pin(4))
pin = machine.Pin(16, machine.Pin.OUT)
pin.value(0)
pin.value(1)

oled_width = 128
oled_height = 64
oled = ssd1306.SSD1306_I2C(oled_width, oled_height, i2c)

while True:
    dht_pin = machine.Pin(28, machine.Pin.OUT, machine.Pin.PULL_DOWN)
    sensor = DHT11(dht_pin)
    try:
        t = sensor.temperature
        h = sensor.humidity

        oled.fill(0)
        oled.text("Temp: {}".format(t), 0, 10)
        oled.text("Humi: {}".format(h), 0, 25)
        oled.show()
    except InvalidChecksum:
        pass

    utime.sleep(2)