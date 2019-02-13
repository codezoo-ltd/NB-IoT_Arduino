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

int	TPB23::init()
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

int TPB23::getCGMR(char* szCGMR, int nBufferSize)
{
    char    szCmd[16];
    char*   aLine[NB_LINE];  //let's read up to 20 lines

    strcpy(szCmd, "AT+CGMR");
    int nNbLine = sendATcmd(szCmd, aLine, NB_LINE);

    char*  sLine;
    memset(szCGMR, 0, nBufferSize);

    for (int i=0; i<nNbLine; i++)
    {
        sLine = aLine[i];

        SWIR_TRACE(F("getCGMR, line[%d]: %s\n"), i, sLine);

        char * pTemp = sLine;
        while (pTemp < (sLine+strlen(sLine)))       //trim ending
        {
            if (*pTemp == '\r') //remove cariage return
            {
                *pTemp = 0; //zero terminate string
                break;
            }
            if (*pTemp == '\n') //remove cariage return
            {
                *pTemp = 0; //zero terminate string
                break;
            }
            pTemp++;
        }

        int nLen = strlen(sLine);
        if (nLen != 15)
        {
            continue;
        }
        for (int k=0; k<nLen; k++)
        {
            if (sLine[k] < '0' || sLine[k] > '9')
            {
                continue;
            }
        }
        strcpy(szCGMR, sLine);
        free(aLine[i]);
        free(aLine[i+1]);   /* free "OK" Line memory */
    }

    SWIR_TRACE(F("free memeory %d Bytes\r\n"), TPB23_freeRam());

    return (strlen(szCGMR) > 0 ? 0 : 1);
}

int TPB23::getIMEI(char* szIMEI, int nBufferSize)
{
    char    szCmd[16];
    int     ret;

    memset(szIMEI, 0, nBufferSize);

	strcpy(szCmd, "AT+CGSN=1");
    ret = sendATcmd(szCmd, szIMEI, nBufferSize, "+CGSN:");

    SWIR_TRACE(F("free memeory %d Bytes\r\n"), TPB23_freeRam());

    return ret;
}

int TPB23::getCFUN(int *value)
{
	char    szCmd[16];
	char    resBuffer[16];
	int     ret, _value;

	strcpy(szCmd, "AT+CFUN?");

	ret = sendATcmd(szCmd, resBuffer, sizeof(resBuffer), "+CFUN:");
    
    _value = atoi(resBuffer);
    *value = _value;

    SWIR_TRACE(F("free memeory %d Bytes\r\n"), TPB23_freeRam());

    return ret;
}

int TPB23::setCFUN(int value)
{
	char    szCmd[16];
	char    resBuffer[16];
    int     ret;

	sprintf(szCmd, "AT+CFUN=%d",value);

	ret = sendATcmd(szCmd, resBuffer, sizeof(resBuffer), "OK", 3000);

    SWIR_TRACE(F("free memeory %d Bytes\r\n"), TPB23_freeRam());

    return ret;
}

int TPB23::getCIMI(char* szCIMI, int nBufferSize)
{
    char    szCmd[16];
    char*   aLine[NB_LINE];  //let's read up to 20 lines

    strcpy(szCmd, "AT+CIMI");
    int nNbLine = sendATcmd(szCmd, aLine, NB_LINE);

    char*  sLine;
    memset(szCIMI, 0, nBufferSize);

    for (int i=0; i<nNbLine; i++)
    {
        sLine = aLine[i];

        SWIR_TRACE(F("getCIMI, line[%d]: %s\n"), i, sLine);

        char * pTemp = sLine;
        while (pTemp < (sLine+strlen(sLine)))       //trim ending
        {
            if (*pTemp == '\r') //remove cariage return
            {
                *pTemp = 0; //zero terminate string
                break;
            }
            if (*pTemp == '\n') //remove cariage return
            {
                *pTemp = 0; //zero terminate string
                break;
            }
            pTemp++;
        }

        int nLen = strlen(sLine);
        if (nLen != 15)
        {
            continue;
        }
        for (int k=0; k<nLen; k++)
        {
            if (sLine[k] < '0' || sLine[k] > '9')
            {
                continue;
            }
        }
        strcpy(szCIMI, sLine);
        free(aLine[i]);
        free(aLine[i+1]);   /* free "OK" Line memory */
    }

    SWIR_TRACE(F("free memeory %d Bytes\r\n"), TPB23_freeRam());

    return (strlen(szCIMI) > 0 ? 0 : 1);
}

