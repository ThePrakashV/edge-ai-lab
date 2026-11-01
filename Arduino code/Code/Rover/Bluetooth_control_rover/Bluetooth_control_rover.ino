#include <SoftwareSerial.h>

SoftwareSerial BT(10, 11);

int IN1 = 2;
int IN2 = 3;
int IN3 = 4;
int IN4 = 5;

char command;

void setup()
{
  Serial.begin(9600);
  BT.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopRover();

  Serial.println("Bluetooth + Serial Control");
  Serial.println("U D L R 1");
}

void loop()
{
  if (Serial.available()>0)
  {
    command = Serial.read();
    controlRover(command);
  }

  else if (BT.available())
  {
    command = BT.read();
    controlRover(command);
  }
}

void controlRover(char cmd)
{
  if (cmd >= 'a' && cmd <= 'z')
  {
    cmd = cmd - 32;
  }

  if (cmd == 'U')
  {
    forward();
  }

  else if (cmd == 'D')
  {
    backward();
  }

  else if (cmd == 'L')
  {
    left();
  }

  else if (cmd == 'R')
  {
    right();
  }

  else if (cmd == '1')
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

void backward()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void left()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void right()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopRover()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
