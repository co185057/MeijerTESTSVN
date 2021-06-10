// Scanner.cpp : implementation file
//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "stdafx.h"
#include "NCRTBDevice.h"
#include "Scanner.h"
#include "TBOposIO.h"

#include "scannerMsg.h"


#include "SocketForTBDevice.h"
/////////////////////////////////////////////////////////////////////////////
// CScanner Construction/Destruction

CScanner::CScanner(int nPort, int nId,  CmDataCapture& cmDc )
					   :CTBDevice(OPOS_CLASSKEY_SCAN,nId,nPort, cmDc)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CScanner::CScanner") );
}

CScanner::~CScanner()
{
    CDataCapEntryExit DCE( m_cmDc, _T("CScanner::~CScanner") );
}


/////////////////////////////////////////////////////////////////////////////
// CScanner message handlers

// 
// Function:  CommunicationFailure
// Purpose:  Handle a failure of the communication mechanism with the TB
// Inputs:  None
// Outputs:  None
// Assumptions:  None
// Comments:  None
//
void CScanner::CommunicationFailure()
{
	CDataCapEntryExit DCE( m_cmDc, _T("CScanner::CommunicationsFailure") );
	CTBDevice::CommunicationFailure();
}

// 
// Function:  ProcessRawData
// Purpose:  Process scanner data sent from the TB.
// Inputs:  pData - pointer to the message contents.  The message format is 
//                  defined in TBOposIO.h.
//          dwByteLen - Length of the entire message.
// Outputs:  None
// Assumptions:  The TB sends scanner data from the FastLane Application.
//   POLL messages are ignored since the scanner has nothing to send back.
// Comments:  None
//
void CScanner::ProcessRawData(void* pData, DWORD dwByteLen )
{
	CDataCapEntryExit DCE( m_cmDc, _T("CScanner::ProcessRawData") );

   if (CSocketForTBDevice::GetSpecVersion() > 1) {
      CScannerMsg scannerMsg;
      scannerMsg.Parse((BYTE* ) pData);
      CString csData= scannerMsg.Data();
	  CTBDevice::ProcessRawData( (BYTE*)(LPCTSTR)csData, csData.GetLength()*sizeof(TCHAR) );
   }
   else {
      OPOSIO_HDR* pHdr = (OPOSIO_HDR*) pData;
	   if ( pHdr->deviceID != 0xFF )
	   {
		   OPOSIO_SCANNER *pMsg = (OPOSIO_SCANNER*) pData;
		   void* pScanData;
		   DWORD dwLen;

		   pScanData = (void*)pMsg->Data;
		   dwLen = pMsg->dataLen;

		   CTBDevice::ProcessRawData( pScanData, dwLen );
	   }
	   else
	   {
		   m_cmDc.DCPrintf( 0x01, _T("ProcessRawData ignoring POLL request" ));
	   }
   }
}