int TPB23::setPDP()
{
	char    szCmd[16];
	char    resBuffer[16];
    int     ret;

	strcpy(szCmd, "AT+CGDCONT=1,\"IP\",\"\"");

	ret = sendATcmd(szCmd, resBuffer, sizeof(resBuffer), "OK");

    SWIR_TRACE(F("free memeory %d Bytes\r\n"), TPB23_freeRam());

    return ret;
}

int TPB23::setPLMN()
{
	char    szCmd[16];
	char    resBuffer[16];
    int     ret;

	strcpy(szCmd, "AT+COPS=1,2,\"45006\"");

	ret = sendATcmd(szCmd, resBuffer, sizeof(resBuffer), "OK");

    SWIR_TRACE(F("free memeory %d Bytes\r\n"), TPB23_freeRam());

    return ret;
}

int TPB23::canConnect()
{
    char    szATcmd[16];
    char    resBuffer[16];

    sprintf(szATcmd, "AT+CEREG?");
    if (0 == sendATcmd(szATcmd, resBuffer, sizeof(resBuffer), "+CEREG"))
    {
        char * pszState = NULL;

        pszState = strstr(resBuffer, ",");
        if (pszState != NULL)
        {
            pszState++;
            if (*pszState == '1' || *pszState == '5')
            {
                SWIR_TRACE(F("free memeory %d Bytes\r\n"), TPB23_freeRam());
                return 0;//ready to connect. CEREG equals to 1 (home) nor 5 (roaming)
            }
        }
    }

    SWIR_TRACE(F("Not ready to make data call..."));
    return 1;   //not attached to GPRS yet
}

int TPB23::getIP(char* szIP, int nBufferSize)
{
    char    szCmd[16];
    int     ret;

    memset(szIP, 0, nBufferSize);

	strcpy(szCmd, "AT+CGPADDR");
    ret = sendATcmd(szCmd, szIP, nBufferSize, "+CGPADDR:0,",3000);

    SWIR_TRACE(F("free memeory %d Bytes\r\n"), TPB23_freeRam());

    return ret;
}

int TPB23::getCSQ(int *rssi)
{
    char    szATcmd[16];
    char    resBuffer[16];
    int     csq;

    sprintf(szATcmd, "AT+CSQ");
    if (0 == sendATcmd(szATcmd, resBuffer, sizeof(resBuffer), "+CSQ:"))
    {
        csq = atoi(resBuffer);

        if(csq == 99)
            return 1;// Not ready Network
        else{
            SWIR_TRACE(F("free memeory %d Bytes\r\n"), TPB23_freeRam());
            SWIR_TRACE(F("csq %d \r\n"), csq);
            *rssi = -113 + (2 * csq);
            return 0;
        }
    }

    SWIR_TRACE(F("Not ready Network..."));
    return 1;   //not attached to GPRS yet
}

int TPB23::getSignalPower(int *sigPower)
{
    char    szATcmd[16];
    char    resBuffer[16];
	int		signalPower;

    sprintf(szATcmd, "AT+NUESTATS");
    if (0 == sendATcmd(szATcmd, resBuffer, sizeof(resBuffer), "Signal power:"))
    {
		signalPower = atoi(resBuffer);

		SWIR_TRACE(F("free memeory %d Bytes\r\n"), TPB23_freeRam());
		SWIR_TRACE(F("signal Power %d \r\n"), signalPower);

		*sigPower = signalPower;
		return 0;
	}

    SWIR_TRACE(F("get Signal Power Fail..."));
    return 1;
}

int TPB23::getSnr(int *snr)
{
    char    szATcmd[16];
    char    resBuffer[16];
	int		SnR;

    sprintf(szATcmd, "AT+NUESTATS");
    if (0 == sendATcmd(szATcmd, resBuffer, sizeof(resBuffer), "SNR:"))
    {
		SnR = atoi(resBuffer);

		SWIR_TRACE(F("free memeory %d Bytes\r\n"), TPB23_freeRam());
		SWIR_TRACE(F("SnR %d \r\n"), SnR);

		*snr = SnR;
		return 0;
	}

    SWIR_TRACE(F("get Signal to noise ratio Fail..."));
    return 1;
}

