int LIR = 2;
int RIR = 3;

int IN1 = 8;
int IN2 = 9;
int IN3 = 10;
int IN4 = 11;

int leftSensor;
int rightSensor;

void setup()
{
  pinMode(LIR, INPUT);
  pinMode(RIR, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.begin(9600);
}

void loop()
{
  leftSensor = digitalRead(LIR);
  rightSensor = digitalRead(RIR);

  Serial.print("LIR: ");
  Serial.print(leftSensor);

  Serial.print(" RIR: ");
  Serial.println(rightSensor);

  if (leftSensor == HIGH && rightSensor == HIGH)
  {
    forward();
  }
  else if (leftSensor == LOW && rightSensor == HIGH)
  {
    left();
  }
  else if (leftSensor == HIGH && rightSensor == LOW)
  {
    right();
  }
  else
  {
    stopRover();
  }
}

void forward()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void left()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void right()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void stopRover()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
