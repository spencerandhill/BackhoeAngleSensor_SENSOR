// Sensor
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#define SENSOR_BNO055_ADDRESS (0x28)
#define SENSOR_BNO055_SDA_PIN 5
#define SENSOR_BNO055_SCL_PIN 6

// Declaration for IMU Sensor BNO055
Adafruit_BNO055 bno;
TwoWire I2CBNO = TwoWire();

float horizonAngle;
float verticalAngle;

// In case, the sensor was attached in a different direction
bool flipXYAxis = false;

void initSensor(void)
{
  Serial.println("Sensor Begin");
  I2CBNO.begin(SENSOR_BNO055_SDA_PIN, SENSOR_BNO055_SCL_PIN, 50000);
  bno = Adafruit_BNO055(55, SENSOR_BNO055_ADDRESS, &I2CBNO);

  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  delay(500);
  bno.setExtCrystalUse(true);
}

void loopSensor(void)
{
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

  horizonAngle = euler.y(); // horizonAngle
  verticalAngle = euler.z(); // verticalAngle

  // Serial.print("Y: ");Serial.println(horizonAngle);
  // Serial.print("Z: ");Serial.println(verticalAngle);  
}

float getVerticalAngleWithOffset(void)
{
  return getVerticalAngleRaw() - getVOffset();
}

float getHorizontalAngleWithOffset(void)
{
  return getHorizontalAngleRaw() - getHOffset();
}

float getVerticalAngleRaw(void)
{
  return flipXYAxis ? verticalAngle : horizonAngle;
}

float getHorizontalAngleRaw(void)
{
  return flipXYAxis ? horizonAngle : verticalAngle;
}