#include <Wire.h>

void setup(void)
{
  Serial.begin(115200);
  Serial.println("BackhoeAngleSensor BAS - (c) Author Jan Rathmer"); Serial.println("");

  // initSensor();
  initEEPROM();
  initWiFi();
}

void loop(void)
{
  loopSensor();
  delay(5);
}
