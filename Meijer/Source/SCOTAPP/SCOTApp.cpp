//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SCOTAPP.cpp
//
// TITLE:   Defines the class behaviors for the application.
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// CHANGES: Start
// CHANGES: End
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Common.h"			// includes common to most
#include "SCOTApp.h"
#include "SCOTAPPDlg.h"
#include "LimitSingleInstance.h"
#include "AfxOle.h" //tar 254586

DLLEXPORT extern HWND hw; //RFC263887
CList<CDeferredMessage, CDeferredMessage&> s_qDeferredMessages; //RFC263887

// The one and only CLimitSingleInstance object
// Used to limit scotapp to one instance without relying on mfc
CLimitSingleInstance g_SingleInstanceObj(TEXT("{20B03650-1A84-11d6-876B-00E029249437}"));

#define COMP_ID ID_MP
#define T_ID _T("SCOTAppApp")


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSCOTAPPApp

BEGIN_MESSAGE_MAP(CSCOTAPPApp, CWinApp)
//{{AFX_MSG_MAP(CSCOTAPPApp)
// NOTE - the ClassWizard will add and remove mapping macros here.
//  DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG
ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

//+TAR205142  Addresses the issue of unhandled exceptions.
// RFQ 7477
/* Save this for a later build. This needs to be tested heavily. */
#if 0
/* BEGIN PMP - Override system exception catch in C++ try/catch blocks */

extern "C" void die_on_exception(unsigned int, EXCEPTION_POINTERS*)
{
    throw;
}

extern "C" void (*old_translator)(unsigned, EXCEPTION_POINTERS*) = _set_se_translator(die_on_exception);

/* END PMP - Override system exception catch in C++ try/catch blocks */
#endif
// RFQ 7477
static LONG WINAPI FLUnhandledExceptionFilter(struct _EXCEPTION_POINTERS *lpExceptionInfo);
static LONG (WINAPI *lpfnCRTUnhandledExceptionFilter)(struct _EXCEPTION_POINTERS *lpExceptionInfo) = 0;

