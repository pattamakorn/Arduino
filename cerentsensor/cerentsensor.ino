void setup() {

Serial.begin(9600);

}

void loop() {

float value = analogRead(A0);
//Serial.println((((value-871)*5)/1024)/0.185);
for (int i = 0 ; i<= 10;i++){
  float kk = (((value-871)*5)/1024)/0.185;
  Serial.println(kk+k);
    }
 
//Serial.println(value);
delay(250);

}
