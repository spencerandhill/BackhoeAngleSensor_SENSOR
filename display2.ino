#include "SPI.h"
#include <LovyanGFX.hpp>
#include "makerfabs_pin.h"

#include "FT6236.h"
const int i2c_touch_addr = TOUCH_I2C_ADD;

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

static lgfx::LGFX_SPI<LGFX_Config> tft;
static LGFX_Sprite sprite(&tft);
static lgfx::Panel_ILI9488 panel;

int last_pos[2] = {0, 0};
int draw_color = TFT_WHITE;

void initDisplay2()
{
    Serial.println("\n Init Display ILI9488 with FT6236 Touch Driver");

    Wire.begin(I2C_SDA, I2C_SCL);
    byte error, address;

    Wire.beginTransmission(i2c_touch_addr);
    error = Wire.endTransmission();

    if (error == 0)
    {
        Serial.print("I2C device found at address 0x");
        Serial.print(i2c_touch_addr, HEX);
        Serial.println("  !");
    }
    else if (error == 4)
    {
        Serial.print("Unknown error at address 0x");
        Serial.println(i2c_touch_addr, HEX);
    }

    pinMode(LCD_CS, OUTPUT);
    SPI_OFF_TFT;
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

    //TFT(SPI) init
    SPI_ON_TFT;

    set_tft();
    tft.begin();
    //tft.init();
    tft.fillScreen(COLOR_RED);
    // tft.fillRect(0, 0, 80, 40, TFT_RED);
    // tft.fillRect(80, 0, 80, 40, TFT_GREEN);
    // tft.fillRect(160, 0, 80, 40, TFT_BLUE);
    // tft.fillRect(240, 0, 80, 40, TFT_YELLOW);
    SPI_OFF_TFT;

    // tft.drawEllipse
}

void drawHeader(void)
{
    tft.setTextSize(4);
    tft.setTextColor(COLOR_WHITE);
    tft.setCursor(50, 16);
    tft.println("Weidemann Sensor");

    tft.setTextSize(2);
    tft.setCursor(80, 200);
    tft.println("Rathmer"); 
}

void drawValues(float horizonAngle, float verticalAngle)
{
    // Serial.printf("%d,%d\n", horizonAngle, verticalAngle);

// Make every value a positive value, no negatives
    horizonAngle = horizonAngle < 0 ? horizonAngle *-1 : horizonAngle;
    verticalAngle = verticalAngle < 0 ? verticalAngle *-1 : verticalAngle;

    // int horizonValue = ((int)(horizonAngle * 100 ))/ 100.0;
    // int verticalValue = ((int)(verticalAngle * 100 ))/ 100.0;

    // Write the values next to the rectangle
    tft.setTextSize(3);
    tft.setTextColor(COLOR_WHITE, COLOR_RED);
    tft.setCursor(50, 100);
    tft.print("H:");
    tft.println(horizonAngle, 2);

    tft.setCursor(150, 80);
    tft.print("V:");
    tft.println(verticalAngle, 2);
}

void loopDisplay2()
{
    tft.clearDisplay(COLOR_RED);
    drawHeader();
    drawValues(getHorizontalAngleWithOffset(), getVerticalAngleWithOffset());

// Read Touch Values
    int pos[2] = {0, 0};
    ft6236_pos(pos);
    
    tft.fillRect(pos[0], pos[1], 3, 3, draw_color);
}

void set_tft()
{
    // パネルクラスに各種設定値を代入していきます。
    // （LCD一体型製品のパネルクラスを選択した場合は、
    //   製品に合った初期値が設定されているので設定は不要です）

    // 通常動作時のSPIクロックを設定します。
    // ESP32のSPIは80MHzを整数で割った値のみ使用可能です。
    // 設定した値に一番近い設定可能な値が使用されます。
    panel.freq_write = 60000000;
    //panel.freq_write = 20000000;

    // 単色の塗り潰し処理時のSPIクロックを設定します。
    // 基本的にはfreq_writeと同じ値を設定しますが、
    // より高い値を設定しても動作する場合があります。
    panel.freq_fill = 60000000;
    //panel.freq_fill  = 27000000;

    // LCDから画素データを読取る際のSPIクロックを設定します。
    panel.freq_read = 16000000;

    // SPI通信モードを0~3から設定します。
    panel.spi_mode = 0;

    // データ読み取り時のSPI通信モードを0~3から設定します。
    panel.spi_mode_read = 0;

    // 画素読出し時のダミービット数を設定します。
    // 画素読出しでビットずれが起きる場合に調整してください。
    panel.len_dummy_read_pixel = 8;

    // データの読取りが可能なパネルの場合はtrueを、不可の場合はfalseを設定します。
    // 省略時はtrueになります。
    panel.spi_read = true;

    // データの読取りMOSIピンで行うパネルの場合はtrueを設定します。
    // 省略時はfalseになります。
    panel.spi_3wire = false;

    // LCDのCSを接続したピン番号を設定します。
    // 使わない場合は省略するか-1を設定します。
    panel.spi_cs = LCD_CS;

    // LCDのDCを接続したピン番号を設定します。
    panel.spi_dc = LCD_DC;

    // LCDのRSTを接続したピン番号を設定します。
    // 使わない場合は省略するか-1を設定します。
    panel.gpio_rst = LCD_RST;

    // LCDのバックライトを接続したピン番号を設定します。
    // 使わない場合は省略するか-1を設定します。
    panel.gpio_bl = LCD_BL;

    // バックライト使用時、輝度制御に使用するPWMチャンネル番号を設定します。
    // PWM輝度制御を使わない場合は省略するか-1を設定します。
    panel.pwm_ch_bl = -1;

    // バックライト点灯時の出力レベルがローかハイかを設定します。
    // 省略時は true。true=HIGHで点灯 / false=LOWで点灯になります。
    panel.backlight_level = true;

    // invertDisplayの初期値を設定します。trueを設定すると反転します。
    // 省略時は false。画面の色が反転している場合は設定を変更してください。
    panel.invert = false;

    // パネルの色順がを設定します。  RGB=true / BGR=false
    // 省略時はfalse。赤と青が入れ替わっている場合は設定を変更してください。
    panel.rgb_order = false;

    // パネルのメモリが持っているピクセル数（幅と高さ）を設定します。
    // 設定が合っていない場合、setRotationを使用した際の座標がずれます。
    // （例：ST7735は 132x162 / 128x160 / 132x132 の３通りが存在します）
    panel.memory_width = LCD_WIDTH;
    panel.memory_height = LCD_HEIGHT;

    // パネルの実際のピクセル数（幅と高さ）を設定します。
    // 省略時はパネルクラスのデフォルト値が使用されます。
    panel.panel_width = LCD_WIDTH;
    panel.panel_height = LCD_HEIGHT;

    // パネルのオフセット量を設定します。
    // 省略時はパネルクラスのデフォルト値が使用されます。
    panel.offset_x = 0;
    panel.offset_y = 0;

    // setRotationの初期化直後の値を設定します。
    panel.rotation = 3;

    // setRotationを使用した時の向きを変更したい場合、offset_rotationを設定します。
    // setRotation(0)での向きを 1の時の向きにしたい場合、 1を設定します。
    panel.offset_rotation = 0;

    // 設定を終えたら、LGFXのsetPanel関数でパネルのポインタを渡します。
    tft.setPanel(&panel);
}