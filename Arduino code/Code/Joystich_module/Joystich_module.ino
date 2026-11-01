const int X_PIN = A0;
const int Y_PIN = A1;
const int SW_PIN = 2;

int xValue;
int yValue;
int button;

void setup()
{
  Serial.begin(9600);

  pinMode(SW_PIN, INPUT_PULLUP);
}

void loop()
{
  xValue = analogRead(X_PIN);
  yValue = analogRead(Y_PIN);
  button = digitalRead(SW_PIN);

  Serial.print("X: ");
  Serial.print(xValue);

  Serial.print(" | Y: ");
  Serial.print(yValue);

  Serial.print(" | Button: ");
  Serial.println(button);

  delay(200);
}
