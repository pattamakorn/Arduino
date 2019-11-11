int motorPin1 = D1;// Blue   - 28BYJ48 pin 1

int motorPin2 = D2;// Pink   - 28BYJ48 pin 2

int motorPin3 = D3;// Yellow - 28BYJ48 pin 3

int motorPin4 = D4;// Orange - 28BYJ48 pin 4

                        // Red    - 28BYJ48 pin 5 (VCC)

 

int motorSpeed = 0;     //variable to set stepper speed

int potPin = D0; //potentiometer connected to A2

int potValue = 0; //variable to read A0 input

 

 

//////////////////////////////////////////////////////////////////////////////

void setup() {

  //declare the motor pins as outputs

  pinMode(motorPin1, OUTPUT);

  pinMode(motorPin2, OUTPUT);

  pinMode(motorPin3, OUTPUT);

  pinMode(motorPin4, OUTPUT);

  Serial.begin(9600);

}

 

//////////////////////////////////////////////////////////////////////////////

void loop(){

 

  potValue = analogRead(potPin);     // read the value of the potentiometer

  Serial.println(potValue);          // View full range from 0 - 1024 in Serial Monitor

  if (potValue < 535){               // if potentiometer reads 0 to 535 do this

    motorSpeed = (potValue/15 + 5);  //scale potValue to be useful for motor

    clockwise();                     //go to the ccw rotation function

  }

  else {                             //value of the potentiometer is 512 - 1024

    motorSpeed = ((1024-potValue)/15 + 5); //scale potValue for motor speed

    counterclockwise(); //go the the cw rotation function

  }

}

 

//////////////////////////////////////////////////////////////////////////////

//set pins to ULN2003 high in sequence from 1 to 4

//delay "motorSpeed" between each pin setting (to determine speed)

 

void counterclockwise (){

  // 1

  digitalWrite(motorPin1, HIGH);

  digitalWrite(motorPin2, LOW);

  digitalWrite(motorPin3, LOW);

  digitalWrite(motorPin4, LOW);

  delay(motorSpeed);

  // 2

  digitalWrite(motorPin1, HIGH);

  digitalWrite(motorPin2, HIGH);

  digitalWrite(motorPin3, LOW);

  digitalWrite(motorPin4, LOW);

  delay (motorSpeed);

  // 3

  digitalWrite(motorPin1, LOW);

  digitalWrite(motorPin2, HIGH);

  digitalWrite(motorPin3, LOW);

  digitalWrite(motorPin4, LOW);

  delay(motorSpeed);

  // 4

  digitalWrite(motorPin1, LOW);

  digitalWrite(motorPin2, HIGH);

  digitalWrite(motorPin3, HIGH);

  digitalWrite(motorPin4, LOW);

  delay(motorSpeed);

  // 5

  digitalWrite(motorPin1, LOW);

  digitalWrite(motorPin2, LOW);

  digitalWrite(motorPin3, HIGH);

  digitalWrite(motorPin4, LOW);

  delay(motorSpeed);

  // 6

  digitalWrite(motorPin1, LOW);

  digitalWrite(motorPin2, LOW);

  digitalWrite(motorPin3, HIGH);

  digitalWrite(motorPin4, HIGH);

  delay (motorSpeed);

  // 7

  digitalWrite(motorPin1, LOW);

  digitalWrite(motorPin2, LOW);

  digitalWrite(motorPin3, LOW);

  digitalWrite(motorPin4, HIGH);

  delay(motorSpeed);

  // 8

  digitalWrite(motorPin1, HIGH);

  digitalWrite(motorPin2, LOW);

  digitalWrite(motorPin3, LOW);

  digitalWrite(motorPin4, HIGH);

  delay(motorSpeed);

}

 

//////////////////////////////////////////////////////////////////////////////

//set pins to ULN2003 high in sequence from 4 to 1

//delay "motorSpeed" between each pin setting (to determine speed)

 

void clockwise(){

  // 1

  digitalWrite(motorPin4, HIGH);

  digitalWrite(motorPin3, LOW);

  digitalWrite(motorPin2, LOW);

  digitalWrite(motorPin1, LOW);

  delay(motorSpeed);

  // 2

  digitalWrite(motorPin4, HIGH);

  digitalWrite(motorPin3, HIGH);

  digitalWrite(motorPin2, LOW);

  digitalWrite(motorPin1, LOW);

  delay (motorSpeed);

  // 3

  digitalWrite(motorPin4, LOW);

  digitalWrite(motorPin3, HIGH);

  digitalWrite(motorPin2, LOW);

  digitalWrite(motorPin1, LOW);

  delay(motorSpeed);

  // 4

  digitalWrite(motorPin4, LOW);

  digitalWrite(motorPin3, HIGH);

  digitalWrite(motorPin2, HIGH);

  digitalWrite(motorPin1, LOW);

  delay(motorSpeed);

  // 5

  digitalWrite(motorPin4, LOW);

  digitalWrite(motorPin3, LOW);

  digitalWrite(motorPin2, HIGH);

  digitalWrite(motorPin1, LOW);

  delay(motorSpeed);

  // 6

  digitalWrite(motorPin4, LOW);

  digitalWrite(motorPin3, LOW);

  digitalWrite(motorPin2, HIGH);

  digitalWrite(motorPin1, HIGH);

  delay (motorSpeed);

  // 7

  digitalWrite(motorPin4, LOW);

  digitalWrite(motorPin3, LOW);

  digitalWrite(motorPin2, LOW);

  digitalWrite(motorPin1, HIGH);

  delay(motorSpeed);

  // 8

  digitalWrite(motorPin4, HIGH);

  digitalWrite(motorPin3, LOW);

  digitalWrite(motorPin2, LOW);

  digitalWrite(motorPin1, HIGH);

  delay(motorSpeed);

}
