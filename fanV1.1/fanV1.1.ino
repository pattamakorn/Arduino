#include <ESP8266WiFi.h>    //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>     //https://github.com/tzapu/WiFiManager
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <SPI.h>
#include <RTClib.h>
#include <Bounce2.h>

#define BUTTON_PIN D6

RTC_DS3231 RTC;
int led = D4;
int ledState = LOW;
String idmmachine = "switch01";
String sensor = "sensor1";
Bounce debouncer = Bounce();

#define D0 16
#define Plug_PIN D0
#define D3 0
#define ConfigWiFi_Pin D3 
#define ESP_AP_NAME "Switch V1.2"

//---------------Tag settime---------------//
int starttimehr;
int starttimemin;
int stoptimehr = 22;
int stoptimemin = 0;
//-------------End Tag settime-------------//
float temp;
int mstatus;

void setup() {
  Serial.begin(115200);
  setwifi();
  settime();
  setbutton();
}

void loop() {
  process();
  delay(800);
}

void setwifi(){
    pinMode(Plug_PIN,OUTPUT);
    pinMode(ConfigWiFi_Pin,INPUT_PULLUP);
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

 void settime(){
    Wire.begin();
    RTC.begin();
    DateTime now = RTC.now();
  }

 void setbutton(){
  debouncer.attach(BUTTON_PIN,INPUT_PULLUP);
  debouncer.interval(25);
  pinMode(led,OUTPUT);
  }
  
void process(){
  //DateTime now = RTC.now();
  tempsensor();
  if(temp >= 30){
    digitalWrite(led,HIGH);
    }else if(temp < 30){
      digitalWrite(led,LOW);
      }
  Serial.println(temp);
  
}


  
  void tempsensor(){
    if (WiFi.status() == WL_CONNECTED) { 
    HTTPClient http;
    http.begin("http://203.154.83.137/SmartRoom/temp.php?id=sensor1");
    int httpCode = http.GET();
    if (httpCode > 0) {
    String payload = http.getString(); 
    temp = payload.toFloat();
    }
    http.end();
    }
}
