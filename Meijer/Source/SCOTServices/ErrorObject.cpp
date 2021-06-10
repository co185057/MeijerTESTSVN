//////////////////////////////////////////////////////////////////////////////
//
// FILE:  ErrorObject.CPP
//
// TITLE:   Class implementation file (describe class here)
//
//
// AUTHOR:  DcL-
//
//////////////////////////////////////////////////////////////////////////////
//
// CHANGES: Start
// DcL-092497 Added the swap file routines.
// DcL-092597 Changed locking scheem for file swap and cleanup.
// PD -093097 Format, Tabify, convert from CCmdTarget to CObject
// CHANGES: End
//
//////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxmt.h>                  // multi-threading error
#include "mmsystem.h"

//#include "CommonServices.h"         // MGV services header
#include "SCOTServicesMsg.h"
#include "ErrorObject.h"
#include "MsgObject.h"
#include "TraceObject.h"
#include "ScotEventCat.h"



static CCriticalSection CritSec;

#define COMP_ID ID_EO
#define T_ID _T("EO")

//////////////////////////////////////////////////////////////////////////////
TCHAR * ErrorObject::FileErrorCause(CFileException &eFerr)
{
  //TCHAR szError[256];     //Tar 366301
  TCHAR *pszError = new _TCHAR[257];
  eFerr.GetErrorMessage(pszError,256);   //change to this for Internationalize
                         /*switch (eFerr.m_cause)
  {
    // The enumerators and their meanings are as follows: CFileException.m_cause
		
  case 0: szError = "None. No error occurred."; break;
  case 1: szError = "Generic. An unspecified error occurred."; break;
  case 2: szError = "FileNotFound. The file could not be located."; break;
  case 3: szError = "BadPath. All or part of the path is invalid."; break;
  case 4: szError = "TooManyOpenFiles. The permitted number of open files was exceeded."; break;
  case 5: szError = "AccessDenied. The file could not be accessed."; break;
  case 6: szError = "InvalidFile. There was an attempt to use an invalid file handle."; break;
  case 7: szError = "RemoveCurrentDir. The current working directory cannot be removed."; break;
  case 8: szError = "DirectoryFull. There are no more directory entries."; break;
  case 9: szError = "BadSeek. There was an error trying to set the file pointer."; break;
  case 0x0a: szError = "HardIO. There was a hardware error."; break;
  case 0x0b: szError = "SharingViolation. SHARE.EXE was not loaded, or a shared region was locked."; break;
  case 0x0c: szError = "LockViolation. There was an attempt to lock a region that was already locked."; break;
  case 0x0d: szError = "DiskFull. The disk is full."; break;
  case 0x0e: szError = "EndOfFile. The end of file was reached."; break;
  default: szError = "Unknown file error"; break;
  }
  */

  return pszError;
}


//////////////////////////////////////////////////////////////////////////////
// ErrorObject constructor
//
ErrorObject::ErrorObject(LPCTSTR EventSource)
 :fRestartInProgress(false), m_EventSource(EventSource), fUninitialize(false)
{
    ErrLog = new CNCREventLog;
}

//////////////////////////////////////////////////////////////////////////////
// ErrorObject destructor close the Error tracing files.
ErrorObject::~ErrorObject()
{
    if (ErrLog != NULL)
    {
        delete ErrLog;
    }
}


void ErrorObject::Initialize()
{
   HRESULT rc = ErrLog->Open(m_EventSource);
   ASSERT( rc == S_OK);
}


void ErrorObject::Uninitialize()
{
   HRESULT rc = ErrLog->Close();
   ASSERT( rc == S_OK);
   fUninitialize = true;
}

//////////////////////////////////////////////////////////////////////////////
// This method of the error object class writes the error into the trace file.
int ErrorObject::Error(const long lComp_ID,
                       LPCTSTR szTrcId,
                       const int nSourceLine,
                       const int nErrorLevel,
                       WORD eventCategory,              // NCREventLog Category
                       ULONG eventID,                   // Event Log Message ID
                       CString *pErrorText,            // error text Out
                       TraceObject* pTrace,             //Trace Object
                       LPCTSTR szPfmt,                   // para format
                       ...                              // para list
                       )
{
    va_list argptr;
    va_start( argptr, szPfmt );			  // variable parameter list processing 
    Error(lComp_ID, szTrcId, nSourceLine, nErrorLevel, eventCategory,
      eventID, pErrorText, pTrace, szPfmt, argptr);
    return 0;
}

