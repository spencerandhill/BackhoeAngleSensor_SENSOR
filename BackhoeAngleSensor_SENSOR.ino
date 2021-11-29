#include <Wire.h>

// Uncomment following line, the first time flashing to the controller!
// #define FLASHING_FIRST_TIME

void setup(void) {
  Serial.begin(115200);
  Serial.println("BackhoeAngleSensor BAS - (c) Author Jan Rathmer"); Serial.println("");

  initSensor();
  initEEPROM();
  initEspNow();

// Use following lines to set the Offset to a new Zero  
  // delay(2000);
  // setOffsetToNow();
}

void loop(void) {
  loopEspNow();
}
