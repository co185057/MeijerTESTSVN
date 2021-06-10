//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SCOTSSFErrorObject.H
//
// TITLE:   Class interface definition file (describe class here)
//
//
// AUTHOR:  Feng Zhang
//
//////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SCOTSSFErrorOBJECT
#define _SCOTSSFErrorOBJECT

#include "ErrorObject.h"

#define ScotError eo.Error              // the global trace object
#ifndef _SCOT
#define error eo.Error
#endif


class SCOTSSFErrorObject : public ErrorObject
{
public:
 
  SCOTSSFErrorObject(LPCTSTR);
  ~SCOTSSFErrorObject();
  void Initialize(TraceObject* pTrace);
  
  int Error(const long,             // component ID
    LPCTSTR,                         // T_ID, trace ID
    const int,                      // source line number
    const int,                      // error action, FATAL,RETURN...
    WORD  ,                          // NCR Event category
    ULONG ,                          // Event Message ID
    LPCTSTR = NULL,                         // print fmt string
    ...);                           // variable data

  int Error(const long,             // component ID
    LPCTSTR,                         // T_ID, trace ID
    const int,                      // source line number
    const int,                      // error action, FATAL,RETURN...
    WORD  ,                          // NCR Event category
    ULONG ,                          // Event Message ID
    long ,                          // Event Action Mask
    LPCTSTR = NULL,                         // print fmt string
    ...);                           // variable data


  int FileError(const long,        // component ID
    LPCTSTR,                         // T_ID, trace ID
    const int,                      // source line number
    const int,                      // error action, FATAL,RETURN...
    LPCTSTR,                         // print fmt string
    CFileException &);              // error codes

  int FileError(const long,        // component ID
    LPCTSTR,                         // T_ID, trace ID
    const int,                      // source line number
    const int,                      // error action, FATAL,RETURN...
    long ,                          //Event Action Mask
    LPCTSTR,                         // print fmt string
    CFileException &);              // error codes

  void PostErrorMsg(const int );   // post error msg

  // The Scotapp version of this is implemented in MPProcedures
  void MainErrorLogger(							  // must be provided by main process somewhere
                     const long,	  // component ID, long
                 	 LPCTSTR,			  // trace ID
                     const int,		  // line number in source file
                     const long,	  // error level C_FATAL...C_INFO
                     CString * );      // error text



private:
	TraceObject* m_pTraceObj;	// false to turn off trace, true to turn on trace

};







#endif