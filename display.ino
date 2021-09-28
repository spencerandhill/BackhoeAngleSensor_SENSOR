// New Display
#include <LovyanGFX.hpp>

// Old Display
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define CIRCLE_CENTER_X 24 // That's, when the x-value is completely centered
#define CIRCLE_CENTER_Y 40 // That's, when the y-value is completely centered


// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void initDisplay(void)
{
  Serial.println("Display Begin");
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(1000);
  display.clearDisplay();
  drawBootSequence();

  display.drawRoundRect(0, 16, 48, 48, 2, WHITE);
  display.display();
}

void drawBootSequence(void)
{

}

void loopDisplay()
{

  display.clearDisplay();
  display.drawRoundRect(0, 16, 48, 48, 2, WHITE);

  drawCircle(getHorizontalAngleWithOffset(), getVerticalAngleWithOffset());
  drawValues(getHorizontalAngleWithOffset(), getVerticalAngleWithOffset());
  drawHeader();
  
  display.display(); 
}

void drawCircle(float horizonAngle, float verticalAngle)
{
  int circlePositionX = calculateCirclePositionX(horizonAngle);
  int circlePositionY = calculateCirclePositionY(verticalAngle);

  // When we're completely centered, fill the circle
  if(CIRCLE_CENTER_X == circlePositionX && CIRCLE_CENTER_Y == circlePositionY) {
    display.fillCircle(CIRCLE_CENTER_X, CIRCLE_CENTER_Y, 5, WHITE);
  } else {
    display.drawCircle(circlePositionX, circlePositionY, 5, WHITE);
  }
}

int calculateCirclePositionX(float horizonAngle)
{
  int multiplyFactor = horizonAngle < 0 ? -1 : 1; // If the value is negative, we have to move the circle to the other side

  if(horizonAngle > 25 || horizonAngle < -25) {
      return CIRCLE_CENTER_X + (25 * multiplyFactor);
  }
  
  if(horizonAngle > 20 || horizonAngle < -20) {
      return CIRCLE_CENTER_X + (20 * multiplyFactor);
  }
  
  if(horizonAngle > 15 || horizonAngle < -15) {
      return CIRCLE_CENTER_X + (15 * multiplyFactor);
  }

  if(horizonAngle > 10 || horizonAngle < -10) {
      return CIRCLE_CENTER_X + (10 * multiplyFactor);
  }

  if(horizonAngle > 5 || horizonAngle < -5) {
      return CIRCLE_CENTER_X + (5 * multiplyFactor);
  }

  return CIRCLE_CENTER_X;
}

int calculateCirclePositionY(float verticalAngle)
{
  int multiplyFactor = verticalAngle < 0 ? -1 : 1; // If the value is negative, we have to move the circle to the other side

  if(verticalAngle > 25 || verticalAngle < -25) {
      return CIRCLE_CENTER_Y + (25 * multiplyFactor);
  }
  
  if(verticalAngle > 20 || verticalAngle < -20) {
      return CIRCLE_CENTER_Y + (20 * multiplyFactor);
  }
  
  if(verticalAngle > 15 || verticalAngle < -15) {
      return CIRCLE_CENTER_Y + (15 * multiplyFactor);
  }

  if(verticalAngle > 10 || verticalAngle < -10) {
      return CIRCLE_CENTER_Y + (10 * multiplyFactor);
  }

  if(verticalAngle > 5 || verticalAngle < -5) {
      return CIRCLE_CENTER_Y + (5 * multiplyFactor);
  }

  return CIRCLE_CENTER_Y;
}

void drawValues(float horizonAngle, float verticalAngle)
{
// Make every value a positive value, no negatives
  horizonAngle = horizonAngle < 0 ? horizonAngle *-1 : horizonAngle;
  verticalAngle = verticalAngle < 0 ? verticalAngle *-1 : verticalAngle;

// Write the values next to the rectangle
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(50, 16);
  display.print("H:");
  display.println(horizonAngle, 1);
  
  display.setCursor(50, 50);
  display.print("V:");
  display.println(verticalAngle, 1);
}

void drawHeader(void)
{
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Weidemann Sensor");

  display.setCursor(80, 8);
  display.println("Rathmer"); 
}