/*
 * rpi_devices.hpp
 *
 *   Declares the core classes that is used for the RPI branch of the
 *   fluav framework. It inherits from the uav_core library that can
 *   be found at https://github.com/Di4l/fluav-core.git
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
 *  Created on: 17/09/2014
 *      Author: Raúl Hermoso Sánchez
 *      e-mail: raul.hermoso@gmail.com
 */

//-----------------------------------------------------------------------------
#ifndef RPI_CORE_HPP_
#define RPI_CORE_HPP_
//-----------------------------------------------------------------------------
#include "interface.hpp"
#include "system.hpp"
#include <string>
//-----------------------------------------------------------------------------

namespace nsUav
{
	//-------------------------------------------------------------------------

	template <class T>
	class TRPiContainer : public TContainer<T>
	{
	private:
		typedef TContainer<T> _prnt;

	protected:
	public:
		TRPiContainer() : TContainer<T>() {}
		virtual ~TRPiContainer() {}

		inline T* operator [](uav_id uiId) { return _prnt::operator [](uiId); }
		inline T* operator [](size_t sIdx) { return _prnt::operator [](sIdx); }
		T* operator [](std::string strName);
	};
	//-------------------------------------------------------------------------

	template <class T>
	T* TRPiContainer<T>::operator [](std::string strName)
	{
		typename _map<uav_id, T*>::iterator iter = this->m_mItems.begin();

		while(iter != this->m_mItems.end() && iter->second->name() != strName)
			++iter;

		return iter == this->m_mItems.end() ? NULL : iter->second;
	}
	//-------------------------------------------------------------------------

	template <typename T1, typename T2>
	T1* rpi_item(TContainer<T2>& src, std::string x)
	{
		typename _map<uav_id, T2*>::iterator iter = src.begin();
		T1* ret = NULL;

		while(iter != src.end())
		{
			ret = dynamic_cast<T1*>(iter->second);
			if(ret->name() == x)
				return ret;
			++iter;
		}

		return NULL;
	}
	//-------------------------------------------------------------------------





	class TRPiDevice : public TDevice
	{
	private:
		std::string m_strName;

	protected:
	public:
		TRPiDevice(uav_id uiId) : TDevice(uiId), m_strName("") {}
		virtual ~TRPiDevice() {}

		inline std::string& name() { return m_strName; }
	};
	//-------------------------------------------------------------------------

	class TRPiInterface : public TInterface
	{
	private:
		std::string m_strName;
		std::string m_strConnect;

	protected:
	public:
		TRPiInterface(uav_id uiId, uav_id uiKnd) : TInterface(uiId, uiKnd) {}
		virtual ~TRPiInterface() {}

		inline std::string& name()         { return m_strName;    }
		inline std::string& connectParam() { return m_strConnect; }
	};
	//-------------------------------------------------------------------------

	class TRPiPilot : public TObject
	{
	private:
		std::string               m_strName;
		TContainer<TRPiInterface> m_interfaces;

	protected:
	public:
		TRPiPilot(uav_id uiId) : TObject(uiId), m_strName("") {}
		virtual ~TRPiPilot() {}

		inline TContainer<TRPiInterface>& interfaces() { return m_interfaces; }

		inline std::string& name() { return m_strName; }
	};
	//-------------------------------------------------------------------------

	class TRPiPilots : public TRPiContainer<TRPiPilot>
	{
	private:
		typedef TContainer<TRPiPilot>    _gprnt;
		typedef TRPiContainer<TRPiPilot> _prnt;

		uav_id m_idActive;

	protected:
	public:
		TRPiPilots() : _prnt(), m_idActive(UI_INVALID_ID) {}
		virtual ~TRPiPilots() {}

		inline TRPiPilot* active() { return (*this)[m_idActive]; }

		uav_id setActive(uav_id uiId);
		uav_id setActive(size_t sIdx);
		uav_id setActive(std::string pltName);
	};
	//-------------------------------------------------------------------------

	class TRPiSystem : public TSystem
	{
	private:
		std::string m_strName;
		std::string m_strPath;

		void setEnablePilot(TRPiPilot* pilot, bool bEnable = true);
		void changePilot(uav_id uiId);

	protected:
	public:
		TRPiSystem() : TSystem(), m_strName(""), m_strPath("") {}
		virtual ~TRPiSystem() {}

		inline std::string& name() { return m_strName; }

		void configure(char* cParam);

		void connect();
		void disconnect();
	};
	//-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
std::string kind2Str(uav_id uiId);
uav_id str2Kind(std::string strKind);
//-----------------------------------------------------------------------------
extern nsUav::TRPiPilots Pilots;
//-----------------------------------------------------------------------------
#endif /* RPI_CORE_HPP_ */
//-----------------------------------------------------------------------------
