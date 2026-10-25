const int soundPin = 2;
int soundState;

void setup() {
  pinMode(soundPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  soundState = digitalRead(soundPin);

  if (soundState == LOW) {
    Serial.println("Sound Off");
  }
  else {
    Serial.println("Sound On");
  }

  delay(200);
}
