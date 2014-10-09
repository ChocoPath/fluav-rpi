/*
 * ini_file.cpp
 *
 *   Defines the classes that will allow access a .ini configuration file.
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
 *  Created on: 15/03/2012
 *      Author: Raúl Hermoso Sánchez
 *      e-mail: raul.hermoso@gmail.com
 */

//-----------------------------------------------------------------------------
#include "ini_file.hpp"
#include <fstream>
//#include <iostream>
//-----------------------------------------------------------------------------
using namespace nsUav;
//-----------------------------------------------------------------------------

std::string& trim(std::string& strTxt, const std::string strDel = " \t")
{
	size_t st_end = strTxt.find_last_not_of(strDel) + 1;
	size_t st_beg = strTxt.find_first_not_of(strDel);

	strTxt.substr(st_beg, st_end - st_beg);

	return strTxt;
}
//-----------------------------------------------------------------------------

TParam::TParam(std::string strName)
{
	m_strName    = trim(strName);
	m_strComment = "";
	m_ssValue.clear();
}
//-----------------------------------------------------------------------------

TParam::~TParam()
{
	m_strName    = "";
	m_strComment = "";
	m_ssValue.clear();
}
//-----------------------------------------------------------------------------







TSection::TSection(std::string strName)
{
	m_strName    = trim(strName);
	m_strComment = "";
	clear();
}
//-----------------------------------------------------------------------------

TSection::~TSection()
{
	m_strName    = "";
	m_strComment = "";
	clear();
}
//-----------------------------------------------------------------------------

TParam* TSection::add(std::string strName, std::string strComment)
{
	std::map<std::string, TParam*>::iterator found = m_mParams.find(trim(strName));

	if(found == m_mParams.end())
	{
		TParam* param = new TParam(strName);

		param->comment()   = strComment;
		m_mParams[strName] = param;

		return param;
	}

	return found->second;
}
//-----------------------------------------------------------------------------

bool TSection::del(std::string strName)
{
	bool b_ret = false;
	std::map<std::string, TParam*>::iterator found = m_mParams.find(trim(strName));

	if(found != m_mParams.end())
	{
		delete found->second;
		m_mParams.erase(found);

		b_ret = true;
	}

	return b_ret;
}
//-----------------------------------------------------------------------------

bool TSection::del(unsigned int uiIndex)
{
	bool b_ret = false;
	std::map<std::string, TParam*>::iterator iter = m_mParams.begin();

	for(unsigned int i = 0; i < uiIndex && iter != m_mParams.end(); ++i)
		++iter;

	if(iter != m_mParams.end())
	{
		delete iter->second;
		m_mParams.erase(iter);

		b_ret = true;
	}

	return b_ret;
}
//-----------------------------------------------------------------------------

void TSection::clear()
{
	while(m_mParams.size() > 0)
	{
		delete m_mParams.begin()->second;
		m_mParams.erase(m_mParams.begin());
	}
	m_mParams.clear();
}
//-----------------------------------------------------------------------------

TParam* TSection::operator[](std::string strName)
{
	std::map<std::string, TParam*>::iterator found = m_mParams.find(trim(strName));

	return found == m_mParams.end() ? NULL : found->second;
}
//-----------------------------------------------------------------------------

TParam* TSection::operator[](unsigned int uiIndex)
{
	std::map<std::string, TParam*>::iterator iter = m_mParams.begin();

	for(unsigned int i = 0; i < uiIndex && iter != m_mParams.end(); ++i)
		++iter;

	return iter == m_mParams.end() ? NULL : iter->second;
}
//-----------------------------------------------------------------------------







TIniFile::TIniFile()
{
	close();
}
//-----------------------------------------------------------------------------

TIniFile::~TIniFile()
{
	close();
}
//-----------------------------------------------------------------------------

void TIniFile::readLine(char* cLine, TSection*& sSection, TParam*& pParam)
{
	std::string str_line(cLine);
	std::string str_val;
	size_t      pos = str_line.find_first_of("#[=");
	size_t      end = 0;
	size_t      com = 0;

	if(str_line.empty())
	{
		if(m_strHeader.empty())
			m_strHeader = m_strComments;
		m_strComments.clear();
	}
	else if(pos != std::string::npos)
	{
		switch(str_line[pos])
		{
		case '[':
			end = str_line.find(']', pos + 1);
			if(end != std::string::npos)
			{
				sSection = add(str_line.substr(pos + 1, end - pos - 1));
				sSection->comment() = m_strComments;
				m_strComments.clear();
			}
			else
			{
				// TODO: Lanzar error de formato de secciÃ³n. No se encuentra
				// el terminador ']'
			}
			break;

		case '=':
			if(sSection)
			{
				pParam = sSection->add(str_line.substr(0, pos));
				com = str_line.find('#', pos + 1);
				if(com != std::string::npos)
				{
					end = com;
					while(str_line[end - 1] == ' ')
						--end;
					while(str_line[com + 1] == ' ')
						++com;

					str_val = str_line.substr(pos + 1, end - pos - 1);
					pParam->value() << trim(str_val);
					pParam->comment() = str_line.substr(com + 1);
				}
				else
				{
					pParam->value() << str_line.substr(pos + 1);
				}
			}
			else
			{
				// TODO: Lanzar error de formato. No existe secciÃ³n para el
				// parÃ¡metro dado
			}
			break;

		case '#':
			end = str_line.size();
			if(end == str_line.rfind(' ') + 1)
				while(str_line[end - 1] == ' ')
					--end;
			while(str_line[pos + 1] == ' ')
				++pos;
			if(end == pos + 1)
			{
				m_strComments += std::string("\n");
			}
			else
			{
				if(!m_strComments.empty())
					m_strComments += std::string("\n");
				m_strComments += str_line.substr(pos + 1, end - pos - 1);
			}
			break;

		default:
			break;
		}
	}
}
//-----------------------------------------------------------------------------

