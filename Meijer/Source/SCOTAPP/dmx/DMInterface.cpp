//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  FILE:    DMInterface.cpp
//
//  TITLE:   Device Manager (NCR API) Interface
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DMInterface.h"
#include "NcrDevMgr.h"
#include "DmxOposIf.h"
#include "..\devmgr.h"
#include "..\Common.h"

#include "DCAPMethodTrace.h"         // RFC 445940:  Add diagnostic tracing

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COMP_ID ID_DM							  // used by the trace() calls to identify this component
#define T_ID    _T("DMInterface")
//+TAR#329381
static int g_DevDelay = 100;
static bool g_bReadReg = false;
//-TAR#329381
//////////////////////////
// DMInterface

DMInterface::DMInterface( 
   CDeviceManagerEx& rDMX,
   CmDataCapture&    cmDc )
   :  m_uiThread( cmDc ),
   m_rDMX( rDMX ),
   m_cmDc( cmDc )
   {
	HKEY hKey = NULL;
	bool bResult = false;

    // RFC 445940:  Add diagnostic tracing
    DCAP_API_TRACE(_T("DMInterface::DMInterface"));

	//+TAR#329381
    if  ( g_bReadReg )
    {
        return;
    }

	long rc = RegOpenKey( HKEY_LOCAL_MACHINE, _T( "SOFTWARE\\NCR\\SCOT\\CurrentVersion\\DMConfig" ), &hKey );

	if	( rc == ERROR_SUCCESS )
	{
		  DWORD dwType;
		  DWORD dwLength;
		  CString s;

          g_bReadReg = true;

		  // Look for value; get data size.
		  long nRC = RegQueryValueEx(
									  hKey,                   // handle of key to query
									  _T("DevDelay"),	  // address of name of value to query
									  NULL,                   // reserved
									  &dwType,                // address of buffer for value type
									  NULL,                   // address of data buffer: just return size.
									  &dwLength );            // address of data buffer size

		  // If success and type is string, get the data.
		  if ( nRC == ERROR_SUCCESS && dwType == REG_SZ )
		  {
			LPTSTR p = s.GetBuffer( dwLength );
			RegQueryValueEx(
							hKey,					// handle of key to query
							_T("DevDelay"),         // address of name of value to query
							NULL,					// reserved
							&dwType,				// address of buffer for value type
							(PBYTE) p,				// address of data buffer
							&dwLength );			// address of data buffer size
			s.ReleaseBuffer();

            int iDevDelay = _ttoi( ( LPCTSTR ) s );

            if  ( iDevDelay < 0 ) 
                iDevDelay = 0;

            if  ( iDevDelay > 1000 )
                iDevDelay = 1000;

            g_DevDelay = iDevDelay;
          }

        RegCloseKey( hKey );
   }
   //-TAR#329381
}

///////////////////////////
DMInterface::~DMInterface()
   {
    // RFC 445940:  Add diagnostic tracing
   DCAP_API_TRACE(_T("DMInterface::~DMInterface"));
   UnInitialize( );
   }


BOOL DMInterface::Initialize( 
   BOOL bNewThread /* = TRUE */ )
   {
    // RFC 445940:  Add diagnostic tracing
   DCAP_API_TRACE(_T("DMInterface::Initialize"));

   m_cmDc.DCPrintf(DCTRACE_NORMAL, _T("bNewThread = %s"), 
                   bNewThread ? _T("TRUE") : _T("FALSE"));

   BOOL bResult = FALSE;
   if ( bNewThread != FALSE )
      bResult = m_uiThread.Start( this, this );
   else
      bResult = CreateEx(
         0,                                  // Extended style.
         AfxRegisterWndClass( 0 ),           // Class name.
         _T("DMXServiceWindow"),             // Window name.
         0,                                  // Window style,
         0, 0,                               // Initial x,y.
         0, 0,                               // Width, Height.
         NULL,                               // Parent.
         0,                                  // Menu ID.
         0 );                                // Params.
   return bResult;
   }

void DMInterface::UnInitialize( 
   void )
   {
   // RFC 445940:  Add diagnostic tracing
   DCAP_API_TRACE(_T("DMInterface::UnInitialize"));
   m_uiThread.Stop( );

   m_cmDc.DCPrintf(DCTRACE_NORMAL, _T("m_hWnd:  %#X"), m_hWnd);
   if ( m_hWnd != NULL )
      DestroyWindow();
   return;
   }



   
////////////////////////////////////
// DMInterface message handlers
BEGIN_MESSAGE_MAP(DMInterface, CWnd)
//{{AFX_MSG_MAP(DMInterface)
   ON_WM_TIMER()
//}}AFX_MSG_MAP
   IMPLEMENT_MARSHAL_MESSAGES( )
   ON_MESSAGE( WMU_CREATE_DEVICE, OnCreateDevice )
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////////////////////
// DMInterface message handlers

//BOOL DMInterface::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
//{
//  devmgr.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo); // pass it to the DM wrapper class
//  return CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
//}


//////////////////////////////////////
BEGIN_EVENTSINK_MAP(DMInterface, CWnd)
//{{AFX_EVENTSINK_MAP(DMInterface)
//}}AFX_EVENTSINK_MAP
ON_EVENT_RANGE(DMInterface, BASE_DEVID(DMCLASS_PRINTER), MAX_DEVID(DMCLASS_CARDDISPENSER ), 1, OnOposDataEvent, VTS_I4 VTS_I4)
ON_EVENT_RANGE(DMInterface, BASE_DEVID(DMCLASS_PRINTER), MAX_DEVID(DMCLASS_CARDDISPENSER ), 2, OnOposDirectIOEvent, VTS_I4 VTS_I4 VTS_PI4 VTS_PBSTR)
ON_EVENT_RANGE(DMInterface, BASE_DEVID(DMCLASS_PRINTER), MAX_DEVID(DMCLASS_CARDDISPENSER ), 3, OnOposErrorEvent, VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_PI4)
ON_EVENT_RANGE(DMInterface, BASE_DEVID(DMCLASS_PRINTER), MAX_DEVID(DMCLASS_CARDDISPENSER ), 4, OnOposOutputCompleteEvent, VTS_I4 VTS_I4)
ON_EVENT_RANGE(DMInterface, BASE_DEVID(DMCLASS_PRINTER), MAX_DEVID(DMCLASS_CARDDISPENSER ), 5, OnOposStatusUpdateEvent,   VTS_I4 VTS_I4)
END_EVENTSINK_MAP()


