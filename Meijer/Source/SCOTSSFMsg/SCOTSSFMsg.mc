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


;// SCOTSSF Message ID start from 8000

MessageID=8000
SymbolicName=SCOTSSF_SM_MSG_START
Language=English
Event log message from SCOTSSF.  It could be replaced...%n
.
;//-------------------------------------------------------------------------------------------------
;// A mobile transaction failed to complete
MessageID=8001
SymbolicName=SCOTSSF_SM_MSG_MOBILE_TRANSFER_FAILED
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   Mobile Ticket Transfer Failure %n %n %4
%nMeaning - A mobile ticket was sent to the lane, but the transaction was not processed. %n
%nAction - Run Get Diag files.
.
;//-------------------------------------------------------------------------------------------------
;// A mobile transaction failed to complete
MessageID=8002
SymbolicName=SCOTSSF_SM_MSG_MOBILE_UPLOAD_FAILED
Severity=Error
Facility=InternalError
Language=English
%1-%2@%3   Mobile Ticket Transfer Failure %n %n %4
%nMeaning - A mobile ticket was sent to the lane, but the transaction was not processed. %n
%nAction - Run Get Diag files.
.
