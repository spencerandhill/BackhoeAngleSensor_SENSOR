#include "SPI.h"
#include <LovyanGFX.hpp>
#include "makerfabs_pin.h"

//SPI control
#define SPI_ON_TFT digitalWrite(LCD_CS, LOW)
#define SPI_OFF_TFT digitalWrite(LCD_CS, HIGH)

struct LGFX_Config
{
    static constexpr spi_host_device_t spi_host = VSPI_HOST;
    static constexpr int dma_channel = 1;
    static constexpr int spi_sclk = LCD_SCK;
    static constexpr int spi_mosi = LCD_MOSI;
    static constexpr int spi_miso = LCD_MISO;
};

static lgfx::LGFX_SPI<LGFX_Config> display2;
static lgfx::Panel_ILI9488 panel;

static LGFX_Sprite hValueCanvas;
static LGFX_Sprite vValueCanvas;

// int last_pos[2] = {0, 0};
// int draw_color = TFT_WHITE;

void initDisplay2()
{
    Serial.println("\n Init Display ILI9488");

    pinMode(LCD_CS, OUTPUT);
    SPI_OFF_TFT;
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

    //TFT(SPI) init
    SPI_ON_TFT;

    set_tft();
    display2.begin();
    display2.fillScreen(TFT_RED);

    SPI_OFF_TFT;

    // // Setup Canvas for H and V Values
    // vValueCanvas.setColorDepth(8);
    // vValueCanvas.setTextWrap(false);
    // vValueCanvas.setTextSize(3);
    // vValueCanvas.setColor(255,0,0);
    // vValueCanvas.setTextColor(TFT_WHITE);
    // vValueCanvas.createSprite(130, 36);

    // hValueCanvas.setColorDepth(8);
    // hValueCanvas.setTextWrap(false);
    // hValueCanvas.setTextSize(3);
    // hValueCanvas.setColor(255,0,0);
    // hValueCanvas.setTextColor(TFT_WHITE);
    // hValueCanvas.createSprite(130, 36);
    // display2.drawEllipse
}

void drawHeader(void)
{
    display2.setTextSize(4);
    display2.setTextColor(TFT_WHITE);
    display2.setCursor(50, 16);
    display2.println("Weidemann Sensor");

    display2.setTextSize(2);
    display2.setCursor(300, 50);
    display2.println("© by Jan Rathmer"); 
}

void drawValues(float horizonAngle, float verticalAngle)
{
    // Serial.printf("%d,%d\n", horizonAngle, verticalAngle);

// Make every value a positive value, no negatives
    horizonAngle = horizonAngle < 0 ? horizonAngle *-1 : horizonAngle;
    verticalAngle = verticalAngle < 0 ? verticalAngle *-1 : verticalAngle;

    // Write the values next to the rectangle
    display2.setTextSize(3);
    display2.setTextColor(TFT_WHITE, TFT_RED);
    display2.setCursor(300, 100);
    display2.print("H:");
    display2.println(horizonAngle, 2);

    display2.setCursor(300, 150);
    display2.print("V:");
    display2.println(verticalAngle, 2);

    // // Flicker Free Text Update
    // hValueCanvas.setCursor(2, 10);
    // hValueCanvas.print("H:");
    // hValueCanvas.println(horizonAngle, 2);
    // hValueCanvas.pushSprite(&display2, 300, 100);

    // vValueCanvas.setCursor(2, 10);
    // vValueCanvas.print("V:");
    // vValueCanvas.println(verticalAngle, 2);
    // vValueCanvas.pushSprite(&display2, 300, 150);
}

void loopDisplay2()
{
    drawHeader();
    drawValues(getHorizontalAngleWithOffset(), getVerticalAngleWithOffset());

    // display2.fillRect(pos[0], pos[1], 3, 3, draw_color);
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
    display2.setPanel(&panel);
}