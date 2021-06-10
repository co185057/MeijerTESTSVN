;//
;// This is the SCOTAPP Message (Source) File.
;//
;//
;//  
;//**  Copyright NCR Corp. 1997
;//**  This media contains an authorized
;//**  copy or copies of material copyrighted
;//**  by NCR Corp.  This copyright notice
;//**  and any other copyright notices
;//**  included in machine readable copies
;//**  must be reproduced on all authorized
;//**  copies.  Confidential, Unpublished
;//**  Property of NCR Corporation
;//
;//
;// The following lines should be in your application specific message file:
;//      //NMAS_REGISTRY_NAME should be 18 char or less
;//      #define NMAS_REGISTRY_NAME	"test"
;// 
;// The install process for "NMAS_REGISTRY_NAME" should add the following key
;//     and values to the NT registry.

;//         HKEY_LOCAL_MACHINE 
;//           System 
;//             CurrentControlSet 
;//               Services 
;//                 EventLog 
;//                   Application 
;//                     <NMAS_REGISTRY_NAME> 
;//							EventMessageFile:REG_EXPAND_SZ:Full Path name of the 
;//								executable or DLL containing the messages 
;//							CategoryMessageFile:REG_EXPAND_SZ:Full Path name of the 
;//								executable or DLL containing the Categories can be same 
;//								as EventMessageFile
;//							CategoryCount:REG_DWORD:Hex value of "NMAS_CATEGORY_COUNT"
;//								or the total number of categories in CategoryMessageFile
;//							TypesSupported:REG_DWORD:0x07
;//
;
;//Within the message text, several escape sequences are supported for
;//    dynamically formatting the message.  The percent sign character (%)
;//    begins all escape sequences.
;//
;//
;//        %0 - This terminates a message text line without a trailing
;//             newline.  This can be used to build up long lines or to
;//             terminate the message itself without a trailing newline,
;//             which is useful for prompt messages.
;//
;//        %n!printf format string!  - This identifies an insert.  The
;//             value of n can be between 1 and 99.  The printf format
;//             string must be bracketed by exclamation marks.  It is
;//             optional and defaults to !s! if not specified.
;//
;//             The printf format string can contain the * specifier for
;//             either the precision or width components, and if so, they
;//             will consume inserts %n+1 and %n+2 for their values at run
;//             time.  MC will print a warning message if an explicit
;//             reference is made to these inserts elsewhere in the message
;//             text.
;//
;//    Inserts must reference a parameter passed to the FormatMessage API
;//    call.  It will return an error if a message refers to an insert that
;//    was not passed to the FormatMessage API call.
;//
;//    Any other character following a percent sign, other than a digit will
;//    be formatted in the output message without the percent sign.  Some
;//    examples:
;//
;//        %% - will output a single percent sign in the formatted message text.
;//
;//        %n - will output a hard line break when it occurs at the end of a
;//             a line.  Useful when FormatMessage is supplying normal line
;//             breaks so the message fits in a certain width.
;//
;//        %r - will output a hard carriage return, without a trailing newline.
;//
;//        %b - will output a space in the formatted message text.  This
;//             can be used to insure there are the appropriate number of
;//             trailing spaces in a message text line.
;//
;//        %t - will output a tab in the formatted message text.
;//
;//        %. - will output a single period in the formatted message text.
;//             This can be used to get a single period at the beginning of
;//             a line without terminating the message text definition.
;//
;//        %! - will output a single exclamation point in the formatted
;//             message text.  This can be used to get an exclamation point
;//             immediately after an insert without it being mistaken for
;//             the beginning of a printf format string.
;//
;//  The Following is a sample Message
;//
;//  MessageId=0x0100
;//  SymbolicName=ERROR_ONE
;//  Language=English
;//  Starting tmssvr%0
;//  .
;//  Language=French
;//  Commencant tmssvr%0
;//  .
;//
;//
;//  sample message entry is below
;// Parameter 1 is a file name used by SCOT application
;// MessageId=2200
;// SymbolicName=SCOTAPP_FILE_NOTFOUND
;// Language=English
;// %1 file not found.
;// %nMeaning - The application can not find file %1. 
;// %nAction - Reinstall the application. If still problem exists, contact an NCR Support representative.%n%0
;// .
;//-------------------------------------------------------------------------------------------------
;// All entries - %1-%2@%3 writes out the system category, file name abbreviation, and line number

;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.



;// Copied from Platform mc files
;//
;// The StoreMinder CNCREventLog class uses the Severity field to 
;// pass the Severity of the message being logged from the message 
;// file to ReportEvent.
;//
;// The CONVERT_Severity macro is used for this purpose.
;//
;

SeverityNames = (
 	Success            = 0x0:Severity_SM_SUCCESS
 	Informational      = 0x1:Severity_SM_INFORMATION
 	Warning            = 0x2:Severity_SM_WARNING
 	Error              = 0x3:Severity_SM_ERROR
 	)

;
;
;#define CONVERT_SM_Severity(X)           \
;		(((X>>30)==Severity_SM_SUCCESS)    ? evlTypeAuditSuccess:           \
;		(((X>>30)==Severity_SM_INFORMATION)? evlTypeInformation:            \

;		(((X>>30)==Severity_SM_WARNING)    ? evlTypeWarning:                \
;		(((X>>30)==Severity_SM_ERROR)      ? evlTypeError : evlTypeInformation))))
;
;//
;// The StoreMinder CNCREventLog class uses the Facility field to 
;// pass the category of the message being logged from the message 
;// file to ReportEvent.  
;//
;// The CONVERT_SM_CATEGORY macro is used for this purpose.
;//

FacilityNames = (
    StartupInit         = 1:SM_CAT_STARTUP_INIT
    StopAbort           = 2:SM_CAT_STOP_ABORT
    Configuration       = 3:SM_CAT_CONFIGURATION
    InputParameter      = 4:SM_CAT_INPUT_PARAMETER
    SystemCall          = 5:SM_CAT_SYSTEM_CALL
    ThirdPartyInterface = 6:SM_CAT_3rdPARTY_INTERFACE
    InternalInterface   = 7:SM_CAT_INTERNAL_INTERFACE
    DataCapture         = 8:SM_CAT_DATA_CAPTURE
    LanWanComm          = 9:SM_CAT_LAN_WAN_COMM
    Security            = 10:SM_CAT_SECURITY
    FileIO              = 11:SM_CAT_FILE_IO
    Resource            = 12:SM_CAT_RESOURCE
    IPC= 13:SM_CAT_IPC
    PeripheralComm      = 14:SM_CAT_PERIPHERAL_COMM
    Hardware            = 15:SM_CAT_HARDWARE
    DevicePeripheral    = 16:SM_CAT_DEVICE_PERIPHERAL
    Operational         = 17:SM_CAT_OPERATIONAL
    InternalError       = 18:SM_CAT_INTERNAL_ERROR
)


;// SM_CATEGORY_COUNT should be the total number of categories
;#define SM_CATEGORY_COUNT 18

LanguageNames = (English=0x0409:MSG00001)

;//SOURCEID=SCOTAPP
;#define CONVERT_SM_CATEGORY(X)   (WORD)((X>>16) & 0xFFF)														\
;		
;
;//END SECTION

;// XMLOFF
MessageID=5000
SymbolicName=SCOTAPP_INITIALIZE
Severity=Informational
Facility=StartupInit
Language=English
%1-%2@%3   Starting SCOT...   %n %n %4
%nMeaning - Informational, when application is starting up - SCOTAPPDlg.cpp  ::OnInitDialog
%nAction - none.
.
;//-------------------------------------------------------------------------------------------------
;// not in use at this time 
;//XMLOFF
MessageID=5001
SymbolicName=SCOTAPP_INITIALIZED
Severity=Informational
Facility=StartupInit
Language=English
%1-%2@%3   SCOTAPP was initialized. %n %n %4
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
;//XMLOFF
MessageID=5002
SymbolicName=SCOTAPP_EXIT
Severity=Informational
Facility=StopAbort
Language=English
%1-%2@%3   SCOTAPP was ended. %n %n %4
%nMeaning - Application has been stopped. %n
%nAction - none
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5003
SymbolicName=SCOTAPP_RESTART
Severity=Error
Facility=StopAbort
Language=English
%1-%2@%3   SCOTAPP was forced to restart. %n %n %4
%nMeaning - Application has been forced to restart. %n
%nAction - Run Get Diag files.
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Text msg from GetErrorMessage() is used in the 1st parameter at the right.
MessageID=5004
SymbolicName=SCOTAPP_RESTART2
Severity=Error
Facility=StopAbort
Language=English
%1-%2@%3   SCOTAPP was forced to restart <%4>. %n %n %5
%nMeaning - Fatal"Stop Sign", exception during message processing has forced a restart. %n
%nAction - Run Get Diag files.
.
;//-------------------------------------------------------------------------------------------------
;// not in use at this time 
MessageID=5005
SymbolicName=SCOTAPP_FILE_READ_ERROR
Severity=Error
Facility=FileIO
Language=English
%1-%2@%3   File read %n %n %4
.

;//-------------------------------------------------------------------------------------------------
;// Called from 2 locations. Section, Key, Default text messages are used in 1st 3 parameters

