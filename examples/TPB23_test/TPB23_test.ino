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
  
  delay(2000);
  
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

int rssi;
  if( !TPB23.getCSQ(&rssi) )
  {
    DebugSerial.print("RSSI : ");
    DebugSerial.print(rssi);
    DebugSerial.println("");
  }

int sPower;
  if( !TPB23.getSignalPower(&sPower) )
  {
    DebugSerial.print("Signal Power : ");
    DebugSerial.print(sPower);
    DebugSerial.println("");
  }

int snR;
  if( !TPB23.getSnr(&snR) )
  {
    DebugSerial.print("SNR : ");
    DebugSerial.print(snR);
    DebugSerial.println("");
  }

/* UDP Socket Create & Close */
  if( !TPB23.socketCreate(1024) ) /* A number in the range 0-65535 except 5683 */
    DebugSerial.println("Socket Create!!!");
  
  delay(2000);

  if( !TPB23.socketClose() )
    DebugSerial.println("Socket Close!!!");


    
#endif
/* Ublox UDP server */
//char _IP[] = "195.34.89.241";
//int  _PORT = 7;

/* CodeZoo UDP server */
char _IP[] = "175.193.45.24";
int  _PORT = 5203;

//char sendBuffer[] = "Hello CodeZoo !!!";
char sendBuffer[32];
char recvBuffer[32];

memset(sendBuffer, 0, sizeof(sendBuffer));

for(int k=0; k<32; k++)
  sendBuffer[k] = k;


/* UDP Socket Create & Close */
  if( !TPB23.socketCreate(1024) ) /* A number in the range 0-65535 except 5683 */
    DebugSerial.println("Socket Create!!!");
    
if( !TPB23.socketSend(_IP, _PORT, sendBuffer, sizeof(sendBuffer), 1) )
  DebugSerial.println("UDP Send!!!");
else
  DebugSerial.println("Send Fail!!!");
  

if( !TPB23.socketRecv(recvBuffer, sizeof(recvBuffer), 5000) ){  
  DebugSerial.println("UDP Recv>>>>>>>>>>>>>>>>\r\n");
  for(int j=0; j<32; j++){
    DebugSerial.print(recvBuffer[j],DEC);
    DebugSerial.print(',');
  }
  DebugSerial.println();
}
else
  DebugSerial.println("Recv Fail!!!");


  if( !TPB23.socketClose() )
    DebugSerial.println("Socket Close!!!");  
  delay(2000);      
}
