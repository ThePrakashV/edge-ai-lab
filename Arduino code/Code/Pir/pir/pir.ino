const int Pir = 3;
int Pir_value;
void setup() {
  pinMode(Pir, INPUT);
  Serial.begin(9600);
}

void loop() {
  Pir_value = digitalRead(Pir_value);

  if (Pir_value == LOW) {
    Serial.println("Pir is Detected");
  }
  else{
    Serial.println("Pir is Not Detected");
  }
}
