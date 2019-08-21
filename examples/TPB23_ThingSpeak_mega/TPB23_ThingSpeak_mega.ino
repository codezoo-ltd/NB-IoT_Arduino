#include <Time.h>       /* https://github.com/PaulStoffregen/Time */
#include <TimeAlarms.h> /* https://github.com/PaulStoffregen/TimeAlarms */
#include <Wire.h>
#include <Arduino.h>
#include <HTS221.h>     /* https://github.com/codezoo-ltd/sme-hts221-library */
#include "TPB23.h"      /* https://github.com/codezoo-ltd/NB-IoT_Arduino */

#define DebugSerial Serial
#define NBSerial    Serial3

#define NB_IOT_LED      4

String WApiKey = "****************";  //Thing Speak Write API Key
float temp = 0.0;           //Stores temperature value
float humi = 0.0;           //Stores humidity value

String fieldTemp = "field1";    //Air temperature
String fieldHumi = "field2";    //Air humidity

int _sendTag = 0;

TPB23 TPB23(NBSerial, DebugSerial);

void setup() {

	pinMode(NB_IOT_LED, OUTPUT);
	digitalWrite(NB_IOT_LED, LOW); /* NB-IoT LED ON */

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

	char szCCLK[32];
	int _year, _month, _day, _hour, _min, _sec;

	/* Get Time information from the Telco base station */
	if(TPB23.getCCLK(szCCLK, sizeof(szCCLK)) == 0)
	{
		DebugSerial.println(szCCLK);
		sscanf(szCCLK,"%d/%d/%d,%d:%d:%d+*%d",&_year,&_month,&_day,&_hour,&_min,&_sec);

		/* Time Initialization */
		setTime(_hour,_min,_sec,_month,_day,_year);
		digitalWrite(NB_IOT_LED, HIGH); /* NB-IoT LED ON */
	}

	/* Alarm Enable */
	Alarm.timerRepeat(3600, Repeats); //1hour (60sec * 60)

	/* Network QoS Report to LGU+ (No-charge packet) */
	if (TPB23.reportDevice() == 0)
	{
		DebugSerial.println("TPB23 Device Report Success!!!");
	}

	delay(2000);

	DebugSerial.println("TPB23 Module Ready!!!");
}

void Repeats()
{
	DebugSerial.println("alarmed timer!");
	_sendTag = 1;
}

void loop() {

	if(_sendTag){
		_sendTag = 0;

		/* Get NB-IoT Sensor */
		temp = smeHumidity.readTemperature();
		humi = smeHumidity.readHumidity();

		/* 5. TCP Socket Binary Echo Test */
		char _IP[] = "52.22.86.23"; //ThingSpeak Report Server api.thingspeak.com
		unsigned long  _PORT = 80;

		char recvBuffer[1024];
		String  data = "GET /update";
		data += "?api_key=" + WApiKey + "&" + fieldTemp + "=" + String(temp) + "&" + fieldHumi + "=" + String(humi);
		data += " HTTP/1.1\r\n";

		data += "Host: api.thingspeak.com\r\n";
		data += "Connection: close\r\n\r\n";
		/* A number in the range 0-65535 except 5683 */

		if (TPB23.tcpSocketCreate(1024, 0) == 0)
			DebugSerial.println("TCP Socket Create!!!");

		/* Socket Send */
		if (TPB23.tcpSocketConnect(_IP, _PORT) == 0) {
			DebugSerial.println("TCP Socket Connect!!!");
		}

		/* Socket Send */
		if (TPB23.tcpSocketSend(data.c_str()) == 0) {
			DebugSerial.print("[TCP Send] >>  ");
			DebugSerial.println(data);
		}
		else
			DebugSerial.println("Send Fail!!!");

		delay(3000);

		/* Socket Close */
		TPB23.socketClose();

		DebugSerial.println("TCP Socket Close!!!");

	}

	/* Time Test */
	DebugSerial.print(hour());
	DebugSerial.print(":");  
	DebugSerial.print(minute());
	DebugSerial.print(":");    
	DebugSerial.print(second());
	DebugSerial.println("");

	Alarm.delay(1000);

}
