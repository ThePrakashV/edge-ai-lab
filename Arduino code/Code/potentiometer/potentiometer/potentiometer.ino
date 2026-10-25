const int potPin = A0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int potValue = analogRead(potPin);

  Serial.print("Potentiometer Value: ");
  Serial.println(potValue);

  delay(100);
}
