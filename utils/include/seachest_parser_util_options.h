//
// Do NOT modify or remove this copyright and license
//
// Copyright (c) 2012 - 2018 Seagate Technology LLC and/or its Affiliates, All Rights Reserved
//
// This software is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// ******************************************************************************************
//
// \file seachest_util_options.h
// \brief This file defines the functions and macros to make building a seachest utility easier.

#pragma once


#include <string>
#include "common.h"
#include <iostream>
#include "Opensea_Parser_Helper.h"

//this is being defined for using bools with getopt since using a bool (1 byte typically) will cause stack corruption at runtime
//This type should only be used where a boolean is desired when using the getopt parser (which expects an int), otherwise bool will do just fine
#define getOptBool int
#define goFalse 0
#define goTrue !goFalse
#define FORMAT_SIZE 80
typedef enum _eUtilExitCodes
{
    UTIL_EXIT_NO_ERROR = 0,
    UTIL_EXIT_ERROR_IN_COMMAND_LINE,
    UTIL_EXIT_INVALID_DEVICE_HANDLE,
    UTIL_EXIT_OPERATION_FAILURE,
    UTIL_EXIT_OPERATION_NOT_SUPPORTED,
	UTIL_EXIT_OPERATION_STILL_IN_PROGRESS,
    UTIL_EXIT_OPERATION_ABORTED,
	UTIL_EXIT_OPERATION_BAD_PARAMETER,
	UTIL_EXIT_OPERATION_MEMORY_FAILURE,
	UTIL_EXIT_OPERATION_INVALID_LENGTH,
    UTIL_EXIT_PATH_NOT_FOUND,
    UTIL_EXIT_CANNOT_OPEN_FILE,
    UTIL_EXIT_FILE_ALREADY_EXISTS,
    UTIL_EXIT_FILE_NOT_VALID,
}eUtilExitCodes;

typedef enum _eLogTypes
{
	SEAGATE_LOG_TYPE_UNKNOWN = 0,
	SEAGATE_LOG_TYPE_FARM,
	SEAGATE_LOG_TYPE_DEVICE_STATISTICS_LOG,
	SEAGATE_LOG_TYPE_EXT_COMPREHENSIVE_LOG,
	SEAGATE_LOG_TYPE_EXT_DST_LOG,
	SEAGATE_LOG_TYPE_IDENTIFY_LOG,
	SEAGATE_LOG_TYPE_IDENTIFY_DEVICE_DATA,
	SEAGATE_LOG_TYPE_SCT_TEMP_LOG,
	SEAGATE_LOG_TYPE_POWER_CONDITION_LOG,
	SEAGATE_LOG_TYPE_NCQ_CMD_ERROR_LOG,
	SEAGATE_LOG_TYPE_SCSI_LOG_PAGES,

}logTypes;

typedef enum _ePrintTypes
{
	SEAGATE_LOG_PRINT_JSON,     // default
	SEAGATE_LOG_PRINT_TEXT,
	SEAGATE_LOG_PRINT_CSV,
	SEAGATE_LOG_PRINT_FLAT_CSV,
	SEAGATE_LOG_PRINT_FAILURE,
}printTypes;

//Log Parser Input flag
#define INPUT_LOG_FILE_FLAG inputLog
#define INPUT_LOG_FILE_NAME InputLogFile
#define INPUT_LOG_FILE_VAR \
	bool INPUT_LOG_FILE_FLAG = false;\
	std::string INPUT_LOG_FILE_NAME;
#define INPUT_LOG_LONG_OPT_STRING "inputLog"
#define INPUT_LOG_LONG_OPT { INPUT_LOG_LONG_OPT_STRING, required_argument, NULL, 0}
	void print_Input_Log_Help(bool shortHelp);

	//Log Parser Output flag
#define OUTPUT_LOG_FILE_FLAG outputFile
#define OUTPUT_LOG_FILE_NAME outputLogFile
#define OUTPUT_LOG_FILE_VAR \
	bool OUTPUT_LOG_FILE_FLAG = false; \
	std::string OUTPUT_LOG_FILE_NAME;
#define OUTPUT_LOG_LONG_OPT_STRING "outputLog"
#define OUTPUT_LOG_LONG_OPT { OUTPUT_LOG_LONG_OPT_STRING, required_argument, NULL, 0 }


#define LOG_TYPE_STRING_FARM						"farmLog"
#define LOG_TYPE_STRING_DEVICE_STATISTICS_LOG		"deviceStatisticsLog"
#define LOG_TYPE_STRING_EXT_COMPREHENSIVE_LOG		"extCompErrorLog"
#define LOG_TYPE_STRING_COMMON_ERROR_LOG			"commonErrorLog"
#define LOG_TYPE_STRING_EXT_DST_LOG					"extSelfTestLog"
#define LOG_TYPE_STRING_IDENTIFY_LOG				"identify"
#define	LOG_TYPE_STRING_IDENTIFY_DEVICE_DATA_LOG	"IDDataLog"
#define LOG_TYPE_STRING_SCT_TEMP_LOG				"sctTempLog"
#define LOG_TYPE_STRING_POWER_CONDITION_LOG			"powerConditionLog"
#define LOG_TYPE_STRING_NCQ_COMMAND_ERROR_LOG		"ncqErrorLog"
#define LOG_TYPE_STRING_SCSI_LOG_PAGES				"scsiLogPages"


