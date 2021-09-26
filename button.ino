void calibrate(void)
{
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  float horizonAngle = euler.y();
  float verticalAngle = euler.z();

  setOffsets(horizonAngle, verticalAngle);
}
