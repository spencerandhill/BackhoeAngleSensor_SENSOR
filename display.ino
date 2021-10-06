#include "SPI.h"
#include <LovyanGFX.hpp>
#include "makerfabs_pin.h"

//SPI control
#define SPI_ON_TFT digitalWrite(LCD_CS, LOW)
#define SPI_OFF_TFT digitalWrite(LCD_CS, HIGH)

#define CIRCLE_CENTER_X 24 // That's, when the x-value is completely centered
#define CIRCLE_CENTER_Y 40 // That's, when the y-value is completely centered

struct LGFX_Config
{
    static constexpr spi_host_device_t spi_host = VSPI_HOST;
    static constexpr int dma_channel = 1;
    static constexpr int spi_sclk = LCD_SCK;
    static constexpr int spi_mosi = LCD_MOSI;
    static constexpr int spi_miso = LCD_MISO;
};

static lgfx::LGFX_SPI<LGFX_Config> display;
static lgfx::Panel_ILI9488 panel;

static LGFX_Sprite hValueCanvas;
static LGFX_Sprite vValueCanvas;

void initDisplay()
{
    Serial.println("\n Init Display ILI9488");

    pinMode(LCD_CS, OUTPUT);
    SPI_OFF_TFT;
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

    //TFT(SPI) init
    SPI_ON_TFT;

    set_tft();
    display.begin();
    display.fillScreen(TFT_RED);

    SPI_OFF_TFT;

    drawBootSequence();
    // display.drawEllipse
}

void drawBootSequence(void)
{

}

void drawHeader(void)
{
    display.setTextSize(4);
    display.setTextColor(TFT_WHITE);
    display.setCursor(50, 16);
    display.println("Weidemann Sensor");

    display.setTextSize(2);
    display.setCursor(300, 50);
    display.println("by Jan Rathmer"); 
}

void loopDisplay()
{
    display.drawRoundRect(0, 16, 48, 48, 2, TFT_WHITE);
    drawCircle(getHorizontalAngleWithOffset(), getVerticalAngleWithOffset());
    drawValues(getHorizontalAngleWithOffset(), getVerticalAngleWithOffset());
    drawHeader();
}

void drawValues(float horizonAngle, float verticalAngle)
{
// Make every value a positive value, no negatives
    horizonAngle = horizonAngle < 0 ? horizonAngle *-1 : horizonAngle;
    verticalAngle = verticalAngle < 0 ? verticalAngle *-1 : verticalAngle;

    // Write the values next to the rectangle
    display.setTextSize(3);
    display.setTextColor(TFT_WHITE, TFT_RED);
    display.setCursor(300, 100);
    display.print("H:");
    display.println(horizonAngle, 2);

    display.setCursor(300, 150);
    display.print("V:");
    display.println(verticalAngle, 2);
}

void drawCircle(float horizonAngle, float verticalAngle)
{
  int circlePositionX = calculateCirclePositionX(horizonAngle);
  int circlePositionY = calculateCirclePositionY(verticalAngle);

  // When we're completely centered, fill the circle
  if(CIRCLE_CENTER_X == circlePositionX && CIRCLE_CENTER_Y == circlePositionY) {
    display.fillCircle(CIRCLE_CENTER_X, CIRCLE_CENTER_Y, 5, TFT_WHITE);
  } else {
    display.drawCircle(circlePositionX, circlePositionY, 5, TFT_WHITE);
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

void set_tft()
{
    // LCD Setup
    // ESP32 SPI 80MHz
    panel.freq_write = 60000000; //panel.freq_write = 20000000;
    panel.freq_fill = 60000000; //panel.freq_fill  = 27000000;

    panel.freq_read = 16000000;
    panel.spi_mode = 0;
    panel.spi_mode_read = 0;
    panel.len_dummy_read_pixel = 8;
    panel.spi_read = true;
    panel.spi_3wire = false;
    panel.spi_cs = LCD_CS; // LCD CS Pin
    panel.spi_dc = LCD_DC; // LCD DC Pin
    panel.gpio_rst = LCD_RST; // LCD Reset Pin
    panel.gpio_bl = LCD_BL; // LCD Backlight Pin
    panel.pwm_ch_bl = -1; // PWM Change Backlight false
    panel.backlight_level = true; //true; true=HIGH / false=LOW
    panel.invert = false; // invertDisplay Default false
    panel.rgb_order = false; // RGB=true / BGR=false
    panel.memory_width = LCD_WIDTH;
    panel.memory_height = LCD_HEIGHT;
    panel.panel_width = LCD_WIDTH;
    panel.panel_height = LCD_HEIGHT;
    panel.offset_x = 0;
    panel.offset_y = 0;
    panel.rotation = 3; // Values between 0 - 3
    panel.offset_rotation = 0;

    // Tell LGFX the used panel
    display.setPanel(&panel);
}