#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

// LCD setup (I2C address: 0x27)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin Definitions
#define GAS_SENSOR_PIN A2
#define DHT_PIN A0
#define RELAY_GAS_PIN 5    // Relay for gas sensor
#define RELAY_TEMP_PIN 7   // Relay for temperature

// DHT Sensor type
#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);

// Thresholds
const int GAS_THRESHOLD_HIGH = 400;    // High gas sensor threshold
const int GAS_THRESHOLD_LOW = 100;     // Low gas sensor threshold
const float TEMP_THRESHOLD = 35.0;     // Temperature threshold in Celsius
const float HUMIDITY_THRESHOLD = 70.0; // Humidity threshold in percentage

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  Serial.println("System Initializing...");

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("System Init...");
  
  // DHT Sensor Initialization
  dht.begin();
  
  // Relay setup
  pinMode(RELAY_GAS_PIN, OUTPUT);
  pinMode(RELAY_TEMP_PIN, OUTPUT);
  digitalWrite(RELAY_GAS_PIN, LOW); // Start with relay for gas OFF
  digitalWrite(RELAY_TEMP_PIN, LOW); // Start with relay for temperature OFF

  delay(2000); // Initial delay for setup
  lcd.clear();
  Serial.println("Initialization Complete.");
}

void loop() {
  // Read sensors
  int gasValue = analogRead(GAS_SENSOR_PIN);
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Handle Sensor Errors
  if (isnan(temperature) || isnan(humidity)) {
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!");
    Serial.println("DHT11 Sensor Error!");
    digitalWrite(RELAY_GAS_PIN, LOW);  // Turn off gas relay in case of error
    digitalWrite(RELAY_TEMP_PIN, LOW); // Turn off temperature relay in case of error
    delay(2000);
    return;
  }

  // Display readings on Serial Monitor
  Serial.print("Gas Value: ");
  Serial.println(gasValue);
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Display readings on I2C LCD
  lcd.setCursor(0, 0);
  lcd.print("Gas: ");
  lcd.print(gasValue);

  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(temperature);
  lcd.print("C H:");
  lcd.print(humidity);

  // Relay Activation for Gas Sensor
  if (gasValue > GAS_THRESHOLD_HIGH || gasValue < GAS_THRESHOLD_LOW) {
    lcd.setCursor(0, 0);
    lcd.print("Gas Alert!      "); // Clear remaining chars
    digitalWrite(RELAY_GAS_PIN, HIGH); // Turn on gas relay
    Serial.println("Alert: Gas Abnormal! Gas Relay ON.");
  } else {
    digitalWrite(RELAY_GAS_PIN, LOW); // Turn off gas relay
    Serial.println("Status: Gas Normal. Gas Relay OFF.");
  }

  // Relay Activation for Temperature
  if (temperature > TEMP_THRESHOLD) {
    lcd.setCursor(0, 1);
    lcd.print("High Temp!      "); // Clear remaining chars
    digitalWrite(RELAY_TEMP_PIN, HIGH); // Turn on temperature relay
    Serial.println("Alert: High Temperature! Temp Relay ON.");
  } else {
    digitalWrite(RELAY_TEMP_PIN, LOW); // Turn off temperature relay
    Serial.println("Status: Temperature Normal. Temp Relay OFF.");
  }

  // Add a delay for refresh
  delay(1000); // Refresh every second
}
