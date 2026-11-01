#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;   // change to RTC_DS1307 rtc; if you're using a DS1307 module

void setup() {
  Serial.begin(9600);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // ---- Set the RTC time (choose ONE method below) ----

  // Method 1: Set to the date & time this sketch was compiled
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  // Method 2: Set a specific date & time manually
  // Format: DateTime(year, month, day, hour, minute, second)
  // rtc.adjust(DateTime(2026, 7, 29, 14, 30, 0));

  Serial.println("Time set!");
}

void loop() {
  DateTime now = rtc.now();

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  delay(1000);
}
