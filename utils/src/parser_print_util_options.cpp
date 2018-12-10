//
// Print_Ooptions.cpp
//
// Do NOT modify or remove this copyright and license
//
// Copyright (c) 2015 - 2018 Seagate Technology LLC and/or its Affiliates, All Rights Reserved
//
// This software is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// ******************************************************************************************

// \file Print_Options.cpp
// \brief This file defines the specific funtions and macros for parser Utilities. 

#include "parser_print_util_options.h"
#include <sstream>
#include <algorithm>
#include <string>
using namespace opensea_parser;
// trim from beginning of string (left)
inline std::string& ltrim(std::string& s, const char* t = " \t\r\f\v")
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

inline bool replaceCharacter(std::string &s, char c, char r, size_t pos, bool backward)
{
    std::size_t found = backward ? s.rfind(c, pos) : s.find(c, pos);

    if (found != std::string::npos)
    {
        s.replace(found, 1, 1, r);
        return true;
    }

    return false;

}
//-----------------------------------------------------------------------------
//
//! \fn CPrintJSON::CPrintJSON()
//
//! \brief
//!   Description: Class constructor
//
//  Entry:
// \param pdata = pointer to the buffer data.
//
//  Exit:
//!   \return 
//
//---------------------------------------------------------------------------

CPrintJSON::CPrintJSON()
	: m_JsonData()
	, m_jsonMessage("")
{
}
//-----------------------------------------------------------------------------
//
//! \fn CPrintJSON::CPrintJSON()
//
//! \brief
//!   Description: Class constructor
//
//  Entry:
// \param pdata = pointer to the buffer data.
//
//  Exit:
//!   \return 
//
//---------------------------------------------------------------------------

CPrintJSON::CPrintJSON(JSONNODE *masterData)
    :m_JsonData(masterData)
{
    json_char *jc = json_write_formatted(masterData);
    m_jsonMessage.assign( jc);
    if (VERBOSITY_DEFAULT < g_verbosity)
    {
        printf("\n%s", m_jsonMessage.c_str());
    }
    json_free(jc);
}
//-----------------------------------------------------------------------------
//
//! \fn CPrintJSON::~CPrintJSON()
//
//! \brief
//!   Description: Class deconstructor 
//
//  Entry:
//
//  Exit:
//!   \return 
//
//---------------------------------------------------------------------------

CPrintJSON::~CPrintJSON()
{
}

//-----------------------------------------------------------------------------
//
//! \fn CPrintCSV::CPrintCSV()
//
//! \brief
//!   Description: Class constructor
//
//  Entry:
// 
//
//  Exit:
//!   \return 
//
//---------------------------------------------------------------------------

CPrintCSV::CPrintCSV()
	: m_csvData()
	, m_line("")
    , m_csv(true)
{

}
//-----------------------------------------------------------------------------
//
//! \fn CPrintCSV::~CPrintCSV()
//
//! \brief
//!   Description: Class deconstructor 
//
//  Entry:
//
//  Exit:
//!   \return 
//
//---------------------------------------------------------------------------

