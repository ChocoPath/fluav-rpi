/*
 * ifc_loader.hpp
 *
 *  Created on: 22/09/2014
 *      Author: rhermoso
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
