;//  UtilsMsg.mc (.h)
;//
;//**  Copyright NCR Corp. 2005
;//**  This media contains an authorized
;//**  copy or copies of material copyrighted
;//**  by NCR Corp.  This copyright notice
;//**  and any other copyright notices
;//**  included in machine readable copies
;//**  must be reproduced on all authorized
;//**  copies.  Confidential, Unpublished
;//**  Property of NCR Corporation
;//
;// Error Messages Definitions
;//
;// NCRStateMachine is the name of the "SOURCE" Logging errors.  NCRStateMachine
;//    can be used in the RegisterSource function of the CErrorLog Class or 
;//    in the WIN32SDK function RegisterEventSource.
;//
;//
;// The following lines should be in your application specific message file:
;//      //NCRUtilities should be 18 char or less
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

MessageId=1000
SymbolicName=STATEM_ASSERTION_FAILURE
Language=English
An assertion has failed
.
