void loop() {
  unsigned char btn = lastButton;
  lastButton = NO_BTN;
  
  if (status == WAKED_BY_CSS || status == WAKED) {
    if (counter < 25) { // 25 minute
      counter++;
    }
    takeMeasurements();
    if (status == WAKED) {
      status = WAKED_BY_USER;
    }
    if (status == WAKED_BY_USER) {
      readBattery(); // read battery only when waked by user  
    }
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
      case BASELINE_SETUP_SCREEN:
        baselineScreen();
        break;
    }

  }

  if (counter == 25) { // cca 25 minute to warm up sensor
    measuringTVOC = true;
    counter = 255; // just to never run again
    if (baseline > 0) {
      CCS811Core::CCS811_Status_e errorStatus = ccs811.setBaseline(baseline); // restore baseline after reset
      if (errorStatus != CCS811Core::CCS811_Stat_SUCCESS) {
        Serial.print("Error writing baseline: ");
        Serial.println(ccs811.statusString(errorStatus));
      } 
    }
  }
  
  if ((millis() - lastBtnPress) < sleepAfter) {
    LowPower.powerDown(SLEEP_15MS, ADC_OFF, BOD_OFF);
  } else {
    oled.ssd1306WriteCmd(0xAE); // turn off oled
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    if (status == WAKED_BY_USER) {
      initOled();
    }
    lastButton = NO_BTN; // after waking up by any button don't do action
  }
}
