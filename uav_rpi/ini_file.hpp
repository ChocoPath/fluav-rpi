/*
 * ini_file.hpp
 *
 *   Provide the menas for opening, saving, acessing and closing .ini
 *   configuration files.
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
#ifndef __PARSER_HPP__
#define __PARSER_HPP__
//-----------------------------------------------------------------------------
#include <string>
#include <sstream>
#include <map>
#include <vector>
//-----------------------------------------------------------------------------

namespace nsUav
{
	//-------------------------------------------------------------------------

	class TParam
	{
	private:
		std::string       m_strName;
		std::string       m_strComment;
		std::stringstream m_ssValue;

	protected:
	public:
		TParam(std::string strName = "");
		virtual ~TParam();

		inline std::string        name()    { return m_strName;    }
		inline std::string&       comment() { return m_strComment; }
		inline std::stringstream& value()   { return m_ssValue;    }
	};
	//-------------------------------------------------------------------------

	class TSection
	{
	private:
		std::string                    m_strName;
		std::string                    m_strComment;
		std::map<std::string, TParam*> m_mParams;

	protected:
	public:
		TSection(std::string strName = "");
		virtual ~TSection();

		TParam* add(std::string strName, std::string strComment = "");
		bool    del(std::string strName);
		bool    del(unsigned int uiIndex);
		void    clear();

		TParam* operator[](std::string strName);
		TParam* operator[](unsigned int uiIndex);

		inline std::string  name()    { return m_strName;        }
		inline std::string& comment() { return m_strComment;     }
		inline size_t       size()    { return m_mParams.size(); }
	};
	//-------------------------------------------------------------------------

	class TIniFile
	{
	private:
		std::string                      m_strName;
		std::string                      m_strHeader;
		std::string                      m_strComments;
		std::map<std::string, TSection*> m_mSections;

	protected:
		virtual void readLine(char* cLine, TSection*& sSection, TParam*& pParam);

	public:
		TIniFile();
		virtual ~TIniFile();

		inline std::string& header() { return m_strHeader; }

		void loadFile(std::string strFilename);
		void saveFile(std::string strFilename);
		void close();

		TSection* add(std::string strName);
		bool      del(std::string strName);
		bool      del(unsigned int uiIndex);
		void      clear();

		TSection* operator[](std::string strName);
		TSection* operator[](unsigned int uiIndex);

		inline std::string name() { return m_strName;          }
		inline size_t      size() { return m_mSections.size(); }
	};
	//-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
#endif /* __PARSER_HPP__ */
//-----------------------------------------------------------------------------
