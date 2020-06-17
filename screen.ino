void mainScreen() {
  int row = 1;

  oled.setRow(row++);
  oled.setCol(1);
  if (measuringTVOC) {
    oled.print("eCO2:    ");
  } else {
    oled.print("! eCO2:  ");  
  }
  if (latest.eco2 != DEFAULT_DATA_VALUE) {
    oled.print(latest.eco2);
  } else {
    oled.print("--");  
  }
  oled.print("ppm");
  oled.clearToEOL ();

  oled.setRow(row++);
  oled.setCol(1);
  if (measuringTVOC) {
    oled.print("TVOC:    ");
  } else {
    oled.print("! TVOC:  ");
  }
  if (latest.tvoc != DEFAULT_DATA_VALUE) {
    oled.print(latest.tvoc);
  } else {
    oled.print("--");  
  }
  oled.print("ppb");
  oled.clearToEOL ();

  oled.setRow(row++);
  oled.setCol(1);
  oled.print("Teplota: "); // Temperature
  if (latest.temperature != DEFAULT_DATA_VALUE) {
    oled.print(latest.temperature, 1);
  } else {
    oled.print("--");  
  }
  oled.print("C");
  oled.clearToEOL ();

  oled.setRow(row++);
  oled.setCol(1);
  oled.print("Vlhkost: "); // Humidity
  if (latest.humidity != DEFAULT_DATA_VALUE) {
    oled.print(latest.humidity);
  } else {
    oled.print("--");  
  }
  oled.print("%");
  oled.clearToEOL ();

  oled.setRow(row++);
  oled.setCol(1);
  oled.print("Tlak:    "); // Pressure
  if (latest.pressure != DEFAULT_DATA_VALUE) {
    oled.print(latest.pressure, 1);
  } else {
    oled.print("--");  
  }
  oled.print("hPa");
  oled.clearToEOL ();

  if (voltage > 1) { // if connected
    oled.setRow(row++);
    oled.setCol(1);
    oled.print("Bateria: "); // Battery
    oled.print(voltage,1);
    oled.print("V");
    oled.clearToEOL ();
  }  
}

void menuScreen() {
  byte row = 1;
  oled.setRow(row++);
  oled.setCol(1);
  oled.print("MENU:");
  oled.clearToEOL ();

  oled.setRow(row++);
  oled.setCol(1);
  if (screenPosition == MENU_SLEEP_POSITION) {
    oled.print("*");
  } else {
    oled.print(" ");
  }
  oled.print("Usporny rezim"); // Sleep mode
  oled.clearToEOL ();

  oled.setRow(row++);
  oled.setCol(1);
  if (screenPosition == MENU_ALTITUDE_POSITION) {
    oled.print("*");
  } else {
    oled.print(" ");
  }
  oled.print("Nadmorska vyska"); // Altitude
  oled.clearToEOL ();

  oled.setRow(row++);
  oled.setCol(1);
  if (screenPosition == MENU_BASELINE_POSITION) {
    oled.print("*");
  } else {
    oled.print(" ");
  }
  oled.print("CCS811 Baseline");
  oled.clearToEOL ();

  oled.setRow(row++);
  oled.setCol(1);
  if (screenPosition == MENU_EXIT_POSITION) {
    oled.print("*");
  } else {
    oled.print(" ");
  }
  oled.print("Exit");
  oled.clearToEOL ();
}

void sleepScreen() {
  oled.setRow(1);
  oled.setCol(1);
  oled.print("Usporny rezim"); // Sleep mode
  oled.clearToEOL ();

  oled.setRow(3);
  oled.setCol(1);
  oled.print(sleepAfter / 1000, DEC);
  oled.print("s");
  oled.clearToEOL ();
}

void altitudeScreen() {
  char altitudeString[5];
  sprintf(altitudeString, "%04d", altitudeSetting);

  oled.setRow(1);
  oled.setCol(1);
  oled.print("Nadmorska vyska"); // Altitude
  oled.clearToEOL ();

  oled.setRow(3);
  oled.setCol(1);
  oled.print(altitudeString);
  oled.print("m.n.m."); // m a.s.l.
  oled.clearToEOL ();

  oled.setRow(4);
  oled.setCol(1);
  switch (screenPosition) {
    case ALTITUDE_THOUSAND_POSSITION:
      oled.print("^");
      break;
    case ALTITUDE_HUNDRED_POSSITION:
      oled.print(" ^");
      break;
    case ALTITUDE_TENS_POSSITION:
      oled.print("  ^");
      break;
    case ALTITUDE_UNITS_POSSITION:
      oled.print("   ^");
      break;
  }
  oled.clearToEOL ();
}

void baselineScreen() {
  int row = 1;
  oled.setRow(row++);
  oled.setCol(1);
  oled.print("Baseline");
  oled.clearToEOL ();

  oled.setRow(row++);
  oled.setCol(1);
  oled.print("H:uloz, D:obnov"); // Up btn to save, Down btn to restore
  oled.clearToEOL ();
  
  oled.setRow(row++);
  oled.setCol(1);
  oled.print("Aktualna hodnota:"); // Actual value
  oled.clearToEOL ();
  
  oled.setRow(row++);
  oled.setCol(1);
  unsigned int actualBaseline = ccs811.getBaseline();     
  oled.print(actualBaseline, DEC);
  oled.clearToEOL ();

  oled.setRow(row++);
  oled.setCol(1);
  oled.print("Ulozena hodnota:"); // Saved value
  oled.clearToEOL ();

  oled.setRow(row++);
  oled.setCol(1);     
  oled.print(baseline, DEC);
  oled.clearToEOL ();
}