//////////////////////////////////////////////////////////////////////////////
// This method of the error object class writes the error into the trace file.
int ErrorObject::Error(const long lComp_ID,
                       LPCTSTR szTrcId,
                       const int nSourceLine,
                       const int nErrorLevel,
                       WORD eventCategory,              // NCREventLog Category
                       ULONG eventID,                   // Event Log Message ID
                       long eventActionMask,            // Event Action Mask
                       CString *pErrorText,            // error text Out
                       TraceObject* pTrace,             //Trace Object
                       LPCTSTR szPfmt,                   // para format
                       ...                              // para list
                       )
{
    va_list argptr;
    va_start( argptr, szPfmt );			  // variable parameter list processing 
    Error(lComp_ID, szTrcId, nSourceLine, nErrorLevel, eventCategory,
      eventID, eventActionMask, pErrorText, pTrace, szPfmt, argptr);
    return 0;
}

//////////////////////////////////////////////////////////////////////////////
// This method of the error object class writes the error into the trace file.
int ErrorObject::Error(const long lComp_ID,
                       LPCTSTR szTrcId,
                       const int nSourceFileLine,
                       const int nErrorLevel,
                       WORD eventCategory,              // NCREventLog Category
                       ULONG eventID,                   // Event Log Message ID
                       CString* pErrorText,            // error text Out
                       TraceObject* pTrace,             // Trace Object
                       LPCTSTR szPfmt,                   // para format
                       va_list argptr                   // para list
                       )
{
    return Error(lComp_ID, szTrcId, nSourceFileLine, nErrorLevel, eventCategory,
        eventID, NCRSM_DEFAULT_ACTIONS, pErrorText, pTrace, szPfmt, argptr);
}

