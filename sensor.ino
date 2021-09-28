#include "makerfabs_pin.h"
// Sensor
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

// Declaration for Bosch Sensor
Adafruit_BNO055 bno;
TwoWire I2CBNO = TwoWire(0);

float horizonAngle;
float verticalAngle;

void initSensor(void)
{
    Serial.println("Sensor Begin");
    I2CBNO.begin(I2C_SDA, I2C_SCL);
    bno = Adafruit_BNO055(55, BNO055_ADDRESS_A, &I2CBNO);

  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  delay(2000);
  bno.setExtCrystalUse(true);
}

void loopSensor(void)
{
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

  horizonAngle = euler.y(); // horizonAngle
  verticalAngle = euler.z(); // verticalAngle
  
  Serial.print("Y: ");Serial.println(horizonAngle);
  Serial.print("Z: ");Serial.println(verticalAngle);  
}

float getVerticalAngleWithOffset(void)
{
    return verticalAngle + getVOffset();
}

float getHorizontalAngleWithOffset(void)
{
    return horizonAngle + getHOffset();
}

float getVerticalAngleRaw(void)
{
    return verticalAngle;
}

float getHorizontalAngleRaw(void)
{
    return horizonAngle;
}