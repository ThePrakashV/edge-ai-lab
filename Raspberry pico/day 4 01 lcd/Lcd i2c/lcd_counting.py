import machine
from machine import I2C
from lcd_api import LcdApi
from pico_i2c_lcd import I2cLcd
from time import sleep

I2C_ADDR = 0x27
totalRows = 2
totalColumns = 16

i2c = I2C(0, sda=machine.Pin(0), scl=machine.Pin(1), freq=400000)
lcd = I2cLcd(i2c, I2C_ADDR, totalRows, totalColumns)

while True:
    lcd.putstr("Lets Count 0-10!")
    sleep(2)
    lcd.clear()
    for i in range(10):
        lcd.putstr(str(i))
        sleep(1)
        lcd.clear()