//////////////////////////////////////////////////////////////////////////////
//
// FILE:  TraceObject.CPP
//
// TITLE:   Class implementation file (describe class here)
//
//
// AUTHOR:  DcL-
//
//////////////////////////////////////////////////////////////////////////////
// Revision History
//.tve.212462  8/20/02 Corrected mS timebase from start of application to start of NT to  
//				agree with DataCapture logs.  Also increased field size from 7 to 10 digits
//				to trace the complete time stamp.  

#include <stdafx.h>                 // MS MFC always needed first
#include <afxmt.h>                  // Multithreading header
#include <io.h>

#include "ErrorObject.h"            
#include "TraceObject.h"
#include "psapi.h"

#define COMP_ID ID_TO
#define T_ID _T("Trace")

static CCriticalSection CritSec;

extern int SCOTDumpStack( CStdioFile& traceFile );


#define EVENTSOURCE _T("SCOTSERVICES") 
#define MAX_TRACE_WINDOW_STRING_LENGTH 493
const DWORD DEFAULT_TRACE_LEVELS = 0x7 ; 
const int MAX_LINES_BEFORE_FLUSH = 50 ;
TraceObject* TraceObject::ListofAllTraceObjs[100] = {NULL};
short TraceObject::nTraceObjectCount = 0;

///////////////////////////
// Constructor
//////////////////////////
TraceObject::TraceObject(LPCTSTR TraceFile)
: nCurrentLevel(6),								  // tracing to level 6 until CO changes it
nFocusLevel(0),										  // no special level
lFocusComps(0),										  // no special focus
nSavedEntry(0),
m_nEntriesToFlush(MAX_LINES_BEFORE_FLUSH),							  // current number
m_nMaxLogSize(0),
m_nCountOfEntries(0),
m_strTraceFileName(TraceFile),
m_bTraceFileOpen(false),
m_bTraceOn(true),  
m_eoServices(EVENTSOURCE),
fFirstTime(true),
m_bFirstMemTime(true),
m_bHavePSAPI(true),
m_bInitialized(false),
m_bFlushFromTimerThread(false),
m_pTimerThread(NULL)
{
  // Resolve any environment string(s) that may exist in m_strTraceFileName.
  ExpandEnvironmentStrings(TraceFile, m_strTraceFileName.GetBuffer(_MAX_PATH), _MAX_PATH);
  m_strTraceFileName.ReleaseBuffer();

  for (int i=0;i<MAX_SAVED_TRACES;i++)
  {
    nSavedLine[i] = 0;
    csSavedTid[i] = _T("?");
  }
  m_fileTrace.m_hFile = NULL;
	if (!InitRegistrySettings())
		m_dwTraceLevels = DEFAULT_TRACE_LEVELS ;  // default trace levels: Error, Warning and Info
    if (!TraceObject::nTraceObjectCount)
    {
        for(int x=0;x<100;++x)
        {
            TraceObject::ListofAllTraceObjs[x] = NULL;
        }
    }
    TraceObject::ListofAllTraceObjs[TraceObject::nTraceObjectCount] = this;
    ++TraceObject::nTraceObjectCount;
    TRACE(_T("*********  ANOTHER TRACE OBJECT CREATED   ************"));
}

////////////////////////////////////////////////////////////////////////////
// Destructor
//   Close open trace files. If an error  occurs here then notify and abort.
////////////////////////////////////////////////////////////////////////////
TraceObject::~TraceObject()
{
  UnInitialize();
    // Set reference in list of objects to null
    for(int x=0; x<TraceObject::nTraceObjectCount ;++x)
    {

        if (TraceObject::ListofAllTraceObjs[x] == this)
        {
            TraceObject::ListofAllTraceObjs[x] = NULL;
        }
    }

}

