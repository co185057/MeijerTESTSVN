// TraceSystem.cpp: implementation of the TraceSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

// if using MFC, include the MFC TraceMgr and collection logic
#ifndef ATLASSERT
	#define ATLASSERT ASSERT
#endif

#define TRACESYSTEM_MAIN
#include "TraceSystem.h"

CRITICAL_SECTION CritSecTraceCreate;

HANDLE HandleVector[3];
DWORD WINAPI COMClientThreadFunction(LPVOID ptr);

TCHAR *commaprint(unsigned long n)
{
	static int comma = _T(',');
	static TCHAR retbuf[30];
	TCHAR *p = &retbuf[sizeof(retbuf)-1];
	int i = 0;

	*p = _T('\0');
	do {
		if(i%3 == 0 && i != 0)
			*--p = comma;
		*--p = (TCHAR) (_T('0') + (n % 10));
		n /= 10;
		i++;
	} while((n != 0));

	return p;
}

void writeEventLog(LPCTSTR *szMsg)
{
	HANDLE hes= RegisterEventSource(NULL, _T("NCRTraceClient"));
	if (hes) {
		ReportEvent(hes, EVENTLOG_ERROR_TYPE,
			0,
			9999,
			NULL,
			1,
			0,
			szMsg,
			NULL);
		DeregisterEventSource(hes);
	}
}

CTraceSystemMgr::CTraceSystemMgr(LPCTSTR szTraceName/*=NULL*/, ULONG moduleId/*=TM_MODULE1*/)
:m_hCOMThread(0)
{
   m_tdata.m_pmq=NULL;
   m_tdata.m_csTraceName= szTraceName;
   m_moduleId= moduleId;
	createQueue();
	SetupCOMClientThread();
   InitializeCriticalSection(&CritSecTraceCreate);
}

CTraceSystemMgr::~CTraceSystemMgr()
{
/*
	CTraceSystem *pts;
	void *pvoid;
	EnterCriticalSection(&CritSecTraceCreate);
	for (POSITION pos = m_WriterPtrs.GetStartPosition(); pos!=NULL; ) {
		ASSERT(0); // USE ENDtbtrace in every thread to cleanup its trace object
		m_WriterPtrs.GetNextAssoc(pos, pvoid, (void *&)pts);
		//delete pts;  
		//pts=NULL;
	}
*/
	LeaveCriticalSection(&CritSecTraceCreate);
	DeleteCriticalSection(&CritSecTraceCreate);
    if ( NULL != HandleVector[2] )
    {
        SetEvent(HandleVector[2]);
    }
    if ( NULL != m_hCOMThread )
    {
        DWORD dwWait = WaitForSingleObject( m_hCOMThread, 10000 );
        if ( WAIT_OBJECT_0 != dwWait )
        {
            TerminateThread(this->m_hCOMThread, 0 );
        }
        CloseHandle(this->m_hCOMThread );
    }
    for ( int i=0; i<3; i++ )
    {
        if ( NULL != HandleVector[i] )
        {
            CloseHandle(HandleVector[i]);
        }
    }
    if ( NULL != m_tdata.m_pmq )
    {
	    qData *pqdata=NULL;
        while( ! m_tdata.m_pmq->empty() )
        {
            pqdata = m_tdata.m_pmq->front();
            m_tdata.m_pmq->pop();
            delete pqdata;
        }
        delete this->m_tdata.m_pmq;
    }
}


void CTraceSystemMgr::putMsg(qData *pqdata)
{
    if ( NULL == m_tdata.m_pmq )
    {
        delete pqdata;
        return;
    }

	WaitForSingleObject(HandleVector[1], INFINITE);	
	
	m_tdata.m_pmq->push( pqdata );        //insert the msg at the end of the queue
   
	SetEvent(HandleVector[0]);
	ReleaseMutex(HandleVector[1]); 
}

void CTraceSystemMgr::SetupCOMClientThread()
{
	DWORD IDThread=0;
	HandleVector[0]= CreateEvent(NULL, TRUE, FALSE, NULL);
	HandleVector[1]= CreateMutex(NULL, FALSE, NULL);
    HandleVector[2]= CreateEvent(NULL, FALSE, FALSE, NULL);
	if ((HandleVector[1] == NULL) || (HandleVector[0] == NULL) || (NULL == m_tdata.m_pmq)) {
		LPCTSTR szmsg= _T("CTraceSystemMgr::SetupCOMClientThread Error Creating Event&Mutex. COMClientThread aborted");
		writeEventLog(&szmsg);
		return;
	}
	m_hCOMThread = CreateThread(NULL, 0, COMClientThreadFunction, &m_tdata, 0, &IDThread);
}

