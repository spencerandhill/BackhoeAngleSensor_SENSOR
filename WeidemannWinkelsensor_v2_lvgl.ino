#include <Wire.h>
#include <lvgl.h>
#include <TFT_eSPI.h>

// #include "graphics/bootGraphics.h"

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
bool read_encoder(lv_indev_drv_t * indev, lv_indev_data_t * data);

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
  delay(5);
}
