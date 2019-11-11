#include "DHT.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define DHTPIN D1 
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
int sensorPin = A0; 
int sensorValue = 0;
int inPin = D2;
int motion = 0;

const char* ssid = "kornpattamakorn";
const char* password = "korn2540";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.println("Connecting..");
  }
}

void loop() {
  lightsensor();
  delay(2000);
}

void lightsensor(){
    if (WiFi.status() == WL_CONNECTED) { 
    HTTPClient http;
    http.begin("http://203.154.83.137/SmartRoom/lightsensor.php");
    int httpCode = http.GET();   
    //Serial.println(httpCode);
    if (httpCode > 0) {
    String payload = http.getString(); 
    int light = payload.toInt();
    Serial.println(light);
    }
    http.end();
    }
}
