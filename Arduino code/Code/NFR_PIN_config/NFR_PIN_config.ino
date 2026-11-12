// ---------- NRF24L01 pins (software SPI) ----------
#define NRF_CE    2
#define NRF_CSN   13
#define NRF_SCK   A0
#define NRF_MOSI  A1
#define NRF_MISO  A2

// ---------- Settings (must match transmitter) ----------
const byte address[5] = {'0', '0', '0', '0', '1'};   // "00001"
const uint8_t NRF_CHANNEL  = 108;
const uint8_t PAYLOAD_SIZE = 32;

// ---------- NRF24 registers ----------
#define REG_CONFIG      0x00
#define REG_EN_AA       0x01
#define REG_EN_RXADDR   0x02
#define REG_SETUP_AW    0x03
#define REG_RF_CH       0x05
#define REG_RF_SETUP    0x06
#define REG_STATUS      0x07
#define REG_RX_ADDR_P1  0x0B
#define REG_RX_PW_P1    0x12
#define REG_FIFO_STATUS 0x17
#define REG_DYNPD       0x1C
#define REG_FEATURE     0x1D

#define CMD_W_REGISTER   0x20
#define CMD_R_RX_PAYLOAD 0x61
#define CMD_FLUSH_RX     0xE2

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
  ANGLEX_DATA,
  ANGLEY_DATA,
  TOTAL_DATA
};

int data[TOTAL_DATA];

unsigned long lastReceived = 0;
unsigned long lastPrint = 0;
unsigned long packetCount = 0;

// ================= Software SPI =================
uint8_t spiByte(uint8_t b) {
  uint8_t r = 0;
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(NRF_MOSI, (b & 0x80) ? HIGH : LOW);
    b <<= 1;
    digitalWrite(NRF_SCK, HIGH);
    r <<= 1;
    if (digitalRead(NRF_MISO)) r |= 1;
    digitalWrite(NRF_SCK, LOW);
  }
  return r;
}

uint8_t readReg(uint8_t reg) {
  digitalWrite(NRF_CSN, LOW);
  spiByte(reg);
  uint8_t v = spiByte(0xFF);
  digitalWrite(NRF_CSN, HIGH);
  return v;
}

void writeReg(uint8_t reg, uint8_t val) {
  digitalWrite(NRF_CSN, LOW);
  spiByte(CMD_W_REGISTER | reg);
  spiByte(val);
  digitalWrite(NRF_CSN, HIGH);
}

void writeRegBuf(uint8_t reg, const uint8_t *buf, uint8_t len) {
  digitalWrite(NRF_CSN, LOW);
  spiByte(CMD_W_REGISTER | reg);
  for (uint8_t i = 0; i < len; i++) spiByte(buf[i]);
  digitalWrite(NRF_CSN, HIGH);
}

void command(uint8_t cmd) {
  digitalWrite(NRF_CSN, LOW);
  spiByte(cmd);
  digitalWrite(NRF_CSN, HIGH);
}

// ================= NRF24 driver (receiver only) =================
bool nrfBegin() {
  pinMode(NRF_CE, OUTPUT);
  pinMode(NRF_CSN, OUTPUT);
  pinMode(NRF_SCK, OUTPUT);
  pinMode(NRF_MOSI, OUTPUT);
  pinMode(NRF_MISO, INPUT);

  digitalWrite(NRF_CE, LOW);
  digitalWrite(NRF_CSN, HIGH);
  digitalWrite(NRF_SCK, LOW);
  delay(100);

  writeReg(REG_SETUP_AW, 0x03);
  writeReg(REG_RF_CH, NRF_CHANNEL);
  if (readReg(REG_SETUP_AW) != 0x03 || readReg(REG_RF_CH) != NRF_CHANNEL) {
    return false;
  }

  writeReg(REG_CONFIG, 0x0C);
  writeReg(REG_EN_AA, 0x3F);
  writeReg(REG_EN_RXADDR, 0x02);
  writeReg(REG_RF_SETUP, 0x22);        // 250KBPS
  writeReg(REG_DYNPD, 0x00);
  writeReg(REG_FEATURE, 0x00);
  writeRegBuf(REG_RX_ADDR_P1, address, 5);
  writeReg(REG_RX_PW_P1, PAYLOAD_SIZE);

  command(CMD_FLUSH_RX);
  writeReg(REG_STATUS, 0x70);

  writeReg(REG_CONFIG, 0x0F);          // power up, RX mode
  delay(5);
  digitalWrite(NRF_CE, HIGH);
  delay(2);
  return true;
}

bool nrfAvailable() {
  return !(readReg(REG_FIFO_STATUS) & 0x01);
}

void nrfRead(void *buf, uint8_t len) {
  uint8_t *p = (uint8_t *)buf;
  digitalWrite(NRF_CSN, LOW);
  spiByte(CMD_R_RX_PAYLOAD);
  for (uint8_t i = 0; i < PAYLOAD_SIZE; i++) {
    uint8_t b = spiByte(0xFF);
    if (i < len) p[i] = b;
  }
  digitalWrite(NRF_CSN, HIGH);
  writeReg(REG_STATUS, 0x40);
}

// ================= Setup =================
void setup() {
  Serial.begin(115200);
  Serial.println(F("NRF24L01 Receiver Starting..."));

  if (!nrfBegin()) {
    Serial.println(F("NRF NOT DETECTED!"));
    Serial.print(F("STATUS = 0x"));
    Serial.println(readReg(REG_STATUS), HEX);
    while (1) {}
  }

  Serial.println(F("NRF24 DETECTED!"));
  Serial.println(F("Waiting for transmitter data..."));
}

// ================= Loop =================
void loop() {

  if (nrfAvailable()) {
    nrfRead(data, sizeof(data));
    lastReceived = millis();
    packetCount++;
  }

  if (millis() - lastPrint >= 500) {
    lastPrint = millis();

    if (lastReceived == 0) {
      Serial.println(F("No data received yet (check transmitter: address, channel 108, 250KBPS)"));
      return;
    }

    if (millis() - lastReceived > 500) {
      Serial.println(F("SIGNAL LOST"));
      return;
    }

    Serial.println(F("----- RECEIVED DATA -----"));
    for (int i = 0; i < TOTAL_DATA; i++) {
      Serial.print(F("data["));
      Serial.print(i);
      Serial.print(F("] = "));
      Serial.println(data[i]);
    }
    Serial.print(F("Packets: "));
    Serial.println(packetCount);
    Serial.println(F("-------------------------"));
  }
}