int TPB23::socketCreate(int localPort)
{
	char    szCmd[32];
	char	recvBuf[16];
	char*   aLine[NB_LINE];  //let's read up to 20 lines

	// only Datagram/UDP is supported
	sprintf(szCmd, "AT+NSOCR=DGRAM,17,%d,1",localPort);
	int nNbLine = sendATcmd(szCmd, aLine, NB_LINE);

	char*  sLine;
	memset(recvBuf, 0, sizeof(recvBuf));

	for (int i=0; i<nNbLine; i++)
	{   
		sLine = aLine[i];

		char * pTemp = sLine;
		while (pTemp < (sLine+strlen(sLine)))       //trim ending
		{
			if (*pTemp == '\r') //remove cariage return
			{
				*pTemp = 0; //zero terminate string
				break;
			}
			if (*pTemp == '\n') //remove cariage return
			{
				*pTemp = 0; //zero terminate string
				break;
			}
			pTemp++;
		}

		int nLen = strlen(sLine);
		if (nLen != 1) 
		{
			continue;
		}
		for (int k=0; k<nLen; k++)
		{
			if (sLine[k] < '0' || sLine[k] > '6')
			{
				continue;
			}
		}
		strcpy(recvBuf, sLine);

		_nSocket = atoi(recvBuf);
		SWIR_TRACE(F("Socket Number : %d\r\n"), _nSocket);

		free(aLine[i]);
		free(aLine[i+1]);   /* free "OK" Line memory */
	}   

	SWIR_TRACE(F("free memeory %d Bytes\r\n"), TPB23_freeRam());

	return (strlen(recvBuf) > 0 ? 0 : 1); 
}

int TPB23::socketClose()
{
	char    szCmd[16];
	char    resBuffer[16];
	int     ret;

	sprintf(szCmd, "AT+NSOCL=%d",_nSocket);

	ret = sendATcmd(szCmd, resBuffer, sizeof(resBuffer), "OK", 3000);

	SWIR_TRACE(F("free memeory %d Bytes\r\n"), TPB23_freeRam());

	return ret;
}

#define NIBBLE_TO_HEX_CHAR(i) ((i <= 9) ? ('0' + i) : ('A' - 10 + i))
#define HIGH_NIBBLE(i) ((i >> 4) & 0x0F)
#define LOW_NIBBLE(i) (i & 0x0F)

int TPB23::socketSend(char* remoteIP, int remotePort, char* buffer, int size, int echo)
{
	char	resBuffer[16];
	int		ret;

	_serial.setTimeout(10000);
	TPB23_serial_clearbuf();

	if( size > 256 )
		return 1;


	_serial.print("AT+NSOST=");
	_serial.print(_nSocket);
	_serial.print(',');
	_serial.print(remoteIP);

	_debug.println(remoteIP);

	_serial.print(',');
	_serial.print(remotePort);

	_debug.println(remotePort);

	_serial.print(',');
	_serial.print(size);

	_debug.println(size);

	_serial.print(',');

	for (size_t i = 0; i < size; ++i) {
		_serial.print(static_cast<char>(NIBBLE_TO_HEX_CHAR(HIGH_NIBBLE(buffer[i]))));
		_serial.print(static_cast<char>(NIBBLE_TO_HEX_CHAR(LOW_NIBBLE(buffer[i]))));
	}

	ret = readATresponseLine(resBuffer, sizeof(resBuffer), "OK", 3000);

	if(echo)
	{	
		ret = readATresponseLine(resBuffer, sizeof(resBuffer), "+NSONMI:", 5000);
		if(!ret){				
			sscanf(resBuffer, "%*d,%d", &_readUDP);
			SWIR_TRACE(F("readData : %d \r\n"), _readUDP);
			ret = 0;
		}
		SWIR_TRACE(F("free memeory %d Bytes\r\n"), TPB23_freeRam());
	}
	return ret;
}

int TPB23::socketSend(char* remoteIP, int remotePort, const char* str, int echo)
{
	return socketSend(remoteIP, remotePort, (char *)str, strlen(str), echo);
}

