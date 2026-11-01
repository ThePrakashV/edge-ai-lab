import machine
import utime
from machine import Pin, SoftI2C
import ssd1306

mq2 = machine.ADC(26)
conversion_factor = 3.3 / 65535

i2c = machine.SoftI2C(scl=machine.Pin(5), sda=machine.Pin(4))
pin = machine.Pin(16, machine.Pin.OUT)
pin.value(0)
pin.value(1)

oled_width = 128
oled_height = 64
oled = ssd1306.SSD1306_I2C(oled_width, oled_height, i2c)

threshold = 1.5

while True:
    voltage = mq2.read_u16() * conversion_factor
    print("Output voltage is", voltage)

    oled.fill(0)
    oled.text("Voltage: {:.2f}".format(voltage), 0, 10)

    if voltage > threshold:
        oled.text("Gas Detected!", 0, 30)
    else:
        oled.text("Gas: Normal", 0, 30)

    oled.show()
    utime.sleep(0.2)