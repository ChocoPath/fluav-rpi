/*
 * ui_serial.hpp
 *
 *  Created on: 01/09/2014
 *      Author: rhermoso
 */

//-----------------------------------------------------------------------------
#ifndef UI_SERIAL_HPP_
#define UI_SERIAL_HPP_
//-----------------------------------------------------------------------------
#ifdef ARDUINO
	#define _serial Serial_*
#else
	#include "serial.hpp"

	#define _serial nsSerial::TSerial
#endif

#include "rpi_core.hpp"
//#include "messages.hpp"
//-----------------------------------------------------------------------------
#define UISER_BUFFER_SIZE		1024
//-----------------------------------------------------------------------------

namespace nsUav
{
	//-------------------------------------------------------------------------

	class TISerial : public TRPiInterface
	{
	private:
		_serial  m_sCom;
		char     m_cBuffer[UISER_BUFFER_SIZE];

		void sendConfig();

	protected:
	public:
		TISerial(uav_id uiId) : TRPiInterface(uiId, UII_SERIAL), m_sCom(NULL) {}
		virtual ~TISerial() { disconnect(); }

#ifdef ARDUINO
		inline bool isConnected() { return Serial ? true : false; }
#endif
		virtual bool connect(void* vParam);
		virtual void disconnect();

		virtual void read();
		virtual void write();
	};
	//-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
#endif /* UI_SERIAL_HPP_ */
//-----------------------------------------------------------------------------
