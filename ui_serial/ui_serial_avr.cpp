/*
 * ui_serial.cpp
 *
 *  Created on: 01/09/2014
 *      Author: rhermoso
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

