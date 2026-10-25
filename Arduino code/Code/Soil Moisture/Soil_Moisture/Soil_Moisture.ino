int soilPin = A1;
int soilValue = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  soilValue = analogRead(soilPin);

  Serial.print("Soil Moisture Value: ");
  Serial.println(soilValue);

  delay(1000);
}
