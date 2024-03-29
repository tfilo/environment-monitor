/*******************
   PROCESS BUTTONS
 *******************/

void handleSetButton() {
  oled.clear();

  if (actualScreen == MAIN_SCREEN) {
    actualScreen = MENU_SCREEN;
    screenPosition = POSITION_DEFAULT;
    return;
  }

  if (actualScreen == MENU_SCREEN) {
    switch (screenPosition) {
      case MENU_SLEEP_POSITION:
        actualScreen = SLEEP_SETUP_SCREEN;
        screenPosition = POSITION_DEFAULT;
        break;
      case MENU_ALTITUDE_POSITION:
        altitudeSetting = altitude;
        actualScreen = ALTITUDE_SETUP_SCREEN;
        screenPosition = POSITION_DEFAULT;
        break;
      case MENU_BASELINE_POSITION:
        actualScreen = BASELINE_SETUP_SCREEN;
        screenPosition = POSITION_DEFAULT;
        break;
      case MENU_HEATER_SI7021_POSITION:
        if (si.isHeaterEnabled()) {
          si.heater(false);
        } else {
          si.setHeatLevel(SI_HEATLEVEL_MEDIUM);
          si.heater(true);
        }
        break;
      case MENU_EXIT_POSITION:
        actualScreen = MAIN_SCREEN;
        screenPosition = POSITION_DEFAULT;
        break;
    }
    return;
  }

  if (actualScreen == SLEEP_SETUP_SCREEN) {
    byte sleepByte;
    switch (sleepAfter) {
      case 15000:
        sleepByte = 1;
        break;
      case 30000:
        sleepByte = 2;
        break;
      case 45000:
        sleepByte = 3;
        break;
      case 60000:
        sleepByte = 4;
        break;
      default:
        sleepByte = 2;
        break;
    }
    EEPROM.write(SLEEP_EEPROM_ADDR, sleepByte);
    EEPROM.write(SLEEP_EEPROM_ADDR + 1, 255 ^ sleepByte); //Sleep XOR value used as checksum.
    actualScreen = MENU_SCREEN;
    screenPosition = POSITION_DEFAULT;
    return;
  }

  if (actualScreen == ALTITUDE_SETUP_SCREEN) {
    switch (screenPosition) {
      case ALTITUDE_THOUSAND_POSSITION:
      case ALTITUDE_HUNDRED_POSSITION:
      case ALTITUDE_TENS_POSSITION:
        screenPosition++;
        break;
      case ALTITUDE_UNITS_POSSITION:
        altitude = altitudeSetting;
        EEPROM.put(ALTITUDE_EEPROM_ADDR, altitude);
        EEPROM.put(ALTITUDE_EEPROM_ADDR + 2, 32767 ^ altitude); // Altitude XOR value used as checksum.
        actualScreen = MENU_SCREEN;
        screenPosition = POSITION_DEFAULT;
        break;
    }
    return;
  }

  if (actualScreen == BASELINE_SETUP_SCREEN) {
    actualScreen = MENU_SCREEN;
    screenPosition = POSITION_DEFAULT;
    return;
  }
}

void handleUpButton() {
  if (actualScreen == MENU_SCREEN) {
    if (screenPosition == POSITION_DEFAULT) {
      screenPosition = MENU_EXIT_POSITION;
    } else {
      screenPosition--;
    }
    return;
  }

  if (actualScreen == SLEEP_SETUP_SCREEN) {
    if (sleepAfter >= 60000) {
      sleepAfter = 15000;
    } else {
      sleepAfter += 15000;
    }
    return;
  }

  if (actualScreen == ALTITUDE_SETUP_SCREEN) {
    short step = 1;
    byte n = 4;
    switch (screenPosition) {
      case ALTITUDE_THOUSAND_POSSITION:
        step = 1000;
        break;
      case ALTITUDE_HUNDRED_POSSITION:
        step = 100;
        break;
      case ALTITUDE_TENS_POSSITION:
        step = 10;
        break;
      case ALTITUDE_UNITS_POSSITION:
        step = 1;
        break;
    }
    if (getDigit(altitudeSetting, step) == 9) {
      altitudeSetting -= step * 9;
    } else {
      altitudeSetting+=step;
    }
    return;
  }

  if (actualScreen == BASELINE_SETUP_SCREEN) {
    unsigned int actualBaseline = ccs811.getBaseline();
    EEPROM.put(BASELINE_EEPROM_ADDR, actualBaseline);
    EEPROM.put(BASELINE_EEPROM_ADDR + 2, 65535 ^ actualBaseline); // Baseline XOR value used as checksum
    baseline = actualBaseline;
    return;
  }
}

void handleDownButton() {
  if (actualScreen == MENU_SCREEN) {
    if (screenPosition == MENU_EXIT_POSITION) {
      screenPosition = POSITION_DEFAULT;
    } else {
      screenPosition++;
    }
    return;
  }

  if (actualScreen == SLEEP_SETUP_SCREEN) {
    if (sleepAfter <= 15000) {
      sleepAfter = 60000;
    } else {
      sleepAfter -= 15000;
    }
    return;
  }

  if (actualScreen == ALTITUDE_SETUP_SCREEN) {
    short step = 1;
    byte n = 4;
    switch (screenPosition) {
      case ALTITUDE_THOUSAND_POSSITION:
        step = 1000;
        break;
      case ALTITUDE_HUNDRED_POSSITION:
        step = 100;
        break;
      case ALTITUDE_TENS_POSSITION:
        step = 10;
        break;
      case ALTITUDE_UNITS_POSSITION:
        step = 1;
        break;
    }
    if (getDigit(altitudeSetting, step) == 0) {
      altitudeSetting += step * 9;
    } else {
      altitudeSetting-=step;
    }
    return;
  }

  if (actualScreen == BASELINE_SETUP_SCREEN) {
    CCS811Core::CCS811_Status_e errorStatus = ccs811.setBaseline(baseline);

    if (errorStatus != CCS811Core::CCS811_Stat_SUCCESS) {
      Serial.print("ERROR;writing_baseline;");
      Serial.println(ccs811.statusString(errorStatus));
    }
    return;
  }
}

byte getDigit(int num, int step) { // Get digit on thousands, hundreds, tens, ones position as needed for altitude setting menu.
  return (num / step) % 10;
}
