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
}

void loop() {

  // 4 & 5
  digitalWrite(led4, HIGH);
  digitalWrite(led5, HIGH);
  delay(200);

  digitalWrite(led4, LOW);
  digitalWrite(led5, LOW);


  // 3 & 6
  digitalWrite(led3, HIGH);
  digitalWrite(led6, HIGH);
  delay(200);

  digitalWrite(led3, LOW);
  digitalWrite(led6, LOW);


  // 2 & 7
  digitalWrite(led2, HIGH);
  digitalWrite(led7, HIGH);
  delay(200);

  digitalWrite(led2, LOW);
  digitalWrite(led7, LOW);


  // 1 & 8
  digitalWrite(led1, HIGH);
  digitalWrite(led8, HIGH);
  delay(200);

  digitalWrite(led1, LOW);
  digitalWrite(led8, LOW);
}
