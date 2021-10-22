# BackhoeAngleSensor

Makerfab 3,5" TFT Touch Display and Bosch BNO055 "Absolut Orientation-Sensor"

This project is to combine Makerfab's 3.5" TFT Touch Display with a simple Bosch BNO055 "Absolute Orientation Sensor".

The goal is to mount the sensor on the bucket of a yard loader/backhoe and the display next to the steering wheel. This way you can always read the current tilt of the bucket or pallet fork.

## Hardware
---
### Display
For the Display I used the "Makerfab ESP32 TFT LCD with Camera(3.5")" (see: https://www.makerfabs.com/wiki/index.php?title=ESP32_TFT_LCD_with_Camera(3.5%27%27))

I don't need the camera, but it was on there and I had the board lying around.
   
### Sensor
I used the BlueDot BNO055 9-Axis IMU (see: https://www.bluedot.space/products/bno055/).  
You can get those on amazon or eBay. Maybe even on AliExpress and I guess many other variants/board of the BNO055 will work.
   
## Software
---
### TFT_eSPI
The Display is controlled using the famous TFT_eSPI (https://github.com/Bodmer/TFT_eSPI), together with the FT6x36 touch driver library from Wolfgang Christl.
   
### FT6x36 / tp_i2c
Unfortunately I didn't find any official repo for this libraries from Wolfgang Christl, but I hope it's fine to mention, that this piece of code is from him.

### LVGL aka Light and Versatile Graphics Library
For the UI Elements I'm using LVGL (https://github.com/lvgl/lvgl)

### Adafruit_BNO055
To read the Data from the Bosch BNO055 Sensor, I use the Adafruit_BNO055 (https://github.com/adafruit/Adafruit_BNO055)

### Arduino EEPROM
https://www.arduino.cc/en/Reference/EEPROM

### Additional Stuff
For the Makerfab Hardware I'm also using some code from Makerfab themselves. For example the makerfabs_pin.h from https://github.com/Makerfabs/Makerfabs_FAQ 
I also got some example code, which I modified and remix till I don't exactly know, which part is from me and which is from others. So hopefully I got everyone covered with this source indication. In case you feel uncovered, please tell me.