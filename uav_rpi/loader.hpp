/*
 * loader.hpp
 *
 *   Declares the class TLoader that will open and close a dynamic library. This
 *   library is multiplatform (or should be), making it an ideal interface for
 *   your multiplatform projects.
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
 *  Created on: 02/09/2014
 *      Author: Raúl Hermoso Sánchez
 *      e-mail: raul.hermoso@gmail.com
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
