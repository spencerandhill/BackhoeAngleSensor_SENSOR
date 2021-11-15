#include <ESP8266WiFi.h>

void initWiFi(){
  Serial.println();
  Serial.print("ESP8266 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());

  Serial.println("CHILL TIME, Laura!! <3");
}