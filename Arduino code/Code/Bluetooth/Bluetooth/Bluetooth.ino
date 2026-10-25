#include <SoftwareSerial.h>

SoftwareSerial bluetooth(10, 11);
// Arduino D10 = RX
// Arduino D11 = TX

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);

  Serial.println("HC-05 Bluetooth Ready");
}

void loop() {

  // Bluetooth -> Arduino
  if (bluetooth.available()>0) {
    char data = bluetooth.read();

    Serial.print("Received: ");
    Serial.println(data);
  } 
}