#define INPUT_LOG_TYPE_FLAG logType
#define INPUT_LOG_TYPE_VAR logTypes INPUT_LOG_TYPE_FLAG = SEAGATE_LOG_TYPE_UNKNOWN;
#define INPUT_LOG_TYPE_LONG_OPT_STRING "logType"
#define INPUT_LOG_TYPE_LONG_OPT { INPUT_LOG_TYPE_LONG_OPT_STRING, required_argument, NULL, 0}


// output file types
#define LOG_PRINT_STRING_JSON "json"
#define LOG_PRINT_STRING_TEXT "text"
#define LOG_PRINT_STRING_CSV "csv"
#define LOG_PRINT_STRING_FLATCSV "flatcsv"

#define OUTPUT_LOG_PRINT_FLAG printType
#define OUTPUT_LOG_PRINT_VAR printTypes OUTPUT_LOG_PRINT_FLAG = SEAGATE_LOG_PRINT_JSON;
#define OUTPUT_LOG_PRINT_LONG_OPT_STRING "printType"
#define OUTPUT_LOG_PRINT_LONG_OPT { OUTPUT_LOG_PRINT_LONG_OPT_STRING, required_argument, NULL, 0 }


//standard utility options
	
#define SHOW_HELP_FLAG showHelp
#define SHOW_HELP_VAR bool SHOW_HELP_FLAG = false;
#define HELP_SHORT_OPT 'h'
#define HELP_LONG_OPT_STRING "help"
#define HELP_LONG_OPT { HELP_LONG_OPT_STRING, no_argument, NULL, HELP_SHORT_OPT }

#define SHOW_BANNER_FLAG showBanner
#define SHOW_BANNER_VAR bool SHOW_BANNER_FLAG = false;
#define VERSION_SHORT_OPT 'V'
#define VERSION_LONG_OPT_STRING "version"
#define VERSION_LONG_OPT { VERSION_LONG_OPT_STRING, no_argument, NULL, VERSION_SHORT_OPT }

#define VERBOSE_SHORT_OPT 'v'
#define VERBOSE_LONG_OPT_STRING "verbose"
#define VERBOSE_LONG_OPT { VERBOSE_LONG_OPT_STRING, required_argument, NULL, VERBOSE_SHORT_OPT }

#define OUTPUTPATH_FLAG outputPathPtr
#define OUTPUTPATH_VAR char *OUTPUTPATH_FLAG = NULL;

#define PATH_LONG_OPT_STRING "outputPath"
#define OUTPUTPATH_LONG_OPT { PATH_LONG_OPT_STRING, required_argument, NULL, 0}

#define OUTPUTFILE_FLAG outputFilePtr
#define OUTPUTFILE_VAR char *OUTPUTFILE_FLAG = NULL;

#define OUTPUTFILE_LONG_OPT_STRING "outputFile"
#define OUTPUTFILE_LONG_OPT { OUTPUTFILE_LONG_OPT_STRING, required_argument, NULL, 0}

#define LICENSE_FLAG showLicense
#define LICENSE_VAR getOptBool LICENSE_FLAG = goFalse;
#define LICENSE_LONG_OPT_STRING "license"
#define LICENSE_LONG_OPT { LICENSE_LONG_OPT_STRING, no_argument, &LICENSE_FLAG, goTrue }

#define ECHO_COMMAND_LINE_FLAG echoCommandLine
#define ECHO_COMMAND_LINE_VAR getOptBool ECHO_COMMAND_LINE_FLAG = goFalse;
#define ECHO_COMMAND_LINE_LONG_OPT_STRING "echoCommandLine"
#define ECHO_COMMAND_LINE_LONG_OPT { ECHO_COMMAND_LINE_LONG_OPT_STRING, no_argument, &ECHO_COMMAND_LINE_FLAG, goTrue }

#define PROGRESS_CHAR progressTest
#define PROGRESS_VAR char *PROGRESS_CHAR = NULL;
#define PROGRESS_SHORT_OPT '%'
#define PROGRESS_LONG_OPT_STRING "progress"
#define PROGRESS_LONG_OPT { PROGRESS_LONG_OPT_STRING, required_argument, NULL, PROGRESS_SHORT_OPT }

//time related flags
#define HOURS_TIME_FLAG timeHours
#define HOURS_TIME_VAR \
uint8_t HOURS_TIME_FLAG = 0;
#define HOURS_TIME_LONG_OPT_STRING "hours"
#define HOURS_TIME_LONG_OPT { HOURS_TIME_LONG_OPT_STRING, required_argument, NULL, 0 }