LONG WINAPI FLUnhandledExceptionFilter(struct _EXCEPTION_POINTERS *lpExceptionInfo)
{
	to.Trace(L1, _T("+FLUnhandledExceptionFilter()"));   //tar 273441
	
	long lresult = EXCEPTION_EXECUTE_HANDLER;
	
    LPVOID lpMsgBuf;
	DWORD dwChars = 0;
    dwChars = ::FormatMessage( 
							FORMAT_MESSAGE_ALLOCATE_BUFFER | 
							FORMAT_MESSAGE_FROM_SYSTEM,
							NULL,
							lpExceptionInfo->ExceptionRecord->ExceptionCode,
							MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
							(LPTSTR) &lpMsgBuf,
							sizeof(lpMsgBuf),//0,
							NULL);

	if (dwChars > 0)
	{	
		//	MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );	
		to.Trace(L1, _T("FLUnhandledExceptionFilter error is <%s>"), lpMsgBuf);  //tar 273441
		
		if (lpMsgBuf > 0) LocalFree( lpMsgBuf );
	}
	else
	{
		to.Trace(L1, _T("FLUnhandledExceptionFilter couldn't translate error <%08x>"),	lpExceptionInfo->ExceptionRecord->ExceptionCode); //tar 273441
	}
// RFQ 7477
    /* BEGIN SOTF8951 - Add more descriptive exception information to trace */
	to.Trace(L1, L"Exception %08X at instruction %08X", 
        lpExceptionInfo->ExceptionRecord->ExceptionCode, lpExceptionInfo->ExceptionRecord->ExceptionAddress);

	PCONTEXT pCtx = lpExceptionInfo->ContextRecord;

	if (pCtx->ContextFlags & CONTEXT_INTEGER)
	{
		to.Trace(L1, L"EAX: %08X", pCtx->Eax);
		to.Trace(L1, L"EBX: %08X", pCtx->Ebx);
		to.Trace(L1, L"ECX: %08X", pCtx->Ecx);
		to.Trace(L1, L"EDX: %08X", pCtx->Edx);
		to.Trace(L1, L"ESI: %08X", pCtx->Esi);
		to.Trace(L1, L"EDI: %08X", pCtx->Edi);
    }

	if (pCtx->ContextFlags & CONTEXT_CONTROL)
	{
		to.Trace(L1, L"EIP: %08X", pCtx->Eip);
		to.Trace(L1, L"ESP: %08X", pCtx->Esp);
		to.Trace(L1, L"EBP: %08X", pCtx->Ebp);
		to.Trace(L1, L"EFL: %08X", pCtx->EFlags);
		to.Trace(L1, L" SS: %08X", pCtx->SegSs);
		to.Trace(L1, L" CS: %08X", pCtx->SegCs);
	}

	if (pCtx->ContextFlags & CONTEXT_SEGMENTS)
	{
		to.Trace(L1, L" DS: %08X", pCtx->SegDs);
		to.Trace(L1, L" ES: %08X", pCtx->SegEs);
		to.Trace(L1, L" FS: %08X", pCtx->SegFs);
		to.Trace(L1, L" GS: %08X", pCtx->SegGs);
	}

	DWORD end = 224;

	if (pCtx->ContextFlags & CONTEXT_CONTROL && pCtx->Ebp && pCtx->Esp)
	{
		DWORD ebp = pCtx->Ebp;
        DWORD esp = pCtx->Esp;

		DWORD tmp = ebp;

		while (tmp)
		{
			tmp = *((PDWORD)(tmp));

			if (tmp)
			{
				end = (tmp - ebp) + 4;
			}
		}

		end += 32;
        end += (pCtx->Esp - pCtx->Ebp);

		DWORD offset = 0;

		while (offset < end)
		{
            DWORD start = esp + offset;

			DWORD d1 = *((PDWORD)(esp + offset));
			BYTE* c1 = (BYTE*)(&d1);
			offset += 4;

			DWORD d2 = *((PDWORD)(esp + offset));
			BYTE* c2 = (BYTE*)(&d2);
			offset += 4;

			DWORD d3 = *((PDWORD)(esp + offset));
			BYTE* c3 = (BYTE*)(&d3);
			offset += 4;

			DWORD d4 = *((PDWORD)(esp + offset));
			BYTE* c4 = (BYTE*)(&d4);
			offset += 4;

			to.Trace(L1, L"%08x: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X", 
                start, c1[0], c1[1], c1[2], c1[3], c2[0], c2[1], c2[2], c2[3], c3[0], c3[1], c3[2], c3[3], c4[0], c4[1], c4[2], c4[3]);
		
		}
	}
    /* END SOTF8951 - Add more descriptive exception information to trace */
// RFQ 7477
	to.FlushAllObjects();
	
	//+THD Performance RFC
	to.Trace(L6, _T("Exception flush completed normally"));
	to.TraceFileFlush();
	//-THD Performance RFC
	// If we were able to record the address of the CRT exception filter, chain it
	// and let it do its default action.
	if (lpfnCRTUnhandledExceptionFilter)
	{	
		lresult = lpfnCRTUnhandledExceptionFilter(lpExceptionInfo);
	}
	return lresult;
}
//-TAR205142  Addresses the issue of unhandled exceptions.

/////////////////////////////////////////////////////////////////////////////
// CSCOTAPPApp construction

