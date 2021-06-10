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
#include "ErrorObjectConstants.h"

class TraceObject ;
// Component ID defines, used by many components
// Placed here because every one includes ErrorObject.h.
// each ID is a unique bit in a log so they can be or'd together
// in a mask for trace and error analysis




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