MessageID=5006
SymbolicName=SCOTAPP_MISSING_OPTION
Severity=Informational
Facility=Configuration
Language=English
%1-%2@%3   Missing option [%4] %5, using default <%6> %n %n %7
%nMeaning - Configuration option not defined or incorrect. %n
%nAction - if the default value is not desired, set the option.
.
 
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Status of Cash Value List & Cash Low List are used in 2 parameters
MessageID=5007
SymbolicName=SCOTAPP_FAIL_PARSE
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   Failed to parse csLocaleCashValueList=%4 or csLocaleCashLowList=%5. 
%nMeaning - Configuration option csLocaleCashValueList and/or csLocaleCashLowList are empty. %n
%nAction - Make sure both lists are defined and have the same number of entries.
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Denominations is used in the 1st parametter
MessageID=5008
SymbolicName=SCOTAPP_TOO_MANY_TENDER_TYPE
Severity=Error
Facility=InputParameter
Language=English
%1-%2@%3   Too many denominations in list <%4> - only the first 7 are accepted %n %n %5
%nMeaning - More than 7 tender denominations have been defined. %n
%nAction - Verify the "Denominations" configuration setting in Scotopts.dat and Scotopts.xxx files.
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5009
SymbolicName=SCOTAPP_DMPROCEDURES_INIT_FAILED
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   Unable to initialize sound path %n %n %4%n
%nMeaning - Validation of registry settings for SoundInitPath & SCOTPriceSoundInit failed. %n
%nAction - Make sure the ScotPriceSound path is correct in the registry
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5010
SymbolicName=SCOTAPP_DMPROCEDURES_SAYAMOUNT
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   SayAmount failed %n %n %4
%nMeaning - Failure while attempting to play amount sound files. %n
%nAction - make sure the speaker devices are installed and operating. 
.
;//-------------------------------------------------------------------------------------------------
;// Called from 2 locations. Parameters at the right not used at this time.
MessageID=5011
SymbolicName=SCOTAPP_DMPROCEDURES_SAYPHRASE
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   SayPhrase failed %n %n %4
%nMeaning - Failure while attempting to play sound file. %n
%nAction - make sure the speaker devices are installed and operating. %n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 2 locations. Parameters at the right not used at this time.
MessageID=5012
SymbolicName=SCOTAPP_DMPROCEDURES_SAYSECURITY
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   SaySecurity failed %n %n %4
%nMeaning - Failure while attempting to play security sound file. %n
%nAction - make sure the speaker devices are installed and operating. 
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5013
SymbolicName=SCOTAPP_DMPROCEDURES_SAYSIGNAL
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   SaySignal failed %n %n %4
%nMeaning - Failure while attempting to play security signal (beep) file. %n
%nAction - make sure the speaker devices are installed and operating. 
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5014
SymbolicName=SCOTAPP_DMPROCEDURES_ENCRYPTORKEY_LENGTH
Severity=Error
Facility=Configuration
Language=English
%1-%2@%3   DMProcedures::StoreEncryptorKey length > MAXENCRYPTORPIN %n %n %4
%nMeaning - Maximum STORE ENCRYPTOR PIN length is exceeded. %n
%nAction - make sure the correct encryptor pin is used
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5015
SymbolicName=SCOTAPP_TIMER_ESTABLISH_FAILED
Severity=Error
Facility=SystemCall
Language=English
%1-%2@%3   ScotApp failed to establish a timer for resending pages %n %n %4
%nMeaning - Unable to set the timer for resending a page. %n
%nAction - Run Get Diag files.
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5016
SymbolicName=SCOTAPP_PAGER_UNAVAILABLE
Severity=Informational
Facility=ThirdPartyInterface
Language=English
%1-%2@%3   SCOT Paging System is unavailable %n %n %4
%nMeaning - The pager server is unavailable at start up. %n
%nAction - If the pager server should be available, check the network connections. If the pager server should NOT be available - then un-install the pager server
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. The state that failed is used in the 1st parameter at the right.
MessageID=5017
SymbolicName=SCOTAPP_STATE_POINT
Severity=Error
Facility=InternalInterface
Language=English
%1-%2@%3   Invalid state pointer %4 %n %n %5
%nMeaning - Fatal "Stop Sign", State = NULL || STATE_STOP || STATE_RETURN || STATE_RESUME || STATE_RESTART || STATE_ENDED || STATE_ERROR || STATE_NULL %n
%nAction - Reboot NT and Run Get Diag files. 
.
;//-------------------------------------------------------------------------------------------------
MessageID=5018
;// Called from 1 location. Parameters at the right not used at this time.
SymbolicName=SCOTAPP_STATE
Severity=Error
Facility=InternalInterface
Language=English
%1-%2@%3   State error indicated %n %n %4
%nMeaning - Next state = STATE_ERROR, an error has occurred - ScotApp is going Out Of Service. %n
%nAction - Run Get Diag files.
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5019
SymbolicName=SCOTAPP_STOP_REQUEST
Severity=Error
Facility=Configuration
Language=English
%1-%2@%3   Stop request %n %n %4
%nMeaning - Next state = STATE_STOP, ScotApp is stopping due to error. %n
%nAction - Run Get Diag files.
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5020
SymbolicName=SCOTAPP_START_REQUEST
Severity=Informational
Facility=StartupInit
Language=English
%1-%2@%3   Restart request %n %n %4
%nMeaning - Next state = STATE_RESTART, ScotApp is restarting. %n
%nAction - Run Get Diag files.
.
;//-------------------------------------------------------------------------------------------------
;// Not Used. Bitmap name that could not be set is used in the 1st parameter at the right
MessageID=5021
SymbolicName=SCOTAPP_MISS_BMP
Severity=Error
Facility=Configuration
Language=English
%1-%2@%3   Missing screen BMP file %4 %n %n %5
%nMeaning - Not able to set the background bmp. %n
%nAction - Make sure the bitmap is in the image directory
.
;//-------------------------------------------------------------------------------------------------
;// Not Used. 
MessageID=5022
SymbolicName=SCOTAPP_PSTHREAD_CREATE
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   Cannot create PSThread %n %n %4
%nMeaning - Fatal"Stop Sign", unable to create PSThread. %n
%nAction - Run Get Diag files%n.
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at right defined in the above list -  "%s|%s|%d|%s|%s|%d", szWho,szWindow,nIdc,szDesc,szText,rc)
MessageID=5023
SymbolicName=SCOTAPP_PSINTERFACE
Severity=Error
Facility=SystemCall
Language=English
%1-%2@%3   Error in PSInterface: %4, %5, ID=%6, %7, %8, RC=%9 %n %n %10
%nMeaning - Fatal "Stop Sign", Called from PSInterface.cpp methods that manipulate window and button controls if any fail to init and the IDC (control id) is not 0 - PSInterface.cpp _Initialize %n
;//		RcError(const int nLine,
;//				const int nLevel,
;//				LPCSTR szWho,		// caller name
;//				LPCSTR szWindow,	// name of window
;//				const int nIdc,		// ID of element or 0
;//				LPCSTR szDesc,		// desc of event
;//				LPCSTR szText,		// formatted text
;//				const PSRC rc)		// rc from PS function call %n %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at right defined in the above list -  "%s|%s|%s|%d", szWho,szDesc,szText,rc)
MessageID=5024
SymbolicName=SCOTAPP_PSINTERFACE2
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   Error in PSInterface: %4, %5, %6, RC=%7 %n %n %8%n
%nMeaning - Fatal"Stop Sign", Called from PSInterface.cpp methods that manipulate window and button controls if any fail and the IDC (control id) is = 0 - PSInterface.cpp _Initialize
;//		RcError(const int nLine,
;//				const int nLevel,
;//				LPCSTR szWho,		// caller name
;//				LPCSTR szWindow,	// name of window
;//				const int nIdc,		// ID of element or 0
;//				LPCSTR szDesc,		// desc of event
;//				LPCSTR szText,		// formatted text
;//				const PSRC rc)		// rc from PS function call %n %n
%nAction - Run Get Diag files.
.
;//-------------------------------------------------------------------------------------------------
;// Called from 4 locations
MessageID=5025
SymbolicName=SCOTAPP_PS_FILE
Severity=Error
Facility=Configuration
Language=English
%1-%2@%3   SCOT could not open file: \%4\ %n %n %5%n
%nMeaning - Informational, unable to open file.  The file is not critical to continuing operations. %n
;// File name is used in the 1st parameter at the right
%nAction - make sure the file name exists and is not corrupt%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 5 locations
MessageID=5026
SymbolicName=SCOTAPP_MISS_ID
Severity=Error
Facility=Configuration
Language=English
%1-%2@%3   Requested ID = %4 was not found, missing(in) %5. %n %n %6
%nMeaning - Bitmap, text or wave ID not defined in configuration files. %n 
;// Wave/text/bmp ID that could not be found is used in the 1st parameter, Filename is used in the 2nd parameter %n
%nAction - Make sure that ID is correctly defined in configuration file.
.
;//-------------------------------------------------------------------------------------------------
;// not in use at this time 
MessageID=5027
SymbolicName=SCOTAPP_MISS_BMP_NAME
Severity=Error
Facility=Configuration
Language=English
%1-%2@%3   Missing BMP name %4 %n %n %5
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Error message is used in the 1st parameter at the right.
MessageID=5028
SymbolicName=SCOTAPP_PSTHREAD_EXCEPTION
Severity=Error
Facility=SystemCall
Language=English
%1-%2@%3   PS Thread CException <%4> %n %n %5
%nMeaning - CException was caught while handling callback events for message object posting user activity (MO.PostPS). %n
%nAction - Software problem, run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.%n
MessageID=5029
SymbolicName=SCOTAPP_PSTHREAD_EXCEPTION2
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   PS Thread exception %n %n %4%n
%nMeaning - Exception was caught while handling callback events for message object posting user activity (MO.PostPS). %n
%nAction - Software problem, run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Msg, Description, and Error are used in 3 of the parameters%n
MessageID=5030
SymbolicName= SCOTAPP_SAINTERFACE_OLEDISPATCH_EXCEPTION
Severity=Error
Facility=InternalInterface
Language=English
%1-%2@%3   %4 COleDispatchException[ %5 ] Code(0x%6) %n %n %7
%nMeaning - COleDispatchException was caught while interfacing with security. %n
%nAction - Software problem, run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Msg, Description, and Error are used in 3 of the parameters%n
MessageID=5031
SymbolicName=SCOTAPP_SAINTERFACE_OLE_EXCEPTION
Severity=Error
Facility=InternalInterface
Language=English
%1-%2@%3   %4 COleException Error[ %5 ] Code(0x%6) %n %n %7
%nMeaning - COleException was caught while interfacing with security. %n
%nAction - Software problem, run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Msg and Description are used in 2 of the parameters%n
MessageID=5032
SymbolicName=SCOTAPP_SAINTERFACE_C_EXCEPTION
Severity=Error
Facility=InternalInterface
Language=English
%1-%2@%3   %4 CException Error[ %5 ] %n %n %6
%nMeaning - CException was caught while interfacing with security. %n
%nAction - Software problem, run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 5 locations. Msg and Error are used in 2 of the parameters at the right%n
MessageID=5033
SymbolicName=SCOTAPP_SAINTERFACE_EXCEPTION
Severity=Error
Facility=InternalInterface
Language=English
%1-%2@%3   %4 catch(...) error = %5 %n %n %6
%nMeaning - Exceptions was caught while interfacing with security. %n
%nAction - Software problem, run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.%n
MessageID=5034
SymbolicName=SCOTAPP_SAINTERFACE_STOP
Severity=Informational
Facility=StopAbort
Language=English
%1-%2@%3   Failed to create SA control %n %n %4
%nMeaning - Failed to create the Security Agent control.%n
%nAction - Software problem, run Get Diag files.%n
.

;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5035
SymbolicName=SCOTAPP_SAPROCEDURE_SENDSCOTINPUT
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   Security SendSCOTInput ERROR %n %n %4
%nMeaning - Unable to send events to security. %n
%nAction - Run Get Diag files. Might temporarily turn on Security DataCapture for more logging.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.%n
MessageID=5036
SymbolicName=SCOTAPP_SAPROCEDURE_MAIN_WINDOW_FAILED
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   Security Agent Procedures main window NOT Created %n %n %4%n
%nMeaning - System Restart (Fatal), if cannot create the main security procedures handle to a window - SAProcedures.cpp  ::Initialize %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5037
SymbolicName=SCOTAPP_SAPROCEDURE_WINDOW_FAILED
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   Security Agent Interface window NOT Created %n %n %4%n
%nMeaning - System Restart (Fatal), Cannot create the security interface handle to a window. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 2 locations. Filename is used in the 1st parameter at the right%n
MessageID=5038
SymbolicName=SCOTAPP_SAPROCEDURE_LOAD_ACTION
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   Cannot load Security actions file %4 %n %n %5%n
%nMeaning - System Restart (Fatal), failed to load the SASIActions.dat file %n
%nAction - Make sure the SASIActions file is installed and not corrupt%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5039
SymbolicName=SCOTAPP_SAINTERFACE_WINDOW_FAILED
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   Security Agent Interface window NOT Created %n %n %4%n
%nMeaning - System Restart (Fatal), cannot create the security interface.%n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5040
SymbolicName=SCOTAPP_SAPROCEDURE_INIT_FAILED
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   Security Agent did Not Initialize %n %n %4%n
%nMeaning - Fatal"Stop Sign", cannot create the main security procedures.%n
%nAction - Run Get Diag files. Might temporarily turn on Security DataCapture for more logging.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5041
SymbolicName=SCOTAPP_SAPROCEDURE_UNINIT
Severity=Error
Facility=StopAbort
Language=English
%1-%2@%3   Security Agent did Not Un-Initialize %n %n %4%n
%nMeaning - Fatal"Stop Sign", failed to uninitialize security interface.%n
%nAction - Run Get Diag files. Might temporarily turn on Security DataCapture for more logging.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5042
SymbolicName=SCOTAPP_SAPROCEDURE_WINDOW_DESTROY
Severity=Error
Facility=StopAbort
Language=English
%1-%2@%3   Security Agent Procedures window was not destroyed %n %n %4%n
%nMeaning - Fatal"Stop Sign", failed attempt to destroy security interface window.%n
%nAction - Run Get Diag files. Reboot NT.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5043
SymbolicName=SCOTAPP_TB_INIT
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   TB - RESTART %n %n %4%n
%nMeaning - Unable to initialize Transaction Broker, restarting ScotApp. %n
%nAction - Transaction Broker problem. Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 6 locations. Parameters at the right not used at this time.
MessageID=5044
SymbolicName=SCOTAPP_CO_INIT
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   CO - RESTART %n %n %4%n
%nMeaning - Unable to load the Config Object option file OR Tender option file. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 4 location. 
MessageID=5045
SymbolicName=SCOTAPP_PS_INIT
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   PS - RESTART %n %n %4%n
%nMeaning - Unable to initialize Presentation Services, restarting ScotApp. %n
;// Parameters at the right not used for these instance.
%nAction - Run Get Diag files. %n 
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5046
SymbolicName=SCOTAPP_DM_INIT
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   DM - RESTART %n %n %4%n
%nMeaning - Unable to Initialize Device Manager, restarting ScotApp. %n

%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5047
SymbolicName=SCOTAPP_SA_INIT
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   SA - RESTART %n %n %4%n
%nMeaning - Unable to Initialize Security Agent, restarting ScotApp. %n
%nAction - Run Get Diag files.
.

