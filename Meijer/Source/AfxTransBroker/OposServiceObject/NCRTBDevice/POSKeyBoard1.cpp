// POSKeyBoard1.cpp: implementation of the CPOSKeyBoard class.
//
//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "stdafx.h"
#include "ncrtbdevice.h"
#include "POSKeyBoard1.h"
#include "KeyboardMsg.h"

#include "SocketForTBDevice.h"
#include "tbservice.h"

extern CSocketForTBDevice *pClientNCRTBTBDeviceSock;
extern _globalInfo g_info;

#include "tboposio.h"
#include "tbservice.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPOSKeyBoard::CPOSKeyBoard(int nPort, int nId,  CmDataCapture& cmDc )
					   :CTBDevice(OPOS_CLASSKEY_KBD,nId,nPort, cmDc)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CPOSKeyBoard::CPOSKeyBoard") );
}

CPOSKeyBoard::~CPOSKeyBoard()
{

}

//
// Function:  Open
// Purpose:  Opens the KeyBoard device, loads configuration from the registry
// Inputs:  pService - pointer to the Service object controlling the device
//          oRegistry - pointer to the registry settings for the device
// Outputs:  OPOS_SUCCESS on success
// Assumptions:  The registry settings are located in the key created for the 
//               OPOS profile for the device. 
// Comments:  None
//  
long CPOSKeyBoard::Open( CTBService* pService, COposRegistry& oRegistry )
{
	CDataCapEntryExit DCE( m_cmDc, _T("CPOSKeyBoard::Open") );
//	BOOL bCapStatus;
	long lResult;


//	pService->InitializeCashDrawer( bCapStatus );
	
	lResult = CTBDevice::Open( pService, oRegistry);

	return lResult;
}

//
// Function:  Close
// Purpose:  Closes the PPOSKeyBoard device and cleans up resources related
//           to the device.
// Inputs:  pService - pointer to the Service object controlling the device
// Outputs:  None
// Assumptions:  None
// Comments:  None
//
void CPOSKeyBoard::Close( CTBService* pService )
{
	CTBDevice::Close( pService);
}

// 
// Function:  ProcessRawData
// Purpose:  Process KeyBoard data sent from the TB.
// Inputs:  pData - pointer to the message contents.  The message format is 
//                  defined in TBOposIO.h.
//          dwByteLen - Length of the entire message.
// Outputs:  None
// Assumptions:  The TB sends KeyBoard data from the FastLane Application.
//   POLL messages are ignored since the KeyBoard has nothing to send back.
// Comments:  None
//
void CPOSKeyBoard::ProcessRawData(void* pData, DWORD dwByteLen )
{
	CDataCapEntryExit DCE( m_cmDc, _T("CPOSKeyBoard::ProcessRawData") );
   if (CSocketForTBDevice::GetSpecVersion() > 1) {

		EnterCriticalSection(&g_info.CritSecAcknowledge);
		pClientNCRTBTBDeviceSock->m_TBCountForKeyboardDataLeft++;
		ResetEvent(g_info.hConsumed);
		LeaveCriticalSection(&g_info.CritSecAcknowledge);

        CTBDevice::ProcessRawData(pData, dwByteLen);
   }
   else {
	   OPOSIO_HDR* pHdr = (OPOSIO_HDR*) pData;

	   if ( pHdr->deviceID != 0xFF )
	   {

		   EnterCriticalSection(&g_info.CritSecAcknowledge);
		   pClientNCRTBTBDeviceSock->m_TBCountForKeyboardDataLeft++;
		   ResetEvent(g_info.hConsumed);
		   LeaveCriticalSection(&g_info.CritSecAcknowledge);


		   OPOSIO_KEYSTROKE *pMsg = (OPOSIO_KEYSTROKE*) pData;
		   long pFlags; 
		   pFlags = pMsg->flags;
   //temp		if((pFlags == KBD_ET_DOWN_UP ) && ((m_pService->GetPropertyNumber(PIDXKbd_EventTypes)) == KBD_ET_DOWN))
   //temp			return;
		   void* PKeyData;
		   DWORD dwLen = KEYSTROKE_LEN;

		   PKeyData = (void*)pMsg->scanKeyCode;
		   CTBDevice::ProcessRawData( pData, dwLen );
	   }
	   else
	   {
		   m_cmDc.DCPrintf( 0x01, _T("ProcessRawData ignoring POLL request" ));
	   }
   }
}

//antonio end

