#include <ESP8266WiFi.h>    //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>     //https://github.com/tzapu/WiFiManager
#include <ESP8266HTTPClient.h>
#include <time.h>

#define D0 16
#define Plug_PIN D0
#define D1 5
#define ConfigWiFi_Pin D1 
#define ESP_AP_NAME "Smart Plug"

int timezone = 7 * 3600;                    //ตั้งค่า TimeZone ตามเวลาประเทศไทย
int dst = 0; 
int str;
int stp;
String wid = "plug";

void setup() {
  pinMode(Plug_PIN,OUTPUT);
  pinMode(ConfigWiFi_Pin,INPUT_PULLUP);
  Serial.begin(115200);
  Serial.setDebugOutput(true);

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
  //digitalWrite(ledPin,HIGH);
    configTime(timezone, dst, "pool.ntp.org", "time.nist.gov");     //ดึงเวลาจาก Server
    Serial.println("\nWaiting for time");
    while (!time(nullptr)) {
      Serial.print(".");
      delay(1000);
    }
    Serial.println("");
 
}

void loop() {
Time();
timestart();
timestop();
Serial.println(str);
Serial.println(stp);
}
void Time(){
    configTime(timezone, dst, "pool.ntp.org", "time.nist.gov");    //ดีงเวลาปัจจุบันจาก Server อีกครั้ง
    time_t now = time(nullptr);
    struct tm* p_tm = localtime(&now);
    Serial.println(ctime(&now));
    Serial.print(p_tm->tm_hour);Serial.print(":");
    Serial.print(p_tm->tm_min);Serial.print(":"); 
    Serial.println(p_tm->tm_sec); 
    if(p_tm->tm_hour >= str and p_tm->tm_hour < stp ){
      digitalWrite(Plug_PIN,HIGH);
      Serial.println("State : On");
      Serial.print("Start :");
      Serial.println(str);
      Serial.print("Stop :");
      Serial.println(stp); 
      }else{
        digitalWrite(Plug_PIN,LOW);
        Serial.println("State : Off");
      }
    delay(500);
}
void timestart(){
  if (WiFi.status() == WL_CONNECTED) { 
    HTTPClient http;
    http.begin("http://203.154.83.137/SmartRoom/timestart.php?id="+String(wid));
    int httpCode = http.GET();   
    //Serial.println(httpCode);
    if (httpCode > 0) {
    String readstart = http.getString(); 
    str = readstart.toInt();
//    Serial.print("str : ");
//    Serial.println(str);
    }
    http.end();
    }
  }
void timestop(){
  if (WiFi.status() == WL_CONNECTED) { 
    HTTPClient http;
    http.begin("http://203.154.83.137/SmartRoom/timestop.php?id="+String(wid));
    int httpCode = http.GET();   
    //Serial.println(httpCode);
    if (httpCode > 0) {
    String readstop = http.getString(); 
    stp = readstop.toInt();
//    Serial.print("stp : ");
//    Serial.println(stp);
    }
    http.end();
    }
  }
