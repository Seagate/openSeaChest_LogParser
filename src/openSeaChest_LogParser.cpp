//
// openSeaChest_LogParser.cpp
//
// Do NOT modify or remove this copyright and license
//
// Copyright (c) 2018 Seagate Technology LLC and/or its Affiliates, All Rights Reserved
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
#if defined (__unix__) || defined(__APPLE__) //using this definition because linux and unix compilers both define this. Apple does not define this, which is why it has it's own definition
#include <unistd.h>
#include <getopt.h>
#elif defined (_WIN32)
#include "getopt.h"
#else
#error "OS Not Defined or known"
#endif
#include "common.h"
#include "EULA.h"
#include "seachest_parser_util_options.h"
#include "parser_print_util_options.h"
#include "Opensea_Parser_Version.h"
#include "Opensea_Parser_Helper.h"
#include "libjson.h"
#include <inttypes.h>
#include <string>

#include "CFARM_Log.h"
#include "CAta_Device_Stat_Log.h"
#include "CAta_Ext_Comprehensive_Log.h"
#include "CAta_Ext_DST_Log.h"
#include "CAta_Identify_Log.h"
#include "CAta_Power_Conditions_Log.h"
#include "CAta_NCQ_Command_Error_Log.h"
#include  "CScsi_Log.h"

using namespace opensea_parser;

////////////////////////
//  Global Variables  //
////////////////////////
#if defined (BUILD_FARM_ONLY)
    std::string util_name = "openSeaChest_LogParser_FARM";
#else
    std::string util_name = "openSeaChest_LogParser";
#endif

std::string buildVersion = "0.1.2";
std::string buildDate = __DATE__;
time_t     pCurrentTime;
std::string timeString = "";

////////////////////////////
//  functions to declare  //
////////////////////////////
static void utility_Usage(bool shortUsage); 

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
    eReturnValues       retStatus = IN_PROGRESS;
    eUtilExitCodes      exitCode = UTIL_EXIT_NO_ERROR;
    LICENSE_VAR
    ECHO_COMMAND_LINE_VAR
    SHOW_BANNER_VAR
    SHOW_HELP_VAR
    //Tool specific
    INPUT_LOG_FILE_VAR
    OUTPUT_LOG_FILE_VAR
    INPUT_LOG_TYPE_VAR
    OUTPUT_LOG_PRINT_VAR
    //OUTPUTPATH_VAR
    //OUTPUTFILE_VAR

    int8_t args = 0;
    uint8_t argIndex = 0;
    int32_t optionIndex = 0;

    //add -- options to this structure DO NOT ADD OPTIONAL ARGUMENTS! Optional arguments are a GNU extension and are not supported in Unix or some compilers- TJE
    struct option longopts[] = {
        //common command line options
        HELP_LONG_OPT,
        VERSION_LONG_OPT,
        VERBOSE_LONG_OPT,
        LICENSE_LONG_OPT,
        ECHO_COMMAND_LIN_LONG_OPT,
        //tool specific options go here
        INPUT_LOG_LONG_OPT,
        INPUT_LOG_TYPE_LONG_OPT,
        OUTPUT_LOG_LONG_OPT,
        OUTPUT_LOG_PRINT_LONG_OPT,
        OUTPUTPATH_LONG_OPT,
        OUTPUTFILE_LONG_OPT,
        LONG_OPT_TERMINATOR
    };

    g_verbosity = VERBOSITY_DEFAULT;

    atexit(print_Final_newline);

#if defined (BUILD_FARM_ONLY)
            INPUT_LOG_TYPE_FLAG = SEAGATE_LOG_TYPE_FARM;

#endif
    ////////////////////////
    //  Argument Parsing  //
    ////////////////////////
    if (argc < 2)
    {
		seachest_utility_Info(util_name, buildVersion, OPENSEA_PARSER_VERSION);
        utility_Usage(true);
        exit(UTIL_EXIT_ERROR_IN_COMMAND_LINE);
    }
    //get options we know we need
    while (1) //changed to while 1 in order to parse multiple options when longs options are involved
    {
        args =(int8_t) getopt_long(argc, argv, "d:hisF:Vv:q%:", longopts, &optionIndex);
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
                INPUT_LOG_FILE_FLAG = true;
                INPUT_LOG_FILE_NAME.resize(sizeof(optarg));
                INPUT_LOG_FILE_NAME.assign(optarg);
            }

#if defined BUILD_FARM_ONLY 
			if (strcmp(optarg, LOG_TYPE_STRING_FARM) == 0)
			{
				INPUT_LOG_TYPE_FLAG = SEAGATE_LOG_TYPE_FARM;
			}
