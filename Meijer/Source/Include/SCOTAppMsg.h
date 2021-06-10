//
// This is the SCOTAPP Message (Source) File.
//
//
//  
//**  Copyright NCR Corp. 1997
//**  This media contains an authorized
//**  copy or copies of material copyrighted
//**  by NCR Corp.  This copyright notice
//**  and any other copyright notices
//**  included in machine readable copies
//**  must be reproduced on all authorized
//**  copies.  Confidential, Unpublished
//**  Property of NCR Corporation
//
//
// The following lines should be in your application specific message file:
//      //NMAS_REGISTRY_NAME should be 18 char or less
//      #define NMAS_REGISTRY_NAME	"test"
// 
// The install process for "NMAS_REGISTRY_NAME" should add the following key
//     and values to the NT registry.
//         HKEY_LOCAL_MACHINE 
//           System 
//             CurrentControlSet 
//               Services 
//                 EventLog 
//                   Application 
//                     <NMAS_REGISTRY_NAME> 
//							EventMessageFile:REG_EXPAND_SZ:Full Path name of the 
//								executable or DLL containing the messages 
//							CategoryMessageFile:REG_EXPAND_SZ:Full Path name of the 
//								executable or DLL containing the Categories can be same 
//								as EventMessageFile
//							CategoryCount:REG_DWORD:Hex value of "NMAS_CATEGORY_COUNT"
//								or the total number of categories in CategoryMessageFile
//							TypesSupported:REG_DWORD:0x07
//

//Within the message text, several escape sequences are supported for
//    dynamically formatting the message.  The percent sign character (%)
//    begins all escape sequences.
//
//
//        %0 - This terminates a message text line without a trailing
//             newline.  This can be used to build up long lines or to
//             terminate the message itself without a trailing newline,
//             which is useful for prompt messages.
//
//        %n!printf format string!  - This identifies an insert.  The
//             value of n can be between 1 and 99.  The printf format
//             string must be bracketed by exclamation marks.  It is
//             optional and defaults to !s! if not specified.
//
//             The printf format string can contain the * specifier for
//             either the precision or width components, and if so, they
//             will consume inserts %n+1 and %n+2 for their values at run
//             time.  MC will print a warning message if an explicit
//             reference is made to these inserts elsewhere in the message
//             text.
//
//    Inserts must reference a parameter passed to the FormatMessage API
//    call.  It will return an error if a message refers to an insert that
//    was not passed to the FormatMessage API call.
//
//    Any other character following a percent sign, other than a digit will
//    be formatted in the output message without the percent sign.  Some
//    examples:
//
//        %% - will output a single percent sign in the formatted message text.
//
//        %n - will output a hard line break when it occurs at the end of a
//             a line.  Useful when FormatMessage is supplying normal line
//             breaks so the message fits in a certain width.
//
//        %r - will output a hard carriage return, without a trailing newline.
//
//        %b - will output a space in the formatted message text.  This
//             can be used to insure there are the appropriate number of
//             trailing spaces in a message text line.
//
//        %t - will output a tab in the formatted message text.
//
//        %. - will output a single period in the formatted message text.
//             This can be used to get a single period at the beginning of
//             a line without terminating the message text definition.
//
//        %! - will output a single exclamation point in the formatted
//             message text.  This can be used to get an exclamation point
//             immediately after an insert without it being mistaken for
//             the beginning of a printf format string.
//
//  The Following is a sample Message
//
//  MessageId=0x0100
//  SymbolicName=ERROR_ONE
//  Language=English
//  Starting tmssvr%0
//  .
//  Language=French
//  Commencant tmssvr%0
//  .
//
//
//  sample message entry is below
// Parameter 1 is a file name used by SCOT application
// MessageId=2200
// SymbolicName=SCOTAPP_FILE_NOTFOUND
// Language=English
// %1 file not found.
// %nMeaning - The application can not find file %1. 
// %nAction - Reinstall the application. If still problem exists, contact an NCR Support representative.%n%0
// .
//-------------------------------------------------------------------------------------------------
// All entries - %1-%2@%3 writes out the system category, file name abbreviation, and line number
//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
// Copied from Platform mc files
//
// The StoreMinder CNCREventLog class uses the Severity field to 
// pass the Severity of the message being logged from the message 
// file to ReportEvent.
//
// The CONVERT_Severity macro is used for this purpose.
//



#define CONVERT_SM_Severity(X)           \
		(((X>>30)==Severity_SM_SUCCESS)    ? evlTypeAuditSuccess:           \
		(((X>>30)==Severity_SM_INFORMATION)? evlTypeInformation:            \
		(((X>>30)==Severity_SM_WARNING)    ? evlTypeWarning:                \
		(((X>>30)==Severity_SM_ERROR)      ? evlTypeError : evlTypeInformation))))

//
// The StoreMinder CNCREventLog class uses the Facility field to 
// pass the category of the message being logged from the message 
// file to ReportEvent.  
//
// The CONVERT_SM_CATEGORY macro is used for this purpose.
//
// SM_CATEGORY_COUNT should be the total number of categories
#define SM_CATEGORY_COUNT 18
//SOURCEID=SCOTAPP
#define CONVERT_SM_CATEGORY(X)   (WORD)((X>>16) & 0xFFF)														\
		

//END SECTION
// XMLOFF
//
//  Values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//
#define SM_CAT_3rdPARTY_INTERFACE        0x6
#define SM_CAT_SYSTEM_CALL               0x5
#define SM_CAT_STOP_ABORT                0x2
#define SM_CAT_STARTUP_INIT              0x1
#define SM_CAT_SECURITY                  0xA
#define SM_CAT_RESOURCE                  0xC
#define SM_CAT_PERIPHERAL_COMM           0xE
#define SM_CAT_OPERATIONAL               0x11
#define SM_CAT_LAN_WAN_COMM              0x9
#define SM_CAT_IPC                       0xD
#define SM_CAT_INTERNAL_INTERFACE        0x7
#define SM_CAT_INTERNAL_ERROR            0x12
#define SM_CAT_INPUT_PARAMETER           0x4
#define SM_CAT_HARDWARE                  0xF
#define SM_CAT_FILE_IO                   0xB
#define SM_CAT_DEVICE_PERIPHERAL         0x10
#define SM_CAT_DATA_CAPTURE              0x8
#define SM_CAT_CONFIGURATION             0x3


//
// Define the severity codes
//
#define Severity_SM_WARNING              0x2
#define Severity_SM_SUCCESS              0x0
#define Severity_SM_INFORMATION          0x1
#define Severity_SM_ERROR                0x3


//
// MessageId: SCOTAPP_INITIALIZE
//
// MessageText:
//
//  %1-%2@%3   Starting SCOT...   %n %n %4
//  %nMeaning - Informational, when application is starting up - SCOTAPPDlg.cpp  ::OnInitDialog
//  %nAction - none.
//
#define SCOTAPP_INITIALIZE               0x40011388L

//-------------------------------------------------------------------------------------------------
// not in use at this time 
//XMLOFF
//
// MessageId: SCOTAPP_INITIALIZED
//
// MessageText:
//
//  %1-%2@%3   SCOTAPP was initialized. %n %n %4
//
#define SCOTAPP_INITIALIZED              0x40011389L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//XMLOFF
//
// MessageId: SCOTAPP_EXIT
//
// MessageText:
//
//  %1-%2@%3   SCOTAPP was ended. %n %n %4
//  %nMeaning - Application has been stopped. %n
//  %nAction - none
//
#define SCOTAPP_EXIT                     0x4002138AL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_RESTART
//
// MessageText:
//
//  %1-%2@%3   SCOTAPP was forced to restart. %n %n %4
//  %nMeaning - Application has been forced to restart. %n
//  %nAction - Run Get Diag files.
//
#define SCOTAPP_RESTART                  0xC002138BL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Text msg from GetErrorMessage() is used in the 1st parameter at the right.
//
// MessageId: SCOTAPP_RESTART2
//
// MessageText:
//
//  %1-%2@%3   SCOTAPP was forced to restart <%4>. %n %n %5
//  %nMeaning - Fatal"Stop Sign", exception during message processing has forced a restart. %n
//  %nAction - Run Get Diag files.
//
#define SCOTAPP_RESTART2                 0xC002138CL

//-------------------------------------------------------------------------------------------------
// not in use at this time 
//
// MessageId: SCOTAPP_FILE_READ_ERROR
//
// MessageText:
//
//  %1-%2@%3   File read %n %n %4
//
#define SCOTAPP_FILE_READ_ERROR          0xC00B138DL

//-------------------------------------------------------------------------------------------------
// Called from 2 locations. Section, Key, Default text messages are used in 1st 3 parameters
//
// MessageId: SCOTAPP_MISSING_OPTION
//
// MessageText:
//
//  %1-%2@%3   Missing option [%4] %5, using default <%6> %n %n %7
//  %nMeaning - Configuration option not defined or incorrect. %n
//  %nAction - if the default value is not desired, set the option.
//
#define SCOTAPP_MISSING_OPTION           0x4003138EL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Status of Cash Value List & Cash Low List are used in 2 parameters
//
// MessageId: SCOTAPP_FAIL_PARSE
//
// MessageText:
//
//  %1-%2@%3   Failed to parse csLocaleCashValueList=%4 or csLocaleCashLowList=%5. 
//  %nMeaning - Configuration option csLocaleCashValueList and/or csLocaleCashLowList are empty. %n
//  %nAction - Make sure both lists are defined and have the same number of entries.
//
#define SCOTAPP_FAIL_PARSE               0xC012138FL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Denominations is used in the 1st parametter
//
// MessageId: SCOTAPP_TOO_MANY_TENDER_TYPE
//
// MessageText:
//
//  %1-%2@%3   Too many denominations in list <%4> - only the first 7 are accepted %n %n %5
//  %nMeaning - More than 7 tender denominations have been defined. %n
//  %nAction - Verify the "Denominations" configuration setting in Scotopts.dat and Scotopts.xxx files.
//
#define SCOTAPP_TOO_MANY_TENDER_TYPE     0xC0041390L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_DMPROCEDURES_INIT_FAILED
//
// MessageText:
//
//  %1-%2@%3   Unable to initialize sound path %n %n %4%n
//  %nMeaning - Validation of registry settings for SoundInitPath & SCOTPriceSoundInit failed. %n
//  %nAction - Make sure the ScotPriceSound path is correct in the registry
//
#define SCOTAPP_DMPROCEDURES_INIT_FAILED 0xC0011391L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_DMPROCEDURES_SAYAMOUNT
//
// MessageText:
//
//  %1-%2@%3   SayAmount failed %n %n %4
//  %nMeaning - Failure while attempting to play amount sound files. %n
//  %nAction - make sure the speaker devices are installed and operating. 
//
#define SCOTAPP_DMPROCEDURES_SAYAMOUNT   0xC0121392L

//-------------------------------------------------------------------------------------------------
// Called from 2 locations. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_DMPROCEDURES_SAYPHRASE
//
// MessageText:
//
//  %1-%2@%3   SayPhrase failed %n %n %4
//  %nMeaning - Failure while attempting to play sound file. %n
//  %nAction - make sure the speaker devices are installed and operating. %n
//
#define SCOTAPP_DMPROCEDURES_SAYPHRASE   0x40031393L

//-------------------------------------------------------------------------------------------------
// Called from 2 locations. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_DMPROCEDURES_SAYSECURITY
//
// MessageText:
//
//  %1-%2@%3   SaySecurity failed %n %n %4
//  %nMeaning - Failure while attempting to play security sound file. %n
//  %nAction - make sure the speaker devices are installed and operating. 
//
#define SCOTAPP_DMPROCEDURES_SAYSECURITY 0xC0121394L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_DMPROCEDURES_SAYSIGNAL
//
// MessageText:
//
//  %1-%2@%3   SaySignal failed %n %n %4
//  %nMeaning - Failure while attempting to play security signal (beep) file. %n
//  %nAction - make sure the speaker devices are installed and operating. 
//
#define SCOTAPP_DMPROCEDURES_SAYSIGNAL   0xC0121395L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_DMPROCEDURES_ENCRYPTORKEY_LENGTH
//
// MessageText:
//
//  %1-%2@%3   DMProcedures::StoreEncryptorKey length > MAXENCRYPTORPIN %n %n %4
//  %nMeaning - Maximum STORE ENCRYPTOR PIN length is exceeded. %n
//  %nAction - make sure the correct encryptor pin is used
//
#define SCOTAPP_DMPROCEDURES_ENCRYPTORKEY_LENGTH 0xC0031396L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_TIMER_ESTABLISH_FAILED
//
// MessageText:
//
//  %1-%2@%3   ScotApp failed to establish a timer for resending pages %n %n %4
//  %nMeaning - Unable to set the timer for resending a page. %n
//  %nAction - Run Get Diag files.
//
#define SCOTAPP_TIMER_ESTABLISH_FAILED   0xC0051397L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_PAGER_UNAVAILABLE
//
// MessageText:
//
//  %1-%2@%3   SCOT Paging System is unavailable %n %n %4
//  %nMeaning - The pager server is unavailable at start up. %n
//  %nAction - If the pager server should be available, check the network connections. If the pager server should NOT be available - then un-install the pager server
//
#define SCOTAPP_PAGER_UNAVAILABLE        0x40061398L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. The state that failed is used in the 1st parameter at the right.
//
// MessageId: SCOTAPP_STATE_POINT
//
// MessageText:
//
//  %1-%2@%3   Invalid state pointer %4 %n %n %5
//  %nMeaning - Fatal "Stop Sign", State = NULL || STATE_STOP || STATE_RETURN || STATE_RESUME || STATE_RESTART || STATE_ENDED || STATE_ERROR || STATE_NULL %n
//  %nAction - Reboot NT and Run Get Diag files. 
//
#define SCOTAPP_STATE_POINT              0xC0071399L

//-------------------------------------------------------------------------------------------------
//
// MessageId: SCOTAPP_STATE
//
// MessageText:
//
//  %1-%2@%3   State error indicated %n %n %4
//  %nMeaning - Next state = STATE_ERROR, an error has occurred - ScotApp is going Out Of Service. %n
//  %nAction - Run Get Diag files.
//
#define SCOTAPP_STATE                    0xC007139AL

// Called from 1 location. Parameters at the right not used at this time.
//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_STOP_REQUEST
//
// MessageText:
//
//  %1-%2@%3   Stop request %n %n %4
//  %nMeaning - Next state = STATE_STOP, ScotApp is stopping due to error. %n
//  %nAction - Run Get Diag files.
//
#define SCOTAPP_STOP_REQUEST             0xC003139BL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_START_REQUEST
//
// MessageText:
//
//  %1-%2@%3   Restart request %n %n %4
//  %nMeaning - Next state = STATE_RESTART, ScotApp is restarting. %n
//  %nAction - Run Get Diag files.
//
#define SCOTAPP_START_REQUEST            0x4001139CL

//-------------------------------------------------------------------------------------------------
// Not Used. Bitmap name that could not be set is used in the 1st parameter at the right
//
// MessageId: SCOTAPP_MISS_BMP
//
// MessageText:
//
//  %1-%2@%3   Missing screen BMP file %4 %n %n %5
//  %nMeaning - Not able to set the background bmp. %n
//  %nAction - Make sure the bitmap is in the image directory
//
#define SCOTAPP_MISS_BMP                 0xC003139DL

