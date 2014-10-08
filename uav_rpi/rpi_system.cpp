/*
 * rpi_system.cpp
 *
 *  Created on: 19/09/2014
 *      Author: rhermoso
 */

//-----------------------------------------------------------------------------
#include "rpi_core.hpp"
#include "ini_file.hpp"
#include "ifc_loader.hpp"
//-----------------------------------------------------------------------------
using namespace nsUav;
//-----------------------------------------------------------------------------

std::string kind2Str(uav_id uiId)
{
	std::string ret;

	switch(uiId)
	{
	case UII_SERIAL:
		ret = "serial";
		break;

	case UII_TCP:
		ret = "tcp/ip";
		break;

	case UII_PWM_RD:
		ret = "pwm (read only)";
		break;

	case UII_PWM:
		ret = "pwm";
		break;

	case UII_I2C_RD:
		ret = "i2c (read only)";
		break;

	case UII_I2C:
		ret = "i2c";
		break;

	case UII_DIGITAL_RD:
		ret = "digital (read only)";
		break;

	case UII_DIGITAL:
		ret = "digital";
		break;

	case UII_ANALOG_RD:
		ret = "analog (read only)";
		break;

	case UII_ANALOG:
		ret = "analog";
		break;

	case UI_INVALID_ID:
		ret = "invalid ID";
		break;

	default:
		ret = "unknown";
	}

	return ret;
}
//-----------------------------------------------------------------------------

uav_id str2Kind(std::string strKind)
{
	uav_id ret = UI_INVALID_ID;

	if(strKind.find("rpwm") != std::string::npos)
		ret = UII_PWM_RD;
	else if(strKind.find("pwm") != std::string::npos)
		ret = UII_PWM;
	else if(strKind.find("ri2c") != std::string::npos)
		ret = UII_I2C_RD;
	else if(strKind.find("i2c") != std::string::npos)
		ret = UII_I2C;
	else if(strKind.find("rdig") != std::string::npos)
		ret = UII_DIGITAL_RD;
	else if(strKind.find("dig") != std::string::npos)
		ret = UII_DIGITAL;
	else if(strKind.find("ranl") != std::string::npos)
		ret = UII_ANALOG_RD;
	else if(strKind.find("anl") != std::string::npos)
		ret = UII_ANALOG;

	return ret;
}
//-----------------------------------------------------------------------------






inline TRPiDevice* rpi_device(std::string strName)
{
	return rpi_item<TRPiDevice, TDevice>(Devices, strName);
}
//-----------------------------------------------------------------------------

inline TRPiInterface* rpi_interface(std::string strName)
{
	return rpi_item<TRPiInterface, TInterface>(Interfaces, strName);
}
//-----------------------------------------------------------------------------






bool rpi_configure_devices(TIniFile& file)
{
	TSection* sec = file["devices"];
	if(sec)
	{
		std::string str_aux;
		short       s_aux;
		TParam*     param;
		TSection*   sc_dev;
		TRPiDevice* dev;

		for(size_t i = 0; i < sec->size(); ++i)
		{
			param  = (*sec)[i];
			sc_dev = file[std::string("dev_") + param->name()];

			if(sc_dev)
			{
				param->value() >> s_aux;
				dev = new TRPiDevice(uav_id(s_aux));
				s_aux = UI_INVALID_ID;

				dev->name()    = param->name();
				dev->enabled() = false;
//				dev->enabled() = param->value().str().find("disabled") == std::string::npos;
				dev->kind()    = str2Kind((*sc_dev)["kind"]->value().str());
				if(dev->kind() == UI_INVALID_ID)
				{
					(*sc_dev)["kind"]->value() >> s_aux; dev->kind() = uav_id(s_aux);
				}
				(*sc_dev)["min"]->value()  >> dev->minimum();
				(*sc_dev)["max"]->value()  >> dev->maximum();

//				if(dev->kind() == UI_INVALID_ID)
//					dev->enabled() = false;

				Devices.insert(dev);
			}
			else
			{
				//-- TODO: No information for declared device!!!
			}
		}
	}
	else
	{
		file.close();
		return false;
	}

	return true;
}
//-----------------------------------------------------------------------------

bool rpi_configure_interfaces(TIniFile& file, std::string& strPath)
{
	TSection*      sec = file["interfaces"];
	TIfcLoader*    ldr = NULL;
	TRPiInterface* ifc = NULL;

	if(sec)
	{
		std::string str_aux;
		short       s_aux;
		TParam*     param;
		TSection*   sc_dev;
		TDevice*    dev;

		for(size_t i = 0; i < sec->size(); ++i)
		{
			ifc    = NULL;
			param  = (*sec)[i];
			sc_dev = file[std::string("int_") + param->name()];

			param->value() >> s_aux;

			if(sc_dev)
			{
				ldr = new TIfcLoader;

				str_aux = strPath + (*sc_dev)["file"]->value().str();
				if(ldr->load(str_aux))
				{
					ldr->create(uav_id(s_aux), (TInterface**)&ifc);

					if(ifc)
					{
						ifc->name()    = param->name();
						ifc->enabled() = false;
//						ifc->enabled() = param->value().str().find("disabled") == std::string::npos;
						(*sc_dev)["connect"]->value() >> ifc->connectParam();

						Interfaces.insert(ifc);

						//-- Load devices for this interface
						for(size_t j = 0; j < sc_dev->size(); ++j)
						{
							dev = rpi_device((*sc_dev)[j]->name());
							if(dev)
								ifc->devices().insert(dev);
						}
					}
					else
					{
						//-- TODO: Could not create interface
						delete ldr;
					}
				}
				else
				{
					//-- TODO: Could not load library
					//   or could not find functions
					delete ldr;
				}
			}
			else
			{
				//-- TODO: No information for declared interface!!!
			}
		}
	}
	else
	{
		file.close();
		return false;
	}

	return true;
}
//-----------------------------------------------------------------------------

