#include <ESP8266WiFi.h>
#include <espnow.h>

// Prototypes
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus);
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len);

unsigned long lastMillis=0;

//###################### SEND DATA ################################
// Structure to send data
// Must match the receiver structure
typedef struct struct_sensor_data {
  float horizonAngle;
  float verticalAngle;
  bool flipXYAxis;
  int temperature;
} struct_sensor_data;
// Create a container for  Sensor Data for the Display
struct_sensor_data sensorData;

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {

  if(sendStatus != 0) { // Only log, if Delivery failes
    Serial.print("\r\nLast Packet Delivery Status:\t");
    Serial.println("Delivery Fail");
    Serial.println();
  }
}

//###################### RECEIVE DATA ################################
int incomingCommandType;
bool incomingCommandValue;

bool newCommandReceived = false;
// Structure to receive data
// Must match the sender structure
typedef struct struct_sensor_command {
  int commandType;
  bool commandValue;
} struct_sensor_command;

// Create a container for  Sensor Commands from the Display
struct_sensor_command sensorCommand;

// Callback when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&sensorCommand, incomingData, sizeof(sensorCommand));
  Serial.print("Bytes received: ");
  Serial.println(len);
  incomingCommandType = sensorCommand.commandType;
  incomingCommandValue = sensorCommand.commandValue;
  newCommandReceived = true;
}

void initEspNow() {
  Serial.println("Init ESP-NOW");
  Serial.print("ESP8266 Sensor-Board MAC Address:  ");
  Serial.println(WiFi.macAddress());

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Set Role of Sensor to "Controller"
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  // Register Send Callback Method and get status of transmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddressEsp32Display, ESP_NOW_ROLE_COMBO, 1, NULL, 0);

  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);

  lastMillis = millis();
}

void sendSensorValues() {
  Serial.print(".");
  // Get fresh Values from sensor
  updateSensor();
  // Pack Sensor-Values to datastructure
  updateSensorDataStructure();

  // Send message via ESP-NOW
  int result = esp_now_send(broadcastAddressEsp32Display, (uint8_t *) &sensorData, sizeof(sensorData));
  if (result != 0) { // Only log, if sending failes
    Serial.println("Error sending data");

    Serial.print("Horizontal: ");Serial.println(getHorizontalAngleWithOffset());
    Serial.print("Vertical: ");Serial.println(getVerticalAngleWithOffset());
    Serial.print("flipXYAxis: ");Serial.println(getFlipXY());
    Serial.print("temperature: ");Serial.println(getTemperature());
    Serial.println();
  }
  
  // Serial.println(millis());
}

void updateSensorDataStructure() {
    sensorData.horizonAngle = getHorizontalAngleWithOffset();
    sensorData.verticalAngle = getVerticalAngleWithOffset();
    sensorData.flipXYAxis = getFlipXY();
    sensorData.temperature = getTemperature();
}

void processCommand() {
  switch(incomingCommandType) {
    case DISPLAY_COMMAND_SET_OFFSET:
      Serial.print("DISPLAY_COMMAND_SET_OFFSET. Value: ");
      Serial.println(incomingCommandValue);
    break;
    
    case DISPLAY_COMMAND_FLIP_XY:
      Serial.print("DISPLAY_COMMAND_FLIP_XY. Value: ");
      Serial.println(incomingCommandValue ? "True" : "False");
      setFlipXYToEEPROM(incomingCommandValue);
    break;
  }

}

void loopEspNow() {
  
  if(millis() - lastMillis > WIFI_UPDATE_PERIOD) {
    lastMillis = millis();
    sendSensorValues();
  }

  if(newCommandReceived == true) {
    Serial.println("New Command received!");
    processCommand();
    newCommandReceived = false;
  }
}