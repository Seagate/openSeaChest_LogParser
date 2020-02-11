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
//! \param numberOfTabs - the number of tab or comma's to be place
//  Exit:
//!   \return bool
//
//---------------------------------------------------------------------------
bool CPrintCSV::parse_Json(JSONNODE *nData, uint16_t numberOfTabs)
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
        if (json_type(*i) == JSON_ARRAY)
        {
            json_char *main_name = json_name(*i);
            JSONNODE_ITERATOR q = json_begin(*i);
            if (q == NULL)
            {
                json_free(main_name);
                break;
            }
            if (json_type(*q) == JSON_STRING)
            {
                // Get the main name created in the title
                size_t si = strlen(main_name);
                title.resize(si );
                // need to add in a , for tabing in the xls otherwise for flatcsv it will just add a comma
                if (m_csv)
                {
                    for (uint16_t j = 0; j < numberOfTabs; j++)
                    {
                        title.insert(j, ",");

                    }
                }
                else    // for the flatcsv
                {
                    title = ",";
                }
                strncat((char*)title.c_str(), main_name, si);
                title = title + ",";
                m_line.append(title);
                m_line.append("\n");
                title = "";

                // Now get the next node string and then get the node data
                json_char *node_name = json_name(*q);
                if (m_csv)
                {
                    for (uint16_t j = 0; j < numberOfTabs +1; j++)
                    {
                        title.insert(j, ",");

                    }
                }
                else    // for the flatcsv
                {
                    title = ",";
                }

                si = strlen(node_name) + numberOfTabs +1;
                title.resize(si);
                strncat((char*)title.c_str(), node_name, si);
                title = title + ",";


                // go through all the nodes and get the data but don't change the title
                while (q != json_end(*i))
                {
                    //char *newData = (char*)calloc((BASIC * 4), sizeof(char));
                    json_char* newData = json_as_string(*q);
                    si = strlen(newData);
                    data.resize(si);
                    strncpy((char*)data.c_str(), newData, si);
                    json_free(newData);
                    q++;
                }
                m_csvData.title.resize(sizeof(title));
                m_csvData.title = title;
                m_csvData.data.resize(data.size());
                strncpy((char*)m_csvData.data.c_str(), (char*)data.c_str(), data.size());
                if (m_csvData.data.size() > 1)  // check to make sure title and data have data ??
                {
                    m_line.append(m_csvData.title);     // move the title to the line
                    m_line.append(m_csvData.data);      // move the data to the line
                    m_line.append("\n");
                    m_csvData.title = "";               // clear title
                    m_csvData.data = "";                // clear data
                }
                // clean up on the json_char*
                json_free(node_name);
                json_free(main_name);
            }
            else if (json_type(*q) == JSON_NUMBER)
            {
                json_char *node_name = json_name(*q);
                // Take the main node string 
                size_t si = strlen(main_name);
                
                // need to add in a , for tabing in the xls otherwise for flatcsv it will just add a comma
                if (m_csv)
                {
                    for (uint16_t j = 0; j < numberOfTabs; j++)
                    {
                        title.insert(j, ",");

                    }
                }
                else    // for the flatcsv
                {
                    title = ",";
                }
                title.resize(si + numberOfTabs);
                
                strncat((char*)title.c_str(), main_name, si);
                title = title + ",";
                // set the string data for the array of data
                char *intData = (char*)calloc((BASIC*4), sizeof(char));
                memset(intData, 0 , (BASIC * 4));
                while (q != json_end(*i))
                {
                    char *newData = (char*)calloc((BASIC*4), sizeof(char));
                    // this format seems to be perfect and the size is working
                    if (m_csv)
                    {
                        snprintf(newData, BASIC, "%" PRIi32",", (uint32_t)json_as_int(*q));
                        strncat(intData,newData,sizeof(newData));
                    }
                    else
                    {
                        m_csvData.title = m_csvData.title + title;
                        snprintf(intData, BASIC, "%" PRIi32",", (uint32_t)json_as_int(*q));
                        m_csvData.data.append(intData,sizeof(intData));
                    }
                    if ( BASIC * 3 < strlen(intData))   // break out before we see the full size of intData as a string
                    {
                        break;
                    }
                    safe_Free(newData);
                    q++;
                }
                json_free(node_name);
                if (m_csv)
                {
                    m_csvData.title.resize(sizeof(title));
                    m_csvData.title = title;
                    m_csvData.data.resize(strlen(intData));
                    strncpy((char*)m_csvData.data.c_str(), intData, strlen(intData));
                }
                safe_Free(intData);      // free the memory for intData
            }
            else if (json_type(*q) == JSON_NODE)
            {
                
                if (m_csv)
                {
                    if (numberOfTabs != 0)
                    {
                        char *tempName = (char*)calloc((BASIC*2), sizeof(char));
                        memset(tempName, 0, BASIC);
                        for (uint16_t j = 0; j <= numberOfTabs; j++)
                        {
                            strcat(tempName, ",");
                        }
                        strncat(tempName, main_name, strlen(main_name));
                        //strncpy(main_name, tempName, sizeof(tempName));
                        strcat(tempName, "\0");
                        
                        m_line.append(tempName);
                        m_line.append("\n");
                        safe_Free(tempName);
                    }
                    else
                    {
                        m_line.append(main_name);
                        m_line.append("\n");
                    }
                }
                else        // flatcsv needs to copy the data   to the m_csvData strings
                {
                    m_csvData.title = m_csvData.title + main_name;
                    m_csvData.title = m_csvData.title + ",";
                    // data is just a comma
                    m_csvData.data = m_csvData.data + ",";
                }
                parse_Json(*q, numberOfTabs + 2);
                //do something here
            }
            else
            {
                json_free(main_name);
            }
            if (m_csv)
            {
                if (m_csvData.data.size() > 1)  // check to make sure title and data have data ??
                {
                    m_line.append(m_csvData.title);     // move the title to the line
                    m_line.append(m_csvData.data);      // move the data to the line
                    m_line.append("\n");
                    m_csvData.title = "";               // clear title
                    m_csvData.data = "";                // clear data
                }
            }
        }
        if ( json_type(*i) == JSON_NODE)
        {
            json_char *main_name = json_name(*i);
            JSONNODE_ITERATOR q = json_begin(*i);
            // need to parse out the 64 bit data for the flat csv 
            if (q == NULL)
            {
                break;
            }
			if (m_csv)
			{
                if (numberOfTabs != 0)
                {
                    char *tempName = (char*)calloc((BASIC*4), sizeof(char));
                    memset(tempName, 0, BASIC);
                    for (uint16_t j = 0; j < numberOfTabs; j++)
                    {
                        strcpy(tempName, ",");
                    }
                    strncat(tempName, main_name, strlen(main_name));
                    //strcat(tempName, "\0");
                    m_line.append(tempName);
                    m_line.append("\n");
                    safe_Free(tempName);
                }
                else
                {
                    m_line.append(main_name);
                    m_line.append("\n");
                }
			}
            else        // flatcsv needs to copy the data   to the m_csvData strings
            {
                m_csvData.title = m_csvData.title + main_name;
                m_csvData.title = m_csvData.title + ",";
                        // data is just a comma
                
                m_csvData.data = m_csvData.data + ",";
               
            }
            json_free(main_name);
            parse_Json(*i, numberOfTabs + 1);
            
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
            
			createData(title, data, numberOfTabs);
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
			snprintf((char*)intData.c_str(), vsi , "%" PRIu32 " ", (uint32_t)json_as_int(*i));
			createData(title, intData, numberOfTabs);
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

    uint16_t numberOfTabs = 0;

    parse_Json(masterData, numberOfTabs);
    rTitle.insert(0, m_csvData.title);
    rData.insert(0, m_csvData.data);
  
    m_csvData.title = "";
    m_csvData.data = "";

    rTitle.append("\n");
    rData.append("\n");
    rTitle.append( rData );
    return rTitle;
}
//-----------------------------------------------------------------------------
//
//! \fn createData
//
//! \brief
//!   Description: Takes the title and the data to format the information for csv or flatcsv 
//
//  Entry:
//! \param title -> title string
//! \param data -> data from the nodes
//! \param numberOfTabs - the number of tabs or comma's to use
//
//  Exit:
//!   \return bool
//
//---------------------------------------------------------------------------
bool CPrintCSV::createData( std::string &title, std::string &data, uint16_t numberOfTabs)
{
	if (m_csv)
	{
        for (uint16_t i = 0; i < numberOfTabs; i++)
        {
            title.insert(i, ",");
            
        }
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
    uint16_t numberOfTabs = 0;
	parse_Json(masterData, numberOfTabs);
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
//! \fn Create_Tabs()
//
//! \brief
//!   Description: creates the Data for the text tabs
//
//  Entry:
//
//  Exit:
//!   \return 
//
//---------------------------------------------------------------------------

bool CPrintTXT::Create_Tabs(std::string &title, std::string&data, uint16_t numberOfTabs)
{
    sFrameData frame;
    frame.data.resize(sizeof(data));
    for (uint16_t i = 0; i < numberOfTabs; i++)              // create the tabs for the node name
    {
        title.insert(i, "\t");
        
    }
    frame.title = title;
    frame.data = data;
    m_vData.push_back(frame);
    return true;
}
//-----------------------------------------------------------------------------
//
//! \fn CPrintTXT::parse_Json_to_Text()
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
bool CPrintTXT::parse_Json_to_Text(JSONNODE *nData, uint16_t numberOfTabs)
{
#define CHARSIZE 4
    std::string title = "";
    std::string data = "";
    title.resize(BASIC);
    data.resize(BASIC * 4);
    sFrameData frame;
    JSONNODE_ITERATOR i = json_begin(nData);
    while (i != json_end(nData))
    {
        title = "";
        data = "";
        // recursively call ourselves to dig deeper into the tree
        if (json_type(*i) == JSON_ARRAY)
        {
            json_char *main_name = json_name(*i);
            JSONNODE_ITERATOR q = json_begin(*i);
            if (q == NULL)
            {
                json_free(main_name);
                size_t si = strlen(main_name);
                title.resize(si);
                strncpy((char*)title.c_str(), main_name, si);
                Create_Tabs(title, data, numberOfTabs);
            }
            else if (json_type(*q) == JSON_STRING)
            {
                // Get the main name created in the title
                size_t si = strlen(main_name);
                title.resize(si);
                strncpy((char*)title.c_str(), main_name, si);
                Create_Tabs(title, data,numberOfTabs);
                title = "";
                // Now get the next node string and then get the node data
                //json_char *node_name = json_name(*q);
                uint16_t stringTabs = numberOfTabs + 1;
                // go through all the nodes and get the data but don't change the title
                while (q != json_end(*i))
                {
                    json_char *newData = json_as_string(*q);
                    size_t dd = strlen(newData);                            // get the size of the data
                    data.resize(dd);
                    strncpy((char*)data.c_str(), newData, dd);
                    Create_Tabs(title, data, (stringTabs));                 // call Create Tabs to add the tabs into the title
                    title = "";                                             // clear it 
                    json_free(newData);
                    q++;
                }
                // clean up on the char*
                //json_free(node_name);
                json_free(main_name);
            }
            else if (json_type(*q) == JSON_NUMBER)
            {
                // Get the main name created in the title
                size_t si = strlen(main_name);
                title.resize(si);
                strncpy((char*)title.c_str(), main_name, si);

                char *nodeData = (char*)calloc((BASIC * 4), sizeof(char));
                while (q != json_end(*i))
                {
                    char *newData = (char*)calloc((BASIC * 4), sizeof(char));
                    uint32_t jsonint = (uint32_t)json_as_int(*q);
                    // this format seems to be perfect and the size is working
                    snprintf(newData, sizeof(uint32_t) + 2, "%" PRIu32 ",", jsonint);
                    strncat(nodeData, newData, strlen(newData));
                    safe_Free(newData);
                    q++;
                }
                // clean up on the char*
                data = nodeData;
                Create_Tabs(title, data, (numberOfTabs));                 // call Create Tabs to add the tabs into the title
                safe_Free(nodeData);
                json_free(main_name);
            }
            else if (json_type(*q) == JSON_NODE)
            {  
                JSONNODE_ITERATOR k = json_begin(*q);
                json_char *node_name = json_name(*k);
                // need to parse out the NULLS
                if (k == NULL)
                {
                    json_free(node_name);
                    json_free(main_name);
                    break;
                }
                // set the string data for the array of data
                char *tempName = (char*)calloc((BASIC * 4), sizeof(char));
                memset(tempName, 0, BASIC);
                strncat(tempName, main_name, strlen(main_name));
                strcat(tempName, "\0");
                size_t si = strlen(main_name);
                title.resize(si);
                title.append(tempName);
                title.append("\n");
                frame.title = title;
                m_vData.push_back(frame);
                frame.title = "";
                frame.data = "";
                safe_Free(tempName);

                parse_Json_to_Text(*q, (numberOfTabs +1));
                json_free(node_name);
                json_free(main_name);
                
            }
            else
            {
                 json_free(main_name);
            }

        }
        if (json_type(*i) == JSON_NODE)
        {
            json_char *main_name = json_name(*i);
            JSONNODE_ITERATOR q = json_begin(*i);
            // need to parse out the Nulls
            if (q == NULL)
            {
		        json_free(main_name);
                break;
            }
            title =  main_name;
            Create_Tabs(title, data, numberOfTabs);
            //m_vData.push_back(frame);
            json_free(main_name);
            title = "";
            parse_Json_to_Text(*i, (numberOfTabs + 1));

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

            Create_Tabs(title,data,numberOfTabs);
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
            // this format seems to be perfect and the size is working
            snprintf((char*)intData.c_str(), vsi , "%" PRIu32 " ", (uint32_t)json_as_int(*i));         
            Create_Tabs(title, intData, numberOfTabs);
            json_free(node_name);
            json_free(node_value);
        }
        i++;
    }
    return true;
}
//-----------------------------------------------------------------------------
//
//! \fn CPrintTXT::get_Msg_Text_Format()
//
//! \brief
//!   Description: Take the vector of data and create a string for a text file
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
    for (std::vector<sFrameData>::iterator it = m_vData.begin(); it != m_vData.end(); ++it)
    {
        r.append(it->title);
        r.append(" : ");
        r.append(it->data);
        r.append("\n");
    }
    // completed now we will clear the vector 
    m_vData.clear();
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
    , printStatus(NOT_SUPPORTED)
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
    , printStatus(NOT_SUPPORTED)
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
        parse_Json_to_Text(msgData,0);                        // parse the json data into a vector 
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