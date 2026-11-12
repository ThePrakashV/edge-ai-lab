
/*
  TCS3200 - 24 Bit RGB Colour Identifier
  Total RGB combinations: 16,777,216

  Board: Arduino UNO
  Sensor: TCS3200
*/

#define S0 2
#define S1 3
#define S2 4
#define S3 5
#define SENSOR_OUT 10

unsigned long redFreq;
unsigned long greenFreq;
unsigned long blueFreq;

int R, G, B;

void setup() {

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  pinMode(SENSOR_OUT, INPUT);

  // Frequency scaling: 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  Serial.begin(9600);

  Serial.println("TCS3200 RGB COLOUR IDENTIFIER");
  Serial.println("24-BIT RGB SYSTEM");
  Serial.println("-----------------------------");
}

unsigned long readColor(int s2, int s3) {

  digitalWrite(S2, s2);
  digitalWrite(S3, s3);

  delay(30);

  unsigned long value = pulseIn(
    SENSOR_OUT, LOW, 100000
  );

  return value;
}

// Convert frequency reading to RGB intensity.
// These limits are initial estimates.
// Calibrate them using actual sensor readings.
int frequencyToRGB(unsigned long frequency,
                   unsigned long minFreq,
                   unsigned long maxFreq) {

  if (frequency == 0) return 0;

  int value = map(
    constrain(frequency, minFreq, maxFreq),
    maxFreq, minFreq,
    0, 255
  );

  return constrain(value, 0, 255);
}

String getColourName(int r, int g, int b) {

  // Basic colour classification

  int maxValue = max(r, max(g, b));
  int minValue = min(r, min(g, b));

  if (maxValue < 35)
    return "BLACK";

  if (minValue > 220)
    return "WHITE";

  if (maxValue - minValue < 20)
    return "GRAY";

  if (r > g && r > b) {
    if (g > b * 1.5) return "ORANGE";
    if (b > g * 1.3) return "PINK";
    return "RED";
  }

  if (g > r && g > b) {
    if (r > b * 1.5) return "YELLOW";
    return "GREEN";
  }

  if (b > r && b > g) {
    if (r > g * 1.3) return "PURPLE";
    return "BLUE";
  }

  if (r > 150 && g > 150 && b < 100)
    return "YELLOW";

  return "MIXED COLOUR";
}

void loop() {

  redFreq = readColor(LOW, LOW);
  greenFreq = readColor(HIGH, HIGH);
  blueFreq = readColor(LOW, HIGH);

  // Initial conversion ranges.
  // Replace after calibration.
  R = frequencyToRGB(redFreq, 20, 500);
  G = frequencyToRGB(greenFreq, 20, 500);
  B = frequencyToRGB(blueFreq, 20, 500);

  String colourName = getColourName(R, G, B);

  // Generate 24-bit HEX colour
  char hexColour[8];

  sprintf(hexColour, "#%02X%02X%02X", R, G, B);

  Serial.println("\n----- COLOUR RESULT -----");

  Serial.print("Red Frequency: ");
  Serial.println(redFreq);

  Serial.print("Green Frequency: ");
  Serial.println(greenFreq);

  Serial.print("Blue Frequency: ");
  Serial.println(blueFreq);

  Serial.println("-------------------------");

  Serial.print("RGB VALUE: (");
  Serial.print(R);
  Serial.print(", ");
  Serial.print(G);
  Serial.print(", ");
  Serial.print(B);
  Serial.println(")");

  Serial.print("HEX CODE: ");
  Serial.println(hexColour);

  Serial.print("COLOUR NAME: ");
  Serial.println(colourName);

  Serial.println("-------------------------");

  delay(500);
}

