#define F_CPU 8000000L
#include "Wire.h"
#include "SPI.h"
#include "LowPower.h"
#include "EEPROM.h"

#include "SparkFunCCS811.h"
#include "Adafruit_BMP280.h"
#include "Adafruit_Si7021.h"

#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

// CONSTANTS
#define DEFAULT_DATA_VALUE -1000.0F
#define CYCLES_TO_WAIT_BEFORE_MEASSURE 30

#define OLED_I2C_ADDRESS 0x3C
#define CCS811_ADDR 0x5A
#define DEFAULT_ALTITUDE 425
#define DEFAULT_SLEEP 30000
#define CCS811_DRIVE_MODE_60SEC 0x03

#define CSS_INTERRUPT_PIN 2
#define BTN_INTERRUPT_PIN 3
#define SLEEP 0
#define WAKED_BY_USER 1
#define WAKED_BY_CSS 2
#define WAKED 3

#define BTN_SET 7
#define BTN_UP 6
#define BTN_DOWN 5
#define NO_BTN 99 //dummy value

#define MAIN_SCREEN 0
#define MENU_SCREEN 1
#define SLEEP_SETUP_SCREEN 2
#define ALTITUDE_SETUP_SCREEN 3
#define BASELINE_SETUP_SCREEN 4

#define POSITION_DEFAULT 0
#define MENU_SLEEP_POSITION 0
#define MENU_ALTITUDE_POSITION 1
#define MENU_BASELINE_POSITION 2
#define MENU_EXIT_POSITION 3

#define ALTITUDE_THOUSAND_POSSITION 0
#define ALTITUDE_HUNDRED_POSSITION 1
#define ALTITUDE_TENS_POSSITION 2
#define ALTITUDE_UNITS_POSSITION 3

#define SLEEP_EEPROM_ADDR 0
#define ALTITUDE_EEPROM_ADDR 10
#define BASELINE_EEPROM_ADDR 20

// DEVICES
CCS811 ccs811(CCS811_ADDR);
Adafruit_Si7021 si = Adafruit_Si7021();
Adafruit_BMP280 bmp;
SSD1306AsciiAvrI2c oled;

// VARIABLES
byte counter = 0;
bool measuringTVOC = false;
byte status = WAKED_BY_USER;
unsigned long lastBtnPress = 0; // time in ms
unsigned char lastButton = 0; // number of button
unsigned int sleepAfter;
short altitude;
short altitudeSetting;
unsigned char actualScreen = 0; // actual screen where we are
unsigned char screenPosition = 0; // position in actual screen
float voltage = 0;
unsigned int baseline;

float temperature, pressure, altitudeMeasure; 

struct data
{
  float temperature;
  float humidity;
  float pressure;
  float eco2;
  float tvoc;
};
typedef struct data Data;

Data latest;
