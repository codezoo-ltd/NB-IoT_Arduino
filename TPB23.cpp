/*
 * A library for controlling TPB23 NB-IoT.
 *
 * @Author Rooney.Jang
 * @Date 01/28/2018
 *
 */

#include "Arduino.h"
#include "TPB23.h"

extern "C" {
#include "string.h"
#include "stdlib.h"
}

#define SWIR_TRACE(...)     TPB23_trace(__VA_ARGS__);
#define TRACE_BUFFER_SIZE   256
#define NB_LINE 20    //Limit 20 Line

#define __NB_IOT_DEBUG		//Debug mode 
/*     
 *  @param serial Needs to be an already opened Stream ({Software/Hardware}Serial) 
 *  to write to and read from.
 */ 
TPB23::TPB23(Stream& serial, Stream& debug):    
	_serial(serial), _debug(debug)    
{    
	_timeOut = 2000;    /* default Timeout */     
	_serial.setTimeout( (_timeOut+500) );   /* +500ms, Serial TX/RX Timeout default 2000 */  
}

int	TPB23::TPB23_init()
{
	char szCmd[16];
	char resBuffer[16];
	int cnt = 0;
	int ret;

	delay(3000);
	TPB23_serial_clearbuf();

	strcpy(szCmd, "AT");

	do{

		ret = sendATcmd(szCmd, resBuffer, sizeof(resBuffer), "OK", 3000);

		if(ret==0)
			break;

	}while(cnt<10);

	return ret;
}

void TPB23::TPB23_CGMR()
{
	char szCmd[16];
	char resBuffer[16];

	TPB23_serial_clearbuf();

	strcpy(szCmd, "AT+CGMR");

	sendATcmd(szCmd, resBuffer, sizeof(resBuffer), "OK");
}

void TPB23::TPB23_CGSN()
{
	char szCmd[16];
	char resBuffer[16];

	TPB23_serial_clearbuf();

	strcpy(szCmd, "AT+CGSN=1");

	sendATcmd(szCmd, resBuffer, sizeof(resBuffer), "+CGSN:");

	SWIR_TRACE(F("IMEI : (%s) "), resBuffer);
}

int TPB23::TPB23_IsCFUN()
{
	char szCmd[16];
	char resBuffer[16];
	int ret=0;

	TPB23_serial_clearbuf();

	strcpy(szCmd, "AT+CFUN?");

	if(!sendATcmd(szCmd, resBuffer, sizeof(resBuffer), "+CFUN:"))
	{
		ret = atoi(resBuffer);
	}

	SWIR_TRACE(F("CFUN : (%d) "), ret);

	return ret;
}


int TPB23::sendATcmd(char* szCmd, char* szResponse, int nResponseBufSize, 
		const char* szResponseFilter, unsigned long ulWaitDelay)
{
	int     nRet = 0;

	memset(szResponse, 0, nResponseBufSize);

	SWIR_TRACE(F("sendATcmd (%s) - %d..."), szCmd, ulWaitDelay);

	SWIR_TRACE(F("sendATcmd (%s)..."), szCmd);
	_serial.setTimeout(ulWaitDelay+500);
	_serial.println(szCmd);

	nRet = readATresponseLine(szResponse, nResponseBufSize, szResponseFilter, ulWaitDelay);

	if (nRet == 0)
	{
		SWIR_TRACE(F("...sendATcmd OK"));
	}
	else
	{
		SWIR_TRACE(F("...sendATcmd Fails"));
	}

	return nRet;
}

int TPB23::readATresponseLine(char* szLine, int nLineBufSize, 
		const char* szResponseFilter, unsigned long ulDelay)
{
	int             nbLine = 0;
	char*           aLine[NB_LINE];
	Countdown       oCountdown(ulDelay);
	char *          pszSubstr = NULL;

	memset(szLine, 0, nLineBufSize);

	do
	{
		if (_serial.available())
		{
			String sStr;
			sStr = _serial.readStringUntil('\n');
			int nLen = sStr.length();
			if (nLen > 1)
			{
				aLine[nbLine] = (char *) malloc(nLen+1);
				sStr.toCharArray(aLine[nbLine], nLen);
				aLine[nbLine][nLen] = 0;

				pszSubstr = strstr(aLine[nbLine], "OK");
				if (pszSubstr != NULL)
				{
					nbLine++;
					SWIR_TRACE(F("Found OK"));
					break;
				}

				nbLine++;

			}
		}
		if (nbLine >= NB_LINE)
		{
			break;
		}
	} while (!oCountdown.expired());

	SWIR_TRACE(F("readATresponseLine: %d line(s)\n"), nbLine);

	int i;

	int nRet = 3;

	for (i=0; i<nbLine; i++)
	{
		SWIR_TRACE(F("line[%d]: %s\n"), i, aLine[i]);

		if (szResponseFilter == NULL)
		{
			//Not filtering response
			strcpy(szLine, aLine[i]);
			nRet = 0;
		}
		else if (strlen(szResponseFilter) > 0)
		{
			//Filtering Response
			char * pszSubstr = NULL;

			pszSubstr = strstr(aLine[i], szResponseFilter);
			if (pszSubstr != NULL)
			{
				pszSubstr += strlen(szResponseFilter);
				while (isSpace( *pszSubstr))    //trim heading
				{
					pszSubstr++;
				}
				char * pTemp = pszSubstr;
				while (pTemp < (aLine[i]+strlen(aLine[i])))     //trim ending
				{
					if (*pTemp == '\n') //remove cariage return
					{
						*pTemp = 0; //zero terminate string
						break;
					}
					pTemp++;
				}

				SWIR_TRACE(F("Filtered response: %s\n"), pszSubstr);
				strcpy(szLine, pszSubstr);
				nRet = 0;
			}
		}
		else
		{
			//Not filtering response
			strcpy(szLine, aLine[i]);
			nRet = 0;
		}

		free(aLine[i]);
	}

	return nRet;
}


void TPB23::TPB23_serial_clearbuf()
{
	_serial.flush();

	while(_serial.available()) {
		_serial.read();
	}
}

void TPB23::TPB23_trace(const __FlashStringHelper * szTrace, ... )
{
#ifdef __NB_IOT_DEBUG
	char    szBuf[TRACE_BUFFER_SIZE];
	va_list args;

	va_start (args, szTrace);
#ifdef __AVR__
	vsnprintf_P(szBuf, sizeof(szBuf), (const char *)szTrace, args); // progmem for AVR
#else
	vsnprintf(szBuf, sizeof(szBuf), (const char *)szTrace, args); // for the rest of the world
#endif
	va_end(args);

	_debug.println(szBuf);
#endif	
}

