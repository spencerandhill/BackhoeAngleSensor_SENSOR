#include "FT6236.h"

int lcdHeight = 0;
int lcdWidth = 0;

void setLcdDimensions(int height, int width)
{
    lcdHeight = height;
    lcdWidth = width;
}

void setHeight(int height)
{
    lcdHeight = height;
}

void setWidth(int width)
{
    lcdWidth = width;
}

int readTouchReg(int reg)
{
    int data = 0;
    Wire.beginTransmission(TOUCH_I2C_ADD);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(TOUCH_I2C_ADD, 1);
    if (Wire.available())
    {
        data = Wire.read();
    }
    return data;
}

int getTouchPointY(bool flipY)
{
    int XL = 0;
    int XH = 0;

    XH = readTouchReg(TOUCH_REG_XH);
    //Serial.println(XH >> 6,HEX);
    if (XH >> 6 == 1)
        return -1;
    XL = readTouchReg(TOUCH_REG_XL);

    int result = ((XH & 0x0F) << 8) | XL;

    if(flipY)
        result = lcdWidth - result; // The Y value is sometimes counted from bottom to top. Here we flip this

    return result;
}

int getTouchPointX(bool flipX)
{
    int YL = 0;
    int YH = 0;

    YH = readTouchReg(TOUCH_REG_YH);
    YL = readTouchReg(TOUCH_REG_YL);

    int result = ((YH & 0x0F) << 8) | YL;
     
    if(flipX)
        result = lcdHeight - result; // The X value is sometimes counted from bottom to top. Here we flip this

    return result;
}

void ft6236_pos(int pos[2], bool flipX, bool flipY)
{
    int XL = 0;
    int XH = 0;
    int YL = 0;
    int YH = 0;

    XH = readTouchReg(TOUCH_REG_XH);
    if (XH >> 6 == 1)
    {
        pos[0] = -1;
        pos[1] = -1;
        return;
    }
    XL = readTouchReg(TOUCH_REG_XL);
    YH = readTouchReg(TOUCH_REG_YH);
    YL = readTouchReg(TOUCH_REG_YL);

    pos[0] = ((YH & 0x0F) << 8) | YL; // In my case, this is the X-value not the Y-Value as it might look like
    pos[1] = ((XH & 0x0F) << 8) | XL; // In my case, this is the Y-value not the Y-Value as it might look like

    if(flipX)
        pos[0] = lcdHeight - pos[0]; // The X value is counted from right to left. Here we flip this

    if(flipY)
        pos[1] = lcdWidth - pos[1]; // The Y value is counted from right to left. Here we flip this
}