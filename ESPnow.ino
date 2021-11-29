#include <ESP8266WiFi.h>
#include <espnow.h>

// Prototypes
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus);

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

void initEspNow() {
  Serial.println("Init ESP-NOW");
  Serial.print("ESP8266 Sensor-Board MAC Address:  ");
  Serial.println(WiFi.macAddress());

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Set Role of Sensor to "Controller"
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  // Register Send Callback Method and get status of transmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddressEsp32Display, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  lastMillis = millis();
}

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

void sendSensorValues() {
  Serial.println("SEND Values");
  // Get fresh Values from sensor
  updateSensor();
  // Pack Sensor-Values to datastructure
  updateSensorDataStructure();

  // Send message via ESP-NOW
  esp_now_send(broadcastAddressEsp32Display, (uint8_t *) &sensorData, sizeof(sensorData));

  Serial.print("Vertical: ");Serial.println(getVerticalAngleWithOffset());
  Serial.print("Horizontal: ");Serial.println(getHorizontalAngleWithOffset());

  Serial.print("flipXYAxis: ");Serial.println(getFlipXY());
  Serial.print("temperature: ");Serial.println(getTemperature());
  Serial.println();
  
  // Serial.println(millis());
}

void updateSensorDataStructure() {
    sensorData.horizonAngle = getHorizontalAngleWithOffset();
    sensorData.verticalAngle = getVerticalAngleWithOffset();
    sensorData.flipXYAxis = getFlipXY();
    sensorData.temperature = getTemperature();
}

void loopEspNow() {
  
  if(millis() - lastMillis > WIFI_UPDATE_PERIOD) {
    lastMillis = millis();
    sendSensorValues();
  }
}