CSCOTAPPApp::CSCOTAPPApp()
: hDdraw(NULL)
{
	// Place all significant initialization in InitInstance
	m_SCOTWndName = _T("SCOTInputWindow");
	
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSCOTAPPApp object

CSCOTAPPApp theApp;


#ifdef _DEBUG

// The following code provides a way for us to use the MFC assert 
// functionality without pumping messages from withing the messagebox.
// Otherwise, events can be fired into the application while MFC is
// waiting for the developer to press abort, retry or fail.

static BOOL SCOTAssertThread(void *p);
static BOOL SCOTAssert(LPCTSTR lpszAssertDesc);
static _CRT_REPORT_HOOK PreviousCrtReportHook = NULL;

// Actual messagebox code that runs on the new thread.

BOOL SCOTAssertThread(void *p)
{
	CString sMessage;
	sMessage.Format(_T("The following assertion was encountered:\n%s\n")
		_T("Select _T('Abort') to exit, _T('Retry') to debug or _T('Ignore') to continue."),p);
	int iResult = ::MessageBox(NULL, (LPCTSTR)sMessage, 
		_T("NCR SCOT Application - Assertion Failed:"), MB_SETFOREGROUND | MB_ICONERROR | 
		MB_DEFBUTTON3 | MB_ABORTRETRYIGNORE | MB_TASKMODAL);
	if (iResult == IDABORT)
		ExitProcess(0);            // Abort -- just punt.
	else if (iResult == IDRETRY)
		return TRUE;
	return FALSE;
}

// This function starts up a new thread into SCOTAssertThread() 
// and blocks on the thread.  When the user terminates the message
// box, the return value is returned to the hook.

BOOL SCOTAssert(LPCTSTR lpszAssertDesc)
{
	DWORD dwStatus = 0;
	CWinThread *pThread = AfxBeginThread(
		(AFX_THREADPROC) SCOTAssertThread, (void*)lpszAssertDesc, 
		THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	// TK - Tar 200278 - Dr. Watson & trace.log do not show what caused the violation.
	// Added tracing to get the thread id of all threads that are started by the app.
	trace(L6,_T("ScotAssert thread created. Thread Id = (0x%x)"),pThread->m_nThreadID);
	
	if (pThread)
	{
		pThread->m_bAutoDelete = FALSE;
		pThread->ResumeThread();
		WaitForSingleObject(pThread->m_hThread, INFINITE);
		GetExitCodeThread(pThread->m_hThread, &dwStatus);
		delete pThread;
	}
	return dwStatus;
}

// This hook handles assertions via SCOTAssert() above.
// All other report types are passed to the previous handler (MFC)
// or deferred to the C Runtime Debug Library.

int SCOTReportHook( int reportType, LPTSTR message, int *returnValue )
{
	
	if (reportType == _CRT_ASSERT)
	{
		*returnValue = SCOTAssert(message);
		return TRUE;
	}
	// This hooks all report occurences (TRACE, etc) so we must call the
	// previous report hook for other report types, which we don't handle
	else if(PreviousCrtReportHook)
		return PreviousCrtReportHook(reportType, (LPSTR) message, returnValue);
	else
		return FALSE;
}

#endif

BOOL CSCOTAPPApp::InitInstance()
{
	srand( (unsigned)time( NULL ) ); //rfc375570
	
#ifdef _DEBUG
	PreviousCrtReportHook = 
		_CrtSetReportHook((_CRT_REPORT_HOOK )SCOTReportHook);
#endif
	
	AfxEnableControlContainer();
	OleInitialize(NULL);
	
	if (g_SingleInstanceObj.IsAnotherInstanceRunning())
	{
		eo.Initialize(NULL);	// trace :NULL; uninitialize on exit instance
		ScotError(RETURN,SM_CAT_STOP_ABORT, SCOTAPP_PREV_INSTANCE);
		return false;                             
	}
	
	eo.Initialize(&to);	// pass trace object address to trace errors in a file also
	appBusy.Initialize();
    appBusy.SetBusyStatus(2);  //Make sure RSM busy processes are started

	//+TAR205142  Addresses a bigger issue of unhandled exceptions.
	//This is what we did in RAP.
	
	// Save the CRT Unhandled exception filter so we can still get a housecall from the
	// doctor after we flush our trace file, if we wish.
	lpfnCRTUnhandledExceptionFilter = SetUnhandledExceptionFilter(FLUnhandledExceptionFilter);
	//-TAR205142
	
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need.
	
#ifdef _AFXDLL
	Enable3dControls();               // Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();         // Call this when linking to MFC statically
#endif

   // TAR 282398 start
   hDdraw = LoadLibrary(_T("Ddraw.dll"));
   if (NULL == hDdraw)
   {
      trace(L0, _T("Load of Ddraw.dll failed (0x%x)"), GetLastError());
   }
   // TAR 282398 end

	if(AfxOleInit( )) //tar 254586
	{
		COleMessageFilter* pFilter = AfxOleGetMessageFilter();
	if(pFilter)
	{
		pFilter->EnableBusyDialog(FALSE);
		pFilter->SetMessagePendingDelay(15000);//stop retrying after 15000 ms
		pFilter->SetRetryReply(250);//retry immediately
	}
	else
	{
		trace(L0, _T("AfxOleGetMessageFilter Failed!"));
	}
    }
    else
    {
        trace(L0, _T("AfxOleInit Failed!"));
    }
	
	CCommandLineInfo cif;
	ParseCommandLine(cif);
	
	CSCOTAPPDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse;
	// TK - Tar 200278 - Dr. Watson & trace.log do not show what caused the violation.
	// Putting try & catch block here to flush all objects in to the traces file and
	// re-throw the exception. This can help us to find the root cause of the 
	// memory access violation.
	
	//+TAR205142  Don't need, nor want the try/catch blocks that were here, with the unhandledexceptionhandler active
	//The new unhandledexceptionhandler also addresses the Tar in 20078
	nResponse = dlg.DoModal();
	trace(L0, _T("CSOTAPPApp::InitInstance() Main dialog exited from DoModal()"));
	//	to.FlushAllObjects(); //problem is, stream no longer exists.....
	//-TAR205142

	if (nResponse == IDOK)
	{
		// dismissed with OK	
	}
	else if (nResponse == IDCANCEL)
	{
		// dismissed with Cancel
	}
	
	// Since the dialog has been closed, return false so that we exit the
	// application, rather than start the application's message pump.
	return false;
}

int CSCOTAPPApp::ExitInstance()
{
	//+TAR205142 Restore original exception handler..
	//static LONG (WINAPI *tempptr)(struct _EXCEPTION_POINTERS *lpExceptionInfo);
	trace(L0, _T("CSOTAPPApp::ExitInstance() Main dialog exiting"));

   // TAR 282398 start
   if (NULL != hDdraw)
   {
      FreeLibrary(hDdraw);
      hDdraw = NULL;
   }
   // TAR 282398 end

	Sleep( 5000 );	// Wait to let exiting threads use error objects until they terminate
	SetUnhandledExceptionFilter(lpfnCRTUnhandledExceptionFilter);
	
	to.UnInitialize();
	eo.Uninitialize();
	OleUninitialize();
	misMatchTrace.UnInitialize();	//NN and BZ RFC 210177
	
#ifndef _UNICODE
#ifdef _DEBUG
	VERIFY( SCOTReportHook == _CrtSetReportHook((_CRT_REPORT_HOOK )PreviousCrtReportHook ));
#endif
#endif
	
	return CWinApp::ExitInstance();
}

BOOL CSCOTAPPApp::ProcessShellCommand(CCommandLineInfo &cif)
{
	return true;
}

//TAR263887+
BOOL CSCOTAPPApp::PreTranslateMessage( MSG* pMsg )
{
	MSG message;

	while (s_qDeferredMessages.GetCount())
	{
		memset((char *)&message, 0, sizeof(MSG)); //start with a clean structure
        CDeferredMessage Msg = s_qDeferredMessages.RemoveHead();
        trace(L0, _T("CSCOTAPPApp::PreTranslateMessage - pull message from deferred list: (message=%08.8x, wParam=%08.8x, lParam=%08.8x)"), Msg.message, Msg.wParam, Msg.lParam);
  	    message.hwnd = hw;
		message.lParam = Msg.lParam;
		message.wParam = Msg.wParam;
		message.message = Msg.message;

		::TranslateMessage(&message);
		::DispatchMessage(&message);
	}

   
	return CWinApp::PreTranslateMessage(pMsg);
}
//RFC263887-

