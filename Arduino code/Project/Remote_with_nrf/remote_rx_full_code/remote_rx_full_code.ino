
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);  // CE, CSN
const byte address[6] = "00001";

// Data indexes must match TX
enum DataIndex {
  BUTTON1_DATA = 0,
  BUTTON2_DATA,
  BUTTON3_DATA,
  BUTTON4_DATA,

  TOGGLE1_DATA,
  TOGGLE2_DATA,

  LEFT_JOYSTICK_X_DATA,
  LEFT_JOYSTICK_Y_DATA,

  RIGHT_JOYSTICK_X_DATA,
  RIGHT_JOYSTICK_Y_DATA,

  RIGHT_POT_DATA,
  LEFT_POT_DATA,

  anglex,
  angley,

  TOTAL_DATA
};

// Receive all 14 values
int data[TOTAL_DATA];

const int buzzer = 10;

void setup() {
  Serial.begin(9600);

  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);

  if (!radio.begin()) {
    Serial.println("NRF24L01 not detected!");
    digitalWrite(buzzer, HIGH);
    while (1) {}
  }

  digitalWrite(buzzer, LOW);

  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(108);
  radio.setAutoAck(true);
  radio.startListening();

  Serial.println("RX Ready - Waiting for data");
}

void loop() {
  if (radio.available()) {

    radio.read(data, sizeof(data));

    Serial.println("======= RECEIVED DATA =======");

    // Print all array values
    for (int i = 0; i < TOTAL_DATA; i++) {
      Serial.print("data[");
      Serial.print(i);
      Serial.print("] = ");
      Serial.println(data[i]);
    }

    Serial.println("-----------------------------");

    // Read individual values
    int button1 = data[BUTTON1_DATA];
    int button2 = data[BUTTON2_DATA];

    int toggle1 = data[TOGGLE1_DATA];
    int toggle2 = data[TOGGLE2_DATA];

    int leftJoystickX = data[LEFT_JOYSTICK_X_DATA];
    int leftJoystickY = data[LEFT_JOYSTICK_Y_DATA];

    int rightJoystickX = data[RIGHT_JOYSTICK_X_DATA];
    int rightJoystickY = data[RIGHT_JOYSTICK_Y_DATA];

    int rightPot = data[RIGHT_POT_DATA];
    int leftPot = data[LEFT_POT_DATA];

    int angleX = data[anglex];
    int angleY = data[angley];

    Serial.print("MPU Angle X: ");
    Serial.println(angleX);

    Serial.print("MPU Angle Y: ");
    Serial.println(angleY);

    Serial.println("=============================");
  }
}
