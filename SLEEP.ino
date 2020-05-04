void wakeUp() {
  lastButton = NO_BTN; // after waking up by any button don't do action assignet to button
  noInterrupts();
  sleep_disable();
  power_all_enable();
  interrupts();
  // analogReference(INTERNAL); // TODO only if makes higher consuption during sleep
  if (status == WAKED_BY_USER) {
    initOled();
  }
}

void putToSleep() {
  noInterrupts();
  oled.ssd1306WriteCmd(0xAE); // turn off oled
  // analogReference(EXTERNAL); // TODO only if makes higher consuption during sleep
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // set powerdown state for ATmega
  ADCSRA = 0; // put ADC to sleep, save around 0.250mA
  power_all_disable(); // put everything other to sleep
  sleep_enable();
  interrupts();
  status = SLEEP;
  sleep_cpu(); // When sleeping current goes down to 0.035mA
  wakeUp();
}
