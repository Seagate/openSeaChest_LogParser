//
// Do NOT modify or remove this copyright and license
//
// Copyright (c) 2012 - 2024 Seagate Technology LLC and/or its Affiliates, All Rights Reserved
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

enum class eUtilExitCodes
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
	UTIL_EXIT_VALIDATION_FAILURE = 20,
	UTIL_EXIT_STRIP_HDR_FOOTER_FAILURE = 21,
	UTIL_EXIT_PARSE_FAILURE = 22,
};

//Log Parser Input flag
#define INPUT_LOG_FILE_FLAG inputLog
#define INPUT_LOG_FILE_NAME InputLogFile
#define INPUT_LOG_FILE_VAR \
	bool INPUT_LOG_FILE_FLAG = false;\
	bool INPUT_LOG_FROM_PIPE_FLAG = false;\
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

#define INPUT_LOG_FILE_FOUND_FLAG eUtilExitCodes
#define INPUT_LOG_FILE_FOUND_VAR eUtilExitCodes INPUT_LOG_FILE_FOUND_FLAG = eUtilExitCodes::UTIL_EXIT_NO_ERROR;
#define INPUT_LOG_FILE_FOUND_LONG_OPT_STRING "logFound"
#define INPUT_LOG_FILE_FOUND_LONG_OPT { INPUT_LOG_FILE_FOUND_LONG_OPT_STRING, required_argument, NULL, 0}

	// defines for the printing of the logs in the help file
#define LOG_NAME_PRINT_STRING_FARM_LOG                          "farmLog"
#define LOG_NAME_PRINT_STRING_DEVICE_STATISTICS_LOG	            "deviceStatisticsLog"   
#define LOG_NAME_PRINT_STRING_EXT_COMPREHENSIVE_LOG	            "extCompErrorLog"
#define LOG_NAME_PRINT_STRING_COMMON_ERROR_LOG		            "commonErrorLog"
#define LOG_NAME_PRINT_STRING_EXT_SELF_TEST_LOG		            "extSelfTestLog"
#define LOG_NAME_PRINT_STRING_DST								"dstLog"
#define LOG_NAME_PRINT_STRING_IDENTIFY_DEVICE_DATA_LOG	        "identifyDeviceDataLog" 
#define LOG_NAME_PRINT_STRING_IDENTIFY_LOG                      "identify"
#define LOG_NAME_PRINT_STRING_SCT_TEMP_LOG	 	                "sctTempLog"
#define LOG_NAME_PRINT_STRING_POWER_CONDITION_LOG	            "powerConditionLog"
#define LOG_NAME_PRINT_STRING_NCQ_COMMAND_ERROR_LOG	            "ncqErrorLog"
#define LOG_NAME_PRINT_STRING_SCSI_LOG_PAGES		            "scsiLogPages"


#define LOG_TYPE_STRING_FARM_LOG                                "FARMLOG"
#define LOG_TYPE_STRING_FARM						            "FARM"                      // nickname for the farm log
#define LOG_TYPE_STRING_DEVICE_STATISTICS_LOG					"DEVICESTATISTICSLOG"  
#define LOG_TYPE_STRING_DEVICE_STATISTICS						"DEVICESTATISTICS"  
#define LOG_TYPE_STRING_EXT_COMPREHENSIVE_LOG					"EXTCOMPERRORLOG"
#define LOG_TYPE_STRING_EXT_COMPREHENSIVE						"EXTCOMPERROR"
#define LOG_TYPE_STRING_COMMON_ERROR_LOG						"COMMONERRORLOG"
#define LOG_TYPE_STRING_COMMON_ERROR							"COMMONERROR"
#define LOG_TYPE_STRING_EXT_DST_LOG								"EXTSELFTESTLOG"
#define LOG_TYPE_STRING_EXT_DST									"EXTSELFTEST"
#define LOG_TYPE_STRING_DST_LOG									"DSTLOG"
#define LOG_TYPE_STRING_DST										"DST"
#define LOG_TYPE_STRING_IDENTIFY								"IDENTIFY"
#define LOG_TYPE_STRING_IDENTIFY_LOG							"IDENTIFYLOG"
#define LOG_TYPE_STRING_IDENT									"IDENT"
#define LOG_TYPE_STRING_IDENT_LOG								"IDENTLOG"
#define	LOG_TYPE_STRING_IDENTIFY_DEVICE_DATA_LOG				"IDDATALOG"
#define LOG_TYPE_STRING_SCT_TEMP_LOG							"SCTTEMPLOG"
#define LOG_TYPE_STRING_SCT_TEMP								"SCTTEMP"
#define LOG_TYPE_STRING_POWER_CONDITION_LOG						"POWERCONDITIONLOG"
#define LOG_TYPE_STRING_POWER_CONDITION							"POWERCONDITION"
#define LOG_TYPE_STRING_NCQ_COMMAND_ERROR_LOG					"NCQERRORLOG"
#define LOG_TYPE_STRING_NCQ_COMMAND_ERROR						"NCQERROR"
#define LOG_TYPE_STRING_SCSI_LOG_PAGES							"SCSILOGPAGES"
#define LOG_TYPE_STRING_LOG_PAGES								"LOGPAGES"
#define LOG_TYPE_STRING_SAS_LOG_PAGES							"SASLOGPAGES"
#define LOG_TYPE_STRING_SAS										"SAS"
#define LOG_TYPE_STRING_SCSI									"SCSI"