long CTraceSystemMgr::Trace(ULONG traceMask,  LPCTSTR szsource, const int iLineNum, LPCTSTR sFormat, ...)
{
/******
   CTraceSystem *pts;
   long lResult;
	// can only call from same thread that created it
	EnterCriticalSection(&CritSecTraceCreate);
	try {
		if (0 == m_WriterPtrs.Lookup((void*)GetCurrentThreadId(), (void*&)pts)) {
   		CoInitialize(NULL);
			//new thread using trace. create a new object for it.
			pts = new CTraceSystem(m_csTraceName, m_moduleId);
			m_WriterPtrs.SetAt((void *)GetCurrentThreadId(), pts);
		}
		LeaveCriticalSection(&CritSecTraceCreate);
	}
	catch(...) {
		LeaveCriticalSection(&CritSecTraceCreate);
		LPCTSTR szmsg=_T("Exception in CTraceSystemMgr::Trace");
		writeEventLog(&szmsg);
	}

   try
   {
******/
		CString svComp(_T(""));
	   if(iLineNum && szsource && szsource[0])
		   svComp.Format(_T("%s@%d:"), szsource, iLineNum);

	   va_list argptr;
	   va_start( argptr, sFormat);
	   HRESULT lResult = internalTrace(traceMask, svComp, sFormat, argptr);
		va_end( argptr);
/******

   }
	catch(_com_error e)
   {
      LPCTSTR szmsg;
      szmsg= e.Description();
		writeEventLog(&szmsg);
   }
   catch(...)
   {
      LPCTSTR szmsg=_T("Exception in CTraceSystemMgr::Trace");
		writeEventLog(&szmsg);
   }
*****/
   return lResult;
}

long CTraceSystemMgr::HexTrace(ULONG traceMask, LPCTSTR szTitle, UCHAR *phexData, ULONG hexDataLen /*=0*/)
{
	qData *pqdata=new qData();
	pqdata->m_traceMask= traceMask;
	pqdata->m_csTrace.Format(_T("%s (%d)"),szTitle, hexDataLen);
	pqdata->SetData(phexData, hexDataLen);
	putMsg(pqdata);
   return S_OK;
}

long CTraceSystemMgr::HexTrace(ULONG traceMask, LPCTSTR szsource, const int iLineNum, 
							LPCTSTR szTitle, UCHAR *phexData, ULONG hexDataLen)
{
/*
	CTraceSystem *pts;
	// can only call from same thread that created it
	EnterCriticalSection(&CritSecTraceCreate);
	try {
		if (0 == m_WriterPtrs.Lookup((void*)GetCurrentThreadId(), (void*&)pts)) {
  			CoInitialize(NULL);
			//new thread using trace. create a new object for it.
			pts = new CTraceSystem(m_csTraceName, m_moduleId);
			m_WriterPtrs.SetAt((void *)GetCurrentThreadId(), pts);
		}
		LeaveCriticalSection(&CritSecTraceCreate);
	}
	catch(...) {
		LeaveCriticalSection(&CritSecTraceCreate);
      LPCTSTR szmsg=_T("Exception in CTraceSystemMgr::HexTrace");
		writeEventLog(&szmsg);
	}
	long lResult = pts->HexTrace(traceMask, szsource, iLineNum, szTitle, phexData, hexDataLen);
	return lResult;
*/

	CString strTemp;
	strTemp.Format(_T("%s@%d:%s"), szsource, iLineNum, szTitle);

   return HexTrace(traceMask, (LPCTSTR) strTemp, phexData, hexDataLen);
}