;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5048
SymbolicName=SCOTAPP_HT_INIT
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   HT - RESTART %n %n %4%n
%nMeaning - Unable to Initialize Hard Totals. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 2 locations. Parameters at the right not used at this time.
MessageID=5049
SymbolicName=SCOTAPP_UE_INIT
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   UE - RESTART %n %n %4
%nMeaning - User Exits Enabled but this user exit is invalid. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5050
SymbolicName= SCOTAPP_GP_RESTART
Severity=Error
Facility=StopAbort
Language=English
%1-%2@%3   GP - RESTART %n %n %4%n
%nMeaning - General Purpose Notify = RESTART, restarting ScotApp. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Latest Msg ID is used in the 1st parameter at the right
MessageID=5051
SymbolicName=SCOTAPP_BS_UNKNOWN_MSG
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   Base State unknown msg ID: %4 %n %n %5
%nMeaning - Unexpected or unknown message event processed. %n
%nAction - None.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5052
SymbolicName=SCOTAPP_BS_STACK_FULL
Severity=Error
Facility=Resource
Language=English
%1-%2@%3   FIFO Stack is full, lost last state %n %n %4%n
%nMeaning - Base State stack is full, unable to push state onto stack. Deleting state. %n
%nAction - Run Get Diag files. Reboot NT.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. File Name and Error Msg are used in 2 of the parameters
MessageID=5053
SymbolicName=SCOTAPP_BS_JOURNAL
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   Journal File Exception, File handle: %4(invalid!!) %n %n Error text: %5 %n %6
%nMeaning - Base State file exception occurred while printing journal. %n
%nAction - Make sure the Journal File exists and is not corrupt%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 2 locations. File and Error are used in 2 of the parameters at the right
MessageID=5054
SymbolicName=SCOTAPP_BS_RECEIPT
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   Receipt File Exception, File handle: %4(invalid !!) %n %n Error text: %5 %n %6%n
%nMeaning - Base State file exception occurred while printing and saving receipt data. %n
%nAction - Make sure the Receipt File exists and is not corrupt%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Last Evt and Last Parm are used in 2 of the parameters at the right
MessageID=5055
SymbolicName=SCOTAPP_SM_PS_UNKNOWN_MSG
Severity=Error
Facility=InternalInterface
Language=English
%1-%2@%3   PS unknown msg: %4,0x%5  %n %n %6
%nMeaning - Unknown message while processing user events. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Not Used. 
MessageID=5056
SymbolicName=SCOTAPP_SM_SA_IN_CUSTOMER
Severity=Informational
Facility=InternalError
Language=English
%1-%2@%3   Already in SA customer mode %n %n %4%n
%nMeaning - Already in customer mode and sent another customer mode event to security agent. %n
%nAction - none.%n
.
;//-------------------------------------------------------------------------------------------------
;// Not Used. 
;//XMLOFF
MessageID=5057
SymbolicName=SCOTAPP_SM_SA_IN_STORE
Severity=Informational
Facility=InternalError
Language=English
%1-%2@%3   Already in SA store mode %n %n %4%n
%nMeaning - Already in store mode and sent another store mode event to security agent. %n
%nAction - none.%n
.
;//-------------------------------------------------------------------------------------------------
;// Not Used. 
MessageID=5058
SymbolicName=SCOTAPP_SM_SA_NO_MODE
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   SA major mode not set, store/customer %n %n %4%n
%nMeaning - NOT in store mode and NOT in customer mode. %n
%nAction - Run Get Diag files. Might temporarily turn on Security DataCapture for more logging.%n
.
;//-------------------------------------------------------------------------------------------------
;// Not Used. Parameters at the right not used at this time.
MessageID=5059
SymbolicName=SCOTAPP_SM_SA_MISS_ITEM_COMP
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   Missed SA itemization or tender before complete %n %n %4%n
%nMeaning - Error, if NOT in a transaction OR NOT tendering, but received a TRANSACTION_COMPLETE. %n
%nAction - Run Get Diag files. Might temporarily turn on Security DataCapture for more logging.%n
.
;//-------------------------------------------------------------------------------------------------
;// Not Used. Event InputID is used in the 1st parameter at the right
MessageID=5060
SymbolicName=SCOTAPP_SM_SA_MISS_ITEM
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   Missed SA itemization before item entry - %4 %n %n %5%n
%nMeaning - NOT in Itemize, but received a UPC_SCANNED or UPC_MANUAL. %n
%nAction - Run Get Diag files. Might temporarily turn on Security DataCapture for more logging.%n
.
;//-------------------------------------------------------------------------------------------------
;// Not Used. 
MessageID=5061
SymbolicName=SCOTAPP_SM_SA_MISS_ITEM_COMP_ID
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   Missed SA itemization before item complete - %4 %n %n %5%n
%nMeaning - NOT in Itemize, but received a COUPON _REDEEMED, _VOID, OR _REFUSED. %n
%nAction - Run Get Diag files. Might temporarily turn on Security DataCapture for more logging.%n
.
;//-------------------------------------------------------------------------------------------------
;// Not Used.
MessageID=5062
SymbolicName=SCOTAPP_SM_SA_MISS_ITEM_SALE_ID
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   Missed SA item sale before item complete - %4 %n %n %5%n
%nMeaning - NOT in Item Sale, but received a UPC _SCANNED, _MANUAL, ITEM_SOLD, ITEM_VOID or CANCEL, COUPON _REDEEMED, _VOID, or _REFUSED. %n
%nAction - Run Get Diag files. Might temporarily turn on Security DataCapture for more logging.%n
.
;//-------------------------------------------------------------------------------------------------
;// Not Used.
MessageID=5063
SymbolicName=SCOTAPP_SM_SA_MISS_TENDER
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   Missed SA tender before tender start %n %n %4%n
%nMeaning - NOT in Tender state, but customer begins paying. %n
%nAction - Run Get Diag files. Might temporarily turn on Security DataCapture for more logging.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 5 locations. 
MessageID=5064
SymbolicName=SCOTAPP_SM_SA_UNKNOWN_EVENT
Severity=Error
Facility=InternalInterface
Language=English
%1-%2@%3   Unknown SA event ID - %4 %n %n %5%n
%nMeaning - Error, default case when processing unknown security events OR unknown security maintenance mode events. %n
;// the Event InputID is used in the 1st parameter at the right
;// the Event InputID and the result code are used in 2 of the parameters at the right
%nAction - Run Get Diag files. Might temporarily turn on Security DataCapture for more logging.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
;//XMLOFF
MessageID=5065
SymbolicName=SCOTAPP_SM_TB_USERID
Severity=Informational
Facility=InternalError
Language=English
%1-%2@%3   Invalid user ID %n %n %4%n
%nMeaning - Transaction Broker returned TB_E_INVALIDUSERID in response to a TB API call. %n
%nAction - None. User typed in an invalid User ID%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
;//XMLOFF
MessageID=5066
SymbolicName=SCOTAPP_SM_TB_PASSWORD
Severity=Informational
Facility=InternalError
Language=English
%1-%2@%3   Invalid password %n %n %4%n
%nMeaning - Transaction Broker returned TB_E_INVALIDUSERPASSWORD in response to a TB API call. %n
%nAction - None. User typed in an invalid Password%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5067
SymbolicName=SCOTAPP_SM_TB_STATE
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   Invalid procedure to perform at this time %n %n %4%n
%nMeaning - Transaction Broker returned TB_E_INVALIDSTATE, 
%n         indicating that Scotapp issued a TB API call at a time 
%n         that the Transaction Broker could not execute it. %n
%nAction - Possible Transaction Broker problemTransaction broker was not in the correct state to execute an API call made by Scotapp.  Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Filename is used in the 1st parameter at the right
MessageID=5068
SymbolicName=SCOTAPP_SM_TB_FILE
Severity=Error
Facility=FileIO
Language=English
%1-%2@%3   File not found %n %n %4%n
%nMeaning - File exception while performing file io. %n
%nAction - make sure the File exists and is not corrupt%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5069
SymbolicName=SCOTAPP_SM_TB_PROC
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   Procedure not complete %n %n %4%n
%nMeaning - Transaction Broker returned TB_E_INCOMPLETEPROCEDURE in response to a TB API call, indicating that the API has not been implemented. %n
%nAction - Possible Transaction Broker problem. Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5070
SymbolicName=SCOTAPP_SM_TB_WINDOW
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   Invalid window handle passed %n %n %4%n
%nMeaning - Transaction Broker returned TB_E_INVALIDWINDOW in response to a TB API call. %n
%nAction - Possible Transaction Broker problem. Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// parameters at the right not used at this time.
MessageID=5071
SymbolicName=SCOTAPP_SM_TB_DISK
Severity=Error
Facility=Resource
Language=English
%1-%2@%3   Disk full %n %n %4%n
%nMeaning - Transaction Broker returned TB_E_DISKFULL in response to a TB API call. %n
%nAction - Possible hardware problem. Make Insure there is adequate free disk space Sure the hard disk is not full and Restart SCOT%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 4 locations. Parameters at the right not used at this time.
MessageID=5072
SymbolicName=SCOTAPP_SM_TB_PARA
Severity=Error
Facility=InputParameter
Language=English
%1-%2@%3   Invalid parameter(s) %n %n %4%n
%nMeaning - Transaction Broker returned TB_E_INVALIDPARAM in response to a TB API call. %n  
%nAction - Possible Transaction Broker problem. Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5073
SymbolicName=SCOTAPP_SM_TB_CREDIT
Severity=Error
Facility=InternalInterface
Language=English
%1-%2@%3   No host credit %n %n %4%n
%nMeaning - Transaction Broker returned TB_E_NOHOSTCREDIT in response to a TB API call. %n
%nAction - Possible Transaction Broker or Host communication problem. Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5074
SymbolicName=SCOTAPP_SM_TB_WORKINGKEY
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   No Working Key %n %n %4%n
%nMeaning - Transaction Broker returned TB_E_NOWORKINGKEY in response to a TB API call. 
%n          The TB was unable to provide a working key for the PINpad. %n
%nAction - Possible Transaction Broker problem. Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5075
SymbolicName=SCOTAPP_SM_TB_NOTINIT
Severity=Error
Facility=StopAbort
Language=English
%1-%2@%3   TB has not been successfully initialized %n %n %4%n
%nMeaning - Transaction Broker returned TB_E_NOTINITIALIZED in response to a TB API call OR when app tries to UnInitialize TB. The Transaction Broker was not initialized. %n
%nAction - Possible Transaction Broker problem. Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5076
SymbolicName=SCOTAPP_SM_TB_HOST
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   No host application communicated %n %n %4%n
%nMeaning - Transaction Broker returned TB_E_NOHOST in response to a TB API call. The Transaction Broker cannot communicate with the POS application. %n
%nAction - Possible Transaction Broker or POS application problem. Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5077
SymbolicName=SCOTAPP_SM_TB_OTHER
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   Unexpected fatal error. %n %n %4%n
%nMeaning - Transaction Broker returned an unhandled return code (default case). The  Transaction Broker returned an unrecognized value. %n
%nAction - Possible Transaction Broker problem. Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 2 locations. Parameters at the right not used at this time.
;//XMLOFF
MessageID=5078
SymbolicName=SCOTAPP_CARD_PURGED
Severity=Informational
Facility=InternalError
Language=English
%1-%2@%3   Card purged - not removed by customer before timeout %n %n %4%n
%nMeaning - Card Acceptor (MSR) has stored credit/loyalty in purge bin within machine. %n
%nAction - Open lane and retrieve purged card from Card Reader purge bin.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
;//XMLOFF
MessageID=5079
SymbolicName=SCOTAPP_BILL_PURGED
Severity=Informational
Facility=InternalError
Language=English
%1-%2@%3   Bills purged - not removed by customer before timeout %n %n %4%n
%nMeaning - Fastlane has retrieved currency after timeout. %n
%nAction - none.  Open ATM lane and remove bills.%n
.
;//-------------------------------------------------------------------------------------------------
;// Not Used.
MessageID=5080
SymbolicName=SCOTAPP_EO_FILE
Severity=Error
Facility=FileIO
Language=English