//-------------------------------------------------------------------------------------------------
// Not Used. 
//
// MessageId: SCOTAPP_PSTHREAD_CREATE
//
// MessageText:
//
//  %1-%2@%3   Cannot create PSThread %n %n %4
//  %nMeaning - Fatal"Stop Sign", unable to create PSThread. %n
//  %nAction - Run Get Diag files%n.
//
#define SCOTAPP_PSTHREAD_CREATE          0xC012139EL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at right defined in the above list -  "%s|%s|%d|%s|%s|%d", szWho,szWindow,nIdc,szDesc,szText,rc)
//
// MessageId: SCOTAPP_PSINTERFACE
//
// MessageText:
//
//  %1-%2@%3   Error in PSInterface: %4, %5, ID=%6, %7, %8, RC=%9 %n %n %10
//  %nMeaning - Fatal "Stop Sign", Called from PSInterface.cpp methods that manipulate window and button controls if any fail to init and the IDC (control id) is not 0 - PSInterface.cpp _Initialize %n
//  ;//		RcError(const int nLine,
//  ;//				const int nLevel,
//  ;//				LPCSTR szWho,		// caller name
//  ;//				LPCSTR szWindow,	// name of window
//  ;//				const int nIdc,		// ID of element or 0
//  ;//				LPCSTR szDesc,		// desc of event
//  ;//				LPCSTR szText,		// formatted text
//  ;//				const PSRC rc)		// rc from PS function call %n %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_PSINTERFACE              0xC005139FL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at right defined in the above list -  "%s|%s|%s|%d", szWho,szDesc,szText,rc)
//
// MessageId: SCOTAPP_PSINTERFACE2
//
// MessageText:
//
//  %1-%2@%3   Error in PSInterface: %4, %5, %6, RC=%7 %n %n %8%n
//  %nMeaning - Fatal"Stop Sign", Called from PSInterface.cpp methods that manipulate window and button controls if any fail and the IDC (control id) is = 0 - PSInterface.cpp _Initialize
//  ;//		RcError(const int nLine,
//  ;//				const int nLevel,
//  ;//				LPCSTR szWho,		// caller name
//  ;//				LPCSTR szWindow,	// name of window
//  ;//				const int nIdc,		// ID of element or 0
//  ;//				LPCSTR szDesc,		// desc of event
//  ;//				LPCSTR szText,		// formatted text
//  ;//				const PSRC rc)		// rc from PS function call %n %n
//  %nAction - Run Get Diag files.
//
#define SCOTAPP_PSINTERFACE2             0xC01213A0L

//-------------------------------------------------------------------------------------------------
// Called from 4 locations
//
// MessageId: SCOTAPP_PS_FILE
//
// MessageText:
//
//  %1-%2@%3   SCOT could not open file: \%4\ %n %n %5%n
//  %nMeaning - Informational, unable to open file.  The file is not critical to continuing operations. %n
//  ;// File name is used in the 1st parameter at the right
//  %nAction - make sure the file name exists and is not corrupt%n
//
#define SCOTAPP_PS_FILE                  0xC00313A1L

//-------------------------------------------------------------------------------------------------
// Called from 5 locations
//
// MessageId: SCOTAPP_MISS_ID
//
// MessageText:
//
//  %1-%2@%3   Requested ID = %4 was not found, missing(in) %5. %n %n %6
//  %nMeaning - Bitmap, text or wave ID not defined in configuration files. %n 
//  ;// Wave/text/bmp ID that could not be found is used in the 1st parameter, Filename is used in the 2nd parameter %n
//  %nAction - Make sure that ID is correctly defined in configuration file.
//
#define SCOTAPP_MISS_ID                  0x400313A2L

//-------------------------------------------------------------------------------------------------
// not in use at this time 
//
// MessageId: SCOTAPP_MISS_BMP_NAME
//
// MessageText:
//
//  %1-%2@%3   Missing BMP name %4 %n %n %5
//
#define SCOTAPP_MISS_BMP_NAME            0xC00313A3L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Error message is used in the 1st parameter at the right.
//
// MessageId: SCOTAPP_PSTHREAD_EXCEPTION
//
// MessageText:
//
//  %1-%2@%3   PS Thread CException <%4> %n %n %5
//  %nMeaning - CException was caught while handling callback events for message object posting user activity (MO.PostPS). %n
//  %nAction - Software problem, run Get Diag files.%n
//
#define SCOTAPP_PSTHREAD_EXCEPTION       0xC00513A4L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.%n
//
// MessageId: SCOTAPP_PSTHREAD_EXCEPTION2
//
// MessageText:
//
//  %1-%2@%3   PS Thread exception %n %n %4%n
//  %nMeaning - Exception was caught while handling callback events for message object posting user activity (MO.PostPS). %n
//  %nAction - Software problem, run Get Diag files.%n
//
#define SCOTAPP_PSTHREAD_EXCEPTION2      0xC01213A5L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Msg, Description, and Error are used in 3 of the parameters%n
//
// MessageId: SCOTAPP_SAINTERFACE_OLEDISPATCH_EXCEPTION
//
// MessageText:
//
//  %1-%2@%3   %4 COleDispatchException[ %5 ] Code(0x%6) %n %n %7
//  %nMeaning - COleDispatchException was caught while interfacing with security. %n
//  %nAction - Software problem, run Get Diag files.%n
//
#define SCOTAPP_SAINTERFACE_OLEDISPATCH_EXCEPTION 0xC00713A6L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Msg, Description, and Error are used in 3 of the parameters%n
//
// MessageId: SCOTAPP_SAINTERFACE_OLE_EXCEPTION
//
// MessageText:
//
//  %1-%2@%3   %4 COleException Error[ %5 ] Code(0x%6) %n %n %7
//  %nMeaning - COleException was caught while interfacing with security. %n
//  %nAction - Software problem, run Get Diag files.%n
//
#define SCOTAPP_SAINTERFACE_OLE_EXCEPTION 0xC00713A7L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Msg and Description are used in 2 of the parameters%n
//
// MessageId: SCOTAPP_SAINTERFACE_C_EXCEPTION
//
// MessageText:
//
//  %1-%2@%3   %4 CException Error[ %5 ] %n %n %6
//  %nMeaning - CException was caught while interfacing with security. %n
//  %nAction - Software problem, run Get Diag files.%n
//
#define SCOTAPP_SAINTERFACE_C_EXCEPTION  0xC00713A8L

//-------------------------------------------------------------------------------------------------
// Called from 5 locations. Msg and Error are used in 2 of the parameters at the right%n
//
// MessageId: SCOTAPP_SAINTERFACE_EXCEPTION
//
// MessageText:
//
//  %1-%2@%3   %4 catch(...) error = %5 %n %n %6
//  %nMeaning - Exceptions was caught while interfacing with security. %n
//  %nAction - Software problem, run Get Diag files.%n
//
#define SCOTAPP_SAINTERFACE_EXCEPTION    0xC00713A9L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.%n
//
// MessageId: SCOTAPP_SAINTERFACE_STOP
//
// MessageText:
//
//  %1-%2@%3   Failed to create SA control %n %n %4
//  %nMeaning - Failed to create the Security Agent control.%n
//  %nAction - Software problem, run Get Diag files.%n
//
#define SCOTAPP_SAINTERFACE_STOP         0x400213AAL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_SAPROCEDURE_SENDSCOTINPUT
//
// MessageText:
//
//  %1-%2@%3   Security SendSCOTInput ERROR %n %n %4
//  %nMeaning - Unable to send events to security. %n
//  %nAction - Run Get Diag files. Might temporarily turn on Security DataCapture for more logging.%n
//
#define SCOTAPP_SAPROCEDURE_SENDSCOTINPUT 0xC01213ABL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.%n
//
// MessageId: SCOTAPP_SAPROCEDURE_MAIN_WINDOW_FAILED
//
// MessageText:
//
//  %1-%2@%3   Security Agent Procedures main window NOT Created %n %n %4%n
//  %nMeaning - System Restart (Fatal), if cannot create the main security procedures handle to a window - SAProcedures.cpp  ::Initialize %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_SAPROCEDURE_MAIN_WINDOW_FAILED 0xC00113ACL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_SAPROCEDURE_WINDOW_FAILED
//
// MessageText:
//
//  %1-%2@%3   Security Agent Interface window NOT Created %n %n %4%n
//  %nMeaning - System Restart (Fatal), Cannot create the security interface handle to a window. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_SAPROCEDURE_WINDOW_FAILED 0xC00113ADL

//-------------------------------------------------------------------------------------------------
// Called from 2 locations. Filename is used in the 1st parameter at the right%n
//
// MessageId: SCOTAPP_SAPROCEDURE_LOAD_ACTION
//
// MessageText:
//
//  %1-%2@%3   Cannot load Security actions file %4 %n %n %5%n
//  %nMeaning - System Restart (Fatal), failed to load the SASIActions.dat file %n
//  %nAction - Make sure the SASIActions file is installed and not corrupt%n
//
#define SCOTAPP_SAPROCEDURE_LOAD_ACTION  0xC00113AEL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_SAINTERFACE_WINDOW_FAILED
//
// MessageText:
//
//  %1-%2@%3   Security Agent Interface window NOT Created %n %n %4%n
//  %nMeaning - System Restart (Fatal), cannot create the security interface.%n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_SAINTERFACE_WINDOW_FAILED 0xC00113AFL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_SAPROCEDURE_INIT_FAILED
//
// MessageText:
//
//  %1-%2@%3   Security Agent did Not Initialize %n %n %4%n
//  %nMeaning - Fatal"Stop Sign", cannot create the main security procedures.%n
//  %nAction - Run Get Diag files. Might temporarily turn on Security DataCapture for more logging.%n
//
#define SCOTAPP_SAPROCEDURE_INIT_FAILED  0xC00113B0L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_SAPROCEDURE_UNINIT
//
// MessageText:
//
//  %1-%2@%3   Security Agent did Not Un-Initialize %n %n %4%n
//  %nMeaning - Fatal"Stop Sign", failed to uninitialize security interface.%n
//  %nAction - Run Get Diag files. Might temporarily turn on Security DataCapture for more logging.%n
//
#define SCOTAPP_SAPROCEDURE_UNINIT       0xC00213B1L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_SAPROCEDURE_WINDOW_DESTROY
//
// MessageText:
//
//  %1-%2@%3   Security Agent Procedures window was not destroyed %n %n %4%n
//  %nMeaning - Fatal"Stop Sign", failed attempt to destroy security interface window.%n
//  %nAction - Run Get Diag files. Reboot NT.%n
//
#define SCOTAPP_SAPROCEDURE_WINDOW_DESTROY 0xC00213B2L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_TB_INIT
//
// MessageText:
//
//  %1-%2@%3   TB - RESTART %n %n %4%n
//  %nMeaning - Unable to initialize Transaction Broker, restarting ScotApp. %n
//  %nAction - Transaction Broker problem. Run Get Diag files.%n
//
#define SCOTAPP_TB_INIT                  0xC00113B3L

//-------------------------------------------------------------------------------------------------
// Called from 6 locations. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_CO_INIT
//
// MessageText:
//
//  %1-%2@%3   CO - RESTART %n %n %4%n
//  %nMeaning - Unable to load the Config Object option file OR Tender option file. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_CO_INIT                  0xC00113B4L

//-------------------------------------------------------------------------------------------------
// Called from 4 location. 
//
// MessageId: SCOTAPP_PS_INIT
//
// MessageText:
//
//  %1-%2@%3   PS - RESTART %n %n %4%n
//  %nMeaning - Unable to initialize Presentation Services, restarting ScotApp. %n
//  ;// Parameters at the right not used for these instance.
//  %nAction - Run Get Diag files. %n 
//
#define SCOTAPP_PS_INIT                  0xC00113B5L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_DM_INIT
//
// MessageText:
//
//  %1-%2@%3   DM - RESTART %n %n %4%n
//  %nMeaning - Unable to Initialize Device Manager, restarting ScotApp. %n
//  
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_DM_INIT                  0xC00113B6L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_SA_INIT
//
// MessageText:
//
//  %1-%2@%3   SA - RESTART %n %n %4%n
//  %nMeaning - Unable to Initialize Security Agent, restarting ScotApp. %n
//  %nAction - Run Get Diag files.
//
#define SCOTAPP_SA_INIT                  0xC00113B7L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_HT_INIT
//
// MessageText:
//
//  %1-%2@%3   HT - RESTART %n %n %4%n
//  %nMeaning - Unable to Initialize Hard Totals. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_HT_INIT                  0xC00113B8L

//-------------------------------------------------------------------------------------------------
// Called from 2 locations. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_UE_INIT
//
// MessageText:
//
//  %1-%2@%3   UE - RESTART %n %n %4
//  %nMeaning - User Exits Enabled but this user exit is invalid. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_UE_INIT                  0xC00113B9L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_GP_RESTART
//
// MessageText:
//
//  %1-%2@%3   GP - RESTART %n %n %4%n
//  %nMeaning - General Purpose Notify = RESTART, restarting ScotApp. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_GP_RESTART               0xC00213BAL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Latest Msg ID is used in the 1st parameter at the right
//
// MessageId: SCOTAPP_BS_UNKNOWN_MSG
//
// MessageText:
//
//  %1-%2@%3   Base State unknown msg ID: %4 %n %n %5
//  %nMeaning - Unexpected or unknown message event processed. %n
//  %nAction - None.%n
//
#define SCOTAPP_BS_UNKNOWN_MSG           0xC01213BBL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_BS_STACK_FULL
//
// MessageText:
//
//  %1-%2@%3   FIFO Stack is full, lost last state %n %n %4%n
//  %nMeaning - Base State stack is full, unable to push state onto stack. Deleting state. %n
//  %nAction - Run Get Diag files. Reboot NT.%n
//
#define SCOTAPP_BS_STACK_FULL            0xC00C13BCL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. File Name and Error Msg are used in 2 of the parameters
//
// MessageId: SCOTAPP_BS_JOURNAL
//
// MessageText:
//
//  %1-%2@%3   Journal File Exception, File handle: %4(invalid!!) %n %n Error text: %5 %n %6
//  %nMeaning - Base State file exception occurred while printing journal. %n
//  %nAction - Make sure the Journal File exists and is not corrupt%n
//
#define SCOTAPP_BS_JOURNAL               0xC01213BDL

//-------------------------------------------------------------------------------------------------
// Called from 2 locations. File and Error are used in 2 of the parameters at the right
//
// MessageId: SCOTAPP_BS_RECEIPT
//
// MessageText:
//
//  %1-%2@%3   Receipt File Exception, File handle: %4(invalid !!) %n %n Error text: %5 %n %6%n
//  %nMeaning - Base State file exception occurred while printing and saving receipt data. %n
//  %nAction - Make sure the Receipt File exists and is not corrupt%n
//
#define SCOTAPP_BS_RECEIPT               0xC01213BEL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Last Evt and Last Parm are used in 2 of the parameters at the right
//
// MessageId: SCOTAPP_SM_PS_UNKNOWN_MSG
//
// MessageText:
//
//  %1-%2@%3   PS unknown msg: %4,0x%5  %n %n %6
//  %nMeaning - Unknown message while processing user events. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_SM_PS_UNKNOWN_MSG        0xC00713BFL

