void printToSerial() {
  Serial.print("DATA:");
  Serial.print(latest.eco2, 2);
  Serial.print(";");
  Serial.print(latest.tvoc, 2);
  Serial.print(";");
  Serial.print(latest.temperature, 2);
  Serial.print(";");
  Serial.print(latest.humidity, 2);
  Serial.print(";");
  Serial.println(latest.pressure, 2);
}
