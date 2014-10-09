/*
 * ifc_loader.hpp
 *
 *   Declares the class TIfcLoader that will open, close and access an
 *   interface dynamic library.
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
#ifndef IFC_LOADER_HPP_
#define IFC_LOADER_HPP_
//-----------------------------------------------------------------------------
#include "loader.hpp"
#include "rpi_core.hpp"
//-----------------------------------------------------------------------------

namespace nsUav
{
	//-------------------------------------------------------------------------

	typedef void (*fnc_ifc_create)(uav_id, TInterface**);
	typedef void (*fnc_ifc_destroy)(TInterface**);
	//-------------------------------------------------------------------------

	class TIfcLoader : public TObject
	{
	private:
		static uav_id  m_nextId;
		static TLoader m_loader;

		uav_lib m_hLib;

	protected:
	public:
		TIfcLoader();
		~TIfcLoader();

		inline uav_lib libId() { return m_hLib; }

		bool load(std::string strLibName);
		bool free();

		fnc_ifc_create  create;
		fnc_ifc_destroy destroy;
	};
	//-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
extern nsUav::TContainer<nsUav::TIfcLoader> Loader;
//-----------------------------------------------------------------------------
#endif /* IFC_LOADER_HPP_ */
//-----------------------------------------------------------------------------
