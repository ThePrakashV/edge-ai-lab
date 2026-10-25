const int buttonPin = 3;
int buttonState;
void setup() {
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  buttonState = digitalRead(buttonPin);

  if (buttonState == LOW) {
    Serial.println("Button Off");
  }
  else{
    Serial.println("Button On");
  }
}