uav_id configure_pilots(TIniFile& file)
{
	TSection* sec = file["pilots"];
	uav_id    act = UI_INVALID_ID;

	if(sec)
	{
		TIniFile       pil_file;
		TRPiInterface* ifc;
		TRPiPilot*     pilot;
		std::string    str_aux;
		TParam*        param;
		TSection*      sc_dev;
		bool           b_active;

		for(size_t i = 0; i < sec->size(); ++i)
		{
			param  = (*sec)[i];
			sc_dev = file[std::string("pil_") + param->name()];

			if(sc_dev)
			{
				pilot = new TRPiPilot(Pilots.size());
				pilot->name()    = param->name();
				pilot->enabled() = param->value().str().find("disabled") == std::string::npos;

				if(pilot->enabled())
				{
					b_active = param->value().str().find("default") != std::string::npos;

					str_aux = (*sc_dev)["file"]->value().str();
					if(!str_aux.empty())
					{
						pil_file.loadFile(str_aux);

						sc_dev = pil_file["interfaces"];
						if(sc_dev)
						{
							for(size_t j = 0; j < sc_dev->size(); ++j)
							{
								param = (*sc_dev)[j];
								ifc   = rpi_interface(param->name());

								if(ifc && param->value().str().find("disabled") == std::string::npos)
									pilot->interfaces().insert(ifc);
							}

							if(pilot->interfaces().size())
							{
								Pilots.insert(pilot);

								if(b_active && act == UI_INVALID_ID)
									act = pilot->id();
							}
							else
							{
								//-- TODO: No interfaces defined for the pilot.
								//   Cannot use it.
								delete pilot;
							}
						}
						else
						{
							//-- TODO: No section interfaces found in pilot config file
							//   Cannot use pilot.
							delete pilot;
						}

						pil_file.close();
					}
					else
					{
						//-- TODO: Field 'file' is empty. Cannot use this pilot.
						delete pilot;
					}
				}
				else
				{
					//-- TODO: Pilot disabled in config file. Do not load it.
					delete pilot;
				}
			}
			else
			{
				//-- TODO: No information for declared pilot.
			}
		}
	}

	return act;
}
//-----------------------------------------------------------------------------








void TRPiSystem::setEnablePilot(TRPiPilot* pilot, bool bEnable)
{
	if(!pilot)
		return;

	_map<uav_id, TRPiInterface*>::iterator iifc = pilot->interfaces().begin();
	_map<uav_id, TDevice*>::iterator       idev;

	while(iifc != pilot->interfaces().end())
	{
		iifc->second->enabled() = bEnable;

		idev = iifc->second->devices().begin();
		while(idev != iifc->second->devices().end())
		{
			idev++->second->enabled() = bEnable;
		}
		++iifc;
	}
}
//-----------------------------------------------------------------------------

void TRPiSystem::changePilot(uav_id uiId)
{
	TRPiPilot* act = Pilots.active();

	if(!act || (act && act->id() != uiId))
	{
		//-- Change the active pilot. The steps to follow are:
		//   1. Disable devices and interfaces of current active pilot
		setEnablePilot(act, false);
		//   2. Set new active pilot
		Pilots.setActive(uiId);
		//   3. Enable devices and interfaces of new active pilot
		setEnablePilot(Pilots.active(), true);
	}
}
//-----------------------------------------------------------------------------

void TRPiSystem::configure(char* cParam)
{
	TIniFile file;
	TParam*  param = NULL;

	file.loadFile(std::string(cParam));

	//-- Start by reading the [aircraft] section
	TSection* sec = file["aircraft"];
	if(sec)
	{
		param = (*sec)["name"];
		m_strName = param ? param->value().str() : std::string("Stealth Aircraft??");

		param = (*sec)["libraries"];
		m_strPath = param ? param->value().str() : std::string("./");
	}
	else
	{
		//-- TODO: No aircraft information!!!
		file.close();
		return;
	}

	//-- Next load the devices
	if(rpi_configure_devices(file))
	{
		//-- Load the interfaces to use with the devices
		if(rpi_configure_interfaces(file, m_strPath))
		{
			//-- Load different pilots and set active to default
			changePilot(configure_pilots(file));
		}
	}
}
//-----------------------------------------------------------------------------

void TRPiSystem::connect()
{
	TRPiPilot* pilot = Pilots.active();

	if(pilot)
	{
		_map<uav_id, TRPiInterface*>::iterator iter = pilot->interfaces().begin();

		while(iter != pilot->interfaces().end())
		{
			if(iter->second->enabled())
			{
				//-- TODO: Check whether connection is successful and accordingly
				iter->second->connect((void*)iter->second->connectParam().c_str());
			}
			++iter;
		}
	}
}
//-----------------------------------------------------------------------------

void TRPiSystem::disconnect()
{
	TRPiPilot* pilot = Pilots.active();

	if(pilot)
	{
		_map<uav_id, TRPiInterface*>::iterator iter = pilot->interfaces().begin();

		while(iter != pilot->interfaces().end())
		{
			iter++->second->disconnect();
		}
	}
}
//-----------------------------------------------------------------------------