//-------------------------------------------------------------------------------------------------
// Not Used. 
//
// MessageId: SCOTAPP_SM_SA_IN_CUSTOMER
//
// MessageText:
//
//  %1-%2@%3   Already in SA customer mode %n %n %4%n
//  %nMeaning - Already in customer mode and sent another customer mode event to security agent. %n
//  %nAction - none.%n
//
#define SCOTAPP_SM_SA_IN_CUSTOMER        0x401213C0L

//-------------------------------------------------------------------------------------------------
// Not Used. 
//XMLOFF
//
// MessageId: SCOTAPP_SM_SA_IN_STORE
//
// MessageText:
//
//  %1-%2@%3   Already in SA store mode %n %n %4%n
//  %nMeaning - Already in store mode and sent another store mode event to security agent. %n
//  %nAction - none.%n
//
#define SCOTAPP_SM_SA_IN_STORE           0x401213C1L

//-------------------------------------------------------------------------------------------------
// Not Used. 
//
// MessageId: SCOTAPP_SM_SA_NO_MODE
//
// MessageText:
//
//  %1-%2@%3   SA major mode not set, store/customer %n %n %4%n
//  %nMeaning - NOT in store mode and NOT in customer mode. %n
//  %nAction - Run Get Diag files. Might temporarily turn on Security DataCapture for more logging.%n
//
#define SCOTAPP_SM_SA_NO_MODE            0xC01213C2L

//-------------------------------------------------------------------------------------------------
// Not Used. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_SM_SA_MISS_ITEM_COMP
//
// MessageText:
//
//  %1-%2@%3   Missed SA itemization or tender before complete %n %n %4%n
//  %nMeaning - Error, if NOT in a transaction OR NOT tendering, but received a TRANSACTION_COMPLETE. %n
//  %nAction - Run Get Diag files. Might temporarily turn on Security DataCapture for more logging.%n
//
#define SCOTAPP_SM_SA_MISS_ITEM_COMP     0xC01213C3L

//-------------------------------------------------------------------------------------------------
// Not Used. Event InputID is used in the 1st parameter at the right
//
// MessageId: SCOTAPP_SM_SA_MISS_ITEM
//
// MessageText:
//
//  %1-%2@%3   Missed SA itemization before item entry - %4 %n %n %5%n
//  %nMeaning - NOT in Itemize, but received a UPC_SCANNED or UPC_MANUAL. %n
//  %nAction - Run Get Diag files. Might temporarily turn on Security DataCapture for more logging.%n
//
#define SCOTAPP_SM_SA_MISS_ITEM          0xC01213C4L

//-------------------------------------------------------------------------------------------------
// Not Used. 
//
// MessageId: SCOTAPP_SM_SA_MISS_ITEM_COMP_ID
//
// MessageText:
//
//  %1-%2@%3   Missed SA itemization before item complete - %4 %n %n %5%n
//  %nMeaning - NOT in Itemize, but received a COUPON _REDEEMED, _VOID, OR _REFUSED. %n
//  %nAction - Run Get Diag files. Might temporarily turn on Security DataCapture for more logging.%n
//
#define SCOTAPP_SM_SA_MISS_ITEM_COMP_ID  0xC01213C5L

//-------------------------------------------------------------------------------------------------
// Not Used.
//
// MessageId: SCOTAPP_SM_SA_MISS_ITEM_SALE_ID
//
// MessageText:
//
//  %1-%2@%3   Missed SA item sale before item complete - %4 %n %n %5%n
//  %nMeaning - NOT in Item Sale, but received a UPC _SCANNED, _MANUAL, ITEM_SOLD, ITEM_VOID or CANCEL, COUPON _REDEEMED, _VOID, or _REFUSED. %n
//  %nAction - Run Get Diag files. Might temporarily turn on Security DataCapture for more logging.%n
//
#define SCOTAPP_SM_SA_MISS_ITEM_SALE_ID  0xC01213C6L

//-------------------------------------------------------------------------------------------------
// Not Used.
//
// MessageId: SCOTAPP_SM_SA_MISS_TENDER
//
// MessageText:
//
//  %1-%2@%3   Missed SA tender before tender start %n %n %4%n
//  %nMeaning - NOT in Tender state, but customer begins paying. %n
//  %nAction - Run Get Diag files. Might temporarily turn on Security DataCapture for more logging.%n
//
#define SCOTAPP_SM_SA_MISS_TENDER        0xC01213C7L

//-------------------------------------------------------------------------------------------------
// Called from 5 locations. 
//
// MessageId: SCOTAPP_SM_SA_UNKNOWN_EVENT
//
// MessageText:
//
//  %1-%2@%3   Unknown SA event ID - %4 %n %n %5%n
//  %nMeaning - Error, default case when processing unknown security events OR unknown security maintenance mode events. %n
//  ;// the Event InputID is used in the 1st parameter at the right
//  ;// the Event InputID and the result code are used in 2 of the parameters at the right
//  %nAction - Run Get Diag files. Might temporarily turn on Security DataCapture for more logging.%n
//
#define SCOTAPP_SM_SA_UNKNOWN_EVENT      0xC00713C8L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//XMLOFF
//
// MessageId: SCOTAPP_SM_TB_USERID
//
// MessageText:
//
//  %1-%2@%3   Invalid user ID %n %n %4%n
//  %nMeaning - Transaction Broker returned TB_E_INVALIDUSERID in response to a TB API call. %n
//  %nAction - None. User typed in an invalid User ID%n
//
#define SCOTAPP_SM_TB_USERID             0x401213C9L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//XMLOFF
//
// MessageId: SCOTAPP_SM_TB_PASSWORD
//
// MessageText:
//
//  %1-%2@%3   Invalid password %n %n %4%n
//  %nMeaning - Transaction Broker returned TB_E_INVALIDUSERPASSWORD in response to a TB API call. %n
//  %nAction - None. User typed in an invalid Password%n
//
#define SCOTAPP_SM_TB_PASSWORD           0x401213CAL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_SM_TB_STATE
//
// MessageText:
//
//  %1-%2@%3   Invalid procedure to perform at this time %n %n %4%n
//  %nMeaning - Transaction Broker returned TB_E_INVALIDSTATE, 
//  %n         indicating that Scotapp issued a TB API call at a time 
//  %n         that the Transaction Broker could not execute it. %n
//  %nAction - Possible Transaction Broker problemTransaction broker was not in the correct state to execute an API call made by Scotapp.  Run Get Diag files.%n
//
#define SCOTAPP_SM_TB_STATE              0xC01213CBL

//-------------------------------------------------------------------------------------------------
// Filename is used in the 1st parameter at the right
//
// MessageId: SCOTAPP_SM_TB_FILE
//
// MessageText:
//
//  %1-%2@%3   File not found %n %n %4%n
//  %nMeaning - File exception while performing file io. %n
//  %nAction - make sure the File exists and is not corrupt%n
//
#define SCOTAPP_SM_TB_FILE               0xC00B13CCL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_SM_TB_PROC
//
// MessageText:
//
//  %1-%2@%3   Procedure not complete %n %n %4%n
//  %nMeaning - Transaction Broker returned TB_E_INCOMPLETEPROCEDURE in response to a TB API call, indicating that the API has not been implemented. %n
//  %nAction - Possible Transaction Broker problem. Run Get Diag files.%n
//
#define SCOTAPP_SM_TB_PROC               0xC01213CDL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_SM_TB_WINDOW
//
// MessageText:
//
//  %1-%2@%3   Invalid window handle passed %n %n %4%n
//  %nMeaning - Transaction Broker returned TB_E_INVALIDWINDOW in response to a TB API call. %n
//  %nAction - Possible Transaction Broker problem. Run Get Diag files.%n
//
#define SCOTAPP_SM_TB_WINDOW             0xC01213CEL

//-------------------------------------------------------------------------------------------------
// parameters at the right not used at this time.
//
// MessageId: SCOTAPP_SM_TB_DISK
//
// MessageText:
//
//  %1-%2@%3   Disk full %n %n %4%n
//  %nMeaning - Transaction Broker returned TB_E_DISKFULL in response to a TB API call. %n
//  %nAction - Possible hardware problem. Make Insure there is adequate free disk space Sure the hard disk is not full and Restart SCOT%n
//
#define SCOTAPP_SM_TB_DISK               0xC00C13CFL

//-------------------------------------------------------------------------------------------------
// Called from 4 locations. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_SM_TB_PARA
//
// MessageText:
//
//  %1-%2@%3   Invalid parameter(s) %n %n %4%n
//  %nMeaning - Transaction Broker returned TB_E_INVALIDPARAM in response to a TB API call. %n  
//  %nAction - Possible Transaction Broker problem. Run Get Diag files.%n
//
#define SCOTAPP_SM_TB_PARA               0xC00413D0L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_SM_TB_CREDIT
//
// MessageText:
//
//  %1-%2@%3   No host credit %n %n %4%n
//  %nMeaning - Transaction Broker returned TB_E_NOHOSTCREDIT in response to a TB API call. %n
//  %nAction - Possible Transaction Broker or Host communication problem. Run Get Diag files.%n
//
#define SCOTAPP_SM_TB_CREDIT             0xC00713D1L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_SM_TB_WORKINGKEY
//
// MessageText:
//
//  %1-%2@%3   No Working Key %n %n %4%n
//  %nMeaning - Transaction Broker returned TB_E_NOWORKINGKEY in response to a TB API call. 
//  %n          The TB was unable to provide a working key for the PINpad. %n
//  %nAction - Possible Transaction Broker problem. Run Get Diag files.%n
//
#define SCOTAPP_SM_TB_WORKINGKEY         0xC01213D2L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_SM_TB_NOTINIT
//
// MessageText:
//
//  %1-%2@%3   TB has not been successfully initialized %n %n %4%n
//  %nMeaning - Transaction Broker returned TB_E_NOTINITIALIZED in response to a TB API call OR when app tries to UnInitialize TB. The Transaction Broker was not initialized. %n
//  %nAction - Possible Transaction Broker problem. Run Get Diag files.%n
//
#define SCOTAPP_SM_TB_NOTINIT            0xC00213D3L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_SM_TB_HOST
//
// MessageText:
//
//  %1-%2@%3   No host application communicated %n %n %4%n
//  %nMeaning - Transaction Broker returned TB_E_NOHOST in response to a TB API call. The Transaction Broker cannot communicate with the POS application. %n
//  %nAction - Possible Transaction Broker or POS application problem. Run Get Diag files.%n
//
#define SCOTAPP_SM_TB_HOST               0xC01213D4L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_SM_TB_OTHER
//
// MessageText:
//
//  %1-%2@%3   Unexpected fatal error. %n %n %4%n
//  %nMeaning - Transaction Broker returned an unhandled return code (default case). The  Transaction Broker returned an unrecognized value. %n
//  %nAction - Possible Transaction Broker problem. Run Get Diag files.%n
//
#define SCOTAPP_SM_TB_OTHER              0xC01213D5L

//-------------------------------------------------------------------------------------------------
// Called from 2 locations. Parameters at the right not used at this time.
//XMLOFF
//
// MessageId: SCOTAPP_CARD_PURGED
//
// MessageText:
//
//  %1-%2@%3   Card purged - not removed by customer before timeout %n %n %4%n
//  %nMeaning - Card Acceptor (MSR) has stored credit/loyalty in purge bin within machine. %n
//  %nAction - Open lane and retrieve purged card from Card Reader purge bin.%n
//
#define SCOTAPP_CARD_PURGED              0x401213D6L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//XMLOFF
//
// MessageId: SCOTAPP_BILL_PURGED
//
// MessageText:
//
//  %1-%2@%3   Bills purged - not removed by customer before timeout %n %n %4%n
//  %nMeaning - Fastlane has retrieved currency after timeout. %n
//  %nAction - none.  Open ATM lane and remove bills.%n
//
#define SCOTAPP_BILL_PURGED              0x401213D7L

//-------------------------------------------------------------------------------------------------
// Not Used.
//
// MessageId: SCOTAPP_EO_FILE
//
// MessageText:
//
//  
//  %1-%2@%3   Error on file '%4', '%5' error '%6' (%7) %n %n %8
//
#define SCOTAPP_EO_FILE                  0xC00B13D8L

//-------------------------------------------------------------------------------------------------
// Called from 3 locations. Msg, Description, and Error are used in 3 of the parameters
//
// MessageId: SCOTAPP_DMINTERFACE
//
// MessageText:
//
//  %1-%2@%3   %4 catch(...)[%5] error = %6 %n %n %7%n
//  %nMeaning - Exception occurred in Device Manager Interface %n
//  %nAction - Possible hardware problem, make sure the device is plugged in and activated.%n
//
#define SCOTAPP_DMINTERFACE              0xC01013D9L

//-------------------------------------------------------------------------------------------------
// Called from 3 locations. Msg, Description, and Error are used in 3 of the parameters
//XMLOFF
//
// MessageId: SCOTAPP_DMINTERFACE2
//
// MessageText:
//
//  %1-%2@%3   %4 NOT Enabled @%5 %n %n %6%n
//  %nMeaning - Device Manager generated exception or IS_NOT_ENABLED_RETURN. %n
//  %nAction - Possible hardware problem, make sure the device is plugged in and activated.%n
//
#define SCOTAPP_DMINTERFACE2             0x400713DAL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Enabled status, Description, Device ID and Status are used in 4 of the parameters
//XMLOFF
//
// MessageId: SCOTAPP_DMINTERFACE_DEVICE
//
// MessageText:
//
//  %1-%2@%3   Unable to %4 %5 %6 %7 %n %n %8%n
//  %nMeaning - Device status return code is not equal to 1 or 0. %n
//  %nAction - Possible hardware problem, make sure the device is plugged in and activated.%n
//
#define SCOTAPP_DMINTERFACE_DEVICE       0xC01013DBL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_DMINTERFACE_DEVICE_CREATE
//
// MessageText:
//
//  %1-%2@%3   Failed to create DM control %n %n %4%n
//  %nMeaning - Fatal"Stop Sign", Failed to create Device Manager control. %n
//  %nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
//
#define SCOTAPP_DMINTERFACE_DEVICE_CREATE 0xC00113DCL

