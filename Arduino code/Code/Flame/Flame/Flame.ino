const int Flame = 3;
int Flame_value;
void setup() {
  pinMode(Flame, INPUT);
  Serial.begin(9600);
}

void loop() {
  Flame_value = digitalRead(Flame);

  if (Flame_value == LOW) {
    Serial.println("Flame is Detected");
  }
  else{
    Serial.println("Flame is Not Detected");
  }
}
