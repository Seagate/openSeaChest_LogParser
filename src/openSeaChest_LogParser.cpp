//
// openSeaChest_LogParser.cpp
//
// Do NOT modify or remove this copyright and license
//
// Copyright (c) 2024 Seagate Technology LLC and/or its Affiliates, All Rights Reserved
//
// This software is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// ******************************************************************************************
// \file openSeaChest_LogParser.cpp Binary command line that parses various log files for customers 

//////////////////////
//  Included files  //
//////////////////////
#include <stdio.h>
#include <ctype.h>
#include <vector>
#if defined (__unix__) || defined(__APPLE__) //using this definition because linux and unix compilers both define this. Apple does not define this, which is why it has it's own definition
#include <unistd.h>
#include <getopt.h>
#elif defined (_WIN32)
#include "getopt.h"
#else
#error "OS Not Defined or known"
#endif
#include <inttypes.h>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <fstream>

#include "common.h"
#include "EULA.h"
#include "seachest_parser_util_options.h"
#include "parser_print_util_options.h"
#include "Opensea_Parser_Version.h"
#include "Opensea_Parser_Helper.h"
#include "libjson.h"


#include "CFARM_Log.h"
#include "CAta_Device_Stat_Log.h"
#include "CAta_Ext_Comprehensive_Log.h"
#include "CAta_Ext_DST_Log.h"
#include "CAta_Identify_Log.h"
#include "CAta_Power_Conditions_Log.h"
#include "CAta_NCQ_Command_Error_Log.h"
#include "CScsi_Log.h"

using namespace opensea_parser;

////////////////////////
//  Global Variables  //
////////////////////////
#if defined (BUILD_FARM_ONLY)
    std::string util_name = "openSeaChest_LogParser_FARM";
#else
    std::string util_name = "openSeaChest_LogParser";
#endif

std::string buildVersion = "1.5.4";
std::string buildDate = __DATE__;

////////////////////////////
//  functions to declare  //
////////////////////////////
static void utility_Usage(bool shortUsage); 
static void UtilityHeader(JSONNODE *masterData);
inline void Echo_Command_Line(int argc, char* argv[]);
//-----------------------------------------------------------------------------
//
//  main()
//
//! \brief   Description:  main function that runs and decides what to do based on the passed in args
//
//  Entry:
//!   \param argc =
//!   \param argv =
//!
//  Exit:
//!   \return exitCode = error code returned by the application
//
//-----------------------------------------------------------------------------


