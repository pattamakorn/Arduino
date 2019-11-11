
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

#define D0 16
#define LBUTTON_PIN D0
#define D2 4
#define Light_PIN D2
#define D3 0
#define Plug_PIN D3
#define D1 5
#define ConfigWiFi_Pin D1 
#define ESP_AP_NAME "Smart Plug"

int ledState = LOW;
int motion;


Bounce debouncer = Bounce(); // Instantiate a Bounce object

int timezone = 7 * 3600;                    //ตั้งค่า TimeZone ตามเวลาประเทศไทย
int dst = 0; 

void setup() {
  
  debouncer.attach(LBUTTON_PIN,INPUT_PULLUP); // Attach the debouncer to a pin with INPUT_PULLUP mode
  debouncer.interval(25); // Use a debounce interval of 25 milliseconds
  
  
  pinMode(Light_PIN,OUTPUT); // Setup the LED
  pinMode(Plug_PIN,OUTPUT);
  pinMode(ConfigWiFi_Pin,INPUT_PULLUP);
  digitalWrite(Light_PIN,ledState);
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
motionsensor();
//if(ledState == 1){
//  Serial.println("On 1");
//  }else{
//    Serial.println("Off 0");
//    }
   
}

void button(){
  debouncer.update(); // Update the Bounce instance
   
   if ( debouncer.fell() ) {  // Call code if button transitions from HIGH to LOW
     ledState = !ledState; // Toggle LED state
     digitalWrite(Light_PIN,ledState); // Apply new LED state
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
    if(p_tm->tm_hour >= 6 and p_tm->tm_hour < 18 ){
      //button();
        digitalWrite(Plug_PIN,HIGH);
        Serial.println("Plug :On");
        Serial.println("Light :off");
      }else if(p_tm->tm_hour >= 0 and p_tm->tm_hour < 6 ){
        button();
        if (motion == 1){digitalWrite(Light_PIN,HIGH);}
        else{digitalWrite(Light_PIN,LOW);}
        }
      else if(p_tm->tm_hour >= 18 and p_tm->tm_hour <= 23 ){
        button();
        if (motion == 1){digitalWrite(Light_PIN,HIGH);}
        else{digitalWrite(Light_PIN,LOW);}
        }
    delay(500);
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
