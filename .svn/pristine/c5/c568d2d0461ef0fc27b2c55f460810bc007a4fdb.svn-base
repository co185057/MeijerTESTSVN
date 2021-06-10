//////////////////////////////////////////////////////////////////////////////
//
// FILE:  SCOTSSFErrorObject.CPP
//
// TITLE:   Class implementation file (describe class here)
//
//
// AUTHOR:  Feng Zhang
//
//////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first

#include "CommonServices.h"         // MGV services header
#include "ScotSSFErrorObject.h"

#define COMP_ID ID_EO
#define T_ID _T("SCOTEO")


////////////////////////////////////////////////////////////
SCOTSSFErrorObject::SCOTSSFErrorObject(LPCTSTR EventSource)
: ErrorObject(EventSource) 
{

}


///////////////////////////////////////////////////////////
SCOTSSFErrorObject::~SCOTSSFErrorObject()
{
}


void SCOTSSFErrorObject::Initialize(TraceObject* pTrace)
{
	m_pTraceObj = pTrace;
	ErrorObject::Initialize();
}

/////////////////////////////////////////////////////////////
int SCOTSSFErrorObject::Error(const long lCompID,             // component ID
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
 
  /*
  //Log error text to server 
  if(nErrorLevel > C_INFO)
      MainErrorLogger(lCompID, szTrcID, nSourceLine, nErrorLevel, &csErrorText);
  */
  //Post error message 
  PostErrorMsg(nErrorLevel);

  return 0;
}

/////////////////////////////////////////////////////////////
int SCOTSSFErrorObject::Error(const long lCompID,             // component ID
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
int SCOTSSFErrorObject::FileError(const long lCompId,      // component ID
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

void SCOTSSFErrorObject::PostErrorMsg(const int nErrorLevel)
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


void SCOTSSFErrorObject::MainErrorLogger(const long lCompId,	  // component ID, long
                     LPCTSTR szTrcId,			  // trace ID
                     const int nLine,		  // line number in source file
                     const long nLevel,	  // error level C_FATAL...C_INFO
                     CString* pErrorText)          // variable data
    {
       CString csCompId;
       switch (lCompId)
      {
      case ID_MP: csCompId = _T("MP"); break;	// MP Main process
      case ID_SM: csCompId = _T("SM"); break;	// All state classes
      case ID_HC: csCompId = _T("HC"); break;	// HC Emulator components
      case ID_PS: csCompId = _T("PS"); break;	// PS Interface components
      case ID_RP: csCompId = _T("RP"); break;	// RP components
      case ID_DM: csCompId = _T("DM"); break;	// DM Interface components
      case ID_SA: csCompId = _T("SA"); break;	// SA Interface components
      case ID_TB: csCompId = _T("TB"); break;	// TS Interface components
      case ID_TO: csCompId = _T("TO"); break;	// Trace service
      case ID_EO: csCompId = _T("EO"); break;	// Error service
      case ID_MO: csCompId = _T("MO"); break;	// Message service
      case ID_CO: csCompId = _T("CO"); break;	// Configuration object
      case ID_UE: csCompId = _T("UE"); break;	// User Exits
      case ID_GP: csCompId = _T("GP"); break;	// File Utilities
      case ID_TI: csCompId = _T("TI"); break;	// TB interface
	  case ID_XM: csCompId = _T("XM"); break;	// Cash Management
      }

 	
      UCHAR  nSeverity = 0;
  	
      switch (nLevel)
      {
    	case C_FATAL:
    		nSeverity = 1;								  // sev 1
    		break;
  	  case C_RESTART:
  		nSeverity = 1;								  // sev 1
    		break;
    	case C_RETURN:
    		nSeverity = 2;								  // sev 2
    		break;
    	case C_INFO:
    		nSeverity = 3;								  // sev 3
    		break;
    	default:
    		nSeverity = 4;								  // sev 4

 		  break;

      }

 	

/* ts.LogErrorTo4690(AppName,        // application name
 		'B',													  // B for smkt
 		nMsgNum,											  // msg number B200
 		nSeverity,										  // 1 high... 5 low
 		nEvent,												  // 0-255, 64-81 cause alerts on NPDA
 		10,														  // length of unique
 		szUnique);										  // 10 data bytes any format
 */

	try	// Sometimes this is throwing an exception itself.  
	{
		
		if(tb.IsTBInitialized() == TB_SUCCESS) // only if TB is currently initialized, call LogErrorToServer
		{
			TBRC rc = tb.LogErrorToServer((LPCTSTR) *pErrorText, nSeverity, lCompId, csCompId, szTrcId, nLine);
		  ASSERT(rc == TB_SUCCESS);
		}
	} 
	catch(...)
	{
		// We can't log an exception, that would cause a recursive call right back here.  
		// This has in the past, caused recursive exceptions in the app, crashing it.   
	}
	return;
}