%1-%2@%3   Error on file '%4', '%5' error '%6' (%7) %n %n %8
.
;//-------------------------------------------------------------------------------------------------
;// Called from 3 locations. Msg, Description, and Error are used in 3 of the parameters
MessageID=5081
SymbolicName=SCOTAPP_DMINTERFACE
Severity=Error
Facility=DevicePeripheral
Language=English
%1-%2@%3   %4 catch(...)[%5] error = %6 %n %n %7%n
%nMeaning - Exception occurred in Device Manager Interface %n
%nAction - Possible hardware problem, make sure the device is plugged in and activated.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 3 locations. Msg, Description, and Error are used in 3 of the parameters
;//XMLOFF
MessageID=5082
SymbolicName=SCOTAPP_DMINTERFACE2
Severity=Informational
Facility=InternalInterface
Language=English
%1-%2@%3   %4 NOT Enabled @%5 %n %n %6%n
%nMeaning - Device Manager generated exception or IS_NOT_ENABLED_RETURN. %n
%nAction - Possible hardware problem, make sure the device is plugged in and activated.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Enabled status, Description, Device ID and Status are used in 4 of the parameters
;//XMLOFF
MessageID=5083
SymbolicName=SCOTAPP_DMINTERFACE_DEVICE
Severity=Error
Facility=DevicePeripheral
Language=English
%1-%2@%3   Unable to %4 %5 %6 %7 %n %n %8%n
%nMeaning - Device status return code is not equal to 1 or 0. %n
%nAction - Possible hardware problem, make sure the device is plugged in and activated.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5084
SymbolicName=SCOTAPP_DMINTERFACE_DEVICE_CREATE
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   Failed to create DM control %n %n %4%n
%nMeaning - Fatal"Stop Sign", Failed to create Device Manager control. %n
%nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 2 locations. Device name is used in the 1st parameter
MessageID=5085
SymbolicName=SCOTAPP_DMINTERFACE_DEVICE_LOAD
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   Device %4 (%5) is not loaded, issuing CheckHealth %n %n %6%n
%nMeaning - Exception occurred in Device Manager Interface. %n
%nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Device ID is used in the 1st parameter
MessageID=5086
SymbolicName=SCOTAPP_DMINTERFACE_DEVICE_AVAI
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   Failed to get device availability for device %4 %n %n %5%n
%nMeaning - Exception occurred in Device Manager Interface %n
%nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. The number of devices in error is used in the 1st parameter
MessageID=5087
SymbolicName=SCOTAPP_DMINTERFACE_DEVICE_ERROR
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   Still %4 devices in error, Init failed %n %n %5%n
%nMeaning - Devices are in error during Device Manager Interface processing. %n
%nAction - Probable hardware problem, make sure the devices is plugged in and activated.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. The number of devices in error is used in the 1st parameter
MessageID=5088
SymbolicName=SCOTAPP_DMINTERFACE_DEVICE_ERROR2
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   A total of %4 devices were in error, re-checking %n %n %5%n
%nMeaning - Error, if any devices are in error during Device Manager Interface. %n
%nAction - Probable hardware problem, make sure the devices is plugged in and activated.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 2 locations. Parameters at the right not used at this time.
;//XMLOFF
MessageID=5089
SymbolicName=SCOTAPP_DM_CHECK_HEALTH
Severity=Informational
Facility=ThirdPartyInterface
Language=English
%1-%2@%3   DM check health required %n %n %4%n
%nMeaning - CASE_HEALTH_NOW and CASE_HEALTH_L8R while checking the health of a device. %n
%nAction - none.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5090
SymbolicName=SCOTAPP_DM_CASH
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   Invalid Cash insert amount <%4> %n %n %5%n
%nMeaning - Cash inserted event showed cash inserted is < zero. %n
%nAction - Possible hardware problem, make sure the devices is plugged in and activated. Run Get Diag Files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5091
SymbolicName= SCOTAPP_DM_COIN
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   Invalid Coin insert amount <%4> %n %n %5%n
%nMeaning - Coin inserted event shows amount inserted is < zero. %n
%nAction - Possible hardware problem, make sure the devices is plugged in and activated. Run Get Diag Files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Last Msg, Last Dev, Last Code and Last Length are used in 4 of the parameters
;//XMLOFF
MessageID=5092
SymbolicName=SCOTAPP_DM_CSWORK
Severity=Informational
Facility=InternalError
Language=English
%1-%2@%3   DM Unsupported event %4, device %5, code %6 length %7 %n %n %8%n
%nMeaning - Scotapp received a device event it did not recognize. %n
%nAction - none.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 3 locations. Last Msg, Description, Error and Dev Class ID are used in 4 of the parameters
MessageID=5093
SymbolicName=SCOTAPP_DMINTERFACE_OLE
Severity=Error
Facility=DevicePeripheral
Language=English
%1-%2@%3   %4 COleDispatchException[ %5 ] Codes(0x%6), device %7 (%8) %n %n %9%n
%nMeaning - Report Exception occured & the Device Manager is opened. %n
%nAction - Possible hardware problem, make sure the devices is plugged in and activated. Run Get Diag Files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5094
SymbolicName=SCOTAPP_PSITEMLIST_INSERT
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   Insert in order error %n %n %4%n
%nMeaning - Memory Exception occurred while sorting the item list. %n
%nAction - none.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Last Msg, Description, Error and Dev Class ID are used in 4 of the parameters
MessageID=5095
SymbolicName=SCOTAPP_DMINTERFACE_EXCEPTION
Severity=Error
Facility=InternalInterface
Language=English
%1-%2@%3   %4 CException[ %5 ], device %6 (%7) %n %n %8%n
%nMeaning - Fatal"Stop Sign", Ole exception occurred. %n
%nAction - Possible hardware problem, make sure the devices is plugged in and activated. Run Get Diag Files.
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Last Msg is in 1st parameter at right
MessageID=5096
SymbolicName=SCOTAPP_DM_MESSAGE
Severity=Error
Facility=InputParameter
Language=English
%1-%2@%3   Invalid DM Message type %4 %n %n %5%n
%nMeaning - Invalid Device Manager message. %n
%nAction - Possible hardware problem, make sure the devices is plugged in and activated. Run Get Diag Files.
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5097
SymbolicName=SCOTAPP_RA_INIT
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   RA initialize error %n %n %4%n
%nMeaning - Failed to initialize RAP at startup. %n
%nAction - Check the network connection to the RAP server. Make sure RAP installed properly.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 2 locations. Path and filename in 1st parameter at right
;//XMLOFF
MessageID=5098
SymbolicName=SCOTAPP_DM_EVENT
Severity=Informational
Facility=Configuration
Language=English
%1-%2@%3   DM event occurred: %n %n %4%n
%nMeaning - Unhandled device event. %n
%nAction - none%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5099
SymbolicName=SCOTAPP_PREV_INSTANCE
Severity=Error
Facility=StopAbort
Language=English 
%1-%2@%3   Previous instance of SCOT Application exists.%n%n
%nMeaning: SCOT Application wasn't shutdown properly before. Previous instance of application still exists.%n
%nAction:  Kill previous instance of SCOT Application and reboot before you start application.%n%n%4%n

.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
;//XMLOFF
MessageID=5100
SymbolicName=SCOTAPP_STOP_REQUEST_FROM_DIALOG
Severity=Informational
Facility=Configuration
Language=English
%1-%2@%3   SCOTAPP Dialog sent a request to stop Fastlane. %4%n
%nMeaning - The Fastlane application was shut down using ScotAppDialog. %n
%nAction - none%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
;//XMLOFF
MessageID=5101
SymbolicName=SCOTAPP_STOP_REQUEST_FROM_LOADCONTROLLER
Severity=Informational
Facility=Configuration
Language=English
%1-%2@%3   Load Controller sent  a request to stop Fastlane. %4%n
%nMeaning - The load controller program sent a shut down message to Fastlane app. %n
%nAction - none%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 3 locations. Path and filename in 1st parameter at right
MessageID=5102
SymbolicName=SCOTAPP_NULL_RECEIPT
Severity=Informational
Facility=InternalInterface
Language=English
%1-%2@%3   SCOT App got no receipt data. File handle from TBGetReceiptData(): %4, File length:%5.%n %6%n
%nMeaning - No receipt data recieved for printing. %n
%nAction - make sure the file and is not corrupt%n 
.
;//-------------------------------------------------------------------------------------------------
;// Called from 4 locations. The Transaction Broker message and error message are used in the 1st two parameters at right
MessageID=5103
SymbolicName=SCOTAPP_GOT_TBEXCEPTION
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   SCOT App got an exception from TransactionBroker: %4, ErrorMessage:%5 %n %6 %n
%nMeaning - Transaction Broker Exception when trying to restart the app. %n
%nAction - Possible Transaction Broker problem. Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Total sale amt and if item is waiting are used in the 1st two parameters at right
MessageID=5104
SymbolicName=SCOTAPP_GOT_SAFALSETRXSTART
Severity=Warning
Facility=InternalError
Language=English
%1-%2@%3   'False Transaction Start' from Security Agent: Trx total:%4, Pending items:%5 %n %6 %n
%nMeaning - Fastlane received EndOfTransaction from Security agent while still in a transaction. %n
%nAction - None. %n
.
;//-------------------------------------------------------------------------------------------------
;// Called 5 times.  The config/opt file not found/opened is printed in the 1st parameter
MessageID=5105
SymbolicName=SCOTAPP_MISS_FILE
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   %4 was missing.%n
%nMeaning - Fatal"Stop Sign", if unable to find/open one of the configuration or options files. App immediately stops. %n
%nAction - Check that the file is installed and not corrupted%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. The option name is used in one of the parameters.
MessageID=5106
SymbolicName=SCOTAPP_DUAL_LANGUAGE_BAD_SETUP
Severity=Error
Facility=Configuration
Language=English
%1-%2@%3   Missing or incorrect option [%4] %5, disabling Dual Language feature. Option should be of the format %5=2 character ISO 639 code, Button name, message file name, tare description file name, security conclusions file%n
%nMeaning - DualLanguage=Y but scotopts.xxx does not define both lanquages, or Language code is not 2 characters in length. %n
%nAction - make sure language configuration is correct %n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5107
SymbolicName=SCOTAPP_DF_INIT
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   SCOT Device Factory initialize error %n %n %4%n
%nMeaning - Unable to Initialize the SCOT Device Factory. %n
%nAction - Probable hardware problem, make sure the devices are plugged in and activated.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5108
SymbolicName=SCOTAPP_SE_INIT
Severity=Error
Facility=StartupInit

Language=English
%1-%2@%3   SCOT Encryptor initialize error %n %n %4%n
%nMeaning - Unable to Initialize SCOT Encryptor with SCOT Device Factory. %n
%nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5109
SymbolicName=SCOTAPP_SC_INIT
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   SCOT Signature Capture initialize error %n %n %4%n
%nMeaning - Unable to Initialize SCOT Signature Capture with SCOT Device Factory. %n
%nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5110
SymbolicName=SCOTAPP_AD_INIT
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   SCOT Aux Display initialize error %n %n %4%n
%nMeaning - Unable to Initialize SCOT Auxilliary Display with SCOT Device Factory. %n
%nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
;//XMLOFF
MessageID=5111
SymbolicName=SCOTAPP_PAGER_INITIALIZED
Severity=Informational
Facility=ThirdPartyInterface
Language=English
%1-%2@%3   SCOT Paging System is initialized %n %n %4%n
%nMeaning - The pager service initialized without error. %n
%nAction - none.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Sent status & message are used in the 1st 2 parameters at right.
;//XMLOFF
MessageID=5112
SymbolicName=SCOTAPP_SEND_PAGE
Severity=Informational
Facility=ThirdPartyInterface
Language=English
%1-%2@%3   SCOT Paging System is sending a page.%nPage sent successfully: %4%nMessage: %5%n %n %6%n
%nMeaning - The pager service successfully sent a page. %n
%nAction - none.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. the handle to Dev Factory window & Parent window are used in the 1st 2 parameters at right.
MessageID=5113
SymbolicName=SCOTAPP_DF_NO_WND
Severity=Error
Facility=Configuration
Language=English
%1-%2@%3   Unable to create Device Factory window (ID=%4).%nThe parent window ptr is %5.%n %6%n
%nMeaning - Device Factory Initialize cannot create a window. %n
%nAction - Reboot NT and Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called 7 times. The access action & key value are used in the 1st 2 parameters at right.
MessageID=5114
SymbolicName=SCOTAPP_REG_ACCESS_ERROR
Severity=Error
Facility=Configuration
Language=English
%1-%2@%3   Got error %4 %5 registry key %6.%n%n %7%n
%nMeaning - Error, if an error occurs while accessing a registry value. %n
%nAction - Check that the registry values are not corrupt%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. The device class is used in the 1st parameter.
MessageID=5115
SymbolicName=SCOTAPP_DF_NO_DEV_FOUND
Severity=Error
Facility=Configuration
Language=English
%1-%2@%3   No device was found where 'Configure=Yes' for device class %4.%n%n %5%n
%nMeaning - Device not found but the device class exists. %n
%nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. The device class & device status are used in the 1st 2 parameters.
MessageID=5116
SymbolicName=SCOTAPP_DF_UNKNOWN_DATA_EVENT
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   Received data event for Unknown device (Device Class %4), Status = %5.%n%n %6%n
%nMeaning - Unknown Device sent an event message to the app. %n
%nAction - Probable hardware or configuration problem.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. The device & result code are used in the 1st 2 parameters on the right.
MessageID=5117
SymbolicName=SCOTAPP_DF_ERROR_EVENT
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   Received error event for %4 - result code = %5, extended result code = %6.%n%n %7%n
%nMeaning - The device sent an error message. %n
%nAction - Probable hardware problem.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Language is used in the 1st parameter on the right
MessageID=5118
SymbolicName=SCOTAPP_AD_INVALID_LANG
Severity=Error
Facility=InternalInterface
Language=English
%1-%2@%3   SetLanguage() received non-Primary language code %4, but dual-language is not 
configured.%n%n %5%n
%nMeaning - Dual language not configured, but a SetLanguage call was made with a language 
code that does not match the primary language. %n
%nAction - Software issue.  Get Diag files%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. The primary language & current language are used in the 1st 2 parameters on the right.
MessageID=5119
SymbolicName=SCOTAPP_AD_LANG_NOT_FOUND
Severity=Error
Facility=Configuration
Language=English
%1-%2@%3   Language code '%4' not found in language array, using default of '%5'.%n%n %6%n
%nMeaning - The auxilliary device was instructed to set the language to an undefined language code. %n
%nAction - make sure language code is correct in SCOTopts%n
.
;//-------------------------------------------------------------------------------------------------
;// Called 3 times. The screen id is used in the 1st parameter.
MessageID=5120
SymbolicName=SCOTAPP_AD_CURRENT_SCREEN_NULL
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   %4 failed because m_pCurrentScreen is NULL.%n%n %5%n
%nMeaning - The auxilliary device or IVI forms received a null pointer for screen display. %n
%nAction - Software Problem. Get Diag Files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called 8 times. device information is used in the parameter to the right

