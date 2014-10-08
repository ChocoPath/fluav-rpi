/*
 * ifc_loader.cpp
 *
 *  Created on: 22/09/2014
 *      Author: rhermoso
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
