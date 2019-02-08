/*
 * A library for controlling TPB23 NB-IoT.
 *
 * @Author Rooney.Jang
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
	int TPB23_init(void);

	/*
	 * Request Manufacturer Revision.
	 */
	void TPB23_CGMR(void);

	/*
	 * Request Product Serial Number (returns the IMEI).
	 */
	void TPB23_CGSN(void);

	/*
	 * Request Change device functionality.
	 */
	int TPB23_IsCFUN(void);


	/*
	 * Reset the module.
	 */
//	void TPB23_reset(void);


	private:
	int sendATcmd(char* szCmd, char* szResponse, int nResponseBufSize, 
			const char* szResponseFilter, unsigned long ulWaitDelay=2000);

	int readATresponseLine(char* szLine, int nLineBufSize,
			const char* szResponseFilter, unsigned long ulDelay);

	void TPB23_serial_clearbuf(void);

	void TPB23_trace(const __FlashStringHelper * szTrace, ... );

	Stream&		_serial;
	Stream&		_debug;
	int			_timeOut = 0;

};
#endif

