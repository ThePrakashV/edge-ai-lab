#include <IRremote.hpp>

#define IR_PIN 2

void setup()
{
  Serial.begin(9600);

  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);

  Serial.println("IR Receiver Ready");
}

void loop()
{
  if (IrReceiver.decode())
  {
    Serial.println("\n----- IR DATA -----");

    Serial.print("Protocol: ");
    Serial.println(getProtocolString(IrReceiver.decodedIRData.protocol));

    Serial.print("Address: ");
    Serial.println(IrReceiver.decodedIRData.address, HEX);

    Serial.print("Command: ");
    Serial.println(IrReceiver.decodedIRData.command, HEX);

    Serial.print("Raw Data: ");
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);

    Serial.print("Bits: ");
    Serial.println(IrReceiver.decodedIRData.numberOfBits);

    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT)
    {
      Serial.println("Repeat Signal");
    }

    Serial.println("-------------------");

    IrReceiver.resume();
  }
}
