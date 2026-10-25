const int relayPin = 7;

void setup() {
  pinMode(relayPin, OUTPUT);
}

void loop() {
  digitalWrite(relayPin, HIGH);  // Relay ON
  delay(2000);

  digitalWrite(relayPin, LOW);   // Relay OFF
  delay(2000);
}
