// CashDrawer.cpp: implementation of the CCashDrawer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "CashDrawer.h"
#include "TBService.h"
#include "TBOposIO.h"
#include "Resource.h"
#include "SocketForTBDevice.h"
#include "cashdrawerMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCashDrawer::CCashDrawer(int nPort, int nId,  CmDataCapture& cmDc )
					   :CTBDevice(OPOS_CLASSKEY_CASH,nId,nPort, cmDc)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CCashDrawer::CCashDrawer") );
}

CCashDrawer::~CCashDrawer()
{
    CDataCapEntryExit DCE( m_cmDc, _T("CCashDrawer::~CCashDrawer") );
}

//
// Function:  Open
// Purpose:  Opens the CashDrawer device, loads configuration from the registry
// Inputs:  pService - pointer to the Service object controlling the device
//          oRegistry - pointer to the registry settings for the device
// Outputs:  OPOS_SUCCESS on success
// Assumptions:  The registry settings are located in the key created for the 
//               OPOS profile for the device. The following values can be defined
//                   CapStatus - (default TRUE) - Specifies whether the cash
//                   drawer can report its open status.
// Comments:  None
//  
long CCashDrawer::Open( CTBService* pService, COposRegistry& oRegistry )
{
	CDataCapEntryExit DCE( m_cmDc, _T("CCashDrawer::Open") );
	BOOL bCapStatus;
	long lResult;

	bCapStatus = oRegistry.ReadInt(_T("CapStatus"), 0, TRUE );
	m_lMaxCloseDelay = oRegistry.ReadInt(_T("MaxCloseDelay"), 0, 2000 );
	m_DrawerNumber = oRegistry.ReadInt(_T("DrawerNumber"), 0, 1 );

	m_lAutoCloseTime = oRegistry.ReadInt(_T("AutoCloseDrawerTime"), 0, INFINITE );

	pService->InitializeCashDrawer( bCapStatus );
	
	lResult = CTBDevice::Open( pService, oRegistry);

	return lResult;
}

//
// Function:  Close
// Purpose:  Closes the CashDrawer device and cleans up resources related
//           to the device.
// Inputs:  pService - pointer to the Service object controlling the device
// Outputs:  None
// Assumptions:  None
// Comments:  None
//
void CCashDrawer::Close( CTBService* pService )
{
	CDataCapEntryExit DCE( m_cmDc, _T("CCashDrawer::Close") );

	pService->CloseCashDrawerService();

	CTBDevice::Close( pService );
}

//
// Function:  OpenDrawer
// Purpose:  Implements OPOS call to open the cash drawer.
// Inputs:  None
// Outputs:  OPOS_SUCCESS on success
//           OPOS_E_FAILURE - failed to send data to TB
// Assumptions:
//    The Open is reported to the TB and recorded internal by the service
//    object.  The TB can send a close message to indicate that
//    the drawer is closed or the safety thread will close the drawer
//    automatically after a timeout.
// Comments:  None
//
long CCashDrawer::OpenDrawer( void )
{
	CDataCapEntryExit DCE( m_cmDc, _T("CCashDrawer::OpenDrawer") );

	// Only modify the open status if the device supports reporting the
	// the status
	BOOL bCapStatus;
	HANDLE hClosed;

	bCapStatus = m_pTBService->GetCashDrawerStatusAndEvent( &hClosed );

	if ( bCapStatus )
	{
		ResetEvent( hClosed );
		this->OnStatus( CASHDRAWER_OPEN );
		//
		// Start a safety thread in case the TB does not respond
		// 
		AfxBeginThread( CCashDrawer::DrawerCloseProc, (void*) this, 0,0,0,NULL );
	}

	// Notify the TB
	NotifyTB( CASHDRAWER_OPEN );

	// Always return OPOS_SUCCESS to keep the App happy
	return OPOS_SUCCESS;
}

