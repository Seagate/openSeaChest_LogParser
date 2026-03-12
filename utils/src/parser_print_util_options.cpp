//
// parser_print_util_options.cpp
//
// Do NOT modify or remove this copyright and license
//
// Copyright 2012 - 2026 Seagate Technology LLC and/or its Affiliates, All Rights Reserved
//
// This software is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// ******************************************************************************************

// \file parser_print_util_options.cpp
// \brief This file defines the specific funtions and macros for parser Utilities. 

#include "parser_print_util_options.h"
#include <sstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <iterator>

using namespace opensea_parser;

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
    if (eVerbosityLevels::VERBOSITY_BUFFERS < g_verbosity)
    {
        std::cout << "\n" << m_jsonMessage.c_str() << std::endl;
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
//! \fn CPrintCSV::parse_Json_Flat()
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
bool CPrintCSV::parse_Json_Flat(JSONNODE *nData)
{
    JSONNODE_ITERATOR i = json_begin(nData);
    while (i != json_end(nData))
    {
        // recursively call ourselves to dig deeper into the tree
        if (json_type(*i) == JSON_ARRAY)
        {
            json_char *main_name = json_name(*i);
            JSONNODE_ITERATOR q = json_begin(*i);
            if (q == M_NULLPTR)
            {
                json_free(main_name);
                break;
            }
            if (json_type(*q) == JSON_STRING)
            {
                 // go through all the nodes and get the data but don't change the title
                while (q != json_end(*i))
                {
                    json_char* newData = json_as_string(*q);
                    try
                    {
                        createLineData(main_name, newData);
                    }
                    catch (...)
                    {
                        std::cout << "Data too Large for file format." << main_name << " exiting Early" << std::endl;
                        break;
                    }
                    json_free(newData);
                    q++;
                }
            }
            else if (json_type(*q) == JSON_NUMBER)
            {
                uint32_t count = 0;
                // set the string data for the array of data
                while (q != json_end(*i))
                {
                    std::ostringstream myStr;
                    myStr << main_name << " " << std::dec << count;
                    // this format seems to be perfect and the size is working
                    m_singleLine.title.append(myStr.str());
                    m_singleLine.title.append(",");
                    std::ostringstream intData;
                    intData << std::dec << json_as_int(*q) << ",";
                    m_singleLine.data.append(intData.str());
                    q++;
                    count++;
                }
            }
            else if (json_type(*q) == JSON_NODE)
            {  
                createLineData(main_name, "");
                parse_Json_Flat(*q);
            }
            else   // Node is not found
            {
                std::cout << "Something went wrong (array)" << std::endl;
            }
            json_free(main_name);
        }
        if (json_type(*i) == JSON_NODE)
        {
            json_char *main_name = json_name(*i);
            try
            {
                createLineData(main_name, "");   
            }
            catch (...)
            {
                json_free(main_name);  // Always free before throwing/returning
                if (eVerbosityLevels::VERBOSITY_COMMAND_VERBOSE <= g_verbosity)
                {
                    std::cout << "\nSomething went wrong (JSON Node)" << std::endl;
                }
                return false;
                
            }
            json_free(main_name);
            try
            {
                parse_Json_Flat(*i);
                if (eVerbosityLevels::VERBOSITY_COMMAND_VERBOSE <= g_verbosity)
                {
                    std::cout << "\nreturn from a new parse" << std::endl;
                }
            }
            catch (...)
            {
                if (eVerbosityLevels::VERBOSITY_COMMAND_VERBOSE <= g_verbosity)
                {
                    std::cout << "\nSomething went wrong (JSON Node)" << std::endl;
                }
                return false;
            }  
        }
        if (json_type(*i) == JSON_STRING || json_type(*i) == JSON_BOOL)
        {
            json_char *node_name = json_name(*i);
            json_char *node_value = json_as_string(*i);
            if (node_name == M_NULLPTR || node_value == M_NULLPTR)
            {
                if (eVerbosityLevels::VERBOSITY_COMMAND_VERBOSE <= g_verbosity)
                {
                    std::cout << "\nSomething went wrong (string or bool)" << std::endl;
                }
                json_free(node_name);
                json_free(node_value);
                return false;
            }
            createLineData(node_name, node_value);
            json_free(node_name);
            json_free(node_value);

        }
        if ((json_type(*i) == JSON_NUMBER))
        {
            json_char *node_name = json_name(*i);
            json_char *node_value = json_as_string(*i);
            if (node_name == M_NULLPTR || node_value == M_NULLPTR)
            {
                if (eVerbosityLevels::VERBOSITY_COMMAND_VERBOSE <= g_verbosity)
                {
                    std::cout << "\nSomething went wrong JSON Number" << std::endl;
                }
                json_free(node_name);
                json_free(node_value);
                return false;
            }
            createLineData(node_name, node_value);
            json_free(node_name);
            json_free(node_value);
        }
        i++;
    }
    return true;
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
    std::ostringstream titleStream;
    std::string title;
    std::string data;
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
            if (q == M_NULLPTR)
            {
                json_free(main_name);
                break;
            }
            if (json_type(*q) == JSON_STRING)
            {
                // Get the main name created in the title
                // need to add in a , for tabing in the xls otherwise for flatcsv it will just add a comma
                if (m_csv)
                {
                    for (uint16_t j = 0; j < numberOfTabs; j++)
                    {
                        titleStream << ",";
                    }
                    title = titleStream.str();
                }
                else    // for the flatcsv
                {
                    titleStream << ",";
                }
                titleStream << main_name;
                titleStream << ",";
                m_line.append(titleStream.str());
                titleStream.str("");

                // go through all the nodes and get the data but don't change the title
                if (m_csv)
                {
                    while (q != json_end(*i))
                    {
                        data.assign(numberOfTabs + 1, ',');  
                        json_char* newData = json_as_string(*q);
                        data.append(newData);
                        m_csvData.data.append(data.c_str());
                        json_free(newData);
                        data = "";
                        q++;
                    }
                    m_csvData.title.resize(sizeof(title));
                    m_csvData.title = title;

                    if (m_csvData.data.size() > 1)  // check to make sure title and data have data ??
                    {
                        m_line.append(m_csvData.title);     // move the title to the line
                        m_line.append(m_csvData.data);      // move the data to the line
                        m_line.append("\n");
                        m_csvData.title = "";               // clear title
                        m_csvData.data = "";                // clear data
                    }
                }
                else
                {
                    while (q != json_end(*i))
                    {
                        json_char* newData = json_as_string(*q);
                        data.append(newData);
                        json_free(newData);
                        q++;
                        try 
                        {
                            data.append(",");
                            data.append(m_csvData.title);
                            data.append(m_csvData.data);
                        }
                        catch (...)
                        {
                            std::cout << "Data too Large for file format." << title << " exiting Early" << std::endl;
                            break;
                        }
                        data = "";
                    }
                    
                }
                // clean up on the json_char*
                json_free(main_name);
            }
            else if (json_type(*q) == JSON_NUMBER)
            {
                json_char *node_name = json_name(*q);
                // Take the main node string 
                titleStream.str("");
                // need to add in a , for tabing in the xls otherwise for flatcsv it will just add a comma
                if (m_csv) {
                    titleStream << std::string(numberOfTabs, ',');  // Repeat comma numberOfTabs times
                }
                else {
                    titleStream << ",";
                }
                titleStream << main_name;
                titleStream << ",";
                title.append(titleStream.str());
                titleStream.str("");
                // set the string data for the array of data
                std::ostringstream intDataStream;
                while (q != json_end(*i))
                {
                    
                    // this format seems to be perfect and the size is working
                    if (m_csv)
                    {
                        json_char* node_value = json_as_string(*q);
                        intDataStream << node_value << ",";
                        json_free(node_value);
                    }
                    else
                    {
                        m_csvData.title = m_csvData.title + title;
                        intDataStream << std::dec << static_cast<uint32_t>(json_as_int(*q));
                        m_csvData.data.append(intDataStream.str());
                    }
                    q++;
                }
                json_free(node_name);
                if (m_csv)
                {
                    m_csvData.title.resize(sizeof(title));
                    m_csvData.title = title;
                    m_csvData.data.resize(intDataStream.str().size());
                    m_csvData.data.assign(intDataStream.str());
                }
                else
                {
                    m_csvData.title.append(title);
                    m_csvData.data.append(data);
                }
            }
            else if (json_type(*q) == JSON_NODE)
            {
                
                if (m_csv)
                {
                    if (numberOfTabs != 0)
                    {
                        for (uint16_t j = 1; j <= numberOfTabs; j++)
                        {
                            m_line.append(",");
                        }
                        m_line.append(main_name);
                        m_line.append("\n");
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
            if (q == M_NULLPTR)
            {
                break;
            }
			if (m_csv)
			{
                if (numberOfTabs != 0)
                {
                    for (uint16_t j = 0; j < numberOfTabs; j++)
                    {
                        m_line.append(",");
                    }
                    m_line.append(main_name);
                    m_line.append("\n");
                }
                else
                {
                    m_line.append(main_name);
                    m_line.append("\n");
                }
			}
            else        // flatcsv needs to copy the data   to the m_csvData strings
            {
                m_csvData.title.append(main_name);
                m_csvData.title.append(",");
                m_csvData.data.append("NONE");
            }
            json_free(main_name);
            parse_Json(*i, numberOfTabs + 1);
            
        }
        if (json_type(*i) == JSON_STRING || json_type(*i) == JSON_BOOL)
        {
            json_char *node_name = json_name(*i);
            json_char *node_value = json_as_string(*i);
            if (node_name == M_NULLPTR || node_value == M_NULLPTR) {
                json_free(node_name);
                json_free(node_value);
                if (eVerbosityLevels::VERBOSITY_COMMAND_VERBOSE <= g_verbosity)
                {
                    std::cout << "\nSomething went wrong JSON (string or bool)" << std::endl;
                }
                return false;
            }
            title.assign(node_name);
            data.assign(node_value);
			createData(title, data, numberOfTabs);
            json_free(node_name);
            json_free(node_value);
        }
        if ((json_type(*i) == JSON_NUMBER))
        {
            json_char *node_name = json_name(*i);
            json_char *node_value = json_as_string(*i);
            if (node_name == M_NULLPTR || node_value == M_NULLPTR) {
                json_free(node_name);
                json_free(node_value);
                if (eVerbosityLevels::VERBOSITY_COMMAND_VERBOSE <= g_verbosity)
                {
                    std::cout << "\nSomething went wrong JSON Number" << std::endl;
                }
                return false;
            }
            title.assign(node_name);
            std::string intData = node_value;
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
    std::string rTitle = "";
    std::string rData = "";

    parse_Json_Flat(masterData);
    rTitle.insert(0, m_singleLine.title);
    rData.insert(0, m_singleLine.data);
  
    m_singleLine.title = "";
    m_singleLine.data = "";

    rTitle.append("\n");
    rData.append("\n");
    rTitle.append( rData );
    return rTitle;
}
//-----------------------------------------------------------------------------
//
//! \fn createLineData
//
//! \brief
//!   Description: Takes the title and the data to format the information for csv or flatcsv 
//
//  Entry:
//! \param title -> title string
//! \param data -> data from the nodes
//
//  Exit:
//!   \return bool
//
//---------------------------------------------------------------------------
bool CPrintCSV::createLineData(const char *title, const char *data)
{
    m_singleLine.title.append(title);
    m_singleLine.title.append(",");
    m_singleLine.data.append(data);
    m_singleLine.data.append(",");
	return true;
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
bool CPrintCSV::createData(std::string &title, std::string &data, uint16_t numberOfTabs)
{
    if (m_csv)
    {
        title.insert(0, numberOfTabs, ',');  // Insert all commas at once
        title = title + ",";
        m_line.append(title);
        data = data + "\n";
        m_line.append(data);
    }
    else
    {
        title = title + ",";
        m_csvData.title.append(title);
		data = data + ",";
        m_csvData.data.append(data);
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
//! \fn create_Tabs()
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

void CPrintTXT::create_Tabs(std::string &title, const std::string &data, uint16_t numberOfTabs)
{
    sFrameData frame;
    title.insert(0, numberOfTabs, '\t'); // Insert tabs at the beginning
    frame.title = title;
    frame.data = data;
    m_vData.push_back(frame);
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
    std::string title;
    std::string data;
    
    JSONNODE_ITERATOR i = json_begin(nData);
    while (i != json_end(nData))
    {
        title = "";
        data = "";
        // recursively call ourselves to dig deeper into the tree
        if (json_type(*i) == JSON_ARRAY)
        {
            get_Array(*i, title, data, numberOfTabs);

        }
        if (json_type(*i) == JSON_NODE)
        {
            get_Node(*i, title, data, numberOfTabs);
        }

        if (json_type(*i) == JSON_STRING || json_type(*i) == JSON_BOOL)
        {
            get_String_Or_Bool(*i, title, data, numberOfTabs);
        }
        if ((json_type(*i) == JSON_NUMBER))
        {
            get_Number(*i,title,data,numberOfTabs);
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
std::string CPrintTXT::get_Msg_Text_Format()
{
    std::string r;
    r.reserve(m_vData.size() * 100);  // Estimate size
    for (const auto& frame : m_vData)
    {
        r.append(frame.title);
        r.append(" : ");
        r.append(frame.data);
        r.append("\n");
    }
    // completed now we will clear the vector 
    m_vData.clear();
    return r;
}
//-----------------------------------------------------------------------------
//! \fn get_Array()
//
//! \brief Extracts the name and value from a JSON array and formats them with tabs.
//
// This function retrieves the name and value from the given JSON array, assigns them 
// to the provided title and data strings, and then formats them with the specified number of tabs.
//
//!   \param node A pointer to the JSON node from which the name and value are extracted.
//!   \param title A reference to a string where the name of the JSON node will be stored.
//!   \param data A reference to a string where the value of the JSON node will be stored.
//!   \param numberOfTabs The number of tabs to be used for formatting the title and data.
//-----------------------------------------------------------------------------
void CPrintTXT::get_Array(JSONNODE* node, std::string& title, std::string& data, uint16_t numberOfTabs)
{
    json_char* main_name = json_name(node);
    JSONNODE_ITERATOR q = json_begin(node);
    if (q == M_NULLPTR)
    {
        title.assign("NULL");//I don't know if this is correct, or if this should be "NULL", but the free was not looking like the correct behavior.
        create_Tabs(title, data, numberOfTabs);
        json_free(main_name);
    }
    else if (json_type(*q) == JSON_STRING)
    {
        // Get the main name created in the title
        title.assign(main_name);
        create_Tabs(title, data, numberOfTabs);
        title.clear();
        uint16_t stringTabs = numberOfTabs + 1;
        // go through all the nodes and get the data but don't change the title
        while (q != json_end(node))
        {
            json_char* newData = json_as_string(*q);
            
            data.assign(newData);
            create_Tabs(title, data, (stringTabs));                 // call Create Tabs to add the tabs into the title
            title.clear();                                             // clear it 
            json_free(newData);
            q++;
        }
        json_free(main_name);
    }
    else if (json_type(*q) == JSON_NUMBER)
    {
        // Get the main name created in the title
        title.assign(main_name);
        std::string nodeData;
        // fill in the data and add ",' between the numbers
        while (q != json_end(node))
        {
            std::ostringstream tempStream;
            tempStream << std::dec << json_as_int(*q) << ",";
            nodeData.append(tempStream.str());
            q++;
        }
        // clean up on the char*
        data = nodeData;
        create_Tabs(title, data, (numberOfTabs));                 // call Create Tabs to add the tabs into the title
        json_free(main_name);
    }
    else if (json_type(*q) == JSON_NODE)
    {
        JSONNODE_ITERATOR k = json_begin(*q);
        if (k != M_NULLPTR)
        {
            json_char* node_name = json_name(*k);
            // set the string data for the array of data
            title.assign(main_name);
            title.append("\n");
            create_Tabs(title, data, (numberOfTabs));                 // call Create Tabs to add the tabs into the title
            parse_Json_to_Text(*q, (numberOfTabs + 1));
            json_free(node_name);
            json_free(main_name);
        }
    }
    else
    {
        json_free(main_name);
    }
}
//-----------------------------------------------------------------------------
//! \fn get_Node()
//
//! \brief Extracts the name and value from a JSON node and formats them with tabs.
//
// This function retrieves the name and value from the given JSON node, assigns them 
// to the provided title and data strings, and then formats them with the specified number of tabs.
//
//!   \param node A pointer to the JSON node from which the name and value are extracted.
//!   \param title A reference to a string where the name of the JSON node will be stored.
//!   \param data A reference to a string where the value of the JSON node will be stored.
//!   \param numberOfTabs The number of tabs to be used for formatting the title and data.
//-----------------------------------------------------------------------------
void CPrintTXT::get_Node(JSONNODE* node, std::string& title, const std::string& data, uint16_t numberOfTabs)
{
    json_char* main_name = json_name(node);
    JSONNODE_ITERATOR q = json_begin(node);
    if (q == M_NULLPTR) {
        json_free(main_name);
        return;
    }
    title = main_name;
    create_Tabs(title, data, numberOfTabs);
    parse_Json_to_Text(node, numberOfTabs + 1);
    json_free(main_name);
}
//-----------------------------------------------------------------------------
//! \fn get_String_Or_Bool()
//
//! \brief Extracts the name and value from a JSON string or bools and formats them with tabs.
//
// This function retrieves the name and value from the given JSON string or bools, assigns them 
// to the provided title and data strings, and then formats them with the specified number of tabs.
//
//!   \param node A pointer to the JSON node from which the name and value are extracted.
//!   \param title A reference to a string where the name of the JSON node will be stored.
//!   \param data A reference to a string where the value of the JSON node will be stored.
//!   \param numberOfTabs The number of tabs to be used for formatting the title and data.
//-----------------------------------------------------------------------------
void CPrintTXT::get_String_Or_Bool(JSONNODE* node, std::string& title, std::string& data, uint16_t numberOfTabs)
{
    char* node_name = json_name(node);
    char* node_value = json_as_string(node);
    if (node_value == M_NULLPTR || node_name == M_NULLPTR) {
        json_free(node_name);
        json_free(node_value);
        return;
    }
    title = node_name;
    data = node_value;
    create_Tabs(title, data, numberOfTabs);
    json_free(node_name);
    json_free(node_value);
}
//-----------------------------------------------------------------------------
//! \fn get_Number()
//
//! \brief Extracts the name and value from a JSON number and formats them with tabs.
//
// This function retrieves the name and value from the given JSON number, assigns them to the provided
// title and data strings, and then formats them with the specified number of tabs.
//
//!   \param node A pointer to the JSON node from which the name and value are extracted.
//!   \param title A reference to a string where the name of the JSON node will be stored.
//!   \param data A reference to a string where the value of the JSON node will be stored.
//!   \param numberOfTabs The number of tabs to be used for formatting the title and data.
//-----------------------------------------------------------------------------
void CPrintTXT::get_Number(JSONNODE* node, std::string& title, std::string& data, uint16_t numberOfTabs) 
{
    json_char* node_name = json_name(node);
    json_char* node_value = json_as_string(node);
    if (node_value == M_NULLPTR || node_name == M_NULLPTR) {
        json_free(node_name);
        json_free(node_value);
        return;
    }
    title = node_name;
    data = node_value;
    create_Tabs(title, data, numberOfTabs);
    json_free(node_name);
    json_free(node_value);
}
/////////////////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//
//! \fn CPrintPyDiction::CPrintPyDiction()
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

//---------------------------------------------------------------------------
//  Parses JSON and converts it to key-value pairs with labels, storing that in a struct
//
//! \param  Pointer to raw JSON data (*JSONNODE)
//! \param  The drive's serial number (string)
//! \param  The name of the JSON node superceding (one level above) the current one
//---------------------------------------------------------------------------
void CPrintProm::parseJSONToProm(JSONNODE* nData, std::string inserialNumber, json_char *json_nodeName) {
    // Declare a metric
    JSONNODE_ITERATOR it_json = json_begin(nData);
    // Iterate through master JSON data
    while (it_json != json_end(nData)) {
        // Clear the metric struct
        metric currentMetric;
        // If the current JSON object is an array, check the types of the objects in the array
        if (json_type(*it_json) == JSON_ARRAY) {
            json_char *jsonArrayName = json_name(*it_json);
            JSONNODE_ITERATOR it_jsonArray = json_begin(*it_json);
            if (json_type(*it_jsonArray) == JSON_NUMBER || json_type(*it_jsonArray) == JSON_STRING || json_type(*it_jsonArray) == JSON_BOOL) {
                // Go through all the items in this array and create new metrics
                int arrayIndex = 0;
                while (it_jsonArray != json_end(*it_json)) {
                    // Set the key
                    currentMetric.key = trim(std::string(jsonArrayName), " ");
                    json_char *currentValue = json_as_string(*it_jsonArray);
                    // Insert the index of the array as a label
                    std::ostringstream stream;
                    stream << arrayIndex;
                    std::string arrayIndex_str = stream.str();
                    currentMetric.labelMap.insert(std::pair<std::string, std::string>("index", arrayIndex_str));
                    // Insert the drive's serial number as a label
                    currentMetric.labelMap.insert(std::pair<std::string, std::string>("device", inserialNumber));
                    // If the JSON type is a string, check if it can be parsed as a number; otherwise, pass the value in though the label
                    if (json_type(*it_json) == JSON_STRING) {
                        // If the value is a number, set the value accordingly
                        currentMetric.value = trim(std::string(currentValue), " ");
                        // If the value is not a number (by our definition), then pass the value in through a label and set the value to NaN
                        if (!isNumber(currentMetric.value)) {
                            currentMetric.labelMap.insert(std::pair<std::string, std::string>("value", currentMetric.value));
                            currentMetric.value = "NaN";
                        }
                    // If the JSON type is a number, set the value
                    } else if (json_type(*it_json) == JSON_NUMBER) {
                        currentMetric.value = std::string(currentValue);
                    // If the JSON type is a boolean, set the value to 0 for false or 1 for true
                    } else if (json_type(*it_json) == JSON_BOOL) {
                        if (std::string(currentValue) == "true") {
                            currentMetric.value = "1";
                        } else {
                            currentMetric.value = "0";
                        }
                    }
                    // Push the metric to the total metric vector
                    m_metricList.push_back(currentMetric);
                    // Clear the pointer holding the value
                    json_free(currentValue);
                    // Clear the metric struct
                    currentMetric.key.clear();
                    currentMetric.labelMap.clear();
                    currentMetric.value.clear();
                    arrayIndex++;
                    it_jsonArray++;
                }
                // Clear the pointer storing the JSON array's name
                json_free(jsonArrayName);
            // If the values in the array are nodes, run this method recursively on the JSON array
            } else if (json_type(*it_jsonArray) == JSON_NODE) {
                JSONNODE_ITERATOR it_jsonArrayNode = json_begin(*it_jsonArray);
                json_char *jsonArrayNodeName = json_name(*it_jsonArrayNode);
                // Do nothing if the node is NULL

                // Run this method recursively
                parseJSONToProm(*it_jsonArray, inserialNumber, jsonArrayNodeName);
                // Clear pointers
                json_free(jsonArrayNodeName);
                json_free(jsonArrayName);
            // Otherwise, simply free the data as it cannot be made into a Prometheus metric
            } else {
                json_free(jsonArrayName);
            }
        // If the current JSON object is a string, number, or boolean, set the key, value, and labels of the current metric
        } else if (json_type(*it_json) == JSON_STRING || json_type(*it_json) == JSON_BOOL || json_type(*it_json) == JSON_NUMBER) {
            json_char *jsonName = json_name(*it_json);
            // Set the key
            currentMetric.key = trim(std::string(jsonName), " ");
            json_char *currentValue = json_as_string(*it_json);
            // Insert the drive's serial number as a label
            currentMetric.labelMap.insert(std::pair<std::string, std::string>("device", inserialNumber));
            // If the key is describing a head with a given number, convert that to a label
            if (currentMetric.key.find("Head ") != std::string::npos) {
                if (currentMetric.key.find_first_of("0123456789") != std::string::npos) {
                    currentMetric = headToLabel(currentMetric);
                }
            }
            // If the key is describing a test zone with a given number, convert that to a label
            if (json_nodeName != M_NULLPTR) {
                if (std::string(json_nodeName).find("Test Zone ") != std::string::npos || currentMetric.key.find("Zone ") != std::string::npos) {
                    if (currentMetric.key.find_first_of("0123456789") != std::string::npos) {
                        currentMetric = zoneToLabel(currentMetric);
                        // Define the label names
                        const std::string ZONE_NUMBER_LABEL = "zone";
                        // If the zone number is in the node name, not key, add it as a label
                        if (currentMetric.labelMap.find(ZONE_NUMBER_LABEL) == currentMetric.labelMap.end()) {
                            // Put string of the current metric's node's name into a stream
                            std::string nodeName = std::string(json_nodeName);
                            std::stringstream currentKeyStream(nodeName);
                            // Copy the contents into a vector splitting the node name into words (whitespace separated)
                            std::vector<std::string> nodeNameFields((std::istream_iterator<std::string>(currentKeyStream)), std::istream_iterator<std::string>());
                            // If the node name contains "Zone " followed by its zone number, add that number as a label
                            for (std::string::size_type i = 0; i < nodeNameFields.size() - 1; i++) {
                                if (nodeNameFields.at(i) == "Zone") {
                                    if (isNumber(nodeNameFields.at(i + 1))) {
                                        currentMetric.labelMap.insert(std::pair<std::string, std::string>(ZONE_NUMBER_LABEL, nodeNameFields.at(i + 1)));
                                    }
                                }
                            }
                        }
                    }
                }
            }
            // If the JSON type is a string, check if it can be parsed as a number; otherwise, pass the value in though the label
            if (json_type(*it_json) == JSON_STRING) {
                // If the value is a number, set the value accordingly
                currentMetric.value = trim(std::string(currentValue), " ");
                // If the value is not a number (by our definition), then pass the value in through a label and set the value to NaN
                if (!isNumber(currentMetric.value)) {
                    currentMetric.labelMap.insert(std::pair<std::string, std::string>("value", currentMetric.value));
                    currentMetric.value = "NaN";
                }
            // If the JSON type is a number, set the value
            } else if (json_type(*it_json) == JSON_NUMBER) {
                currentMetric.value = std::string(currentValue);
            // If the JSON type is a boolean, set the value to 0 for false or 1 for true
            } else if (json_type(*it_json) == JSON_BOOL) {
                if (std::string(currentValue) == "true") {
                    currentMetric.value = "1";
                } else {
                    currentMetric.value = "0";
                }
            }
            // Push the metric to the total metric vector
            m_metricList.push_back(currentMetric);
            // Clear the pointer holding the value
            json_free(currentValue);
            // Clear the pointer storing the JSON array's name
            json_free(jsonName);
            // Clear the metric struct
            currentMetric.key.clear();
            currentMetric.labelMap.clear();
            currentMetric.value.clear();
        // If the current JSON object is a node, recursively run this function
        } else if (json_type(*it_json) == JSON_NODE) {
            json_char *jsonNodeName = json_name(*it_json);
            JSONNODE_ITERATOR it_jsonNode = json_begin(*it_json);
            // Do nothing if the node is NULL
            if (it_jsonNode == M_NULLPTR) {
                json_free(jsonNodeName);
                break;
            }
            // Run this method recursively
            parseJSONToProm(*it_json, inserialNumber, jsonNodeName);
            // Clear pointers
            json_free(jsonNodeName);
        }
        it_json++;
    }
}

//---------------------------------------------------------------------------
//! \fn Takes values from struct and formats data into a PromQL string
//
//! \return Data in PromQL format (string)
//---------------------------------------------------------------------------
std::string CPrintProm::printProm() {
    std::string promOutput = "";
    // Iterate through the list of metrics and add it to the output string, formatted
    for (std::vector<metric>::iterator it_metric = m_metricList.begin(); it_metric != m_metricList.end(); it_metric++) {
        // Add the key as the metric name after is has been converted to Prometheus' format
        promOutput.append(toPrometheusKey(it_metric->key));
        // Iterate through label map and add labels to the output string, formatted
        for (std::map<std::string, std::string>::iterator it_label = it_metric->labelMap.begin(); it_label != it_metric->labelMap.end(); it_label++) {
            // For the first label in this metric, add an opening curly brace
            if (it_label == it_metric->labelMap.begin()) {
                promOutput.append("{");
            }
            // Add the label: key="value"
            promOutput.append(it_label->first);
            promOutput.append("=\"");
            promOutput.append(it_label->second);
            promOutput.append("\"");
            // For the last label in this metric, add a closing curly brace; otherwise, add a comma and a space
            it_label++;
            if (it_label == it_metric->labelMap.end()) {
                it_label--;
                promOutput.append("}");
            } else {
                it_label--;
                promOutput.append(",");
            }
        }
        // Add a space, the metric value, then a new line
        promOutput.append(" ");
        promOutput.append(it_metric->value);
        promOutput.append("\n");
    }
    // Clear the list afterward
    m_metricList.clear();
    return promOutput;
}

//---------------------------------------------------------------------------
//! \fn Converts a string to a metric name in Prometheus' desired format
//
//! \param  A key to convert (string)
//! \return The modified key in Prometheus' format (string)
//---------------------------------------------------------------------------
std::string CPrintProm::toPrometheusKey(std::string key) {
    // Specifies the prefix to add to each metric and which character separates words
    const std::string PREFIX = "seachest";
    const std::string REPLACE = "_";
    // Removes spaces and other undesired characters with this regular expression
    size_t replaceIndex = 0;
    size_t replaceLength = 0;
    // Replaces non-alphanumeric characters with one REPLACE string (consecutive characters get replaced by one REPLACE string)
    for (std::string::size_type i = 0; i < key.size(); i++) {
        if (!isdigit(key.at(i)) && !isalpha(key.at(i))) {
            if (replaceLength == 0) {
                replaceIndex = i;
            }
            replaceLength++;
            if (i == key.size() - 1) {
                key.replace(replaceIndex, replaceLength, REPLACE);
                break;
            }
        } else {
            if (replaceLength > 0) {
                key.replace(replaceIndex, replaceLength, REPLACE);
                replaceLength = 0;
            }
        }
    }
    return PREFIX + REPLACE + trim(key, REPLACE);
}

//---------------------------------------------------------------------------
//! \fn  Removes all leading instances of a given string
//
//! \param  The string that is being trimmed (string)
//! \param  The given string to replace (const string)
//! \return A string with all its leading instances of REPLACE removed
//---------------------------------------------------------------------------
std::string CPrintProm::trimLeft(std::string s, const std::string &REPLACE) {
    size_t start = s.find_first_not_of(REPLACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

//---------------------------------------------------------------------------
//! \fn Removes all trailing instances of a given string
//
//! \param  The string that is being trimmed (string)
//! \param  The given string to replace (const string)
//! \return A string with all its trailing instances of REPLACE removed
//---------------------------------------------------------------------------
std::string CPrintProm::trimRight(std::string s, const std::string &REPLACE) {
    size_t end = s.find_last_not_of(REPLACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

//---------------------------------------------------------------------------
//! \fn Removes all leading and trailing instances of a given string
//
//! \param  The string that is being trimmed (string)
//! \param  The given string to replace (const string)
//! \return A string with all its leading and trailing instances of REPLACE removed
//---------------------------------------------------------------------------
std::string CPrintProm::trim(std::string s, const std::string &REPLACE) {
    return trimRight(trimLeft(s, REPLACE), REPLACE);
}

//---------------------------------------------------------------------------
//! \fn  Getter for private serialNumber variable
//
//! \return The current drive's serial number (string)
//---------------------------------------------------------------------------
std::string CPrintProm::getSerialNumber() {
    return serialNumber;
}

//---------------------------------------------------------------------------
//! \fn Gets drive serial number from JSON data
//
//! \param  Pointer to raw JSON data (*JSONNODE)
//---------------------------------------------------------------------------
void CPrintProm::setSerialNumber(JSONNODE *nData) {
    JSONNODE_ITERATOR it_json = json_begin(nData);
    // Iterate through the JSON, and if the "Serial Number" field is found, set the serial number accordingly
    while (it_json != json_end(nData)) {
        if (json_type(*it_json) == JSON_STRING) {
            json_char *jsonName = json_name(*it_json);
            if (std::string(jsonName) == "Serial Number") {
                json_char *currentValue = json_as_string(*it_json);
                serialNumber = std::string(currentValue);
                break;
            }
            // Recursively run this method if the type is a node
        } else if (json_type(*it_json) == JSON_NODE) {
            JSONNODE_ITERATOR it_jsonNode = json_begin(*it_json);
            // Do nothing if the node is NULL
            if (it_jsonNode == M_NULLPTR) {
                break;
            }
            // Run this method recursively
            setSerialNumber(*it_json);
        }
        it_json++;
    }
}

//---------------------------------------------------------------------------
//! \fn Determines if a given string can be parsed as a number
//
//! \param  The string to parse (string)
//! \return True if the string is a number; false if the string is not a number
//---------------------------------------------------------------------------
bool CPrintProm::isNumber(std::string s) {
    // Matches an optional sign (+ or -) followed by a floating point number (one decimal followed by integers) or an integer
    bool decimalPresent = false;
    for (std::string::size_type i = 0; i < s.size(); i++) {
        if (!isdigit(s.at(i))) {
            if (s.at(i) == '.') {
                if (i >= s.size() - 1) {
                    return false;
                } else if (decimalPresent) {
                    return false;
                }
                decimalPresent = true;
            } else if (s.at(i) == '+' || s.at(i) == '-') {
                if (i != 0) {
                    return false;
                }
            } else {
                return false;
            }
        }
    }
    return true;
}

//---------------------------------------------------------------------------
//! \fn  Modifies the metric so that its name is concise while the drive head's number is moved to a label
//
//! \param  A metric struct with head in the key (metric)
//! \return A metric struct with head as a label (metric)
//---------------------------------------------------------------------------
CPrintProm::metric CPrintProm::headToLabel(metric currentMetric) {
    // Put string of the current metric's key into a stream
    std::stringstream currentKeyStream(currentMetric.key);
    // Copy the contents into a vector splitting the key into words (whitespace separated)
    std::vector<std::string> keyFields((std::istream_iterator<std::string>(currentKeyStream)), std::istream_iterator<std::string>());
    // Define which words to remove
    std::vector<std::string> REMOVE_BEFORE_VECTOR; 
    REMOVE_BEFORE_VECTOR.push_back("by");
    REMOVE_BEFORE_VECTOR.push_back("for");
    REMOVE_BEFORE_VECTOR.push_back("from");
    std::vector<std::string> REMOVE_AFTER_VECTOR;
    REMOVE_AFTER_VECTOR.push_back("number");
    // Define the label names
    const std::string HEAD_NUMBER_LABEL = "head";
    const std::string HEAD_LOCATION_LABEL = "head_location";
    // Remove any specified words that are followed by "Head"
    for (std::size_t i = 0; i < keyFields.size(); i++) {
        if (std::find(REMOVE_BEFORE_VECTOR.begin(), REMOVE_BEFORE_VECTOR.end(), keyFields.at(i)) != REMOVE_BEFORE_VECTOR.end()) {
            if (i + 1 >= keyFields.size()) {
                keyFields.erase(keyFields.begin() + i);
                break;
            } else if (keyFields.at(i + 1) == "Head") {
                keyFields.erase(keyFields.begin() + i);
                break;
            }
        }
    }
    // Remove any specified words that follow "Head"
    for (std::size_t i = 1; i < keyFields.size(); i++) {
        if (std::find(REMOVE_AFTER_VECTOR.begin(), REMOVE_AFTER_VECTOR.end(), keyFields.at(i)) != REMOVE_AFTER_VECTOR.end()) {
            if (keyFields.at(i - 1) == "Head") {
                keyFields.erase(keyFields.begin() + i);
                break;
            }
        }
    }
    // Remove the head number and add it as a label
    for (std::size_t i = 0; i < keyFields.size() - 1; i++) {
        if (keyFields.at(i) == "Head") {
            if (isNumber(keyFields.at(i + 1))) {
                currentMetric.labelMap.insert(std::pair<std::string, std::string>(HEAD_NUMBER_LABEL, keyFields.at(i + 1)));
                keyFields.erase(keyFields.begin() + i + 1);
                break;
            }
        }
    }
    // Remove the "Head" text if it had a head number associated with it
    for (std::size_t i = 0; i < keyFields.size(); i++) {
        if (keyFields.at(i) == "Head") {
            if (currentMetric.labelMap.find(HEAD_NUMBER_LABEL) != currentMetric.labelMap.end()) {
                keyFields.erase(keyFields.begin() + i);
                break;
            }
        }
    }
    // Add the "inner," "middle," and "outer" labels
    for (std::size_t i = 0; i < keyFields.size(); i++) {
        if (keyFields.at(i) == "inner" || keyFields.at(i) == "Inner") {
            currentMetric.labelMap.insert(std::pair<std::string, std::string>(HEAD_LOCATION_LABEL, "inner"));
            keyFields.erase(keyFields.begin() + i);
            break;
        } else if (keyFields.at(i) == "middle" || keyFields.at(i) == "Middle") {
            currentMetric.labelMap.insert(std::pair<std::string, std::string>(HEAD_LOCATION_LABEL, "middle"));
            keyFields.erase(keyFields.begin() + i);
            break;
        } else if (keyFields.at(i) == "outer" || keyFields.at(i) == "Outer") {
            currentMetric.labelMap.insert(std::pair<std::string, std::string>(HEAD_LOCATION_LABEL, "outer"));
            keyFields.erase(keyFields.begin() + i);
            break;
        }
    }
    // Put the contents of the vector back in a string storing the metric's key
    currentMetric.key = "";
    for (std::size_t i = 0; i < keyFields.size(); i++) {
        currentMetric.key.append(keyFields.at(i));
        if (i <= keyFields.size() - 2) {
            currentMetric.key.append(" ");
        }
    }
    return currentMetric;
}

//---------------------------------------------------------------------------
//! \fn Modifies the metric so that its name is concise while the drive head's number is moved to a label
//
//! \param  A metric struct with test zone in the key (metric)
//! \return A metric struct with test zone as a label (metric)
//---------------------------------------------------------------------------
CPrintProm::metric CPrintProm::zoneToLabel(metric currentMetric) {
    // Put string of the current metric's key into a stream
    std::stringstream currentKeyStream(currentMetric.key);
    // Copy the contents into a vector splitting the key into words (whitespace separated)
    std::vector<std::string> keyFields((std::istream_iterator<std::string>(currentKeyStream)), std::istream_iterator<std::string>());
    // Define which words to remove
    std::vector<std::string> REMOVE_BEFORE_VECTOR;
    REMOVE_BEFORE_VECTOR.push_back("of");
    // Define the label names
    const std::string ZONE_NUMBER_LABEL = "zone";
    // Remove any specified words that are followed by "Zone"
    for (std::size_t i = 0; i < keyFields.size(); i++) {
        if (std::find(REMOVE_BEFORE_VECTOR.begin(), REMOVE_BEFORE_VECTOR.end(), keyFields.at(i)) != REMOVE_BEFORE_VECTOR.end()) {
            if (i + 1 >= keyFields.size()) {
                keyFields.erase(keyFields.begin() + i);
                break;
            } else if (keyFields.at(i + 1) == "Zone") {
                keyFields.erase(keyFields.begin() + i);
                break;
            }
        }
    }
    // Remove the zone number and add it as a label
    for (std::size_t i = 0; i < keyFields.size() - 1; i++) {
        if (keyFields.at(i) == "Zone") {
            if (isNumber(keyFields.at(i + 1))) {
                currentMetric.labelMap.insert(std::pair<std::string, std::string>(ZONE_NUMBER_LABEL, keyFields.at(i + 1)));
                keyFields.erase(keyFields.begin() + i + 1);
                break;
            }
        }
    }
    // Remove the "Test" text if it had a head number associated with it and is followed by "Zone"
    for (std::size_t i = 1; i < keyFields.size(); i++) {
        if (keyFields.at(i) == "Zone") {
            if (currentMetric.labelMap.find(ZONE_NUMBER_LABEL) != currentMetric.labelMap.end()) {
                if (keyFields.at(i - 1) == "Test") {
                    keyFields.erase(keyFields.begin() + i - 1);
                    break;
                }
            }
        }
    }
    // Remove the "Zone" text if it had a head number associated with it
    for (std::size_t i = 0; i < keyFields.size(); i++) {
        if (keyFields.at(i) == "Zone") {
            if (currentMetric.labelMap.find(ZONE_NUMBER_LABEL) != currentMetric.labelMap.end()) {
                keyFields.erase(keyFields.begin() + i);
                break;
            }
        }
    }
    // Put the contents of the vector back in a string storing the metric's key
    currentMetric.key = "";
    for (std::size_t i = 0; i < keyFields.size(); i++) {
        currentMetric.key.append(keyFields.at(i));
        if (i <= keyFields.size() - 2) {
            currentMetric.key.append(" ");
        }
    }
    return currentMetric;
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
	:CPrintJSON(masterData), CPrintTXT(), CPrintCSV(), CPrintProm()
    , msgData(masterData)
    , printStatus(0)
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

CMessage::CMessage(JSONNODE *masterData, std::string fileName, ePrintTypes toolPrintType)
    :CPrintJSON(masterData), CPrintTXT(), CPrintCSV(), CPrintProm()
    , msgData(masterData)
    , printStatus(0)
    , printType(toolPrintType)
    , m_fileName(fileName)
    , message()
{
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
//!   \return eReturnValues::SUCCESS or FILE_OPEN_ERROR                   /// do we need any more ??????
//
//---------------------------------------------------------------------------
int CMessage::WriteBuffer()
{
    switch (printType)                                      // for now it will only do json
    {
    case ePrintTypes::LOG_PRINT_JSON:
        message = get_Msg_JSON_Data();                      // get the string message for printable json data
        break;
    case ePrintTypes::LOG_PRINT_TEXT:
        parse_Json_to_Text(msgData,0);                        // parse the json data into a vector 
        message = get_Msg_Text_Format();             // get the string message for printable test format data
        break;
    case ePrintTypes::LOG_PRINT_CSV:
        message = get_Msg_CSV(msgData);                     // get the string message normal CSV format data
        break;
    case ePrintTypes::LOG_PRINT_FLAT_CSV:
        message = get_Msg_Flat_csv(msgData);                // get the json data to create a flat csv 
        break;
    case ePrintTypes::LOG_PRINT_PROM:
        // If the "prom" printType is specified, parse the JSON and convert it to PromQL
        // First get the drive serial number
        setSerialNumber(msgData);
        // Then parse the JSON, storing it in a struct matching Promtheus' format
        parseJSONToProm(msgData, getSerialNumber(), M_NULLPTR);
        // Then scan through the structure and print the output to a .prom file
        message = printProm();
        break;
    default:
        message = get_Msg_JSON_Data();                      // get the string message for printable json data
        break;
    }

    logFile.open(m_fileName.c_str(), std::ios::out | std::ios::trunc);//only allow reading as a binary file;
    if (logFile.is_open())
    {
        logFile.write(message.c_str(), message.length());            // write the string out to the log
        logFile.close();
    }
    else
    {
        printStatus = static_cast<int>(eReturnValues::FILE_OPEN_ERROR);
        return static_cast<int>(eReturnValues::FILE_OPEN_ERROR);
    }


    if (eVerbosityLevels::VERBOSITY_DEFAULT < g_verbosity)
    {
        std::cout << "\nWrite Buffer wrote " << message.length() << " bytes to file.\n";
    }

    return static_cast<int>(eReturnValues::SUCCESS);
}