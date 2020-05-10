void takeMeasurements() { 
  latest.humidity = si.readHumidity();
  latest.temperature = bmp.readTemperature() - 1.5f;
  latest.pressure = bmp.seaLevelForAltitude(altitude, bmp.readPressure() / 100.0F);
  
  if (ccs811.dataAvailable()) {
    ccs811.readAlgorithmResults();
    latest.eco2 = ccs811.getCO2();
    latest.tvoc = ccs811.getTVOC();
    ccs811.setEnvironmentalData(latest.humidity, latest.temperature);
  } else if (ccs811.checkForStatusError()) {
    printSensorError();  
  }

  printToSerial();
}

void readBattery() {
  ADCSRA |= (1 << ADEN); // wake up ADC
  delay(100);
  int voltageValue = analogRead(A0);
  voltage = (voltageValue * (3.3 / 1023.0)) * 2.0;
  ADCSRA = 0; // put ADC to sleep, save around 0.250mA
  
}