MessageID=5121
SymbolicName=SCOTAPP_OPOS_CTRL_INIT_FAILURE
Severity=Error
Facility=DevicePeripheral
Language=English
%1-%2@%3   Fastlane Device failed to initialize %4. The following function failed: %5.%n%n %6%n
%nMeaning - A device factory device failed to initialize. %n
%nAction - Probable hardware problem, make sure the devices are plugged in and activated.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 2 locations. The data attempted to store, the array index, and the memory return code are used in 3 of the parameters
MessageID=5122
SymbolicName=SCOTAPP_AD_FORM_LOAD_FAILURE
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   Form %4 (slot %5) failed to load - rc = %6.%n%n %7%n
%nMeaning - Unable to store data on the auxilliary device. %n
%nAction - Probable hardware problem, make sure the devices are plugged in and activated.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Device status is used in the 1st parameter at the right
MessageID=5123
SymbolicName=SCOTAPP_AD_RESET_DEVICE_FAILURE
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   Forms device failed to reset - bAlive = %4.%n%n %5%n
%nMeaning - Check health failed to reset device. %n
%nAction - Probable hardware problem, make sure the devices are plugged in and activated.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Device profile is used in the 1st parameter at the right
MessageID=5124
SymbolicName=SCOTAPP_AD_LOAD_CFGFILE_FAILURE
Severity=Error
Facility=Configuration
Language=English
%1-%2@%3   Could not open config file %4.%n%n %5%n
%nMeaning - Unable to load forms config file. %n
%nAction - Probable hardware problem, make sure the devices are plugged in and activated.%n
.

;//-------------------------------------------------------------------------------------------------
;// Called from 2 locations. form profile & screen number are used in the 1st 2 parameters at the right
MessageID=5125
SymbolicName=SCOTAPP_AD_SCREEN_CREATE_ERROR
Severity=Error
Facility=DevicePeripheral
Language=English
%1-%2@%3   AuxScreen %4 (%5) failed to create/initialize.%n
If the EFT device is an NCR 5992 Signature Capture device, after correcting 
the problem, set the registry value FormsForceOneReload=Yes in key
HKLM\Software\NCR\SCOT\CurrentVersion\SCOTAPP. This
will cause the forms to be reloaded next time the application is reloaded.%n %6%n
%nMeaning - Aux Screen = NULL while reading in forms. %n
%nAction - Probable hardware problem, make sure the devices are plugged in and activated.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. The form name, screen num, language, and cfgFile are used in 4 of the parameters.
MessageID=5126
SymbolicName=SCOTAPP_AD_SCREEN_FILE_NOT_EXIST
Severity=Error
Facility=Configuration
Language=English
%1-%2@%3   Form file %4 (Number %5) in language section %6 does not exist
in config file %7.%n
After correcting the problem, set the registry value FormsForceOneReload=Yes in key
HKLM\Software\NCR\SCOT\CurrentVersion\SCOTAPP. This
will cause the forms to be reloaded next time the application is reloaded.%n %n %8%n
%nMeaning - Unable to get the status of a screen form file. %n
%nAction - make sure the auxilliary display screen files are installed and not corrupt.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 2 locations. The screen num, language, and cfgFile are used in 3 of the parameters.
MessageID=5127
SymbolicName=SCOTAPP_AD_SCREEN_EMPTY_ENTRY
Severity=Error
Facility=Configuration
Language=English
%1-%2@%3   Entry number %4 in language section %5 does not exist
in config file %6.%n
If the EFT device is an NCR 5992 Signature Capture device, after correcting 
the problem, set the registry value FormsForceOneReload=Yes in key
HKLM\Software\NCR\SCOT\CurrentVersion\SCOTAPP. This
will cause the forms to be reloaded next time the application is reloaded.%n %7%n
%nMeaning - The entry is empty in a screen form file. %n
%nAction - Make sure the auxilliary display screen files are installed and not corrupt.%n

.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Status, Form File, Screen are used in 3 of the parameters at the right
MessageID=5128
SymbolicName=SCOTAPP_AD_SCREEN_UNHANDLED_CONTROL_DATA
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   Unhandled data event for control ID %4, form %5 (%6).%n%n %7%n
%nMeaning - The data event status id is out of range. %n
%nAction - Software Problem. Run Get Diag Files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Pointer to the Text and the Formatted Text are used in the 1st 2 parameters at right.
MessageID=5129
SymbolicName=SCOTAPP_AD_VARTEXT_FORMAT_ERROR
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   Unable to Format variable data %4 in specifier %5.%n%n %6%n
%nMeaning - Caught an exception while formatting text for auxilliary screen, most likely it is because text is invalid. %n
%nAction - Run Get Diag files %n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. FormFile & TextVal are used in the 1st 2 parameters at right.
MessageID=5130
SymbolicName=SCOTAPP_AD_VARTEXT_VALIDATE_ERROR
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   Unable to validate that variable data form %4 format-specifier %5 
contains one and only one string format specifier.%n%n %6%n
%nMeaning - The variable text is invalid. %n
%nAction - Run Get Diag files %n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Form File is used in the 1st parameter
MessageID=5131
SymbolicName=SCOTAPP_AD_VARTEXT_PARSE_OPEN_ERROR
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   ParseForm() unable to open variable data form %4.%n%n %5%n
%nMeaning - Error during open of variable data form after successful validation of form data. %n
%nAction - Run Get Diag files %n
.
;//------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters identify the failure information
MessageID=5132
SymbolicName=SCOTAPP_SE_PIN_TRANS_FAILED
Severity=Error
Facility=ThirdPartyInterface
Language=English
%1-%2@%3   DoPINTransaction failed in %4, rc = %5, rce = %6.%n
Acct#=%7, Working Key=%8, PinBlockFormat=%9, EncryptionType=%10, Slot=%11.%n %12%n
%nMeaning - PIN transaction failed. %n
%nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Prompt Text, return code, OPOS Prompt Num, IO Prompt Num are used in 4 of the parameters
MessageID=5133
SymbolicName=SCOTAPP_SE_SET_PROMPT_FAILED
Severity=Error
Facility=ThirdPartyInterface
Language=English
%1-%2@%3   SetPromptText of '%4' failed, rc=%5. OPOS Prompt#=%6, DirectIO Prompt#=%7.%n%n %8%n
%nMeaning - Unable to set encryptor prompt text thru direct IO. %n
%nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Calling function, return code and capture form are used in 3 of the parameters
MessageID=5134
SymbolicName=SCOTAPP_SC_BEGCAP_FAILED
Severity=Error
Facility=ThirdPartyInterface
Language=English
%1-%2@%3   Start Capture failed in function '%4' - rc=%5. SigCap form name was %6.%n%n %7%n
%nMeaning - Failure occurred when beginning signature capture. %n
%nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5135
SymbolicName=SCOTAPP_SC_DISPWND_NULL
Severity=Error
Facility=ThirdPartyInterface
Language=English
%1-%2@%3   The SigCap display window handle was NULL.%n%n %7%n
%nMeaning - Invalid SigCap display window handle was passed. %n
%nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Prompt number and Prompt information used in 2 parameters.
MessageID=5136
SymbolicName=SCOTAPP_AD_OPOSPROMPT_OUTOFRANGE
Severity=Error
Facility=Configuration
Language=English
%1-%2@%3   The OPOS prompt number %4 for prompt %5 was out of range.%n%n %6%n
%nMeaning - The auxilliary display prompt number is zero or greater than the maximum value. %n
%nAction -Run Get Diag files%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5137
SymbolicName=SCOTAPP_SM_TB_FUNCTION_NOT_SUPPORTED
Severity=Error
Facility=ThirdPartyInterface
Language=English
%1-%2@%3   Transaction Broker does not support the function %4 %n %n %5%n
%nMeaning - Transaction Broker returned TB_E_FUNCTIONNOTSUPPORTED. %n
%nAction - Run Get Diag files%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 2 locations. Signature array index used in 1st parameter
MessageID=5138
SymbolicName=SCOTAPP_SC_DATA_MISSING
Severity=Error
Facility=ThirdPartyInterface
Language=English
%1-%2@%3   %4 Data is missing from signature receipt file. %n  Receipt Index: %5 %n %n %6%n
%nMeaning - Signature data missing during SLIP TOKEN or SIG TOKEN. %n
%nAction - make sure the signature file is not corrupt%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 2 locations. Signature array index used in 1st parameter
;// Called from 1 location. Return code used in the 1st parameter
MessageID=5139
SymbolicName=SCOTAPP_SC_ENDCAP_FAILED
Severity=Error
Facility=ThirdPartyInterface
Language=English
%1-%2@%3   End Capture failed  - rc=%4.%n%n %5%n
%nMeaning - Failure while ending signature capture. %n
%nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 2 locations. Display Coin Dispenser Status used in parameters
;//XMLOFF
MessageID=5140
SymbolicName=SCOTAPP_DM_INFO
Severity=Informational
Facility=DevicePeripheral
Language=English
%1-%2@%3   <%4> %n %n 
%nMeaning - Coin dispenser status after refill while in Store Mode/Lane Status screen, or status after System Message Screen. %n
%nAction - Make sure the coins are really refilled!%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5141
SymbolicName=SCOTAPP_TB_NOTALLOWED_INTRAINING
Severity=Warning
Facility=ThirdPartyInterface
Language=English
%1-%2@%3 Transaction Broker call, TBIsAllowed(TB_FT_TRAININGMODE) returned false while inside training mode.%n
SCOTApp ignores this return value and allows its user to press 'Leave Training Mode' button.%n
%nMeaning - In training mode and TB doesn't allow it. %n
%nAction - Transaction Broker should recognize and allow Training Mode%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 2 locations. Journal file name and length are used in 1st 2 parameters
MessageID=5142
SymbolicName=SCOTAPP_NULL_JOURNALDATA
Severity=Informational
Facility=FileIO
Language=English
%1-%2@%3   No journal data. File handle from TBGetJournalData(): %4, File length:%5.%n %6%n
%nMeaning - File has a valid file handle but a zero length. %n
%nAction - Make sure the journal file is not corrupt%n 

