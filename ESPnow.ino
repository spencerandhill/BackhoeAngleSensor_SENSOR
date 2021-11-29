#include <ESP8266WiFi.h>
#include <espnow.h>

unsigned long lastMillis=0;

// Structure to send data
// Must match the receiver structure
typedef struct struct_message {
  float horizonAngle;
  float verticalAngle;
  bool flipXYAxis;
  int temperature;
} struct_sensor_data;

// Create a struct_sensor_data container
struct_sensor_data sensorData;

void initWiFi() {
  Serial.println();
  Serial.print("ESP8266 Sensor-Board MAC Address:  ");
  Serial.println(WiFi.macAddress());


  lastMillis = millis();
}

void sendSensorValues() {
  updateSensor();
  updateSensorDataStructure();

  Serial.println("SEND Values: ");

  Serial.print("Vertical: ");Serial.println(getVerticalAngleWithOffset());
  Serial.print("Horizontal: ");Serial.println(getHorizontalAngleWithOffset());
  Serial.println();
  
  // Serial.println(millis());
}

void updateSensorDataStructure() {

}

void loopEspNow() {
  
  if(millis() - lastMillis > WIFI_UPDATE_PERIOD) {
    lastMillis = millis();
    sendSensorValues();
  }
}