#include "Wire.h"
#include "SPI.h"
#include "Adafruit_CCS811.h"
#include "Adafruit_Si7021.h"
#include "Adafruit_BMP280.h"

#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

#include "LowPower.h"

#define I2C_ADDRESS 0x3C

SSD1306AsciiAvrI2c oled;
Adafruit_CCS811 ccs;
Adafruit_Si7021 si = Adafruit_Si7021();
Adafruit_BMP280 bmp;

void setup() {  
  Serial.begin(115200);

  if(!ccs.begin()){
    Serial.println("Failed to start CCS811 sensor! Please check your wiring.");
    while(1);
  }

  if (!si.begin()) {
    Serial.println("Did not find Si7021 sensor!");
    while (1);
  }

  if (!bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }
  
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.setFont(Adafruit5x7); 
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,      /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,      /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X2,      /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X8,        /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_4000); /* Standby time. */  
  oled.clear();

  ADCSRA = 0; 
}

int counter = 100;

void loop() {
  
  int row = 1;
  if (ccs.available()) {  
    float temp = ccs.calculateTemperature();
    float tempSI = si.readTemperature();
    float humSI = si.readHumidity();
    float tempBMP = bmp.readTemperature();
    float pressBMP = bmp.readPressure();
    float pressBMPalt = bmp.seaLevelForAltitude(425, pressBMP);

    if (counter >= 100) {
      counter = 0;
      Serial.print("Updating enviromental data to ");
      Serial.print((uint8_t)humSI);
      Serial.print("%, ");
      Serial.print((double)tempSI);
      Serial.println("C");
      ccs.setEnvironmentalData((uint8_t)humSI, (double)tempSI);
    } else {
      counter++;  
    }
    
    if (!ccs.readData()) {
      float eCO2 = ccs.geteCO2();
      float TVOC = ccs.getTVOC();

      oled.setRow(row++);
      oled.setCol(1);
      oled.print("eCO2: ");
      oled.print(eCO2);
      oled.print("ppm"); 
      oled.clearToEOL ();

      oled.setRow(row++);
      oled.setCol(1);
      oled.print("TVOC: ");
      oled.print(TVOC);
      oled.print("ppb"); 
      oled.clearToEOL ();
      
      oled.setRow(row++);
      oled.setCol(1);
      oled.print("Temp SI7021: ");
      oled.print(tempSI);
      oled.print("C"); 
      oled.clearToEOL ();

      oled.setRow(row++);
      oled.setCol(1);
      oled.print("Hum  SI7021: ");
      oled.print(humSI);
      oled.print("%"); 
      oled.clearToEOL ();

      oled.setRow(row++);
      oled.setCol(1);
      oled.print("Temp BMP280: ");
      oled.print(tempBMP);
      oled.print("C"); 
      oled.clearToEOL ();

      oled.setRow(row++);
      oled.setCol(1);
      oled.print("Pres BMP280: ");
      oled.print(pressBMPalt / 100, 1);
      oled.print("hPa"); 
      oled.clearToEOL ();

      Serial.print("eCO2:");
      Serial.print(eCO2);
      Serial.print(" TVOC:");
      Serial.print(TVOC);
      Serial.print(" TEMP_CCS:");
      Serial.print(temp);
      Serial.print(" TEMP_SI:");
      Serial.print(tempSI);
      Serial.print(" HUM_SI:");
      Serial.print(humSI);
      Serial.print(" TEMP_BMP:");
      Serial.print(tempBMP);
      Serial.print(" PRESS_BMP:");
      Serial.println(pressBMP / 100, 1);
    } else {
      Serial.println("ERROR!");
      while(1);
    }
  }
  delay(10); 
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}
