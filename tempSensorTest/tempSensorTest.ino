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
  Serial.print("Connecting..");
  }
  pinMode(inPin, INPUT);
  Serial.println(F("DHTxx test!"));

  dht.begin();

}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);
  motion = digitalRead(inPin);
  Serial.print("Motion: ");
  Serial.println(motion);
  sensorValue = analogRead(sensorPin);
  Serial.print("Light: ");
  Serial.println(sensorValue);
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  Serial.print(F("Temperature: "));
  
  Serial.print(t-5);
  Serial.println(F("°C "));

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
    HTTPClient http;  //Declare an object of class HTTPClient
    float tem = t-5;
     
    http.begin("http://203.154.83.137/SmartRoom/sensor.php?motion="+String (motion)+"&light="+String(sensorValue)+"&temp="+String(tem));  //Specify request destination
    
    int httpCode = http.GET();   
    Serial.println(httpCode);    //Send the request
     
    if (httpCode > 0) { //Check the returning code
     
    String payload = http.getString();   //Get the request response payload
    Serial.println(payload);                     //Print the response payload
     
    }
     
    http.end();   //Close connection
     
    }
     
    delay(5000);
}