void TraceObject::UnInitialize()
{
  StopTimerThread(); //THD Performance RFC
  if (m_bInitialized)
  {
    CloseTraceFile() ;
    m_eoServices.Uninitialize();
    m_bInitialized = false;
  }
}
bool TraceObject::InitRegistrySettings()
{
	bool bReturn = true;
	HKEY hKey ;
	long lRC ;

	lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\TraceLog"), 
						 0, KEY_QUERY_VALUE, &hKey);
	ASSERT(lRC==ERROR_SUCCESS);
	if (lRC!=ERROR_SUCCESS)
	{
		bReturn = false; 
		// trace the Key is not in registry 
	}
	else
	{
		DWORD dwDataSize = 4 ;
		// Get the value for TraceLevel
		lRC = RegQueryValueEx(hKey, _T("TraceLevels"), NULL, NULL, 
							  (LPBYTE)&m_dwTraceLevels, &dwDataSize);
		ASSERT(lRC==ERROR_SUCCESS ) ;
		bReturn = (lRC==ERROR_SUCCESS) ; // set return value

		::RegCloseKey(hKey);
	}
	return bReturn ;
}

// write a trace line into traces.log with four levels, Warning, Info, Debug and Error
int TraceObject::TraceNew
(
	const long	lCompID,			// COMP_ID
	LPCTSTR		  pszTraceId,			// T_ID,
	const int	  nSourceLineNum ,    // source file line number
	const long	lTraceLevel,        // Warning, Info, Debug and Error
	LPCTSTR		  pszFormat,          // prntf fmt string
	...)							// variabler args
{
  CSingleLock sl( &CritSec, false );
  sl.Lock();                        // enter critical section

  try
  {
	  if(fFirstTime)   // If first time, open the eventlog
	  {
		m_eoServices.Initialize();  // Initialize the ErrorEventLog object
		fFirstTime = false;
		m_bInitialized = true;
	  }

	  // save trace ID and line number in a wrap around array
	  // this can be used in a fatal error report(to event log) to show
	  // the last few trace entries
	  nSavedLine[nSavedEntry] = nSourceLineNum;
	  csSavedTid[nSavedEntry] = pszTraceId;
	  nSavedEntry++;
	  if (nSavedEntry==MAX_SAVED_TRACES)
		nSavedEntry = 0;
		  
	  // Tracing criteria: if focusComps exists and the current component does not belong to focusComponents or
	  // the tracelevels are not levels to be traced, then return without trace.
	  if (!m_bTraceOn || (lFocusComps && !(lCompID & lFocusComps)) || !(lTraceLevel & m_dwTraceLevels))
		return 0;   // trace cretiera is not met, return without tracing
			
		
	  // ok we need to create a trace record
	  _TCHAR szBuff[MAX_TRACE_BUF+1];
	  CString csBuff, csTicks, csSystime, csCompId;
	  CTime ctTime = CTime::GetCurrentTime();
		
	  switch (lCompID)  // switch is not working currently,all breaks
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
	  case ID_TI: csCompId = _T("TI"); break;	// File Utilities
	  case ID_SECMGR: csCompId = _T("SECMGR"); break; // New Security Manager
	  case ID_CU: csCompId = _T("CU"); break;
	 }
		
	  va_list argptr;
	  va_start( argptr, pszFormat );        // variable parameter list processing
	
	  csSystime = ctTime.Format(_T("%m/%d %H:%M:%S;"));
	//.tve.212462  csTicks.Format("%07d",clock());
		csTicks.Format(_T("%010d"),GetTickCount());			//.tve.212462 
	  csSystime += csTicks;

	  if (m_bTraceFileOpen || OpenTraceFile())  // if the file is not opened and 
	  {                                           // now open it successfully
	   // format and write to file here
		csBuff.Format(
		  _T("%4d) %s %s-%s@%-4d, %d, "),
		  m_nCountOfEntries,
		  csSystime,
		  csCompId,
		  pszTraceId,
		  nSourceLineNum,
		  lTraceLevel);
			
		_vsntprintf( szBuff, MAX_TRACE_BUF, pszFormat, argptr );	// variable parameter list processing

	#ifndef _UNICODE		
		// convert all unprintables to '~'
		LPTSTR p = szBuff;
		int i = 0;
		while (p[i] && i < MAX_TRACE_BUF)
		{
		  if (p[i]<0x20) p[i] = _T('~');
		  if (p[i]>0x7f) p[i] = _T('~');
		  i++;
		}
		p[MAX_TRACE_BUF]=0;
	#endif
			
		csBuff += szBuff;
		csBuff += _T("\r\n");                 // include at least one newline
		int nLen = csBuff.GetLength();
		// please do not add any extra text to the below "TRACE" lines
		// because Afxtrace has limitations on the length of the trace buffer
		if (nLen < (MAX_TRACE_WINDOW_STRING_LENGTH))
		{
			TRACE(_T("%s\n"), csBuff);
		}
		else //tar 238655 Modified algorithm to accomodate larger blocks of data without generating assertion in MFC
		{
			int nIndex = 0;
			CString csTemp;
			while (nIndex < nLen)
			{
				csTemp = csBuff.Mid(nIndex, nLen - nIndex >= MAX_TRACE_WINDOW_STRING_LENGTH ? MAX_TRACE_WINDOW_STRING_LENGTH : nLen - nIndex );
    			TRACE(_T("%s\n"), csTemp);
				nIndex += csTemp.GetLength();
			}
		}
			
		try
		{
		  m_fileTrace.Write( csBuff, csBuff.GetLength()*sizeof(TCHAR) );
      
		  if (++m_nCountOfEntries % m_nEntriesToFlush == 0) 
		  {
			TraceFileFlush() ;
		  }
		}
		catch( CFileException *e )
		{
		  m_eoServices.FileError(RETURN,_T("Write"),*e);
		  e->Delete();
		}
	 }
  }
  catch(...)
  {
	 // Be carefull here, you have an exception during a trace. 
	 // Trying to add tracing here to show the message could cause a recursive exception 
	 // and crash the app.  This has been happening.  
	 // There's probably something wrong with the message format.
	 TRACE(_T("TraceObject::TraceNew Exception internally - unable show the trace") );
  }

  sl.Unlock();                      // exit critical section
	
  return 0;
}
	
