void cssInterrupt() {
  if (status == WAKED_BY_USER) {
    status = WAKED;
  } else {
    status = WAKED_BY_CSS;  
  }
}

void btnInterrupt() {
  if (status == WAKED_BY_CSS) {
    status = WAKED;
  } else {
    status = WAKED_BY_USER;  
  }
  if (millis() - lastBtnPress < 300) { // Debounce 300ms
    return;
  }

  lastBtnPress = millis();

  // test for pressed button
  if (!digitalRead(BTN_SET)) {
    lastButton = BTN_SET;
  }
  if (!digitalRead(BTN_UP)) {
    lastButton = BTN_UP;
  }
  if (!digitalRead(BTN_DOWN)) {
    lastButton = BTN_DOWN;
  }
}
