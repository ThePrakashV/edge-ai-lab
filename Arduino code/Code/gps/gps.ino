#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>

static const uint8_t GPS_RX_PIN = 4;   // Arduino D4 <- GPS TX
static const uint8_t GPS_TX_PIN = 3;   // Arduino D3 -> GPS RX
static const uint32_t GPS_BAUD  = 9600;   // NEO-6M default
static const uint32_t PC_BAUD   = 9600;   // App-la select panna baud same irukkanum

SoftwareSerial gpsSerial(GPS_RX_PIN, GPS_TX_PIN);
TinyGPSPlus gps;

unsigned long lastSend = 0;

void setup() {
  Serial.begin(PC_BAUD);
  gpsSerial.begin(GPS_BAUD);
}

void loop() {
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  if (millis() - lastSend >= 1000) {
    lastSend = millis();

    if (gps.location.isValid() && gps.location.age() < 3000) {
      Serial.print("LAT:");
      Serial.print(gps.location.lat(), 6);
      Serial.print(",LON:");
      Serial.print(gps.location.lng(), 6);
      Serial.print(",SAT:");
      Serial.println(gps.satellites.isValid() ? gps.satellites.value() : 0);
    }
  }

  if (millis() > 10000 && gps.charsProcessed() < 10) {
    Serial.println("check GPS wiring (no data from module)");
    delay(2000);
  }
}