CPrintCSV::~CPrintCSV()
{

}
//-----------------------------------------------------------------------------
//
//! \fn CPrintCSV::parse_Json()
//
//! \brief
//!   Description: parse the json informaton to get it into the structue for printing. ( recursion )
//
//  Entry:
//! \param nData - the json node that we are parsing out at this point in time 
//  Exit:
//!   \return bool
//
//---------------------------------------------------------------------------
bool CPrintCSV::parse_Json(JSONNODE *nData)
{
#define CHARSIZE 4
    std::string title = "";
    std::string data = "";
    JSONNODE_ITERATOR i = json_begin(nData);

    while (i != json_end(nData))
    {
        title = "";
        data = "";
        // recursively call ourselves to dig deeper into the tree
        if (json_type(*i) == JSON_ARRAY || json_type(*i) == JSON_NODE)
        {
            json_char *main_name = json_name(*i);
            JSONNODE_ITERATOR q = json_begin(*i);
            // need to parse out the 64 bit data for the flat csv 
            if (json_type(*q) == JSON_BOOL)
            {
                q++;
                if (json_type(*q) == JSON_STRING)
                {
                    json_char *node_name = json_name(*q);
                    // make sure it's the 64 bit value string   now we can get the data
                    if (strcmp(node_name, "64 bit Value String") == 0)
                    {
                        // Take the main node string 
                        size_t si = strlen(main_name);
                        title.resize(si);
                        strncpy((char*)title.c_str(), main_name, si);
                        // get the node value and add it to the structure
                        json_char *node_value = json_as_string(*q);
                        size_t valSize = strlen(node_value);
                        data.resize(valSize);
                        strncpy((char*)data.c_str(), node_value, valSize);
						createData(title, data);
                        json_free(node_value);
                    }
                    json_free(node_name);
                }
                else
                {
                    parse_Json(*i);
                }

            }
			else
			{
				if (m_csv)
				{
					m_line.append(main_name);
					m_line.append("\n");
				}
                parse_Json(*i);
            }
            json_free(main_name);
        }
        if (json_type(*i) == JSON_STRING || json_type(*i) == JSON_BOOL)
        {
            json_char *node_name = json_name(*i);
            json_char *node_value = json_as_string(*i);
            size_t si = strlen(node_name);
            title.resize(si);
            strncpy((char*)title.c_str(), node_name, si);

            size_t valSize = strlen(node_value);
            data.resize(valSize);
            strncpy((char*)data.c_str(), node_value, valSize);
            
			createData(title, data);
            json_free(node_name);
            json_free(node_value);

        }
        if ((json_type(*i) == JSON_NUMBER))
        {
            json_char *node_name = json_name(*i);
            json_char *node_value = json_as_string(*i);
            size_t si = strlen(node_name);
            size_t vsi = strlen(node_value) + 1;
            title.resize(si);
            strncpy((char*)title.c_str(), node_name, si);
            std::string intData = "   ";
            intData.resize(vsi);
            snprintf((char*)intData.c_str(), CHARSIZE, "%" PRId32 " ", (uint32_t)json_as_int(*i));
			createData(title, intData);
            json_free(node_name);
            json_free(node_value);
        }
        i++;
    }
    return true;
}
//-----------------------------------------------------------------------------
//
//! \fn CPrintCSV::get_Msg_Flat_csv()
//
//! \brief
//!   Description: Get the JSON information and flattens the csv
//
//  Entry:
//    \param masterData = the master json node with all the data
//  Exit:
//!   \return the printable string
//
//---------------------------------------------------------------------------
std::string CPrintCSV::get_Msg_Flat_csv(JSONNODE *masterData)
{
	m_csv = false;
    std::string rTitle = "";
    std::string rData = "";
    JSONNODE_ITERATOR i = json_begin(masterData);

    while (i != json_end(masterData))
    {
        parse_Json(*i);
        rTitle = rTitle + m_csvData.title;
        rData = rData + m_csvData.data;
        i++;
        m_csvData.title = "";
        m_csvData.data = "";
    }
    rTitle = rTitle + "\n";
    rTitle = rTitle + rData + "\n";
    return rTitle;
}
//-----------------------------------------------------------------------------
//
//! \fn createData
//
//! \brief
//!   Description: Takes the title and the data to format the informatio for csv or flatcsv 
//
//  Entry:
//! \param title -> title string
//! \param data -> data from the nodes
//
//  Exit:
//!   \return bool
//
//---------------------------------------------------------------------------
bool CPrintCSV::createData( std::string &title, std::string &data)
{
	if (m_csv)
	{
		title.insert(0, "\t");
		title = title + ",";
		m_line.append(title);
		data = data + "\n";
		m_line.append(data);
	}
	else
	{
		title = title + ",";
		m_csvData.title = m_csvData.title + title;
		data = data + ",";
		m_csvData.data = m_csvData.data + data;
	}
	return true;
}
//-----------------------------------------------------------------------------
//
//! \fn CPrintCSV::get_Msg_CSV()
//
//! \brief
//!   Description: Get the JSON string  
//
//  Entry:
//
//  Exit:
//!   \return the string data that we need for the csv
//
//---------------------------------------------------------------------------
std::string CPrintCSV::get_Msg_CSV(JSONNODE *masterData)                             //(const std::string message)
{
	std::string rTitle = "";

	JSONNODE_ITERATOR i = json_begin(masterData);

	while (i != json_end(masterData))
	{
		parse_Json(*i);
		i++;
	}
return m_line;
}

//-----------------------------------------------------------------------------
//
//! \fn CPrintTXT::CPrintTXT()
//
//! \brief
//!   Description: Class constructor
//
//  Entry:
// \param pdata = pointer to the buffer data.
//
//  Exit:
//!   \return 
//
//---------------------------------------------------------------------------

CPrintTXT::CPrintTXT()
{

}
//-----------------------------------------------------------------------------
//
//! \fn CPrintTXT::~CPrintTXT()
//
//! \brief
//!   Description: Class deconstructor 
//
//  Entry:
//
//  Exit:
//!   \return 
//
//---------------------------------------------------------------------------

