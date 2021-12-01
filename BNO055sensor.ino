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

  temperature = bno.getTemp();
}

int getSensorSystemStatus()
{
  //  0 = Idle
  //  1 = System Error
  //  2 = Initializing Peripherals
  //  3 = System Iniitalization
  //  4 = Executing Self-Test
  //  5 = Sensor fusio algorithm running
  //  6 = System running without fusion algorithms
  uint8_t *system_status;

  // 1 = test passed, 0 = test failed
  // Bit 0 = Accelerometer self test
  // Bit 1 = Magnetometer self test
  // Bit 2 = Gyroscope self test
  // Bit 3 = MCU self test
  // 0x0F = all good!
  uint8_t *self_test_result;

  // 0 = No error
  // 1 = Peripheral initialization error
  // 2 = System initialization error
  // 3 = Self test result failed
  // 4 = Register map value out of range
  // 5 = Register map address out of range
  // 6 = Register map write error
  // 7 = BNO low power mode not available for selected operat ion mode
  // 8 = Accelerometer power mode not available

  // 9 = Fusion algorithm configuration error
  // 10 = Sensor configuration error
  uint8_t *system_error;

  bno.getSystemStatus(system_status, self_test_result, system_error);

  bool warning = (int) system_status == 6 || ((int) system_status >= 2 && (int) system_status <= 4);
  bool error = (int) system_status == 1 || (int) system_error != 0;

  if(error) {
    return SENSOR_STATUS_ERROR;
  } else if(warning) {
    return SENSOR_STATUS_WARNING;
  }

  return SENSOR_STATUS_OK;
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

int getTemperature(void) {
  return temperature;
}