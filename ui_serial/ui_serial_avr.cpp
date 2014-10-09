/*
 * ui_serial.cpp
 *
 *   Defines the serial fluav interface for the Arduino
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
#ifdef ARDUINO
//-----------------------------------------------------------------------------
#include "ui_serial.hpp"

//-----------------------------------------------------------------------------
using namespace nsUav;
//-----------------------------------------------------------------------------

TISerial::TISerial() : TInterface(UII_SERIAL), m_sCom(&Serial), onConfigure(NULL)
{
}
//-----------------------------------------------------------------------------

TISerial::~TISerial()
{
	disconnect();
	m_sCom = NULL;
}
//-----------------------------------------------------------------------------

bool TISerial::connect(char* cParam)
{
	int baud = cParam ? *((int*)cParam) : 115200;

	m_sCom->begin(baud);

	return (*m_sCom) ? true : false;
}
//-----------------------------------------------------------------------------

void TISerial::disconnect()
{
	m_sCom->flush();
	m_sCom->end();
}
//-----------------------------------------------------------------------------

void TISerial::read()
{
	memset(m_cBuffer, 0, UISER_BUFFER_SIZE);
	int size = UISER_BUFFER_SIZE;

	//-- Read from serial port
	if((size = m_sCom->available()))
	{
		size = m_sCom->readBytes(m_cBuffer, size);

		//-- Unserialize data and write values to devices
		if(size)
		{
			char* buff = m_cBuffer;

			m_msg.unserialize(*this, buff);
		}
	}
}
//-----------------------------------------------------------------------------

void TISerial::write(uav_msg umId)
{
	//-- Serialize data from devices and send them over the serial port
	char* data = NULL;

	size_t   siz;

	//-- 1) Serialize data
	if((siz = m_msg.serialize(umId, *this, data)) > 2)
	{
		//-- If there is data serialized, send data over the port
		m_sCom->write(data, (unsigned int)siz);

		delete[] data;
	}
}
//-----------------------------------------------------------------------------
#endif

