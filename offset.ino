#include <EEPROM.h>
#define EESIZE 512

int hOffset = 0;
int vOffset = 0;

void initEEPROM(void)
{
  Serial.println("EEPROM Begin");
  
  setOffsets(10, -20);
  readOffsets();
}

int readOffsets(void)
{
  bool vorzeichenH;
  EEPROM.get(EEPROM_ADDRESS_OFFSET_H_negativ, vorzeichenH);
  bool vorzeichenV;
  EEPROM.get(EEPROM_ADDRESS_OFFSET_V_negativ, vorzeichenV);
 
  uint8_t temp;
  EEPROM.get(EEPROM_ADDRESS_OFFSET_H, temp);
  hOffset = vorzeichenH ? temp * -1 : temp;

  EEPROM.get(EEPROM_ADDRESS_OFFSET_V, temp);
  vOffset = vorzeichenV ? temp * -1 : temp;
}

int setOffsets(uint8_t h, uint8_t v)
{
  // Store if the value is negativ or not
  EEPROM.write(EEPROM_ADDRESS_OFFSET_H_negativ, h < 0 ? true : false);
  EEPROM.write(EEPROM_ADDRESS_OFFSET_V_negativ, v < 0 ? true : false);

  // Make the values always positive
  h = h < 0 ? h*-1 : h;
  v = v < 0 ? v*-1 : v;

  // Store the values
  EEPROM.write(EEPROM_ADDRESS_OFFSET_H, h);
  EEPROM.write(EEPROM_ADDRESS_OFFSET_V, v);
}