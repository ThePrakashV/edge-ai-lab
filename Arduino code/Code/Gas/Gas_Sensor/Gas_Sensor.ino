const int Gas = A0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int GasValue = analogRead(Gas);

  Serial.print("Gas Value: ");
  Serial.println(GasValue);

  delay(500);
}
