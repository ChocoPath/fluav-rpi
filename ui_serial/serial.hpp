/*
 * serial.hpp
 *
 *   Serial communications wrapper. This code compiles under Windows and
 *   Linux. Handy for multiplatform projects that use seria lcommunications.
 *
 *   Copyright (C) 2014  Raúl Hermoso Sánchez
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License along
 *   with this program; if not, write to the Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *  Created on: 18/04/2013
 *      Author: Raúl Hermoso Sánchez
 *      e-mail: raul.hermoso@gmail.com
 */

//-----------------------------------------------------------------------------
#ifndef __SERIAL_HPP__
#define __SERIAL_HPP__
//-----------------------------------------------------------------------------
#ifdef __RPI__
	#include <wiringSerial.h>
//	#include <termios.h>

	typedef short   FILETIME;
	typedef int     uav_fd;
//	typedef termios ser_dcb;

	#define USR_INVALID_HANDLE		-1

#elif defined(ARDUINO)

#elif defined(WIN32)
	#include <windows.h>

	typedef HANDLE uav_fd;
	typedef DCB    ser_dcb;

	#define USR_INVALID_HANDLE		NULL
#endif
//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------

namespace nsSerial
{
	//-------------------------------------------------------------------------
	enum TBaudRate    { br110 = 110, br300 = 300, br600 = 600, br1200 = 1200,
					    br2400 = 2400, br4800 = 4800, br9600 = 9600,
			   		    br14400 = 14400, br19200 = 19200, br38400 = 38400,
					    br56000 = 56000, br57600 = 57600, br115200 = 115200,
					    br128000 = 128000, br256000 = 256000 };
	enum TByteSize    { bs4 = 4, bs5, bs6, bs7, bs8 };
	enum TParity      { pNone = 0, pOdd, pEven, pMark, pSpace };
	enum TStopBits    { sb1 = 0, sb3_2, sb2 };
	enum TFlowControl { fcNone = 0, fcHardware, fcSoftware };
	enum TControl     { cNone = 0, cHalfDuplex, cFullDuplex };
	//-------------------------------------------------------------------------

#ifdef WIN32
	struct SSerialConf
	{
		unsigned long BaudRate;
		TByteSize     ByteSize;
		TParity		  Parity;
		TStopBits     StopBits;
		TFlowControl  FlowControl;
		TControl      Control;

		SSerialConf();
		virtual ~SSerialConf();
	};
	//-------------------------------------------------------------------------

	class TSerial;

	typedef void (*evntOnSerial)(void*, nsSerial::TSerial*, char*, unsigned long);
	typedef void (*evntOnError)(void*, nsSerial::TSerial*, char*, int);
#endif
	//-------------------------------------------------------------------------

	class TSerial
	{
	private:
		void*       m_vUser;
		uav_fd      m_hSerial;
		std::string m_strPort;
		std::string m_strError;
#ifdef WIN32
		SSerialConf    m_scSetts;
		ser_dcb        m_dcbSetts;
		ULARGE_INTEGER m_liWriteTime;
		ULARGE_INTEGER m_liReadTime;

		void getTimeNow(ULARGE_INTEGER& liNow);

		void setError(unsigned long ulError, std::string strTitle = "");

		bool setTimeouts();

		void dcb2setts();
		void setts2dcb();

		bool getDCB();
		bool setDCB();
#endif

	protected:
	public:
		TSerial(void* vUser);
		virtual ~TSerial();

		inline void*       user()      { return m_vUser;    }
		inline std::string port()      { return m_strPort;  }
		inline std::string lastError() { return m_strError; }
		inline bool        isOpened()  { return m_hSerial != USR_INVALID_HANDLE; }
#ifdef WIN32
		inline unsigned long& baudRate()    { return m_scSetts.BaudRate;    }
		inline TByteSize&     byteSize()    { return m_scSetts.ByteSize;    }
		inline TParity&       parity()      { return m_scSetts.Parity;      }
		inline TStopBits&     stopBits()    { return m_scSetts.StopBits;    }
		inline TFlowControl&  flowControl() { return m_scSetts.FlowControl; }
		inline TControl&      control()     { return m_scSetts.Control;     }

		inline bool setSettings() { return getDCB(); }
		inline bool getSettings() { return setDCB(); }

		double waitTime();

		bool setConfig(int iBauds, TByteSize bsSize, TParity pParity, TStopBits sbStop);
#endif
		bool open(std::string strPort);
		bool open(std::string strPort, int iBauds, TByteSize bsSize,
				TParity pParity, TStopBits sbStop);
		bool close();

		unsigned long write(char* cData, int iSize);
		int           read(char* cData, int& iSize);
		unsigned long wrrd(char* cData, int iSize);
#ifdef WIN32
		bool          read();
		evntOnSerial onRead;
		evntOnSerial onWrite;
		evntOnSerial onDebug;
		evntOnError  onError;
#endif
	};
	//-------------------------------------------------------------------------
};
//-----------------------------------------------------------------------------
#endif /* __SERIAL_HPP__ */
//-----------------------------------------------------------------------------
