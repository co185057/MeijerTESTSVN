//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  CommonServices.h
//
// TITLE:   Header file to include most common service headers.
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// CHANGES: Start
// CHANGES: End
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _COMMONSERVICES
#define _COMMONSERVICES

#define SCOTDRIVE   _T("C")             // drive C
#define SCOTPATH    _T("\\SCOT")        // path for local scot file
#define SCOTCONFIG  _T("%APP_DRIVE%\\SCOT\\CONFIG") // path to local scot config file
//T.D 3.1
#define SCOTREPORT  _T("\\SCOT\\REPORT") // path to local scot config file
#define SCOTIMAGE   _T("%APP_DRIVE%\\SCOT\\IMAGE")       // path to local BMP images
#define SCOTSOUND   _T("%APP_DRIVE%\\SCOT\\SOUND")       // path to local WAV files
#define SCOTOPTS    _T("SCOTOPTS")      // name of local scot options file, INI,000,nnn
#define SCOTTEND    _T("SCOTTEND")      // name of local SCOTTEND
#define SCOTTARE    _T("SCOTTARE")      // name of 4690 tare desc file, .DAT
#define SCOTUPDT    _T("SCOTUPDT")      // name of 4690 maintenance list file, .DAT
#define SCOTLOGO    _T("SCOTLOGO")      // name of local logo file built from 4690 write logo
#define SCOTSERVER  _T("SCOT:")
#define SCUDBMPS    _T("SCUDBMPS")      // name of SCot User Defined bitmaps
#define SCUDMSGS    _T("SCUDMSGS")      //name of the SCot User Defined Message file
#define SCUDMSGFILE(langEnum) langEnum==SCOT_LANGUAGE_PRIMARY ? _T("SCUDMSGS.DAT") : _T("SCUDMSG%d.DAT"),((langEnum - SCOT_LANGUAGE_PRIMARY) + 1)
                                     //base name for dual language message file (number is appended for non-primary languages).
#define SECCONFIG   _T("SecurityConfig")  // name of the Security Configuration File ( TAR 287143)
#define SCOTPICKLISTIMAGE   _T("%APP_DRIVE%\\SCOT\\IMAGE\\PICKLIST") // path to picklist images
#define SHRINKPICKLISTIMAGECMD _T("%APP_DRIVE%\\Scot\\Bin\\ShrinkImage -p %APP_DRIVE%\\Scot\\config\\picklist.xml %APP_DRIVE%\\Scot\\Image\\Picklist 99 92")
const TCHAR PICKLISTEDITOR[] = _T("PickListEditor");   // name of PickListEditor tool

#define SCOTMSGID   _T("SCOTISMESSAGEID")  // for RegisterWindowMessage()
#define DEFAULTRESOLUTIONFORPICKLIST   _T("10x7")	//TAR 431887 default resolution for picklist
#define DATAENTRYINIFILE _T("%APP_DRIVE%\\SCOT\\CONFIG\\SCOTDATAENTRY.ini")


// SCOT Controls window resource ID constants
#define IDW_DMPWINDOW                   101
#define IDW_DMIWINDOW                   102
#define IDW_SAPWINDOW                   103
#define IDW_SAIWINDOW                   104
#define IDW_HTPWINDOW                   105
#define IDW_SDFWINDOW                   106
#define IDW_SECMGRPWINDOW               107
#define IDW_SECMGRIWINDOW               108
#define IDW_RCMGRPWINDOW                109
#define IDW_RCMGRIWINDOW                110
// EOD options
#define OPTION_LANE_CONTROL 	1
#define OPTION_TB_CONTROL	 	2
#define OPTION_OPERATOR_CONTROL	3

#include "TraceObject.h"            // MGV Trace Object
#include "MsgObject.h"              // MGV Message Object
#include "SCOTAPPErrorObject.h"            // MGV Error Object
#include "SCOTAPPBusyStatus.h"

#ifdef _SCOT
#include "SCOTAPPMsg.h"
#endif

#ifdef _SCOTSSF_
#include "SCOTSSFMsg.h"
#endif

#ifdef _SCOTSSF_
DLLIMPORT extern TraceObject     to;          // global trace Object
DLLIMPORT extern SCOTAPPErrorObject     eo;          // global error Object
DLLIMPORT extern MsgObject       mo;          // global message Object
DLLIMPORT extern ScotAppBusyStatus appBusy;  // global busy status Object
#else
DLLEXPORT extern TraceObject     to;          // global trace Object
DLLEXPORT extern SCOTAPPErrorObject     eo;          // global error Object
DLLEXPORT extern MsgObject       mo;          // global message Object
DLLEXPORT extern ScotAppBusyStatus appBusy;  // global busy status Object
#endif

#endif