CPrintTXT::~CPrintTXT()
{

}
//-----------------------------------------------------------------------------
//
//! \fn CPrintTXT::get_Msg_Text_Format()
//
//! \brief
//!   Description: Get the JSON string  
//
//  Entry:
//
//  Exit:
//!   \return 
//
//---------------------------------------------------------------------------
std::string CPrintTXT::get_Msg_Text_Format(const std::string message)
{
    std::string r = "";
    std::istringstream f(message);
    std::string line;
    std::string nextLine;
    while (std::getline(f, line))
    {
        if (line.rfind('{') != std::string::npos)
        {
            std::getline(f, nextLine);
            if (nextLine.find("64 bit Value String in Hex") != std::string::npos)
            {
                while (replaceCharacter(line, '{', ' ', 0, false));
                std::getline(f, nextLine);
                size_t found = nextLine.rfind(':') + 1;
                line.append(nextLine.begin() + found, nextLine.end());
                std::getline(f, nextLine);                              // read the next three lines 
                std::getline(f, nextLine);
                std::getline(f, nextLine);
            }
            else
            {
                while (replaceCharacter(line, '{', '\n', 0, false));
                while (replaceCharacter(line, '}', '\n', std::string::npos, true));
            }
        }
        else
        {
            while (replaceCharacter(line, '}', ' ', std::string::npos, true));
            while (replaceCharacter(line, ',', ' ', 0, false));
        }

        if (line.rfind('\n') == std::string::npos)
            line = line + "\n";
        while (replaceCharacter(line, '"', ' ', 0, false));

        ltrim(line);

        r = r + line;
    }
    return r;
}
//-----------------------------------------------------------------------------
//
//! \fn CMessage::CMessage()
//
//! \brief
//!   Description: Class constructor
//
//  Entry:
//! \param masterData = all of the json information
//
//  Exit:
//!   \return 
//
//---------------------------------------------------------------------------
CMessage::CMessage(JSONNODE *masterData)
	:CPrintJSON(masterData), CPrintTXT(), CPrintCSV()
    , msgData(masterData)
    , printType()
    , m_fileName()
{

}
//-----------------------------------------------------------------------------
//
//! \fn CMessage::CMessage()
//
//! \brief
//!   Description: Class constructor
//
//  Entry:
//! \param masterData = all of the json information
//! \param fileName - this is the file name that we will print everything out to
//! \param tooPrintType - what type of file extention will it be
//
//  Exit:
//!   \return 
//
//---------------------------------------------------------------------------

CMessage::CMessage(JSONNODE *masterData, std::string fileName, int toolPrintType)
    :CPrintJSON(masterData), CPrintTXT(), CPrintCSV()
    , msgData(masterData)
    , printType(toolPrintType)
    , m_fileName(fileName)
{
    message = "";
    WriteBuffer();
}
//-----------------------------------------------------------------------------
//
//! \fn CMessage::~CMessage()
//
//! \brief
//!   Description: Class deconstructor 
//
//  Entry:
//
//  Exit:
//!   \return 
//
//---------------------------------------------------------------------------
CMessage::~CMessage()
{

}
//-----------------------------------------------------------------------------
//
//! \fn CMessage::WriteBuffer()
//
//! \brief
//!   Description: Write the buffer data out to the file and makes the choice from the printType
//
//  Entry:
//
//  Exit:
//!   \return SUCCESS or FILE_OPEN_ERROR                   /// do we need any more ??????
//
//---------------------------------------------------------------------------
int CMessage::WriteBuffer()
{
    switch (printType)                                      // for now it will only do json
    {
    case OPENSEA_LOG_PRINT_JSON:
        message = get_Msg_JSON_Data();                      // get the string message for printable json data
        break;
    case OPENSEA_LOG_PRINT_TEXT:
		message = get_Msg_JSON_Data();                      // get the string message for printable json data
        message = get_Msg_Text_Format(message);             // get the string message for printable test format data
        break;
    case OPENSEA_LOG_PRINT_CSV:
        message = get_Msg_CSV(msgData);                     // get the string message normal CSV format data
        break;
    case OPENSEA_LOG_PRINT_FLAT_CSV:
        message = get_Msg_Flat_csv(msgData);                // get the json data to create a flat csv 
        break;
    default:
        message = get_Msg_JSON_Data();                      // get the string message for printable json data
        break;
    }

    logFile.open(m_fileName.c_str(), std::ios::out | std::ios::trunc);//only allow reading as a binary file;
    if (logFile.is_open())
    {
        logFile.write((char*)message.c_str(), message.length());            // write the string out to the log
        logFile.close();
    }
    else
    {
        printStatus = FILE_OPEN_ERROR;
        return FILE_OPEN_ERROR;
    }


    if (VERBOSITY_DEFAULT < g_verbosity)
    {
        printf("\nWrite Buffer wrote %zd bytes to file. \n", message.length());
    }

    return SUCCESS;
}