//-------------------------------------------------------------------------------------------------
// Called from 2 locations. Device name is used in the 1st parameter
//
// MessageId: SCOTAPP_DMINTERFACE_DEVICE_LOAD
//
// MessageText:
//
//  %1-%2@%3   Device %4 (%5) is not loaded, issuing CheckHealth %n %n %6%n
//  %nMeaning - Exception occurred in Device Manager Interface. %n
//  %nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
//
#define SCOTAPP_DMINTERFACE_DEVICE_LOAD  0xC00113DDL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Device ID is used in the 1st parameter
//
// MessageId: SCOTAPP_DMINTERFACE_DEVICE_AVAI
//
// MessageText:
//
//  %1-%2@%3   Failed to get device availability for device %4 %n %n %5%n
//  %nMeaning - Exception occurred in Device Manager Interface %n
//  %nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
//
#define SCOTAPP_DMINTERFACE_DEVICE_AVAI  0xC00113DEL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. The number of devices in error is used in the 1st parameter
//
// MessageId: SCOTAPP_DMINTERFACE_DEVICE_ERROR
//
// MessageText:
//
//  %1-%2@%3   Still %4 devices in error, Init failed %n %n %5%n
//  %nMeaning - Devices are in error during Device Manager Interface processing. %n
//  %nAction - Probable hardware problem, make sure the devices is plugged in and activated.%n
//
#define SCOTAPP_DMINTERFACE_DEVICE_ERROR 0xC00113DFL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. The number of devices in error is used in the 1st parameter
//
// MessageId: SCOTAPP_DMINTERFACE_DEVICE_ERROR2
//
// MessageText:
//
//  %1-%2@%3   A total of %4 devices were in error, re-checking %n %n %5%n
//  %nMeaning - Error, if any devices are in error during Device Manager Interface. %n
//  %nAction - Probable hardware problem, make sure the devices is plugged in and activated.%n
//
#define SCOTAPP_DMINTERFACE_DEVICE_ERROR2 0xC00113E0L

//-------------------------------------------------------------------------------------------------
// Called from 2 locations. Parameters at the right not used at this time.
//XMLOFF
//
// MessageId: SCOTAPP_DM_CHECK_HEALTH
//
// MessageText:
//
//  %1-%2@%3   DM check health required %n %n %4%n
//  %nMeaning - CASE_HEALTH_NOW and CASE_HEALTH_L8R while checking the health of a device. %n
//  %nAction - none.%n
//
#define SCOTAPP_DM_CHECK_HEALTH          0x400613E1L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_DM_CASH
//
// MessageText:
//
//  %1-%2@%3   Invalid Cash insert amount <%4> %n %n %5%n
//  %nMeaning - Cash inserted event showed cash inserted is < zero. %n
//  %nAction - Possible hardware problem, make sure the devices is plugged in and activated. Run Get Diag Files.%n
//
#define SCOTAPP_DM_CASH                  0xC01213E2L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_DM_COIN
//
// MessageText:
//
//  %1-%2@%3   Invalid Coin insert amount <%4> %n %n %5%n
//  %nMeaning - Coin inserted event shows amount inserted is < zero. %n
//  %nAction - Possible hardware problem, make sure the devices is plugged in and activated. Run Get Diag Files.%n
//
#define SCOTAPP_DM_COIN                  0xC01213E3L

//-------------------------------------------------------------------------------------------------
// Last Msg, Last Dev, Last Code and Last Length are used in 4 of the parameters
//XMLOFF
//
// MessageId: SCOTAPP_DM_CSWORK
//
// MessageText:
//
//  %1-%2@%3   DM Unsupported event %4, device %5, code %6 length %7 %n %n %8%n
//  %nMeaning - Scotapp received a device event it did not recognize. %n
//  %nAction - none.%n
//
#define SCOTAPP_DM_CSWORK                0x401213E4L

//-------------------------------------------------------------------------------------------------
// Called from 3 locations. Last Msg, Description, Error and Dev Class ID are used in 4 of the parameters
//
// MessageId: SCOTAPP_DMINTERFACE_OLE
//
// MessageText:
//
//  %1-%2@%3   %4 COleDispatchException[ %5 ] Codes(0x%6), device %7 (%8) %n %n %9%n
//  %nMeaning - Report Exception occured & the Device Manager is opened. %n
//  %nAction - Possible hardware problem, make sure the devices is plugged in and activated. Run Get Diag Files.%n
//
#define SCOTAPP_DMINTERFACE_OLE          0xC01013E5L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_PSITEMLIST_INSERT
//
// MessageText:
//
//  %1-%2@%3   Insert in order error %n %n %4%n
//  %nMeaning - Memory Exception occurred while sorting the item list. %n
//  %nAction - none.%n
//
#define SCOTAPP_PSITEMLIST_INSERT        0xC01213E6L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Last Msg, Description, Error and Dev Class ID are used in 4 of the parameters
//
// MessageId: SCOTAPP_DMINTERFACE_EXCEPTION
//
// MessageText:
//
//  %1-%2@%3   %4 CException[ %5 ], device %6 (%7) %n %n %8%n
//  %nMeaning - Fatal"Stop Sign", Ole exception occurred. %n
//  %nAction - Possible hardware problem, make sure the devices is plugged in and activated. Run Get Diag Files.
//
#define SCOTAPP_DMINTERFACE_EXCEPTION    0xC00713E7L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Last Msg is in 1st parameter at right
//
// MessageId: SCOTAPP_DM_MESSAGE
//
// MessageText:
//
//  %1-%2@%3   Invalid DM Message type %4 %n %n %5%n
//  %nMeaning - Invalid Device Manager message. %n
//  %nAction - Possible hardware problem, make sure the devices is plugged in and activated. Run Get Diag Files.
//
#define SCOTAPP_DM_MESSAGE               0xC00413E8L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_RA_INIT
//
// MessageText:
//
//  %1-%2@%3   RA initialize error %n %n %4%n
//  %nMeaning - Failed to initialize RAP at startup. %n
//  %nAction - Check the network connection to the RAP server. Make sure RAP installed properly.%n
//
#define SCOTAPP_RA_INIT                  0xC00113E9L

//-------------------------------------------------------------------------------------------------
// Called from 2 locations. Path and filename in 1st parameter at right
//XMLOFF
//
// MessageId: SCOTAPP_DM_EVENT
//
// MessageText:
//
//  %1-%2@%3   DM event occurred: %n %n %4%n
//  %nMeaning - Unhandled device event. %n
//  %nAction - none%n
//
#define SCOTAPP_DM_EVENT                 0x400313EAL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_PREV_INSTANCE
//
// MessageText:
//
//  %1-%2@%3   Previous instance of SCOT Application exists.%n%n
//  %nMeaning: SCOT Application wasn't shutdown properly before. Previous instance of application still exists.%n
//  %nAction:  Kill previous instance of SCOT Application and reboot before you start application.%n%n%4%n
//  
//
#define SCOTAPP_PREV_INSTANCE            0xC00213EBL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//XMLOFF
//
// MessageId: SCOTAPP_STOP_REQUEST_FROM_DIALOG
//
// MessageText:
//
//  %1-%2@%3   SCOTAPP Dialog sent a request to stop Fastlane. %4%n
//  %nMeaning - The Fastlane application was shut down using ScotAppDialog. %n
//  %nAction - none%n
//
#define SCOTAPP_STOP_REQUEST_FROM_DIALOG 0x400313ECL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//XMLOFF
//
// MessageId: SCOTAPP_STOP_REQUEST_FROM_LOADCONTROLLER
//
// MessageText:
//
//  %1-%2@%3   Load Controller sent  a request to stop Fastlane. %4%n
//  %nMeaning - The load controller program sent a shut down message to Fastlane app. %n
//  %nAction - none%n
//
#define SCOTAPP_STOP_REQUEST_FROM_LOADCONTROLLER 0x400313EDL

//-------------------------------------------------------------------------------------------------
// Called from 3 locations. Path and filename in 1st parameter at right
//
// MessageId: SCOTAPP_NULL_RECEIPT
//
// MessageText:
//
//  %1-%2@%3   SCOT App got no receipt data. File handle from TBGetReceiptData(): %4, File length:%5.%n %6%n
//  %nMeaning - No receipt data recieved for printing. %n
//  %nAction - make sure the file and is not corrupt%n 
//
#define SCOTAPP_NULL_RECEIPT             0x400713EEL

//-------------------------------------------------------------------------------------------------
// Called from 4 locations. The Transaction Broker message and error message are used in the 1st two parameters at right
//
// MessageId: SCOTAPP_GOT_TBEXCEPTION
//
// MessageText:
//
//  %1-%2@%3   SCOT App got an exception from TransactionBroker: %4, ErrorMessage:%5 %n %6 %n
//  %nMeaning - Transaction Broker Exception when trying to restart the app. %n
//  %nAction - Possible Transaction Broker problem. Run Get Diag files.%n
//
#define SCOTAPP_GOT_TBEXCEPTION          0xC01213EFL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Total sale amt and if item is waiting are used in the 1st two parameters at right
//
// MessageId: SCOTAPP_GOT_SAFALSETRXSTART
//
// MessageText:
//
//  %1-%2@%3   'False Transaction Start' from Security Agent: Trx total:%4, Pending items:%5 %n %6 %n
//  %nMeaning - Fastlane received EndOfTransaction from Security agent while still in a transaction. %n
//  %nAction - None. %n
//
#define SCOTAPP_GOT_SAFALSETRXSTART      0x801213F0L

//-------------------------------------------------------------------------------------------------
// Called 5 times.  The config/opt file not found/opened is printed in the 1st parameter
//
// MessageId: SCOTAPP_MISS_FILE
//
// MessageText:
//
//  %1-%2@%3   %4 was missing.%n
//  %nMeaning - Fatal"Stop Sign", if unable to find/open one of the configuration or options files. App immediately stops. %n
//  %nAction - Check that the file is installed and not corrupted%n
//
#define SCOTAPP_MISS_FILE                0xC01213F1L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. The option name is used in one of the parameters.
//
// MessageId: SCOTAPP_DUAL_LANGUAGE_BAD_SETUP
//
// MessageText:
//
//  %1-%2@%3   Missing or incorrect option [%4] %5, disabling Dual Language feature. Option should be of the format %5=2 character ISO 639 code, Button name, message file name, tare description file name, security conclusions file%n
//  %nMeaning - DualLanguage=Y but scotopts.xxx does not define both lanquages, or Language code is not 2 characters in length. %n
//  %nAction - make sure language configuration is correct %n
//
#define SCOTAPP_DUAL_LANGUAGE_BAD_SETUP  0xC00313F2L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_DF_INIT
//
// MessageText:
//
//  %1-%2@%3   SCOT Device Factory initialize error %n %n %4%n
//  %nMeaning - Unable to Initialize the SCOT Device Factory. %n
//  %nAction - Probable hardware problem, make sure the devices are plugged in and activated.%n
//
#define SCOTAPP_DF_INIT                  0xC00113F3L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_SE_INIT
//
// MessageText:
//
//  %1-%2@%3   SCOT Encryptor initialize error %n %n %4%n
//  %nMeaning - Unable to Initialize SCOT Encryptor with SCOT Device Factory. %n
//  %nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
//
#define SCOTAPP_SE_INIT                  0xC00113F4L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_SC_INIT
//
// MessageText:
//
//  %1-%2@%3   SCOT Signature Capture initialize error %n %n %4%n
//  %nMeaning - Unable to Initialize SCOT Signature Capture with SCOT Device Factory. %n
//  %nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
//
#define SCOTAPP_SC_INIT                  0xC00113F5L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_AD_INIT
//
// MessageText:
//
//  %1-%2@%3   SCOT Aux Display initialize error %n %n %4%n
//  %nMeaning - Unable to Initialize SCOT Auxilliary Display with SCOT Device Factory. %n
//  %nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
//
#define SCOTAPP_AD_INIT                  0xC00113F6L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//XMLOFF
//
// MessageId: SCOTAPP_PAGER_INITIALIZED
//
// MessageText:
//
//  %1-%2@%3   SCOT Paging System is initialized %n %n %4%n
//  %nMeaning - The pager service initialized without error. %n
//  %nAction - none.%n
//
#define SCOTAPP_PAGER_INITIALIZED        0x400613F7L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Sent status & message are used in the 1st 2 parameters at right.
//XMLOFF
//
// MessageId: SCOTAPP_SEND_PAGE
//
// MessageText:
//
//  %1-%2@%3   SCOT Paging System is sending a page.%nPage sent successfully: %4%nMessage: %5%n %n %6%n
//  %nMeaning - The pager service successfully sent a page. %n
//  %nAction - none.%n
//
#define SCOTAPP_SEND_PAGE                0x400613F8L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. the handle to Dev Factory window & Parent window are used in the 1st 2 parameters at right.
//
// MessageId: SCOTAPP_DF_NO_WND
//
// MessageText:
//
//  %1-%2@%3   Unable to create Device Factory window (ID=%4).%nThe parent window ptr is %5.%n %6%n
//  %nMeaning - Device Factory Initialize cannot create a window. %n
//  %nAction - Reboot NT and Run Get Diag files.%n
//
#define SCOTAPP_DF_NO_WND                0xC00313F9L

//-------------------------------------------------------------------------------------------------
// Called 7 times. The access action & key value are used in the 1st 2 parameters at right.
//
// MessageId: SCOTAPP_REG_ACCESS_ERROR
//
// MessageText:
//
//  %1-%2@%3   Got error %4 %5 registry key %6.%n%n %7%n
//  %nMeaning - Error, if an error occurs while accessing a registry value. %n
//  %nAction - Check that the registry values are not corrupt%n
//
#define SCOTAPP_REG_ACCESS_ERROR         0xC00313FAL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. The device class is used in the 1st parameter.
//
// MessageId: SCOTAPP_DF_NO_DEV_FOUND
//
// MessageText:
//
//  %1-%2@%3   No device was found where 'Configure=Yes' for device class %4.%n%n %5%n
//  %nMeaning - Device not found but the device class exists. %n
//  %nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
//
#define SCOTAPP_DF_NO_DEV_FOUND          0xC00313FBL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. The device class & device status are used in the 1st 2 parameters.
//
// MessageId: SCOTAPP_DF_UNKNOWN_DATA_EVENT
//
// MessageText:
//
//  %1-%2@%3   Received data event for Unknown device (Device Class %4), Status = %5.%n%n %6%n
//  %nMeaning - Unknown Device sent an event message to the app. %n
//  %nAction - Probable hardware or configuration problem.%n
//
#define SCOTAPP_DF_UNKNOWN_DATA_EVENT    0xC01213FCL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. The device & result code are used in the 1st 2 parameters on the right.
//
// MessageId: SCOTAPP_DF_ERROR_EVENT
//
// MessageText:
//
//  %1-%2@%3   Received error event for %4 - result code = %5, extended result code = %6.%n%n %7%n
//  %nMeaning - The device sent an error message. %n
//  %nAction - Probable hardware problem.%n
//
#define SCOTAPP_DF_ERROR_EVENT           0xC01213FDL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Language is used in the 1st parameter on the right
//
// MessageId: SCOTAPP_AD_INVALID_LANG
//
// MessageText:
//
//  %1-%2@%3   SetLanguage() received non-Primary language code %4, but dual-language is not 
//  configured.%n%n %5%n
//  %nMeaning - Dual language not configured, but a SetLanguage call was made with a language 
//  code that does not match the primary language. %n
//  %nAction - Software issue.  Get Diag files%n
//
#define SCOTAPP_AD_INVALID_LANG          0xC00713FEL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. The primary language & current language are used in the 1st 2 parameters on the right.
//
// MessageId: SCOTAPP_AD_LANG_NOT_FOUND
//
// MessageText:
//
//  %1-%2@%3   Language code '%4' not found in language array, using default of '%5'.%n%n %6%n
//  %nMeaning - The auxilliary device was instructed to set the language to an undefined language code. %n
//  %nAction - make sure language code is correct in SCOTopts%n
//
#define SCOTAPP_AD_LANG_NOT_FOUND        0xC00313FFL

