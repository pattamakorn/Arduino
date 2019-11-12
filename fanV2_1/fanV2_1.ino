#include <Bounce2.h>
#include <ESP8266WiFi.h>    //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>     //https://github.com/tzapu/WiFiManager
#include <ESP8266HTTPClient.h>

#define BUTTON_PIN_3 D1
#define BUTTON_PIN_1 D6
#define BUTTON_PIN_2 D7
#define BUTTON_PIN_0 D5
#define LED_PIN_1 D0
#define LED_PIN_2 D3
#define LED_PIN_3 D4
Bounce debouncer1 = Bounce();
Bounce debouncer2 = Bounce(); 
Bounce debouncer3 = Bounce();
Bounce debouncer4 = Bounce(); 
int ledState0 = LOW;
int ledState1 = LOW;
int ledState2 = LOW;
int ledState3 = LOW;
#define ESP_AP_NAME "fan V.1"
String idmmachine = "fan01";
String idsensor = "sensor1";
float temperat;
int State;
Bounce debouncer = Bounce();

void setup() {
  Serial.begin(9600);
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

    pinMode(BUTTON_PIN_0,INPUT_PULLUP);
    debouncer1.attach(BUTTON_PIN_0);
    debouncer1.interval(5);
    pinMode(BUTTON_PIN_1,INPUT_PULLUP);
    debouncer2.attach(BUTTON_PIN_1);
    debouncer2.interval(5);
    pinMode(BUTTON_PIN_2,INPUT_PULLUP);
    debouncer3.attach(BUTTON_PIN_2);
    debouncer3.interval(5);
    pinMode(BUTTON_PIN_3,INPUT_PULLUP);
    debouncer4.attach(BUTTON_PIN_3);
    debouncer4.interval(5);
  
  
    //Setup the LED :
    pinMode(LED_PIN_1,OUTPUT);
    pinMode(LED_PIN_2,OUTPUT);
    pinMode(LED_PIN_3,OUTPUT);
    digitalWrite(LED_PIN_3,LOW);
}

void loop() {
  debouncer1.update();
  debouncer2.update();
  debouncer3.update();
  debouncer4.update();
  loadtemp();
  loadstatus();
  curent();
  Serial.println(ledState0);
  Serial.println(ledState1);
  Serial.println(ledState2);
  Serial.println(ledState3);
  if ( debouncer1.fell() ) {
    ledState0 = !ledState0;
    Serial.print("Num 0 = ");Serial.println(ledState0);
   }
   if ( debouncer2.fell() ) {
     ledState1 = !ledState1;
     Serial.print("Num 1 = ");Serial.println(ledState1);
     digitalWrite(LED_PIN_1,ledState1);
   }
   if ( debouncer3.fell() ) {
     ledState2 = !ledState2;
     Serial.print("Num 2 = ");Serial.println(ledState2);
     digitalWrite(LED_PIN_2,ledState2);
   }
   if ( debouncer4.fell() ) {
     ledState3 = !ledState3;
     Serial.print("Num 3 = ");Serial.println(ledState3);
     digitalWrite(LED_PIN_3,ledState3);
   }

  if((temperat >= 23 and temperat < 26) or State == 1){
    Serial.println("Num1");
     digitalWrite(LED_PIN_1,HIGH);
     digitalWrite(LED_PIN_2,LOW);
     digitalWrite(LED_PIN_3,LOW);
    }
  if((temperat >= 26 and temperat < 28) or State == 2){
    Serial.println("Num2");
      digitalWrite(LED_PIN_1,LOW);
     digitalWrite(LED_PIN_2,HIGH);
     digitalWrite(LED_PIN_3,LOW);
    }
  if(temperat >= 28 or State == 3){
    Serial.println("Num3");
    digitalWrite(LED_PIN_1,LOW);
     digitalWrite(LED_PIN_2,LOW);
     digitalWrite(LED_PIN_3,HIGH);
    }
  if(temperat < 23 or State == 0){
    Serial.println("Off");
    digitalWrite(LED_PIN_1,LOW);
     digitalWrite(LED_PIN_2,LOW);
     digitalWrite(LED_PIN_3,LOW);
    }
delay(1000);
}

void loadtemp(){
    if (WiFi.status() == WL_CONNECTED) { 
    HTTPClient http;
    http.begin("http://203.154.83.137/SmartRoom/temp.php?id="+String(idsensor));
    int httpCode = http.GET();
    if (httpCode > 0) {
    String payload = http.getString(); 
    temperat = payload.toFloat();
    Serial.print("Light : ");Serial.println(temperat);
    }
    http.end();
    }
}

void loadstatus(){
    if (WiFi.status() == WL_CONNECTED) { 
    HTTPClient http;
    http.begin("http://203.154.83.137/SmartRoom/state.php?mid="+String(idmmachine));
    int httpCode = http.GET();
    if (httpCode > 0) {
    String payload = http.getString(); 
    State = payload.toInt();
    Serial.println(State);
    }
    http.end();
    }
}

void curent(){
    if (WiFi.status() == WL_CONNECTED) { 
      float value = analogRead(A0);
      float curent = ((((value-4)*5)/1024)/0.185);
      Serial.println((((value-4)*5)/1024)/0.185);
      Serial.println(value);
    HTTPClient http;
    http.begin("http://203.154.83.137/SmartRoom/curent.php?id="+String(idmmachine)+"&volt="+String(curent));
    int httpCode = http.GET();
    if (httpCode > 0) {
    String payload = http.getString(); 
    }
    http.end();
    }
}
