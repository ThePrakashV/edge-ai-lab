int LDR_Pin = A0;
int LED_Pin = 9;  
int LDR_Value = 0; 
int LED_Brightness = 0; 

void setup() {
  pinMode(LED_Pin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  LDR_Value = analogRead(LDR_Pin); 
  
  
  LED_Brightness = map(LDR_Value, 0, 1023, 255, 0); 
  
  analogWrite(LED_Pin, LED_Brightness); 
  
  
  Serial.print("LDR Value: ");
  Serial.print(LDR_Value);
  Serial.print(" | LED Brightness: ");
  Serial.println(LED_Brightness);

  delay(100);
}
