#include <AltSoftSerial.h>
#include "TPB23.h"

#define DebugSerial Serial

// AltSoftSerial always uses these pins:
//
// Board          Transmit  Receive   PWM Unusable
// -----          --------  -------   ------------
// Teensy 3.0 & 3.1  21        20         22
// Teensy 2.0         9        10       (none)
// Teensy++ 2.0      25         4       26, 27
// Arduino Uno        9         8         10
// Arduino Leonardo   5        13       (none)
// Arduino Mega      46        48       44, 45
// Wiring-S           5         6          4
// Sanguino          13        14         12


AltSoftSerial NBSerial;

TPB23 TPB23(NBSerial, DebugSerial);

void setup() {
    // put your setup code here, to run once:
    NBSerial.begin(9600);
    DebugSerial.begin(9600);

    /* TPB23 Module Initialization */
    if(TPB23.init()){
        DebugSerial.println("TPB23 Module Error!!!");
    }

    /* Network Regsistraiton Check */
    while(TPB23.canConnect() != 0){
        DebugSerial.println("Network not Ready !!!");
        delay(2000);
    }

    delay(2000);

    DebugSerial.println("TPB23 Module Ready!!!");
}

int loopCount = 0;
int value = 0;
int ret = 0;

void loop() {

#if 0
    /* 1. CGMR, IMEI, CIMI */
    char szCGMR[20];

    if(TPB23.getCGMR(szCGMR, sizeof(szCGMR)) == 0)
    {
        DebugSerial.print("TPB23 CGMR : ");
        DebugSerial.println(szCGMR);
    }

    char szIMEI[16];
    if(TPB23.getIMEI(szIMEI, sizeof(szIMEI)) == 0)
    {
        DebugSerial.print("TPB23 IMEI : ");
        DebugSerial.println(szIMEI);
    }    

    char szCIMI[16];

    if(TPB23.getCIMI(szCIMI, sizeof(szCIMI)) == 0)
    {
        DebugSerial.print("CIMI : ");
        DebugSerial.println(szCIMI);
    }
#endif

#if 0 
    /* 2. RSSI, SignalPower, SNR */

    /* Get CSQ to convert RSSI */
    int rssi;

    if(TPB23.getCSQ(&rssi) == 0)
    {
        DebugSerial.print("RSSI : ");
        DebugSerial.print(rssi);
        DebugSerial.println("");
    }

    /* Get Signal Power */
    int sPower;

    if(TPB23.getSignalPower(&sPower) == 0)
    {
        DebugSerial.print("Signal Power : ");
        DebugSerial.print(sPower);
        DebugSerial.println("");
    }

    /* Get SNR */
    int snR;

    if(TPB23.getSnr(&snR) == 0)
    {
        DebugSerial.print("SNR : ");
        DebugSerial.print(snR);
        DebugSerial.println("");
    }
#endif

#if 0
    /* 3. CFUN setting 0: disable, 1: Full Function */
    if(value == 0)
        value = 1;
    else
        value = 0;

    if(TPB23.setCFUN(value) == 0)
        DebugSerial.println("CFUN set OK");

    delay(2000);

    /* Network Check */
    if(TPB23.getCFUN(&value) == 0)
    {
        DebugSerial.print("CFUN get : ");
        DebugSerial.print(value);
        DebugSerial.println("");
    }  
#endif

#if 0      
    /* 4. UDP Socket Text Echo Test */
    char _IP[] = "195.34.89.241"; //Ublox UDP Test Server
    int  _PORT = 7;

    char sendBuffer[] = "Hello CodeZoo!!!";
    char recvBuffer[32];

    /* 0:TCP, 1:UDP A number in the range 0-65535 except 5683 */
    loopCount++;
    if(TPB23.socketCreate(1024) == 0) 
        DebugSerial.println("Socket Create!!!");

    /* Socket Send */
    if(TPB23.socketSend(_IP, _PORT, sendBuffer, 1) == 0){
        DebugSerial.print("[UDP Send] >>  ");
        DebugSerial.println(sendBuffer);
    }
    else
        DebugSerial.println("Send Fail!!!");

    /* Socket Recv */
    if(TPB23.socketRecv(recvBuffer, sizeof(recvBuffer), 5000) == 0){
        DebugSerial.print("[UDP Recv] >>  ");
        DebugSerial.print(recvBuffer);
        DebugSerial.print("[ ");
        DebugSerial.print(loopCount, DEC);
        DebugSerial.println(" ]");
    }else{
        DebugSerial.println("Recv Fail!!!");
    }

    /* Socket Close */
    if(TPB23.socketClose() == 0)
        DebugSerial.println("Socket Close!!!");
#endif


#if 1
    /* 5. UDP Socket Binary Echo Test */
    char _IP[] = "195.34.89.241"; //Ublox UDP Test Server
    int  _PORT = 7;

    char sendBuffer[32];
    char recvBuffer[32];

    memset(sendBuffer, 0, sizeof(sendBuffer));

    for(int k=0; k<32; k++)
        sendBuffer[k] = k;

    loopCount++;
    /* A number in the range 0-65535 except 5683 */
    if(TPB23.socketCreate(1024) == 0) 
        DebugSerial.println("Socket Create!!!");    

    if( !TPB23.socketSend(_IP, _PORT, sendBuffer, sizeof(sendBuffer), 1) )
        DebugSerial.println("UDP Binary Data Send!!!");
    else
        DebugSerial.println("Send Fail!!!");


    if( !TPB23.socketRecv(recvBuffer, sizeof(recvBuffer), 5000) ){  
        DebugSerial.print("[UDP Recv] [");
        DebugSerial.print(loopCount, DEC);
        DebugSerial.print("]  :  ");

        for(int j=0; j<32; j++){
            DebugSerial.print(recvBuffer[j],DEC);
            DebugSerial.print(',');
        }
        DebugSerial.println();
    }
    else{
        DebugSerial.println("Recv Fail!!!");
    }

    if( !TPB23.socketClose() ){
        DebugSerial.println("Socket Close!!!");
    }
#endif

    delay(2000);      
}
