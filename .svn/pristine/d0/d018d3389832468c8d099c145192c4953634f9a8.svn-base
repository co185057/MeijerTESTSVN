// MSR.cpp: implementation of the CMSR class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NCRTBDevice.h"
#include "MSR.h"
#include "tbservice.h"
#include "TBOposIO.h"
#include "SocketForTBDevice.h"
#include "msrMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMSR::CMSR(int nPort, int nId,  CmDataCapture& cmDc )
			 :CTBDevice(OPOS_CLASSKEY_MSR,nId,nPort, cmDc)
{
   CDataCapEntryExit DCE( m_cmDc, _T("CMSR::CMSR") );
}

CMSR::~CMSR()
{
    CDataCapEntryExit DCE( m_cmDc, _T("CMSR::~CMSR") );
}

/////////////////////////////////////////////////////////////////////////////
// CMSR message handlers

// 
// Function:  CommunicationFailure
// Purpose:  Handle a failure of the communication mechanism with the TB
// Inputs:  None
// Outputs:  None
// Assumptions:  None
// Comments:  None
//
void CMSR::CommunicationFailure()
{
	CDataCapEntryExit DCE( m_cmDc, _T("CMSR::CommunicationsFailure") );
	CTBDevice::CommunicationFailure();
}

// 
// Function:  ProcessRawData
// Purpose:  Process scanner data sent from the TB.
// Inputs:  pData - pointer to the message contents.  The message format is 
//                  defined in TBOposIO.h.
//          dwByteLen - Length of the entire message.
// Outputs:  None
// Assumptions:  The TB sends MSR data from the FastLane Application.
//   POLL messages are ignored since the scanner has nothing to send back.
// Comments:  None
//
void CMSR::ProcessRawData(void* pData, DWORD dwByteLen )
{
	CDataCapEntryExit DCE( m_cmDc, _T("CMSR::ProcessRawData") );
   BYTE lStatus[4]={0};
   if (CSocketForTBDevice::GetSpecVersion() > 1) {
      CMSRMsg msr;
      msr.Parse((BYTE*)pData);
      lStatus[3]= (BYTE) msr.Track1Len();   // track1 length
      lStatus[0]= (BYTE) msr.Track1Len();   // track1 length
      lStatus[1]= (BYTE) msr.Track2Len();   // track2 length
      lStatus[2]= (BYTE) msr.Track3Len();   // track3 length
      long *pl= (long *) &lStatus[0];
      OnData((LPCTSTR) pData, *pl , dwByteLen );
   }
   else {
      OPOSIO_MSRTRACKS   *pMsrData= (OPOSIO_MSRTRACKS *) pData;

	   if ( pMsrData->hdr.deviceID != 0xFF )	{
         lStatus[3]= (BYTE) pMsrData->track1Len;   // track1 length
         lStatus[0]= (BYTE) pMsrData->track1Len;   // track1 length
         lStatus[1]= (BYTE) pMsrData->track2Len;   // track2 length
         lStatus[2]= (BYTE) pMsrData->track3Len;   // track3 length
         long *pl= (long *) &lStatus[0];

         m_pTBService->m_MsrProp.ParseMsrData(pData);
         pData= &pMsrData->Data[6];
         dwByteLen= pMsrData->track1Len + pMsrData->track2Len + pMsrData->track3Len;

         OnData((LPCTSTR) pData, *pl , dwByteLen );
	   }else
		   m_cmDc.DCPrintf( 0x01, _T("ProcessRawData ignoring POLL request" ));
   }
}

void CMSR::HandleClose()
{
	//ClientNCRTBMSRSock.Close();
}


long CMSR::Open(CTBService *pService, COposRegistry &oRegistry)
{
   CDataCapEntryExit DCE( m_cmDc, _T("CMSR::Open") );
   pService->InitializeMSR();
	return CTBDevice::Open( pService, oRegistry );
}
