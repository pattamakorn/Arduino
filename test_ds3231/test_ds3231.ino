#include <Wire.h>
#include <SPI.h>
#include <RTClib.h>
#include <Bounce2.h>

#define BUTTON_PIN D6

RTC_DS3231 RTC;
int led = D7;
int ledState = LOW;
Bounce debouncer = Bounce(); // Instantiate a Bounce object

//---------------Tag settime---------------//
int starttimehr = 1;
int starttimemin = 25;
int stoptimehr = 1;
int stoptimemin = 27;
//-------------End Tag settime-------------//

void setup () {
    Serial.begin(115200);
    pinMode(led, OUTPUT);
    Wire.begin();
    RTC.begin();
    DateTime now = RTC.now();
//  Switch
  debouncer.attach(BUTTON_PIN,INPUT_PULLUP);
  debouncer.interval(25);
  digitalWrite(led,ledState);
//  Switch
}

void loop () {
    offline();
}
void offline(){
  DateTime now = RTC.now();
    Serial.print("Date : ");Serial.print(now.day(), DEC);Serial.print('/');Serial.print(now.month(), DEC);Serial.print('/');Serial.println(now.year(), DEC);
    Serial.print("Time : ");Serial.print(now.hour(), DEC);Serial.print(':');Serial.print(now.minute(), DEC);Serial.print(':');Serial.println(now.second(), DEC);
    Serial.println("Settime");
    Serial.print("● On  : ");Serial.print(starttimehr);Serial.print(':');Serial.println(starttimemin);
    Serial.print("● Off : ");Serial.print(stoptimehr);Serial.print(':');Serial.println(stoptimemin);
    if(now.hour() == starttimehr && now.minute()== starttimemin){
      debouncer.update();
       if ( debouncer.fell() ) {
         ledState = !ledState;
         digitalWrite(led,ledState);
         Serial.print("State : ");
         Serial.println(ledState);
      }}
      else if(now.hour() == stoptimehr && now.minute()== stoptimemin){
      int aa= 1;
      if(aa == 1){
        digitalWrite(led, HIGH);
        Serial.println("Affter 18:00");
        }
       else{
        digitalWrite(led, LOW);
        Serial.println("Affter 18:00");
        }
      }
    Serial.println();
    delay(1000);
}
void online(){
  
}
