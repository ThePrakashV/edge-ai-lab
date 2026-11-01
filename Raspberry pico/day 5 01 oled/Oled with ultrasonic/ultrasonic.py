import machine
from machine import Pin, SoftI2C
import ssd1306
import utime


i2c = machine.SoftI2C(scl=machine.Pin(5), sda=machine.Pin(4))
pin = machine.Pin(16, machine.Pin.OUT)
pin.value(0)  # set GPIO16 low to reset OLED
pin.value(1)  # while OLED is running, must set GPIO16 in high
oled_width = 128 # different width & height size's for different oled's 128 * 32 / 128 * 64 
oled_height = 64
oled = ssd1306.SSD1306_I2C(oled_width, oled_height, i2c)

trigger = Pin(20, Pin.OUT)


echo = Pin(21, Pin.IN)


def ultra():


    trigger.low()


    utime.sleep_us(2)


    trigger.high()


    utime.sleep_us(5)


    trigger.low()


    while echo.value() == 0:


        signaloff = utime.ticks_us()


    while echo.value() == 1:


        signalon = utime.ticks_us()


    timepassed = signalon - signaloff


    distance = (timepassed * 0.0343) / 2


    return distance


while True:


    # Measure the distance


    distance_cm = ultra()


    # Update the OLED display


    oled.fill(0)


    oled.text("Distance:",0,10,1) # 0 x position goes upto 128, 10-y position goes upto 32/64 rows, 0or1 for color on/off


    oled.text("{:.4f} cm".format(distance_cm), 0, 20)


    oled.show()


    # Wait for a short interval before taking the next measurement


    utime.sleep(0.2)