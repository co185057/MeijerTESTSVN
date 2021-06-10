//////////////////////////////////////////////////////////////////////////////
//
// FILE:  SCOTAPPErrorObject.CPP
//
// TITLE:   Class implementation file (describe class here)
//
//
// AUTHOR:  Feng Zhang
//
//////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first

#include "CommonServices.h"         // MGV services header

#define COMP_ID ID_EO
#define T_ID _T("SCOTEO")


////////////////////////////////////////////////////////////
SCOTAPPErrorObject::SCOTAPPErrorObject(LPCTSTR EventSource)
: ErrorObject(EventSource) 
{

}


///////////////////////////////////////////////////////////
SCOTAPPErrorObject::~SCOTAPPErrorObject()
{
}


void SCOTAPPErrorObject::Initialize(TraceObject* pTrace)
{
	m_pTraceObj = pTrace;
	ErrorObject::Initialize();
}

/////////////////////////////////////////////////////////////
int SCOTAPPErrorObject::Error(const long lCompID,             // component ID
    LPCTSTR szTrcID,                         // T_ID, trace ID
    const int nSourceLine,                      // source line number
    const int nErrorLevel,                      // error action, FATAL,RETURN...
    WORD  eventCategory,                          // NCR Event category
    ULONG eventID,                          // Event Message ID
    LPCTSTR szPfmt,                         // print fmt string
    ...)                           // variable data
{
  CString csErrorText;           //Error Text
  va_list argptr;
  va_start( argptr, szPfmt );			  // variable parameter list processing
  
  // log error to error object
  // output error text
  ErrorObject::Error(lCompID, szTrcID, nSourceLine, nErrorLevel,
    eventCategory, eventID, &csErrorText, m_pTraceObj, szPfmt,argptr); 
  
  //Log error text to server 
  if(nErrorLevel > C_INFO)
      MainErrorLogger(lCompID, szTrcID, nSourceLine, nErrorLevel, &csErrorText);

  //Post error message 
  PostErrorMsg(nErrorLevel);

  return 0;
}

/////////////////////////////////////////////////////////////
int SCOTAPPErrorObject::Error(const long lCompID,             // component ID
    LPCTSTR szTrcID,                         // T_ID, trace ID
    const int nSourceLine,                      // source line number
    const int nErrorLevel,                      // error action, FATAL,RETURN...
    WORD  eventCategory,                          // NCR Event category
    ULONG eventID,                          // Event Message ID
    long eventActionMask,            // Event Action Mask
    LPCTSTR szPfmt,                         // print fmt string
    ...)                           // variable data
{
  CString csErrorText;           //Error Text
  va_list argptr;
  va_start( argptr, szPfmt );			  // variable parameter list processing
  
  // log error to error object
  // output error text
  ErrorObject::Error(lCompID, szTrcID, nSourceLine, nErrorLevel,
    eventCategory, eventID, eventActionMask, &csErrorText, m_pTraceObj, szPfmt,argptr); 
  
  //Log error text to server 
  if(nErrorLevel > C_INFO)
      MainErrorLogger(lCompID, szTrcID, nSourceLine, nErrorLevel, &csErrorText);

  //Post error message 
  PostErrorMsg(nErrorLevel);

  return 0;
}



////////////////////////////////////////////////////////////////
int SCOTAPPErrorObject::FileError(const long lCompId,      // component ID
                                   LPCTSTR szTrcId,          // T_ID, trace ID
                                   const int nSourceLine,   // source line number
                                   const int nErrorLevel,   // error action, FATAL,RETURN..
                                   LPCTSTR szPfmt,           // print fmt string
                                   CFileException & eFileExce)  // error codes
{
  CString csErrorText;
  //Log error to ErrorObject
  ErrorObject::FileError(lCompId, szTrcId, nSourceLine, nErrorLevel,szPfmt,eFileExce,&csErrorText, &to);
  //Log error to server
  if(nErrorLevel > C_INFO)
      MainErrorLogger(lCompId, szTrcId, nSourceLine, nErrorLevel, &csErrorText);

  //Post error message 
  PostErrorMsg(nErrorLevel);
  return 0;
}

void SCOTAPPErrorObject::PostErrorMsg(const int nErrorLevel)
{
        //post error message
    switch( nErrorLevel )
    {
    case C_FATAL:
      mo.PostGP(GP_STOP);
      break;
    case C_RESTART:
      mo.PostGP(GP_RESTART);
      break;
    case C_RETURN:
      break;
    case C_INFO:
    case C_NONE:
      break;
    default:
      break;
    }
 return;
}


