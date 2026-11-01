#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio( 5,6); // CE, CSN
const byte address[6] = "00001";
void setup() {
  radio.begin();
  Serial.begin(9600);
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  const char text[] = "ROBOTICS";
  radio.write(&text, sizeof(text));
}

void loop() {
const char text[] = "AUTOMATIONS";
  radio.write(&text, sizeof(text));
  delay(500);
  }