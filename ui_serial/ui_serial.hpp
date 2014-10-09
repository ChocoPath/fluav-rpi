/*
 * ui_serial.hpp
 *
 *   Declaration of the Serial interface for fluav
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
 *  Created on: 01/09/2014
 *      Author: Raúl Hermoso Sánchez
 *      e-mail: raul.hermoso@gmail.com
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