.
;//-------------------------------------------------------------------------------------------------
;// Called from 2 locations. the value of the option: ForceWeighingOFPriceEmbedded is used in the 1st parameter
MessageID=5143
SymbolicName=SCOTAPP_FORCED_TO_OVERRIDE_OPTION
Severity=Informational
Facility=Configuration
Language=English
%1-%2@%3   Fastlane forced to override an option: \%4\ %n %n %5%n
%nMeaning - Configuration specifies ForceWeighingOFPriceEmbedded=Y but there is no scale, overriding with ForceWeighingOFPriceEmbedded=N. %n
%nAction - If the scanner does not have a scale, then the SCOTOpts option ForceWeighingOFPriceEmbedded should be set to N%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Device status id used in the 1st parameter
MessageID=5144
SymbolicName=SCOTAPP_DEVFACTORY_MOTION_SENSOR_FAILED
Severity=Error
Facility=DevicePeripheral
Language=English
%1-%2@%3   SCOT forced to simulate motion begin and ended due to Motion Sensor Failure: %n %n %5%n
%nMeaning - Motion Sensor failure event received. %n
%nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Security System status id used in the 1st parameter
MessageID=5145
SymbolicName=SCOTAPP_SM_SA_STATUS_EVENT
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   Security Agent reported a status event of: %n %4%n
%nMeaning - Security sent a status update event. Not necessarily failure. %n
%nAction - If no failure - none.  If failure, check the network connection to the security server.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5146
SymbolicName=SCOTAPP_MS_INIT
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   SCOT Motion Sensor initialize error %n %n %4%n
%nMeaning - Failure while initializing Motion Sensor device. %n
%nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5147
SymbolicName=SCOTAPP_ED_INIT
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   SCOT EAS Deactivator initialize error %n %n %4%n
%nMeaning - Failure while initializing Electronic Article Surveillance Deactivator device. %n
%nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 locations. Section, Key, Default text messages are used in 1st 3 parameters
MessageID=5148
SymbolicName=SCOTAPP_INVALID_CURRENCY_COUNTRY_CODE
Severity=Informational
Facility=Configuration
Language=English
%1-%2@%3   Invalid registry value [%4] %5, using default <%6> %n %n %7%n
%nMeaning - Currency country code is invalid, default currency USD bitmap will be used %n
%nAction - if the default value is not desired, set the option.%n
.
;//-------------------------------------------------------------------------------------------------
MessageID=5149
SymbolicName=SCOTAPP_DBMPROCEDURE_MAIN_WINDOW_FAILED
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   Client Database Manager Procedures main window NOT Created %n %n %4%n
%nMeaning - System Restart (Fatal), cannot create the main security procedures handle to a window. %n
;// Called from 1 location. Parameters at the right not used at this time.
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5150
SymbolicName=SCOTAPP_DBMPROCEDURE_WINDOW_FAILED
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   Client Database Manager Interface window NOT Created %n %n %4%n
%nMeaning - System Restart (Fatal), cannot create the security interface handle to a window. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5151
SymbolicName=SCOTAPP_DBMPROCEDURE_INIT_FAILED
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   Security Agent did Not Initialize %n %n %4%n
%nMeaning - Fatal"Stop Sign", cannot create the main security procedures. %n
%nAction - Run Get Diag files. Might temporarily turn on Security DataCapture for more logging.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5152
SymbolicName=SCOTAPP_DBMINTERFACE_WINDOW_FAILED
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   Client Database Manager Interface window NOT Created %n %n %4%n
%nMeaning - System Restart (Fatal), cannot create the security interface. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5153
SymbolicName=SCOTAPP_DBMPROCEDURE_UNINIT
Severity=Error
Facility=StopAbort
Language=English
%1-%2@%3   Client Database Manager did Not Un-Initialize %n %n %4%n
%nMeaning - Fatal"Stop Sign", failed to un-init security interface. %n
%nAction - Run Get Diag files. Might temporarily turn on Security DataCapture for more logging.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5154
SymbolicName=SCOTAPP_DBMPROCEDURE_WINDOW_DESTROY
Severity=Error
Facility=StopAbort
Language=English
%1-%2@%3   Client Database Manager Procedures window was not destroyed %n %n %4%n
%nMeaning - Fatal"Stop Sign", failed to DestroyWindow security interface. %n
%nAction - Run Get Diag files. Reboot NT.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5155
SymbolicName=SCOTAPP_SSPROCEDURE_MAIN_WINDOW_FAILED
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   Smart Scale Procedures main window NOT Created %n %n %4%n
%nMeaning - System Restart (Fatal), if cannot create the main security procedures handle to a window. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5156
SymbolicName=SCOTAPP_SSPROCEDURE_WINDOW_FAILED
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3    Smart Scale Interface window NOT Created %n %n %4%n
%nMeaning - System Restart (Fatal), cannot create the security interface handle to a window. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Not Used.
MessageID=5157
SymbolicName=SCOTAPP_SSPROCEDURE_INIT_FAILED
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   Smart Scale did Not Initialize %n %n %4%n
%nMeaning - Fatal"Stop Sign", cannot create the main security procedures. %n
%nAction - Run Get Diag files. Might temporarily turn on Security DataCapture for more logging.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5158
SymbolicName=SCOTAPP_SSINTERFACE_WINDOW_FAILED
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   Smart Scale Interface window NOT Created %n %n %4%n
%nMeaning - System Restart (Fatal), cannot create the security interface. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5159
SymbolicName=SCOTAPP_SSPROCEDURE_UNINIT
Severity=Error
Facility=StopAbort
Language=English
%1-%2@%3   Smart Scale did Not Un-Initialize %n %n %4%n
%nMeaning - Fatal"Stop Sign", failed to un-init security interface. %n
%nAction - Run Get Diag files. Might temporarily turn on Security DataCapture for more logging.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5160
SymbolicName=SCOTAPP_SSPROCEDURE_WINDOW_DESTROY
Severity=Error
Facility=StopAbort
Language=English
%1-%2@%3   Smart Scale Procedures window was not destroyed %n %n %4%n
%nMeaning - Fatal"Stop Sign", failed to DestroyWindow security interface. %n
%nAction - Run Get Diag files. Reboot NT.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5161
SymbolicName=SCOTAPP_WTINFOPROCEDURE_INIT_FAILED
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   Weight Infomation dll did Not Initialize %n %n %4%n
%nMeaning - Fatal"Stop Sign", cannot create the main security procedures. %n
%nAction - Run Get Diag files. Might temporarily turn on Security DataCapture for more logging.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Msg, Description, and Error are used in 3 of the parameters
MessageID=5162
SymbolicName= SCOTAPP_SSINTERFACE_OLEDISPATCH_EXCEPTION
Severity=Error
Facility=InternalInterface
Language=English
%1-%2@%3   %4 COleDispatchException[ %5 ] Code(0x%6) %n %n %7%n
%nMeaning - COleDispatchException caught while interfacing with security. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Msg, Description, and Error are used in 3 of the parameters
MessageID=5163
SymbolicName=SCOTAPP_SSINTERFACE_OLE_EXCEPTION
Severity=Error
Facility=InternalInterface
Language=English
%1-%2@%3   %4 OleException Error[ %5 ] Code(0x%6) %n %n %7%n
%nMeaning - OleException caught while interfacing with security. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Msg and Description are used in 2 of the parameters
MessageID=5164
SymbolicName=SCOTAPP_SSINTERFACE_C_EXCEPTION
Severity=Error
Facility=InternalInterface
Language=English
%1-%2@%3   %4 CException Error[ %5 ] %n %n %6%n
%nMeaning - CException caught while interfacing with security. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 5 locations. Msg and Error are used in 2 of the parameters at the right
MessageID=5165
SymbolicName=SCOTAPP_SSINTERFACE_EXCEPTION
Severity=Error
Facility=InternalInterface
Language=English
%1-%2@%3   %4 catch(...) error = %5 %n %n %6%n
%nMeaning - Exception caught while interfacing with security. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Msg, Description, and Error are used in 3 of the parameters
MessageID=5166
SymbolicName=SCOTAPP_DBMINTERFACE_OLEDISPATCH_EXCEPTION
Severity=Error
Facility=InternalInterface
Language=English
%1-%2@%3   %4 COleDispatchException[ %5 ] Code(0x%6) %n %n %7%n
%nMeaning - COleDispatchException caught while interfacing with security database. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Msg, Description, and Error are used in 3 of the parameters
MessageID=5167
SymbolicName=SCOTAPP_DBMINTERFACE_OLE_EXCEPTION
Severity=Error
Facility=InternalInterface
Language=English
%1-%2@%3   %4 COleException Error[ %5 ] Code(0x%6) %n %n %7%n
%nMeaning - COleException caught while interfacing with security database. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Msg and Description are used in 2 of the parameters
MessageID=5168
SymbolicName=SCOTAPP_DBMINTERFACE_C_EXCEPTION
Severity=Error
Facility=InternalInterface
Language=English
%1-%2@%3   %4 CException Error[ %5 ] %n %n %6%n
%nMeaning - CException caught while interfacing with security database. %n
%nAction - Run Get Diag files.%n 
.
;//-------------------------------------------------------------------------------------------------
;// Called from 5 locations. Msg and Error are used in 2 of the parameters at the right
MessageID=5169
SymbolicName=SCOTAPP_DBMINTERFACE_EXCEPTION
Severity=Error
Facility=InternalInterface
Language=English
%1-%2@%3   %4 catch(...) error = %5 %n %n %6%n
%nMeaning - Exception caught while interfacing with security database. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5170
SymbolicName=SCOTAPP_TAB_INIT
Severity=Error
Facility=DevicePeripheral
Language=English
%1-%2@%3   Takeaway Belt initialize error %n %n %4%n
%nMeaning - Failure while initializing Takeaway Belt control. %n
%nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
.
;//-------------------------------------------------------------------------------------------------
MessageID=5171
SymbolicName=SCOTAPP_SSINTERFACE_STOP
Severity=Informational
Facility=StopAbort
Language=English
%1-%2@%3   Failed to create SmartScale control %n %n %4%n
%nMeaning - Failure while trying to create the SS control. %n
;// Called from 1 location. Parameters at the right not used at this time.
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5172
SymbolicName=SCOTAPP_DBMINTERFACE_STOP
Severity=Informational
Facility=StopAbort
Language=English
%1-%2@%3   Failed to create DBM control %n %n %4%n
%nMeaning - Failure while trying to create the DBM control. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5173
SymbolicName=SCOTAPP_OPERATORLOGINBARCODEMASK
Severity=Informational
Facility=InputParameter
Language=English
%1-%2@%3   OperatorLoginBarcodeMask is incorrect %n %n %4%n
%nMeaning - OperatorLogin Barcode is incorrect. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
MessageId=5174
SymbolicName=FLR_CREATE_INSTANCE_FAILURE
Severity=Error
Facility=Configuration
Language=English
Method %1 reports Internal software error %2 - Unable to create an instance of the DOM.  The Document was not loaded.
%nMeaning - Unable to create an instance of the DOM.  The Document was not loaded.%n
%nAction - The problem may be the result of improper installation of the XML parser.  Reinstall XML parser, which is part of the reporting installation procedure.%n
.
;//-------------------------------------------------------------------------------------------------
MessageId=5175
SymbolicName=FLR_XML_FAILURE
Severity=Error
Facility=InternalError
Language=English
Method %1 reports internal software error - Unspecified Failure.  Occurs when a function call fails or
when required data is not present. Exception was caught.%n
%nMeaning - "Unspecified Failure.  Occurs when a function call fails or when required data is not present. Exception was caught".%n
%nAction - This is logged for one of many internal software errors.  Inform NCR support of the details of the error message.%n
.
;//-------------------------------------------------------------------------------------------------
MessageId=5176
SymbolicName=FLR_LOAD_XML_FAILURE
Severity=Error
Facility=InternalError
Language=English
Method %1 reports error - Unable to successfully load the DOM document.
%nMeaning -  Unable to successfully load the DOM document.%n
%nAction - This is an internal software error that should not be encountered.  If this error is reported, it should be forwarded to NCR support for analysis.%n
.
;//-------------------------------------------------------------------------------------------------
MessageId=5177
SymbolicName=FLR_COM_ERROR
Severity=Error
Facility=InternalError
Language=English
Method %1 detected COM Error %n
Source - %2 %n
Description - %3 %n
Error Code - %4 %n
Message - %5 
%nMeaning - Caught exception (_com_error e) while interfacing with Reporting -  PROCESS_COM ERRORS. %n
%nAction - This error references a number of possible errors.   This may be caused by formatting problems with various XML files or other COM errors.  Forward to NCR support for analysis of the specific problem.%n
.
;//-------------------------------------------------------------------------------------------------
MessageId=5178
SymbolicName=SCOTAPP_PSX_CREATE
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   Cannot create PSX Object %n %n %4
%nMeaning - Cannot create PSX Object%n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
MessageId=5179
SymbolicName=FLR_OLE_ERROR
Severity=Error
Facility=SystemCall
Language=English
Method %1 detected OLEDispatch Error %n
Source - %2 %n
Description - %3 %n
Error Code - %4 %n
%nMeaning - caught exception (OleException) while interfacing with Reporting - PROCESS_EXCEPTIONS%n
%nAction - An internal error has been reported.  This is a general error event.  Forward the event information to NCR Support for further analysis.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. SCOTopts array type in 1st param. (e.g. CashBack)
MessageID=5180
SymbolicName=SCOTAPP_NUM_ARRAY_OPTIONS
Severity=Warning
Facility=InternalError
Language=English
%1-%2@%3   Number of %4 choices is %5 %n %n 
%nMeaning - combine %4 from SCOTOpts.DAT & overrides - ConfigObject.cpp  ::Initialize %n %n
%nAction - Zero choices might be unwanted if expecting to use this feature.
.

