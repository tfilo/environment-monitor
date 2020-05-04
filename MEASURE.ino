void takeMeasurements() {
  latest.humidity = si.readHumidity();
  
  if (ccs.available()) {
    if (measuringTVOC && !ccs.readData()) { // start measuring after first loop (cca 20min) of counter because sensor neet time to stabilise
      latest.eco2 = ccs.geteCO2();
      latest.tvoc = ccs.getTVOC();
    }
  }

  bmp280.triggerMeasurement();
  bmp280.awaitMeasurement();
  bmp280.getTemperature(temperature);
  bmp280.getPressure(pressure);
  latest.temperature = temperature;
  latest.pressure = (pressure / 100) / pow(1.0 - (altitude / 44330.0), 5.255);
    
  printToSerial();
}

void readBattery() {
  ADCSRA |= (1 << ADEN); // wake up ADC
  delay(100);
  int voltageValue = analogRead(A0);
  voltage = (voltageValue * (3.3 / 1023.0)) * 2.0;
  ADCSRA = 0; // put ADC to sleep, save around 0.250mA
}
