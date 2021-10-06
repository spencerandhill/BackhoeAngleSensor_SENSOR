#include <Wire.h>

void setup(void)
{
  Serial.begin(115200);
  Serial.println("Orientation Sensor Weidemann - (c) Author Jan Rathmer"); Serial.println("");

  initEEPROM();
  initSensor();
  // initDisplay();
  initDisplay2();
  initButton();
}

void loop(void)
{
// Draw everything on the Display
  // loopDisplay();

// Check Buttons
  loopButton();
  loopSensor();
  loopDisplay2();
  delay(20);
}