//////////////////////////////////////////////////////////////////////////////
// This method of the error object class writes the error into the trace file.
int ErrorObject::Error(const long lComp_ID,
                       LPCTSTR szTrcId,
                       const int nSourceFileLine,
                       const int nErrorLevel,
                       WORD eventCategory,              // NCREventLog Category
                       ULONG eventID,                   // Event Log Message ID
                       long eventActionMask,            // Event Log Action Mask
                       CString* pErrorText,            // error text Out
                       TraceObject* pTrace,             // Trace Object
                       LPCTSTR szPfmt,                   // para format
                       va_list argptr                   // para list
                       )
{
	
  if (nErrorLevel==C_NONE)
		return 0;

  TCHAR szBuff[MAX_ERROR_BUF+1];
  CString  csCompId;

    CString csTrace = _T("");    // to contain trace message
    EventTypeEnum eventType;    //Event type
    TCHAR *p;


  csCompId = GetCompText(lComp_ID);


  CSingleLock sl( &CritSec, false );
  sl.Lock();                        // enter critical section

  try
  {
	  TCHAR szLogBuff[MAX_ERROR_BUF+1];           //String to log to eventlog

	  _stprintf( szLogBuff,_T("%s|%s|%d"),   //First log compid, traceid and line no.
						csCompId,                     // Component ID
						szTrcId,                      // Trace ID
						nSourceFileLine);

 
	  if(szPfmt != NULL)
	  {  
   		_vsntprintf( szBuff, MAX_ERROR_BUF, szPfmt, argptr ); // variable parameter list processing
    
		// convert all unprintables to '~'
		p = szBuff;
		int i = 0;
		while (p[i] && i < MAX_ERROR_BUF)
		{
		  if (p[i] == _T('\n')) p[i] = _T(' ');  // On EventLog, can not use \n to change line, show space instead
		  else if (!_istprint(p[i])) p[i] = _T('~') ;   
		  //if (*p<0x20) *p = '~';
		  //if (*p>0x7f) *p = '~';
		  i++;
		}
		p[MAX_ERROR_BUF]=0; 
        _tcscat(szLogBuff,_T("|"));
		_tcscat(szLogBuff, szBuff); 
  
	  }
	   // also print the last saved trace info if a fatal error
	  if (nErrorLevel==C_FATAL && pTrace != NULL)
	  {

		int n = pTrace->nSavedEntry;
		for (int i=0;i<MAX_SAVED_TRACES;i++)
		{
		  n--;
		  if (n<0) n = MAX_SAVED_TRACES-1;

		  //Begin_LogEvent
		  CString csTrc;
		  csTrc.Format(_T("Trace %d: %s %d      "),i+1,
			pTrace->csSavedTid[n],pTrace->nSavedLine[n]);
		  csTrace +=  csTrc;   // get trace message
		  //End_LogEvent

		}
	  }
			  //Begin_EventLog
		// Log Error to NCREventLog


		LPTSTR	FormatPtr;           // pointer for locating delimiters in FormatBuf
		WORD	NumParams = 0;
		LPCTSTR	ParamsArray[21];     // maximum of 20 insertion strings
		for(int n=0; n<21; n++)     
		  ParamsArray[n] = 0;
 
		// place all of the parameter strings into the parameter array
		FormatPtr = szLogBuff;
		CString csLogBuff = szLogBuff;
    
		PutStringIntoArray(FormatPtr, ParamsArray, NumParams);


		va_end(argptr);
		ParamsArray[NumParams++] = csTrace;   //Log the trace for fatal error
	  //  ParamsArray[NumParams] = NULL;
  
		eventType = GetEventType(nErrorLevel);

	  if(!fUninitialize)
	  {
		if (pTrace)
		  pTrace->Trace(lComp_ID,szTrcId,nSourceFileLine,0,_T("before calling store minder error log event"));
		HRESULT rc= S_OK;
		try
		{
		   rc = ErrLog->LogEventArray(evlClassBasic, eventType, eventCategory, eventID, 
					 ParamsArray, NumParams, NULL, 0, eventActionMask);
		}
		catch(...)
		{
		   if (pTrace)
		   {
			  long le = GetLastError();
			  pTrace->Trace(lComp_ID, T_ID _T("::Error"), __LINE__, 0, _T("Error logging event %d, with %d parms in %s|%s"),
							eventID,
							NumParams,
							csLogBuff,
							csTrace);
			  pTrace->Trace(lComp_ID, T_ID _T("::Error"), __LINE__, 0, _T("Last error %d (0x%x) on logging error"), le, le);
		   }
		}
		if (pTrace)
		  pTrace->Trace(lComp_ID,szTrcId,nSourceFileLine,0,_T("after calling store minder error log event"));
		ASSERT(rc == S_OK);
	  }
	  sl.Unlock();                      // exit critical section
		
	  //Add this error to trace
	  if (nErrorLevel == C_INFO) 
		p = _T("INFO");
	  else 
		p = _T("ERROR");
  
	  if(pTrace != NULL)
	  {
		  if(szPfmt != NULL)   // extra parameters specified
		  {
			  pTrace->Trace(lComp_ID,szTrcId,nSourceFileLine,0,_T("%s: %s"),p,szBuff);
		  }
		  else   //no extra parameters specified
		  {
			  pTrace->Trace(lComp_ID,szTrcId,nSourceFileLine,0,_T(""));
		  }
	  }

	  if (fRestartInProgress)    // If get restart error twice, ignore the second one
		return 0;

	  if (nErrorLevel == C_RESTART)
		fRestartInProgress = true;
		
	  //TAR409575: omitting unecessary calls to GetErrorText() should remove the possibility 
	  //that ScotAppMsg.dll loading/unloading is an issue in performance.  
	  if (pErrorText)
	  {
		//Get error text from .mc file 
		CString csErrorTextOut = GetErrorText(nErrorLevel, eventID, szLogBuff, ParamsArray);
		pErrorText->Format(_T("%s"), csErrorTextOut);
	  }
	  //TAR409575: Rmove following code here

  }
  catch(...)
  {
	 // Be carefull here, you have an exception during a error log
	 // Trying to add tracing here to show the message could cause a recursive exception 
	 // and crash the app.  There's probably something wrong with the message format.
	 TRACE(_T("ErrorObject::Error Exception internally - unable show the error") );
  }




  return 0;
}