// the one that all others call.
long CTraceSystemMgr::internalTrace(ULONG traceMask,  LPCTSTR szsourceAndLine, LPCTSTR sFormat, va_list aList)
{
	int nIndent=0;
   CString csPrefix(_T("[?]"));
	ULONG nFinalMask= traceMask & 0x00FF;
   switch (nFinalMask) {
      case 0:  // mask filtered out
         return 0;
      case TRACE_MASK_ENTRY:
      case TRACE_MASK_IN:
         csPrefix=_T("[+]");
         break;
      case TRACE_MASK_OUT:
			nIndent--;
         csPrefix=_T("[-]");
         break;
      case TRACE_MASK_ERROR:
         csPrefix=_T("[E]");
         break;
      case TRACE_MASK_DEBUG:
         csPrefix=_T("[D]");
         break;
      case TRACE_MASK_INFO:
         csPrefix=_T("[I]");
         break;
      case TRACE_MASK_WARNING:
         csPrefix=_T("[W]");
         break;
		default:
			{
				if (nFinalMask & TRACE_MASK_IN)
					csPrefix=_T("[+]");
				else if (nFinalMask & TRACE_MASK_OUT) {
					nIndent--;
					csPrefix=_T("[-]");
				}
			}
   }
  	if (nFinalMask & TRACE_MASK_IN)
   	nIndent++;
   LONG lThreadId= GetCurrentThreadId();
	int nCurrentIndent=0;
	EnterCriticalSection(&CritSecTraceCreate);
	try {
		m_WriterIterator= m_WriterPtrs.find(lThreadId);
      if (m_WriterIterator != m_WriterPtrs.end())
         nCurrentIndent= (*m_WriterIterator).second;

		m_WriterPtrs[lThreadId] = nIndent + nCurrentIndent;
		LeaveCriticalSection(&CritSecTraceCreate);
	}
	catch(...) {
		LeaveCriticalSection(&CritSecTraceCreate);
		LPCTSTR szmsg=_T("Exception in internalTrace lookup");
		writeEventLog(&szmsg);
	}

   if (nIndent < 0)
      nCurrentIndent+= nIndent;
	CString csIndent(' ', nCurrentIndent * 2);

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	CString csBuff;
	csBuff.FormatV( sFormat, aList);
	
/*   CString strTemp;
	strTemp.Format(_T("%02d/%02d %02d:%02d:%02d %13s %04X> %s%s%s%s"), 
		sysTime.wMonth, sysTime.wDay, 
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond, commaprint(GetTickCount()),
      lThreadId,
      csIndent,
      csPrefix,
      szsourceAndLine,
      csBuff);*/
   CString strTemp;
	strTemp.Format(_T("%02d/%02d %02d:%02d:%02d %13u %04X> %s%s%s%s"), 
		sysTime.wMonth, sysTime.wDay, 
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond, GetTickCount(),
      lThreadId,
      csIndent,
      csPrefix,
      szsourceAndLine,
      csBuff);

	//strTemp.Replace(_T("%"), _T("%%"));

	qData *pqdata= new qData();
	pqdata->m_traceMask= m_moduleId | traceMask;
	pqdata->m_csTrace= strTemp;
	putMsg(pqdata);
	return 0;
}



void CTraceSystemMgr::StartTrace()
{
	/*
	CTraceSystem *pts;
	EnterCriticalSection(&CritSecTraceCreate);
	try {
		if (0 == m_WriterPtrs.Lookup((void*)GetCurrentThreadId(), (void*&)pts)) {
			//new thread using trace. create a new object for it.
   		CoInitialize(NULL);
			pts = new CTraceSystem(m_csTraceName, m_moduleId);
			m_WriterPtrs.SetAt((void *)GetCurrentThreadId(), pts);
		}
		LeaveCriticalSection(&CritSecTraceCreate);
	}
	catch(...) {
		LeaveCriticalSection(&CritSecTraceCreate);
      LPCTSTR szmsg=_T("Exception in CTraceSystemMgr::StartTrace");
		writeEventLog(&szmsg);
	}
	*/
}

void CTraceSystemMgr::EndTrace()
{
	/*
	CTraceSystem *pts;
	EnterCriticalSection(&CritSecTraceCreate);
	try {
		if (m_WriterPtrs.Lookup((void*)GetCurrentThreadId(), (void*&)pts)) {
			delete pts;  
			pts=NULL;
			m_WriterPtrs.RemoveKey((void*)GetCurrentThreadId());
   		CoUninitialize();
		}
		LeaveCriticalSection(&CritSecTraceCreate);
	}
	catch(...) {
		LeaveCriticalSection(&CritSecTraceCreate);
      LPCTSTR szmsg=_T("Exception in CTraceSystemMgr::EndTrace");
		writeEventLog(&szmsg);
	}
	*/
}