//////////////////////////////////////////
// Trace
//////////////////////////////////////////
int TraceObject::Trace(const long lCompId,
                       LPCTSTR szTrcId,
                       const int nSrcLine,
                       const int nLevel,
                       LPCTSTR szFmt,...)
{
  CSingleLock sl( &CritSec, false );
  sl.Lock();                        // enter critical section

  try
  {

	  if(fFirstTime)   // If first time, oprn the eventlog
	  {
		m_eoServices.Initialize();  // Initialize the ErrorEventLog object
		fFirstTime = false;
		m_bInitialized = true;
	  }


	  // save trace ID and line number in a wrap around array
	  // this can be used in a fatal error report(to event log) to show
	  // the last few trace entries
	  nSavedLine[nSavedEntry] = nSrcLine;
	  csSavedTid[nSavedEntry] = szTrcId;
	  nSavedEntry++;
	  if (nSavedEntry==MAX_SAVED_TRACES)
		nSavedEntry = 0;
		

	  if (!m_bTraceOn ||!(((lCompId & lFocusComps) && nLevel<=nFocusLevel) || nLevel<=nCurrentLevel))
		return 0 ;	
	  // ok we need to create a trace record
	  _TCHAR szBuff[MAX_TRACE_BUF+1];
	  CString csBuff, csTicks, csSystime, csCompId;
	  CTime ctTime = CTime::GetCurrentTime();
		
	  switch (lCompId)  // switch is not working currently,all breaks
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
	  case ID_TI: csCompId = _T("TI"); break;	// File Utilities
	  }
		
	  va_list argptr;
	  va_start( argptr, szFmt );        // variable parameter list processing
		
	  csSystime = ctTime.Format(_T("%m/%d %H:%M:%S;"));
	//.tve.212462  csTicks.Format("%07d",clock());
	  csTicks.Format(_T("%010d"),GetTickCount());			//.tve.212462 
	  csSystime += csTicks;
		
	  DWORD dwThreadID = GetCurrentThreadId(); //RFC 410844
		
	  if (m_bTraceFileOpen || OpenTraceFile())  // if the file is not opened and 
	  {                                           // now open it successfully
		csBuff.Format(
		  _T("%4d) %s %04X> %s-%s@%-4d "),
		  m_nCountOfEntries,
		  csSystime,
		  dwThreadID,
		  csCompId,
		  szTrcId,
		  nSrcLine);
			
		_vsntprintf( szBuff, MAX_TRACE_BUF, szFmt, argptr );	// variable parameter list processing
		
	#ifndef _UNICODE
		// convert all unprintables to '~'
		LPTSTR p = szBuff;
		int i = 0;
		while (p[i] && i < MAX_TRACE_BUF)
		{
		  if (p[i]<0x20) p[i] = _T('~');
		  if (p[i]>0x7f) p[i] = _T('~');
		  i++;
		}
		p[MAX_TRACE_BUF]=0;
	#endif
			
		csBuff += szBuff;
		csBuff += _T("\r\n"); // include at carriage return/newline chars

		int nLen = csBuff.GetLength();
		// please do not add any extra text to the below "TRACE" lines
		// because Afxtrace has limitations on the length of the trace buffer
		if (nLen < (MAX_TRACE_WINDOW_STRING_LENGTH))
		{
			TRACE(_T("TraceObject::Trace:%s\n"), csBuff);
		}
		else //tar 238655 Modified algorithm to accomodate larger blocks of data without generating assertion in MFC
		{
			int nIndex = 0;
			CString csTemp;
			while (nIndex < nLen)
			{
				csTemp = csBuff.Mid(nIndex, nLen - nIndex >= MAX_TRACE_WINDOW_STRING_LENGTH ? MAX_TRACE_WINDOW_STRING_LENGTH : nLen - nIndex );
    			TRACE(_T("%s\n"), csTemp);
				nIndex += csTemp.GetLength();
			}
		}
			
		try
		{
		  
		  m_fileTrace.Write( csBuff, csBuff.GetLength()*sizeof(TCHAR));
		  if (++m_nCountOfEntries % m_nEntriesToFlush == 0) 
		  {
			TraceFileFlush() ;
		  }
		}
		catch( CFileException *e )
		{
		  m_eoServices.FileError(RETURN,_T("Write"),*e);
		  e->Delete();
		}
			
	  }

  }
  catch(...)
  {
	 // Be carefull here, you have an exception during a trace. 
	 // Trying to add tracing here to show the message could cause a recursive exception 
	 // and crash the app.  This has been happening.  
	 // There's probably something wrong with the message format.
	 TRACE(_T("TraceObject::Trace Exception internally - unable show the trace") );
  }

  sl.Unlock();                      // exit critical section
	
  return 0;
}

