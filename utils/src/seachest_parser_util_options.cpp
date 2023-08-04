//
// seachest_parser_util_options.c
//
// Do NOT modify or remove this copyright and confidentiality notice.
//
// Copyright (c) 2015 - 2023 Seagate Technology LLC and/or its Affiliates, All Rights Reserved
//
// The code contained herein is CONFIDENTIAL to Seagate Technology LLC
// and may be covered under one or more Non-Disclosure Agreements.
// All or portions are also trade secret.
// Any use, modification, duplication, derivation, distribution or disclosure
// of this code, for any reason, not expressly authorized is prohibited.
// All other rights are expressly reserved by Seagate Technology LLC.
//
// *****************************************************************************

// \file seachest_parser_util_options.cpp
// \brief This file defines the functions and macros to make building a utility easier.

#include "seachest_parser_util_options.h"

// ---------------------------------------------------------------------------- -
//
//  seachest_utility_Info()
//
//! \brief   Description:  This function prints version information on the utility.
//
//  Entry:
//! param utilityName -  name of the utiltiy
//! param buildVersion -  build version of the utility
//! param openseaVersion - what is the version of the opensea-parser
//!
//  Exit:
//!   \return VOID
//
//-----------------------------------------------------------------------------
void seachest_utility_Info(const std::string & utilityName, const std::string & buildVersion, const std::string & openseaVersion)
{
    eArchitecture architecture = get_Compiled_Architecture();
    time_t g_curTime = time(NULL);
    struct tm timeStruct;
    char timeCString[64] = { 0 };
    char timeBuffer[CURRENT_TIME_STRING_LENGTH] = { 0 };
    memset(&timeStruct, 0, sizeof(struct tm));

    printf("===============================================================================\n");
    printf(" %s - Seagate drive utilities\n", utilityName.c_str());
    printf(" Copyright (c) 2018-2022 Seagate Technology LLC and/or its Affiliates\n");
    printf(" %s Version: %s-%s ", utilityName.c_str(), buildVersion.c_str(), openseaVersion.c_str());
    print_Architecture(architecture);
    printf("\n");
    printf(" Build Date: %s\n", __DATE__);
    printf(" Today: %s\n", get_Current_Time_String(&g_curTime, timeBuffer, CURRENT_TIME_STRING_LENGTH));
    printf("===============================================================================\n");
    strftime(timeCString, 64, " %Y-%m-%d__%H_%M_%S", get_Localtime(&g_curTime, &timeStruct));
}
//-----------------------------------------------------------------------------
//
//  utility_Full_Version_Info()
//
//! \brief   Description:  This function prints full version information on the utility.
//
//  Entry:
//! param utilityName -  name of the utiltiy
//! param buildVersion -  build version of the utility
//! param openseaVersion -  parser major version,  parser minor version ,  parser patch version
//!
//  Exit:
//!   \return VOID
//
//-----------------------------------------------------------------------------
void utility_Full_Version_Info(const std::string & utilityName, const std::string & buildVersion, const std::string & openseaVersion)
{
    char osNameCstring[OS_NAME_SIZE] = { 0 };
    OSVersionNumber osversionnumber;
    eCompiler compilerUsed = OPENSEA_COMPILER_UNKNOWN;
    compilerVersion compilerVersionInfo;
    memset(&osversionnumber, 0, sizeof(OSVersionNumber));
    memset(&compilerVersionInfo, 0, sizeof(compilerVersion));
    get_Compiler_Info(&compilerUsed, &compilerVersionInfo);
    get_Operating_System_Version_And_Name(&osversionnumber, osNameCstring);

    printf("Version Info for %s:\n", utilityName.c_str());
    printf("\tUtility Version: %s\n", buildVersion.c_str());
    printf("\tLibrary Version: %s\n", openseaVersion.c_str());
    printf("\tBuild Date: %s\n", __DATE__);
    printf("\tCompiled Architecture: ");
    print_Architecture(get_Compiled_Architecture());
    printf("\n\tDetected Endianness: ");
    print_Endianness(get_Compiled_Endianness(), false);
    printf("\n\tCompiler Used: ");
    print_Compiler(compilerUsed);
    printf("\n\tCompiler Version: ");
    print_Compiler_Version_Info(&compilerVersionInfo);
    printf("\n\tOperating System Type: ");
    print_OS_Type(osversionnumber.osVersioningIdentifier);
    printf("\n\tOperating System Version: ");
    print_OS_Version(&osversionnumber);
    printf("\n\tOperating System Name: %s\n", osNameCstring);
}
//-----------------------------------------------------------------------------
//
//  print_Final_newline()
//
//! \brief   Description:  This function prints out a single newline character. This is meant to  /n
//!           be used with atexit() for a newline space before returning to the command prompt.
//
//  Entry:
//!
//  Exit:
//!   \return VOID
//
//-----------------------------------------------------------------------------
void print_Final_newline(void)
{
    if (VERBOSITY_QUIET < g_verbosity)
    {
        printf("\n");
    }
}
//-----------------------------------------------------------------------------
//
//  print_SeaChest_Util_Exit_Codes()
//
//! \brief   Description:  prints outthe exit codes and what they mean
//
//  Entry:
//!
//  Exit:
//!   \return VOID
//
//-----------------------------------------------------------------------------
void print_SeaChest_Util_Exit_Codes(void)
{
    printf("\tGeneric/Common error codes\n");
    printf("\t%d = No Error Found\n", UTIL_EXIT_NO_ERROR);
    printf("\t%d = Error in command line options\n", UTIL_EXIT_ERROR_IN_COMMAND_LINE);
    printf("\t%d = Invalid Device Handle or Missing Device Handle\n", UTIL_EXIT_INVALID_DEVICE_HANDLE);
    printf("\t%d = Operation Failure\n", UTIL_EXIT_OPERATION_FAILURE);
    printf("\t%d = Operation not supported\n", UTIL_EXIT_OPERATION_NOT_SUPPORTED);
    printf("\t%d = Operation Failed and was still in progress \n", UTIL_EXIT_OPERATION_STILL_IN_PROGRESS);
    printf("\t%d = Operation Aborted\n", UTIL_EXIT_OPERATION_ABORTED);
    printf("\t%d = Operation Failed for Bad Parameter in the log \n", UTIL_EXIT_OPERATION_BAD_PARAMETER);
    printf("\t%d = Operation had Memory Failures\n", UTIL_EXIT_OPERATION_MEMORY_FAILURE);
    printf("\t%d = Operation Failed for Invaild Lengths in the log\n", UTIL_EXIT_OPERATION_INVALID_LENGTH);
    printf("\t%d = File Path Not Found\n", UTIL_EXIT_PATH_NOT_FOUND);
    printf("\t%d = Cannot Open File\n", UTIL_EXIT_CANNOT_OPEN_FILE);
    printf("\t%d = File Already Exists\n", UTIL_EXIT_FILE_ALREADY_EXISTS);
    printf("\t%d = Not Valid for this parser\n", UTIL_EXIT_FILE_NOT_VALID);
    printf("\t%d = Validation Failed\n", UTIL_EXIT_VALIDATION_FAILURE);
    printf("\t%d = Error in Header and Footer validation\n", UTIL_EXIT_STRIP_HDR_FOOTER_FAILURE);
    printf("\t%d = Parsing Failure\n", UTIL_EXIT_PARSE_FAILURE);    
    printf("\tAnything else = unknown error\n\n");

}
//-----------------------------------------------------------------------------
//
//  print_Verbose_Help()
//
//! \brief   Description:  This function prints out the short or long help for the verbose option
//
//  Entry:
//!   \param[in] shortHelp = bool used to select when to print short or long help
//!
//  Exit:
//!   \return VOID
//
//-----------------------------------------------------------------------------
void print_Verbose_Help(bool shortHelp)
{
    printf("\t-%c [0-4], --%s [0 | 1 | 2 | 3 | 4]\n", VERBOSE_SHORT_OPT, VERBOSE_LONG_OPT_STRING);
    if (!shortHelp)
    {
        printf("\t\tShow verbose information. Verbosity levels are:\n");
        printf("\t\t0 - quiet\n");
        printf("\t\t1 - default\n");
        printf("\t\t2 - command descriptions\n");
        printf("\t\t3 - command descriptions and values\n");
        printf("\t\tExample: -v 3 or --verbose 3\n\n");
    }
}
//-----------------------------------------------------------------------------
//
//  print_Version_Help()
//
//! \brief   Description:  This function prints out the short or long help for the version option
//
//  Entry:
//!   \param[in] shortHelp = bool used to select when to print short or long help
//!   \param[in] utilName = a string that is the name of the utility
//!
//  Exit:
//!   \return VOID
//
//-----------------------------------------------------------------------------
void print_Version_Help(bool shortHelp, const char *utilName)
{
    printf("\t-%c, --%s\n", VERSION_SHORT_OPT, VERSION_LONG_OPT_STRING);
    if (!shortHelp)
    {
        printf("\t\tShow %s version and copyright information & exit\n\n", utilName);
    }
}
//-----------------------------------------------------------------------------
//
//  print_License_Help()
//
//! \brief   Description:  This function prints out the short or long help for the license option
//
//  Entry:
//!   \param[in] shortHelp = bool used to select when to print short or long help
//!
//  Exit:
//!   \return VOID
//
//-----------------------------------------------------------------------------
void print_License_Help(bool shortHelp)
{
    printf("\t--%s\n", LICENSE_LONG_OPT_STRING);
    if (!shortHelp)
    {
        printf("\t\tDisplay the Seagate End User License Agreement (EULA).\n\n");
    }
}
//-----------------------------------------------------------------------------
//
//  print_Echo_Command_Line_Help()
//
//! \brief   Description:  This function prints out the short or long help for the echo command line option
//
//  Entry:
//!   \param[in] shortHelp = bool used to select when to print short or long help
//!
//  Exit:
//!   \return VOID
//
//-----------------------------------------------------------------------------
void print_Echo_Command_Line_Help(bool shortHelp)
{
    printf("\t--%s\n", ECHO_COMMAND_LINE_LONG_OPT_STRING);
    if (!shortHelp)
    {
        printf("\t\tShows the command line above the banner in the statndard ouput.\n");
        printf("\t\tUseful when saving output to logs\n\n");
    }
}
//-----------------------------------------------------------------------------
//
//  print_Help_Help()
//
//! \brief   Description:  This function prints out the short or long help for the help option
//
//  Entry:
//!   \param[in] shortHelp = bool used to select when to print short or long help
//!
//  Exit:
//!   \return VOID
//
//-----------------------------------------------------------------------------
void print_Help_Help(bool shortHelp)
{
    printf("\t-%c, --%s\n", HELP_SHORT_OPT, HELP_LONG_OPT_STRING);
    if (!shortHelp)
    {
        printf("\t\tShow utility options and example usage (this output you see now)\n\n");
    }
}
//-----------------------------------------------------------------------------
//
//  print_OutputPath_Help()
//
//! \brief   Description:  This function prints out the short or long help for the
//!						   output folder help.
//
//  Entry:
//!   \param[in] shortHelp = bool used to select when to print short or long help
//!
//  Exit:
//!   \return VOID
//
//-----------------------------------------------------------------------------
void print_OutputPath_Help(bool shortHelp)
{
    printf("\t--%s [folder]\n", PATH_LONG_OPT_STRING);
    if (!shortHelp)
    {
        printf("\t\tTo set a path to the folder where all logs should be created.\n");
        printf("\t\tThe directory/folder must already exist with write permissions\n");
        printf("\t\tIf this option is not used, logs are created in the current\n");
        printf("\t\tworking folder\n\n");
    }
}
//-----------------------------------------------------------------------------
//
//  print_OutpuFile_Help()
//
//! \brief   Description:  This function prints out the short or long help for the
//!                        output file name help.
//
//  Entry:
//!   \param[in] shortHelp = bool used to select when to print short or long help
//!
//  Exit:
//!   \return VOID
//
//-----------------------------------------------------------------------------
void print_OutputFile_Help(bool shortHelp)
{
    printf("\t--%s [file] \n", OUTPUTLOG_LONG_OPT_STRING);
    if (!shortHelp)
    {
        printf("\t\tTo set a name of the output file being generated.\n");
        printf("\t\tThis option will overwrite the file if it exists.\n");
        printf("\t\tIf no %s given then data will be printed to the screen.\n\n", OUTPUTLOG_LONG_OPT_STRING);
    }
}

