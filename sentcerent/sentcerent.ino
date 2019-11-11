#include <ESP8266WiFi.h>    //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>     //https://github.com/tzapu/WiFiManager
#include <ESP8266HTTPClient.h>

#define D7 13
#define Plug_PIN D7
#define D1 5
#define ConfigWiFi_Pin D1 
#define ESP_AP_NAME "Test Switch"
String wid = "switch";
float cerent;

void setup() {
  pinMode(Plug_PIN,OUTPUT);
  pinMode(ConfigWiFi_Pin,INPUT_PULLUP);
  Serial.begin(115200);
  // put your setup code here, to run once:
  WiFiManager wifiManager;
  if(digitalRead(ConfigWiFi_Pin) == LOW) // Press button
  {
    wifiManager.resetSettings(); // go to ip 192.168.4.1 to config
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

}

void loop() {
  // put your main code here, to run repeatedly:
  volt();
  cerentsensor();
  delay(1000);

}

void volt(){
  if (WiFi.status() == WL_CONNECTED) { 
    HTTPClient http;
    http.begin("http://203.154.83.137/SmartRoom/swcer.php?volt="+String(cerent)+"&id="+String(wid));
    int httpCode = http.GET();
    if (httpCode > 0) {
    String readstop = http.getString();
    Serial.println(readstop);
    }
    http.end();
    }
  }

  void cerentsensor(){
    float value = analogRead(A0);
    Serial.println((((value-value)*5)/1023)/0.0645);
    cerent = ((((value-value)*5)/1023)/0.0645);
    delay(250);
    }
