
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>

RF24 radio(5, 6);  // CE, CSN
const byte address[6] = "00001";


const int MPU = 0x68;  // MPU6050 I2C address
float AccX, AccY, AccZ;
float GyroX, GyroY;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY;
float angleX, angleY;
float elapsedTime, currentTime, previousTime;

// Input pins
const int button1 = 2;
const int button2 = 3;
const int toggle1 = 4;
const int toggle2 = 7;
const int button3 = 8;
const int button4 = 9;
const int buzzer = 10;

const int left_x_joystick  = A0;
const int left_y_joystick  = A1;
const int right_x_joystick = A2;
const int right_y_joystick = A3;

const int right_pot = A6;
const int left_pot  = A7;

// Array index definitions
enum DataIndex {
  BUTTON1_DATA = 0,
  BUTTON2_DATA,
  BUTTON3_DATA,
  BUTTON4_DATA,
  TOGGLE1_DATA,
  TOGGLE2_DATA,
  LEFT_JOYSTICK_X_DATA,
  LEFT_JOYSTICK_Y_DATA,
  RIGHT_JOYSTICK_X_DATA,
  RIGHT_JOYSTICK_Y_DATA,
  RIGHT_POT_DATA,
  LEFT_POT_DATA,
  anglex,
  angley,
  TOTAL_DATA
};

// All 12 input values
int data[TOTAL_DATA];

void setup() {
  Serial.begin(9600);
    initialize_MPU6050();


  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  pinMode(button4, INPUT);

  pinMode(toggle1, INPUT);
  pinMode(toggle2, INPUT);
  pinMode(buzzer,OUTPUT);

  if (!radio.begin()) {
    Serial.println("NRF24L01 not detected!");
    digitalWrite(buzzer,HIGH);
    while (1) {}
  }
  digitalWrite(buzzer,LOW);

  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(108);
  radio.setAutoAck(true);
  radio.stopListening();

  Serial.println("TX Ready");
}
void initialize_MPU6050() {
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);

  // Accelerometer config: +/- 8g
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);
  Wire.write(0x10);
  Wire.endTransmission(true);

  // Gyro config: 1000 dps
  Wire.beginTransmission(MPU);
  Wire.write(0x1B);
  Wire.write(0x10);
  Wire.endTransmission(true);
}
void read_IMU() {
  // Read accelerometer
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);
  AccX = (Wire.read() << 8 | Wire.read()) / 4096.0;
  AccY = (Wire.read() << 8 | Wire.read()) / 4096.0;
  AccZ = (Wire.read() << 8 | Wire.read()) / 4096.0;

  accAngleX = (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI) + 1.15;
  accAngleY = (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI) - 0.52;

  // Read gyro
  previousTime = currentTime;
  currentTime = millis();
  elapsedTime = (currentTime - previousTime) / 1000.0;

  Wire.beginTransmission(MPU);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 4, true);
  GyroX = (Wire.read() << 8 | Wire.read()) / 32.8;
  GyroY = (Wire.read() << 8 | Wire.read()) / 32.8;
  GyroX += 1.85;
  GyroY -= 0.15;

  gyroAngleX = GyroX * elapsedTime;
  gyroAngleY = GyroY * elapsedTime;

  // Complementary filter
  angleX = 0.98 * (angleX + gyroAngleX) + 0.02 * accAngleX;
  angleY = 0.98 * (angleY + gyroAngleY) + 0.02 * accAngleY;
}

void readData() {
  // Buttons
  data[BUTTON1_DATA] = digitalRead(button1);
  data[BUTTON2_DATA] = digitalRead(button2);
  data[BUTTON3_DATA] = digitalRead(button3);
  data[BUTTON4_DATA] = digitalRead(button4);

  // Toggle switches
  data[TOGGLE1_DATA] = digitalRead(toggle1);
  data[TOGGLE2_DATA] = digitalRead(toggle2);

  // Left joystick
  data[LEFT_JOYSTICK_X_DATA] = analogRead(left_x_joystick);
  data[LEFT_JOYSTICK_Y_DATA] = analogRead(left_y_joystick);

  // Right joystick
  data[RIGHT_JOYSTICK_X_DATA] = analogRead(right_x_joystick);
  data[RIGHT_JOYSTICK_Y_DATA] = analogRead(right_y_joystick);

  // Potentiometers
  data[RIGHT_POT_DATA] = analogRead(right_pot);
  data[LEFT_POT_DATA] = analogRead(left_pot);
  read_IMU();
  data[anglex] = angleX;
  data[angley] = angleY;
}

void loop() {
  readData();

  // Send the complete array
  bool success = radio.write(data, sizeof(data));

  if (success) {
    Serial.println("All data sent successfully");
  } else {
    Serial.println("Send failed - check RX");
  }

  // TX Serial Monitor
  for (int i = 0; i < TOTAL_DATA; i++) {
    Serial.print("data[");
    Serial.print(i);
    Serial.print("] = ");
    Serial.println(data[i]);
  }

  Serial.println("----------------------");
  delay(100);
}