#define MINUTES_TIME_FLAG timeMinutes
#define MINUTES_TIME_VAR \
uint16_t MINUTES_TIME_FLAG = 0;
#define MINUTES_TIME_LONG_OPT_STRING "minutes"
#define MINUTES_TIME_LONG_OPT { MINUTES_TIME_LONG_OPT_STRING, required_argument, NULL , 0 }

#define SECONDS_TIME_FLAG timeSeconds
#define SECONDS_TIME_VAR \
uint32_t SECONDS_TIME_FLAG = 0;
#define SECONDS_TIME_LONG_OPT_STRING "seconds"
#define SECONDS_TIME_LONG_OPT { SECONDS_TIME_LONG_OPT_STRING, required_argument, NULL, 0 }

#define SHOW_STATUS_BIT_FLAG showStatusBits
#define SHOW_STATUS_BIT_VAR bool SHOW_STATUS_BIT_FLAG = false;
#define SHOW_STATUS_BITS_OPT_STRING "showStatusBits"
#define SHOW_STATUS_BITS_OPT { SHOW_STATUS_BITS_OPT_STRING , no_argument, NULL, goTrue }

#define LONG_OPT_TERMINATOR { NULL, 0, NULL, 0 }

// Scsi Log pages defined for print out
#define SCSI_LOG_PAGES_APPLICATION_CLIENT                       "Application Client"
#define SCSI_LOG_PAGES_BACKGROUND_SCAN                          "Background Scan"
#define SCSI_LOG_PAGES_BACKGROUND_OPERATION                     "Background Operation"
#define SCSI_LOG_PAGES_CACHE_STATISTICS                         "Cache Statistics"
#define SCSI_LOG_PAGES_ENVIRONMENTAL_LIMITS                     "Environmental Limits (Coming soon)"
#define SCSI_LOG_PAGES_ENVIRONMENTAL_REPORTING                  "Environmental Reporting (Coming soon)"
#define SCSI_LOG_PAGES_READ_LOG_PAGE                            "Read Log Page"
#define SCSI_LOG_PAGES_WRITE_LOG_PAGE                           "Write Log Page"
#define SCSI_LOG_PAGES_VERIFY_LOG_PAGE                          "Verify Log Page"
#define SCSI_LOG_PAGES_FACTORY_LOG                              "Factory Log"
#define SCSI_LOG_PAGES_FORMAT_STATUS                            "Format Status"
#define SCSI_LOG_PAGES_INFORMATIONAL_EXCEPTIONS                 "Informational Exceptions"
#define SCSI_LOG_PAGES_LOGICAL_BLOCK_PROVISIONING               "Logical Block Provisioning"
#define SCSI_LOG_PAGES_NON_MEDIUM_ERROR                         "Non-Medium Error" 
#define SCSI_LOG_PAGES_PENDING_DEFECTS                          "Pending Defects"
#define SCSI_LOG_PAGES_POWER_CONDITIONS                         "Power Conditions"
#define SCSI_LOG_PAGES_PROTOCOL_PAGE                            "Protocol Page" 
#define SCSI_LOG_PAGES_DST_LOG_PAGE                             "DST Log Page" 
#define SCSI_LOG_PAGES_SOLID_STATE_MEDIA                        "Solid State Media (Coming soon)"
#define SCSI_LOG_PAGES_START_STOP_CYCLE_PAGE                    "Start Stop Cycle Page"
#define SCSI_LOG_PAGES_SUPPORTED_LOG_PAGES                      "Supported Log Pages"
#define SCSI_LOG_PAGES_SUPPORTED_LOG_PAGES_AND_SUBPAGES         "Supported Log Pages and Subpages"
#define SCSI_LOG_PAGES_TEMPERATURE_LOG_PAGE                     "Temperature Log Page" 
#define SCSI_LOG_PAGES_UTILIZATION                              "Utilization (Coming soon)"

void seachest_utility_Info(const std::string & utilityName, const std::string & buildVersion, const std::string & seaCPublicVersion);
void utility_Full_Version_Info(const std::string & utilityName, const std::string & buildVersion, const std::string & openseaVersion);
void print_Final_newline(void);
void print_SeaChest_Util_Exit_Codes(void);
void print_Seachest_logType_options();
void print_Scsi_Log_Pages();
void print_Verbose_Help(bool shortHelp);
void print_Version_Help(bool shortHelp, const char *utilName);
void print_License_Help(bool shortHelp);
void print_Echo_Command_Line_Help(bool shortHelp);
void print_Help_Help(bool shortHelp);
void print_OutputPath_Help(bool shortHelp);
void print_OutputFile_Help(bool shortHelp);
void print_Input_Log_Help(bool shortHelp);
void print_SeaChest_Util_Exit_Codes(void);
void print_Parser_Output_Log_Help(bool shortHelp);
void print_Log_Type_Help(bool shortHelp);
void print_Log_Print_Help(bool shortHelp);
void print_FARM_Command_Line_Option_to_Show_Status_Bytes();


#define OUTPUTPATH_PARSE outputPathPtr = optarg; 
#define OUTPUTFILE_PARSE outputFilePtr = optarg; 

