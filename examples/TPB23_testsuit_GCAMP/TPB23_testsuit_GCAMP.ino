#include <Wire.h>
#include <Arduino.h>
#include <HTS221.h>
#include "SparkFunLSM6DS3.h"
#include "TPB23.h"

#include <AltSoftSerial.h>
// AltSoftSerial always uses these pins:
//
// Board          Transmit  Receive   PWM Unusable
// -----          --------  -------   ------------
// Arduino Uno        9         8         10

#define DebugSerial    Serial
#define NB_IOT_LED      4
#define NB_IOT_RESET    A1

AltSoftSerial NBSerial;

TPB23 TPB23(NBSerial, DebugSerial);

uint16_t errorsAndWarnings = 0;
LSM6DS3 myIMU;

void setup() {
  pinMode(NB_IOT_LED, OUTPUT);
  pinMode(NB_IOT_RESET, OUTPUT);

/* PIN Reset Sequence */
//  digitalWrite(NB_IOT_RESET, HIGH);
//  delay(100);
//  digitalWrite(NB_IOT_RESET, LOW);
//  delay(100);
    
  //Initiate the Wire library and join the I2C bus
  Wire.begin();

  smeHumidity.begin();

  // put your setup code here, to run once:
  NBSerial.begin(9600);
  DebugSerial.begin(9600);



  /* TPB23 Module Initialization */
  if (TPB23.init()) {
    DebugSerial.println("TPB23 Module Error!!!");
  }

  /* Network Regsistraiton Check */
  while (TPB23.canConnect() != 0) {
    DebugSerial.println("Network not Ready !!!");
    delay(2000);
  }

  /* Network QoS Report to LGU+ (No-charge packet) */
  if (TPB23.reportDevice() == 0)
  {
    DebugSerial.println("TPB23 Device Report Success!!!");
  }

  //Call .begin() to configure the IMU
  myIMU.begin();

  delay(2000);

  DebugSerial.println("TPB23 Module Ready!!!");
}

int value = 0;

