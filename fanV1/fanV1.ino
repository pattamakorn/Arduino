#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <time.h>

#define BUTTON_PIN D0
#define LEVEL1_PIN D1
#define LEVEL2_PIN D2
#define LEVEL3_PIN D3
#define FIREBASE_HOST "smart-lobby.firebaseio.com"
#define FIREBASE_AUTH "t4ZZTKulv8DgCqxQP4Zgrc1V6DWhwWQEWT06xeD5"
#define WIFI_SSID "kornpattamakorn"
#define WIFI_PASSWORD "korn2540"

int timezone = 7 * 3600;
int dst = 0; 

void setup() {

  Serial.setDebugOutput(true);
  configTime(timezone, dst, "pool.ntp.org", "time.nist.gov");     //ดึงเวลาจาก Server
    Serial.println("\nWaiting for time");
    while (!time(nullptr)) {
      Serial.print(".");
      delay(1000);
    }
    Serial.println("");
  
  Initialization();
  WiFiConnection();
 
}

void loop() {

  configTime(timezone, dst, "pool.ntp.org", "time.nist.gov");    //ดีงเวลาปัจจุบันจาก Server อีกครั้ง
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
  Serial.print("Time : ");Serial.print(p_tm->tm_hour);
  Serial.print(":");Serial.print(p_tm->tm_min);
  Serial.print(":");Serial.print(p_tm->tm_sec);Serial.println("");

  float temp = Firebase.getFloat("Sensor/TEMP");
  Serial.print("Temperture : ");
  Serial.print(temp);
  Serial.println(" ˚c");
//  if(ledState == 1 or light >=1000){
//    if(p_tm->tm_hour==4 and p_tm->tm_min==56){
//      Serial.println("HIGHT");
//    digitalWrite(LEVEL1_PIN,HIGH);}
//    else{
//      Serial.println("LOW");
//      digitalWrite(LED_PIN,LOW);
//      }
//    }else{
//      Serial.println("LOW");
//      digitalWrite(LED_PIN,LOW);
//      }
  
}


void Initialization(){
  Serial.begin(115200);
  pinMode(LED_BUILTIN,OUTPUT);  
}
void WiFiConnection(){
  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  Serial.print("Connecting...");
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connect : ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH); 
}
