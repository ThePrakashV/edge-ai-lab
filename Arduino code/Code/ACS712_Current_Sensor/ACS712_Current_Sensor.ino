const int sensorPin = A0;

float VREF = 5.0;                 // will be auto-measured in setup()
const float SENSITIVITY = 0.100;  // ACS712 20A = 100 mV/A
const int SAMPLES = 200;          // more samples = less noise

float zeroVoltage = 2.5;

// Reads actual Arduino supply voltage using internal 1.1V reference
long readVcc() {
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2);
  ADCSRA |= _BV(ADSC);
  while (bit_is_set(ADCSRA, ADSC));
  long result = ADCL | (ADCH << 8);
  result = 1125300L / result;   // returns Vcc in mV
  return result;
}

void calibrateZero() {
  long total = 0;
  for (int i = 0; i < SAMPLES; i++) {
    total += analogRead(sensorPin);
    delay(2);
  }
  float average = total / (float)SAMPLES;
  zeroVoltage = average * VREF / 1023.0;
}

void setup() {
  Serial.begin(9600);
  delay(1000);  // let sensor settle before calibration

  // Step 1: measure real supply voltage instead of assuming 5.0V
  VREF = readVcc() / 1000.0;

  // Step 2: zero-current calibration using accurate VREF
  calibrateZero();

  Serial.println("ACS712 Current Sensor");
  Serial.print("Measured VREF: ");
  Serial.println(VREF, 3);
  Serial.print("Zero Voltage: ");
  Serial.println(zeroVoltage, 3);
}

void loop() {
  long total = 0;

  for (int i = 0; i < SAMPLES; i++) {
    total += analogRead(sensorPin);
    delay(1);
  }

  float average = total / (float)SAMPLES;
  float voltage = average * VREF / 1023.0;
  float current = (voltage - zeroVoltage) / SENSITIVITY;

  // Noise correction (dead-band)
  if (abs(current) < 0.05) {
    current = 0;
  }

  Serial.print("Voltage: ");
  Serial.print(voltage, 3);
  Serial.print(" V   Current: ");
  Serial.print(current, 3);
  Serial.println(" A");

  delay(500);
}