bool TraceObject::OpenTraceFile()
{
  // assume the file is not opened
  ASSERT(!m_bTraceFileOpen) ;
  CFileException eFerr;
   if (!m_fileTrace.Open(m_strTraceFileName,
      CFile::modeCreate |
      CFile::modeNoTruncate |  
      CFile::modeWrite |
   	  CFile::shareDenyWrite |
      CFile::typeBinary   , //Make Unicode trace file for Unicode scotapp
      &eFerr))
   {
      //tar 260731
	  //AfxMessageBox(_T("Unable to open ")+m_strTraceFileName+_T(" in TraceObject"),
      //               MB_OK | MB_ICONEXCLAMATION );
#ifdef _DEBUG
   	  TRACE(_T("Unable to open %s in TraceObject\n"), m_strTraceFileName);

      TCHAR *szError = ErrorObject::FileErrorCause(eFerr);
      //AfxMessageBox(szError,
      //              MB_OK | MB_ICONEXCLAMATION );
	  TRACE(_T("%s\n"), szError);
	  delete[] szError;
#endif
    
      m_eoServices.FileError(RETURN,_T("Create ")+m_strTraceFileName,eFerr);

      return false ;
   }
   else
   {
     m_fileTrace.SeekToEnd( );

	 //+TAR 380693
	 #ifdef _UNICODE
     // The unicode standard requires that all unicode files are prefixed with their 
     // type. For Windows UTF-16 we use little endian notation which requires a two byte header 
     // at the top of the file: FF FE.  This will ensure compliance with true unicode text
     // files and will ensure that 3rd party programs can properly interpret the file.
     if( !m_fileTrace.GetPosition() )
     {
         BYTE by[] = { 0xFF, 0xFE };
         m_fileTrace.Write(by, 2);
     }
     #endif
	 //-TAR 380693

     m_bTraceFileOpen=true ;
     return true ;
   }
}

