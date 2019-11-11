
// This example toggles the debug LED (pin 13) on or off
// when a button on pin 2 is pressed.

// Include the Bounce2 library found here :
// https://github.com/thomasfredericks/Bounce2
#include <ESP8266WiFi.h>    //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>     //https://github.com/tzapu/WiFiManager
#include <ESP8266HTTPClient.h>
#include <Bounce2.h>
#include <time.h>

#define D1 5
#define PIN1 D1
#define D2 4
#define PIN2 D2
#define D3 0
#define PIN3 D3
#define D4 2
#define PIN4 D4

#define D5 14
#define ConfigWiFi_Pin D5 
#define ESP_AP_NAME "Smart Morter"



int timezone = 7 * 3600;                    //ตั้งค่า TimeZone ตามเวลาประเทศไทย
int dst = 0; 
int motorSpeed = 5;
int potValue = 535;
int timeStart;
int timeStop;

void setup() {
  
  pinMode(PIN1, OUTPUT);

  pinMode(PIN2, OUTPUT);

  pinMode(PIN3, OUTPUT);

  pinMode(PIN4, OUTPUT);
  pinMode(ConfigWiFi_Pin,INPUT_PULLUP);
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  WiFiManager wifiManager;
  if(digitalRead(ConfigWiFi_Pin) == LOW) // Press button
  {
    wifiManager.resetSettings(); // go to ip 192.168.4.1 to config
    //digitalWrite(D0,LOW);
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
clockwise();
//counterclockwise();
}



    void Time(){
    configTime(timezone, dst, "pool.ntp.org", "time.nist.gov");    //ดีงเวลาปัจจุบันจาก Server อีกครั้ง
    time_t now = time(nullptr);
    struct tm* p_tm = localtime(&now);
    Serial.println(ctime(&now));
    Serial.print(p_tm->tm_hour);Serial.print(":");
    Serial.print(p_tm->tm_min);Serial.print(":"); 
    Serial.println(p_tm->tm_sec); 
//    if(p_tm->tm_hour >= timeStart and p_tm->tm_hour < timeStop ){
//      counterclockwise ();
//      }else{
//        clockwise();
//        }
    }

    void starttime(){
      
      if (WiFi.status() == WL_CONNECTED) { 
          HTTPClient http;
          http.begin("http://203.154.83.137/SmartRoom/timestart.php");
          int httpCode = http.GET();   
          //Serial.println(httpCode);
          if (httpCode > 0) {
          String readtimeStop = http.getString(); 
          timeStop = readtimeStop.toInt();
          Serial.print("timeStop : ");
          Serial.println(timeStop);
          }
          http.end();
          }
      }
      void starttop(){
      
      if (WiFi.status() == WL_CONNECTED) { 
          HTTPClient http;
          http.begin("http://203.154.83.137/SmartRoom/timestop.php");
          int httpCode = http.GET();   
          //Serial.println(httpCode);
          if (httpCode > 0) {
          String readstoptime = http.getString(); 
          timeStop = readstoptime.toInt();
          Serial.print("timeStop : ");
          Serial.println(timeStop);
          }
          http.end();
          }
      }


    void counterclockwise (){

  // 1

  digitalWrite(PIN1, HIGH);

  digitalWrite(PIN2, LOW);

  digitalWrite(PIN3, LOW);

  digitalWrite(PIN4, LOW);

  delay(motorSpeed);

  // 2

  digitalWrite(PIN1, HIGH);

  digitalWrite(PIN2, HIGH);

  digitalWrite(PIN3, LOW);

  digitalWrite(PIN4, LOW);

  delay (motorSpeed);

  // 3

  digitalWrite(PIN1, LOW);

  digitalWrite(PIN2, HIGH);

  digitalWrite(PIN3, LOW);

  digitalWrite(PIN4, LOW);

  delay(motorSpeed);

  // 4

  digitalWrite(PIN1, LOW);

  digitalWrite(PIN2, HIGH);

  digitalWrite(PIN3, HIGH);

  digitalWrite(PIN4, LOW);

  delay(motorSpeed);

  // 5

  digitalWrite(PIN1, LOW);

  digitalWrite(PIN2, LOW);

  digitalWrite(PIN3, HIGH);

  digitalWrite(PIN4, LOW);

  delay(motorSpeed);

  // 6

  digitalWrite(PIN1, LOW);

  digitalWrite(PIN2, LOW);

  digitalWrite(PIN3, HIGH);

  digitalWrite(PIN4, HIGH);

  delay (motorSpeed);

  // 7

  digitalWrite(PIN1, LOW);

  digitalWrite(PIN2, LOW);

  digitalWrite(PIN3, LOW);

  digitalWrite(PIN4, HIGH);

  delay(motorSpeed);

  // 8

  digitalWrite(PIN1, HIGH);

  digitalWrite(PIN2, LOW);

  digitalWrite(PIN3, LOW);

  digitalWrite(PIN4, HIGH);

  delay(motorSpeed);

}

 

//////////////////////////////////////////////////////////////////////////////

//set pins to ULN2003 high in sequence from 4 to 1

//delay "motorSpeed" between each pin setting (to determine speed)

 

void clockwise(){

  // 1

  digitalWrite(PIN4, HIGH);

  digitalWrite(PIN3, LOW);

  digitalWrite(PIN2, LOW);

  digitalWrite(PIN1, LOW);

  delay(motorSpeed);

  // 2

  digitalWrite(PIN4, HIGH);

  digitalWrite(PIN3, HIGH);

  digitalWrite(PIN2, LOW);

  digitalWrite(PIN1, LOW);

  delay (motorSpeed);

  // 3

  digitalWrite(PIN4, LOW);

  digitalWrite(PIN3, HIGH);

  digitalWrite(PIN2, LOW);

  digitalWrite(PIN1, LOW);

  delay(motorSpeed);

  // 4

  digitalWrite(PIN4, LOW);

  digitalWrite(PIN3, HIGH);

  digitalWrite(PIN2, HIGH);

  digitalWrite(PIN1, LOW);

  delay(motorSpeed);

  // 5

  digitalWrite(PIN4, LOW);

  digitalWrite(PIN3, LOW);

  digitalWrite(PIN2, HIGH);

  digitalWrite(PIN1, LOW);

  delay(motorSpeed);

  // 6

  digitalWrite(PIN4, LOW);

  digitalWrite(PIN3, LOW);

  digitalWrite(PIN2, HIGH);

  digitalWrite(PIN1, HIGH);

  delay (motorSpeed);

  // 7

  digitalWrite(PIN4, LOW);

  digitalWrite(PIN3, LOW);

  digitalWrite(PIN2, LOW);

  digitalWrite(PIN1, HIGH);

  delay(motorSpeed);

  // 8

  digitalWrite(PIN4, HIGH);

  digitalWrite(PIN3, LOW);

  digitalWrite(PIN2, LOW);

  digitalWrite(PIN1, HIGH);

  delay(motorSpeed);

}
