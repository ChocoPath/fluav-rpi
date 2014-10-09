/*
 * ifc_loader.cpp
 *
 *   Definition of the class TIfcLoader. It loads up an Interface dynamic
 *   library and provides pointers to the create and destroy methods.
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
 *  Created on: 22/09/2014
 *      Author: Raúl Hermoso Sánchez
 *      e-mail: raul.hermoso@gmail.com
 */

//-----------------------------------------------------------------------------
#include "ifc_loader.hpp"
//-----------------------------------------------------------------------------
using namespace nsUav;
//-----------------------------------------------------------------------------
nsUav::TContainer<nsUav::TIfcLoader> Loader;
//-----------------------------------------------------------------------------
TLoader TIfcLoader::m_loader;
uav_id  TIfcLoader::m_nextId = 0;
//-----------------------------------------------------------------------------

TIfcLoader::TIfcLoader() : TObject(m_nextId++)
{
	m_hLib  = UAV_LIB_INVALID_HANDLE;
	create  = NULL;
	destroy = NULL;
}
//-----------------------------------------------------------------------------

TIfcLoader::~TIfcLoader()
{
	free();
}
//-----------------------------------------------------------------------------

bool TIfcLoader::load(std::string strLibName)
{
	if(m_hLib != UAV_LIB_INVALID_HANDLE)
		free();

	m_hLib = m_loader.open(strLibName);

	if(m_hLib != UAV_LIB_INVALID_HANDLE)
	{
		create  = fnc_ifc_create(m_loader.getAddress(m_hLib, "create"));
		destroy = fnc_ifc_destroy(m_loader.getAddress(m_hLib, "destroy"));

		if(create && destroy)
		{
			Loader.insert(this);
			return true;
		}
		else
		{
			m_loader.close(m_hLib);
		}
	}

	return false;
}
//-----------------------------------------------------------------------------

bool TIfcLoader::free()
{
	if(m_hLib != UAV_LIB_INVALID_HANDLE)
	{
		m_loader.close(m_hLib);
		m_hLib = UAV_LIB_INVALID_HANDLE;

		create  = NULL;
		destroy = NULL;
	}

	return true;
}
//-----------------------------------------------------------------------------
