#include "FT6236.h"

bool previousButtonState;

unsigned long startTimeButtonPressed;
unsigned long endTimeButtonPressed;

void initTouch(void)
{
    Serial.println("FT6236 Touch Driver Begin");

    Wire.begin(I2C_SDA, I2C_SCL);
    byte error;

    Wire.beginTransmission(TOUCH_I2C_ADD);
    error = Wire.endTransmission();

    if (error == 0)
    {
        Serial.print("I2C device found at address 0x");
        Serial.print(TOUCH_I2C_ADD, HEX);
        Serial.println("  !");
    }
    else if (error == 4)
    {
        Serial.print("Unknown error at address 0x");
        Serial.println(TOUCH_I2C_ADD, HEX);
    }


  // Init Switch-Button. Rotary is initialized with "myEncoder"
//   pinMode(BUTTON_PIN, INPUT);
//   pinMode(BUTTON_PIN, INPUT_PULLUP);
  previousButtonState = false;

  // Init Time Measurement
  startTimeButtonPressed = millis();
  endTimeButtonPressed = millis();
}

void loopTouch(void)
{
//   int buttonPressed = digitalRead(BUTTON_PIN);

  boolean buttonPressed = isButtonPressed();

  if(previousButtonState == false && buttonPressed == true) {
    previousButtonState = true;
    startMeasure();
  }

  if(previousButtonState == true && buttonPressed == true) {
    // TODO Zeit zählen!!
    Serial.print("....");
  }

  if(previousButtonState == true && buttonPressed == false) {
    previousButtonState = false;
    endMeasure();
  }

  if(buttonPressed == true) {
    previousButtonState = true;
  } else {
    previousButtonState = false;
  }
}

boolean isButtonPressed(void)
{
// Read Touch Values
    int pos[2] = {0, 0};
    ft6236_pos(pos);
    
    // Serial.printf("%d,%d\n", pos[0], pos[1]);

    return pos[0] > 0 && pos[1] > 0;
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
    // setOffsetsToEEPROM(getHorizontalAngleRaw(), getVerticalAngleRaw());
    Serial.print(result);Serial.println("Set offset");
    return;
  }

// If button was pressed short enough => Switch Page
  if(result > BUTTON_TIME_SWITCH_PAGE) {

    Serial.print(result);Serial.println("Switch Page");
    return;
  }
}
