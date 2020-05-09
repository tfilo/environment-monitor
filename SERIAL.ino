void printToSerial() {
  Serial.print("eco2=");
  Serial.print(latest.eco2, 2);
  Serial.print(";tvoc=");
  Serial.print(latest.tvoc, 2);
  Serial.print(";temperature=");
  Serial.print(latest.temperature, 2);
  Serial.print(";humidity=");
  Serial.print(latest.humidity, 2);
  Serial.print(";pressure=");
  Serial.println(latest.pressure, 2);
}
