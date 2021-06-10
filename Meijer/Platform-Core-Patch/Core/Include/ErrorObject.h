//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  ErrorObject.H
//
// TITLE:   Class interface definition file (describe class here)
//
//
// AUTHOR:  DcL-
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// CHANGES: Start
// PD -093097 Moved the component ID here from MsgObject.h
// PD -093097 Format, Tabify, convert from CCmdTarget to CObject
// CHANGES: End
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ERROROBJECT
#define _ERROROBJECT

#include "DLLDefine.h"
//Begin_EventLog
#include "NCREventLog.h"            // NCR EventLog header file
//#include "ScotEventCat.h"           // NCR Event Catogory  - RFC 262774 - now in ScotAppMsg.h
//#include "NCREventLogging.h"       // NCR EventLog definition JSM 4.0REQ2.1 already called in ncreventlog.h
//End_EventLog

//#include "TraceObject.h"

class TraceObject ;
// Component ID defines, used by many components
// Placed here because every one includes ErrorObject.h.
// each ID is a unique bit in a log so they can be or'd together
// in a mask for trace and error analysis

const long ID_MP = 0x00000001;      // MP Main process thread
const long ID_SM = 0x00000002;      // All state classes

const long ID_HC = 0x00000008;      // HC Emulator components

const long ID_PS = 0x00000010;      // PS Interface components
const long ID_DM = 0x00000020;      // DM Interface components
const long ID_SA = 0x00000040;      // SA Interface components
const long ID_TB = 0x00000080;      // TB Interface components

const long ID_TO = 0x00000100;      // Trace service
const long ID_EO = 0x00000200;      // Error service
const long ID_MO = 0x00000400;      // Message service
const long ID_CO = 0x00000800;      // Configuration object

const long ID_UE = 0x00001000;      // User Exits
const long ID_GP = 0x00002000;      // General Purpose
const long ID_HT = 0x00004000;      // Hard Totals Interface components
const long ID_RA = 0x00008000;      // Remote Approval Interface components
const long ID_TI = 0x00009000;      // TBInterface
const long ID_OD = 0x00010000;      // OPOS Devices and their mgmt classes
const long ID_RP = 0x00020000;      // Report classes
const long ID_SECMGR = 0x00040000;	// Security Manager Ocx
const long ID_CU = 0x00080000;      // Customer Info Object


//#define ScotError eo.Error              // the global trace object

#define C_NONE	  0                 // to not log anything
#define C_INFO    1                 // same ae return but no msgbox if in DEBUG
#define C_RETURN  2                 // simple log error and return to caller
#define C_RESTART 3                 // log error, program restarts itself
#define C_FATAL   4                 // do not return, must stop

#define FATAL   COMP_ID,T_ID,__LINE__,C_FATAL // log and exit program
#define RESTART COMP_ID,T_ID,__LINE__,C_RESTART // log and attempt restart  
#define RETURN  COMP_ID,T_ID,__LINE__,C_RETURN // simple log error and return to caller
#define INFO    COMP_ID,T_ID,__LINE__,C_INFO // same as return but no DEBUG msgbox
#define NONE    COMP_ID,T_ID,__LINE__,C_NONE // log nothing

#define MSGFILEKEY _T("SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application")


const int MAX_ERROR_BUF = 3000;     // max length of any one line
//const int MAXFORMAT = 100;          // max length of any format string
// This is implimented in MPProcedures to handle any additonal processing
// required for errors.


/////////////////////////////////////////////////////////////////////////////
// ErrorObject command target
class DLLEXPORT
ErrorObject : public CObject
{
private:
  bool fUninitialize;  
  const TCHAR* m_EventSource;
  bool fRestartInProgress;
//  void SwapFiles( void );           // implement later...
	  CNCREventLog *ErrLog;        // Global NCR EventLog
	
  // Attributes
public:
  //int m_NumberOfEntries;            // count indicating number of lines in the trace file.
  //int m_MaxNumberOfEntries;         // Maximum number of trace lines which this is the number of lines threshold to swap files...
  //CString CuFileName, PrFileName;
  //CStdioFile hError;

  // Operations

  ErrorObject(LPCTSTR);                    // constructor, sets name and size
  ~ErrorObject();                   // destructor closes file
	void Initialize();                
  void Uninitialize();

  // the first 4 parms to each of these functions are
  // provided by the FATAL, RETURN and INFO defines
	
  int Error(const long,             // component ID
    LPCTSTR,                         // T_ID, trace ID
    const int,                      // source line number
    const int,                      // error action, FATAL,RETURN...
    WORD  ,                         // NCR Event category
    ULONG ,                         // Event Message ID
    CString * = NULL ,                     // error text 
    TraceObject* =NULL  ,             // Trace Object
    LPCTSTR = NULL,                  // print fmt string
    va_list = NULL                  // parameters list
     );                             // variable data
	

  int Error(const long,           // component ID
    LPCTSTR,                         // T_ID, trace ID
    const int,                      // source line number
    const int,                      // error action, FATAL,RETURN...
    WORD  ,                         // NCR Event category
    ULONG ,                         // Event Message ID
    CString * = NULL ,              // error text
    TraceObject* =NULL,             // Trace Object   
    LPCTSTR = NULL,                  // print fmt string
    ...                             // variable data
     );                           

  int Error(const long,             // component ID
    LPCTSTR,                         // T_ID, trace ID
    const int,                      // source line number
    const int,                      // error action, FATAL,RETURN...
    WORD  ,                         // NCR Event category
    ULONG ,                         // Event Message ID
    long,                           // Event Action Mask
    CString * = NULL ,                     // error text 
    TraceObject* =NULL  ,             // Trace Object
    LPCTSTR = NULL,                  // print fmt string
    va_list = NULL                  // parameters list
     );                             // variable data
	

  int Error(const long,           // component ID
    LPCTSTR,                         // T_ID, trace ID
    const int,                      // source line number
    const int,                      // error action, FATAL,RETURN...
    WORD  ,                         // NCR Event category
    ULONG ,                         // Event Message ID
    long,                           // Event Action Mask
    CString * = NULL ,              // error text
    TraceObject* =NULL,             // Trace Object   
    LPCTSTR = NULL,                  // print fmt string
    ...                             // variable data
     );                           


  void FileError(const long,        // component ID
    LPCTSTR,                         // T_ID, trace ID
    const int,                      // source line number
    const int,                      // error action, FATAL,RETURN.
    LPCTSTR,                         // print fmt string
    CFileException & , 
    CString * = NULL ,                         // error text .
    TraceObject* =NULL             //Trace Object
    );              // error codes
	
  void FileError(const long,        // component ID
    LPCTSTR,                         // T_ID, trace ID
    const int,                      // source line number
    const int,                      // error action, FATAL,RETURN.
    long,                           // error action mask
    LPCTSTR,                         // print fmt string
    CFileException & , 
    CString * = NULL ,                         // error text .
    TraceObject* =NULL             //Trace Object
    ); 

  static TCHAR* FileErrorCause(CFileException &eFerr);
  CString GetErrorText(const int nErrorLevel, ULONG eventID, TCHAR* szLogBuff, LPCTSTR * ParamsArray);

private:// 
  EventTypeEnum GetEventType(const int nErrorLevel);  // Get NCR EventLog event type
  CString GetCompText(const long lComp_ID);
  void  PutStringIntoArray(LPTSTR FormatPtr, LPCTSTR *ParamsArray,WORD &NumParams);
  void ShowError(CString, const int);   // Show error message to message box
};



#endif

