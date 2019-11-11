#include <Bounce2.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <time.h>

#define BUTTON_PIN D0
#define LED_PIN D1
#define FIREBASE_HOST "smart-lobby.firebaseio.com"
#define FIREBASE_AUTH "t4ZZTKulv8DgCqxQP4Zgrc1V6DWhwWQEWT06xeD5"
#define WIFI_SSID "paticamp"
#define WIFI_PASSWORD "22221111"

int ledState = LOW;
int timezone = 7 * 3600;
int dst = 0; 


Bounce debouncer = Bounce();

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
  
  debouncer.attach(BUTTON_PIN,INPUT_PULLUP); // Attach the debouncer to a pin with INPUT_PULLUP mode
  debouncer.interval(25); // Use a debounce interval of 25 milliseconds
  
  
  pinMode(LED_PIN,OUTPUT); // Setup the LED
  digitalWrite(LED_PIN,ledState);
 
}

void loop() {

  configTime(timezone, dst, "pool.ntp.org", "time.nist.gov");    //ดีงเวลาปัจจุบันจาก Server อีกครั้ง
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
  Serial.print("Time : ");Serial.print(p_tm->tm_hour);
  Serial.print(":");Serial.print(p_tm->tm_min);
  Serial.print(":");Serial.print(p_tm->tm_sec);Serial.println("");
  //delay(1000);
  
  button();
  float light = Firebase.getFloat("Sensor/LIGHT");
  int stHr = Firebase.getFloat("SetSwitch/StartHour");
  int stMin = Firebase.getFloat("SetSwitch/StartMunite");
  int stpHr = Firebase.getFloat("SetSwitch/StopHour");
  int stpMin = Firebase.getFloat("SetSwitch/StopMunite");
  Serial.println(light);
  if(ledState == 1 or light >=1000){
    if(p_tm->tm_hour >= stHr and p_tm->tm_min >= stMin){
      Serial.println("LOW");
    digitalWrite(LED_PIN,LOW);}
    else{
      Serial.println("HIGH");
      digitalWrite(LED_PIN,HIGH);
      }
    }else{
      Serial.println("HIGH");
      digitalWrite(LED_PIN,HIGH);
      }
  
}

void button()
{
   debouncer.update(); // Update the Bounce instance
   
   if ( debouncer.fell() ) {  // Call code if button transitions from HIGH to LOW
     ledState = !ledState; // Toggle LED state
     Serial.println(ledState);
   }
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
