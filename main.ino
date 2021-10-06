#include <Wire.h>

void setup(void)
{
  Serial.begin(115200);
  Serial.println("Orientation Sensor Weidemann - (c) Author Jan Rathmer"); Serial.println("");

  initEEPROM();
  initSensor();
  // initDisplay();
  initDisplay2();
  initTouch();
}

void loop(void)
{
// Draw everything on the Display
  // loopDisplay();

// Check Buttons
  loopTouch();
  loopSensor();
  loopDisplay2();
  delay(20);
}