//-----------------------------------------------------------------------------
//
//  print_Input_Log_Help()
//
//! \brief   Description:  This function prints out the information about the utility's exit codes (common exit codes only)
//
//  Entry:
//
//  Exit:
//!   \return VOID
//
//-----------------------------------------------------------------------------
void print_Input_Log_Help(bool shortHelp)
{
    std::cout << "\t--" << INPUT_LOG_LONG_OPT_STRING << " [log file name]" << std::endl;
    if (!shortHelp)
    {
        std::cout << "\t\t" << "Use this option to pass a log into the tool for parsing." << std::endl << std::endl;
		std::cout << "\t\t" << "Use --inputLog <fromPipe> option to pass a farm log buffer into the tool for parsing." << std::endl << std::endl;
    }
    else
    {
        std::cout << std::endl;
    }
}
//-----------------------------------------------------------------------------
//
//  print_Seachest_logType_options()
//
//! \brief   Description:  prints the seachest logtype options for the help files
//
//  Entry:
//
//  Exit:
//!   \return VOID
//
//-----------------------------------------------------------------------------
void print_Seachest_logType_options()
{
#if defined BUILD_FARM_ONLY 
	std::cout << LOG_TYPE_STRING_FARM" (for FARM combined, FARM Factory, FARM Time Series, FARM, Saved, FARM Sticky logs), ";       // FarmLog
#else
	std::cout << LOG_TYPE_STRING_FARM " (for FARM combined, FARM Factory, FARM Time Series, FARM, Saved, FARM Sticky logs), ";      // FARM log
	std::cout << std::endl;
	std::cout << "\t\t ";
	#if defined (INCLUDE_IDENTIFY_LOG)
		std::cout << LOG_TYPE_STRING_IDENTIFY_LOG;                       // identify
	#endif
	#if defined (INCLUDE_IDENTIFY_DEVICE_DATA_LOG)
		std::cout << ", " << LOG_TYPE_STRING_IDENTIFY_DEVICE_DATA_LOG;          // "IDDataLog" 
	#endif
	#if defined (INCLUDE_DEVICE_STATISTICS_LOG)
		std::cout << ", " << LOG_TYPE_STRING_DEVICE_STATISTICS_LOG;             //"deviceStatisticsLog"
	#endif
	#if defined (INCLUDE_EXT_COMPREHENSIVE_LOG)
		std::cout << ", " << LOG_TYPE_STRING_EXT_COMPREHENSIVE_LOG;             //ExtCompErrorLog
	#endif

		std::cout << std::endl;

		std::cout << "\t\t ";
	#if defined (INCLUDE_SCT_TEMP_LOG)    
		std::cout << LOG_TYPE_STRING_SCT_TEMP_LOG;                              //sctTempLog
	#endif 
	#if defined (INCLUDE_NCQ_CMD_ERROR_LOG)
		std::cout << ", " << LOG_TYPE_STRING_NCQ_COMMAND_ERROR_LOG;             // ncqErrorLog
	#endif
	#if defined (INCLUDE_POWER_CONDITION_LOG)
		std::cout << ", " << LOG_TYPE_STRING_POWER_CONDITION_LOG;                // PowerConditionLog
	#endif
	#if defined (INCLUDE_COMMON_EXT_DST_LOG)
		std::cout << ", " << LOG_TYPE_STRING_EXT_DST_LOG;                       // selfTestLog
	#endif
		std::cout << std::endl;

		std::cout << "\t\t ";
	#if defined (INCLUDE_SCSI_LOG_PAGES)  
		std::cout << LOG_TYPE_STRING_SCSI_LOG_PAGES;                              //scsiLogPages
	#endif 
#endif
}
//-----------------------------------------------------------------------------
//
//  print_Scsi_Log_Pages()
//
//! \brief   Description:  prints out The Scsi log pages supported
//
//  Entry:
//
//  Exit:
//!   \return VOID
//
//-----------------------------------------------------------------------------
void print_Scsi_Log_Pages()
{
	std::cout << "\t\t SCSI Log Pages List -  "<< std::endl;
	std::cout << "\t\t\t" << SCSI_LOG_PAGES_WRITE_LOG_PAGE << ", " << SCSI_LOG_PAGES_READ_LOG_PAGE << ", " << SCSI_LOG_PAGES_VERIFY_LOG_PAGE << std::endl;
	std::cout << "\t\t\t" << SCSI_LOG_PAGES_APPLICATION_CLIENT << ", " << SCSI_LOG_PAGES_BACKGROUND_SCAN << ", " << SCSI_LOG_PAGES_BACKGROUND_OPERATION << std::endl;
	std::cout << "\t\t\t" << SCSI_LOG_PAGES_CACHE_STATISTICS << ", " << SCSI_LOG_PAGE_COMMAND_DURATION_LIMITES << ", " << SCSI_LOG_PAGES_ENVIRONMENTAL_REPORTING << std::endl;
	std::cout << "\t\t\t" << SCSI_LOG_PAGES_ERROR_COUNTER << ", " << SCSI_LOG_PAGES_ENVIRONMENTAL_LIMITS << ", " << SCSI_LOG_PAGES_ENVIRONMENTAL_REPORTING << std::endl;
	std::cout << "\t\t\t" << SCSI_LOG_PAGES_FACTORY_LOG << ", " << SCSI_LOG_PAGES_FARM << ", " << SCSI_LOG_PAGES_FARM_FACTORY << std::endl;
	std::cout << "\t\t\t" << SCSI_LOG_PAGES_FARM_TIME_SERIES << ", " << SCSI_LOG_PAGES_FARM_STICKY << std::endl;
	std::cout << "\t\t\t" << SCSI_LOG_PAGES_FORMAT_STATUS << ", " << SCSI_LOG_PAGES_INFORMATIONAL_EXCEPTIONS << ", " << SCSI_LOG_PAGES_LOGICAL_BLOCK_PROVISIONING << std::endl;
	std::cout << "\t\t\t" << SCSI_LOG_PAGES_NON_MEDIUM_ERROR << ", " << SCSI_LOG_PAGES_PENDING_DEFECTS << ", " << SCSI_LOG_PAGES_POWER_CONDITIONS <<  std::endl;
	std::cout << "\t\t\t" << SCSI_LOG_PAGES_PROTOCOL_PAGE << ", "  << SCSI_LOG_PAGES_DST_LOG_PAGE << SCSI_LOG_PAGES_SOLID_STATE_MEDIA << std::endl;
	std::cout << "\t\t\t" << SCSI_LOG_PAGES_START_STOP_CYCLE_PAGE << ", "  SCSI_LOG_PAGES_SUPPORTED_LOG_PAGES_AND_SUBPAGES << ", " << SCSI_LOG_PAGES_SUPPORTED_LOG_PAGES << std::endl;
	std::cout << "\t\t\t" << SCSI_LOG_PAGES_TEMPERATURE_LOG_PAGE << ", "<< SCSI_LOG_PAGES_UTILIZATION <<  ", " << SCSI_LOG_PAGES_ZONED_DEVICE_STAT << std::endl;

}
//-----------------------------------------------------------------------------
//
//  print_Log_Type_Help()
//
//! \brief   Description:  This function prints out the information about log types need for parsing
//
//  Entry:
//!  \param shortHelp - if set to true, function will do a short help information only
//
//  Exit:
//!   \return VOID
//
//-----------------------------------------------------------------------------
void print_Log_Type_Help(bool shortHelp)
{
    std::cout << "\t--" << INPUT_LOG_TYPE_LONG_OPT_STRING << " [choose from list below]" << std::endl;
    std::cout << "\t\t{ ";
    print_Seachest_logType_options();
    std::cout << " } " << std::endl << std::endl;

    if (!shortHelp)
    {
        std::cout << "\t\t" << "Use this option to describe the type of log that is being passed in." << std::endl;
        std::cout << std::endl;
        print_Scsi_Log_Pages();

    }
    else
    {
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
//-----------------------------------------------------------------------------
//
//  print_Parser_Output_Log_Help()
//
//! \brief   Description:  This function prints out the information forthe output log
//
//  Entry:
//!  \param shortHelp - if set to true, function will do a short help information only
//
//  Exit:
//!   \return VOID
//
//-----------------------------------------------------------------------------
void print_Parser_Output_Log_Help(bool shortHelp)
{
    std::cout << "\t--" << OUTPUT_LOG_LONG_OPT_STRING << " [log file name]" << std::endl;
    if (!shortHelp)
    {
        std::cout << "\t\t" << "To set a name of the output file being generated. This option will overwrite" << std::endl;
        std::cout << "\t\tfile if it exists." << std::endl;
        std::cout << "\t\tIf no --" << OUTPUTLOG_LONG_OPT_STRING << " given then data will be printed to the screen." << std::endl;
        std::cout << std::endl;
    }
    else
    {
        std::cout << std::endl;
    }
}
//-----------------------------------------------------------------------------
//
//  print_Log_Print_Help()
//
//! \brief   Description:  This function prints out the information and help on the log print types
//
//  Entry:
//!  \param shortHelp - if set to true, function will do a short help information only
//
//  Exit:
//!   \return VOID
//
//-----------------------------------------------------------------------------
void print_Log_Print_Help(bool shortHelp)
{
    std::cout << "\t--" << OUTPUT_LOG_PRINT_LONG_OPT_STRING << " [choose from list below]" << std::endl;
    std::cout << "\t\t{" << LOG_PRINT_STRING_JSON << ", " << LOG_PRINT_STRING_TEXT << ", " << LOG_PRINT_STRING_CSV << ", " << LOG_PRINT_STRING_FLATCSV << ", " << LOG_PRINT_STRING_PROM << "}" << std::endl;
    if (!shortHelp)
    {
        std::cout << "\t\t" << "Use this option to set the output format. \n" << std::endl;
        std::cout << "\t\t" << LOG_PRINT_STRING_JSON << " - prints the data in a printable json format" << std::endl;
        std::cout << "\t\t" << LOG_PRINT_STRING_TEXT << " - prints the data in a printable and human readable format" << std::endl;
        std::cout << "\t\t" << LOG_PRINT_STRING_CSV << "  - The data flows downwards" << std::endl;
        std::cout << "\t\t" << LOG_PRINT_STRING_FLATCSV << " - The data is set to flow in two rows only" << std::endl;
        std::cout << "\t\t" << LOG_PRINT_STRING_PROM << " - Prints the data in a format readable by Prometheus.\n\n"
            << "\t\t\t" << "If no output log is specified (with --outputLog), data is printed\n"
            << "\t\t\t" << "out to standard output and a file is automatically created.\n"
            << "\t\t\t" << "The output file will have the same name as the input file\n"
            << "\t\t\t" << "with the extension \".prom\" which can be opened in any text editor.\n\n" 
            << "\t\t\t" << "If an output log is specified (with --outputLog), data is printed\n"
            << "\t\t\t" << "directly to the specified file.\n" << std::endl;
        std::cout << "\t\t\t" << "[USAGE EXAMPLES]" << std::endl;
        std::cout << "\t\t\t" << "openSeaChest_LogParser --inputLog <fileName>.bin --logType farmLog --printType prom" << std::endl;
        std::cout << "\t\t\t\t" << "Takes in a FARM log, <fileName>.bin, prints the data in Prometheus' format\n"
            << "\t\t\t\t" << "to standard output, and saves the output in the current directory as <fileName>.prom.\n" << std::endl;
        std::cout << "\t\t\t" << "openSeaChest_LogParser --inputLog <inputFile>.bin --logType farmLog --printType prom --outputLog <outputFile>.prom" << std::endl;
        std::cout << "\t\t\t\t" << "Takes in a FARM log, <inputFile>.bin, and saves the output in the current directory\n"
            << "\t\t\t\t" << "<outputFile>.prom without printing to standard output.\n" << std::endl;
    }
    std::cout << std::endl;
}
//-----------------------------------------------------------------------------
//
//  print_FARM_Command_Line_Option_to_Show_Status_Bytes()
//
//! \brief   Description:  This function prints out the show status bytes command
//
//
//  Exit:
//!   \return VOID
//
//-----------------------------------------------------------------------------
void print_FARM_Command_Line_Option_to_Show_Status_Bytes()
{
    std::cout << "\t--" << SHOW_STATUS_BITS_OPT_STRING  << std::endl;
    std::cout << "\t\t" << "Command Line Option for the FARM Log ONLY " << std::endl;
    std::cout << "\t\t" << "Use this option to set the parser to gather the status bytes for each field." << std::endl;
    std::cout << "\t\t" << "For each field the Supported byte will show TRUE or FALSE " << std::endl;
    std::cout << "\t\t" << "For each field the Valid byte will show TRUE or FALSE " << std::endl;
    std::cout << "\t\t" << "Device Information will not show status bytes." << std::endl;

    std::cout << std::endl;
}

//-----------------------------------------------------------------------------
//
//  print_FARM_Analyze_Command_Line_Option()
//
//! \brief   Description:  This function prints out the analyze command
//
//
//  Exit:
//!   \return VOID
//
//-----------------------------------------------------------------------------
void print_FARM_Analyze_Command_Line_Option()
{
    std::cout << "\t--" << ANALYZE_LONG_OPT_STRING << std::endl;
    std::cout << "\t\t" << "Command Line Option for the FARM Log ONLY " << std::endl;
    std::cout << "\t\t" << "Use this option to analyze the FARM Log and state the Drive Health." << std::endl;

    /*
    std::cout << "\t\t" << "Following are the possable Drive Health status " << std::endl;
    std::cout << "\t\t" << "    NTF " << std::endl;
    std::cout << "\t\t" << "    PRESSURE " << std::endl;
    std::cout << "\t\t" << "    FW_ASSERTION " << std::endl;
    std::cout << "\t\t" << "    DATA_PATH " << std::endl;
    std::cout << "\t\t" << "    SEEK_HARD_ERROR " << std::endl;
    std::cout << "\t\t" << "    UNRECOVERABLE_SERVO " << std::endl;
    std::cout << "\t\t" << "    UNRECOVERABLE_WRITE " << std::endl;
    std::cout << "\t\t" << "    UNRECOVERABLE_READ_GLIST " << std::endl;
    std::cout << "\t\t" << "    UNDEFINED " << std::endl;
    */

    std::cout << std::endl;
}

