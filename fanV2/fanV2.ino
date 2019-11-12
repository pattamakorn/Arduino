#include <Bounce2.h>

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

void setup() {
  Serial.begin(115200);
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

   if(ledState0 == 1){
    ledState1 = 0;
    ledState2 = 0;
    ledState3 = 0;
    Serial.print("Num 0 = ");Serial.println(ledState0);
    Serial.print("Num 1 = ");Serial.println(ledState1);
    Serial.print("Num 2 = ");Serial.println(ledState2);
    Serial.print("Num 3 = ");Serial.println(ledState3);
    digitalWrite(LED_PIN_1,HIGH);
    digitalWrite(LED_PIN_2,HIGH);
    digitalWrite(LED_PIN_3,HIGH);
    }
    if(ledState1 == 1){
    ledState0 = 0;
    ledState2 = 0;
    ledState3 = 0;
    Serial.print("Num 0 = ");Serial.println(ledState0);
    Serial.print("Num 1 = ");Serial.println(ledState1);
    Serial.print("Num 2 = ");Serial.println(ledState2);
    Serial.print("Num 3 = ");Serial.println(ledState3);
    digitalWrite(LED_PIN_1,LOW);
    digitalWrite(LED_PIN_2,HIGH);
    digitalWrite(LED_PIN_3,HIGH);
    }
    if(ledState2 == 1){
    ledState1 = 0;
    ledState0 = 0;
    ledState3 = 0;
    Serial.print("Num 0 = ");Serial.println(ledState0);
    Serial.print("Num 1 = ");Serial.println(ledState1);
    Serial.print("Num 2 = ");Serial.println(ledState2);
    Serial.print("Num 3 = ");Serial.println(ledState3);
    digitalWrite(LED_PIN_1,HIGH);
    digitalWrite(LED_PIN_2,LOW);
    digitalWrite(LED_PIN_3,HIGH);
    }
    if(ledState3 == 1){
    ledState1 = 0;
    ledState2 = 0;
    ledState0 = 0;
    Serial.print("Num 0 = ");Serial.println(ledState0);
    Serial.print("Num 1 = ");Serial.println(ledState1);
    Serial.print("Num 2 = ");Serial.println(ledState2);
    Serial.print("Num 3 = ");Serial.println(ledState3);
    digitalWrite(LED_PIN_1,HIGH);
    digitalWrite(LED_PIN_2,HIGH);
    digitalWrite(LED_PIN_3,LOW);
    }
}