#else
            else if (strncmp(longopts[optionIndex].name, INPUT_LOG_TYPE_LONG_OPT_STRING, strlen(longopts[optionIndex].name)) == 0)
            {
#if defined (INCLUDE_FARM_LOG)
                if (strcmp(optarg, LOG_TYPE_STRING_FARM) == 0)
                {
                    INPUT_LOG_TYPE_FLAG = SEAGATE_LOG_TYPE_FARM;
                }
#endif
#if defined (INCLUDE_DEVICE_STATISTICS_LOG)
                if (strcmp(optarg, LOG_TYPE_STRING_DEVICE_STATISTICS_LOG) == 0)
                {
                    INPUT_LOG_TYPE_FLAG = SEAGATE_LOG_TYPE_DEVICE_STATISTICS_LOG;
                }
#endif
#if defined (INCLUDE_EXT_COMPREHENSIVE_LOG)
                if (strcmp(optarg, LOG_TYPE_STRING_EXT_COMPREHENSIVE_LOG) == 0)
                {
                    INPUT_LOG_TYPE_FLAG = SEAGATE_LOG_TYPE_EXT_COMPREHENSIVE_LOG;
                }
#endif
#if defined (INCLUDE_COMMON_EXT_DST_LOG)
                if (strcmp(optarg, LOG_TYPE_STRING_EXT_DST_LOG) == 0)
                {
                    INPUT_LOG_TYPE_FLAG = SEAGATE_LOG_TYPE_EXT_DST_LOG;
                }
#endif

#if defined (INCLUDE_IDENTIFY_LOG)
                if (strcmp(optarg, LOG_TYPE_STRING_IDENTIFY_LOG) == 0)
                {
                    INPUT_LOG_TYPE_FLAG = SEAGATE_LOG_TYPE_IDENTIFY_LOG;
                }
#endif
#if defined (INCLUDE_IDENTIFY_DEVICE_DATA_LOG)
				if (strcmp(optarg, LOG_TYPE_STRING_IDENTIFY_DEVICE_DATA_LOG) == 0)
				{
					INPUT_LOG_TYPE_FLAG = SEAGATE_LOG_TYPE_IDENTIFY_DEVICE_DATA;
				}
#endif
#if defined (INCLUDE_SCT_TEMP_LOG)
                if (strcmp(optarg, LOG_TYPE_STRING_SCT_TEMP_LOG) == 0)
                {
                    INPUT_LOG_TYPE_FLAG = SEAGATE_LOG_TYPE_SCT_TEMP_LOG;
                }
#endif

#if defined (INCLUDE_POWER_CONDITION_LOG)
				if (strcmp(optarg, LOG_TYPE_STRING_POWER_CONDITION_LOG) == 0)
				{
					INPUT_LOG_TYPE_FLAG = SEAGATE_LOG_TYPE_POWER_CONDITION_LOG;
				}
#endif
#if defined (INCLUDE_NCQ_CMD_ERROR_LOG)
                if (strcmp(optarg, LOG_TYPE_STRING_NCQ_COMMAND_ERROR_LOG) == 0)
                {
                    INPUT_LOG_TYPE_FLAG = SEAGATE_LOG_TYPE_NCQ_CMD_ERROR_LOG;
                }
#endif
#if defined (INCLUDE_SCSI_LOG_PAGES)
				if (strcmp(optarg, LOG_TYPE_STRING_SCSI_LOG_PAGES) == 0)
				{
					INPUT_LOG_TYPE_FLAG = SEAGATE_LOG_TYPE_SCSI_LOG_PAGES;
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
                    OUTPUT_LOG_PRINT_FLAG = SEAGATE_LOG_PRINT_JSON;
                }
                else if (strcmp(optarg, LOG_PRINT_STRING_TEXT) == 0)
                {
                    OUTPUT_LOG_PRINT_FLAG = SEAGATE_LOG_PRINT_TEXT;
                }
                else if (strcmp(optarg, LOG_PRINT_STRING_CSV) == 0)
                {
                    OUTPUT_LOG_PRINT_FLAG = SEAGATE_LOG_PRINT_CSV;
                }
                else if (strcmp(optarg, LOG_PRINT_STRING_FLATCSV) == 0)
                {
                    OUTPUT_LOG_PRINT_FLAG = SEAGATE_LOG_PRINT_FLAT_CSV;
                }
                else
                {
                    OUTPUT_LOG_PRINT_FLAG = SEAGATE_LOG_PRINT_JSON;
                }
            }
            else
            {
                printf("Ignoring option --%s\n",longopts[optionIndex].name);
            }
            break;
        case ':'://missing required argument
            exitCode = UTIL_EXIT_ERROR_IN_COMMAND_LINE;
            switch (optopt)
            {
            case 0:
                //check long options for missing arguments
                break;
            case VERBOSE_SHORT_OPT:
                if (VERBOSITY_QUIET < g_verbosity)
                {
                    printf("You must specify a verbosity level. 0 - 4 are the valid levels\n");
                }
                break;
            default:
                if (VERBOSITY_QUIET < g_verbosity)
                {
                    printf("Option %c requires an argument\n", optopt);
                }
                utility_Usage(true);
                return exitCode;
            }
            break;
        case VERSION_SHORT_OPT:
            SHOW_BANNER_FLAG = true;
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
            return UTIL_EXIT_NO_ERROR;
        default:
            break;
        }
    }

    if (ECHO_COMMAND_LINE_FLAG)
    {
        uint64_t commandLineIter = 1;//start at 1 as starting at 0 means printing the directory info+ SeaChest.exe (or ./SeaChest)
        for (commandLineIter = 1; commandLineIter < (uint64_t)argc; commandLineIter++)
        {
            if (strncmp(argv[commandLineIter], "--echoCommandLine", strlen(argv[commandLineIter])) == 0)
            {
                continue;
            }
            printf("%s ", argv[commandLineIter]);
        }
        printf("\n");
    }

    if (VERBOSITY_QUIET < g_verbosity)
    {
        seachest_utility_Info( util_name,  buildVersion,  OPENSEA_PARSER_VERSION);//TODO: We should change the version to a SeaParser version!
    }

    if (SHOW_BANNER_FLAG)
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
    if (SHOW_BANNER_FLAG || LICENSE_FLAG || SHOW_HELP_FLAG)
    {
        return UTIL_EXIT_NO_ERROR;
    }

    //print out errors for unknown arguments for remaining args that haven't been processed yet
    for (argIndex = static_cast<uint8_t>(optind); argIndex < argc; argIndex++)
    {
        if (VERBOSITY_QUIET < g_verbosity)
        {
            printf("Invalid argument: %s\n", argv[argIndex]);
        }
    }

    if (INPUT_LOG_FILE_FLAG)
    {
        JSONNODE *masterJson = json_new(JSON_NODE);
        switch (INPUT_LOG_TYPE_FLAG) 
        {
        case SEAGATE_LOG_TYPE_FARM:   
            {
                CFARMLog *CFarm;
                CFarm = new CFARMLog( INPUT_LOG_FILE_NAME);
                retStatus = CFarm->ParseFarmLog(masterJson);
                delete(CFarm);
            }
            break;
        case   SEAGATE_LOG_TYPE_DEVICE_STATISTICS_LOG:
            {
                CAtaDeviceStatisticsLogs *cDevicStat;
                cDevicStat = new CAtaDeviceStatisticsLogs(INPUT_LOG_FILE_NAME, masterJson);
                retStatus = cDevicStat->get_Device_Stat_Status();
                delete(cDevicStat);
            }
            break;
        case    SEAGATE_LOG_TYPE_EXT_COMPREHENSIVE_LOG:
            {
                CExtComp *cEC;
                cEC = new CExtComp(INPUT_LOG_FILE_NAME, masterJson);
                retStatus = cEC->get_EC_Status();
                delete(cEC);
            }
            break;
        case    SEAGATE_LOG_TYPE_EXT_DST_LOG:
            {
                CAta_Ext_DST_Log *cDST;
                cDST = new CAta_Ext_DST_Log(INPUT_LOG_FILE_NAME, masterJson);
                retStatus = cDST->get_Status();
                delete(cDST);
            }
            break;
        case   SEAGATE_LOG_TYPE_IDENTIFY_LOG:
            {
                CAta_Identify_log * cIdent;
                cIdent = new CAta_Identify_log(INPUT_LOG_FILE_NAME);
                retStatus = cIdent->print_Identify_Information(masterJson);
                delete (cIdent);
            }
			break;
        case SEAGATE_LOG_TYPE_IDENTIFY_DEVICE_DATA:
            {
                CAta_Identify_Log_30 *cIdData = NULL;
                cIdData = new CAta_Identify_Log_30( INPUT_LOG_FILE_NAME);
                retStatus = cIdData->parse_Identify_Log_30(masterJson);
                delete (cIdData);
            }
            break;
        case    SEAGATE_LOG_TYPE_SCT_TEMP_LOG:
            {
                CSAtaDevicStatisticsTempLogs *cSCTTemp;
                cSCTTemp = new CSAtaDevicStatisticsTempLogs(INPUT_LOG_FILE_NAME, masterJson);
                retStatus = cSCTTemp->parse_SCT_Temp_Log();
                delete (cSCTTemp);
            }
            break;
        case SEAGATE_LOG_TYPE_POWER_CONDITION_LOG:
            {
                CAtaPowerConditionsLog * cPowerCon;
                cPowerCon = new CAtaPowerConditionsLog(INPUT_LOG_FILE_NAME);
                retStatus = cPowerCon->printPowerConditionLog(masterJson);
				delete (cPowerCon);
            }
			break;
        case SEAGATE_LOG_TYPE_NCQ_CMD_ERROR_LOG:
            {
                CAta_NCQ_Command_Error_Log * cNCQ;
                cNCQ = new CAta_NCQ_Command_Error_Log(INPUT_LOG_FILE_NAME);
                retStatus = cNCQ->get_NCQ_Command_Error_Log(masterJson);
				delete (cNCQ);
            }
			break;
		case SEAGATE_LOG_TYPE_SCSI_LOG_PAGES:
			{
				CScsiLog * cLogPages;
				cLogPages = new CScsiLog(INPUT_LOG_FILE_NAME, masterJson);
				retStatus = cLogPages->get_Log_Status();
				delete (cLogPages);
			}
			break;
        default:
            // set to unknow to pass through the case statment below no type set was given or didn't match
            exitCode = UTIL_EXIT_ERROR_IN_COMMAND_LINE;
            retStatus = UNKNOWN;
            break;
        }
		// todo really check the status vs error code
		switch (retStatus)
		{
		case SUCCESS:
			exitCode = UTIL_EXIT_NO_ERROR;
			if (OUTPUT_LOG_FILE_FLAG)
			{
				printf("\nParsing completed with no issues \n");
			}
			break;
		case IN_PROGRESS:
			exitCode = UTIL_EXIT_OPERATION_STILL_IN_PROGRESS;
			if (OUTPUT_LOG_FILE_FLAG)
			{
				printf("\nParsing incomplete Operation was still in progress \n");
				json_push_back(masterJson, json_new_a("Parsing Error", "Incomplete Operation was still in progress"));
			}
			break;
		case FAILURE:
			exitCode = UTIL_EXIT_OPERATION_FAILURE;
			if (OUTPUT_LOG_FILE_FLAG)
			{
				printf("\nOperation Failure \n");
				json_push_back(masterJson, json_new_a("Parsing Error", "Operation Failure"));
			}
			break;
		case ABORTED:
			exitCode = UTIL_EXIT_OPERATION_ABORTED;
			if (OUTPUT_LOG_FILE_FLAG)
			{
				printf("\nAborted \n");
				json_push_back(masterJson, json_new_a("Parsing Error", "Aborted"));
			}
			break;
		case BAD_PARAMETER:
			exitCode = UTIL_EXIT_OPERATION_BAD_PARAMETER;
			if (OUTPUT_LOG_FILE_FLAG)
			{
				printf("\nBad Parameter \n");
				printf("Check --logType\n");
				json_push_back(masterJson, json_new_a("Parsing Error", "Bad Parameter"));
			}
			break;
		case MEMORY_FAILURE:
			exitCode = UTIL_EXIT_OPERATION_MEMORY_FAILURE;
			if (OUTPUT_LOG_FILE_FLAG)
			{
				printf("\nMemory Failure \n");
				json_push_back(masterJson, json_new_a("Parsing Error", "Memory Failure"));
			}
			break;
		case FILE_OPEN_ERROR:
			exitCode = UTIL_EXIT_CANNOT_OPEN_FILE;
			if (OUTPUT_LOG_FILE_FLAG)
			{
				printf("\nCould not Open File \n");
				json_push_back(masterJson, json_new_a("Parsing Error", "Could not Open File"));
			}
			break;
		case INVALID_LENGTH:
			exitCode = UTIL_EXIT_OPERATION_INVALID_LENGTH;
			if (OUTPUT_LOG_FILE_FLAG)
			{
				printf("\nBinary File With Invalid Length \n");
				json_push_back(masterJson, json_new_a("Parsing Error", "Invalid Length"));
			}
			break;
		default:
			printf("\n %d failure", retStatus);
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
			myFile = myFile.substr(0, myFile.rfind("."));           // remove the extention from the file
			myFile.append(".jsn");									// Auto add the jsn for json output file
			CMessage *printMessage;
			printMessage = new CMessage(masterJson, myFile, OUTPUT_LOG_PRINT_FLAG);
            std::cout << printMessage->get_Msg_JSON_Data().c_str();	// Pring to the screen
            delete(printMessage);
        }
        json_delete(masterJson);
    }
    else
    {
        //Error, no log given to parse
        exitCode = UTIL_EXIT_ERROR_IN_COMMAND_LINE;
    }
    return exitCode;
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
    print_Parser_Output_Log_Help(shortUsage);
    print_Log_Print_Help(shortUsage);
    
    //utility options - alphabetized
    printf("Utility Options\n");
    printf("===============\n");
    print_Echo_Command_Line_Help(shortUsage);
    print_Help_Help(shortUsage);
    print_License_Help(shortUsage);
    print_Verbose_Help(shortUsage);
    print_Version_Help(shortUsage, (char *)util_name.c_str());
}