//struct initCom 
//{
//	initCom() {CoInitialize(NULL);};
//	~initCom(){CoUninitialize();};
//} g_initCom;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTraceSystem::CTraceSystem(LPCTSTR szTraceLog, ULONG moduleId)
:m_bCreated(false),	m_TraceLog(szTraceLog), m_bStop(false)
, m_ncurMask(0xFFFF),m_threadId(0), m_moduleId(moduleId)
, m_nIndent(0)
{
   m_threadId= GetCurrentThreadId();
}

CTraceSystem::~CTraceSystem()
{
	try {
//		ptw.Release();
   }
	catch(_com_error e){
		// createinstance may have failed
	}
}	

void CTraceSystem::Release()
{
    if ( m_bCreated )
    {
        ptw.Release();
        m_bCreated = false;
    }
}

bool CTraceSystem::Create(void)
{
//	EnterCriticalSection(&CritSecTraceCreate);
	// if at this point it is created, we do not want to do it again
	if (!m_bCreated) {
		try{
			ptw.CreateInstance((__uuidof(CTraceWriter)));
			m_ncurMask= ptw->CreateAndGetMask((LPCTSTR)m_TraceLog);
			m_bCreated=true;
		}
		catch(_com_error e){
			m_bCreated=false;
			LPCTSTR szmsg = _T("TraceWriter Create failed! Starting new instance.");
			writeEventLog(&szmsg);
         
         szmsg= e.Description();
         writeEventLog(&szmsg);
			try {
            Sleep(2000);
				ptw.CreateInstance((__uuidof(CTraceWriter)));
				m_ncurMask= ptw->CreateAndGetMask((LPCTSTR)m_TraceLog);
				m_bCreated=true;
			}
			catch(_com_error e2){
				// if still a problem, shut this off
				m_bCreated=false; 
				m_bStop=true;
				LPCTSTR szmsg2 = _T("Problem recreating TraceWriter. Turning Trace off!");
				writeEventLog(&szmsg2);
            szmsg= e2.Description();
            writeEventLog(&szmsg);
			}
		}
	}
//	LeaveCriticalSection(&CritSecTraceCreate);
	return m_bCreated;
}

void CTraceSystem::SetModuleID(ULONG moduleId)
{
	m_moduleId= moduleId;
}

// Trace: Mask, source and line info.
long CTraceSystem::Trace(ULONG traceMask,  LPCTSTR szsource, const int iLineNum, LPCTSTR sFormat, ...)
{
	CString svComp(_T(""));
	if(iLineNum && szsource && szsource[0])
		svComp.Format(_T("%s@%d:"), szsource, iLineNum);

	va_list argptr;
	va_start( argptr, sFormat);
	long lResult = internalTrace(traceMask, svComp, sFormat, argptr);
	va_end( argptr);
	return lResult;
}

// the one that all others call.
long CTraceSystem::internalTrace(ULONG traceMask,  LPCTSTR szsourceAndLine, LPCTSTR sFormat, va_list aList)
{
   ATLASSERT(m_threadId == GetCurrentThreadId());   
	if (m_threadId != GetCurrentThreadId())
	{
		LPCTSTR szmsg = _T("Error. TraceSystem obj used from a different thread");
		writeEventLog(&szmsg);
	}
   if ((!m_bCreated) && (!m_bStop)) {
      Create();
	}
	
	if (0xFF00 & m_ncurMask) // if the module mask is reported to us, check it
		if (!(m_moduleId & m_ncurMask)) return 0;

   CString csPrefix(_T("[?]"));
//	ULONG nFinalMask= traceMask & m_ncurMask & 0x00FF;
	ULONG nFinalMask= traceMask & 0x00FF;
   switch (nFinalMask) {
      case 0:  // mask filtered out
         return 0;
      case TRACE_MASK_ENTRY:
      case TRACE_MASK_IN:
         csPrefix=_T("[+]");
         break;
      case TRACE_MASK_OUT:
			m_nIndent= max(0, m_nIndent--);
         csPrefix=_T("[-]");
         break;
      case TRACE_MASK_ERROR:
         csPrefix=_T("[E]");
         break;
      case TRACE_MASK_DEBUG:
         csPrefix=_T("[D]");
         break;
      case TRACE_MASK_INFO:
         csPrefix=_T("[I]");
         break;
      case TRACE_MASK_WARNING:
         csPrefix=_T("[W]");
         break;
		default:
			{
				if (nFinalMask & TRACE_MASK_IN)
					csPrefix=_T("[+]");
				else if (nFinalMask & TRACE_MASK_OUT) {
					m_nIndent= max(0, m_nIndent--);
					csPrefix=_T("[-]");
				}
			}
   }
	CString csIndent(' ', m_nIndent * 2);

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	CString csBuff;
	csBuff.FormatV( sFormat, aList);
	
   CString strTemp;
/*	strTemp.Format(_T("%02d/%02d %02d:%02d:%02d %13s %04X>  %s%s%s%s"), 
		sysTime.wMonth, sysTime.wDay, 
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond, commaprint(GetTickCount()),
      GetCurrentThreadId(),
      csIndent,
      csPrefix,
      szsourceAndLine,
      csBuff);
*/	
   strTemp.Format(_T("%02d/%02d %02d:%02d:%02d %13u %04X>  %s%s%s%s"), 
		sysTime.wMonth, sysTime.wDay, 
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond, GetTickCount(),
      GetCurrentThreadId(),
      csIndent,
      csPrefix,
      szsourceAndLine,
      csBuff);

	//strTemp.Replace(_T("%"), _T("%%"));
	if (nFinalMask & TRACE_MASK_IN)
		m_nIndent++;


	return Trace(m_moduleId | traceMask, (LPCTSTR) strTemp);
}