void TIniFile::loadFile(std::string strFilename)
{
	std::fstream file;

	file.open(trim(strFilename).c_str(), std::fstream::in);

	if(!file.is_open())
		return;

	char      c_line[1024];
	int       i_size  = 1024;
	TSection* section = NULL;
	TParam*   param   = NULL;

	while(!file.eof())
	{
		file.getline(c_line, i_size);
		readLine(c_line, section, param);
	}

	file.close();
}
//-----------------------------------------------------------------------------

void TIniFile::saveFile(std::string strFilename)
{
	std::fstream file;

	file.open(trim(strFilename).c_str(), std::fstream::out | std::fstream::trunc);

	if(!file.is_open())
		return;

	std::map<std::string, TSection*>::iterator iter = m_mSections.begin();
	TParam* param = NULL;

	if(!m_strHeader.empty())
	{
		size_t      pos;
		std::string str_aux = m_strHeader;
		while((pos = str_aux.find('\n')) != std::string::npos)
		{
			file << "# " << str_aux.substr(0, pos) << std::endl;
			str_aux = str_aux.substr(pos + 1);
		}
		file << "# " << str_aux.substr(0, pos) << std::endl << std::endl;
	}

	while(iter != m_mSections.end())
	{
		if(iter->second)
		{
			if(iter != m_mSections.begin())
				file << std::endl;

			if(iter->second->comment().size())
			{
				size_t      pos;
				std::string comment = iter->second->comment();
				while((pos = comment.find('\n')) != std::string::npos)
				{
					file << "# " << comment.substr(0, pos) << std::endl;
					comment = comment.substr(pos + 1);
				}
				file << "# " << comment.substr(0, pos) << std::endl;
			}

			file << "[" << iter->second->name() << "]" << std::endl;
			for(size_t i = 0; i < iter->second->size(); ++i)
			{
				param = (*(iter->second))[i];
				if(param)
				{
					file << param->name() << "=" << param->value().str();
					if(param->comment().size())
						file << " # " << param->comment();
					file << std::endl;
				}
			}
		}

		++iter;
	}

	file.close();
}
//-----------------------------------------------------------------------------

void TIniFile::close()
{
	clear();
	m_strName   = "";
	m_strHeader = "";
}
//-----------------------------------------------------------------------------

TSection* TIniFile::add(std::string strName)
{
	std::map<std::string, TSection*>::iterator found = m_mSections.find(trim(strName));

	if(found == m_mSections.end())
	{
		TSection* section = new TSection(strName);
		m_mSections[strName] = section;

		return section;
	}

	return found->second;
}
//-----------------------------------------------------------------------------

bool TIniFile::del(std::string strName)
{
	bool b_ret = false;
	std::map<std::string, TSection*>::iterator found = m_mSections.find(trim(strName));

	if(found != m_mSections.end())
	{
		delete found->second;
		m_mSections.erase(found);

		b_ret = true;
	}

	return b_ret;
}
//-----------------------------------------------------------------------------

bool TIniFile::del(unsigned int uiIndex)
{
	bool b_ret = false;
	std::map<std::string, TSection*>::iterator iter = m_mSections.begin();

	for(unsigned int i = 0; i < uiIndex && iter != m_mSections.end(); ++i)
		++iter;

	if(iter != m_mSections.end())
	{
		delete iter->second;
		m_mSections.erase(iter);

		b_ret = true;
	}

	return b_ret;
}
//-----------------------------------------------------------------------------

void TIniFile::clear()
{
	m_strComments = "";

	while(m_mSections.size() > 0)
	{
		delete m_mSections.begin()->second;
		m_mSections.erase(m_mSections.begin());
	}
	m_mSections.clear();
}
//-----------------------------------------------------------------------------

TSection* TIniFile::operator[](std::string strName)
{
	std::map<std::string, TSection*>::iterator found = m_mSections.find(trim(strName));

	return found == m_mSections.end() ? NULL : found->second;
}
//-----------------------------------------------------------------------------

TSection* TIniFile::operator[](unsigned int uiIndex)
{
	std::map<std::string, TSection*>::iterator iter = m_mSections.begin();

	for(unsigned int i = 0; i < uiIndex && iter != m_mSections.end(); ++i)
		++iter;

	return iter == m_mSections.end() ? NULL : iter->second;
}
//-----------------------------------------------------------------------------