//
// Function:  WaitForDrawerClose
// Purpose:  Waits until the cash drawer is closed.
// Inputs:  BeepTimeout, BeepFrequence, BeepDuration, BeepDelay - All of these
//          inputs are ignored since the goal of this SO is to isolate the 
//          Fastlane system from the Host application (POS).
// Outputs:  OPOS_SUCCESS - The drawer was properly closed.
//           OPOS_E_FAILURE - Failed to wait on drawer close.
// Assumptions:  The drawer close for this SO is accomplished by the TB sending
//               a close message to the SO or by the safety thread timing out.
// Comments:  If CapStatus is FALSE, this function returns immediately with
//            success as specified in the OPOS Application Guide.
//
long CCashDrawer::WaitForDrawerClose( long BeepTimeout, long BeepFrequency,
							long BeepDuration, long BeepDelay )
{
	CDataCapEntryExit DCE( m_cmDc, _T("CCashDrawer::WaitForDrawerClose") );
	BOOL bCapStatus;
	HANDLE hClosed;

	bCapStatus = m_pTBService->GetCashDrawerStatusAndEvent( &hClosed );

	if ( bCapStatus )
	{
		DWORD dwWait;

		dwWait = WaitForSingleObject( hClosed, INFINITE );

		if ( dwWait != WAIT_OBJECT_0 )
		{
			return OPOS_E_FAILURE;
		}
	}
	
	return OPOS_SUCCESS;
}

// 
// Function:  ProcessRawData
// Purpose:  Processes messages received from the TB.
// Inputs:  pData - pointer to the message contents.  The message format is 
//                  defined in TBOposIO.h.
//          dwByteLen - Length of the entire message.
// Outputs:  None
// Assumptions:  The TB can send a message to either close or open the drawer;
//               however, the primary purpose is to close the drawer.
// Comments:  None
//
void CCashDrawer::ProcessRawData( void* pData, DWORD dwByteLen )
{
	CDataCapEntryExit DCE( m_cmDc, _T("CCashDrawer::ProcessRawData") );
    HANDLE hClosed;
	DWORD dwState;
	
   if (CSocketForTBDevice::GetSpecVersion() > 1) {
      CCashDrawerMsg cashDrawer;
      cashDrawer.Parse((BYTE *)pData);
	   // Only modify the internal state if the device supports
	   // status reporting.
	   if ( m_pTBService->GetCashDrawerStatusAndEvent( &hClosed ) )
	   {
		   // CashDrawer supports status reporting
		   dwState = WaitForSingleObject( hClosed, 0 );
		   m_cmDc.DCPrintf( 0x01, _T("Cash Drawer Closed State is %d\n"), dwState );
         if (cashDrawer.Status() == CCashDrawerMsg::CD_CLOSED)
		   {
			   // The Drawer was closed by TB
			   if ( dwState != WAIT_OBJECT_0 )
			   {
				   SetEvent( hClosed );
				   this->OnStatus( CASHDRAWER_CLOSED );
			   }
         } else {
			 
		if ( dwState == WAIT_OBJECT_0 )
			   {
				   ResetEvent( hClosed);
				   this->OnStatus( CASHDRAWER_OPEN );
			   }
		   }
	   }
   }
   else
   {
	   OPOSIO_HDR* pHdr = (OPOSIO_HDR*) pData;
	   if ( pHdr->deviceID != 0xFF )
	   {
		   OPOSIO_CASHDRAWER* pMsg = (OPOSIO_CASHDRAWER*) pData;

		   if ( dwByteLen == CASHDRAWER_LENGTH )
		   {
			   // Only modify the internal state if the device supports
			   // status reporting.
			   if ( m_pTBService->GetCashDrawerStatusAndEvent( &hClosed ) )
			   {
				   // CashDrawer supports status reporting
		   
				   dwState = WaitForSingleObject( hClosed, 0 );
				   m_cmDc.DCPrintf( 0x01, _T("Cash Drawer Closed State is %d\n"), dwState );
				   
				   if ( pMsg->drawerStatus == CASHDRAWER_CLOSED )
				   {
					   // The Drawer was closed by TB
					   if ( dwState != WAIT_OBJECT_0 )
					   {
						   SetEvent( hClosed );
						   this->OnStatus( CASHDRAWER_CLOSED );
					   }
				   }
				   else
				   {
					   // The Drawer was opened by TB
					   if ( dwState == WAIT_OBJECT_0 )
					   {
						   ResetEvent( hClosed);
						   this->OnStatus( CASHDRAWER_OPEN );
					   }
				   }
			   }
		   }
	   }
	   else
	   {
		   // The TB has sent a POLL message
		   m_pTBService->GetCashDrawerStatusAndEvent( &hClosed );
		 
		   DWORD dwWait;

		   dwWait = WaitForSingleObject( hClosed, 0 );

		   switch( dwWait )
		   {
		   case WAIT_OBJECT_0:
		   default:
			   // The Cash Drawer is Closed
			   this->NotifyTB( CASHDRAWER_CLOSED );
			   break;

		   case WAIT_TIMEOUT:
			   // The Cash Drawer is Open
			   this->NotifyTB( CASHDRAWER_OPEN );
			   break;
		   }
	   }
   }
}


