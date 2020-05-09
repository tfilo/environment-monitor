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

void printSensorError()
{
  uint8_t error = ccs811.getErrorRegister();
  if (error == 0xFF) //comm error
  {
    Serial.println("Failed to get ERROR_ID register.");
  }
  else
  {
    Serial.print("Error: ");
    if (error & 1 << 5)
      Serial.print("HeaterSupply");
    if (error & 1 << 4)
      Serial.print("HeaterFault");
    if (error & 1 << 3)
      Serial.print("MaxResistance");
    if (error & 1 << 2)
      Serial.print("MeasModeInvalid");
    if (error & 1 << 1)
      Serial.print("ReadRegInvalid");
    if (error & 1 << 0)
      Serial.print("MsgInvalid");
    Serial.println();
  }
}
