/*
 * A library for controlling TPB23 NB-IoT.
 *
 * @Author Rooney.Jang  [CodeZoo]
 * @Date 01/28/2018
 *
 */

#ifndef	TPB23_h
#define	TPB23_h

#include "Countdown.h" 

class TPB23
{
	public:
	/*
     * A simplified constructor taking only a Stream ({Software/Hardware}Serial) object.
     * The serial port should already be initialised when initialising this library.
	 */
	TPB23(Stream& serial, Stream& debug);

	/*
	 * Initialization the module.
	 */
	int init(void);

    /*
	 * Request Manufacturer Revision.
	 */
    int getCGMR(char* szCGMR, int nBufferSize);

	/*
	 * Request Product Serial Number (returns the IMEI).
	 */
    int getIMEI(char* szIMEI, int nBufferSize);

	/*
	 * Request Change device functionality status. (0-1)
	 */
	int getCFUN(int* value);

	/*
	 * Set Change device functionality. (0-1)
	 */
	int setCFUN(int value);

    /*
	 * Request international mobile subscriber identity.
	 */
    int getCIMI(char* szCIMI, int nBufferSize);

    /*
	 * Define PDP Context. (manual)
	 */
	int setPDP(void);

    /*
	 * PLMN selection. (manual)
	 */
    int setPLMN(void);

    /*
	 * EPS network registration status.
	 */
    int canConnect(void);

	/*
	 * Show PDP Address.
	 */
    int getIP(char* szIP, int nBufferSize);

 	/*
	 * Get signal strength indicator.
	 */
    int getCSQ(int* rssi);

	/*
	 * Get serving cell information.
	 */
	int getServingCell(char* servingCell);

	/*
	 * Get signal power.
	 */
    int getSignalPower(int* sigPower);

	/*
	 * Get TX power.
	 */
	int getTxPower(int* txPower);

	/*
	 * Get signal to noise ratio.
	 */
    int getSnr(int* snr);

	/*
	 * Create UDP Socket.	listen port (0-65535, except 5683)
	 */
    int socketCreate(unsigned long localPort, int enableRecv=1);

	/*
	 * Close UDP Socket.
	 */
    int socketClose(void);

	/*
	 * Send UDP Socket.
	 */
	int socketSend(char* remoteIP, unsigned long remotePort, char* buffer, int size);

	int socketSend(char* remoteIP, unsigned long remotePort, const char* str);
	/*
	 * Receive UDP Socket.
	 */
	int socketRecv(char* buffer, int bufferSize, unsigned long timeout);

	/*
	 * Report Device Network Status.
	 */
	int reportDevice(void);

	/*
	 * Experimental function.
	 */
	int disablePSM(void);

	/*
	 * Reset the module.
	 */
	//	void TPB23_reset(void);

	private:

	int sendUDPcmd(char* szCmd, char* szResponse, int nResponseBufSize,
			const char* szResponseFilter, unsigned long ulWaitDelay=2000);

	int sendATcmd(char* szCmd, char* szResponse, int nResponseBufSize, 
			const char* szResponseFilter, unsigned long ulWaitDelay=2000);

    int sendATcmd(char* szCmd, char* aLine[], int nMaxLine,
            unsigned long ulWaitDelay=2000);

	int readUDPresponseLine(char* szLine, int nLineBufSize,
			const char* szResponseFilter, unsigned long ulDelay);

	int readATresponseLine(char* szLine, int nLineBufSize,
			const char* szResponseFilter, unsigned long ulDelay);

    int readATresponseLine(char* aLine[], int nMaxLine, unsigned long ulDelay);

	void TPB23_serial_clearbuf(void);

	void TPB23_trace(const __FlashStringHelper* szTrace, ... );

    int TPB23_freeRam(void);

	Stream&		_serial;
	Stream&		_debug;
	int			_timeOut = 0;
    int         _nSocket;
	int			_readUDP;
	int			_enableRecv;

};
#endif