void TraceObject::CloseTraceFile()
{
  CSingleLock sl( &CritSec, false );
  sl.Lock();                        // enter critical section
	
  if (m_bTraceFileOpen)
  {
    // Close open file.

	  //+TAR247967 - changing all upper case MFC Macro TRY/CATCH to C++ try/catch
	  try
	  {
		  m_fileTrace.Close();
		  m_fileTrace.m_hFile = NULL; // TAR 311421
	  }
	  catch ( CFileException* e )
	  {
		  e->Delete();
	  }
	  //-TAR247967

    m_bTraceFileOpen = false;
  }
	sl.Unlock();                      // exit critical section
}

void TraceObject::TraceFileFlush()
{
    CSingleLock sl( &CritSec, false );
    sl.Lock();                        // enter critical section
    
	PauseTimerThread(); //THD Performance RFC
    try //added for tar 238817
    {
		if (m_bTraceFileOpen) // TAR 311421
		{
			m_fileTrace.Flush();
		}
    }
    catch( CFileException *e )
    {
        CString csErrorText;
        csErrorText.Format(_T("m_fileTrace.Flush threw CFileException::%d"),e->m_cause);
        m_eoServices.FileError(RETURN,csErrorText,*e);
        e->Delete();
    }
    catch( CException *e)
    {
      TCHAR   szCause[255];
      CString strFormatted;

      e->GetErrorMessage(szCause, 255);  
    }
    
    if (m_fileTrace != NULL)   // TAR 198401
    {
        DWORD dFileLength = m_fileTrace.GetLength() ;
        if ((int)(dFileLength/1024) >= m_nMaxLogSize && m_nMaxLogSize > 0)
        {
            BackupTraceFile();
        }
    }
    ResumeTimerThread();	//THD Performance RFC
    sl.Unlock();                      // exit critical section
}

void TraceObject::BackupTraceFile()
{
  CString csBackupFile, csTraceFile  ; 
  long lFile; 

  csTraceFile = m_fileTrace.GetFilePath() ;
  csBackupFile = csTraceFile + _T(".bak") ;

  struct _tfinddata_t c_file;

  //TAR 357296
  CSingleLock sl( &CritSec, false );
  sl.Lock();                        // enter critical section
  if (m_bTraceFileOpen)
  {
    // Close open file.

	  //+TAR247967 - changing all upper case MFC Macro TRY/CATCH to C++ try/catch
	  try
	  {
		  m_fileTrace.Close();
		  m_fileTrace.m_hFile = NULL; // TAR 311421
	  }
	  catch ( CFileException* e )
	  {
		  m_eoServices.FileError(RETURN,_T("Close for Backup"),*e);
		  e->Delete();
		  sl.Unlock();
          return;
	  }
	  //-TAR247967

    m_bTraceFileOpen = false;
  }

  try
  {
	  if ((lFile = _tfindfirst((LPTSTR) (LPCTSTR)csBackupFile, &c_file )) != -1L)  // found the old file
	  {
		  _findclose(lFile); //must close handle
		  CFile::Remove(csBackupFile) ;
	  }
	  CFile::Rename(csTraceFile, csBackupFile) ;
	  // rename the file ;
	  OpenTraceFile() ;
  }
  catch ( CFileException* e )
  {
	  m_eoServices.FileError(RETURN,_T("Rename for Backup"),*e);
	  e->Delete();
	  sl.Unlock();
  }
  
  sl.Unlock();
  //-TAR 357296 
}
//////////////////////////////////////////////
// Hex Trace
//////////////////////////////////////////////
int TraceObject::HexTrace(const long lCompId,
                          LPCTSTR szTrcId,
                          const int nSrcLine,
                          const int nLevel,
                          LPCTSTR title,
                          LPCTSTR buffer,
                          const int length)
{
	Trace(lCompId,szTrcId,nSrcLine,nLevel,_T("Hex: %s,%d"),
    title,length);

	if (length==0)
		return 0;

  LPTSTR szWork = new _TCHAR[length*3+10];
  LPTSTR p;
  _TUCHAR *b;
  int len = length;

  p = szWork;
  b = (_TUCHAR *)buffer;

  // Fix the assertion problem when the string exceeds 512 bytes.
  if (length > 165) 
  {
     len = 165;
  }	
 
  for (int i=0;i<len;i++)
  {
    _stprintf(p,_T(" %02x"),*b++);
    p += 3;
  }

  Trace(lCompId,szTrcId,nSrcLine,nLevel,szWork);
  	
  delete szWork;
	
  return 0;
}

