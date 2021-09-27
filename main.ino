#include <Wire.h>

void setup(void)
{
  Serial.begin(115200);
  Serial.println("Orientation Sensor Weidemann - (c) Author Jan Rathmer"); Serial.println("");

  initEEPROM();
  initSensor();
  initDisplay();
  initButton();
}

void loop(void)
{
// Draw everything on the Display
  drawEverything();

// Check Buttons
  loopButton();
  loopSensor();
  
  delay(20);
}