// Date, Time and Alarm functions using a DS3231 RTC connected via I2C and Wire lib

#include <Wire.h>
#include <SPI.h>  // not used here, but needed to prevent a RTClib compile error
#include <RTClib.h>
#include <ESP8266WiFi.h>    //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>     //https://github.com/tzapu/WiFiManager
#include <ESP8266HTTPClient.h>

RTC_DS3231 RTC;

int led = D0;
float curentt;
String idmmachine = "plug01";
String statusm;
String setstatus;
#define ESP_AP_NAME "Plug V1.2"

//---------------Tag settime---------------//
int starttimehr;
int starttimemin;
int stoptimehr;
int stoptimemin;
//-------------End Tag settime-------------//

void setup () {
    Serial.begin(57600);
    pinMode(led,OUTPUT);
    Wire.begin();
    RTC.begin();
    digitalWrite(led,LOW);
  RTC.adjust(DateTime(__DATE__, __TIME__));//ลบเมื่อตั้งเวลาเสร็จ
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  DateTime now = RTC.now();
  RTC.setAlarm1Simple(4, 37);
  RTC.turnOnAlarm(1);
  if (RTC.checkAlarmEnabled(1)) {
    Serial.println("Alarm Enabled");
  }
    // put your setup code here, to run once:
    WiFiManager wifiManager;
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

void loop () {
    DateTime now = RTC.now();
    loadtimeshr();
    loadtimesmin();
    loadstimesmin();
    loadstimeshr();
    Serial.print("Status : ");Serial.println(statusm);
    Serial.print(starttimehr);Serial.print(" : ");Serial.println(starttimemin);
    Serial.print(stoptimehr);Serial.print(" : ");Serial.println(stoptimemin);
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    current();
    sendcurent();

      if ((now.hour() >= starttimehr and now.minute() >= starttimemin) and (now.hour() <= stoptimehr and now.minute() < stoptimemin)) {
                Serial.println("HIGH");
                digitalWrite(led,HIGH);
                if (WiFi.status() == WL_CONNECTED) { 
                    HTTPClient http;
                    http.begin("http://203.154.83.137/SmartRoom/setcontrolplug.php?id="+String(idmmachine)+"&status=1");
                    int httpCode = http.GET();
                    if (httpCode > 0) {
                    String payload = http.getString();
                    }
                    http.end();
                  }
          }
      
          if (now.hour() == stoptimehr and now.minute() == stoptimemin) {
            control();
          }
    Serial.println();
    delay(3000);
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

void current(){
  float value = analogRead(A0);
  Serial.println((((value-871)*5)/1024)/0.185);
  curentt = (((value-871)*5)/1024)/0.185;
  delay(250);
  }

void sendcurent(){
    if (WiFi.status() == WL_CONNECTED) { 
    HTTPClient http;
    http.begin("http://203.154.83.137/SmartRoom/curent.php?id="+String(idmmachine)+"&volt="+String(curentt));
    int httpCode = http.GET();
    if (httpCode > 0) {
    String payload = http.getString(); 
    }
    http.end();
    }
}

void control(){
    if (WiFi.status() == WL_CONNECTED) { 
    HTTPClient http;
    http.begin("http://203.154.83.137/SmartRoom/onoffplug.php?mid="+String(idmmachine));
    int httpCode = http.GET();
    if (httpCode > 0) {
    String payload = http.getString(); 
    statusm = payload;
    if(statusm == "1"){
        Serial.println("HIGH");
        digitalWrite(led,HIGH);
      }else if(statusm == "0"){
        Serial.println("LOW");
        digitalWrite(led,LOW);
      }
    }
    http.end();
    }
}
