//
// parser_print_util_options.h
//
// Do NOT modify or remove this copyright and license
//
// Copyright (c) 2015 - 2020 Seagate Technology LLC and/or its Affiliates, All Rights Reserved
//
// This software is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// ******************************************************************************************

// \file parser_print_util_options.h
// \brief This file defines the specific funtions and macros for parser Utilities. 

#pragma once

#include <string>
#include "common.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "libjson.h"
#include "Opensea_Parser_Helper.h"

namespace opensea_parser {
#ifndef PARSER_PRINT_UTIL_OPTIONS
#define PARSER_PRINT_UTIL_OPTIONS


    class CPrintJSON
    {
    private:
        JSONNODE                    *m_JsonData;
        std::string                 m_jsonMessage;

    public:
		CPrintJSON();
        CPrintJSON(JSONNODE *masterData);
        virtual ~CPrintJSON();
        std::string get_Msg_JSON_Data(){ return m_jsonMessage; };                               //!< returns string data for printing json data 
    };

    class CPrintCSV {
    private:
    #pragma pack(push, 1)
        typedef struct _sCSVFrameData
        {
            std::string title;                                                                  //!< title information from the node
            std::string data;                                                                   //!< Data information from the node
        }sCSVFrameData;
    #pragma pack(pop)
        sCSVFrameData               m_csvData;                                                  //!< frame Parameter data
		std::string					m_line;														//!< line string for csv formating
		bool						m_csv;														//!< if true then we need to format for csv print				
       
        bool parse_Json(JSONNODE *nData, uint16_t numberOfTabs);

    public:
        CPrintCSV();
        virtual ~CPrintCSV();
        std::string get_Msg_CSV(JSONNODE *masterData);											//!< returns string data for a normal CSV, comma delimited
        std::string get_Msg_Flat_csv(JSONNODE *masterData);                                     //!< returns string data for creating a csv all on two lines, comma delimited
		bool createData(std::string &title, std::string &data, uint16_t numberOfTabs);          //!< creates the Data for the csv and flat csv, tab vs comma
    };

    class CPrintTXT
    {
    private:
#pragma pack(push, 1)
        typedef struct _sFrameData
        {
            std::string title;                                                                  //!< title information from the node
            std::string data;                                                                   //!< Data information from the node
        }sFrameData;
#pragma pack(pop)
        std::vector<sFrameData>               m_vData;                                      //!< frame Parameter data
        std::string					m_line;														//!< line string for csv formating
        
    public:
        CPrintTXT();
        virtual ~CPrintTXT();
        bool parse_Json_to_Text(JSONNODE *nData, uint16_t numberOfTabs);
        bool Create_Tabs(std::string &title, std::string &data, uint16_t numberOfTabs);          //!< creates the Data for the text tabs
        std::string get_Msg_Text_Format(const std::string message);                              //!< returns the json data as a text string
    };

    class CMessage : public CPrintJSON, public  CPrintTXT, public CPrintCSV
    {
    private:
        JSONNODE *msgData;
        std::fstream logFile;
        int printStatus;
        int printType;
        std::string m_fileName;
        std::string message;

    public:
		CMessage(JSONNODE *masterData);
        CMessage(JSONNODE *masterData, std::string fileName, int toolPrintType);
        virtual ~CMessage();
        void Msg(char *message);
        int WriteBuffer();


    };
#endif // !PARSER_PRINT_UTIL_OPTIONS
};