//-------------------------------------------------------------------------------------------------
// Called 3 times. The screen id is used in the 1st parameter.
//
// MessageId: SCOTAPP_AD_CURRENT_SCREEN_NULL
//
// MessageText:
//
//  %1-%2@%3   %4 failed because m_pCurrentScreen is NULL.%n%n %5%n
//  %nMeaning - The auxilliary device or IVI forms received a null pointer for screen display. %n
//  %nAction - Software Problem. Get Diag Files.%n
//
#define SCOTAPP_AD_CURRENT_SCREEN_NULL   0xC0121400L

//-------------------------------------------------------------------------------------------------
// Called 8 times. device information is used in the parameter to the right
//
// MessageId: SCOTAPP_OPOS_CTRL_INIT_FAILURE
//
// MessageText:
//
//  %1-%2@%3   Fastlane Device failed to initialize %4. The following function failed: %5.%n%n %6%n
//  %nMeaning - A device factory device failed to initialize. %n
//  %nAction - Probable hardware problem, make sure the devices are plugged in and activated.%n
//
#define SCOTAPP_OPOS_CTRL_INIT_FAILURE   0xC0101401L

//-------------------------------------------------------------------------------------------------
// Called from 2 locations. The data attempted to store, the array index, and the memory return code are used in 3 of the parameters
//
// MessageId: SCOTAPP_AD_FORM_LOAD_FAILURE
//
// MessageText:
//
//  %1-%2@%3   Form %4 (slot %5) failed to load - rc = %6.%n%n %7%n
//  %nMeaning - Unable to store data on the auxilliary device. %n
//  %nAction - Probable hardware problem, make sure the devices are plugged in and activated.%n
//
#define SCOTAPP_AD_FORM_LOAD_FAILURE     0xC0011402L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Device status is used in the 1st parameter at the right
//
// MessageId: SCOTAPP_AD_RESET_DEVICE_FAILURE
//
// MessageText:
//
//  %1-%2@%3   Forms device failed to reset - bAlive = %4.%n%n %5%n
//  %nMeaning - Check health failed to reset device. %n
//  %nAction - Probable hardware problem, make sure the devices are plugged in and activated.%n
//
#define SCOTAPP_AD_RESET_DEVICE_FAILURE  0xC0011403L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Device profile is used in the 1st parameter at the right
//
// MessageId: SCOTAPP_AD_LOAD_CFGFILE_FAILURE
//
// MessageText:
//
//  %1-%2@%3   Could not open config file %4.%n%n %5%n
//  %nMeaning - Unable to load forms config file. %n
//  %nAction - Probable hardware problem, make sure the devices are plugged in and activated.%n
//
#define SCOTAPP_AD_LOAD_CFGFILE_FAILURE  0xC0031404L

//-------------------------------------------------------------------------------------------------
// Called from 2 locations. form profile & screen number are used in the 1st 2 parameters at the right
//
// MessageId: SCOTAPP_AD_SCREEN_CREATE_ERROR
//
// MessageText:
//
//  %1-%2@%3   AuxScreen %4 (%5) failed to create/initialize.%n
//  If the EFT device is an NCR 5992 Signature Capture device, after correcting 
//  the problem, set the registry value FormsForceOneReload=Yes in key
//  HKLM\Software\NCR\SCOT\CurrentVersion\SCOTAPP. This
//  will cause the forms to be reloaded next time the application is reloaded.%n %6%n
//  %nMeaning - Aux Screen = NULL while reading in forms. %n
//  %nAction - Probable hardware problem, make sure the devices are plugged in and activated.%n
//
#define SCOTAPP_AD_SCREEN_CREATE_ERROR   0xC0101405L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. The form name, screen num, language, and cfgFile are used in 4 of the parameters.
//
// MessageId: SCOTAPP_AD_SCREEN_FILE_NOT_EXIST
//
// MessageText:
//
//  %1-%2@%3   Form file %4 (Number %5) in language section %6 does not exist
//  in config file %7.%n
//  After correcting the problem, set the registry value FormsForceOneReload=Yes in key
//  HKLM\Software\NCR\SCOT\CurrentVersion\SCOTAPP. This
//  will cause the forms to be reloaded next time the application is reloaded.%n %n %8%n
//  %nMeaning - Unable to get the status of a screen form file. %n
//  %nAction - make sure the auxilliary display screen files are installed and not corrupt.%n
//
#define SCOTAPP_AD_SCREEN_FILE_NOT_EXIST 0xC0031406L

//-------------------------------------------------------------------------------------------------
// Called from 2 locations. The screen num, language, and cfgFile are used in 3 of the parameters.
//
// MessageId: SCOTAPP_AD_SCREEN_EMPTY_ENTRY
//
// MessageText:
//
//  %1-%2@%3   Entry number %4 in language section %5 does not exist
//  in config file %6.%n
//  If the EFT device is an NCR 5992 Signature Capture device, after correcting 
//  the problem, set the registry value FormsForceOneReload=Yes in key
//  HKLM\Software\NCR\SCOT\CurrentVersion\SCOTAPP. This
//  will cause the forms to be reloaded next time the application is reloaded.%n %7%n
//  %nMeaning - The entry is empty in a screen form file. %n
//  %nAction - Make sure the auxilliary display screen files are installed and not corrupt.%n
//  
//
#define SCOTAPP_AD_SCREEN_EMPTY_ENTRY    0xC0031407L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Status, Form File, Screen are used in 3 of the parameters at the right
//
// MessageId: SCOTAPP_AD_SCREEN_UNHANDLED_CONTROL_DATA
//
// MessageText:
//
//  %1-%2@%3   Unhandled data event for control ID %4, form %5 (%6).%n%n %7%n
//  %nMeaning - The data event status id is out of range. %n
//  %nAction - Software Problem. Run Get Diag Files.%n
//
#define SCOTAPP_AD_SCREEN_UNHANDLED_CONTROL_DATA 0xC0121408L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Pointer to the Text and the Formatted Text are used in the 1st 2 parameters at right.
//
// MessageId: SCOTAPP_AD_VARTEXT_FORMAT_ERROR
//
// MessageText:
//
//  %1-%2@%3   Unable to Format variable data %4 in specifier %5.%n%n %6%n
//  %nMeaning - Caught an exception while formatting text for auxilliary screen, most likely it is because text is invalid. %n
//  %nAction - Run Get Diag files %n
//
#define SCOTAPP_AD_VARTEXT_FORMAT_ERROR  0xC0121409L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. FormFile & TextVal are used in the 1st 2 parameters at right.
//
// MessageId: SCOTAPP_AD_VARTEXT_VALIDATE_ERROR
//
// MessageText:
//
//  %1-%2@%3   Unable to validate that variable data form %4 format-specifier %5 
//  contains one and only one string format specifier.%n%n %6%n
//  %nMeaning - The variable text is invalid. %n
//  %nAction - Run Get Diag files %n
//
#define SCOTAPP_AD_VARTEXT_VALIDATE_ERROR 0xC012140AL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Form File is used in the 1st parameter
//
// MessageId: SCOTAPP_AD_VARTEXT_PARSE_OPEN_ERROR
//
// MessageText:
//
//  %1-%2@%3   ParseForm() unable to open variable data form %4.%n%n %5%n
//  %nMeaning - Error during open of variable data form after successful validation of form data. %n
//  %nAction - Run Get Diag files %n
//
#define SCOTAPP_AD_VARTEXT_PARSE_OPEN_ERROR 0xC012140BL

//------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters identify the failure information
//
// MessageId: SCOTAPP_SE_PIN_TRANS_FAILED
//
// MessageText:
//
//  %1-%2@%3   DoPINTransaction failed in %4, rc = %5, rce = %6.%n
//  Acct#=%7, Working Key=%8, PinBlockFormat=%9, EncryptionType=%10, Slot=%11.%n %12%n
//  %nMeaning - PIN transaction failed. %n
//  %nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
//
#define SCOTAPP_SE_PIN_TRANS_FAILED      0xC006140CL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Prompt Text, return code, OPOS Prompt Num, IO Prompt Num are used in 4 of the parameters
//
// MessageId: SCOTAPP_SE_SET_PROMPT_FAILED
//
// MessageText:
//
//  %1-%2@%3   SetPromptText of '%4' failed, rc=%5. OPOS Prompt#=%6, DirectIO Prompt#=%7.%n%n %8%n
//  %nMeaning - Unable to set encryptor prompt text thru direct IO. %n
//  %nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
//
#define SCOTAPP_SE_SET_PROMPT_FAILED     0xC006140DL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Calling function, return code and capture form are used in 3 of the parameters
//
// MessageId: SCOTAPP_SC_BEGCAP_FAILED
//
// MessageText:
//
//  %1-%2@%3   Start Capture failed in function '%4' - rc=%5. SigCap form name was %6.%n%n %7%n
//  %nMeaning - Failure occurred when beginning signature capture. %n
//  %nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
//
#define SCOTAPP_SC_BEGCAP_FAILED         0xC006140EL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_SC_DISPWND_NULL
//
// MessageText:
//
//  %1-%2@%3   The SigCap display window handle was NULL.%n%n %7%n
//  %nMeaning - Invalid SigCap display window handle was passed. %n
//  %nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
//
#define SCOTAPP_SC_DISPWND_NULL          0xC006140FL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Prompt number and Prompt information used in 2 parameters.
//
// MessageId: SCOTAPP_AD_OPOSPROMPT_OUTOFRANGE
//
// MessageText:
//
//  %1-%2@%3   The OPOS prompt number %4 for prompt %5 was out of range.%n%n %6%n
//  %nMeaning - The auxilliary display prompt number is zero or greater than the maximum value. %n
//  %nAction -Run Get Diag files%n
//
#define SCOTAPP_AD_OPOSPROMPT_OUTOFRANGE 0xC0031410L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_SM_TB_FUNCTION_NOT_SUPPORTED
//
// MessageText:
//
//  %1-%2@%3   Transaction Broker does not support the function %4 %n %n %5%n
//  %nMeaning - Transaction Broker returned TB_E_FUNCTIONNOTSUPPORTED. %n
//  %nAction - Run Get Diag files%n
//
#define SCOTAPP_SM_TB_FUNCTION_NOT_SUPPORTED 0xC0061411L

//-------------------------------------------------------------------------------------------------
// Called from 2 locations. Signature array index used in 1st parameter
//
// MessageId: SCOTAPP_SC_DATA_MISSING
//
// MessageText:
//
//  %1-%2@%3   %4 Data is missing from signature receipt file. %n  Receipt Index: %5 %n %n %6%n
//  %nMeaning - Signature data missing during SLIP TOKEN or SIG TOKEN. %n
//  %nAction - make sure the signature file is not corrupt%n
//
#define SCOTAPP_SC_DATA_MISSING          0xC0061412L

//-------------------------------------------------------------------------------------------------
// Called from 2 locations. Signature array index used in 1st parameter
// Called from 1 location. Return code used in the 1st parameter
//
// MessageId: SCOTAPP_SC_ENDCAP_FAILED
//
// MessageText:
//
//  %1-%2@%3   End Capture failed  - rc=%4.%n%n %5%n
//  %nMeaning - Failure while ending signature capture. %n
//  %nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
//
#define SCOTAPP_SC_ENDCAP_FAILED         0xC0061413L

//-------------------------------------------------------------------------------------------------
// Called from 2 locations. Display Coin Dispenser Status used in parameters
//XMLOFF
//
// MessageId: SCOTAPP_DM_INFO
//
// MessageText:
//
//  %1-%2@%3   <%4> %n %n 
//  %nMeaning - Coin dispenser status after refill while in Store Mode/Lane Status screen, or status after System Message Screen. %n
//  %nAction - Make sure the coins are really refilled!%n
//
#define SCOTAPP_DM_INFO                  0x40101414L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_TB_NOTALLOWED_INTRAINING
//
// MessageText:
//
//  %1-%2@%3 Transaction Broker call, TBIsAllowed(TB_FT_TRAININGMODE) returned false while inside training mode.%n
//  SCOTApp ignores this return value and allows its user to press 'Leave Training Mode' button.%n
//  %nMeaning - In training mode and TB doesn't allow it. %n
//  %nAction - Transaction Broker should recognize and allow Training Mode%n
//
#define SCOTAPP_TB_NOTALLOWED_INTRAINING 0x80061415L

//-------------------------------------------------------------------------------------------------
// Called from 2 locations. Journal file name and length are used in 1st 2 parameters
//
// MessageId: SCOTAPP_NULL_JOURNALDATA
//
// MessageText:
//
//  %1-%2@%3   No journal data. File handle from TBGetJournalData(): %4, File length:%5.%n %6%n
//  %nMeaning - File has a valid file handle but a zero length. %n
//  %nAction - Make sure the journal file is not corrupt%n 
//  
//
#define SCOTAPP_NULL_JOURNALDATA         0x400B1416L

//-------------------------------------------------------------------------------------------------
// Called from 2 locations. the value of the option: ForceWeighingOFPriceEmbedded is used in the 1st parameter
//
// MessageId: SCOTAPP_FORCED_TO_OVERRIDE_OPTION
//
// MessageText:
//
//  %1-%2@%3   Fastlane forced to override an option: \%4\ %n %n %5%n
//  %nMeaning - Configuration specifies ForceWeighingOFPriceEmbedded=Y but there is no scale, overriding with ForceWeighingOFPriceEmbedded=N. %n
//  %nAction - If the scanner does not have a scale, then the SCOTOpts option ForceWeighingOFPriceEmbedded should be set to N%n
//
#define SCOTAPP_FORCED_TO_OVERRIDE_OPTION 0x40031417L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Device status id used in the 1st parameter
//
// MessageId: SCOTAPP_DEVFACTORY_MOTION_SENSOR_FAILED
//
// MessageText:
//
//  %1-%2@%3   SCOT forced to simulate motion begin and ended due to Motion Sensor Failure: %n %n %5%n
//  %nMeaning - Motion Sensor failure event received. %n
//  %nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
//
#define SCOTAPP_DEVFACTORY_MOTION_SENSOR_FAILED 0xC0101418L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Security System status id used in the 1st parameter
//
// MessageId: SCOTAPP_SM_SA_STATUS_EVENT
//
// MessageText:
//
//  %1-%2@%3   Security Agent reported a status event of: %n %4%n
//  %nMeaning - Security sent a status update event. Not necessarily failure. %n
//  %nAction - If no failure - none.  If failure, check the network connection to the security server.%n
//
#define SCOTAPP_SM_SA_STATUS_EVENT       0xC0121419L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_MS_INIT
//
// MessageText:
//
//  %1-%2@%3   SCOT Motion Sensor initialize error %n %n %4%n
//  %nMeaning - Failure while initializing Motion Sensor device. %n
//  %nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
//
#define SCOTAPP_MS_INIT                  0xC001141AL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_ED_INIT
//
// MessageText:
//
//  %1-%2@%3   SCOT EAS Deactivator initialize error %n %n %4%n
//  %nMeaning - Failure while initializing Electronic Article Surveillance Deactivator device. %n
//  %nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
//
#define SCOTAPP_ED_INIT                  0xC001141BL