//
// Function:  DrawerCloseProc
// Purpose:  Safety net to ensure the drawer is closed whether or not the TB
//           responds to the drawer open message
// Inputs:  lpParam - pointer to the CCashDrawer class that started the thread
// Outputs:  Always 0
// Assumptions:  The safety thread is only started if the device supports
//               status reporting.  If the TB closes the drawer the thread
//               does nothing
// Comments:  None
// 
UINT CCashDrawer::DrawerCloseProc( void* lpParam )
{
	CCashDrawer* pThis = (CCashDrawer*) lpParam;
	CDataCapEntryExit DCE( pThis->m_cmDc, _T("CCashDrawer::DrawerCloseProc") );

	HANDLE hClosed;

	pThis->m_pTBService->GetCashDrawerStatusAndEvent( &hClosed );

	DWORD dwWait;

	pThis->m_cmDc.DCPrintf( 0x01, _T("Waiting for the Drawer to Close\n"));
//	dwWait = WaitForSingleObject( hClosed, INFINITE); // will wait until TB sends DrawerClosed
	
	dwWait = WaitForSingleObject( hClosed, pThis->m_lAutoCloseTime);   //will wait for the specified time before closing the CahDrawer
	pThis->m_cmDc.DCPrintf( 0x01, _T("Wait returned %d\n"), dwWait );

	switch( dwWait )
	{
	case WAIT_OBJECT_0:
		// The TB closed the virtual drawer
		break;

	case WAIT_TIMEOUT:
	default:
		// TB has not responded, go ahead and close the virtual drawer
		SetEvent( hClosed );
		pThis->OnStatus( CASHDRAWER_CLOSED );
		pThis->NotifyTB( CASHDRAWER_CLOSED );
		break;
	}

	return 0;
}

long CCashDrawer::NotifyTB( BYTE DrawerState )
{
	// Notify the TB
	long				lResult = OPOS_E_FAILURE;
	int					iSent;
	BYTE *				pBuf;

   if (CSocketForTBDevice::GetSpecVersion() == 1) {
	   OPOSIO_CASHDRAWER_SOCKET Msg2;
	   unsigned int iLength = CASHDRAWER_LENGTH_SOCKET;

	   Msg2.Spec.messageID = TBMSG_CASH_DRAWER;
	   Msg2.Spec.nLen = iLength;
	   Msg2.Spec.nVersion = 1;
	   Msg2.drawerStatus = DrawerState;
	   Msg2.hdr.deviceID = TBMSG_CASH_DRAWER;
	   Msg2.hdr.port = m_nPort;
	   Msg2.hdr.unit = m_DrawerNumber;
	   Msg2.hdr.reserved = 0;
	   Msg2.reserved[0] = 0;
	   Msg2.reserved[1] = 0;
	   Msg2.reserved[2] = 0;

      pBuf = (BYTE *) &Msg2;

	   iSent = Send(pBuf, iLength);
	   if (iSent != SOCKET_ERROR)
	   {
		   lResult = OPOS_SUCCESS;
	   }
	   else
	   {
		   m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Failed to write to PipeServer Socket"));
	   }
   }
   else {
      CCashDrawerMsg cashDrawer;
      cashDrawer.Station((CCashDrawerMsg::CashDrawerStations) m_DrawerNumber);
      CCashDrawerMsg::CashDrawerStates curState;
      curState = ((DrawerState == CASHDRAWER_OPEN) ?
                        CCashDrawerMsg::CD_OPEN :
                        CCashDrawerMsg::CD_CLOSED);
      cashDrawer.Status(curState);

      int nLen;
      pBuf= cashDrawer.Serialize(nLen);
      Send(pBuf, nLen);
      delete [] pBuf;
   }
	// Always return OPOS_SUCCESS to keep the App happy
	return OPOS_SUCCESS;
}

