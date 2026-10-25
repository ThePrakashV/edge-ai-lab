const int ir = 3;
int ir_value;
void setup() {
  pinMode(ir, INPUT);
  Serial.begin(9600);
}

void loop() {
  ir_value = digitalRead(ir);

  if (ir_value == LOW) {
    Serial.println("Ir is Detected");
  }
  else{
    Serial.println("Ir is Not Detected");
  }
}
