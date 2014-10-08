/*
 * loader.cpp
 *
 *  Created on: 02/09/2014
 *      Author: rhermoso
 */

//-----------------------------------------------------------------------------
#include "loader.hpp"

#ifdef __RPI__
#include <dlfcn.h>
#endif
//-----------------------------------------------------------------------------
using namespace nsUav;
//-----------------------------------------------------------------------------

TLoader::TLoader()
{
}
//-----------------------------------------------------------------------------

TLoader::~TLoader()
{
	closeAll();
}
//-----------------------------------------------------------------------------

void TLoader::closeAll()
{
	while(m_mLibraries.size())
		close(m_mLibraries.begin()->first);
}
//-----------------------------------------------------------------------------

bool TLoader::isOpened(uav_lib ulLib)
{
	std::map<uav_lib, std::string>::iterator fnd = m_mLibraries.find(ulLib);

	return fnd == m_mLibraries.end() ? false : true;
}
//-----------------------------------------------------------------------------

uav_lib TLoader::open(std::string strFile)
{
	uav_lib ret = (*this)[strFile];

	if(ret == UAV_LIB_INVALID_HANDLE)
	{
#ifdef __RPI__
		ret = dlopen(strFile.c_str(), RTLD_LAZY);

#elif defined(ARDUINO)

#else
		ret = LoadLibrary(strFile.c_str());
#endif
		if(ret == UAV_LIB_INVALID_HANDLE)
		{
			//-- TODO: Process error
			return ret;
		}

		m_mLibraries[ret] = strFile;
	}

	return ret;
}
//-----------------------------------------------------------------------------

void TLoader::close(uav_lib ulLib)
{
	std::map<uav_lib, std::string>::iterator fnd = m_mLibraries.find(ulLib);

	if(fnd != m_mLibraries.end())
	{
#ifdef __RPI__
		dlclose(ulLib);

#elif defined(ARDUINO)

#else
		FreeLibrary(ulLib);
#endif
		m_mLibraries.erase(fnd);
	}
}
//-----------------------------------------------------------------------------

uav_func TLoader::getAddress(uav_lib ulLib, std::string strFunction)
{
	uav_func func = NULL;

	if(isOpened(ulLib))
	{
#ifdef __RPI__
		func = dlsym(ulLib, strFunction.c_str());

#elif defined(ARDUINO)

#else
		func = GetProcAddress(ulLib, strFunction.c_str());
#endif
	}

	return func;
}
//-----------------------------------------------------------------------------

std::string TLoader::operator [](uav_lib ulLib)
{
	std::map<uav_lib, std::string>::iterator fnd = m_mLibraries.find(ulLib);

	return fnd == m_mLibraries.end() ? std::string("") : fnd->second;
}
//-----------------------------------------------------------------------------

uav_lib TLoader::operator [](std::string strFile)
{
	std::map<uav_lib, std::string>::iterator iter = m_mLibraries.begin();

	while(iter != m_mLibraries.end())
	{
		if(iter->second == strFile)
			return iter->first;
		++iter;
	}

	return UAV_LIB_INVALID_HANDLE;
}
//-----------------------------------------------------------------------------
