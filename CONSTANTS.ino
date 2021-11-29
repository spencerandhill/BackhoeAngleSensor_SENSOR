//#############SECTION EEPROM STORAGE ADDRESSES#############
#define EEPROM_ADDRESS_OFFSET_H 1
#define EEPROM_ADDRESS_OFFSET_V 2

#define EEPROM_ADDRESS_OFFSET_H_negativ 3
#define EEPROM_ADDRESS_OFFSET_V_negativ 4

#define EEPROM_ADDRESS_FLIP_XY 5


//#############SECTION ESP NOW #############
#define WIFI_UPDATE_PERIOD 500 // In Milliseconds

uint8_t broadcastAddressEsp32Display[] = {0x24, 0x62, 0xAB, 0xDD, 0x49, 0x68};