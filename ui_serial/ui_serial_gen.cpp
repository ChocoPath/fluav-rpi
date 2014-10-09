/*
 * ui_serial.cpp
 *
 *   Defines the serial fluav interface for the Raspberry Pi
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
#ifndef ARDUINO
//-----------------------------------------------------------------------------
#include "ui_serial.hpp"
#include "messages.hpp"

#include <sstream>
#include <cstring>
//-----------------------------------------------------------------------------
using namespace nsUav;
//-----------------------------------------------------------------------------

extern "C"
{
	//-------------------------------------------------------------------------

	void create(uav_id uiId, TInterface** pIfc)
	{
		if(!*pIfc)
			*pIfc = new TISerial(uiId);
	}
	//-------------------------------------------------------------------------

	void destroy(TInterface** pIfc)
	{
		if(*pIfc)
			delete *pIfc;
		*pIfc = NULL;
	}
	//-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------






void TISerial::sendConfig()
{
	TMessage msg;
	char*    buff = NULL;
	size_t   siz  = msg.serialize(MSG_CMD_CONFIG, *this, buff);

	if(siz)
	{
		m_sCom.write(buff, siz);
	}
}
//-----------------------------------------------------------------------------

bool TISerial::connect(void* vParam)
{
	std::string str_param((char*)vParam);

	if(!m_sCom.isOpened())
	{
		int         baud = 115200;
		std::string port = str_param;
		size_t      pos  = str_param.find(':');

		if(pos != std::string::npos)
		{
			port = str_param.substr(0, pos);

			std::stringstream ss;

			ss << str_param.substr(pos + 1);
			ss >> baud;
		}

		if(m_sCom.open(port, baud, nsSerial::bs8, nsSerial::pNone, nsSerial::sb1))
			sendConfig();
	}

	return false;
}
//-----------------------------------------------------------------------------

void TISerial::disconnect()
{
	m_sCom.close();
}
//-----------------------------------------------------------------------------

void TISerial::read()
{
	memset(m_cBuffer, 0, UISER_BUFFER_SIZE);
	int size = UISER_BUFFER_SIZE;

	//-- Read from serial port
	size = m_sCom.read(m_cBuffer, size);

	if(size)
	{
		//-- Unserialize data and write values to devices
		TMessage msg;
		char*    buff = m_cBuffer;

		msg.unserialize(*this, buff);
	}
}
//-----------------------------------------------------------------------------

void TISerial::write()
{
	//-- Serialize data from devices and send them over the serial port
	char* data = NULL;

	TMessage msg;
	size_t   siz;

	//-- 1) Serialize data
	if((siz = msg.serialize(MSG_CMD_SET, *this, data)) > 2)
	{
		//-- If there is data serialized, send data over the port
		m_sCom.write(data, siz);

		delete[] data;
	}
}
//-----------------------------------------------------------------------------
#endif

