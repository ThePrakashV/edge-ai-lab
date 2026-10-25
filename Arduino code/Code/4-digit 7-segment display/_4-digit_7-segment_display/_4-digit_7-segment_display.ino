#define CLK 5
#define DIO 4

byte digits[] = {
  0x3F,  // 0
  0x06,  // 1
  0x5B,  // 2
  0x4F,  // 3
  0x66,  // 4
  0x6D,  // 5
  0x7D,  // 6
  0x07,  // 7
  0x7F,  // 8
  0x6F   // 9
};

void startSignal() {
  pinMode(DIO, OUTPUT);

  digitalWrite(DIO, HIGH);
  digitalWrite(CLK, HIGH);
  digitalWrite(DIO, LOW);
}

void stopSignal() {
  digitalWrite(CLK, LOW);
  digitalWrite(DIO, LOW);
  digitalWrite(CLK, HIGH);
  digitalWrite(DIO, HIGH);
}

void writeByte(byte data) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(CLK, LOW);

    digitalWrite(DIO, data & 0x01);

    data >>= 1;

    delayMicroseconds(5);

    digitalWrite(CLK, HIGH);

    delayMicroseconds(5);
  }

  // ACK
  digitalWrite(CLK, LOW);
  pinMode(DIO, INPUT);

  digitalWrite(CLK, HIGH);
  delayMicroseconds(5);

  digitalWrite(CLK, LOW);

  pinMode(DIO, OUTPUT);
}

void displayNumber(int num) {

  byte displayData[4];

  displayData[0] = digits[(num / 1000) % 10];
  displayData[1] = digits[(num / 100) % 10];
  displayData[2] = digits[(num / 10) % 10];
  displayData[3] = digits[num % 10];

  // Data command
  startSignal();
  writeByte(0x40);
  stopSignal();

  // Address command
  startSignal();
  writeByte(0xC0);

  for (int i = 0; i < 4; i++) {
    writeByte(displayData[i]);
  }

  stopSignal();

  // Display ON + brightness
  startSignal();
  writeByte(0x88 | 7);
  stopSignal();
}

int count = 0;

void setup() {
  pinMode(CLK, OUTPUT);
  pinMode(DIO, OUTPUT);

  digitalWrite(CLK, HIGH);
  digitalWrite(DIO, HIGH);
}

void loop() {

  displayNumber(count);

  count++;

  if (count > 9999) {
    count = 0;
  }

  delay(1000);
}
