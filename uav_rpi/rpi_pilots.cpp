/*
 * rpi_pilots.cpp
 *
 *  Created on: 23/09/2014
 *      Author: rhermoso
 */

//-----------------------------------------------------------------------------
#include "rpi_core.hpp"
//-----------------------------------------------------------------------------
using namespace nsUav;
//-----------------------------------------------------------------------------
TRPiPilots Pilots;
//-----------------------------------------------------------------------------

uav_id TRPiPilots::setActive(uav_id uiId)
{
	TRPiPilot* aux = (*this)[uiId];

	m_idActive = aux ? aux->id() : UI_INVALID_ID;

	return m_idActive;
}
//-----------------------------------------------------------------------------

uav_id TRPiPilots::setActive(size_t sIdx)
{
	TRPiPilot* aux = (*this)[sIdx];

	m_idActive = aux ? aux->id() : UI_INVALID_ID;

	return m_idActive;
}
//-----------------------------------------------------------------------------

uav_id TRPiPilots::setActive(std::string pltName)
{
	TRPiPilot* aux = (*this)[pltName];

	m_idActive = aux ? aux->id() : UI_INVALID_ID;

	return m_idActive;
}
//-----------------------------------------------------------------------------




