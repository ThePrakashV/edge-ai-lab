#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
const int r =3;
const int b = 2;
RF24 radio(9, 10); 
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  radio.begin();  
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  pinMode(r,OUTPUT);
  pinMode(b,OUTPUT);
  digitalWrite(b,HIGH);
}

void loop() {
  if (radio.available()) {
    char text[32] = " ";
    
    radio.read(&text, sizeof(text));
    Serial.println("RECEIVED DATA=");   
    Serial.println(text);
    String Text = text;
    if(Text =="ROBOTICS"){
      Serial.println("okay");
      digitalWrite(r,HIGH);
      digitalWrite(b,LOW);
      delay(2000);  
      digitalWrite(r,LOW);
      digitalWrite(b,HIGH); 
      delay(2000);}
  }
}