int32_t main(int argc, char *argv[])
{
    /////////////////
    //  Variables  //
    /////////////////
    //common utility variables
    eReturnValues       retStatus = eReturnValues::IN_PROGRESS;
    eUtilExitCodes      exitCode = eUtilExitCodes::UTIL_EXIT_NO_ERROR;
    LICENSE_VAR
    ECHO_COMMAND_LINE_VAR                           // show or echo the command line back out 
    SHOW_STATUS_BIT_VAR                             // show the status bits from the FARM log only
    SHOW_ALL_HEAD_DATA_VAR                          // show full head info in the FARM log only if not populated show NULL 
    SHOW_STATIC_DATA_VAR                            // force the parser to be static and print out the same information all the time. aka no arrays and no data created arrays
    SHOW_VERSION_VAR
    SHOW_HELP_VAR
    //Tool specific
    INPUT_LOG_FILE_VAR
    OUTPUT_LOG_FILE_VAR
    INPUT_LOG_FILE_FOUND_VAR
    INPUT_LOG_TYPE_VAR
    OUTPUT_LOG_PRINT_VAR
    //OUTPUTPATH_VAR
    //OUTPUTFILE_VAR

    int args = 0;
    uint8_t argIndex = 0;
    int32_t optionIndex = 0;

    //add -- options to this structure DO NOT ADD OPTIONAL ARGUMENTS! Optional arguments are a GNU extension and are not supported in Unix or some compilers- TJE
    struct option longopts[] = {
        //common command line options
        HELP_LONG_OPT,
        VERSION_LONG_OPT,
        VERBOSE_LONG_OPT,
        LICENSE_LONG_OPT,
        ECHO_COMMAND_LINE_LONG_OPT,
        SHOW_STATUS_BITS_OPT,
        //tool specific options go here
        INPUT_LOG_LONG_OPT,
        INPUT_LOG_TYPE_LONG_OPT,
        OUTPUT_LOG_LONG_OPT,
        OUTPUT_LOG_PRINT_LONG_OPT,
        OUTPUTPATH_LONG_OPT,
        OUTPUTLOG_LONG_OPT,
        SHOW_STATUS_BITS_OPT,                               // option for FARM only
        SHOW_ALL_HEAD_DATA_OPT,                             // option for FARM only to show all the head data and Filling with NULL
        SHOW_STATIC_DATA_OPT,                               // option for FARM only to print all data statically 
        LONG_OPT_TERMINATOR
    };

    g_verbosity = eVerbosityLevels::VERBOSITY_DEFAULT;

    atexit(print_Final_newline);

#if defined (BUILD_FARM_ONLY)
            INPUT_LOG_TYPE_FLAG = eLogTypes::LOG_TYPE_FARM;

#endif
    ////////////////////////
    //  Argument Parsing  //
    ////////////////////////
    std::vector<uint8_t> lineInputData;
    std::string lineinput;
#if defined(_DEBUG)
    for (int i = 0; i < argc; ++i)
    {
        std::cout << argv[i] << std::endl;
    }
#endif
    if (argc < 2)
    {
        seachest_utility_Info(util_name, buildVersion, OPENSEA_PARSER_VERSION);
        utility_Usage(true);
        exit(static_cast<int>(eUtilExitCodes::UTIL_EXIT_ERROR_IN_COMMAND_LINE));
    }
    //get options we know we need
    while (1) //changed to while 1 in order to parse multiple options when longs options are involved
    {
        args = getopt_long(argc, argv, "d:hfisF:Vv:q%:", longopts, &optionIndex);
        if (args == -1)
        {
            break;
        }
        //printf("Parsing arg <%u>\n", args);
        switch (args)
        {
        case 0:
            //parse long options that have no short option and required arguments here
            if (strncmp(longopts[optionIndex].name, INPUT_LOG_LONG_OPT_STRING, strlen(longopts[optionIndex].name)) == 0)
            {
                if (strstr(optarg, "fromPipe"))
                {
                    while (std::cin >> lineinput)
                    {
                        lineInputData.push_back(static_cast<uint8_t>(std::strtoul(lineinput.c_str(), NULL, 16)));
                        //std::cout << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << static_cast<uint16_t>(lineInputData.at(lineInputData.size() - 1));
                    }
                    INPUT_LOG_FROM_PIPE_FLAG = true;
                }
                else
                {
                    INPUT_LOG_FILE_FLAG = true;
                    INPUT_LOG_FILE_NAME.resize(sizeof(optarg));
                    INPUT_LOG_FILE_NAME.assign(optarg);
                    std::ifstream myFile(optarg);
                    if (!myFile.good())
                    {
                        INPUT_LOG_FILE_FOUND_FLAG = eUtilExitCodes::UTIL_EXIT_CANNOT_OPEN_FILE;
                    }
                    myFile.close();
                }
            }

#if defined BUILD_FARM_ONLY 
			if (strcmp(optarg, LOG_TYPE_STRING_FARM) == 0)
			{
				INPUT_LOG_TYPE_FLAG = eLogTypes::LOG_TYPE_FARM;
			}
#else
            else if (strncmp(longopts[optionIndex].name, INPUT_LOG_TYPE_LONG_OPT_STRING, strlen(longopts[optionIndex].name)) == 0)
            {
                convert_String_To_Upper_Case(optarg);
#if defined (INCLUDE_FARM_LOG)
                if (strncmp(optarg, LOG_TYPE_STRING_FARM_LOG, strnlen(optarg, sizeof(LOG_TYPE_STRING_FARM_LOG))) == 0 || 
                    strncmp(optarg, LOG_TYPE_STRING_FARM,strnlen(optarg,sizeof(LOG_TYPE_STRING_FARM))) == 0)
                {
                    INPUT_LOG_TYPE_FLAG = eLogTypes::LOG_TYPE_FARM;
                }
#endif
#if defined (INCLUDE_DEVICE_STATISTICS_LOG)
                if (strncmp(optarg, LOG_TYPE_STRING_DEVICE_STATISTICS_LOG, strnlen(optarg, sizeof(LOG_TYPE_STRING_DEVICE_STATISTICS_LOG))) == 0 ||
                    strncmp(optarg, LOG_TYPE_STRING_DEVICE_STATISTICS, strnlen(optarg, sizeof(LOG_TYPE_STRING_DEVICE_STATISTICS))) == 0)
                {
                    INPUT_LOG_TYPE_FLAG = eLogTypes::LOG_TYPE_DEVICE_STATISTICS_LOG;
                }
#endif
#if defined (INCLUDE_EXT_COMPREHENSIVE_LOG)
                if (strncmp(optarg, LOG_TYPE_STRING_EXT_COMPREHENSIVE_LOG, strnlen(optarg, sizeof(LOG_TYPE_STRING_EXT_COMPREHENSIVE_LOG))) == 0 ||
                    strncmp(optarg, LOG_TYPE_STRING_EXT_COMPREHENSIVE,strnlen(optarg,sizeof(LOG_TYPE_STRING_EXT_COMPREHENSIVE))) == 0)
                {
                    INPUT_LOG_TYPE_FLAG = eLogTypes::LOG_TYPE_EXT_COMPREHENSIVE_LOG;
                }
#endif
#if defined (INCLUDE_COMMON_EXT_DST_LOG)
                if (strncmp(optarg, LOG_TYPE_STRING_EXT_DST_LOG, strnlen(optarg, sizeof(LOG_TYPE_STRING_EXT_DST_LOG))) == 0 ||
                    strncmp(optarg, LOG_TYPE_STRING_EXT_DST, strnlen(optarg, sizeof(LOG_TYPE_STRING_EXT_DST))) == 0 ||
                    strncmp(optarg, LOG_TYPE_STRING_DST_LOG, strnlen(optarg, sizeof(LOG_TYPE_STRING_DST_LOG))) == 0 ||
                    strncmp(optarg, LOG_TYPE_STRING_DST, strnlen(optarg, sizeof(LOG_TYPE_STRING_DST))) == 0 )
                {
                    INPUT_LOG_TYPE_FLAG = eLogTypes::LOG_TYPE_EXT_DST_LOG;
                }
#endif

#if defined (INCLUDE_IDENTIFY_LOG)
                if (strncmp(optarg, LOG_TYPE_STRING_IDENTIFY_LOG, strnlen(optarg, sizeof(LOG_TYPE_STRING_IDENTIFY_LOG))) == 0 ||
                    strncmp(optarg, LOG_TYPE_STRING_IDENTIFY, strnlen(optarg, sizeof(LOG_TYPE_STRING_IDENTIFY))) == 0 ||
                    strncmp(optarg, LOG_TYPE_STRING_IDENT, strnlen(optarg, sizeof(LOG_TYPE_STRING_IDENT))) == 0 ||
                    strncmp(optarg, LOG_TYPE_STRING_IDENT_LOG, strnlen(optarg, sizeof(LOG_TYPE_STRING_IDENT_LOG))) == 0 )
                {
                    INPUT_LOG_TYPE_FLAG = eLogTypes::LOG_TYPE_IDENTIFY_LOG;
                }
#endif
#if defined (INCLUDE_IDENTIFY_DEVICE_DATA_LOG)
				if (strncmp(optarg, LOG_TYPE_STRING_IDENTIFY_DEVICE_DATA_LOG, strnlen(optarg, sizeof(LOG_TYPE_STRING_IDENTIFY_DEVICE_DATA_LOG))) == 0)
				{
					INPUT_LOG_TYPE_FLAG = eLogTypes::LOG_TYPE_IDENTIFY_DEVICE_DATA;
				}
#endif
#if defined (INCLUDE_SCT_TEMP_LOG)
                if (strncmp(optarg, LOG_TYPE_STRING_SCT_TEMP_LOG, strnlen(optarg, sizeof(LOG_TYPE_STRING_SCT_TEMP_LOG))) == 0 ||
                    strncmp(optarg, LOG_TYPE_STRING_SCT_TEMP, strnlen(optarg, sizeof(LOG_TYPE_STRING_SCT_TEMP))) == 0 )
                {
                    INPUT_LOG_TYPE_FLAG = eLogTypes::LOG_TYPE_SCT_TEMP_LOG;
                }
#endif

#if defined (INCLUDE_POWER_CONDITION_LOG)
				if (strncmp(optarg, LOG_TYPE_STRING_POWER_CONDITION_LOG, strnlen(optarg, sizeof(LOG_TYPE_STRING_POWER_CONDITION_LOG))) == 0 ||
                    strncmp(optarg, LOG_TYPE_STRING_POWER_CONDITION, strnlen(optarg, sizeof(LOG_TYPE_STRING_POWER_CONDITION))) == 0 )
				{
					INPUT_LOG_TYPE_FLAG = eLogTypes::LOG_TYPE_POWER_CONDITION_LOG;
				}
#endif
#if defined (INCLUDE_NCQ_CMD_ERROR_LOG)
                if (strncmp(optarg, LOG_TYPE_STRING_NCQ_COMMAND_ERROR_LOG, strnlen(optarg, sizeof(LOG_TYPE_STRING_NCQ_COMMAND_ERROR_LOG))) == 0 ||
                    strncmp(optarg, LOG_TYPE_STRING_NCQ_COMMAND_ERROR, strnlen(optarg, sizeof(LOG_TYPE_STRING_NCQ_COMMAND_ERROR))) == 0 )
                {
                    INPUT_LOG_TYPE_FLAG = eLogTypes::LOG_TYPE_NCQ_CMD_ERROR_LOG;
                }
#endif
#if defined (INCLUDE_SCSI_LOG_PAGES)
				if (strncmp(optarg, LOG_TYPE_STRING_SCSI_LOG_PAGES, strnlen(optarg, sizeof(LOG_TYPE_STRING_SCSI_LOG_PAGES))) == 0 ||
                    strncmp(optarg, LOG_TYPE_STRING_LOG_PAGES, strnlen(optarg, sizeof(LOG_TYPE_STRING_LOG_PAGES))) == 0 ||
                    strncmp(optarg, LOG_TYPE_STRING_SAS_LOG_PAGES, strnlen(optarg, sizeof(LOG_TYPE_STRING_SAS_LOG_PAGES))) == 0 ||
                    strncmp(optarg, LOG_TYPE_STRING_SAS, strnlen(optarg, sizeof(LOG_TYPE_STRING_SAS))) == 0 ||
                    strncmp(optarg, LOG_TYPE_STRING_SCSI, strnlen(optarg, sizeof(LOG_TYPE_STRING_SCSI))) == 0 )
				{
					INPUT_LOG_TYPE_FLAG = eLogTypes::LOG_TYPE_SCSI_LOG_PAGES;
				}
#endif

            }
#endif
            else if (strcmp(longopts[optionIndex].name, PATH_LONG_OPT_STRING) == 0)
            {
                //TODO: set up vars for output path
            }
            else if (strcmp(longopts[optionIndex].name, OUTPUT_LOG_LONG_OPT_STRING) == 0)
            {
                OUTPUT_LOG_FILE_FLAG = true;
                OUTPUT_LOG_FILE_NAME.resize(sizeof(optarg));
                OUTPUT_LOG_FILE_NAME.assign(optarg);

            }
            else if (strcmp(longopts[optionIndex].name, OUTPUT_LOG_PRINT_LONG_OPT_STRING) == 0)
            {
                if (strcmp(optarg, LOG_PRINT_STRING_JSON) == 0)
                {
                    OUTPUT_LOG_PRINT_FLAG = ePrintTypes::LOG_PRINT_JSON;
                }
                else if (strcmp(optarg, LOG_PRINT_STRING_TEXT) == 0)
                {
                    OUTPUT_LOG_PRINT_FLAG = ePrintTypes::LOG_PRINT_TEXT;
                }
                else if (strcmp(optarg, LOG_PRINT_STRING_CSV) == 0)
                {
                    OUTPUT_LOG_PRINT_FLAG = ePrintTypes::LOG_PRINT_CSV;
                }
                else if (strcmp(optarg, LOG_PRINT_STRING_FLATCSV) == 0)
                {
                    OUTPUT_LOG_PRINT_FLAG = ePrintTypes::LOG_PRINT_FLAT_CSV;
                }
                else if (strcmp(optarg, LOG_PRINT_STRING_PROM) == 0)
                {
                    OUTPUT_LOG_PRINT_FLAG = ePrintTypes::LOG_PRINT_PROM;
                }
                else
                {
                    OUTPUT_LOG_PRINT_FLAG = ePrintTypes::LOG_PRINT_JSON;
                }
            }
            else
            {
                printf("Ignoring option --%s\n",longopts[optionIndex].name);
            }
            break;
        case 1:
            if (strncmp(longopts[optionIndex].name, SHOW_STATUS_BITS_OPT_STRING, strlen(longopts[optionIndex].name)) == 0)
            {
                SHOW_STATUS_BIT_FLAG = true;
            }
            if (strncmp(longopts[optionIndex].name, SHOW_ALL_HEAD_DATA_OPT_STRING, strlen(longopts[optionIndex].name)) == 0)
            {
                SHOW_ALL_HEAD_DATA_FLAG = true;
                g_parseNULL = true;
            }
            // setting the flag for static print information for the FARM
            if (strncmp(longopts[optionIndex].name, SHOW_STATIC_DATA_OPT_STRING, strlen(longopts[optionIndex].name)) == 0)
            {
                SHOW_STATIC_DATA_FLAG = true;
            }
            break;
        case ':'://missing required argument
            exitCode = eUtilExitCodes::UTIL_EXIT_ERROR_IN_COMMAND_LINE;
            switch (optopt)
            {
            case 0:
                //check long options for missing arguments
                break;
            case VERBOSE_SHORT_OPT:
                if (eVerbosityLevels::VERBOSITY_QUIET < g_verbosity)
                {
                    printf("You must specify a verbosity level. 0 - 4 are the valid levels\n");
                }
                break;
            default:
                if (eVerbosityLevels::VERBOSITY_QUIET < g_verbosity)
                {
                    printf("Option %c requires an argument\n", optopt);
                }
                utility_Usage(true);
                return static_cast<int>(exitCode);
            }
            break;
        case VERSION_SHORT_OPT:
            SHOW_VERSION_FLAG = true;
            break;
        case VERBOSE_SHORT_OPT: //verbose
            if (optarg != NULL)
            {
                g_verbosity = static_cast<eVerbosityLevels>(atoi(optarg));
            }
            break;
        case '?': //unknown option
        case 'h': //help
            SHOW_HELP_FLAG = true;
            seachest_utility_Info( util_name,  buildVersion,  OPENSEA_PARSER_VERSION);//TODO: We should change the version to a SeaParser version!
            utility_Usage(false);
            return static_cast<int>(eUtilExitCodes::UTIL_EXIT_NO_ERROR);
        default:
            break;
        }
    }

    if (ECHO_COMMAND_LINE_FLAG)
    {
        //Echo the command line back to the customer
        Echo_Command_Line(argc, argv);
        printf("\n");
    }

    if (eVerbosityLevels::VERBOSITY_QUIET < g_verbosity)
    {
        seachest_utility_Info( util_name,  buildVersion,  OPENSEA_PARSER_VERSION);//TODO: We should change the version to a SeaParser version!
    }

    if (SHOW_VERSION_FLAG)
    {
        utility_Full_Version_Info(util_name, buildVersion, OPENSEA_PARSER_VERSION); //TODO: We should change the version to a SeaParser version!
    }

    if (LICENSE_FLAG)
    {
        print_EULA_To_Screen(false, false);
    }

    // Add to this if list anything that is suppose to be independent.
    // e.g. you can't say enumerate & then pull logs in the same command line.
    // SIMPLE IS BEAUTIFUL
    if (SHOW_VERSION_FLAG || LICENSE_FLAG || SHOW_HELP_FLAG)
    {
        return static_cast<int>(eUtilExitCodes::UTIL_EXIT_NO_ERROR);
    }
#if defined BUILD_FARM_ONLY 
#else
	if (INPUT_LOG_TYPE_FLAG == eLogTypes::LOG_TYPE_UNKNOWN && INPUT_LOG_FILE_FLAG
        || INPUT_LOG_FILE_FOUND_FLAG != eUtilExitCodes::UTIL_EXIT_NO_ERROR)
	{
        std::cout << std::endl;
        std::cout << "\t ******   Missing or unable to find the Input Log ****** " << std::endl;
        std::cout << "\t" << INPUT_LOG_FILE_NAME << std::endl << std::endl;
        //Echo the command line back to the customer
        Echo_Command_Line(argc, argv);
        std::cout << std::endl;
        Short_Utility_Failure_Usage(util_name);
        exitCode = eUtilExitCodes::UTIL_EXIT_ERROR_IN_COMMAND_LINE;
        return static_cast<int>(exitCode);
	}
    if (!OUTPUT_LOG_FILE_FLAG || !INPUT_LOG_FILE_FLAG)
    {
        std::string outputPrefix = INPUT_LOG_FILE_NAME.substr(0, INPUT_LOG_FILE_NAME.rfind('.'));
        
            switch (OUTPUT_LOG_PRINT_FLAG)
            {
            case ePrintTypes::LOG_PRINT_JSON:
                outputPrefix.append(".json");
                break;
            case ePrintTypes::LOG_PRINT_TEXT:
                outputPrefix.append(".txt");
                break;
            case ePrintTypes::LOG_PRINT_CSV:
            case ePrintTypes::LOG_PRINT_FLAT_CSV:
                outputPrefix.append(".csv");
                break;
            case ePrintTypes::LOG_PRINT_PROM:
                outputPrefix.append(".prm");
                break;
            default:
                outputPrefix.append(".json");
                break;
            }

        OUTPUT_LOG_FILE_NAME.resize(sizeof(outputPrefix));
        OUTPUT_LOG_FILE_NAME.assign(outputPrefix);
    }
#endif
    //print out errors for unknown arguments for remaining args that haven't been processed yet
    for (argIndex = static_cast<uint8_t>(optind); argIndex < argc; argIndex++)
    {
        if (eVerbosityLevels::VERBOSITY_QUIET < g_verbosity)
        {
            printf("Invalid argument: %s\n", argv[argIndex]);
        }
    }
 

    if (INPUT_LOG_FILE_FLAG)
    {
        JSONNODE *masterJson = json_new(JSON_NODE);
		UtilityHeader(masterJson);
        switch (INPUT_LOG_TYPE_FLAG) 
        {
        case eLogTypes::LOG_TYPE_FARM:
            {
            CFARMLog* CFarm;
                if (INPUT_LOG_FROM_PIPE_FLAG)
                {
                    uint8_t* plineInputData;
                    plineInputData = lineInputData.data();
                    CFarm = new CFARMLog(plineInputData, lineInputData.size(), SHOW_STATUS_BIT_FLAG, SHOW_STATIC_DATA_FLAG);
                    
                }
                else
                {
                    CFarm = new CFARMLog(INPUT_LOG_FILE_NAME, SHOW_STATUS_BIT_FLAG, SHOW_STATIC_DATA_FLAG);
                }
                retStatus = CFarm->get_FARM_Status();								// check to make sure we read in the file form the construtor.
                if (retStatus == eReturnValues::IN_PROGRESS)										// if eReturnValues::IN_PROGRESS we can continue to parse, else retStatus holds the error information
                {
                    retStatus = CFarm->parse_Device_Farm_Log(masterJson);
                }
                delete(CFarm);
            }
            break;
        case   eLogTypes::LOG_TYPE_DEVICE_STATISTICS_LOG:
            {
                CAtaDeviceStatisticsLogs *cDevicStat;
                cDevicStat = new CAtaDeviceStatisticsLogs(INPUT_LOG_FILE_NAME, masterJson);
                retStatus = cDevicStat->get_Device_Stat_Status();					// All checks and parseing are done in the construtor
                delete(cDevicStat);
            }
            break;
        case    eLogTypes::LOG_TYPE_EXT_COMPREHENSIVE_LOG:
            {
                CExtComp *cEC;
                cEC = new CExtComp(INPUT_LOG_FILE_NAME, masterJson);
                retStatus = cEC->get_EC_Status();									// All checks and parseing are done in the construtor
                delete(cEC);
            }
            break;
        case    eLogTypes::LOG_TYPE_EXT_DST_LOG:
            {
                CAta_Ext_DST_Log *cDST;
                cDST = new CAta_Ext_DST_Log(INPUT_LOG_FILE_NAME, masterJson);
                retStatus = cDST->get_Status();
                delete(cDST);
            }
            break;
        case eLogTypes::LOG_TYPE_IDENTIFY_LOG:
            {
                CAta_Identify_log * cIdent;
                cIdent = new CAta_Identify_log(INPUT_LOG_FILE_NAME);			// constructor will make sure we read in the file and start the parseing of the binary
				retStatus = cIdent->get_Identify_Information_Status();			// if eReturnValues::eReturnValues::IN_PROGRESS we can continue to print out the data
				if (retStatus == eReturnValues::IN_PROGRESS)
				{
					retStatus = cIdent->print_Identify_Information(masterJson);
				}
                delete (cIdent);
            }
			break;
        case eLogTypes::LOG_TYPE_IDENTIFY_DEVICE_DATA:
            {
                CAta_Identify_Log_30 *cIdData = NULL;
                cIdData = new CAta_Identify_Log_30( INPUT_LOG_FILE_NAME);		// constructor will make sure we read in the file and start the parseing of the binary
				retStatus = cIdData->get_identify_Status();						// if eReturnValues::eReturnValues::IN_PROGRESS we can continue to print out the data
				if (retStatus == eReturnValues::IN_PROGRESS)
				{
					retStatus = cIdData->parse_Identify_Log_30(masterJson);
				}
                delete (cIdData);
            }
            break;
        case eLogTypes::LOG_TYPE_SCT_TEMP_LOG:
            {
                CSAtaDevicStatisticsTempLogs *cSCTTemp;
                cSCTTemp = new CSAtaDevicStatisticsTempLogs(INPUT_LOG_FILE_NAME, masterJson);	// constructor will make sure we read in the file and start the parseing of the binary
				retStatus = cSCTTemp->get_Status();										// if eReturnValues::IN_PROGRESS we can continue to print out the data
				if (retStatus == eReturnValues::IN_PROGRESS)
				{
					retStatus = cSCTTemp->print_SCT_Temp_Log();
				}
                delete (cSCTTemp);
            }
            break;
        case eLogTypes::LOG_TYPE_POWER_CONDITION_LOG:
            {
                CAtaPowerConditionsLog * cPowerCon;
                cPowerCon = new CAtaPowerConditionsLog(INPUT_LOG_FILE_NAME);			// constructor will make sure we read in the file and start the parseing of the binary
				retStatus = cPowerCon->get_Power_Status();								// if eReturnValues::IN_PROGRESS we can continue to print out the data
				if (retStatus == eReturnValues::IN_PROGRESS)
				{
					retStatus = cPowerCon->printPowerConditionLog(masterJson);
				}
				delete (cPowerCon);
            }
			break;
        case eLogTypes::LOG_TYPE_NCQ_CMD_ERROR_LOG:
            {
                CAta_NCQ_Command_Error_Log * cNCQ;
                cNCQ = new CAta_NCQ_Command_Error_Log(INPUT_LOG_FILE_NAME);				// constructor will make sure we read in the file and start the parseing of the binary
				retStatus = cNCQ->get_NCQ_Command_Error_Log_Status();					// if eReturnValues::IN_PROGRESS we can continue to print out the data
				if (retStatus == eReturnValues::IN_PROGRESS)
				{
					retStatus = cNCQ->get_NCQ_Command_Error_Log(masterJson);
				}
				delete (cNCQ);
            }
			break;
		case eLogTypes::LOG_TYPE_SCSI_LOG_PAGES:
			{
				CScsiLog * cLogPages;
				cLogPages = new CScsiLog(INPUT_LOG_FILE_NAME, masterJson);				// All checks and parseing are done in the construtor
				retStatus = cLogPages->get_Log_Status();
				delete (cLogPages);
			}
			break;
        default:
            {
                // set to unknown to pass through the case statement below no type set was given or didn't match
                exitCode = eUtilExitCodes::UTIL_EXIT_ERROR_IN_COMMAND_LINE;
                retStatus = eReturnValues::UNKNOWN;
            }
            break;
        }
		// todo really check the status vs error code
		switch (retStatus)
		{
		case eReturnValues::SUCCESS:
            {
                exitCode = eUtilExitCodes::UTIL_EXIT_NO_ERROR;
                if (OUTPUT_LOG_FILE_FLAG)
                {
                    printf("\nParsing completed with no issues \n");
                }
            }
		    break;
        case eReturnValues::NOT_SUPPORTED:
            {
                exitCode = eUtilExitCodes::UTIL_EXIT_OPERATION_NOT_SUPPORTED;
                if (OUTPUT_LOG_FILE_FLAG)
                {
                    printf("\nLog Not supported at this time \n");
                    json_push_back(masterJson, json_new_a("Not Supported", "Log Not Supported"));
                }
            }
            break;
		case eReturnValues::IN_PROGRESS:
            {
                exitCode = eUtilExitCodes::UTIL_EXIT_OPERATION_STILL_IN_PROGRESS;
                if (OUTPUT_LOG_FILE_FLAG)
                {
                    printf("\nParsing incomplete Operation was still in progress \n");
                    json_push_back(masterJson, json_new_a("Parsing Error", "Incomplete Operation was still in progress"));
                }
            }
			break;
		case eReturnValues::FAILURE:
            {
                exitCode = eUtilExitCodes::UTIL_EXIT_OPERATION_FAILURE;
                if (OUTPUT_LOG_FILE_FLAG)
                {
                    printf("\nOperation Failure \n");
                    json_push_back(masterJson, json_new_a("Parsing Error", "Operation Failure"));
                }
            }
			break;
		case eReturnValues::ABORTED:
            {
                exitCode = eUtilExitCodes::UTIL_EXIT_OPERATION_ABORTED;
                if (OUTPUT_LOG_FILE_FLAG)
                {
                    printf("\nAborted \n");
                    json_push_back(masterJson, json_new_a("Parsing Error", "Aborted"));
                }
            }
			break;
		case eReturnValues::BAD_PARAMETER:
            {
                exitCode = eUtilExitCodes::UTIL_EXIT_OPERATION_BAD_PARAMETER;
                if (OUTPUT_LOG_FILE_FLAG)
                {
                    printf("\nBad Parameter \n");
                    printf("Check --logType\n");
                    json_push_back(masterJson, json_new_a("Parsing Error", "Bad Parameter"));
                }
            }
			break;
		case eReturnValues::MEMORY_FAILURE:
            {
                exitCode = eUtilExitCodes::UTIL_EXIT_OPERATION_MEMORY_FAILURE;
                if (OUTPUT_LOG_FILE_FLAG)
                {
                    printf("\nMemory Failure \n");
                    json_push_back(masterJson, json_new_a("Parsing Error", "Memory Failure"));
                }
            }
			break;
		case eReturnValues::FILE_OPEN_ERROR:
            {
                exitCode = eUtilExitCodes::UTIL_EXIT_CANNOT_OPEN_FILE;
                if (OUTPUT_LOG_FILE_FLAG)
                {
                    printf("\nCould not Open File \n");
                    json_push_back(masterJson, json_new_a("Parsing Error", "Could not Open File"));
                }
            }
			break;
        case eReturnValues::VALIDATION_FAILURE:
            {
                exitCode = eUtilExitCodes::UTIL_EXIT_VALIDATION_FAILURE;
                if (OUTPUT_LOG_FILE_FLAG)
                {
                    printf("\nBinary File has Failed Validation \n");
                    json_push_back(masterJson, json_new_a("Parsing Error", "Validation Failure"));
                }
            }
            break;
		case eReturnValues::INVALID_LENGTH:
            {
                exitCode = eUtilExitCodes::UTIL_EXIT_OPERATION_INVALID_LENGTH;
                if (OUTPUT_LOG_FILE_FLAG)
                {
                    printf("\nBinary File With Invalid Length \n");
                    json_push_back(masterJson, json_new_a("Parsing Error", "Invalid Length"));
                }
            }
			break;
		default:
            {
                printf("\n %d failure", retStatus);
            }
			break;
		}
        if (OUTPUT_LOG_FILE_FLAG)
        {
			CMessage *printMessage;
            printMessage = new CMessage(masterJson, OUTPUT_LOG_FILE_NAME, OUTPUT_LOG_PRINT_FLAG);
			delete(printMessage);

        }
        else //print it to stdout. 
        {
			std::string myFile = INPUT_LOG_FILE_NAME;				// myFile for the auto creation of the output file
			myFile = myFile.substr(0, myFile.rfind("."));           // remove the extension from the file
            CMessage *printMessage;
            if (OUTPUT_LOG_PRINT_FLAG == ePrintTypes::LOG_PRINT_JSON)         // Append output extension, .json by default
            {
                myFile.append(".json");
                printMessage = new CMessage(masterJson, myFile, OUTPUT_LOG_PRINT_FLAG); // Get JSON output
                std::cout << printMessage->get_Msg_JSON_Data().c_str();	// Print to the screen
            }
            else if (OUTPUT_LOG_PRINT_FLAG == ePrintTypes::LOG_PRINT_TEXT)
            {
                myFile.append(".txt");
                printMessage = new CMessage(masterJson, myFile, OUTPUT_LOG_PRINT_FLAG); // Get text output
                printMessage->parse_Json_to_Text(masterJson, 0);
                //std::cout << printMessage->get_Msg_Text_Format().c_str();	// Print to the screen
            }
            else if (OUTPUT_LOG_PRINT_FLAG == ePrintTypes::LOG_PRINT_CSV)
            {
                myFile.append(".csv");
                printMessage = new CMessage(masterJson, myFile, OUTPUT_LOG_PRINT_FLAG); // Get CSV output
                std::cout << printMessage->get_Msg_CSV(masterJson).c_str();	// Print to the screen
            }
            else if (OUTPUT_LOG_PRINT_FLAG == ePrintTypes::LOG_PRINT_FLAT_CSV)
            {
                myFile.append(".csv");
                printMessage = new CMessage(masterJson, myFile, OUTPUT_LOG_PRINT_FLAG); // Get flat CSV output
                std::cout << printMessage->get_Msg_Flat_csv(masterJson).c_str();	// Print to the screen
            }
            else if (OUTPUT_LOG_PRINT_FLAG == ePrintTypes::LOG_PRINT_PROM)
            {
                myFile.append(".prom");
                printMessage = new CMessage(masterJson, myFile, OUTPUT_LOG_PRINT_FLAG); // Get Prometheus output
                printMessage->setSerialNumber(masterJson);
                printMessage->parseJSONToProm(masterJson, printMessage->getSerialNumber(), NULL);
                std::cout << printMessage->printProm().c_str();	// Print to the screen
            }
            else
            {
                myFile.append(".json");	
                printMessage = new CMessage(masterJson, myFile, OUTPUT_LOG_PRINT_FLAG); // Get JSON output by default
                std::cout << printMessage->get_Msg_JSON_Data().c_str();	// Print to the screen
            }
            delete(printMessage);
        }
        json_delete(masterJson);
    }
    else
    {
        //Error, no log given to parse
        exitCode = eUtilExitCodes::UTIL_EXIT_ERROR_IN_COMMAND_LINE;
    }
    return static_cast<int>(exitCode);
}

