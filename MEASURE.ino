void takeMeasurements() { 
  latest.humidity = si.readHumidity();
  
  if (ccs.available()) {
    if (measuringTVOC && !ccs.readData()) { // start measuring after first loop (cca 20min) of counter because sensor neet time to stabilise
      latest.eco2 = ccs.geteCO2();
      latest.tvoc = ccs.getTVOC();
    }
  }

  latest.temperature = bmp.readTemperature();
  latest.pressure = bmp.seaLevelForAltitude(altitude, bmp.readPressure() / 100.0F);

  printToSerial();
}

void readBattery() {
  ADCSRA |= (1 << ADEN); // wake up ADC
  delay(100);
  int voltageValue = analogRead(A0);
  voltage = (voltageValue * (3.3 / 1023.0)) * 2.0;
  ADCSRA = 0; // put ADC to sleep, save around 0.250mA
  
}
