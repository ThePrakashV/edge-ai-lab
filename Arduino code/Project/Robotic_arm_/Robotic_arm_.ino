#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Create a PCA9685 object
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// Receiver input pins
#define CH1_PIN 3  
#define CH2_PIN 5 
#define CH3_PIN 6 
#define CH4_PIN 9
#define CH5_PIN 10
#define CH6_PIN 11

// Servo min and max pulse width (Modify based on your servos)
#define SERVO_MIN 150  // Approx. 0 degrees
#define SERVO_MAX 600  // Approx. 180 degrees

void setup() {
  Serial.begin(9600);
  Serial.println("FS-CT6B with PCA9685 Servo Driver");

  // Start PCA9685
  pwm.begin();
  pwm.setPWMFreq(50);  // Standard servo frequency (50Hz)

  // Set receiver pins as input
  pinMode(CH1_PIN, INPUT);
  pinMode(CH2_PIN, INPUT);
  pinMode(CH3_PIN, INPUT);
  pinMode(CH4_PIN, INPUT);
  pinMode(CH5_PIN, INPUT);
  pinMode(CH6_PIN, INPUT);
}

void loop() {
  Rx();  
  delay(20);  // Small delay for stability
}

void Rx() {
  // Read PWM signals from FS-CT6B receiver
  int ch1 = pulseIn(CH1_PIN, HIGH);
  int ch2 = pulseIn(CH2_PIN, HIGH);
  int ch3 = pulseIn(CH3_PIN, HIGH);
  int ch4 = pulseIn(CH4_PIN, HIGH);
  int ch5 = pulseIn(CH5_PIN, HIGH);
  int ch6 = pulseIn(CH6_PIN, HIGH);

  // Debugging - Print channel values
  Serial.print("CH1: "); Serial.print(ch1);
  Serial.print(" | CH2: "); Serial.print(ch2);
  Serial.print(" | CH3: "); Serial.print(ch3);
  Serial.print(" | CH4: "); Serial.print(ch4);
  Serial.print(" | CH5: "); Serial.print(ch5);
  Serial.print(" | CH6: "); Serial.println(ch6);

  // Map FS-CT6B PWM range (1100-1800µs) to PCA9685 pulse width (150-600)
  int servo1 = map(ch1, 1100, 1800, SERVO_MIN, SERVO_MAX);
  int servo2 = map(ch2, 1100, 1800, SERVO_MIN, SERVO_MAX);
  int servo3 = map(ch3, 1100, 1800, SERVO_MIN, SERVO_MAX);
  int servo4 = map(ch4, 1100, 1800, SERVO_MIN, SERVO_MAX);
  int servo5 = map(ch5, 1100, 1800, SERVO_MIN, SERVO_MAX);
  int servo6 = map(ch6, 1100, 1800, SERVO_MIN, SERVO_MAX);

  // Move servos using PCA9685
  pwm.setPWM(0, 0, servo1);
  pwm.setPWM(1, 0, servo2);
  pwm.setPWM(2, 0, servo3);
  pwm.setPWM(3, 0, servo4);
  pwm.setPWM(4, 0, servo5);
  pwm.setPWM(5, 0, servo6);
}