//-------------------------------------------------------------------------------------------------
// Called from 1 locations. Section, Key, Default text messages are used in 1st 3 parameters
//
// MessageId: SCOTAPP_INVALID_CURRENCY_COUNTRY_CODE
//
// MessageText:
//
//  %1-%2@%3   Invalid registry value [%4] %5, using default <%6> %n %n %7%n
//  %nMeaning - Currency country code is invalid, default currency USD bitmap will be used %n
//  %nAction - if the default value is not desired, set the option.%n
//
#define SCOTAPP_INVALID_CURRENCY_COUNTRY_CODE 0x4003141CL

//-------------------------------------------------------------------------------------------------
//
// MessageId: SCOTAPP_DBMPROCEDURE_MAIN_WINDOW_FAILED
//
// MessageText:
//
//  %1-%2@%3   Client Database Manager Procedures main window NOT Created %n %n %4%n
//  %nMeaning - System Restart (Fatal), cannot create the main security procedures handle to a window. %n
//  ;// Called from 1 location. Parameters at the right not used at this time.
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_DBMPROCEDURE_MAIN_WINDOW_FAILED 0xC012141DL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_DBMPROCEDURE_WINDOW_FAILED
//
// MessageText:
//
//  %1-%2@%3   Client Database Manager Interface window NOT Created %n %n %4%n
//  %nMeaning - System Restart (Fatal), cannot create the security interface handle to a window. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_DBMPROCEDURE_WINDOW_FAILED 0xC001141EL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_DBMPROCEDURE_INIT_FAILED
//
// MessageText:
//
//  %1-%2@%3   Security Agent did Not Initialize %n %n %4%n
//  %nMeaning - Fatal"Stop Sign", cannot create the main security procedures. %n
//  %nAction - Run Get Diag files. Might temporarily turn on Security DataCapture for more logging.%n
//
#define SCOTAPP_DBMPROCEDURE_INIT_FAILED 0xC001141FL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_DBMINTERFACE_WINDOW_FAILED
//
// MessageText:
//
//  %1-%2@%3   Client Database Manager Interface window NOT Created %n %n %4%n
//  %nMeaning - System Restart (Fatal), cannot create the security interface. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_DBMINTERFACE_WINDOW_FAILED 0xC0011420L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_DBMPROCEDURE_UNINIT
//
// MessageText:
//
//  %1-%2@%3   Client Database Manager did Not Un-Initialize %n %n %4%n
//  %nMeaning - Fatal"Stop Sign", failed to un-init security interface. %n
//  %nAction - Run Get Diag files. Might temporarily turn on Security DataCapture for more logging.%n
//
#define SCOTAPP_DBMPROCEDURE_UNINIT      0xC0021421L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_DBMPROCEDURE_WINDOW_DESTROY
//
// MessageText:
//
//  %1-%2@%3   Client Database Manager Procedures window was not destroyed %n %n %4%n
//  %nMeaning - Fatal"Stop Sign", failed to DestroyWindow security interface. %n
//  %nAction - Run Get Diag files. Reboot NT.%n
//
#define SCOTAPP_DBMPROCEDURE_WINDOW_DESTROY 0xC0021422L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_SSPROCEDURE_MAIN_WINDOW_FAILED
//
// MessageText:
//
//  %1-%2@%3   Smart Scale Procedures main window NOT Created %n %n %4%n
//  %nMeaning - System Restart (Fatal), if cannot create the main security procedures handle to a window. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_SSPROCEDURE_MAIN_WINDOW_FAILED 0xC0121423L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_SSPROCEDURE_WINDOW_FAILED
//
// MessageText:
//
//  %1-%2@%3    Smart Scale Interface window NOT Created %n %n %4%n
//  %nMeaning - System Restart (Fatal), cannot create the security interface handle to a window. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_SSPROCEDURE_WINDOW_FAILED 0xC0011424L

//-------------------------------------------------------------------------------------------------
// Not Used.
//
// MessageId: SCOTAPP_SSPROCEDURE_INIT_FAILED
//
// MessageText:
//
//  %1-%2@%3   Smart Scale did Not Initialize %n %n %4%n
//  %nMeaning - Fatal"Stop Sign", cannot create the main security procedures. %n
//  %nAction - Run Get Diag files. Might temporarily turn on Security DataCapture for more logging.%n
//
#define SCOTAPP_SSPROCEDURE_INIT_FAILED  0xC0011425L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_SSINTERFACE_WINDOW_FAILED
//
// MessageText:
//
//  %1-%2@%3   Smart Scale Interface window NOT Created %n %n %4%n
//  %nMeaning - System Restart (Fatal), cannot create the security interface. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_SSINTERFACE_WINDOW_FAILED 0xC0011426L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_SSPROCEDURE_UNINIT
//
// MessageText:
//
//  %1-%2@%3   Smart Scale did Not Un-Initialize %n %n %4%n
//  %nMeaning - Fatal"Stop Sign", failed to un-init security interface. %n
//  %nAction - Run Get Diag files. Might temporarily turn on Security DataCapture for more logging.%n
//
#define SCOTAPP_SSPROCEDURE_UNINIT       0xC0021427L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_SSPROCEDURE_WINDOW_DESTROY
//
// MessageText:
//
//  %1-%2@%3   Smart Scale Procedures window was not destroyed %n %n %4%n
//  %nMeaning - Fatal"Stop Sign", failed to DestroyWindow security interface. %n
//  %nAction - Run Get Diag files. Reboot NT.%n
//
#define SCOTAPP_SSPROCEDURE_WINDOW_DESTROY 0xC0021428L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_WTINFOPROCEDURE_INIT_FAILED
//
// MessageText:
//
//  %1-%2@%3   Weight Infomation dll did Not Initialize %n %n %4%n
//  %nMeaning - Fatal"Stop Sign", cannot create the main security procedures. %n
//  %nAction - Run Get Diag files. Might temporarily turn on Security DataCapture for more logging.%n
//
#define SCOTAPP_WTINFOPROCEDURE_INIT_FAILED 0xC0011429L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Msg, Description, and Error are used in 3 of the parameters
//
// MessageId: SCOTAPP_SSINTERFACE_OLEDISPATCH_EXCEPTION
//
// MessageText:
//
//  %1-%2@%3   %4 COleDispatchException[ %5 ] Code(0x%6) %n %n %7%n
//  %nMeaning - COleDispatchException caught while interfacing with security. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_SSINTERFACE_OLEDISPATCH_EXCEPTION 0xC007142AL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Msg, Description, and Error are used in 3 of the parameters
//
// MessageId: SCOTAPP_SSINTERFACE_OLE_EXCEPTION
//
// MessageText:
//
//  %1-%2@%3   %4 OleException Error[ %5 ] Code(0x%6) %n %n %7%n
//  %nMeaning - OleException caught while interfacing with security. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_SSINTERFACE_OLE_EXCEPTION 0xC007142BL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Msg and Description are used in 2 of the parameters
//
// MessageId: SCOTAPP_SSINTERFACE_C_EXCEPTION
//
// MessageText:
//
//  %1-%2@%3   %4 CException Error[ %5 ] %n %n %6%n
//  %nMeaning - CException caught while interfacing with security. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_SSINTERFACE_C_EXCEPTION  0xC007142CL

//-------------------------------------------------------------------------------------------------
// Called from 5 locations. Msg and Error are used in 2 of the parameters at the right
//
// MessageId: SCOTAPP_SSINTERFACE_EXCEPTION
//
// MessageText:
//
//  %1-%2@%3   %4 catch(...) error = %5 %n %n %6%n
//  %nMeaning - Exception caught while interfacing with security. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_SSINTERFACE_EXCEPTION    0xC007142DL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Msg, Description, and Error are used in 3 of the parameters
//
// MessageId: SCOTAPP_DBMINTERFACE_OLEDISPATCH_EXCEPTION
//
// MessageText:
//
//  %1-%2@%3   %4 COleDispatchException[ %5 ] Code(0x%6) %n %n %7%n
//  %nMeaning - COleDispatchException caught while interfacing with security database. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_DBMINTERFACE_OLEDISPATCH_EXCEPTION 0xC007142EL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Msg, Description, and Error are used in 3 of the parameters
//
// MessageId: SCOTAPP_DBMINTERFACE_OLE_EXCEPTION
//
// MessageText:
//
//  %1-%2@%3   %4 COleException Error[ %5 ] Code(0x%6) %n %n %7%n
//  %nMeaning - COleException caught while interfacing with security database. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_DBMINTERFACE_OLE_EXCEPTION 0xC007142FL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Msg and Description are used in 2 of the parameters
//
// MessageId: SCOTAPP_DBMINTERFACE_C_EXCEPTION
//
// MessageText:
//
//  %1-%2@%3   %4 CException Error[ %5 ] %n %n %6%n
//  %nMeaning - CException caught while interfacing with security database. %n
//  %nAction - Run Get Diag files.%n 
//
#define SCOTAPP_DBMINTERFACE_C_EXCEPTION 0xC0071430L

//-------------------------------------------------------------------------------------------------
// Called from 5 locations. Msg and Error are used in 2 of the parameters at the right
//
// MessageId: SCOTAPP_DBMINTERFACE_EXCEPTION
//
// MessageText:
//
//  %1-%2@%3   %4 catch(...) error = %5 %n %n %6%n
//  %nMeaning - Exception caught while interfacing with security database. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_DBMINTERFACE_EXCEPTION   0xC0071431L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_TAB_INIT
//
// MessageText:
//
//  %1-%2@%3   Takeaway Belt initialize error %n %n %4%n
//  %nMeaning - Failure while initializing Takeaway Belt control. %n
//  %nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
//
#define SCOTAPP_TAB_INIT                 0xC0101432L

//-------------------------------------------------------------------------------------------------
//
// MessageId: SCOTAPP_SSINTERFACE_STOP
//
// MessageText:
//
//  %1-%2@%3   Failed to create SmartScale control %n %n %4%n
//  %nMeaning - Failure while trying to create the SS control. %n
//  ;// Called from 1 location. Parameters at the right not used at this time.
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_SSINTERFACE_STOP         0x40021433L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_DBMINTERFACE_STOP
//
// MessageText:
//
//  %1-%2@%3   Failed to create DBM control %n %n %4%n
//  %nMeaning - Failure while trying to create the DBM control. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_DBMINTERFACE_STOP        0x40021434L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_OPERATORLOGINBARCODEMASK
//
// MessageText:
//
//  %1-%2@%3   OperatorLoginBarcodeMask is incorrect %n %n %4%n
//  %nMeaning - OperatorLogin Barcode is incorrect. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_OPERATORLOGINBARCODEMASK 0x40041435L

//-------------------------------------------------------------------------------------------------
//
// MessageId: FLR_CREATE_INSTANCE_FAILURE
//
// MessageText:
//
//  Method %1 reports Internal software error %2 - Unable to create an instance of the DOM.  The Document was not loaded.
//  %nMeaning - Unable to create an instance of the DOM.  The Document was not loaded.%n
//  %nAction - The problem may be the result of improper installation of the XML parser.  Reinstall XML parser, which is part of the reporting installation procedure.%n
//
#define FLR_CREATE_INSTANCE_FAILURE      0xC0031436L

//-------------------------------------------------------------------------------------------------
//
// MessageId: FLR_XML_FAILURE
//
// MessageText:
//
//  Method %1 reports internal software error - Unspecified Failure.  Occurs when a function call fails or
//  when required data is not present. Exception was caught.%n
//  %nMeaning - "Unspecified Failure.  Occurs when a function call fails or when required data is not present. Exception was caught".%n
//  %nAction - This is logged for one of many internal software errors.  Inform NCR support of the details of the error message.%n
//
#define FLR_XML_FAILURE                  0xC0121437L

//-------------------------------------------------------------------------------------------------
//
// MessageId: FLR_LOAD_XML_FAILURE
//
// MessageText:
//
//  Method %1 reports error - Unable to successfully load the DOM document.
//  %nMeaning -  Unable to successfully load the DOM document.%n
//  %nAction - This is an internal software error that should not be encountered.  If this error is reported, it should be forwarded to NCR support for analysis.%n
//
#define FLR_LOAD_XML_FAILURE             0xC0121438L

//-------------------------------------------------------------------------------------------------
//
// MessageId: FLR_COM_ERROR
//
// MessageText:
//
//  Method %1 detected COM Error %n
//  Source - %2 %n
//  Description - %3 %n
//  Error Code - %4 %n
//  Message - %5 
//  %nMeaning - Caught exception (_com_error e) while interfacing with Reporting -  PROCESS_COM ERRORS. %n
//  %nAction - This error references a number of possible errors.   This may be caused by formatting problems with various XML files or other COM errors.  Forward to NCR support for analysis of the specific problem.%n
//
#define FLR_COM_ERROR                    0xC0121439L

//-------------------------------------------------------------------------------------------------
//
// MessageId: SCOTAPP_PSX_CREATE
//
// MessageText:
//
//  %1-%2@%3   Cannot create PSX Object %n %n %4
//  %nMeaning - Cannot create PSX Object%n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_PSX_CREATE               0xC012143AL

//-------------------------------------------------------------------------------------------------
//
// MessageId: FLR_OLE_ERROR
//
// MessageText:
//
//  Method %1 detected OLEDispatch Error %n
//  Source - %2 %n
//  Description - %3 %n
//  Error Code - %4 %n
//  %nMeaning - caught exception (OleException) while interfacing with Reporting - PROCESS_EXCEPTIONS%n
//  %nAction - An internal error has been reported.  This is a general error event.  Forward the event information to NCR Support for further analysis.%n
//
#define FLR_OLE_ERROR                    0xC005143BL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. SCOTopts array type in 1st param. (e.g. CashBack)
//
// MessageId: SCOTAPP_NUM_ARRAY_OPTIONS
//
// MessageText:
//
//  %1-%2@%3   Number of %4 choices is %5 %n %n 
//  %nMeaning - combine %4 from SCOTOpts.DAT & overrides - ConfigObject.cpp  ::Initialize %n %n
//  %nAction - Zero choices might be unwanted if expecting to use this feature.
//
#define SCOTAPP_NUM_ARRAY_OPTIONS        0x8012143CL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. 
//XMLOFF
//
// MessageId: FLR_REPORTSERVER_OFFLINE
//
// MessageText:
//
//  The report server is offline. %1-%2@%3   %4 %n %n 
//  %nMeaning - Fastlane cannot contact the report server. %n %n
//  %nAction - Verify that the report server is up.  Verify network connections.
//
#define FLR_REPORTSERVER_OFFLINE         0xC007143DL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. 
//XMLOFF
//
// MessageId: FLR_REPORTSERVER_NOW_ONLINE
//
// MessageText:
//
//  %1-%2@%3   The report server is back online %4 %n %n 
//  %nMeaning - Fastlane is again able to transfer files to the report server. %n %n
//  %nAction - None required.
//
#define FLR_REPORTSERVER_NOW_ONLINE      0x4007143EL

