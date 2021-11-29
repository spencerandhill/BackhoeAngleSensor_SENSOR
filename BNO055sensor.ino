// Sensor
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#define SENSOR_BNO055_ADDRESS (0x28)
#define SENSOR_BNO055_SDA_PIN 4
#define SENSOR_BNO055_SCL_PIN 5

// Declaration for IMU Sensor BNO055
Adafruit_BNO055 bno;
TwoWire I2CBNO = TwoWire();

float horizonAngle;
float verticalAngle;
int temperature = 18;

void initSensor(void) {
  Serial.println("Sensor Begin");
  I2CBNO.begin(SENSOR_BNO055_SDA_PIN, SENSOR_BNO055_SCL_PIN);
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

void updateSensor(void) {
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

  horizonAngle = euler.y();
  verticalAngle = euler.z();

  // Serial.print("Y: ");Serial.println(horizonAngle);
  // Serial.print("Z: ");Serial.println(verticalAngle);  
}

void setOffsetToNow() {
  updateSensor();

  int horizonAngleRaw = getFlipXY() ? verticalAngle : horizonAngle;
  int verticalAngleRaw = getFlipXY() ? horizonAngle : verticalAngle;

  setOffsetsToEEPROM(horizonAngleRaw, verticalAngleRaw);
}

float getVerticalAngleWithOffset(void) {
  float verticalAngleRaw = getFlipXY() ? horizonAngle : verticalAngle;

  return verticalAngleRaw - getVOffset();
}

float getHorizontalAngleWithOffset(void) {
  float horizonAngleRaw = getFlipXY() ? verticalAngle : horizonAngle;

  return horizonAngleRaw - getHOffset();
}