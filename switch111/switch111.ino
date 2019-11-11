#include <Bounce2.h>
#include <Wire.h>
#include <SPI.h>
#include <RTClib.h>

#define BUTTON_PIN D6
#define LED_PIN D4
#define LED_PIN2 D7

int ledState = LOW;
Bounce debouncer = Bounce(); // Instantiate a Bounce object
RTC_DS3231 RTC;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  RTC.begin();
//  Switch
  debouncer.attach(BUTTON_PIN,INPUT_PULLUP);
  debouncer.interval(25);
  pinMode(LED_PIN,OUTPUT);
  pinMode(LED_PIN2,OUTPUT);
  digitalWrite(LED_PIN,ledState);
//  Switch
// time
  RTC.adjust(DateTime(__DATE__, __TIME__));
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  DateTime now = RTC.now();
// time
 
}

void loop() {
  DateTime now = RTC.now();
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.println(now.minute(), DEC);
  if(now.hour()>=19 && now.minute()>=18){
    debouncer.update();
   if ( debouncer.fell() ) {
     ledState = !ledState;
     digitalWrite(LED_PIN,ledState);
     digitalWrite(LED_PIN2,ledState);
     Serial.print("State : ");
     Serial.println(ledState);
   }
    }
    else if(now.hour()<=19 && now.minute()<=20){
      }
}