//-------------------------------------------------------------------------------------------------
//
// MessageId: SCOTAPP_TB_RESPONSE_TIMEOUT
//
// MessageText:
//
//  %1-%2@%3   Fastlane has timed out waiting for a response from TB. %n %n 
//  %nMeaning - Fastlane did not receive a response from the Transaction Broker within the expected time. %n %n
//  %nAction - None. %n
//
#define SCOTAPP_TB_RESPONSE_TIMEOUT      0xC007143FL

//-------------------------------------------------------------------------------------------------
//
// MessageId: SCOTAPP_TENDER_TIMEOUT
//
// MessageText:
//
//  %1-%2@%3   Fastlane has timed out waiting for a response during Tendering. %n %n 
//  %nMeaning - Fastlane did not receive a response from the Transaction Broker within the expected time. %n %n
//  %nAction - None. %n
//
#define SCOTAPP_TENDER_TIMEOUT           0xC0071440L

//-------------------------------------------------------------------------------------------------
//
// MessageId: SCOTAPP_TB_OUT_OF_SYNC
//
// MessageText:
//
//  %1-%2@%3   Transaction Broker timed out waiting for a POS response. %n %n 
//  %nMeaning - The transaction broker might be out of sync with the POS (host) . %n %n
//  %nAction - Run GetDiagFiles.  Reboot lane. %n
//
#define SCOTAPP_TB_OUT_OF_SYNC           0xC0071441L

//-------------------------------------------------------------------------------------------------
//XMLOFF
//
// MessageId: SCOTAPP_HEARTBEAT
//
// MessageText:
//
//  %1-%2@%3   ScotApp Heartbeat message. %n %4 %n %n
//  %nMeaning - The ScotApp module is sending an "I'm alive" indicator. %n %n
//  %nAction - No Action. %n
//
#define SCOTAPP_HEARTBEAT                0x40071442L

//-------------------------------------------------------------------------------------------------
//XMLOFF
//
// MessageId: SCOTAPP_HEARTBEAT2
//
// MessageText:
//
//  %1-%2@%3   ScotApp Heartbeat message. %n %4 %n %n
//  %nMeaning - The ScotApp module is sending an "I'm alive" indicator. %n %n
//  %nAction - No Action. %n
//
#define SCOTAPP_HEARTBEAT2               0x40071443L

//-------------------------------------------------------------------------------------------------
// Called from 4 locations
//
// MessageId: SCOTAPP_PS_FILEOPENABORT
//
// MessageText:
//
//  %1-%2@%3   SCOT could not open file: \%4\ %n %n %5%n
//  %nMeaning - Fatal"Stop Sign", file Open Failed. A required file is missing or corrupt. %n %n
//  %nAction - make sure the file name exists and is not corrupt%n
//
#define SCOTAPP_PS_FILEOPENABORT         0xC0031444L

//-------------------------------------------------------------------------------------------------
// Filename is used in the 1st parameter at the right
//
// MessageId: SCOTAPP_SM_REPORT_FILEOPEN
//
// MessageText:
//
//  %1-%2@%3   File not found %n %n %4%n
//  %nMeaning - Unable to open report file. %n %n
//  %nAction - make sure the File exists and is not corrupt%n
//
#define SCOTAPP_SM_REPORT_FILEOPEN       0x400B1445L

//-------------------------------------------------------------------------------------------------
// Filename is used in the 1st parameter at the right
//
// MessageId: SCOTAPP_SM_REPORT_FILEWRITE
//
// MessageText:
//
//  %1-%2@%3   File not found %n %n %4%n
//  %nMeaning - Unable to write to report file. %n %n
//  %nAction - Make sure the is not read-only or corrupt.%n
//
#define SCOTAPP_SM_REPORT_FILEWRITE      0x400B1446L

//-------------------------------------------------------------------------------------------------
// Filename is used in the 1st parameter at the right
//
// MessageId: SCOTAPP_FILEOPEN
//
// MessageText:
//
//  %1-%2@%3   Unable to open file %n %n %4%n
//  %nMeaning - Unable to open file. %n %n
//  %nAction - make sure the File exists and is not corrupt%n
//
#define SCOTAPP_FILEOPEN                 0x400B1447L

//-------------------------------------------------------------------------------------------------
// Filename is used in the 1st parameter at the right
//
// MessageId: SCOTAPP_TB_FILENOTFOUND
//
// MessageText:
//
//  %1-%2@%3   TB unable to open file %n %n %4%n
//  %nMeaning - Transaction returned TB_E_FILENOTFOUND to ScotApp. %n %n
//  %nAction - Make sure the File exists and is not corrupt%n
//
#define SCOTAPP_TB_FILENOTFOUND          0xC00B1448L

//-------------------------------------------------------------------------------------------------
// Filename is used in the 1st parameter at the right
//
// MessageId: SCOTAPP_TB_PRINTERFILE
//
// MessageText:
//
//  %1-%2@%3   Unable to open file %n %n %4%n
//  %nMeaning - Unable to open the receipt or slip file. %n %n
//  %nAction - Make sure the File exists and is not corrupt%n
//
#define SCOTAPP_TB_PRINTERFILE           0x400B1449L

//-------------------------------------------------------------------------------------------------
// Called from 3 locations. Last Msg, Description, Error and Dev Class ID are used in 4 of the parameters
//
// MessageId: SCOTAPP_DMINTERFACE_OLE_INFO
//
// MessageText:
//
//  %1-%2@%3   %4 COleDispatchException[ %5 ] Codes(0x%6), device %7 (%8) %n %n %9%n
//  %nMeaning - An OLE exception occurred during a non-critical encryptor key test. %n %n
//  %nAction - None required.%n
//
#define SCOTAPP_DMINTERFACE_OLE_INFO     0x4010144AL

//-------------------------------------------------------------------------------------------------
// Called from 3 locations. Last Msg, Description, Error and Dev Class ID are used in 4 of the parameters
//
// MessageId: SCOTAPP_DMINTERFACE_OLE_FATAL
//
// MessageText:
//
//  %1-%2@%3   %4 COleDispatchException[ %5 ] Codes(0x%6), device %7 (%8) %n %n %9%n
//  %nMeaning - Fatal"Stop Sign", an ole exception occurred %n %n
//  %nAction - Possible hardware problem, make sure the devices is plugged in and activated. Run Get Diag Files.%n
//
#define SCOTAPP_DMINTERFACE_OLE_FATAL    0xC010144BL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. The device class is used in the 1st parameter.
//
// MessageId: SCOTAPP_DF_NO_DEV_FOUND_INFO
//
// MessageText:
//
//  %1-%2@%3   No device was found where 'Configure=Yes' for device class %4.%n%n %5%n
//  %nMeaning - Device not found but the device class exists. %n %n
//  %nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
//
#define SCOTAPP_DF_NO_DEV_FOUND_INFO     0x4003144CL

//-------------------------------------------------------------------------------------------------
//
// MessageId: SCOTAPP_TENDER_COMPLETE
//
// MessageText:
//
//  %1-%2@%3   Fastlane tender has successfully completed. %n %n 
//  %nMeaning - A tender operation has completed.
//  %nAction - None. %n
//
#define SCOTAPP_TENDER_COMPLETE          0x4007144DL

//-------------------------------------------------------------------------------------------------
//
// MessageId: SCOTAPP_ATTRACT_SCREEN_ENTRY
//
// MessageText:
//
//  %1-%2@%3   Fastlane has returned to the Attract screen. %n %n 
//  %nMeaning - Fastlane is waiting for a new customer.
//  %nAction - None. %n
//
#define SCOTAPP_ATTRACT_SCREEN_ENTRY     0x4007144EL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_SEC_MGR_PROCEDURE_MAIN_WINDOW_FAILED
//
// MessageText:
//
//  %1-%2@%3   Security Manager Procedures main window NOT Created %n %n %4%n
//  %nMeaning - System Restart (Fatal), if cannot create the main security procedures handle to a window. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_SEC_MGR_PROCEDURE_MAIN_WINDOW_FAILED 0xC001144FL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_SEC_MGR_PROCEDURE_WINDOW_FAILED
//
// MessageText:
//
//  %1-%2@%3    Security Manager Interface window NOT Created %n %n %4%n
//  %nMeaning - System Restart (Fatal), cannot create the security interface handle to a window. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_SEC_MGR_PROCEDURE_WINDOW_FAILED 0xC0011450L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_SEC_MGR_INTERFACE_WINDOW_FAILED
//
// MessageText:
//
//  %1-%2@%3   Security Manager Interface window NOT Created %n %n %4%n
//  %nMeaning - System Restart (Fatal), cannot create the security interface. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_SEC_MGR_INTERFACE_WINDOW_FAILED 0xC0011451L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_SEC_MGR_PROCEDURE_UNINIT
//
// MessageText:
//
//  %1-%2@%3   Security Manager did Not Un-Initialize %n %n %4%n
//  %nMeaning - Fatal"Stop Sign", failed to un-init security interface. %n
//  %nAction - Run Get Diag files. Might temporarily turn on Security DataCapture for more logging.%n
//
#define SCOTAPP_SEC_MGR_PROCEDURE_UNINIT 0xC0021452L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_SEC_MGR_PROCEDURE_WINDOW_DESTROY
//
// MessageText:
//
//  %1-%2@%3   Security Manager Procedures window was not destroyed %n %n %4%n
//  %nMeaning - Fatal"Stop Sign", failed to DestroyWindow security interface. %n
//  %nAction - Run Get Diag files. Reboot NT.%n
//
#define SCOTAPP_SEC_MGR_PROCEDURE_WINDOW_DESTROY 0xC0021453L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Msg, Description, and Error are used in 3 of the parameters
//
// MessageId: SCOTAPP_SEC_MGR_INTERFACE_OLEDISPATCH_EXCEPTION
//
// MessageText:
//
//  %1-%2@%3   %4 COleDispatchException[ %5 ] Code(0x%6) %n %n %7%n
//  %nMeaning - COleDispatchException caught while interfacing with security. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_SEC_MGR_INTERFACE_OLEDISPATCH_EXCEPTION 0xC0071454L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Msg, Description, and Error are used in 3 of the parameters
//
// MessageId: SCOTAPP_SEC_MGR_INTERFACE_OLE_EXCEPTION
//
// MessageText:
//
//  %1-%2@%3   %4 OleException Error[ %5 ] Code(0x%6) %n %n %7%n
//  %nMeaning - OleException caught while interfacing with security. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_SEC_MGR_INTERFACE_OLE_EXCEPTION 0xC0071455L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Msg and Description are used in 2 of the parameters
//
// MessageId: SCOTAPP_SEC_MGR_INTERFACE_C_EXCEPTION
//
// MessageText:
//
//  %1-%2@%3   %4 CException Error[ %5 ] %n %n %6%n
//  %nMeaning - CException caught while interfacing with security. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_SEC_MGR_INTERFACE_C_EXCEPTION 0xC0071456L

//-------------------------------------------------------------------------------------------------
// Called from 5 locations. Msg and Error are used in 2 of the parameters at the right
//
// MessageId: SCOTAPP_SEC_MGR_INTERFACE_EXCEPTION
//
// MessageText:
//
//  %1-%2@%3   %4 catch(...) error = %5 %n %n %6%n
//  %nMeaning - Exception caught while interfacing with security. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_SEC_MGR_INTERFACE_EXCEPTION 0xC0071457L

//-------------------------------------------------------------------------------------------------
//
// MessageId: SCOTAPP_SEC_MGR_INTERFACE_STOP
//
// MessageText:
//
//  %1-%2@%3   Failed to create Security Manager control %n %n %4%n
//  %nMeaning - Failure while trying to create the Security Manager control. %n
//  ;// Called from 1 location. Parameters at the right not used at this time.
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_SEC_MGR_INTERFACE_STOP   0x40021458L

//-------------------------------------------------------------------------------------------------
//
// MessageId: SCOTAPP_SEC_MGR_PROCEDURE_INIT_FAILED
//
// MessageText:
//
//  %1-%2@%3   Security Manager failed to initialize %n %n %4 %5 %n
//  %nMeaning - System Restart (Fatal). %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_SEC_MGR_PROCEDURE_INIT_FAILED 0xC0011459L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_SEC_MGR_PROCEDURE_ADVISE_FAILED
//
// MessageText:
//
//  %1-%2@%3   Security Manager Procedures Advise failed %n %n %4%n
//  %nMeaning - Fatal"Stop Sign", failed to load security manager state machine XML file. %n
//  %nAction - Turn on Security Manager log and check the error logged. %n
//
#define SCOTAPP_SEC_MGR_PROCEDURE_ADVISE_FAILED 0xC001145AL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. 
//XMLOFF
//
// MessageId: SCOTAPP_TB_HOST_OFFLINE
//
// MessageText:
//
//  The POS is offline. %1-%2@%3   %4 %n %n 
//  %nMeaning - The Point of sale system cannot contact it's server. %n %n
//  %nAction - Verify that the server is up.  Verify network connections.
//
#define SCOTAPP_TB_HOST_OFFLINE          0xC009145BL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. 
//XMLOFF
//
// MessageId: SCOTAPP_TB_HOST_ONLINE
//
// MessageText:
//
//  %1-%2@%3   The POS is back online %4 %n %n 
//  %nMeaning - The Point of sale is back online to it's server. %n %n
//  %nAction - None required.
//
#define SCOTAPP_TB_HOST_ONLINE           0x4009145CL

//-------------------------------------------------------------------------------------------------
//
// MessageId: SCOTAPP_TB_OUT_OF_SYNC_STOP
//
// MessageText:
//
//  ScotApp timed out waiting for a TB response. %1-%2@%3 %4 %n %n 
//  %nMeaning - The transaction broker is out of sync with the SCOTAPP. %n %n
//  %nAction - Reboot Entire System by switch.
//
#define SCOTAPP_TB_OUT_OF_SYNC_STOP      0xC009145DL

//-------------------------------------------------------------------------------------------------
//
// MessageId: SCOTAPP_TB_OUT_OF_SYNC_ASSISTMODE
//
// MessageText:
//
//  Transaction Broker timed out waiting for a POS response. %1-%2@%3 %4 %n %n 
//  %nMeaning - The transaction broker might be out of sync with the POS (host) . %n %n
//  %nAction - Go to Assist Mode and try to sync with POS.
//
#define SCOTAPP_TB_OUT_OF_SYNC_ASSISTMODE 0xC009145EL

