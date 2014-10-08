/*
 * serial.cpp
 *
 *  Created on: 18/04/2013
 *      Author: rhermoso
 */
//-----------------------------------------------------------------------------
#include "serial.hpp"
#include <sstream>
#include <cstring>
#include <unistd.h>
//-----------------------------------------------------------------------------
using namespace nsSerial;
//-----------------------------------------------------------------------------

#ifdef WIN32
SSerialConf::SSerialConf()
{
	BaudRate    = br9600;
	ByteSize    = bs8;
	Parity      = pNone;
	StopBits    = sb1;
	FlowControl = fcNone;
	Control     = cNone;
}
//-----------------------------------------------------------------------------

SSerialConf::~SSerialConf()
{
}
#endif
//-----------------------------------------------------------------------------








TSerial::TSerial(void* vUser)
{
	m_vUser     = vUser;
	m_hSerial   = USR_INVALID_HANDLE;
	m_strPort   = "";
	m_strError  = "N/A";
#ifdef WIN32
	memset(&m_dcbSetts, 0, sizeof(ser_dcb));

	onRead  = NULL;
	onWrite = NULL;
	onDebug = NULL;
	onError = NULL;
#endif
}
//-----------------------------------------------------------------------------

TSerial::~TSerial()
{
	close();
#ifdef WIN32
	onRead  = NULL;
	onError = NULL;
#endif
	m_vUser = NULL;
}
//-----------------------------------------------------------------------------

#ifdef WIN32
void TSerial::getTimeNow(ULARGE_INTEGER& liNow)
{
	FILETIME ft_now;

	GetSystemTimeAsFileTime(&ft_now);
	liNow.HighPart = ft_now.dwHighDateTime;
	liNow.LowPart  = ft_now.dwLowDateTime;
}
//-----------------------------------------------------------------------------

void TSerial::setError(unsigned long ulError, std::string strTitle)
{
	LPVOID lpMsgBuf;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL, ulError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&lpMsgBuf, 0, NULL);

	if(strTitle.empty())
		m_strError = std::string((LPTSTR)lpMsgBuf);
	else
		m_strError = strTitle + std::string(": ") + std::string((LPTSTR)lpMsgBuf);

	LocalFree(lpMsgBuf);

	if(onError)
		onError(m_vUser, this, (char*)m_strError.c_str(), ulError);
}
//-----------------------------------------------------------------------------

bool TSerial::setTimeouts()
{
	if(m_hSerial)
	{
		COMMTIMEOUTS cto_times;

		if(GetCommTimeouts(m_hSerial, &cto_times))
		{
			cto_times.ReadIntervalTimeout        = MAXDWORD; // 10;
			cto_times.ReadTotalTimeoutMultiplier = 0; //1;
			cto_times.ReadTotalTimeoutConstant   = 0; //50;
			return SetCommTimeouts(m_hSerial, &cto_times);
		}
	}

	return false;
}
//-----------------------------------------------------------------------------

void TSerial::dcb2setts()
{
	m_scSetts.BaudRate = m_dcbSetts.BaudRate;
	m_scSetts.ByteSize = TByteSize(m_dcbSetts.ByteSize);

	switch(m_dcbSetts.Parity)
	{
	case EVENPARITY:
		m_scSetts.Parity = pEven;
		break;

	case MARKPARITY:
		m_scSetts.Parity = pMark;
		break;

	case NOPARITY:
		m_scSetts.Parity = pNone;
		break;

	case ODDPARITY:
		m_scSetts.Parity = pOdd;
		break;
	}

	m_scSetts.StopBits = TStopBits(m_dcbSetts.StopBits);

	m_scSetts.FlowControl = fcNone;
	m_scSetts.Control     = cNone;
}
//-----------------------------------------------------------------------------

void TSerial::setts2dcb()
{
	m_dcbSetts.BaudRate = m_scSetts.BaudRate;
	m_dcbSetts.ByteSize = BYTE(m_scSetts.ByteSize);

	switch(m_scSetts.Parity)
	{
	case pSpace:
	case pNone:
		m_dcbSetts.Parity = NOPARITY;
		break;

	case pOdd:
		m_dcbSetts.Parity = ODDPARITY;
		break;

	case pEven:
		m_dcbSetts.Parity = EVENPARITY;
		break;

	case pMark:
		m_dcbSetts.Parity = MARKPARITY;
		break;
	}

	m_dcbSetts.StopBits = BYTE(m_scSetts.StopBits);

	m_dcbSetts.fInX         = FALSE;
	m_dcbSetts.fOutX        = FALSE;
	m_dcbSetts.fOutxDsrFlow = FALSE;
	m_dcbSetts.fOutxCtsFlow = FALSE;
	m_dcbSetts.fDtrControl  = DTR_CONTROL_DISABLE;
	m_dcbSetts.fRtsControl  = RTS_CONTROL_DISABLE;
}
//-----------------------------------------------------------------------------

bool TSerial::getDCB()
{
	bool b_ret = false;

	if(m_hSerial && !(b_ret = GetCommState(m_hSerial, &m_dcbSetts)))
		setError(GetLastError(), "getDCB()");
	else
		dcb2setts();

	return b_ret;
}
//-----------------------------------------------------------------------------

bool TSerial::setDCB()
{
	bool b_ret = false;

	setts2dcb();
	if(m_hSerial && !(b_ret = SetCommState(m_hSerial, &m_dcbSetts)))
	{
		setError(GetLastError(), "setDCB()");
	}
	else if(onDebug)
	{
		std::stringstream ss_debug;

		ss_debug << m_strPort << ":" << m_dcbSetts.BaudRate << ","
				 << short(m_dcbSetts.ByteSize) << ","
				 << short(m_dcbSetts.Parity) << ","
				 << short(m_dcbSetts.StopBits);

		onDebug(m_vUser, this, (char*)ss_debug.str().c_str(), ss_debug.str().size());
	}

	return b_ret;
}
//-----------------------------------------------------------------------------

