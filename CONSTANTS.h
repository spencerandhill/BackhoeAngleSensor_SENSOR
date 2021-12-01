//#############SECTION EEPROM STORAGE ADDRESSES#############
#define EEPROM_ADDRESS_OFFSET_H 1
#define EEPROM_ADDRESS_OFFSET_V 2

#define EEPROM_ADDRESS_OFFSET_H_negativ 3
#define EEPROM_ADDRESS_OFFSET_V_negativ 4

#define EEPROM_ADDRESS_FLIP_XY 5


//#############SECTION ESP NOW #############
#define ESPNOW_SEND_RATE 500 // In Milliseconds

uint8_t broadcastAddressEsp32Display[] = {0xF0, 0x08, 0xD1, 0xFF, 0xC8, 0x38};

#define SENSOR_STATUS_ERROR 0   // Sensor OFFLINE or something really bad happened (Peripheral error, or something else)
#define SENSOR_STATUS_WARNING 1 // Sensor ALIVE but NOT OK (Something is weird, but not crucial)
#define SENSOR_STATUS_OK 2      // Sensor ALIVE and OK

//#############SECTION DISPLAY COMMANDS #############
#define DISPLAY_COMMAND_CALIBRATE_SENSOR 0
#define DISPLAY_COMMAND_FLIP_XY 1