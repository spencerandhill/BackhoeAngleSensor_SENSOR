void setup(void)
{
  Serial.begin(115200);
  Serial.println("Orientation Sensor Weidemann - (c) Author Jan Rathmer"); Serial.println("");
  
  initEEPROM();
  initSensor();
  initDisplay();
}

void loop(void)
{

// Get the Values as angles
  float hAngle = getHorizonAngle();
  float vAngle = getVerticalAngle();
  printEuler();

// Draw everything on the Display
  drawEverything(hAngle, vAngle);

  delay(100);
}
