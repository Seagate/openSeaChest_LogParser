//
// parser_print_util_options.h
//
// Do NOT modify or remove this copyright and license
//
// Copyright (c) 2015 - 2023 Seagate Technology LLC and/or its Affiliates, All Rights Reserved
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
#include <map>

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
        explicit CPrintJSON(JSONNODE *masterData);
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
        sCSVFrameData				m_singleLine;												//!< line string for flatcsv formating
		std::string					m_line;														//!< line string for csv formating
		bool						m_csv;														//!< if true then we need to format for csv print				
       
        bool parse_Json(JSONNODE *nData, uint16_t numberOfTabs);
        bool parse_Json_Flat(JSONNODE* nData);

    public:
        CPrintCSV();
        virtual ~CPrintCSV();
        std::string get_Msg_CSV(JSONNODE *masterData);											//!< returns string data for a normal CSV, comma delimited
        std::string get_Msg_Flat_csv(JSONNODE *masterData);                                     //!< returns string data for creating a csv all on two lines, comma delimited
		bool createData(std::string &title, std::string &data, uint16_t numberOfTabs);          //!< creates the Data for the csv and flat csv, tab vs comma
        bool createLineData(const char* title, const char* data);                               //!< create the data for a csb that is flat only.
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
        std::vector<sFrameData>               m_vData;                                          //!< frame Parameter data
        std::string					m_line;														//!< line string for csv formating
        
    public:
        CPrintTXT();
        virtual ~CPrintTXT();
        bool parse_Json_to_Text(JSONNODE *nData, uint16_t numberOfTabs);
        bool Create_Tabs(std::string &title, const std::string &data, uint16_t numberOfTabs);    //!< creates the Data for the text tabs
        std::string get_Msg_Text_Format(const std::string message);                              //!< returns the json data as a text string
    };

    class CPrintProm {
    private:
        // PromQL metric in format: metric_key{label_key="label_value", ...} metric_value
        typedef struct metric {
            std::string key;                                                                            // Stores key in Prometheus format
            std::map<std::string, std::string> labelMap;                                                // Map of labels
            std::string value;                                                                          // Stores value for metric
        } metric;
        std::vector<metric> m_metricList;                                                               // List of metrics
        std::string serialNumber;                                                                       // Serial number of current drive
        std::string toPrometheusKey(std::string key);                                                   // Converts a key to Prometheus' desired format
        std::string trimLeft(std::string s, const std::string &REPLACE);                                 // Removes all leading instances of the given string REPLACE
        std::string trimRight(std::string s, const std::string &REPLACE);                                // Removes all trailing instances of the given string REPLACE
        std::string trim(std::string s, const std::string &REPLACE);                                     // Removes all leading and trailing instances of the given string REPLACE
        bool isNumber(std::string s);                                                                   // Determines if the given string can be parsed as a number
        metric headToLabel(metric currentMetric);                                                       // Modifies a metric so that the head number is a label rather than part of the key
        metric zoneToLabel(metric currentMetric);                                                       // Modifies a metric so that the test zone number is a label rather than part of the key
    public:
        void setSerialNumber(JSONNODE *nData);                                                          // Sets serial number
        std::string getSerialNumber();                                                                  // Gets serial number
        void parseJSONToProm(JSONNODE *nData, std::string serialNumber, json_char *json_nodeName);      // Takes JSON, sets key-value pairs in struct
        std::string printProm();                                                                        // Takes key-value pairs in struct, prints to string
    };

    class CMessage : public CPrintJSON, public  CPrintTXT, public CPrintCSV, public CPrintProm
    {
    private:
        JSONNODE *msgData;
        std::fstream logFile;
        int printStatus;
        int printType;
        std::string m_fileName;
        std::string message;
    public:
		explicit CMessage(JSONNODE *masterData);
        CMessage(JSONNODE *masterData, std::string fileName, int toolPrintType);
        virtual ~CMessage();
        void Msg(char *message);
        int WriteBuffer();


    };
#endif // !PARSER_PRINT_UTIL_OPTIONS
};