#include <MCP3008.h>
#define CS_PIN D8
#define CLOCK_PIN D5
#define MOSI_PIN D7
#define MISO_PIN D6
MCP3008 adc(CLOCK_PIN, MOSI_PIN, MISO_PIN, CS_PIN);

void setup() {
 Serial.begin(9600);
  
}


void loop() {
  int val = adc.readADC(7);   
  Serial.println(val);
  delay(2);
  
  
  // iterate thru all channels
  /*
  for (int i=0; i<8; i++) {
   int val = adc.readADC(i);
   Serial.print(val);
   Serial.print("\t");
   }
   Serial.println("");
  */
  
}
