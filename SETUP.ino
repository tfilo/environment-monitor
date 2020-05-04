void setup() {
  latest.eco2 = DEFAULT_DATA_VALUE;
  latest.tvoc = DEFAULT_DATA_VALUE;
  latest.temperature = DEFAULT_DATA_VALUE;
  latest.humidity = DEFAULT_DATA_VALUE;
  latest.pressure = DEFAULT_DATA_VALUE;

  Serial.begin(115200);

  initOled();

  readBattery();

  if (!ccs.begin()) {
    Serial.println("CCS811 failed!");
    oled.println("CCS811 failed!");
    while (1);
  }
  ccs.enableInterrupt();
  oled.println("CCS811 ok");

  ccs.setDriveMode(CCS811_DRIVE_MODE_60SEC);
  oled.println("CCS811 drive mode ok");

  if (!si.begin()) {
    Serial.println("Si7021 failed!");
    oled.println("Si7021 failed!");
    while (1);
  }
  oled.println("Si7021 ok");

  if (!bmp280.initialize()) {
    Serial.println("BMP280 failed!");
    oled.println("BMP280 failed!");
    while (1);
  }

  bmp280.setEnabled(0);
  oled.println("BMP280 ok");

  delay(3000);
  oled.clear();

  setupButtons();
  attachInterrupt(digitalPinToInterrupt(CSS_INTERRUPT_PIN), cssInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN_INTERRUPT_PIN), btnInterrupt, FALLING);

  loadEEPROMvariables();
}

void initOled() {
  oled.begin(&Adafruit128x64, OLED_I2C_ADDRESS);
  oled.setFont(Adafruit5x7);
  oled.clear();
}

void setupButtons() {
  pinMode(BTN_SET, INPUT_PULLUP);
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_INTERRUPT_PIN, INPUT_PULLUP);
}

void loadEEPROMvariables() {
  byte sleepByte = EEPROM.read(SLEEP_EEPROM_ADDR);
  byte sleepByteXor = EEPROM.read(SLEEP_EEPROM_ADDR + 1);

  if ((255 ^ sleepByte) != sleepByteXor) { // if checksum (xor) value is not valid, set default
    sleepByte = 2;
  }

  switch (sleepByte) {
    case 1:
      sleepAfter = 15000;
      break;
    case 2:
      sleepAfter = 30000;
      break;
    case 3:
      sleepAfter = 45000;
      break;
    case 4:
      sleepAfter = 60000;
      break;
    default:
      sleepAfter = DEFAULT_SLEEP;
      break;
  }

  short altitudeCheck;
  EEPROM.get(ALTITUDE_EEPROM_ADDR, altitude);
  EEPROM.get(ALTITUDE_EEPROM_ADDR + 2, altitudeCheck);

  if ((32767 ^ altitude) != altitudeCheck) {
    altitude = DEFAULT_ALTITUDE;
  }
}
