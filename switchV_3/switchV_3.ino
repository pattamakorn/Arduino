#include <Bounce2.h>
#include <Wire.h>
#include <SPI.h>  // not used here, but needed to prevent a RTClib compile error
#include <RTClib.h>
#include <ESP8266WiFi.h>    //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>     //https://github.com/tzapu/WiFiManager
#include <ESP8266HTTPClient.h>
RTC_DS3231 RTC;

#define BUTTON_PIN D6
#define LED_PIN D7
#define LED_PIN2 D0

int ledState = LOW;
#define ESP_AP_NAME "Swicth V1.2"
String idmmachine = "34";
String idsensor = "sensor1";
int light;
int motion;
int State;

//---------------Tag settime---------------//
int starttimehr;
int starttimemin;
int stoptimehr;
int stoptimemin;
//-------------End Tag settime-------------//


Bounce debouncer = Bounce();

void setup() {
  Serial.begin(9600);
  debouncer.attach(BUTTON_PIN,INPUT_PULLUP);
  debouncer.interval(25);
  pinMode(LED_PIN,OUTPUT);
  pinMode(LED_PIN2,OUTPUT);
  digitalWrite(LED_PIN,ledState);
  digitalWrite(LED_PIN2,ledState);
  Wire.begin();
    RTC.begin();
  RTC.adjust(DateTime(__DATE__, __TIME__));
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

void loop() {
  loadlight();
  loadmotion();
  loadtimeshr();
  loadtimesmin();
  loadstimesmin();
  loadstimeshr();
  loadstatus();
  Serial.print("State ");
  Serial.println(State);
  DateTime now = RTC.now();
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
    Serial.println(now.second(), DEC);
    Serial.print(starttimehr);Serial.print(" : ");Serial.println(starttimemin);
    Serial.print(stoptimehr);Serial.print(" : ");Serial.println(stoptimemin);

    if ((now.hour() >= starttimehr and now.minute() >= starttimemin) and (now.hour() <= stoptimehr and now.minute() <= stoptimemin)) {
             if(light >= 500){
              ledState = 1;
              digitalWrite(LED_PIN,ledState);
              digitalWrite(LED_PIN2,ledState);
              }else if(light < 500){
                ledState = 0;
                digitalWrite(LED_PIN,ledState);
                digitalWrite(LED_PIN2,ledState);
                }
         }
       if (now.hour() >= stoptimehr and now.minute() >= stoptimemin) {
              if(ledState == 1 or motion == 1 or State == 1){
                //ledState = 1;
                 digitalWrite(LED_PIN,HIGH);
                 digitalWrite(LED_PIN2,HIGH);
              }else if(ledState == 0 and motion == 0 and State == 0){
                //ledState = 0;
                 digitalWrite(LED_PIN,LOW);
                 digitalWrite(LED_PIN2,LOW);
              }
         }

  Serial.println(ledState);

   debouncer.update(); // Update the Bounce instance
   
   if ( debouncer.fell() ) {  // Call code if button transitions from HIGH to LOW
     ledState = !ledState; // Toggle LED state
     digitalWrite(LED_PIN2,ledState);
     digitalWrite(LED_PIN,ledState); // Apply new LED state
   }
}

void loadlight(){
    if (WiFi.status() == WL_CONNECTED) { 
    HTTPClient http;
    http.begin("http://203.154.83.137/SmartRoom/lightsensor.php?id="+String(idsensor));
    int httpCode = http.GET();
    if (httpCode > 0) {
    String payload = http.getString(); 
    light = payload.toInt();
    Serial.print("Light : ");Serial.println(light);
    }
    http.end();
    }
}

void loadmotion(){
    if (WiFi.status() == WL_CONNECTED) { 
    HTTPClient http;
    http.begin("http://203.154.83.137/SmartRoom/motionSersor.php?sensorid="+String(idsensor));
    int httpCode = http.GET();
    if (httpCode > 0) {
    String payload = http.getString(); 
    motion = payload.toInt();
    Serial.print("Motion : ");Serial.println(motion);
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

void loadstatus(){
    if (WiFi.status() == WL_CONNECTED) { 
    HTTPClient http;
    http.begin("http://203.154.83.137/SmartRoom/onoffswitch.php?mid="+String(idmmachine));
    int httpCode = http.GET();
    if (httpCode > 0) {
    String payload = http.getString(); 
    State = payload.toInt();
    }
    http.end();
    }
}
