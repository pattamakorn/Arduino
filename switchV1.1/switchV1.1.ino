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
int led = D7;
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
int li;
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

void lstate(){
  debouncer.update();
  if ( debouncer.fell() ) {
      ledState = !ledState;
      }  
}
void process(){
  DateTime now = RTC.now();
  lisensor();
  loadstate();
  loadtimeshr();
  loadtimesmin();
  loadstimesmin();
  loadstimeshr();
  if(now.hour() >= starttimehr and now.minute() >= starttimemin){
    if(li >= 500){
        if(ledState==1 || mstatus==1){
          Serial.print("1 => ");
            Serial.println(mstatus);
            digitalWrite(led,HIGH);
            digitalWrite(Plug_PIN,HIGH);
        }else if(ledState==0 || mstatus==0){
          Serial.print("0 => ");
              Serial.println(mstatus);
              digitalWrite(led,LOW);
              digitalWrite(Plug_PIN,LOW);
        }
      }else{
        Serial.println("Off");
        }
    }else{
      Serial.print("Off => ");
      Serial.println(mstatus);
      Serial.print("Time Start => ");
      Serial.print(starttimehr);
      Serial.print(":");
      Serial.println(starttimemin);
      Serial.print("Light => ");
      Serial.println(li);
      }
}
  
  void lisensor(){
    if (WiFi.status() == WL_CONNECTED) { 
    HTTPClient http;
    http.begin("http://203.154.83.137/SmartRoom/lightsensor.php?id="+String(sensor));
    int httpCode = http.GET();
    if (httpCode > 0) {
    String payload = http.getString(); 
    li = payload.toInt();
    //Serial.println(motion);
    }
    http.end();
    }
}

void loadstate(){
    if (WiFi.status() == WL_CONNECTED) { 
    HTTPClient http;
    http.begin("http://203.154.83.137/SmartRoom/onoffswitch.php?mid="+String(idmmachine));
    int httpCode = http.GET();
    if (httpCode > 0) {
    String payload = http.getString(); 
    mstatus = payload.toInt();
    //Serial.println(mstatus);
    }
    http.end();
    }
}

void loadtimeshr(){
    if (WiFi.status() == WL_CONNECTED) { 
    HTTPClient http;
    http.begin("http://203.154.83.137/SmartRoom/timestart.php?id="+String(idmmachine));
    int httpCode = http.GET();
    if (httpCode > 0) {
    String payload = http.getString(); 
    starttimehr = payload.toInt();
    }
    http.end();
    }
}

void loadtimesmin(){
    if (WiFi.status() == WL_CONNECTED) { 
    HTTPClient http;
    http.begin("http://203.154.83.137/SmartRoom/strmin.php?id="+String(idmmachine));
    int httpCode = http.GET();
    if (httpCode > 0) {
    String payload = http.getString(); 
    starttimemin = payload.toInt();
    }
    http.end();
    }
}

void loadstimesmin(){
    if (WiFi.status() == WL_CONNECTED) { 
    HTTPClient http;
    http.begin("http://203.154.83.137/SmartRoom/stpmin.php?id="+String(idmmachine));
    int httpCode = http.GET();
    if (httpCode > 0) {
    String payload = http.getString(); 
    stoptimemin = payload.toInt();
    }
    http.end();
    }
}

void loadstimeshr(){
    if (WiFi.status() == WL_CONNECTED) { 
    HTTPClient http;
    http.begin("http://203.154.83.137/SmartRoom/timestop.php?id="+String(idmmachine));
    int httpCode = http.GET();
    if (httpCode > 0) {
    String payload = http.getString(); 
    stoptimehr = payload.toInt();
    }
    http.end();
    }
}
