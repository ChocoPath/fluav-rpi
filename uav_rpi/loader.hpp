/*
 * loader.hpp
 *
 *  Created on: 02/09/2014
 *      Author: rhermoso
 */

//-----------------------------------------------------------------------------
#ifndef LOADER_HPP_
#define LOADER_HPP_
//-----------------------------------------------------------------------------
#define UAV_LIB_INVALID_HANDLE		NULL
//-----------------------------------------------------------------------------

#ifdef __RPI__
	typedef void* uav_lib;
	typedef void* uav_func;

#elif defined(ARDUINO)
#else
	#include <windows.h>

	typedef HMODULE uav_lib;
	typedef FARPROC uav_func;

#endif
//-----------------------------------------------------------------------------
#include <string>
#include <map>
//-----------------------------------------------------------------------------

namespace nsUav
{
	//-------------------------------------------------------------------------

	class TLoader
	{
	private:
		std::map<uav_lib, std::string> m_mLibraries;

		void closeAll();
		bool isOpened(uav_lib ulLib);

	protected:
	public:
		TLoader();
		virtual ~TLoader();

		inline size_t size() { return m_mLibraries.size(); }

		uav_lib  open(std::string strFile);
		void     close(uav_lib ulLib);
		uav_func getAddress(uav_lib ulLib, std::string strFunction);

		std::string operator [](uav_lib ulLib);
		uav_lib     operator [](std::string strName);
	};
	//-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
#endif /* LOADER_HPP_ */
//-----------------------------------------------------------------------------
