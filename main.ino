#include <Wire.h>

void setup(void)
{
  Serial.begin(115200);
  Serial.println("Orientation Sensor Weidemann - (c) Author Jan Rathmer"); Serial.println("");

  initEEPROM();
  initSensor();
  initDisplay();
  initTouch();
}

void loop(void)
{
// Check Buttons
  loopTouch();
  loopSensor();
  loopDisplay();
  delay(20);
}