#define INPUT_LOG_TYPE_FLAG eLogTypes
#define INPUT_LOG_TYPE_VAR eLogTypes INPUT_LOG_TYPE_FLAG = eLogTypes::LOG_TYPE_UNKNOWN;
#define INPUT_LOG_TYPE_LONG_OPT_STRING "logType"
#define INPUT_LOG_TYPE_LONG_OPT { INPUT_LOG_TYPE_LONG_OPT_STRING, required_argument, NULL, 0}


// output file types
#define LOG_PRINT_STRING_JSON "json"
#define LOG_PRINT_STRING_TEXT "text"
#define LOG_PRINT_STRING_CSV "csv"
#define LOG_PRINT_STRING_FLATCSV "flatcsv"
#define LOG_PRINT_STRING_PROM "prom"
#define LOG_PRINT_STRING_TELEMETRY "TELEMETRY"

// print type
#define OUTPUT_LOG_PRINT_FLAG ePrintTypes
#define OUTPUT_LOG_PRINT_VAR ePrintTypes OUTPUT_LOG_PRINT_FLAG = ePrintTypes::LOG_PRINT_JSON;
#define OUTPUT_LOG_PRINT_LONG_OPT_STRING "printType"
#define OUTPUT_LOG_PRINT_LONG_OPT { OUTPUT_LOG_PRINT_LONG_OPT_STRING, required_argument, NULL, 0 }


//standard utility options
	
#define SHOW_HELP_FLAG showHelp
#define SHOW_HELP_VAR bool SHOW_HELP_FLAG = false;
#define HELP_SHORT_OPT 'h'
#define HELP_LONG_OPT_STRING "help"
#define HELP_LONG_OPT { HELP_LONG_OPT_STRING, no_argument, NULL, HELP_SHORT_OPT }

#define SHOW_VERSION_FLAG showVersion
#define SHOW_VERSION_VAR bool SHOW_VERSION_FLAG = false;
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

#define OUTPUTLOG_FLAG outputFilePtr
#define OUTPUTLOG_VAR char *OUTPUTLOG_FLAG = NULL;

#define OUTPUTLOG_LONG_OPT_STRING "outputLog"
#define OUTPUTLOG_LONG_OPT { OUTPUTLOG_LONG_OPT_STRING, required_argument, NULL, 0}

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

// show the status bits from the FARM log only
#define SHOW_STATUS_BIT_FLAG showStatusBits
#define SHOW_STATUS_BIT_VAR bool SHOW_STATUS_BIT_FLAG = false;
#define SHOW_STATUS_BITS_OPT_STRING "showStatusBits"
#define SHOW_STATUS_BITS_OPT { SHOW_STATUS_BITS_OPT_STRING , no_argument, NULL, goTrue }

// show full head info in the FARM log only if not populated show NULL 
#define SHOW_ALL_HEAD_DATA_FLAG showAllHeadData
#define SHOW_ALL_HEAD_DATA_VAR getOptBool SHOW_ALL_HEAD_DATA_FLAG = goFalse;
#define SHOW_ALL_HEAD_DATA_OPT_STRING "showAllHeadData"
#define SHOW_ALL_HEAD_DATA_PRINT_STRING "showAllHeadData"
#define SHOW_ALL_HEAD_DATA_OPT { SHOW_ALL_HEAD_DATA_OPT_STRING , no_argument, NULL, goTrue }

