#include <EEPROM.h>
#define EESIZE 512

int hOffset = 0;
int vOffset = 0;

void initEEPROM(void)
{
  Serial.println("EEPROM Begin");
  printOffsets();

  Serial.println("Write EEPROM Offsets");
  setOffsetsToEEPROM(100, -200);
  readOffsetsFromEEPROM();
  printOffsets();
}

void printOffsets(void)
{
  Serial.print("hOffset: ");Serial.println(getHOffset());
  Serial.print("vOffset: ");Serial.println(getVOffset());
}

float getHOffset(void)
{
  return hOffset;
}

float getVOffset(void)
{
  return vOffset;
}

int readOffsetsFromEEPROM(void)
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

int setOffsetsToEEPROM(int h, int v)
{
  // Store if the value is negativ or not
  EEPROM.write(EEPROM_ADDRESS_OFFSET_H_negativ, h < 0 ? true : false);
  EEPROM.write(EEPROM_ADDRESS_OFFSET_V_negativ, v < 0 ? true : false);

  // Make the values always positive
  h = h < 0 ? h*-1 : h;
  v = v < 0 ? v*-1 : v;

  // Store the values
  EEPROM.write(EEPROM_ADDRESS_OFFSET_H, (uint8_t) h); // MUSS uint8_t sein, da sonst negative Werte im EEPROM flippen
  EEPROM.write(EEPROM_ADDRESS_OFFSET_V, (uint8_t) v); // MUSS uint8_t sein, da sonst negative Werte im EEPROM flippen
  EEPROM.commit();
}