#define HEX_CHAR_TO_NIBBLE(c) ((c >= 'A') ? (c - 'A' + 0x0A) : (c - '0'))
#define HEX_PAIR_TO_BYTE(h, l) ((HEX_CHAR_TO_NIBBLE(h) << 4) + HEX_CHAR_TO_NIBBLE(l))
int TPB23::socketRecv(char* buffer, int bufferSize, unsigned long timeout)
{
	char	szCmd[16];
	char	resBuffer[32];
	char	*temp;
	int		ret, tempLength;

	tempLength = (_readUDP*2)+32;

	temp = (char *) malloc(tempLength);

	SWIR_TRACE(F("free memeory %d Bytes\r\n"), TPB23_freeRam());

	sprintf(szCmd, "AT+NSORF=%d,%d",_nSocket, _readUDP);

	ret = sendUDPcmd(szCmd, temp, tempLength, "OK", timeout);

	if(ret)
		return ret;	/* Data Receive Fail */

	memset(buffer, 0, bufferSize);

	for (size_t i = 0; i < _readUDP * 2; i += 2) {
		buffer[i / 2] = HEX_PAIR_TO_BYTE(temp[i], temp[i + 1]);
	}
	
	free(temp);
	return ret;

}

int TPB23::sendUDPcmd(char* szCmd, char* szResponse, int nResponseBufSize,
		const char* szResponseFilter, unsigned long ulWaitDelay)
{
	int     nRet = 0;

	memset(szResponse, 0, nResponseBufSize);

	SWIR_TRACE(F("sendUDPcmd (%s) - %d..."), szCmd, ulWaitDelay);

	_serial.println(szCmd);

	nRet = readUDPresponseLine(szResponse, nResponseBufSize, szResponseFilter, ulWaitDelay);

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

int TPB23::sendATcmd(char* szCmd, char* szResponse, int nResponseBufSize, 
		const char* szResponseFilter, unsigned long ulWaitDelay)
{
	int     nRet = 0;

	memset(szResponse, 0, nResponseBufSize);

	SWIR_TRACE(F("sendATcmd (%s) - %d..."), szCmd, ulWaitDelay);

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

int TPB23::sendATcmd(char * szCmd, char* aLine[], int nMaxLine, unsigned long ulWaitDelay)
{
    int nRet = 0;

    SWIR_TRACE(F("sendATcmd2 (%s) - %d..."), szCmd, ulWaitDelay);

    _serial.setTimeout(ulWaitDelay+500);
	_serial.println(szCmd);

    nRet = readATresponseLine(aLine, nMaxLine, ulWaitDelay);

    if (nRet > 0)
    {
        SWIR_TRACE(F("...sendATcmd OK"));
    }
    else
    {
        SWIR_TRACE(F("...sendATcmd Fails"));
    }

    return nRet;
}


int TPB23::readUDPresponseLine(char* szLine, int nLineBufSize, 
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

	SWIR_TRACE(F("readUDPresponseLine: %d line(s)\n"), nbLine);

	int i;
	int nRet = 1;

	if(nbLine == 2 )
	{
		char *pszSubstr = NULL;

		pszSubstr = strstr(aLine[1], szResponseFilter);
		if (pszSubstr != NULL)
		{
			char *_final = NULL;
			int count=0;

			_final = aLine[0];

			while( count < 4 )
			{
				if(*_final == ',')
				{
					count++;
					_final++;
				}else{
					_final++;
				}
			}
			*( _final + (_readUDP * 2) ) = '\0';//trim ending

			strcpy(szLine, _final);
			SWIR_TRACE(F("Final : %s\n"), szLine);
			nRet = 0;
		}
	}

	for(i=0; i<nbLine; i++)
		free(aLine[i]);

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

int TPB23::readATresponseLine(char* aLine[], int nMaxLine, unsigned long ulDelay)
{
    int             nbLine = 0;
    Countdown       oCountDown(ulDelay);
    char *          pszSubstr = NULL;

    memset(aLine, 0, nMaxLine);

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
        if (nbLine >= nMaxLine)
        {
            break;
        }
    } while (!oCountDown.expired());

    SWIR_TRACE(F("readATresponseLine2: %d line(s)\n"), nbLine);

    return nbLine;
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

int TPB23::TPB23_freeRam()
{
    extern int __heap_start, *__brkval;
    int v;

    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