//-------------------------------------------------------------------------------------------------
//
// MessageId: SCOTAPP_TB_OUT_OF_SYNC_ASSISTMODE_RECOVER
//
// MessageText:
//
//  %1-%2@%3   out of sync need assist mode has recovered. %n %n 
//  %nMeaning - out of sync with the POS (host) has been resolved. %n %n
//  %nAction - None Required. %n
//
#define SCOTAPP_TB_OUT_OF_SYNC_ASSISTMODE_RECOVER 0x4009145FL

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Keystroke value in 1st param.
//
// MessageId: SCOTAPP_KEY_STUCK
//
// MessageText:
//
//  %1-%2@%3   Repeated keystrokes are being received (%4)%n 
//  %nMeaning - A key is probably stuck in the "down" position on the physical keyboard.
//  %nAction - Reposition the physical keyboard so that no keys are depressed.
//
#define SCOTAPP_KEY_STUCK                0x40091460L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Keystroke count in 1st param.
//
// MessageId: SCOTAPP_KEY_NO_LONGER_STUCK
//
// MessageText:
//
//  %1-%2@%3   Repeated keystrokes are no longer being received after %4 repetitions.%n 
//  %nMeaning - A key is no longer stuck in the "down" position on the physical keyboard.
//  %nAction - No action is necessary.
//
#define SCOTAPP_KEY_NO_LONGER_STUCK      0x40091461L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_RC_MGR_PROCEDURE_MAIN_WINDOW_FAILED
//
// MessageText:
//
//  %1-%2@%3   Remote Console Manager Procedures main window NOT Created %n %n %4%n
//  %nMeaning - System Restart (Fatal), cannot create the main RAP procedures handle to a window. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_RC_MGR_PROCEDURE_MAIN_WINDOW_FAILED 0xC0011462L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_RC_MGR_PROCEDURE_WINDOW_FAILED
//
// MessageText:
//
//  %1-%2@%3    Remote Console Manager Interface window NOT Created %n %n %4%n
//  %nMeaning - System Restart (Fatal), cannot create the RAP interface handle to a window. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_RC_MGR_PROCEDURE_WINDOW_FAILED 0xC0011463L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_RC_MGR_INTERFACE_WINDOW_FAILED
//
// MessageText:
//
//  %1-%2@%3   Remote Console Manager Interface window NOT Created %n %n %4%n
//  %nMeaning - System Restart (Fatal), cannot create the RAP interface. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_RC_MGR_INTERFACE_WINDOW_FAILED 0xC0011464L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_RC_MGR_PROCEDURE_UNINIT
//
// MessageText:
//
//  %1-%2@%3   Remote Console did Not Un-Initialize %n %n %4%n
//  %nMeaning - Fatal"Stop Sign", failed to un-init RCM interface. %n
//  %nAction - Run Get Diag files. Might temporarily turn on RCM DataCapture for more logging.%n
//
#define SCOTAPP_RC_MGR_PROCEDURE_UNINIT  0xC0021465L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Parameters at the right not used at this time.
//
// MessageId: SCOTAPP_RC_MGR_PROCEDURE_WINDOW_DESTROY
//
// MessageText:
//
//  %1-%2@%3   Remote Console Manager Procedures window was not destroyed %n %n %4%n
//  %nMeaning - Fatal"Stop Sign", failed to DestroyWindow RCM interface. %n
//  %nAction - Run Get Diag files. Reboot NT.%n
//
#define SCOTAPP_RC_MGR_PROCEDURE_WINDOW_DESTROY 0xC0021466L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Msg, Description, and Error are used in 3 of the parameters
//
// MessageId: SCOTAPP_RC_MGR_INTERFACE_OLEDISPATCH_EXCEPTION
//
// MessageText:
//
//  %1-%2@%3   %4 COleDispatchException[ %5 ] Code(0x%6) %n %n %7%n
//  %nMeaning - COleDispatchException caught while interfacing with RAP. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_RC_MGR_INTERFACE_OLEDISPATCH_EXCEPTION 0xC0071467L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Msg, Description, and Error are used in 3 of the parameters
//
// MessageId: SCOTAPP_RC_MGR_INTERFACE_OLE_EXCEPTION
//
// MessageText:
//
//  %1-%2@%3   %4 OleException Error[ %5 ] Code(0x%6) %n %n %7%n
//  %nMeaning - OleException caught while interfacing with RAP. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_RC_MGR_INTERFACE_OLE_EXCEPTION 0xC0071468L

//-------------------------------------------------------------------------------------------------
// Called from 1 location. Msg and Description are used in 2 of the parameters
//
// MessageId: SCOTAPP_RC_MGR_INTERFACE_C_EXCEPTION
//
// MessageText:
//
//  %1-%2@%3   %4 CException Error[ %5 ] %n %n %6%n
//  %nMeaning - CException caught while interfacing with RAP. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_RC_MGR_INTERFACE_C_EXCEPTION 0xC0071469L

//-------------------------------------------------------------------------------------------------
// Called from 5 locations. Msg and Error are used in 2 of the parameters at the right
//
// MessageId: SCOTAPP_RC_MGR_INTERFACE_EXCEPTION
//
// MessageText:
//
//  %1-%2@%3   %4 catch(...) error = %5 %n %n %6%n
//  %nMeaning - Exception caught while interfacing with RAP. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_RC_MGR_INTERFACE_EXCEPTION 0xC007146AL

//-------------------------------------------------------------------------------------------------
//
// MessageId: SCOTAPP_RC_MGR_INTERFACE_STOP
//
// MessageText:
//
//  %1-%2@%3   Failed to create Remote Console control %n %n %4%n
//  %nMeaning - Failure while trying to create the Remote Console control. %n
//  ;// Called from 1 location. Parameters at the right not used at this time.
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_RC_MGR_INTERFACE_STOP    0x4002146BL

//-------------------------------------------------------------------------------------------------
//
// MessageId: SCOTAPP_RC_MGR_PROCEDURE_INIT_FAILED
//
// MessageText:
//
//  %1-%2@%3   Remote Console Manager failed to initialize %n %n %4 %5 %n
//  %nMeaning - System Restart (Fatal). %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_RC_MGR_PROCEDURE_INIT_FAILED 0xC001146CL

//-------------------------------------------------------------------------------------------------
//
// MessageId: SCOTAPP_PSX_START_SERVER_FAILED
//
// MessageText:
//
//  %1-%2@%3   PS Initialize - PSX StartServer failed. rc= %n %n %4 %5 %n
//  %nMeaning - RAP and LaunchPad will not be able to communicate with this lane. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_PSX_START_SERVER_FAILED  0xC001146DL

//-------------------------------------------------------------------------------------------------
// Device Error.
//
// MessageId: SCOTAPP_DEVICE_ERROR
//
// MessageText:
//
//  %1-%2@%3   %n%n
//  %4 %n%n
//  %5 %n%n
//  %6
//
#define SCOTAPP_DEVICE_ERROR             0xC001146EL

//-------------------------------------------------------------------------------------------------
//
// MessageId: CUSTINFO_CREATE_INSTANCE_FAILURE
//
// MessageText:
//
//  Method %1 reports Internal software error %2 - Unable to create an instance of the DOM.  The Document was not loaded.
//  %nMeaning - Unable to create an instance of the DOM.  The Document was not loaded.%n
//  %nAction - The problem may be the result of improper installation of the XML parser. %n
//
#define CUSTINFO_CREATE_INSTANCE_FAILURE 0xC003146FL

//-------------------------------------------------------------------------------------------------
//
// MessageId: CUSTINFO_CONFIG_LOAD_FAILURE
//
// MessageText:
//
//  Method %1 reports error - Unable to successfully load the Config file.
//  Source - %2 %n
//  Line - %3 %n
//  Error  - %4 %n
//  %nMeaning -  Unable to successfully load the Customer info configuration file document.%n
//  %nAction - Verify.%n
//
#define CUSTINFO_CONFIG_LOAD_FAILURE     0xC0031470L

//-------------------------------------------------------------------------------------------------
//
// MessageId: CUSTINFO_LOAD_XML_FAILURE
//
// MessageText:
//
//  Method %1 reports error - Unable to successfully load the DOM document.
//  Source - %2 %n
//  Line - %3 %n
//  Error  - %4 %n
//  %nMeaning -  Unable to successfully load the DOM document.%n
//  %nAction - Verify.%n
//
#define CUSTINFO_LOAD_XML_FAILURE        0xC0121471L

//-------------------------------------------------------------------------------------------------
//
// MessageId: CUSTINFO_COM_ERROR
//
// MessageText:
//
//  Method %1 detected COM Error %n
//  Source - %2 %n
//  Line - %3 %n
//  Error  - %4 %n
//  %nMeaning - Caught exception (_com_error e) while interfacing with Personalization Web Service -  PROCESS_COM ERRORS. %n
//  %nAction - This error references a number of possible errors. This may be caused by failure to access the configured web address or other COM errors.  Forward to NCR support for analysis of the specific problem.%n
//
#define CUSTINFO_COM_ERROR               0xC0121472L

//-------------------------------------------------------------------------------------------------
//
// MessageId: CUSTINFO_INIT
//
// MessageText:
//
//  Method %1 unable to initialize %n
//  Source - %2 %n
//  Line - %3 %n
//  Error  - %4 %n
//  %nMeaning - Unable to initialize CustomerInfo (personalization) object %n
//  %nAction - The CustomerInfo object failed to initialize. This may be due to a problem with the CustomerInfoService.xml configuration files%n
//
#define CUSTINFO_INIT                    0xC0011473L

//-------------------------------------------------------------------------------------------------
//
// MessageId: CUSTINFO_GENERAL_EXCEPTION
//
// MessageText:
//
//  Method %1 caught exception %n
//  Source - %2 %n
//  Line - %3 %n
//  Error  - %4 %n
//  %nMeaning - Caught unspecified exception while interfacing with Personalization Web Service.. %n
//  %nAction - This error references a number of possible errors. Forward to NCR support for analysis of the specific problem.%n
//
#define CUSTINFO_GENERAL_EXCEPTION       0xC0121474L

//-------------------------------------------------------------------------------------------------
//
// MessageId: CUSTINFO_ONLINE
//
// MessageText:
//
//  Method %1 detected COM Error %n
//  Source - %2 %n
//  Line - %3 %n
//  Error  - %4 %n
//  %nMeaning - Successfully communicated with Personalization web service %n
//  %nAction - This message is generated to indicate that communication with the personalization web service has bee re-established.%n
//
#define CUSTINFO_ONLINE                  0x40111475L

//-------------------------------------------------------------------------------------------------
//
// MessageId: SCOTOPTS_CONFIG_ERROR
//
// MessageText:
//
//  %1-%2@%3   ScotOpts Configuration Error. %n
//  %nError:   %4 %n
//  %nMeaning: Incorrect values for configuration option %n 
//  %nAction:  Insure that all parameters are configured correctly.
//
#define SCOTOPTS_CONFIG_ERROR            0xC0031476L

//-------------------------------------------------------------------------------------------------
//
// MessageId: SCOTAPP_DEVFACTORY_MOTION_SENSOR_EXCEED_MAX
//
// MessageText:
//
//  %1-%2@%3   SCOT forced to simulate motion begin and ended due to Motion Sensor Exceed Maximum Cycles: %n %n %5%n
//  %nMeaning: Motion Exceed Max event received. %n
//  %nAction:  Probable hardware problem, verify the coupon entry slot is clear.%n
//
#define SCOTAPP_DEVFACTORY_MOTION_SENSOR_EXCEED_MAX 0xC0101477L

//-------------------------------------------------------------------------------------------------
//
// MessageId: SCALEDISPLAY_ERROR
//
// MessageText:
//
//  %1-%2@%3   Scale Display Configuration Error.%n
//  %nError:   %4 %n
//  %nMeaning: Failed to configure display for weights and measures.%n 
//  %nAction:  Restart the lane.  If error re-occurs, contact NCR support.%n
//
#define SCALEDISPLAY_ERROR               0xC0031478L

//-------------------------------------------------------------------------------------------------
//
// MessageId: SCALEMETRIC_ERROR1
//
// MessageText:
//
//  %1-%2@%3   Scale Metric Configuration Error. %n
//  %nError:   %4 %n
//  %nWarning: The ScaleMetric configuration option is 'N' but the scale is metric.%n 
//  %nAction:  Make sure that the setting for the ScaleMetric configuration option is correct.%n
//
#define SCALEMETRIC_ERROR1               0xC0031479L

//-------------------------------------------------------------------------------------------------
//
// MessageId: SCALEMETRIC_ERROR2
//
// MessageText:
//
//  %1-%2@%3   Scale Metric Configuration Error. %n
//  %nError:   %4 %n
//  %nWarning: The ScaleMetric configuration option is 'Y' but the scale is not metric.%n 
//  %nAction:  Make sure that the setting for the ScaleMetric configuration option is correct.%n
//
#define SCALEMETRIC_ERROR2               0xC003147AL

//-------------------------------------------------------------------------------------------------
//
// MessageId: BAGBOX_CONFIG_ERROR
//
// MessageText:
//
//  %1-%2@%3   Configuration Error. %n
//  %nWarning: This is a TAB system and the CustomerBagAllowed configuration option is 'Y'.  This option is not used on TAB systems.%n 
//  %nAction:  Set the CustomerBagAllowed option in the [Operations] section of ScotOpts to 'N'.%n
//
#define BAGBOX_CONFIG_ERROR              0x8003147BL

//-------------------------------------------------------------------------------------------------
//
// MessageId: SCOTAPP_DMXADAPTER_OLEDISPATCH_EXCEPTION
//
// MessageText:
//
//  %1-%2@%3   %4 COleDispatchException[ %5 ] Code(0x%6) %n %n %7%n
//  %nMeaning - COleDispatchException caught while accessing the device. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_DMXADAPTER_OLEDISPATCH_EXCEPTION 0xC010147CL

//-------------------------------------------------------------------------------------------------
//
// MessageId: SCOTAPP_DMXADAPTER_EXCEPTION
//
// MessageText:
//
//  %1-%2@%3   %4 catch(...) error = %5 %n %n %6%n
//  %nMeaning - Exception caught while accessing the device. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_DMXADAPTER_EXCEPTION     0xC010147DL

//-------------------------------------------------------------------------------------------------
//
// MessageId: OWNBAG_CONFIG_ERROR
//
// MessageText:
//
//  %1-%2@%3   Configuration Error. %n
//  %nWarning: This is a TAB system and the CustomerBagAllowedInScanAndBag configuration option is 'Y'.  This option is not used on TAB systems.%n 
//  %nAction:  Set the CustomerBagAllowedInScanAndBag option in the [Operations] section of ScotOpts to 'N'.%n
//
#define OWNBAG_CONFIG_ERROR              0x8003147EL

//-------------------------------------------------------------------------------------------------
//
// MessageId: SCOTAPP_GLORYADAPTER_OLEDISPATCH_EXCEPTION
//
// MessageText:
//
//  %1-%2@%3   %4 COleDispatchException[ %5 ] Code(0x%6) %n %n %7%n
//  %nMeaning - COleDispatchException caught while accessing the device. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_GLORYADAPTER_OLEDISPATCH_EXCEPTION 0xC010147FL

//-------------------------------------------------------------------------------------------------
//
// MessageId: SCOTAPP_GLORYADAPTER_EXCEPTION
//
// MessageText:
//
//  %1-%2@%3   %4 catch(...) error = %5 %n %n %6%n
//  %nMeaning - Exception caught while accessing the device. %n
//  %nAction - Run Get Diag files.%n
//
#define SCOTAPP_GLORYADAPTER_EXCEPTION   0xC0101480L

//-------------------------------------------------------------------------------------------------
//
// MessageId: MULTIPICK_CONFIG_ERROR
//
// MessageText:
//
//  %1-%2@%3   Configuration Error. %n
//  %nWarning: PickListAssist is enabled and AllowMultiPick is 'Y'. This option should be 'N' when PicklistAssistEnable is 'Y'.
//  %nAction:  Set the AllowMultiPick option in the [Operations] section of ScotOpts to 'N'.%n
//
#define MULTIPICK_CONFIG_ERROR           0x80031481L

