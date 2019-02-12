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

  if(TPB23.init()){
    DebugSerial.println("TPB23 Module Error!!!");
  }
  
  DebugSerial.println("TPB23 Module Initialization!!!");
    
}
int value = 0;
int ret;
void loop() {
#if 0  
  char szCGMR[16];
  
  // put your main code here, to run repeatedly:
  /* booting */
  if(TPB23.getCGMR(szCGMR, sizeof(szCGMR)) == 0)
  {
    DebugSerial.println(szCGMR);
    DebugSerial.println("Thank you!!!");
  }

  if(TPB23.getIMEI(szCGMR, sizeof(szCGMR)) == 0)
  {
    DebugSerial.println(szCGMR);
    DebugSerial.println("Thank you2!!!");
  }  
#endif

#if 0
if(value == 0)
  value = 1;
else
  value = 0;
  
  if(!TPB23.setCFUN(value))
    DebugSerial.println("CFUN set OK");

  delay(2000);
  /* Network Check */
  
  if( !TPB23.getCFUN(&value) )
  {
    DebugSerial.print("CFUN : ");
    DebugSerial.print(value);
    DebugSerial.println("");
  }
  char szCIMI[16];
  
  // put your main code here, to run repeatedly:
  /* network check */
  if(TPB23.getCIMI(szCIMI, sizeof(szCIMI)) == 0)
  {
    DebugSerial.println(szCIMI);
    DebugSerial.println("Thank you!!!");
  }

  value = 0;
  
  if(!TPB23.setCFUN(value))
    DebugSerial.println("CFUN set OK");

  delay(2000);

  value = 1;
  
  if(!TPB23.setCFUN(value))
    DebugSerial.println("CFUN set OK");

  delay(2000);

  if(!TPB23.setPDP())
    DebugSerial.println("AT+CGDCONT=1,\"IP\",\"\"");
    
  delay(2000);

  if(!TPB23.setPLMN())
    DebugSerial.println("AT+COPS=1,2,\"45006\"");

  if(!TPB23.canConnect())
    DebugSerial.println("Ready to Network !!!");

  char szIP[16];
  
  // put your main code here, to run repeatedly:
  /* network IP check */
  if(TPB23.getIP(szIP, sizeof(szIP)) == 0)
  {
    DebugSerial.println(szIP);
    DebugSerial.println("Thank you!!!");
  }
    
#endif
int rssi;
  if( !TPB23.getCSQ(&rssi) )
  {
    DebugSerial.print("RSSI : ");
    DebugSerial.print(rssi);
    DebugSerial.println("");
  }


  delay(2000);
    
}
