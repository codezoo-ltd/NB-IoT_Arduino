#include <SoftwareSerial.h>
#include "TPB23.h"
#define DebugSerial Serial

#define TXpin 11
#define RXpin 10

SoftwareSerial NBSerial(RXpin,TXpin);

TPB23 TPB23(NBSerial, DebugSerial);

void setup() {
  // put your setup code here, to run once:
  NBSerial.begin(9600);
  DebugSerial.begin(9600);

  if(TPB23.TPB23_init()){
    DebugSerial.println("TPB23 Module Error!!!");
  }
  
  DebugSerial.println("TPB23 Module Initialization!!!");
    
}

void loop() {
  // put your main code here, to run repeatedly:
  /* booting */
  TPB23.TPB23_CGMR();
  TPB23.TPB23_CGSN();

  /* Network Check */
  TPB23.TPB23_IsCFUN();
  delay(2000);
}