long CTraceSystem::Trace(ULONG traceMask, LPCTSTR strTemp)
{
	try{
      if (m_bStop) {
//         LogEmergencyMessage((LPCTSTR) strTemp);
      }
      else
   		ptw->Trace( m_moduleId | traceMask, (LPCTSTR) strTemp);
	}
	catch(_com_error e){
		LPCTSTR szmsg = _T("TraceWriter.Trace exception. Starting new instance.");
		writeEventLog(&szmsg);
      szmsg= e.Description();
      writeEventLog(&szmsg);
		try {
//			EnterCriticalSection(&CritSecTraceCreate);
			// assume not created to attempt a recreate.
			// do not change in the middle of a recreation process.
			Sleep(1000);
         m_bCreated=false;	
//			LeaveCriticalSection(&CritSecTraceCreate);
			if (Create())
				ptw->Trace(m_moduleId | traceMask, (LPCTSTR) strTemp);
		}
		catch(_com_error e2){
			// if still a problem, shut this off
			m_bCreated=false; 
			m_bStop=true;
			LPCTSTR szmsg2 = _T("Problem recreating/using TraceWriter. Turning TraceWriter off!");
			writeEventLog(&szmsg2);
         szmsg= e2.Description();
         writeEventLog(&szmsg);
//         LogEmergencyMessage((LPCTSTR) strTemp);
		}
	}
	return 0;
}


// HexTrace: Mask, 
long CTraceSystem::HexTrace(ULONG traceMask, LPCTSTR szsource, const int iLineNum, 
							LPCTSTR szTitle, UCHAR *phexData, ULONG hexDataLen /*=0*/)
{
	CString strTemp;
	strTemp.Format(_T("%s@%d:%s"), szsource, iLineNum, szTitle);

	return HexTrace(traceMask, (LPCTSTR) strTemp, phexData, hexDataLen);
}

// HexTrace: Mask, 
long CTraceSystem::HexTrace(ULONG traceMask, LPCTSTR szTitle, UCHAR *phexData, ULONG hexDataLen /*=0*/)
{
	CString csTitle(_T(""));

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

/*	csTitle.Format(_T("%02d/%02d %02d:%02d:%02d %13s %04X> %s"), 
		sysTime.wMonth, sysTime.wDay, 
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond, commaprint(GetTickCount()),
      GetCurrentThreadId(),
      szTitle);
*/
	csTitle.Format(_T("%02d/%02d %02d:%02d:%02d %13u %04X> %s"), 
		sysTime.wMonth, sysTime.wDay, 
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond, GetTickCount(),
      GetCurrentThreadId(),
      szTitle);

	if (!m_bCreated) {
	   if (m_bStop) return 0;  // no Emergency log file for hex trace
	   if (!Create()) return 0;
	}
	if (!(traceMask & m_ncurMask & 0x00FF)) return 0;
	if (0xFF00 & m_ncurMask) // if the module mask is reported to us, check it
		if (!(m_moduleId & m_ncurMask)) return 0;

	if (hexDataLen==0) hexDataLen= (ULONG) _tcslen((LPCTSTR) phexData);

	m_VarSA.SetBuffer(phexData, hexDataLen);

	try{
		ptw->HexTraceV(m_moduleId| traceMask, (LPCTSTR)csTitle, m_VarSA.GetVariant());
	}
	catch(_com_error e){
		LPCTSTR szmsg = _T("TraceWriter.Trace exception. Possible atl srvr crash. Starting new instance.");
		writeEventLog(&szmsg);
      szmsg= e.Description();
      writeEventLog(&szmsg);
		try {
//			EnterCriticalSection(&CritSecTraceCreate);
			// assume not created to attempt a recreate.
			// do not change in the middle of a recreation process.
			m_bCreated=false;	
//			LeaveCriticalSection(&CritSecTraceCreate);
			if (Create())
				ptw->HexTraceV(m_moduleId |traceMask, (LPCTSTR)csTitle, m_VarSA.GetVariant());
		}
		catch(_com_error e2){
			// if still a problem, shut this off
			m_bCreated=false; 
			m_bStop=true;
			LPCTSTR szmsg2 = _T("Problem recreating/using TraceWriter. Turning Trace off!");
			writeEventLog(&szmsg2);
         szmsg= e2.Description();
         writeEventLog(&szmsg);
		}
	}

	return S_OK;
}