double TSerial::waitTime()
{
	return double(m_liReadTime.QuadPart - m_liWriteTime.QuadPart) / 10000.0;
}
//-----------------------------------------------------------------------------

bool TSerial::setConfig(int iBauds, TByteSize bsSize, TParity pParity, TStopBits sbStop)
{
	bool b_ret = false;

	if((b_ret = getDCB()))
	{
		m_scSetts.BaudRate = iBauds;
		m_scSetts.ByteSize = bsSize;
		m_scSetts.Parity   = pParity;
		m_scSetts.StopBits = sbStop;
		b_ret = setDCB();
	}

	return b_ret;
}
#endif
//-----------------------------------------------------------------------------

bool TSerial::open(std::string strPort)
{
	bool b_ret = false;

	if(!isOpened())
	{
#ifdef __RPI__
		m_hSerial = serialOpen(strPort.c_str(), 115200);

		b_ret = m_hSerial ? true : false;

#elif defined(ARDUINO)
#elif WIN32
		m_strPort = strPort;
		std::string str_dev = "\\\\.\\" + m_strPort;

		m_hSerial = CreateFile(str_dev.c_str(), GENERIC_READ | GENERIC_WRITE, 0,
								NULL, OPEN_EXISTING, 0, NULL);
		if(INVALID_HANDLE_VALUE == m_hSerial)
		{
			setError(GetLastError(), "CreateFile()");
			b_ret = false;
		}
		else if(!(b_ret = SetCommMask(m_hSerial, EV_RXCHAR | EV_TXEMPTY)))
		{
			setError(GetLastError(), "SetCommMask()");
		}
		else
		{
			//-- Se lee la configuraci�n actual del puerto serie
			b_ret = getDCB();
		}

		//-- Se crea el hilo de lectura
		if(b_ret)
			setTimeouts();
#endif
	}

	return b_ret;
}
//-----------------------------------------------------------------------------

bool TSerial::open(std::string strPort, int iBauds, TByteSize bsSize,
				TParity pParity, TStopBits sbStop)
{
	bool b_ret = false;

#ifdef __RPI__
	if(!isOpened())
	{
		m_hSerial = serialOpen(strPort.c_str(), iBauds);

		b_ret = m_hSerial < 0 ? false : true;
	}

#elif defined(ARDUINO)
#elif WIN32
	if(open(strPort))
		return setConfig(iBauds, bsSize, pParity, sbStop);
#endif

	return b_ret;
}
//-----------------------------------------------------------------------------

bool TSerial::close()
{
	bool b_ret = true;
	if(isOpened())
	{
#ifdef __RPI__
		serialFlush(m_hSerial);
		serialClose(m_hSerial);

#elif defined(ARDUINO)
#elif WIN32

		if(!(b_ret = CloseHandle(m_hSerial)))
			setError(GetLastError(), "CloseHadle()");
		else
			m_hSerial = USR_INVALID_HANDLE;
#endif
	}

	return b_ret;
}
//-----------------------------------------------------------------------------

unsigned long TSerial::write(char* cData, int iSize)
{
	unsigned long ul_ret = 0;

	if(!cData)
		return ul_ret;
	if(!isOpened())
		return ul_ret;

#ifdef __RPI__
	ul_ret = ::write(m_hSerial, (const void*)cData, (size_t)iSize);

#elif defined(ARDUINO)
#elif defined(WIN32)
	getTimeNow(m_liWriteTime);

	if(!WriteFile(m_hSerial, cData, iSize, &ul_ret, NULL))
	{
		setError(GetLastError(), "WriteFile()");
		ul_ret = 0;
	}
	else
	{
		if(onWrite)
			onWrite(m_vUser, this, cData, iSize);
	}
#endif

	return ul_ret;
}
//-----------------------------------------------------------------------------

#ifdef WIN32
bool TSerial::read()
{
	char          c_buffer[4096];
	unsigned long ul_bytes_read = 0;
	unsigned long ul_tot        = 0;

	memset(c_buffer, 0, 4096 * sizeof(char));

	do
	{
		if(!ReadFile(m_hSerial, &c_buffer[ul_tot++], 1, &ul_bytes_read, NULL))
		{
			setError(GetLastError(), "ReadFile()");
			ul_bytes_read = 0;
		}
	} while(ul_bytes_read > 0);

	getTimeNow(m_liReadTime);

	if(onRead)
		onRead(m_vUser, this, c_buffer, ul_tot - 1);

	return true;
}
#endif
//-----------------------------------------------------------------------------


int TSerial::read(char* cData, int& iSize)
{
	if(!iSize)
		return -1;
	if(!isOpened())
		return -1;

	memset(cData, 0, iSize * sizeof(char));

#ifdef __RPI__
	iSize = ::read(m_hSerial, (void*)cData, iSize);

#elif defined(ARDUINO)
#elif defined(WIN32)
	unsigned long ul_bytes_read = 0;

	if(!ReadFile(m_hSerial, cData, 1, &ul_bytes_read, NULL))
	{
		setError(GetLastError(), "ReadFile()");
		ul_bytes_read = 0;
		iSize = -1;
	}
	else
	{
		iSize = ul_bytes_read;
	}

	getTimeNow(m_liReadTime);
#endif

	return iSize;
}
//-----------------------------------------------------------------------------

unsigned long TSerial::wrrd(char* cData, int iSize)
{
	unsigned long ul_ret = write(cData, iSize);

	if(ul_ret > 0)
		ul_ret = read(cData, iSize);

	return ul_ret;
}
//-----------------------------------------------------------------------------
