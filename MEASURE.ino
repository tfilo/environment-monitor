void takeMeasurements() {
  if (ccs.available()) {
    bmp.takeForcedMeasurement();
    if (si.readTemperature() < bmp.readTemperature()) { // read lowest temperature
      latest.temperature = si.readTemperature();
    } else {
      latest.temperature = bmp.readTemperature();
    }
    latest.humidity = si.readHumidity();
    latest.pressure = bmp.seaLevelForAltitude(altitude, bmp.readPressure() / 100.0F);
    if (measuringTVOC && !ccs.readData()) { // start measuring after first loop (cca 20min) of counter because sensor neet time to stabilise
      latest.eco2 = ccs.geteCO2();
      latest.tvoc = ccs.getTVOC();
    }
    printToSerial();
  }
}

void readBattery() {
  ADCSRA |= (1 << ADEN); // wake up ADC
  delay(100);
  int voltageValue = analogRead(A0);
  voltage = (voltageValue * (3.3 / 1023.0)) * 2.0;
  ADCSRA = 0; // put ADC to sleep, save around 0.250mA
}
