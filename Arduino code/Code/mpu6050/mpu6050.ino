#include <Wire.h>

const int MPU = 0x68;  // MPU6050 I2C address
float AccX, AccY, AccZ;
float GyroX, GyroY;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY;
float angleX, angleY;
float elapsedTime, currentTime, previousTime;

void setup() {
  Serial.begin(9600);
  initialize_MPU6050();
}

void loop() {
  read_IMU();
  Serial.print("AngleX: ");
  Serial.print(angleX);
  Serial.print("  AngleY: ");
  Serial.println(angleY);
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
