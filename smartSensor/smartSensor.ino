#include "DHT.h"
#include <ESP8266WiFi.h>    //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>     //https://github.com/tzapu/WiFiManager
#include <ESP8266HTTPClient.h>
#include <MCP3008.h>
#define CS_PIN D8
#define CLOCK_PIN D5
#define MOSI_PIN D7
#define MISO_PIN D6
MCP3008 adc(CLOCK_PIN, MOSI_PIN, MISO_PIN, CS_PIN);

// set pin numbers:
#define D0 16             // USER LED Wake
#define ledPin  D0
#define D3 0
#define ConfigWiFi_Pin D3 
#define ESP_AP_NAME "Sensor Center"

#define DHTPIN D1 
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
int inPin = D2;
int motion = 0;

const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = D4; // Analog output pin that the LED is attached to

int sensorValue = 0;        // value read from the pot
int outputValue = 0; 

void setup() 
{
  // initialize pin D0 as an output.
  pinMode(ledPin, OUTPUT);
  pinMode(ConfigWiFi_Pin,INPUT_PULLUP);
  dht.begin();


  Serial.begin(115200);
  digitalWrite(ledPin,HIGH);//Turn on the LED

  WiFiManager wifiManager;
  if(digitalRead(ConfigWiFi_Pin) == LOW) // Press button
  {
    wifiManager.resetSettings(); // go to ip 192.168.4.1 to config
    digitalWrite(D0,LOW);
  }
  wifiManager.autoConnect(ESP_AP_NAME); 
  while (WiFi.status() != WL_CONNECTED) 
  {
     delay(250);
     Serial.print(".");
  }
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(ledPin,HIGH);
  Serial.println("");
  
}
void loop() 
{
 sentData();
 Serial.println("-----------------------------");
 
}

void sentData(){
  //แรงดันไฟ
  sensorValue = analogRead(analogInPin);
  outputValue = map(sensorValue, 0, 1023, 0, 255);
  Serial.println(outputValue);
  //แสง
  int light = adc.readADC(7);   
  Serial.println(light);
  //เคลื่อนไหว
  motion = digitalRead(inPin);
  Serial.print("Motion: ");
  Serial.println(motion);
  //อุณภูมิ
  float t = dht.readTemperature();
  if (isnan(t)) {
    if (WiFi.status() == WL_CONNECTED) { 
 
    HTTPClient http;
    float tem = t;
     
    http.begin("http://203.154.83.137/SmartRoom/sensor.php?motion="+String (motion)+"&light="+String(light)+"&temp=0.00&Voltage="+String(outputValue));  //Specify request destination
    
    int httpCode = http.GET();   
    Serial.println(httpCode);
     
    if (httpCode > 0) {
     
    String payload = http.getString();
    Serial.println(payload);            
     
    }
     
    http.end();
     
    }
    Serial.println(F("Failed to read from DHT sensor!"));
    
    return;
  }
  Serial.print(F("Temperature: "));
  
  Serial.print(t);
  Serial.println(F("°C "));
  
  if (WiFi.status() == WL_CONNECTED) { 
 
    HTTPClient http;
    float tem = t;
     
    http.begin("http://203.154.83.137/SmartRoom/sensor.php?motion="+String (motion)+"&light="+String(light)+"&temp="+String(tem)+"&Voltage="+String(outputValue));  //Specify request destination
    
    int httpCode = http.GET();   
    Serial.println(httpCode);
     
    if (httpCode > 0) {
     
    String payload = http.getString();
    Serial.println(payload);            
     
    }
     
    http.end();
     
    }
  
  }