;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. 
;//XMLOFF
MessageID=5181
SymbolicName=FLR_REPORTSERVER_OFFLINE
Severity=Error
Facility=InternalInterface
Language=English
The report server is offline. %1-%2@%3   %4 %n %n 
%nMeaning - Fastlane cannot contact the report server. %n %n
%nAction - Verify that the report server is up.  Verify network connections.
.

;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. 
;//XMLOFF
MessageID=5182
SymbolicName=FLR_REPORTSERVER_NOW_ONLINE
Severity=Informational
Facility=InternalInterface
Language=English
%1-%2@%3   The report server is back online %4 %n %n 
%nMeaning - Fastlane is again able to transfer files to the report server. %n %n
%nAction - None required.
.
;//-------------------------------------------------------------------------------------------------
MessageId=5183
SymbolicName=SCOTAPP_TB_RESPONSE_TIMEOUT
Severity=Error
Facility=InternalInterface
Language=English
%1-%2@%3   Fastlane has timed out waiting for a response from TB. %n %n 
%nMeaning - Fastlane did not receive a response from the Transaction Broker within the expected time. %n %n
%nAction - None. %n
.
;//-------------------------------------------------------------------------------------------------
MessageId=5184
SymbolicName=SCOTAPP_TENDER_TIMEOUT
Severity=Error
Facility=InternalInterface
Language=English
%1-%2@%3   Fastlane has timed out waiting for a response during Tendering. %n %n 
%nMeaning - Fastlane did not receive a response from the Transaction Broker within the expected time. %n %n
%nAction - None. %n
.
;//-------------------------------------------------------------------------------------------------
MessageId=5185
SymbolicName=SCOTAPP_TB_OUT_OF_SYNC
Severity=Error
Facility=InternalInterface
Language=English
%1-%2@%3   Transaction Broker timed out waiting for a POS response. %n %n 
%nMeaning - The transaction broker might be out of sync with the POS (host) . %n %n
%nAction - Run GetDiagFiles.  Reboot lane. %n
.
;//-------------------------------------------------------------------------------------------------
;//XMLOFF
MessageId=5186
SymbolicName=SCOTAPP_HEARTBEAT
Severity=Informational
Facility=InternalInterface
Language=English
%1-%2@%3   ScotApp Heartbeat message. %n %4 %n %n
%nMeaning - The ScotApp module is sending an "I'm alive" indicator. %n %n
%nAction - No Action. %n
.
;//-------------------------------------------------------------------------------------------------
;//XMLOFF
MessageId=5187
SymbolicName=SCOTAPP_HEARTBEAT2
Severity=Informational
Facility=InternalInterface
Language=English
%1-%2@%3   ScotApp Heartbeat message. %n %4 %n %n
%nMeaning - The ScotApp module is sending an "I'm alive" indicator. %n %n
%nAction - No Action. %n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 4 locations
MessageID=5188
SymbolicName=SCOTAPP_PS_FILEOPENABORT
Severity=Error
Facility=Configuration
Language=English
%1-%2@%3   SCOT could not open file: \%4\ %n %n %5%n
%nMeaning - Fatal"Stop Sign", file Open Failed. A required file is missing or corrupt. %n %n
%nAction - make sure the file name exists and is not corrupt%n
.
;//-------------------------------------------------------------------------------------------------
;// Filename is used in the 1st parameter at the right
MessageID=5189
SymbolicName=SCOTAPP_SM_REPORT_FILEOPEN
Severity=Informational
Facility=FileIO
Language=English
%1-%2@%3   File not found %n %n %4%n
%nMeaning - Unable to open report file. %n %n
%nAction - make sure the File exists and is not corrupt%n
.
;//-------------------------------------------------------------------------------------------------
;// Filename is used in the 1st parameter at the right
MessageID=5190
SymbolicName=SCOTAPP_SM_REPORT_FILEWRITE
Severity=Informational
Facility=FileIO
Language=English
%1-%2@%3   File not found %n %n %4%n
%nMeaning - Unable to write to report file. %n %n
%nAction - Make sure the is not read-only or corrupt.%n
.
;//-------------------------------------------------------------------------------------------------
;// Filename is used in the 1st parameter at the right
MessageID=5191
SymbolicName=SCOTAPP_FILEOPEN
Severity=Informational
Facility=FileIO
Language=English
%1-%2@%3   Unable to open file %n %n %4%n
%nMeaning - Unable to open file. %n %n
%nAction - make sure the File exists and is not corrupt%n
.
;//-------------------------------------------------------------------------------------------------
;// Filename is used in the 1st parameter at the right
MessageID=5192
SymbolicName=SCOTAPP_TB_FILENOTFOUND
Severity=Error
Facility=FileIO
Language=English
%1-%2@%3   TB unable to open file %n %n %4%n
%nMeaning - Transaction returned TB_E_FILENOTFOUND to ScotApp. %n %n
%nAction - Make sure the File exists and is not corrupt%n
.
;//-------------------------------------------------------------------------------------------------
;// Filename is used in the 1st parameter at the right
MessageID=5193
SymbolicName=SCOTAPP_TB_PRINTERFILE
Severity=Informational
Facility=FileIO
Language=English
%1-%2@%3   Unable to open file %n %n %4%n
%nMeaning - Unable to open the receipt or slip file. %n %n
%nAction - Make sure the File exists and is not corrupt%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 3 locations. Last Msg, Description, Error and Dev Class ID are used in 4 of the parameters
MessageID=5194
SymbolicName=SCOTAPP_DMINTERFACE_OLE_INFO
Severity=Informational
Facility=DevicePeripheral
Language=English
%1-%2@%3   %4 COleDispatchException[ %5 ] Codes(0x%6), device %7 (%8) %n %n %9%n
%nMeaning - An OLE exception occurred during a non-critical encryptor key test. %n %n
%nAction - None required.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 3 locations. Last Msg, Description, Error and Dev Class ID are used in 4 of the parameters
MessageID=5195
SymbolicName=SCOTAPP_DMINTERFACE_OLE_FATAL
Severity=Error
Facility=DevicePeripheral
Language=English
%1-%2@%3   %4 COleDispatchException[ %5 ] Codes(0x%6), device %7 (%8) %n %n %9%n
%nMeaning - Fatal"Stop Sign", an ole exception occurred %n %n
%nAction - Possible hardware problem, make sure the devices is plugged in and activated. Run Get Diag Files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. The device class is used in the 1st parameter.
MessageID=5196
SymbolicName=SCOTAPP_DF_NO_DEV_FOUND_INFO
Severity=Informational
Facility=Configuration
Language=English
%1-%2@%3   No device was found where 'Configure=Yes' for device class %4.%n%n %5%n
%nMeaning - Device not found but the device class exists. %n %n
%nAction - Probable hardware problem, make sure the device is plugged in and activated.%n
.
;//-------------------------------------------------------------------------------------------------
MessageId=5197
SymbolicName=SCOTAPP_TENDER_COMPLETE
Severity=Informational
Facility=InternalInterface
Language=English
%1-%2@%3   Fastlane tender has successfully completed. %n %n 
%nMeaning - A tender operation has completed.
%nAction - None. %n
.
;//-------------------------------------------------------------------------------------------------
MessageId=5198
SymbolicName=SCOTAPP_ATTRACT_SCREEN_ENTRY
Severity=Informational
Facility=InternalInterface
Language=English
%1-%2@%3   Fastlane has returned to the Attract screen. %n %n 
%nMeaning - Fastlane is waiting for a new customer.
%nAction - None. %n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5199
SymbolicName=SCOTAPP_SEC_MGR_PROCEDURE_MAIN_WINDOW_FAILED
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   Security Manager Procedures main window NOT Created %n %n %4%n
%nMeaning - System Restart (Fatal), if cannot create the main security procedures handle to a window. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5200
SymbolicName=SCOTAPP_SEC_MGR_PROCEDURE_WINDOW_FAILED
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3    Security Manager Interface window NOT Created %n %n %4%n
%nMeaning - System Restart (Fatal), cannot create the security interface handle to a window. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5201
SymbolicName=SCOTAPP_SEC_MGR_INTERFACE_WINDOW_FAILED
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   Security Manager Interface window NOT Created %n %n %4%n
%nMeaning - System Restart (Fatal), cannot create the security interface. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5202
SymbolicName=SCOTAPP_SEC_MGR_PROCEDURE_UNINIT
Severity=Error
Facility=StopAbort
Language=English
%1-%2@%3   Security Manager did Not Un-Initialize %n %n %4%n
%nMeaning - Fatal"Stop Sign", failed to un-init security interface. %n
%nAction - Run Get Diag files. Might temporarily turn on Security DataCapture for more logging.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5203
SymbolicName=SCOTAPP_SEC_MGR_PROCEDURE_WINDOW_DESTROY
Severity=Error
Facility=StopAbort
Language=English
%1-%2@%3   Security Manager Procedures window was not destroyed %n %n %4%n
%nMeaning - Fatal"Stop Sign", failed to DestroyWindow security interface. %n
%nAction - Run Get Diag files. Reboot NT.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Msg, Description, and Error are used in 3 of the parameters
MessageID=5204
SymbolicName= SCOTAPP_SEC_MGR_INTERFACE_OLEDISPATCH_EXCEPTION
Severity=Error
Facility=InternalInterface
Language=English
%1-%2@%3   %4 COleDispatchException[ %5 ] Code(0x%6) %n %n %7%n
%nMeaning - COleDispatchException caught while interfacing with security. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Msg, Description, and Error are used in 3 of the parameters
MessageID=5205
SymbolicName=SCOTAPP_SEC_MGR_INTERFACE_OLE_EXCEPTION
Severity=Error
Facility=InternalInterface
Language=English
%1-%2@%3   %4 OleException Error[ %5 ] Code(0x%6) %n %n %7%n
%nMeaning - OleException caught while interfacing with security. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Msg and Description are used in 2 of the parameters
MessageID=5206
SymbolicName=SCOTAPP_SEC_MGR_INTERFACE_C_EXCEPTION
Severity=Error
Facility=InternalInterface
Language=English
%1-%2@%3   %4 CException Error[ %5 ] %n %n %6%n
%nMeaning - CException caught while interfacing with security. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 5 locations. Msg and Error are used in 2 of the parameters at the right
MessageID=5207
SymbolicName=SCOTAPP_SEC_MGR_INTERFACE_EXCEPTION
Severity=Error
Facility=InternalInterface
Language=English
%1-%2@%3   %4 catch(...) error = %5 %n %n %6%n
%nMeaning - Exception caught while interfacing with security. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
MessageID=5208
SymbolicName=SCOTAPP_SEC_MGR_INTERFACE_STOP
Severity=Informational
Facility=StopAbort
Language=English
%1-%2@%3   Failed to create Security Manager control %n %n %4%n
%nMeaning - Failure while trying to create the Security Manager control. %n
;// Called from 1 location. Parameters at the right not used at this time.
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
MessageID=5209
SymbolicName=SCOTAPP_SEC_MGR_PROCEDURE_INIT_FAILED
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   Security Manager failed to initialize %n %n %4 %5 %n
%nMeaning - System Restart (Fatal). %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5210
SymbolicName=SCOTAPP_SEC_MGR_PROCEDURE_ADVISE_FAILED
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   Security Manager Procedures Advise failed %n %n %4%n
%nMeaning - Fatal"Stop Sign", failed to load security manager state machine XML file. %n
%nAction - Turn on Security Manager log and check the error logged. %n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. 
;//XMLOFF
MessageID=5211
SymbolicName=SCOTAPP_TB_HOST_OFFLINE
Severity=Error
Facility=LanWanComm
Language=English
The POS is offline. %1-%2@%3   %4 %n %n 
%nMeaning - The Point of sale system cannot contact it's server. %n %n
%nAction - Verify that the server is up.  Verify network connections.
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. 
;//XMLOFF
MessageID=5212
SymbolicName=SCOTAPP_TB_HOST_ONLINE
Severity=Informational
Facility=LanWanComm
Language=English
%1-%2@%3   The POS is back online %4 %n %n 
%nMeaning - The Point of sale is back online to it's server. %n %n
%nAction - None required.
.
;//-------------------------------------------------------------------------------------------------
MessageId=5213
SymbolicName=SCOTAPP_TB_OUT_OF_SYNC_STOP
Severity=Error
Facility=LanWanComm
Language=English
ScotApp timed out waiting for a TB response. %1-%2@%3 %4 %n %n 
%nMeaning - The transaction broker is out of sync with the SCOTAPP. %n %n
%nAction - Reboot Entire System by switch.
.
;//-------------------------------------------------------------------------------------------------
MessageId=5214
SymbolicName=SCOTAPP_TB_OUT_OF_SYNC_ASSISTMODE
Severity=Error
Facility=LanWanComm
Language=English
Transaction Broker timed out waiting for a POS response. %1-%2@%3 %4 %n %n 
%nMeaning - The transaction broker might be out of sync with the POS (host) . %n %n
%nAction - Go to Assist Mode and try to sync with POS.
.
;//-------------------------------------------------------------------------------------------------
MessageId=5215
SymbolicName=SCOTAPP_TB_OUT_OF_SYNC_ASSISTMODE_RECOVER
Severity=Informational
Facility=LanWanComm
Language=English
%1-%2@%3   out of sync need assist mode has recovered. %n %n 
%nMeaning - out of sync with the POS (host) has been resolved. %n %n
%nAction - None Required. %n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Keystroke value in 1st param.
MessageID=5216
SymbolicName=SCOTAPP_KEY_STUCK
Language=English
%1-%2@%3   Repeated keystrokes are being received (%4)%n 
%nMeaning - A key is probably stuck in the "down" position on the physical keyboard.
%nAction - Reposition the physical keyboard so that no keys are depressed.
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Keystroke count in 1st param.
MessageID=5217
SymbolicName=SCOTAPP_KEY_NO_LONGER_STUCK
Language=English
%1-%2@%3   Repeated keystrokes are no longer being received after %4 repetitions.%n 
%nMeaning - A key is no longer stuck in the "down" position on the physical keyboard.
%nAction - No action is necessary.
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5218
SymbolicName=SCOTAPP_RC_MGR_PROCEDURE_MAIN_WINDOW_FAILED
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   Remote Console Manager Procedures main window NOT Created %n %n %4%n
%nMeaning - System Restart (Fatal), cannot create the main RAP procedures handle to a window. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5219
SymbolicName=SCOTAPP_RC_MGR_PROCEDURE_WINDOW_FAILED
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3    Remote Console Manager Interface window NOT Created %n %n %4%n
%nMeaning - System Restart (Fatal), cannot create the RAP interface handle to a window. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5220
SymbolicName=SCOTAPP_RC_MGR_INTERFACE_WINDOW_FAILED
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   Remote Console Manager Interface window NOT Created %n %n %4%n
%nMeaning - System Restart (Fatal), cannot create the RAP interface. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5221
SymbolicName=SCOTAPP_RC_MGR_PROCEDURE_UNINIT
Severity=Error
Facility=StopAbort
Language=English
%1-%2@%3   Remote Console did Not Un-Initialize %n %n %4%n
%nMeaning - Fatal"Stop Sign", failed to un-init RCM interface. %n
%nAction - Run Get Diag files. Might temporarily turn on RCM DataCapture for more logging.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Parameters at the right not used at this time.
MessageID=5222
SymbolicName=SCOTAPP_RC_MGR_PROCEDURE_WINDOW_DESTROY
Severity=Error
Facility=StopAbort
Language=English
%1-%2@%3   Remote Console Manager Procedures window was not destroyed %n %n %4%n
%nMeaning - Fatal"Stop Sign", failed to DestroyWindow RCM interface. %n
%nAction - Run Get Diag files. Reboot NT.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Msg, Description, and Error are used in 3 of the parameters
MessageID=5223
SymbolicName= SCOTAPP_RC_MGR_INTERFACE_OLEDISPATCH_EXCEPTION
Severity=Error
Facility=InternalInterface
Language=English
%1-%2@%3   %4 COleDispatchException[ %5 ] Code(0x%6) %n %n %7%n
%nMeaning - COleDispatchException caught while interfacing with RAP. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Msg, Description, and Error are used in 3 of the parameters
MessageID=5224
SymbolicName=SCOTAPP_RC_MGR_INTERFACE_OLE_EXCEPTION
Severity=Error
Facility=InternalInterface
Language=English
%1-%2@%3   %4 OleException Error[ %5 ] Code(0x%6) %n %n %7%n
%nMeaning - OleException caught while interfacing with RAP. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 1 location. Msg and Description are used in 2 of the parameters
MessageID=5225
SymbolicName=SCOTAPP_RC_MGR_INTERFACE_C_EXCEPTION
Severity=Error
Facility=InternalInterface
Language=English
%1-%2@%3   %4 CException Error[ %5 ] %n %n %6%n
%nMeaning - CException caught while interfacing with RAP. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Called from 5 locations. Msg and Error are used in 2 of the parameters at the right
MessageID=5226
SymbolicName=SCOTAPP_RC_MGR_INTERFACE_EXCEPTION
Severity=Error
Facility=InternalInterface
Language=English
%1-%2@%3   %4 catch(...) error = %5 %n %n %6%n
%nMeaning - Exception caught while interfacing with RAP. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
MessageID=5227
SymbolicName=SCOTAPP_RC_MGR_INTERFACE_STOP
Severity=Informational
Facility=StopAbort
Language=English
%1-%2@%3   Failed to create Remote Console control %n %n %4%n
%nMeaning - Failure while trying to create the Remote Console control. %n
;// Called from 1 location. Parameters at the right not used at this time.
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
MessageID=5228
SymbolicName=SCOTAPP_RC_MGR_PROCEDURE_INIT_FAILED
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   Remote Console Manager failed to initialize %n %n %4 %5 %n
%nMeaning - System Restart (Fatal). %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
MessageID=5229
SymbolicName=SCOTAPP_PSX_START_SERVER_FAILED
Severity=Error
Facility=StartupInit
Language=English
%1-%2@%3   PS Initialize - PSX StartServer failed. rc= %n %n %4 %5 %n
%nMeaning - RAP and LaunchPad will not be able to communicate with this lane. %n
%nAction - Run Get Diag files.%n
.
;//-------------------------------------------------------------------------------------------------
;// Device Error.
MessageID=5230
SymbolicName=SCOTAPP_DEVICE_ERROR
Language=English
%1-%2@%3   %n%n
%4 %n%n
%5 %n%n
%6
.
;//-------------------------------------------------------------------------------------------------
MessageId=5231
SymbolicName=CUSTINFO_CREATE_INSTANCE_FAILURE
Severity=Error
Facility=Configuration
Language=English
Method %1 reports Internal software error %2 - Unable to create an instance of the DOM.  The Document was not loaded.
%nMeaning - Unable to create an instance of the DOM.  The Document was not loaded.%n
%nAction - The problem may be the result of improper installation of the XML parser. %n
.
;//-------------------------------------------------------------------------------------------------
MessageId=5232
SymbolicName=CUSTINFO_CONFIG_LOAD_FAILURE
Severity=Error
Facility=Configuration
Language=English
Method %1 reports error - Unable to successfully load the Config file.
Source - %2 %n
Line - %3 %n
Error  - %4 %n
%nMeaning -  Unable to successfully load the Customer info configuration file document.%n
%nAction - Verify.%n
.
;//-------------------------------------------------------------------------------------------------
MessageId=5233
SymbolicName=CUSTINFO_LOAD_XML_FAILURE
Severity=Error
Facility=InternalError
Language=English
Method %1 reports error - Unable to successfully load the DOM document.
Source - %2 %n
Line - %3 %n
Error  - %4 %n
%nMeaning -  Unable to successfully load the DOM document.%n
%nAction - Verify.%n
.
;//-------------------------------------------------------------------------------------------------
MessageId=5234
SymbolicName=CUSTINFO_COM_ERROR
Severity=Error
Facility=InternalError
Language=English
Method %1 detected COM Error %n
Source - %2 %n
Line - %3 %n
Error  - %4 %n
%nMeaning - Caught exception (_com_error e) while interfacing with Personalization Web Service -  PROCESS_COM ERRORS. %n
%nAction - This error references a number of possible errors. This may be caused by failure to access the configured web address or other COM errors.  Forward to NCR support for analysis of the specific problem.%n
.
;//-------------------------------------------------------------------------------------------------
MessageId=5235
SymbolicName=CUSTINFO_INIT
Severity=Error
Facility=StartupInit
Language=English
Method %1 unable to initialize %n
Source - %2 %n
Line - %3 %n
Error  - %4 %n
%nMeaning - Unable to initialize CustomerInfo (personalization) object %n
%nAction - The CustomerInfo object failed to initialize. This may be due to a problem with the CustomerInfoService.xml configuration files%n
.
;//-------------------------------------------------------------------------------------------------
MessageId=5236
SymbolicName=CUSTINFO_GENERAL_EXCEPTION
Severity=Error
Facility=InternalError
Language=English
Method %1 caught exception %n
Source - %2 %n
Line - %3 %n
Error  - %4 %n
%nMeaning - Caught unspecified exception while interfacing with Personalization Web Service.. %n
%nAction - This error references a number of possible errors. Forward to NCR support for analysis of the specific problem.%n
.
;//-------------------------------------------------------------------------------------------------
MessageId=5237
SymbolicName=CUSTINFO_ONLINE
Severity=Informational
Facility=Operational
Language=English
Method %1 detected COM Error %n
Source - %2 %n
Line - %3 %n
Error  - %4 %n
%nMeaning - Successfully communicated with Personalization web service %n
%nAction - This message is generated to indicate that communication with the personalization web service has bee re-established.%n
.
;//-------------------------------------------------------------------------------------------------
MessageId=5238
SymbolicName=SCOTOPTS_CONFIG_ERROR
Severity=Error
Facility=Configuration
Language=English
%1-%2@%3   ScotOpts Configuration Error. %n
%nError:   %4 %n
%nMeaning: Incorrect values for configuration option %n 
%nAction:  Insure that all parameters are configured correctly.
.
;//-------------------------------------------------------------------------------------------------
MessageID=5239
SymbolicName=SCOTAPP_DEVFACTORY_MOTION_SENSOR_EXCEED_MAX
Severity=Error
Facility=DevicePeripheral
Language=English
%1-%2@%3   SCOT forced to simulate motion begin and ended due to Motion Sensor Exceed Maximum Cycles: %n %n %5%n
%nMeaning: Motion Exceed Max event received. %n
%nAction:  Probable hardware problem, verify the coupon entry slot is clear.%n
.