// force the parser to be static and print out the same information all the time. aka no arrays and no data created arrays
#define SHOW_STATIC_DATA_FLAG showStatic
#define SHOW_STATIC_DATA_VAR getOptBool SHOW_STATIC_DATA_FLAG = goFalse;
#define SHOW_STATIC_DATA_OPT_STRING "showStatic"
#define SHOW_STATIC_DATA_PRINT_STRING "showStatic"
#define SHOW_STATIC_DATA_OPT { SHOW_STATIC_DATA_OPT_STRING , no_argument, NULL, goTrue }


#define LONG_OPT_TERMINATOR { NULL, 0, NULL, 0 }

// Scsi Log pages defined for print out
#define SCSI_LOG_PAGES_APPLICATION_CLIENT                       "Application Client"
#define SCSI_LOG_PAGES_BACKGROUND_SCAN                          "Background Scan"
#define SCSI_LOG_PAGES_BACKGROUND_OPERATION                     "Background Operation"
#define SCSI_LOG_PAGES_CACHE_STATISTICS                         "Cache Memory Statistics 19h 20h"
#define SCSI_LOG_PAGE_COMMAND_DURATION_LIMITES					"Command Duration Limites Statistics 19h 21h"
#define SCSI_LOG_PAGES_ERROR_COUNTER							"Error Counter"
#define SCSI_LOG_PAGES_ENVIRONMENTAL_LIMITS                     "Environmental Limits"
#define SCSI_LOG_PAGES_ENVIRONMENTAL_REPORTING                  "Environmental Reporting"
#define SCSI_LOG_PAGES_READ_LOG_PAGE                            "Read Log Page"
#define SCSI_LOG_PAGES_WRITE_LOG_PAGE                           "Write Log Page"
#define SCSI_LOG_PAGES_VERIFY_LOG_PAGE                          "Verify Log Page"
#define SCSI_LOG_PAGES_FACTORY_LOG                              "Factory Log"
#define SCSI_LOG_PAGES_FARM										"FARM Log 3Dh 03h"
#define SCSI_LOG_PAGES_FARM_FACTORY								"FARM Factory Log 3Dh 04h"
#define SCSI_LOG_PAGES_FARM_TIME_SERIES							"FARM Time Series Log 3Dh 10h - 1Fh"
#define SCSI_LOG_PAGES_FARM_STICKY								"FARM Sticky Log 3Dh C0h - C7h"
#define SCSI_LOG_PAGES_FORMAT_STATUS                            "Format Status"
#define SCSI_LOG_PAGES_INFORMATIONAL_EXCEPTIONS                 "Informational Exceptions"
#define SCSI_LOG_PAGES_LOGICAL_BLOCK_PROVISIONING               "Logical Block Provisioning"
#define SCSI_LOG_PAGES_NON_MEDIUM_ERROR                         "Non-Medium Error" 
#define SCSI_LOG_PAGES_PENDING_DEFECTS                          "Pending Defects"
#define SCSI_LOG_PAGES_POWER_CONDITIONS                         "Power Conditions"
#define SCSI_LOG_PAGES_PROTOCOL_PAGE                            "Protocol Specific Port" 
#define SCSI_LOG_PAGES_DST_LOG_PAGE                             "Self-Test Results Log" 
#define SCSI_LOG_PAGES_SOLID_STATE_MEDIA                        "Solid State Media"
#define SCSI_LOG_PAGES_START_STOP_CYCLE_PAGE                    "Start Stop Cycle Page"
#define SCSI_LOG_PAGES_SUPPORTED_LOG_PAGES                      "Supported Log Pages"
#define SCSI_LOG_PAGES_SUPPORTED_LOG_PAGES_AND_SUBPAGES         "Supported Log Pages and Subpages"
#define SCSI_LOG_PAGES_TEMPERATURE_LOG_PAGE                     "Temperature Log Page" 
#define SCSI_LOG_PAGES_UTILIZATION                              "Utilization"
#define SCSI_LOG_PAGES_ZONED_DEVICE_STAT						"Zoned Device Statistics"

void seachest_utility_Info(const std::string & utilityName, const std::string & buildVersion, const std::string & seaCPublicVersion);
void Short_Utility_Failure_Usage(const std::string & util_name);
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
void print_FARM_Command_Line_Option_to_Show_All_Head_Data();
void print_FARM_Command_Line_Option_to_Show_Static();


#define OUTPUTPATH_PARSE outputPathPtr = optarg; 
#define OUTPUTFILE_PARSE outputFilePtr = optarg; 