typedef BOOL (WINAPI *FP_PSAPI_MEMFUNC)(HANDLE Process, 
                                        PPROCESS_MEMORY_COUNTERS ppsmemCounters,
                                        DWORD cb);

int TraceObject::TraceMemUsage(const long lCompId,
                               LPCTSTR szTrcId,
                               const int nSrcLine,
                               const int nLevel)
{
  int rc = 0;


  if (!m_bTraceOn ||!(((lCompId & lFocusComps) && nLevel<=nFocusLevel) || nLevel<=nCurrentLevel))
    return 0 ;	

  if (m_bFirstMemTime)
  {
    m_bHavePSAPI = false;
    m_bFirstMemTime = false;
    // Open up our process using psapi.dll
    m_hPSAPI = LoadLibrary(_T("psapi.dll"));
    if (m_hPSAPI != NULL)
    {
      m_pMemFunc = GetProcAddress(m_hPSAPI, ("GetProcessMemoryInfo"));
      if (m_pMemFunc != NULL)
      {
        m_hProcess = GetCurrentProcess();
        if (m_hProcess != NULL)
        {
          m_bHavePSAPI = true;
        }
      }
    }
  }

  if (m_bHavePSAPI)
  {
    ASSERT(m_hPSAPI && m_pMemFunc && m_hProcess);
    PROCESS_MEMORY_COUNTERS memCounters;

    if (((FP_PSAPI_MEMFUNC)m_pMemFunc)(m_hProcess, &memCounters, sizeof(memCounters)))
    {
      CString csBuff;
      csBuff.Format(_T("WSS=%d, PWSS=%d, PFS=%d, PPFS=%d, PFC=%d"),
                    memCounters.WorkingSetSize,
                    memCounters.PeakWorkingSetSize,
                    memCounters.PagefileUsage,
                    memCounters.PeakPagefileUsage,
                    memCounters.PageFaultCount);

      Trace(lCompId,szTrcId,nSrcLine,nLevel, csBuff);
    }
  }
  return rc;
}


int TraceObject::TraceStackDump(const long lCompId,
                                LPCTSTR szTrcId,
                                const int nSrcLine,
                                const int nLevel)
{
  int rc = 0;


  if (!m_bTraceOn ||!(((lCompId & lFocusComps) && nLevel<=nFocusLevel) || nLevel<=nCurrentLevel))
    return 0 ;	

  Trace(lCompId,szTrcId,nSrcLine,nLevel, _T("Dumping stack ..."));
  SCOTDumpStack(m_fileTrace);
  Trace(lCompId,szTrcId,nSrcLine,nLevel, _T("Finished dumping stack."));

  return rc;
}
int TraceObject::FlushAllObjects()
{
	try 
	{
		for(int x=0; x<TraceObject::nTraceObjectCount ;++x)
		{
			TraceObject* pObj = TraceObject::ListofAllTraceObjs[x];
			Trace(COMP_ID, _T("TraceObject"), 999, 6, _T("Flushing object %x"), pObj);
			if (pObj)
			{
				pObj->TraceFileFlush();
			}
		}
	}
    catch( CFileException *e )
    {
      m_eoServices.FileError(RETURN,_T("Write"),*e);
      e->Delete();
    }
    return 1; // not of any use right now
}
//+THD Performance RFC
void TraceObject::SetTimeToFlush( int nMilliseconds )
{
	m_nMillisecondsToFlush = nMilliseconds ;
	if ( m_nMillisecondsToFlush > 0 )
	{
		if ( NULL == m_pTimerThread ) 
		{
			m_pTimerThread = AfxBeginThread( (AFX_THREADPROC)TimerThreadEntry, (LPVOID)this,0 ,0, NULL); 
			ASSERT( NULL != m_pTimerThread  );
		}	
	}else
	{
		if ( NULL != m_pTimerThread  )
		{
			StopTimerThread();	
			ASSERT( NULL == m_pTimerThread );
		}
	}
}