//////////////////////////////////////////////////////////////////////////////////////////////////
void ErrorObject::FileError(const long lComp_ID,
                            LPCTSTR szTrcId,
                            const int nSourceFileLine,
                            const int nErrorLevel,
                            LPCTSTR szDesc,
                            CFileException &eFerr,
                            CString * pErrorText,            // error text    
                            TraceObject* pTrace             //Trace Object
                            )
{
  TCHAR *szError = FileErrorCause(eFerr);


  Error(
    lComp_ID,
    szTrcId,
    nSourceFileLine,
    nErrorLevel,
    SM_CAT_FILE_IO,
    SCOTSERVICES_EO_FILE,
    pErrorText,
    pTrace,
    _T("%s|%s|%s|%x"),
    eFerr.m_strFileName,
    szDesc,
    szError,
    eFerr.m_cause);

  delete [] szError;

  return;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void ErrorObject::FileError(const long lComp_ID,
                            LPCTSTR szTrcId,
                            const int nSourceFileLine,
                            const int nErrorLevel,
                            long eventActionMask,            // Event Log Action Mask
                            LPCTSTR szDesc,
                            CFileException &eFerr,
                            CString * pErrorText,            // error text    
                            TraceObject* pTrace             //Trace Object
                            )
{
  TCHAR *szError = FileErrorCause(eFerr);


  Error(
    lComp_ID,
    szTrcId,
    nSourceFileLine,
    nErrorLevel,
    SM_CAT_FILE_IO,
    SCOTSERVICES_EO_FILE,
    eventActionMask,
    pErrorText,
    pTrace,
    _T("%s|%s|%s|%x"),
    eFerr.m_strFileName,
    szDesc,
    szError,
    eFerr.m_cause);

  delete [] szError;

  return;
}

/////////////////////////////////////////////////////////////
// Get event type from error level
EventTypeEnum ErrorObject::GetEventType(const int nErrorLevel)
{
  EventTypeEnum eventType;
   switch ( nErrorLevel )     //transfer error level to event type
  {
    case C_INFO:
        eventType = evlTypeInformation;
        break;
    case C_RETURN:
        eventType = evlTypeWarning;
        break;       
    case C_RESTART:
        eventType = evlTypeError;
        break;
    case C_FATAL:
        eventType = evlTypeError;
        break;
    default:
        ASSERT(0);    //should not come here
  } 
   return eventType;
}

/////////////////////////////////////////////////////////////
// Get Comp text
CString ErrorObject::GetCompText(const long lComp_ID)
{
  CString csCompId;
  switch (lComp_ID)
  {
  case ID_MP: csCompId = _T("MP"); break;	// MP Main process
  case ID_SM: csCompId = _T("SM"); break;	// All state classes
  case ID_HC: csCompId = _T("HC"); break;	// HC Emulator components
  case ID_PS: csCompId = _T("PS"); break;	// PS Interface components
  case ID_DM: csCompId = _T("DM"); break;	// DM Interface components
  case ID_SA: csCompId = _T("SA"); break;	// SA Interface components
  case ID_TB: csCompId = _T("TB"); break;	// TS Interface components
  case ID_TO: csCompId = _T("TO"); break;	// Trace service
  case ID_EO: csCompId = _T("EO"); break;	// Error service
  case ID_MO: csCompId = _T("MO"); break;	// Message service
  case ID_CO: csCompId = _T("CO"); break;	// Configuration object
  case ID_UE: csCompId = _T("UE"); break;	// User Exits
  case ID_GP: csCompId = _T("GP"); break;	// File Utilities
  case ID_TI: csCompId = _T("TI"); break;	// TB Interface
  case ID_OD: csCompId = _T("OD"); break;	// New OPOS Device Support for 2.0
  case ID_SECMGR: csCompId = _T("SECMGR"); break; // New Security Manager
  case ID_RA: csCompId = _T("RA"); break;	// New Remote Console Manager
  case ID_RP: csCompId = _T("RP"); break;    //Report Transporter or Server
  case ID_CU: csCompId = _T("CU"); break;    //Customer Info object
  case ID_XM: csCompId = _T("XM"); break;   // XM Interface components
  default:
    ASSERT(0);   // Should not be here
  }
  return csCompId;
}


/////////////////////////////////////////////////////////////
// Transfer parameter list to array
void  ErrorObject::PutStringIntoArray(LPTSTR FormatPtr, LPCTSTR *ParamsArray, WORD &NumParams)
{
    BOOL Done = FALSE;
    static const TCHAR* szBlank = _T(" ");
    bool bNullString;
    while (!Done)
    {
        bNullString = true;
        // store the pointer to the current parameter string
        ParamsArray[NumParams] = FormatPtr;
        // advance FormatPtr to the end of the current parameter string
        while (*FormatPtr && (*FormatPtr != _T('|')))
        {
            bNullString = false;
            FormatPtr++;
        }
        if (*FormatPtr == _T('|'))
        {
            // format specifier was terminated by a delimiter so change the
            // delimiter to a null terminator
            *FormatPtr++ = 0;
        }
        else
        {
            // format specifier was terminated by the null terminator so
            // set the Done flag to exit the parameter loop
            Done = TRUE;
        }
        if (bNullString)
        {
            ParamsArray[NumParams] = szBlank;
        }
        NumParams++;
    }
  return;
}

CString ErrorObject::GetErrorText(const int nErrorLevel, ULONG eventID, TCHAR* szLogBuff, LPCTSTR * ParamsArray)
{
    //Get message from scotappmsg.mc
    HKEY hEventSource;
    CString message;
    CString subKey;
    CString path;

    subKey.Format(_T("%s\\%s"), MSGFILEKEY, m_EventSource);
    if (RegOpenKey(HKEY_LOCAL_MACHINE, subKey, &hEventSource) == ERROR_SUCCESS)
    {
        DWORD type;
        path.GetBuffer(_MAX_PATH);
        DWORD len = _MAX_PATH;

        RegQueryValueEx(hEventSource, _T("EventMessageFile"), NULL, &type, (LPBYTE) (LPCTSTR) path, &len);
        RegCloseKey(hEventSource);
    }

    HINSTANCE hDll = LoadLibrary(path);

    message.GetBuffer(1024);

    DWORD rc = FormatMessage(FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_ARGUMENT_ARRAY, 
                             (HMODULE) hDll, eventID,
                             0, (LPTSTR) (LPCTSTR) message, 1024, (va_list *)ParamsArray);

    message.ReleaseBuffer();

    if (rc == 0)
    {
        message.Format(_T("MsgId %d not found - insertion strings: %s\n"), eventID, szLogBuff);
        //AfxMessageBox(message); //tar 260690 - problem reported from KVAT
        message = _T("");
    }

    FreeLibrary(hDll);

    return  message;     // Get the error text 
	/*	MainErrorLogger(
			nErrorLevel,
			lComp_ID,
			csCompId,
			szTrcId,
			nSourceFileLine,
			szBuff);
      */
}

void ErrorObject::ShowError(CString message, const int nErrorLevel)
{

    UINT nType;
    switch (nErrorLevel)
    {
    case C_RETURN:
      nType = MB_ICONEXCLAMATION | MB_TOPMOST; // TAR 107096
      break;
    default:
      nType = MB_ICONERROR | MB_TOPMOST; // TAR 107096
      break;
    }
    CString csTemp;
    int nFound = message.Find(_T("Trace 1:"));
    if (nFound == -1)  //not found
      csTemp = message;
    else
      csTemp = message.Left(nFound-2);  //also remove two newline symbol before "Trace 1:"

    AfxMessageBox(csTemp,nType);
    
    return;
}