//-----------------------------------------------------------------------------
//
//  Utility_usage()
//
//! \brief   Description:  Dump the utility usage information
//
//  Entry:
//!   \param NONE
//!
//  Exit:
//!   \return VOID
//
//-----------------------------------------------------------------------------
void utility_Usage(bool shortUsage)
{
    //everything needs a help option right?
    printf("Usage\n");
    printf("=====\n");
    printf("\t %s {arguments} {options}\n\n", util_name.c_str());


    printf("Examples\n");
    printf("========\n");
    //example usage
#if defined BUILD_FARM_ONLY 
    printf("\t%s --inputLog <filename> --printType json --outputLog <filename>\n", util_name.c_str());
#else
    printf("\t%s --inputLog <filename> --logType %s --printType json --outputLog <filename>\n", util_name.c_str(), LOG_TYPE_STRING_FARM);
#endif
	printf("\n");
    //return codes
    printf("Return Codes\n");
    printf("============\n");
    print_SeaChest_Util_Exit_Codes();

    //the test options
    printf("Utility Arguments\n");
    printf("=================\n");
    //Common (across utilities) - alphabetized
    print_Input_Log_Help(shortUsage);
#if defined BUILD_FARM_ONLY 
#else
    print_Log_Type_Help(shortUsage);
#endif
    print_FARM_Command_Line_Option_to_Show_Status_Bytes();
    print_FARM_Command_Line_Option_to_Show_All_Head_Data();
    print_FARM_Command_Line_Option_to_Show_Static();
    print_Parser_Output_Log_Help(shortUsage);
    print_Log_Print_Help(shortUsage);
    
    //utility options - alphabetized
    printf("Utility Options\n");
    printf("===============\n");
    print_Echo_Command_Line_Help(shortUsage);
    print_Help_Help(shortUsage);
    print_License_Help(shortUsage);
    print_Verbose_Help(shortUsage);
    print_Version_Help(shortUsage, util_name.c_str());
}
//-----------------------------------------------------------------------------
//
//! \fn UtilityHeader()
//
//! \brief
//!   Description:  gets the current time and date and build information and adds it to the \n
//!    json node to print out utility information
//
//  Entry:
//! \param masterData - pointer to the json data that will be printed or passed on
//
//  Exit:
//!   \return eReturnValues::SUCCESS
//
//---------------------------------------------------------------------------
static void UtilityHeader(JSONNODE *masterData)
{
	// get current Time and Date 
    char timeCString[64];
    const char * constTimeCString = &timeCString[0];
	time_t pCurrentTime = time(NULL);
    struct tm localTimeBuffer;
    memset(&localTimeBuffer, 0, sizeof(struct tm));
	strftime(timeCString, 64, "%m-%d-%Y__%H:%M:%S", get_Localtime(&pCurrentTime, &localTimeBuffer));
	JSONNODE *toolHeader = json_new(JSON_NODE);
	json_set_name(toolHeader, util_name.c_str());
	json_push_back(toolHeader, json_new_a("Utility Build Version", buildVersion.c_str()));
    json_push_back(toolHeader, json_new_a("Library Build Version", OPENSEA_PARSER_VERSION));
	json_push_back(toolHeader, json_new_a("Build Date", buildDate.c_str()));
	json_push_back(toolHeader, json_new_a("Run as Date", constTimeCString));
	json_push_back(masterData, toolHeader);
}
//-----------------------------------------------------------------------------
//
//! \fn Echo_Command_Line()
//
//! \brief
//!   Description:  parses out the inpult file name for the Telemetry data. 
//! this will create the formated file name for the TRD file produced for Telemetry from the FARM log
//
//  Entry:
//
//  Exit:
//!   \return eReturnValues::SUCCESS
//
//---------------------------------------------------------------------------
inline void Echo_Command_Line(int argc, char *argv[])
{

    for (int commandLineIter = 1; commandLineIter < static_cast<uint64_t>(argc); commandLineIter++)
    {
        if (strncmp(argv[commandLineIter], "--echoCommandLine", strlen(argv[commandLineIter])) == 0)
        {
            continue;
        }
        printf("%s ", argv[commandLineIter]);
    }
    printf("\n");
}