IMPLEMENT_MARSHAL_CONTROL( DMInterface )

LONG DMInterface::OnCreateDevice( 
   UINT nCtrlId, LONG pDevice )
   {
   DCAP_API_TRACE(_T("DMInterface::OnCreateDevice"));  // RFC 445940
	//tar 315510
	LONG nRC; 
	try{
		trace(L6, _T("DMInterface::OnCreateDevice():nCtrlId=%d pDevice=%d"),
              nCtrlId, pDevice );
		//+TAR#329381
        if  ( g_DevDelay > 0 )
            Sleep(g_DevDelay);
		//-TAR#329381
		nRC= ((CGenericOposIF*)pDevice)-> Create( this, nCtrlId );	
	}
	catch(...)
	{ 
		TRACE(_T("Unexpected exception caught in DMInterface::OnCreateDevice()\n"));
		trace(L6, _T("Unexpected exception caught in DMInterface::OnCreateDevice()\n"));
	}
	return nRC;
} 
 

 
///////////////////////////////////////////////////////////////////////////////////////////////////
// CThreadWndIF virtuals

/* virtual */
BOOL DMInterface::InitWindow( 
   void )
   {
   DCAP_API_TRACE(_T("DMInterface::InitWindow"));   // RFC 445940

   HRESULT hResult = CoInitialize( NULL );
   BOOL bResult = FALSE;

   m_cmDc.DCPrintf(DCTRACE_NORMAL, _T("CoInitialize returned: %d"), hResult);
   if ( SUCCEEDED( hResult ) != FALSE )
      bResult = CreateEx(
      0,                                  // Extended style.
      AfxRegisterWndClass( 0 ),           // Class name.
      _T("DMXServiceWindow"),             // Window name.
      0,                                  // Window style,
      0, 0,                               // Initial x,y.
      0, 0,                               // Width, Height.
      NULL,                               // Parent.
      0,                                  // Menu ID.
      0 );                                // Params.
   return bResult;
   }

/* virtual */
void DMInterface::ExitWindow( 
   void )
   {
   DCAP_API_TRACE(_T("DMInterface::ExitWindow"));   // RFC 445940

   m_cmDc.DCPrintf(DCTRACE_NORMAL, _T("m_hWnd = %#X"), m_hWnd);
   if ( m_hWnd != NULL )
      DestroyWindow();
   CoUninitialize();
   return;
   }



/* afx_msg */
BOOL DMInterface::OnOposErrorEvent( 
   UINT  nCtrlId, 
   long  nResult, 
   long  nRcEx, 
   long  nErrorLocus, 
   long FAR* pnErrorResponse )
   {
   DCAP_API_TRACE_EXT(_T("DMInterface::OnOposErrorEvent"));       // RFC 445940.
   return m_rDMX.OnOposErrorEvent( nCtrlId, nResult, nRcEx, nErrorLocus, pnErrorResponse );
   }


/* afx_msg */
BOOL DMInterface::OnOposDataEvent( 
   UINT nCtrlId, 
   long nStatus )
   {
   DCAP_API_TRACE_EXT(_T("DMInterface::OnOposDataEvent"));        // RFC 445940.
   return m_rDMX.OnOposDataEvent( nCtrlId, nStatus );
   }



/* afx_msg */
BOOL DMInterface::OnOposStatusUpdateEvent( 
   UINT  nCtrlId, 
   long  nStatus )
   {
   DCAP_API_TRACE_EXT(_T("DMInterface::OnOposStatusUpdateEvent"));// RFC 445940
   return m_rDMX.OnOposStatusUpdateEvent( nCtrlId, nStatus );
   }

// DMInterface::OnOposOutputCompleteEvent
//
// Purpose: Pass the event back to the printer (wrapper) that fired this event
//          The wrapper will then pass the event back to us with the correct DMalike 
//          parameters. 

/* afx_msg */
BOOL DMInterface::OnOposOutputCompleteEvent( 
   UINT  nCtrlId, 
   long  nOutputId )
   {
   DCAP_API_TRACE_EXT(_T("DMInterface::OnOposOutputCompleteEvent"));// RFC445940
   return m_rDMX.OnOposOutputCompleteEvent( nCtrlId, nOutputId );
   }

/* afx_msg */
BOOL DMInterface::OnOposDirectIOEvent( 
   UINT    nCtrlId, 
   long    nEventId, 
   long*   plData, 
   BSTR*   pbstrData )
   {
   DCAP_API_TRACE_EXT(_T("DMInterface::OnOposDirectIOEvent"));    // RFC 445940
   return m_rDMX.OnOposDirectIOEvent( nCtrlId, nEventId, plData, pbstrData );
   }

void DMInterface::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
   DCAP_API_TRACE_EXT(_T("DMInterface::OnTimer"));                // RFC 445940

   CGenericOposIF* pDevice = m_rDMX.GetDeviceByCtrlId( nIDEvent );
   pDevice->OnTimer( nIDEvent );
	CWnd::OnTimer(nIDEvent);
}