UINT TraceObject::TimerThreadEntry( LPVOID pParam )
{
   ((TraceObject*)pParam )->TimerThread();
   return 0;
}

void TraceObject::TimerThread()
{
	m_hThreadEvents[ PAUSE ] = CreateEvent( NULL, FALSE, FALSE, NULL );
	m_hThreadEvents[ RESUME ] = CreateEvent( NULL, FALSE, FALSE, NULL  );
	m_hThreadEvents[ STOP ] = CreateEvent( NULL, FALSE, FALSE, NULL  );
	m_hThreadEvents[ PAUSESUCCESSFUL ] = CreateEvent( NULL, FALSE, FALSE, NULL  );
	bool bLoop = true;
	DWORD dwEvents ;
	while( bLoop )
	{
		//wait for events for a period (m_nMillisecondsToFlush) milliseconds
		//if events are not received then call TraceFileFlush()
		dwEvents = WaitForMultipleObjects( MAX_THREAD_EVENTS, m_hThreadEvents, FALSE, m_nMillisecondsToFlush );  
		switch( dwEvents  )
		{
			case WAIT_OBJECT_0 + 0: 
				//TRACE( _T(" TimerThread - receive PAUSE  \n" ) );
				//TRACE( _T(" TimerThread - fire PAUSESUCCESSFUL \n" ) );
				SetEvent( m_hThreadEvents[ PAUSESUCCESSFUL ] );
				WaitForSingleObject( m_hThreadEvents[ RESUME ], INFINITE);  //reset flush timer
				//TRACE(_T("TimerThread - receive RESUME \n"));
				break;
			case WAIT_OBJECT_0 + 2: // stop thread
				//TRACE( _T(" TimerThread - receive STOP \n" ) );
				bLoop = false;
				// Fall through without break to default case that flushes buffer
			default:
				m_bFlushFromTimerThread = true;
				TraceFileFlush();
				m_bFlushFromTimerThread = false;
		}
	}
}

void TraceObject::PauseTimerThread()
{
	if ( NULL != m_pTimerThread  && !m_bFlushFromTimerThread  )
	{
		//TRACE( _T(" TimerThread - fire  PAUSE \n" ) );		
		SetEvent( m_hThreadEvents[ PAUSE ] ); //tell TimerThread to pause
		WaitForSingleObject( m_hThreadEvents[ PAUSESUCCESSFUL ], INFINITE); //wait for timer thread to pause
		//TRACE( _T(" TimerThread - receive  PAUSESUCCESSFUL \n" ) );
	}
}

void TraceObject::ResumeTimerThread()
{
	if ( NULL != m_pTimerThread  && !m_bFlushFromTimerThread  )
	{
		//TRACE( _T(" TimerThread - fire RESUME \n" ) );
		SetEvent( m_hThreadEvents[ RESUME ] ); //resume TimerThread
	}
}

void TraceObject::StopTimerThread()
{
  if ( NULL != m_pTimerThread  )
  {
	//TRACE( _T(" TimerThread - fire STOP \n" ) );
	SetEvent( m_hThreadEvents[ STOP ] );  // fire STOP event of timer thread
	WaitForSingleObject( m_pTimerThread->m_hThread, INFINITE ); //wait for timer thread to stop
	//TRACE( _T(" TimerThread - EXIT \n" ) );

	//+close thread events 
	for ( int iLoop = 0; iLoop < MAX_THREAD_EVENTS; iLoop++ )
	{
		CloseHandle( m_hThreadEvents[ iLoop ] );
	}
	//-close thread events  

	m_pTimerThread  = NULL;
  }
}

//-THD Performance RFC
