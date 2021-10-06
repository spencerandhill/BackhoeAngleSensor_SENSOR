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

    // Set Offsets for Touch Recognition
    setLcdDimensions(LCD_HEIGHT, LCD_WIDTH);

    // Init Time Measurement
    previousButtonState = false;
    startTimeButtonPressed = millis();
    endTimeButtonPressed = millis();
}

void loopTouch(void)
{
    loopOffsetButton();
}

void loopOffsetButton()
{
  boolean setOffsetPressed = isSetOffsetPressed();

  if(previousButtonState == false && setOffsetPressed == true) {
    previousButtonState = true;
    startMeasureOffsetButton();
  }

  if(previousButtonState == true && setOffsetPressed == true) {
    // TODO Offset Grafik anzeigen / Ladebalken
    // Serial.print(".");
  }

  if(previousButtonState == true && setOffsetPressed == false) {
    previousButtonState = false;
    endMeasureOffsetButton();
  }

  if(setOffsetPressed == true) {
    previousButtonState = true;
  } else {
    previousButtonState = false;
  }    
}

boolean isSetOffsetPressed(void)
{
// Read Touch Values
    int pos[2] = {0, 0}; // Y, X
    ft6236_pos(pos, true, false);
    
    // pos[1] = LCD_HEIGHT - pos[1]; // The Y value is counted from right to left. Here we flip this

    boolean offsetPressed = pos[0] > 0 && pos[1] > 0;

    if(offsetPressed) {
        // Serial.printf("X: %d,Y: %d\n", pos[0], pos[1]);
    }

    return offsetPressed;
}

void startMeasureOffsetButton(void)
{   
  Serial.println();
  Serial.println("OffsetButton Time measurement started");
  startTimeButtonPressed = millis();
}

void endMeasureOffsetButton(void)
{
  Serial.println();
  Serial.println("OffsetButton measurement ended");
  endTimeButtonPressed = millis();

  unsigned long result = endTimeButtonPressed - startTimeButtonPressed;
  
  Serial.print(result);Serial.println(" milliseconds measured");

// If button was pressed long enough => Set Offset
  if(result > BUTTON_TIME_SET_OFFSET) {
    // setOffsetsToEEPROM(getHorizontalAngleRaw(), getVerticalAngleRaw());
    Serial.print(result);Serial.println("Set offset to:");

    Serial.print("New H:");Serial.println(getHorizontalAngleRaw());
    Serial.print("New V:");Serial.println(getVerticalAngleRaw());

    return;
  }

// If button was pressed short enough => Switch Page
  if(result > BUTTON_TIME_SWITCH_PAGE) {

    Serial.print(result);Serial.println("Switch Page");
    return;
  }
}
