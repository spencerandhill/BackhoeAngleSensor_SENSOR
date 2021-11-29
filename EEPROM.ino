#include <EEPROM.h>
#define EESIZE 512

int hOffset = 0;
int vOffset = 0;

// In case, the sensor was attached in a different direction
bool flipXYAxis = false;

void initEEPROM(void) {
  Serial.println("EEPROM Begin");
  EEPROM.begin(EESIZE); // Just for ESP32 or ESP8266. On regular Arduino's EESIZE is not needed!

  #ifdef FLASHING_FIRST_TIME
    initEepromDefaultOnce();
  #endif

  readOffsetsFromEEPROM();
  readFlipXYFromEEPROM();
  printEEPROMValues();
}

// In case the Offset was never written to EEPROM, initialize it!
void initEepromDefaultOnce() {

  Serial.println("Setting Default Values to EEPROM");
  setOffsetsToEEPROM(0,0);
  setFlipXYToEEPROM(false);

}

void printEEPROMValues(void) {
  Serial.print("hOffset: ");Serial.println(getHOffset());
  Serial.print("vOffset: ");Serial.println(getVOffset());
  Serial.print("XY Axis flipped: ");Serial.println(getFlipXY() ? "Yes" : "No");
}

float getHOffset(void) {
  return hOffset;
}

float getVOffset(void) {
  return vOffset;
}

void readOffsetsFromEEPROM(void) {
  bool vorzeichen;
  uint8_t temp;

  // Read Horizontal Offset Value
  EEPROM.get(EEPROM_ADDRESS_OFFSET_H_negativ, vorzeichen);
  EEPROM.get(EEPROM_ADDRESS_OFFSET_H, temp);
  hOffset = vorzeichen ? temp * -1 : temp;

  EEPROM.get(EEPROM_ADDRESS_OFFSET_V_negativ, vorzeichen);
  EEPROM.get(EEPROM_ADDRESS_OFFSET_V, temp);
  vOffset = vorzeichen ? temp * -1 : temp;
}

int setOffsetsToEEPROM(int h, int v) {
  hOffset = h;
  vOffset = v;
  
  // Store if the value is negativ or not
  EEPROM.write(EEPROM_ADDRESS_OFFSET_H_negativ, h < 0 ? true : false);
  EEPROM.write(EEPROM_ADDRESS_OFFSET_V_negativ, v < 0 ? true : false);

  // Make the values always positive
  h = h < 0 ? h*-1 : h;
  v = v < 0 ? v*-1 : v;

  // Store the values
  EEPROM.write(EEPROM_ADDRESS_OFFSET_H, (uint8_t) h); // HAS to be uint8_t, cause otherwise the negativ values get an overflow in EEPROM
  EEPROM.write(EEPROM_ADDRESS_OFFSET_V, (uint8_t) v); // HAS to be uint8_t, cause otherwise the negativ values get an overflow in EEPROM
  EEPROM.commit();
}

bool getFlipXY(void) {
  return flipXYAxis;
}

void setFlipXYToEEPROM(bool flipXY) {
  flipXYAxis = flipXY;
  EEPROM.write(EEPROM_ADDRESS_FLIP_XY, flipXY);
  EEPROM.commit();
}

void readFlipXYFromEEPROM() {
  bool flipXY;
  EEPROM.get(EEPROM_ADDRESS_FLIP_XY, flipXY);
  flipXYAxis = flipXY;
}

