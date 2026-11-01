#include <Servo.h>

Servo myServo;

// Ultrasonic pins
const int trigPin = 9;
const int echoPin = 10;

// Servo pin
const int servoPin = 6;

// Motor driver pins
const int IN1 = 2;
const int IN2 = 3;
const int IN3 = 4;
const int IN4 = 5;

int centerDistance;
int leftDistance;
int rightDistance;

void setup() {

  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  myServo.attach(servoPin);

  myServo.write(90);

  stopRover();

  delay(1000);
}

void loop() {

  myServo.write(90);
  delay(400);

  centerDistance = getDistance();

  Serial.print("Center: ");
  Serial.println(centerDistance);

  if (centerDistance > 20) {

    forward();

  }

  else {

    stopRover();
    delay(300);

    // Scan Left
    myServo.write(150);
    delay(500);

    leftDistance = getDistance();

    Serial.print("Left: ");
    Serial.println(leftDistance);

    // Scan Right
    myServo.write(30);
    delay(500);

    rightDistance = getDistance();

    Serial.print("Right: ");
    Serial.println(rightDistance);

    // Return to Center
    myServo.write(90);
    delay(300);

    if (leftDistance > rightDistance &&
        leftDistance > 20) {

      turnLeft();
      delay(500);

    }

    else if (rightDistance > leftDistance &&
             rightDistance > 20) {

      turnRight();
      delay(500);

    }

    else {

      reverseRover();
      delay(500);

      turnRight();
      delay(800);

    }

    stopRover();
  }
}

// Ultrasonic distance function
int getDistance() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0) {
    return 0;
  }

  int distance = duration * 0.034 / 2;

  return distance;
}

// Motor functions

void forward() {

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void reverseRover() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopRover() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
