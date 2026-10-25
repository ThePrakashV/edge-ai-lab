const int buzzer = 3;

void setup() {
  pinMode(buzzer, OUTPUT);
}

void loop() {
  // Buzzer ON
  digitalWrite(buzzer, HIGH);
  delay(1000);

  // Buzzer OFF
  digitalWrite(buzzer, LOW);
  delay(1000);
}