;//-------------------------------------------------------------------------------------------------
MessageID=5240
SymbolicName=SCALEDISPLAY_ERROR
Severity=Error
Facility=Configuration
Language=English
%1-%2@%3   Scale Display Configuration Error.%n
%nError:   %4 %n
%nMeaning: Failed to configure display for weights and measures.%n 
%nAction:  Restart the lane.  If error re-occurs, contact NCR support.%n
.

;//-------------------------------------------------------------------------------------------------
MessageID=5241
SymbolicName=SCALEMETRIC_ERROR1
Severity=Error
Facility=Configuration
Language=English
%1-%2@%3   Scale Metric Configuration Error. %n
%nError:   %4 %n
%nWarning: The ScaleMetric configuration option is 'N' but the scale is metric.%n 
%nAction:  Make sure that the setting for the ScaleMetric configuration option is correct.%n
.

;//-------------------------------------------------------------------------------------------------
MessageID=5242
SymbolicName=SCALEMETRIC_ERROR2
Severity=Error
Facility=Configuration
Language=English
%1-%2@%3   Scale Metric Configuration Error. %n
%nError:   %4 %n
%nWarning: The ScaleMetric configuration option is 'Y' but the scale is not metric.%n 
%nAction:  Make sure that the setting for the ScaleMetric configuration option is correct.%n
.
;//-------------------------------------------------------------------------------------------------
MessageID=5243
SymbolicName=BAGBOX_CONFIG_ERROR
Severity=Warning
Facility=Configuration
Language=English
%1-%2@%3   Configuration Error. %n
%nWarning: This is a TAB system and the CustomerBagAllowed configuration option is 'Y'.  This option is not used on TAB systems.%n 
%nAction:  Set the CustomerBagAllowed option in the [Operations] section of ScotOpts to 'N'.%n
.

