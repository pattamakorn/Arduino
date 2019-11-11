#include <ESP8266WiFi.h>    //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>     //https://github.com/tzapu/WiFiManager
#include <ESP8266HTTPClient.h>
#include <time.h>
//-----Buttom--------
#include <Bounce2.h>
#define BUTTON_PIN D1
int ledState = LOW;
Bounce debouncer = Bounce();
//-------------------
int timezone = 7*3600;
int dst = 0;
// set pin numbers:
#define ledPin  D0
//#define ConfigWiFi_Pin D1 
#define ESP_AP_NAME "ESP8266 Smart Plug"
void setup() 
{
  configWifi();
  debouncer.attach(BUTTON_PIN,INPUT_PULLUP); // Attach the debouncer to a pin with INPUT_PULLUP mode
  debouncer.interval(25);
  Serial.begin(115200);
  Serial.setDebugOutput(true);
}
void loop() 
{
  Button();
  Serial.println(ledState);
  time_t now = time(nullptr);
  struct tm* p_tm=localtime(&now);
  
  Serial.println(ctime(&now));
  Serial.print(p_tm->tm_hour);Serial.print(":");
  Serial.print(p_tm->tm_min);Serial.print(":"); 
  Serial.println(p_tm->tm_sec);
  lightsensor();
  delay(1000);
}


void configWifi(){
  pinMode(ledPin, OUTPUT);
  //pinMode(ConfigWiFi_Pin,INPUT_PULLUP);
  digitalWrite(ledPin,LOW);
  WiFiManager wifiManager;
//  if(digitalRead(ConfigWiFi_Pin) == LOW) // Press button
//  {
    //reset saved settings
//    wifiManager.resetSettings(); // go to ip 192.168.4.1 to config
//    digitalWrite(ledPin,LOW);
//  }
  Button();
  if(ledState == 1){
    wifiManager.resetSettings();
    Serial.println("Reset Wifi");
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

  configTime(timezone, dst, "pool.ntp.org", "time.nist.gov");
    Serial.println("\nWaiting for time");
    while (!time(nullptr)) {
    Serial.print(".");
    delay(1000);}
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
    Serial.print("Light : ");
    Serial.println(light);
    }
    http.end();
    }
}

void Button(){
   debouncer.update(); // Update the Bounce instance
   
   if ( debouncer.fell() ) {  // Call code if button transitions from HIGH to LOW
     ledState = !ledState; // Toggle LED state
     digitalWrite(ledPin,ledState); // Apply new LED state
   }
  }
