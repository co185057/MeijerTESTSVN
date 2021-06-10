//
// This is the Transaction broker Message (Source) File.
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


//
// Define the severity codes
//


//
// MessageId: TRANS_BROKER_NOT_INITIALIZED
//
// MessageText:
//
//  Transaction broker failed to initialize. Window handle is NULL.
//  %nMeaning - Application wihdow handle was not passed to TB properly.
//  %nAction - Reboot SCOT machine and start SCOT application again.If still problem exists, contact an NCR Support representative.%n%0
//
#define TRANS_BROKER_NOT_INITIALIZED     0x00001F40L

//
// MessageId: TRANS_BROKER_EXIT
//
// MessageText:
//
//  Transaction Broker was closed.
//
#define TRANS_BROKER_EXIT                0x00001F41L

//
// MessageId: TB_EID_NO_VERSIONINFO
//
// MessageText:
//
//  Transaction Broker could not get version information.
//
#define TB_EID_NO_VERSIONINFO            0x00001F42L

//
// MessageId: TB_INITIALIZED
//
// MessageText:
//
//  Transaction Broker (emulator) is successfully initialized.
//
#define TB_INITIALIZED                   0x00001F43L

//
// MessageId: TB_MISSING_PLU_LIST
//
// MessageText:
//
//  Missing PLU List File.
//
#define TB_MISSING_PLU_LIST              0x00001F44L

//
// MessageId: TB_MISSING_DISPLAY_TEXT
//
// MessageText:
//
//  Missing Display Text Table File.
//
#define TB_MISSING_DISPLAY_TEXT          0x00001F45L

//
// MessageId: TB_MISSING_EXCEPT_LIST
//
// MessageText:
//
//  Missing Exception List File.
//
#define TB_MISSING_EXCEPT_LIST           0x00001F46L

