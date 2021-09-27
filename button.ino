bool previousButtonState;

unsigned long startTimeButtonPressed;
unsigned long endTimeButtonPressed;

void initButton(void)
{
  Serial.println("Button Begin");
  
  // Init Switch-Button. Rotary is initialized with "myEncoder"
  pinMode(BUTTON_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  previousButtonState = false;

  // Init Time Measurement
  startTimeButtonPressed = millis();
  endTimeButtonPressed = millis();
}

void loopButton(void)
{
  int buttonPressed = digitalRead(BUTTON_PIN);

  if(previousButtonState == false && buttonPressed == LOW) {
    previousButtonState = true;
    startMeasure();
  }

  if(previousButtonState == true && buttonPressed == LOW) {
    // TODO Zeit zählen!!
    Serial.print("....");
  }

  if(previousButtonState == true && buttonPressed == HIGH) {
    previousButtonState = false;
    endMeasure();
  }

  if(buttonPressed == LOW) {
    previousButtonState = true;
  } else {
    previousButtonState = false;
  }
}

void startMeasure(void)
{   
  Serial.println();
  Serial.println("Time measurement started");
  startTimeButtonPressed = millis();
}

void endMeasure(void)
{
  Serial.println();
  Serial.println("Time measurement ended");
  endTimeButtonPressed = millis();

  unsigned long result = endTimeButtonPressed - startTimeButtonPressed;
  
  Serial.print(result);Serial.println(" milliseconds measured");

// If button was pressed long enough => Set Offset
  if(result > BUTTON_TIME_SET_OFFSET) {
    // setOffsets(horizonAngle, verticalAngle);
    Serial.print(result);Serial.println("Set offset");
    return;
  }

// If button was pressed short enough => Switch Page
  if(result > BUTTON_TIME_SWITCH_PAGE) {

    Serial.print(result);Serial.println("Switch Page");
    return;
  }
}
