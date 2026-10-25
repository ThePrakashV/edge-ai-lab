int led1 = 2;
int led2 = 3;
int led3 = 4;
int led4 = 5;
int led5 = 6;
int led6 = 7;
int led7 = 8;
int led8 = 9;

void setup() {

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(led7, OUTPUT);
  pinMode(led8, OUTPUT);

  randomSeed(analogRead(A0));
}

void loop() {

  // All OFF
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  digitalWrite(led5, LOW);
  digitalWrite(led6, LOW);
  digitalWrite(led7, LOW);
  digitalWrite(led8, LOW);

  int randomLED = random(1, 9);

  if (randomLED == 1)
    digitalWrite(led1, HIGH);

  if (randomLED == 2)
    digitalWrite(led2, HIGH);

  if (randomLED == 3)
    digitalWrite(led3, HIGH);

  if (randomLED == 4)
    digitalWrite(led4, HIGH);

  if (randomLED == 5)
    digitalWrite(led5, HIGH);

  if (randomLED == 6)
    digitalWrite(led6, HIGH);

  if (randomLED == 7)
    digitalWrite(led7, HIGH);

  if (randomLED == 8)
    digitalWrite(led8, HIGH);

  delay(200);
}