void loop() {

#if 0
  /* 1. CGMR, IMEI, CIMI, servingCell */
  char szCGMR[20];

  if (TPB23.getCGMR(szCGMR, sizeof(szCGMR)) == 0)
  {
    DebugSerial.print("TPB23 CGMR : ");
    DebugSerial.println(szCGMR);
  }

  char szIMEI[16];
  if (TPB23.getIMEI(szIMEI, sizeof(szIMEI)) == 0)
  {
    DebugSerial.print("TPB23 IMEI : ");
    DebugSerial.println(szIMEI);
  }

  char szCIMI[16];

  if (TPB23.getCIMI(szCIMI, sizeof(szCIMI)) == 0)
  {
    DebugSerial.print("CIMI : ");
    DebugSerial.println(szCIMI);
  }

  char servingCell[10];
  if (TPB23.getServingCell(servingCell) == 0)
  {
    DebugSerial.print("ServingCell : ");
    DebugSerial.print(servingCell);
    DebugSerial.println("");
  }
  else
  {
    DebugSerial.println("ServingCell don't read");
  }
#endif

#if 0
  /* 2. RSSI, SignalPower, SNR */

  /* Get CSQ to convert RSSI */
  int rssi;

  if (TPB23.getCSQ(&rssi) == 0)
  {
    DebugSerial.print("RSSI : ");
    DebugSerial.print(rssi);
    DebugSerial.println("");
  }

  /* Get Signal Power */
  int sPower;

  if (TPB23.getSignalPower(&sPower) == 0)
  {
    DebugSerial.print("Signal Power : ");
    DebugSerial.print(sPower);
    DebugSerial.println("");
  }

  /* Get SNR */
  int snR;

  if (TPB23.getSnr(&snR) == 0)
  {
    DebugSerial.print("SNR : ");
    DebugSerial.print(snR);
    DebugSerial.println("");
  }
#endif

#if 0
  /* 3. CFUN setting 0: disable, 1: Full Function */
  if (value == 0)
    value = 1;
  else
    value = 0;

  if (TPB23.setCFUN(value) == 0)
    DebugSerial.println("CFUN set OK");

  delay(2000);

  /* Network Check */
  if (TPB23.getCFUN(&value) == 0)
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

  /* A number in the range 0-65535 except 5683 */
  if (TPB23.socketCreate(1024) == 0)
    DebugSerial.println("Socket Create!!!");

  /* Socket Send */
  if (TPB23.socketSend(_IP, _PORT, sendBuffer) == 0) {
    DebugSerial.print("[UDP Send] >>  ");
    DebugSerial.println(sendBuffer);
  }
  else
    DebugSerial.println("Send Fail!!!");

  /* Socket Recv */
  if (TPB23.socketRecv(recvBuffer, sizeof(recvBuffer), 5000) == 0) {
    DebugSerial.print("[UDP Recv] >>  ");
    DebugSerial.println(recvBuffer);
  } else {
    DebugSerial.println("Recv Fail!!!");
  }

  /* Socket Close */
  if (TPB23.socketClose() == 0)
    DebugSerial.println("Socket Close!!!");

  /* NB-IoT Network Detatch */
  if (TPB23.setCFUN(0) == 0)
    DebugSerial.println("NB-IoT Network Detach");

  delay(2000);

  while (1) {
    delay(100);
  }

#endif

#if 0
  /* 5. UDP Socket Binary Echo Test */
  char _IP[] = "195.34.89.241"; //Ublox UDP Test Server
  int  _PORT = 7;

  char sendBuffer[32];
  char recvBuffer[32];

  memset(sendBuffer, 0, sizeof(sendBuffer));
  memset(recvBuffer, 0, sizeof(recvBuffer));

  for (int k = 0; k < 32; k++)
    sendBuffer[k] = k;

  /* A number in the range 0-65535 except 5683 */
  if (TPB23.socketCreate(1024) == 0)
    DebugSerial.println("Socket Create!!!");

  if ( !TPB23.socketSend(_IP, _PORT, sendBuffer, sizeof(sendBuffer)) )
    DebugSerial.println("UDP Binary Data Send!!!");
  else
    DebugSerial.println("Send Fail!!!");


  if ( !TPB23.socketRecv(recvBuffer, sizeof(recvBuffer), 5000) ) {
    DebugSerial.print("[UDP Recv] : ");

    for (int j = 0; j < 32; j++) {
      DebugSerial.print(recvBuffer[j], DEC);
      DebugSerial.print(',');
    }
    DebugSerial.println();
  }
  else {
    DebugSerial.println("Recv Fail!!!");
  }

  if ( !memcmp(sendBuffer, recvBuffer, sizeof(sendBuffer)) )
    DebugSerial.println("Send & Recv OK!!!");

  if ( !TPB23.socketClose() ) {
    DebugSerial.println("Socket Close!!!");
  }

  /* NB-IoT Network Detatch */
  if (TPB23.setCFUN(0) == 0)
    DebugSerial.println("NB-IoT Network Detach");

  delay(2000);

  while (1) {

    digitalWrite(NB_IOT_LED, LOW);
    delay(300);

    digitalWrite(NB_IOT_LED, HIGH);       // turn the LED on
    delay(300);

  }

#endif

#if 1
  /* 5. UDP Socket Temperature & Humidity Report Test */
  char _IP[] = "195.34.89.241"; //Ublox UDP Test Server
  int  _PORT = 7;

  char sendBuffer[32];
  char recvBuffer[32];

  memset(sendBuffer, 0, sizeof(sendBuffer));
  memset(recvBuffer, 0, sizeof(recvBuffer));

  double dataT = 0.0;
  double dataH = 0.0;

  char  temp[8];
  char  humi[8];

  dataH = smeHumidity.readHumidity();

  DebugSerial.print("Humidity   : ");
  DebugSerial.print(dataH);
  DebugSerial.println(" %");

  dataT = smeHumidity.readTemperature();

  DebugSerial.print("Temperature: ");
  DebugSerial.print(dataT);
  DebugSerial.println(" celsius");

  dtostrf(dataT, 5, 2, temp);
  dtostrf(dataH, 5, 2, humi);

  sprintf(sendBuffer, "T:%s,H:%s", temp, humi);


  /* A number in the range 0-65535 except 5683 */
  if (TPB23.socketCreate(1024) == 0)
    DebugSerial.println("Socket Create!!!");

  if ( !TPB23.socketSend(_IP, _PORT, sendBuffer, sizeof(sendBuffer)) )
    DebugSerial.println("UDP Binary Data Send!!!");
  else
    DebugSerial.println("Send Fail!!!");


  /* Socket Recv */
  if (TPB23.socketRecv(recvBuffer, sizeof(recvBuffer), 5000) == 0) {
    DebugSerial.print("[UDP Recv] >>  ");
    DebugSerial.println(recvBuffer);
  } else {
    DebugSerial.println("Recv Fail!!!");
  }

  if ( !memcmp(sendBuffer, recvBuffer, sizeof(sendBuffer)) )
    DebugSerial.println("Send & Recv OK!!!");

  if ( !TPB23.socketClose() ) {
    DebugSerial.println("Socket Close!!!");
  }

  /* NB-IoT Network Detatch */
  if (TPB23.setCFUN(0) == 0)
    DebugSerial.println("NB-IoT Network Detach");

  delay(2000);

  while (1) {

    digitalWrite(NB_IOT_LED, LOW);
    delay(300);   

    //Get all parameters
    DebugSerial.print("\nAccelerometer:\n");
    DebugSerial.print(" X = ");
    DebugSerial.println(myIMU.readFloatAccelX(), 4);
    DebugSerial.print(" Y = ");
    DebugSerial.println(myIMU.readFloatAccelY(), 4);
    DebugSerial.print(" Z = ");
    DebugSerial.println(myIMU.readFloatAccelZ(), 4);

    DebugSerial.print("\nGyroscope:\n");
    DebugSerial.print(" X = ");
    DebugSerial.println(myIMU.readFloatGyroX(), 4);
    DebugSerial.print(" Y = ");
    DebugSerial.println(myIMU.readFloatGyroY(), 4);
    DebugSerial.print(" Z = ");
    DebugSerial.println(myIMU.readFloatGyroZ(), 4);

    DebugSerial.print("\nThermometer:\n");
    DebugSerial.print(" Degrees C = ");
    DebugSerial.println(myIMU.readTempC(), 4);
    DebugSerial.print(" Degrees F = ");
    DebugSerial.println(myIMU.readTempF(), 4);

    DebugSerial.print("\nBus Errors Reported:\n");
    DebugSerial.print(" All '1's = ");
    DebugSerial.println(myIMU.allOnesCounter);
    DebugSerial.print(" Non-success = ");
    DebugSerial.println(myIMU.nonSuccessCounter);

    digitalWrite(NB_IOT_LED, HIGH);       // turn the LED on
    delay(300);

  }

#endif

  delay(10000);
}
