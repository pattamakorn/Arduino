#include <SoftwareSerial.h>
SoftwareSerial mySerial(11,10); //RX,TX
const int lamp = 7;
void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(lamp,OUTPUT);
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

  if(mySerial.available()>0){
    char x = mySerial.read();
    if(x == 'A'){
      digitalWrite(lamp,LOW);
      Serial.println("Status : A");
      }else if(x == 'B'){
        digitalWrite(lamp,HIGH);
        Serial.println("Status : B");
        }
    }

}