void CTraceSystem::LogEmergencyMessage(LPCTSTR szMessage)
{

}

CTraceEntry::CTraceEntry (ULONG traceMask, LPCTSTR szsource, const int iLineNum, LPCTSTR szEntry)
: m_traceMask(traceMask),
  m_csSource(szsource),
  m_nLine(iLineNum),
  m_csText(szEntry)
{
	tbtrace(m_traceMask | TRACE_MASK_IN, m_csSource, m_nLine, m_csText);
}

CTraceEntry::~CTraceEntry()
{
	tbtrace(m_traceMask | TRACE_MASK_OUT, m_csSource, m_nLine, m_csText);
}

// We had to isolate the COM client to a separate thread so it would not pump
// message on FastLane's main thread - causing WinProc re-entry.
DWORD WINAPI COMClientThreadFunction(LPVOID ptr)
{

	CStringsQueue *pmq = ((tdata *) ptr)->m_pmq;
   CString csTracename = ((tdata *) ptr)->m_csTraceName;
	qData *pqdata=NULL;

	CoInitialize(NULL);
	CTraceSystem ts(csTracename);
	ts.Create();
    bool bProcessing = true;
    DWORD dwWait;
    HANDLE MyHandles[2];
    MyHandles[0] = HandleVector[2]; // Terminate Event
    MyHandles[1] = HandleVector[0]; // Data Event
	while (bProcessing) {
		dwWait = WaitForMultipleObjects(2, MyHandles,FALSE, INFINITE);
        switch( dwWait )
        {
        case WAIT_OBJECT_0:
            bProcessing = false;
            break;

        case WAIT_OBJECT_0 + 1:
            dwWait = WaitForSingleObject( HandleVector[1], INFINITE );
		    try {
			    pqdata = pmq->front(); //get the first message
			    pmq->pop();
			    if (pmq->empty())
				    ResetEvent(HandleVector[0]);
		    }
		    catch (...) 
		    {
			    LPCTSTR szmsg = _T("Error Popping Msg");
			    writeEventLog(&szmsg);
                pqdata = NULL;
		    }
		    ReleaseMutex(HandleVector[1]); 

            if (NULL != pqdata)
            {
		        try {
			        if (pqdata->m_pData) // hex
				        ts.HexTrace(pqdata->m_traceMask, pqdata->m_csTrace, pqdata->m_pData, pqdata->m_nDataLen);
			        else
				        ts.Trace(pqdata->m_traceMask, pqdata->m_csTrace);
			        delete pqdata;
		        }
		        catch( CString csBuff)
		        {
			        CString cs(csBuff + pqdata->m_csTrace);
			        LPCTSTR szmsg= (LPCTSTR) cs;
			        writeEventLog(&szmsg);
			        delete pqdata;
		        }
		        catch(...) {
			        CString cs(CString(_T("Exception:")) + pqdata->m_csTrace);
			        LPCTSTR szmsg=(LPCTSTR) cs;
			        writeEventLog(&szmsg);
			        delete pqdata;
		        }
            }
            break;

        default:
            break;
        }
	}
    ts.Release();
	CoUninitialize();
	return 0;	  
}
