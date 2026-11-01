#include <Servo.h>
#include <AFMotor.h>

#define Echo  A0
#define Trig  A1
#define SERVO_PIN 10
#define SPEED     170

AF_DCMotor M1(1);
AF_DCMotor M2(2);
AF_DCMotor M3(3);
AF_DCMotor M4(4);

char value;

void setup() {
  Serial.begin(9600);
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  M1.setSpeed(SPEED);
  M2.setSpeed(SPEED);
  M3.setSpeed(SPEED);
  M4.setSpeed(SPEED);
}

void loop() {
  if (Serial.available() > 0) {
    value = Serial.read();
    Serial.println(value);
  }

  if (value == 'U') {
    if (ultrasonic() <= 20) {
      Stop();
      Serial.println("Obkect detected! Stopped.");
    } else {
      forward();
    }
  }
  else if (value == 'D') { backward(); }
  else if (value == 'R') { right();    }
  else if (value == 'L') { left();     }
  else if (value == 'S') { Stop();     }
}

int ultrasonic() {
  digitalWrite(Trig, LOW);
  delayMicroseconds(4);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  long t  = pulseIn(Echo, HIGH, 30000UL);
  long cm = t / 29 / 2;
  return (int)cm;
}
void forward() {
  M1.run(FORWARD);  M2.run(FORWARD);
  M3.run(FORWARD);  M4.run(FORWARD);
}
void backward() {
  M1.run(BACKWARD); M2.run(BACKWARD);
  M3.run(BACKWARD); M4.run(BACKWARD);
}
void right() {
  M1.run(BACKWARD); M2.run(BACKWARD);
  M3.run(FORWARD);  M4.run(FORWARD);
}
void left() {
  M1.run(FORWARD);  M2.run(FORWARD);
  M3.run(BACKWARD); M4.run(BACKWARD);
}
void Stop() {
  M1.run(RELEASE);  M2.run(RELEASE);
  M3.run(RELEASE);  M4.run(RELEASE);
}
