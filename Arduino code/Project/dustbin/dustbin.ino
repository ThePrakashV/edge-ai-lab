#include <CheapStepper.h>
#include <Servo.h>

#define IR_PIN 5
#define PROXI_PIN 6
#define BUZZER_PIN 12
#define SOIL_PIN A0

Servo servo1;
CheapStepper stepper(8, 9, 10, 11);

void setup() {
  Serial.begin(9600);

  pinMode(IR_PIN, INPUT);
  pinMode(PROXI_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  servo1.attach(7);
  servo1.write(70);  // Ready position

  stepper.setRpm(17); // Stepper speed

  Serial.println("Smart Waste Segregator Initialized");
}

void loop() {
  int irValue = digitalRead(IR_PIN);
  int proxiValue = digitalRead(PROXI_PIN);

  if (irValue == LOW) { // Object detected
    Serial.println("Object Detected");
    tone(BUZZER_PIN, 1000, 300);
    delay(500); // Let sensors settle

    // Read and average soil moisture
    int soilTotal = 0;
    for (int i = 0; i < 3; i++) {
      int rawSoil = analogRead(SOIL_PIN);
      rawSoil = constrain(rawSoil, 485, 1023);
      soilTotal += map(rawSoil, 485, 1023, 100, 0);
      delay(75);
    }
    int avgSoil = soilTotal / 3;

    Serial.print("Soil Moisture: ");
    Serial.print(avgSoil);
    Serial.println("%");

    // Waste Classification and Handling
    if (proxiValue == HIGH) {
      // Metal Waste
      Serial.println("Metal Waste Detected - Rotating to 0°");
      // No rotation needed for metal bin (default)
    } else if (avgSoil >= 20) {
      // Wet Waste
      Serial.println("Wet Waste Detected - Rotating to 180°");
      stepper.moveDegreesCW(150);
      delay(500);
    } else {
      // Dry Waste
      Serial.println("Dry Waste Detected - Rotating to 360° (0°)");
      stepper.moveDegreesCW(270);
      delay(500);
    }

    // Servo drops item
    servo1.write(180);
    delay(1000);
    servo1.write(70);
    delay(1000);

    // Return to home position (rotate back)
    if (proxiValue == HIGH) {
      // No movement
    } else if (avgSoil >= 20) {
      stepper.moveDegreesCCW(150);
      delay(500);
    } else {
      stepper.moveDegreesCCW(270);
      delay(500);
    }
  }
}
