#include <ESP8266WiFi.h>    //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>     //https://github.com/tzapu/WiFiManager
#include <ESP8266HTTPClient.h>
#include <time.h>
// set pin numbers:
#define D0 16             // USER LED Wake
#define ledPin  D0
#define D2 4
#define lightPin  D2 
#define D1 5
#define ConfigWiFi_Pin D1 
#define ESP_AP_NAME "Smart Light"


int light;
int motion;
int timezone = 7 * 3600;                    //ตั้งค่า TimeZone ตามเวลาประเทศไทย
int dst = 0; 
void setup() 
{
  // initialize pin D0 as an output.
  pinMode(ledPin, OUTPUT);
  pinMode(lightPin, OUTPUT);
  pinMode(ConfigWiFi_Pin,INPUT_PULLUP);


  Serial.begin(115200);
  Serial.setDebugOutput(true);
  digitalWrite(ledPin,HIGH);//Turn on the LED
  digitalWrite(lightPin,HIGH);
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  if(digitalRead(ConfigWiFi_Pin) == LOW) // Press button
  {
    wifiManager.resetSettings(); // go to ip 192.168.4.1 to config
    digitalWrite(D0,LOW);
  }
  //fetches ssid and password from EEPROM and tries to connect
  //if it does not connect, it starts an access point with the specified name
  //and goes into a blocking loop awaiting configuration
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

  configTime(timezone, dst, "pool.ntp.org", "time.nist.gov");     //ดึงเวลาจาก Server
    Serial.println("\nWaiting for time");
    while (!time(nullptr)) {
      Serial.print(".");
      delay(1000);
    }
    Serial.println("");
  
}
void loop() 
{
 digitalWrite(D0, HIGH);  // turn off the LED  
 delay(2000);             // wait for two seconds
 digitalWrite(D0, LOW);   // turn on the LED
 delay(2000);             // wait for two seconds
 lightsensor();
 motionsensor();
 Time();
 Serial.println("-----------------------------");
 delay(2000);
}

void lightsensor(){
    if (WiFi.status() == WL_CONNECTED) { 
    HTTPClient http;
    http.begin("http://203.154.83.137/SmartRoom/lightsensor.php");
    int httpCode = http.GET();   
    //Serial.println(httpCode);
    if (httpCode > 0) {
    String readlight = http.getString(); 
    light = readlight.toInt();
    Serial.print("Light : ");
    Serial.println(light);
    }
    http.end();
    }
}

void motionsensor(){
  if (WiFi.status() == WL_CONNECTED) { 
    HTTPClient http;
    http.begin("http://203.154.83.137/SmartRoom/motionSersor.php");
    int httpCode = http.GET();   
    //Serial.println(httpCode);
    if (httpCode > 0) {
    String readmotion = http.getString(); 
    motion = readmotion.toInt();
    Serial.print("Motion : ");
    Serial.println(motion);
    }
    http.end();
    }
  }

  void Time(){
    configTime(timezone, dst, "pool.ntp.org", "time.nist.gov");    //ดีงเวลาปัจจุบันจาก Server อีกครั้ง
    time_t now = time(nullptr);
    struct tm* p_tm = localtime(&now);
    Serial.println(ctime(&now));
    Serial.print(p_tm->tm_hour);Serial.print(":");
    Serial.print(p_tm->tm_min);Serial.print(":"); 
    Serial.println(p_tm->tm_sec); 
    if(p_tm->tm_hour >= 7 and p_tm->tm_hour <= 16 ){
      if(light <= 200){
        digitalWrite(lightPin,LOW);
        Serial.println("Light : Off");
        }
      }else if(p_tm->tm_hour >= 0 and p_tm->tm_hour < 7 ){
        if(motion == 1){
          digitalWrite(lightPin,LOW);
          Serial.println("Light : On");
          }else{
            digitalWrite(lightPin,HIGH);
            Serial.println("Light : Off");
            }
        }
    delay(1000);
    }
