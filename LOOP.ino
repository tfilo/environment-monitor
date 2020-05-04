void loop() {
  unsigned char btn = lastButton;
  lastButton = NO_BTN;
  
  if (status == WAKED_BY_CSS || status == WAKED) {
    takeMeasurements();
    status = WAKED_BY_USER;
  }

  if (status == WAKED_BY_USER) {
    switch (btn) {
      case BTN_SET:
        handleSetButton();
        break;
      case BTN_UP:
        handleUpButton();
        break;
      case BTN_DOWN:
        handleDownButton();
        break;
    }
    switch (actualScreen) {
      case MAIN_SCREEN:
        mainScreen();
        break;
      case MENU_SCREEN:
        menuScreen();
        break;
      case SLEEP_SETUP_SCREEN:
        sleepScreen();
        break;
      case ALTITUDE_SETUP_SCREEN:
        altitudeScreen();
        break;
    }

  }

  if (counter >= 60) { // cca every hour
    counter = 0;
    Serial.print("Updating enviromental data to ");
    Serial.print((uint8_t)latest.humidity);
    Serial.print("%, ");
    Serial.print((double)latest.temperature);
    Serial.println("C");
    ccs.setEnvironmentalData((uint8_t)latest.humidity, (double)latest.temperature);
    measuringTVOC = true;
    readBattery();
  } else {
    counter++;
  }
  
  delay(1);
  
  if ((millis() - lastBtnPress) < sleepAfter) {
    LowPower.powerDown(SLEEP_120MS, ADC_OFF, BOD_OFF);
  } else {
    putToSleep();
  }
}
