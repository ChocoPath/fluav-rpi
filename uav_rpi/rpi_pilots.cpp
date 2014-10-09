/*
 * rpi_pilots.cpp
 *
 *   Defines the pilot class. This class stores a "flight" configuration.
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
 *  Created on: 23/09/2014
 *      Author: Raúl Hermoso Sánchez
 *      e-mail: raul.hermoso@gmail.com
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




