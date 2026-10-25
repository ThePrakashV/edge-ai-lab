int leds[] = {2, 3, 4, 5, 6, 7, 8, 9};

void setup() {
  for (int i = 0; i < 8; i++) {
    pinMode(leds[i], OUTPUT);
  }
}

void loop() {
  // ON one by one
  for (int i = 0; i < 8; i++) {
    digitalWrite(leds[i], HIGH);
    delay(200);
  }

  // OFF one by one
  for (int i = 0; i < 8; i++) {
    digitalWrite(leds[i], LOW);
    delay(200);
  }
}
