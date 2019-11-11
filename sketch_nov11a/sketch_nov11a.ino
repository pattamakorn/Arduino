#include <Bounce2.h>

#define BUTTON_PIN_0 D0
#define BUTTON_PIN_1 D3
#define BUTTON_PIN_2 D4
#define BUTTON_PIN_3 D5

//#define LED_PIN 13
Bounce debouncer0 = Bounce();
Bounce debouncer1 = Bounce(); 
Bounce debouncer2 = Bounce(); 
Bounce debouncer3 = Bounce(); 

void setup() {
  pinMode(BUTTON_PIN_0,INPUT_PULLUP);
  debouncer0.attach(BUTTON_PIN_0);
  debouncer0.interval(5);
  pinMode(BUTTON_PIN_1,INPUT_PULLUP);
  debouncer1.attach(BUTTON_PIN_1);
  debouncer1.interval(5);
  pinMode(BUTTON_PIN_2,INPUT_PULLUP);
  debouncer2.attach(BUTTON_PIN_2);
  debouncer2.interval(5);
  pinMode(BUTTON_PIN_3,INPUT_PULLUP);
  debouncer3.attach(BUTTON_PIN_3);
  debouncer3.interval(5);
  
  //pinMode(LED_PIN,OUTPUT);

}

void loop() {
  debouncer0.update();
  debouncer1.update();
  debouncer2.update();
  debouncer3.update();

  int value0 = debouncer0.read();
  int value1 = debouncer1.read();
  int value2 = debouncer2.read();
  int value3 = debouncer3.read();

  Serial.print("V0 = ");Serial.println(value0);
  Serial.print("V1 = ");Serial.println(value1);
  Serial.print("V2 = ");Serial.println(value2);
  Serial.print("V3 = ");Serial.println(value3);

}
