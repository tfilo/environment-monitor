void setup() {
  latest.eco2 = DEFAULT_DATA_VALUE;
  latest.tvoc = DEFAULT_DATA_VALUE;
  latest.temperature = DEFAULT_DATA_VALUE;
  latest.humidity = DEFAULT_DATA_VALUE;
  latest.pressure = DEFAULT_DATA_VALUE;

  Serial.begin(115200);

  initOled();

  readBattery();

  CCS811Core::CCS811_Status_e returnCode = ccs811.beginWithStatus();
  oled.print("INIT: ");
  oled.println(ccs811.statusString(returnCode));
  returnCode = ccs811.setDriveMode(CCS811_DRIVE_MODE_60SEC);
  oled.print("MODE: ");
  oled.println(ccs811.statusString(returnCode));
  returnCode = ccs811.enableInterrupts();
  oled.print("INTE: ");
  oled.println(ccs811.statusString(returnCode));

  if (!si.begin()) {
    oled.println("Si7021 failed!");
    while (1);
  }
  oled.println("Si7021 ok");

  if (!bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID)) {
    oled.println("BMP280 failed!");
    while (1);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_4000); /* Standby time. */

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

  unsigned int baselineCheck;
  EEPROM.get(BASELINE_EEPROM_ADDR, baseline);
  EEPROM.get(BASELINE_EEPROM_ADDR + 2, baselineCheck);

  if ((65535 ^ baseline) != baselineCheck) {
    baseline = 0;
  }
  
}
