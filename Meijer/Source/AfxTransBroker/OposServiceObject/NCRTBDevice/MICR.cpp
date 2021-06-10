// MICR.cpp: implementation of the CMICR class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "MICR.h"
#include "TBService.h"
#include "TBOposIO.h"
#include "Resource.h"

#include "SocketForTBDevice.h"
#include "micrMsg.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMICR::CMICR(int nPort, int nId,  CmDataCapture& cmDc )
					   :CTBDevice(OPOS_CLASSKEY_PTR,nId,nPort, cmDc)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CMICR::CMICR") );
}

CMICR::~CMICR()
{
    CDataCapEntryExit DCE( m_cmDc, _T("CMICR::~CMICR") );
}

//
// Function:  Open
// Purpose:  Opens the MICR and initializes operations based on the 
//           contents of the registry key for the device profile.
// Inputs:  pService - pointer to CTBService object that handles the service
//                     operations for the device.
//          oRegistry - Registry class for accessing profile properties.
// Outputs:  Return code from Base Class
// Assumptions:  None
// Comments:  None
// 
long CMICR::Open( CTBService* pService, COposRegistry& oRegistry )
{
	CString sRemoveNonDigits;
    CDataCapEntryExit DCE( m_cmDc, _T("CMICR::Open") );

	oRegistry.ReadString(_T("RemoveNonDigits"), sRemoveNonDigits, 0, _T("TA") );

    pService->InitializeMICR(sRemoveNonDigits);

	return CTBDevice::Open( pService, oRegistry );
}

// 
// Function: Close
// Purpose:  Frees resources related to an instance of a MICR device
// Inputs:  pService - pointer to CTBService object that handles the service
//                     operations for the device.
// Outputs:  None
// Assumptions:  None
// Comments:  None
//
void CMICR::Close( CTBService* pService )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CMICR::Close") );
	CTBDevice::Close( pService);
}

// 
// Function:  CommunicationFailure
// Purpose:  Handle a failure of the communication mechanism with the TB
// Inputs:  None
// Outputs:  None
// Assumptions:  None
// Comments:  None
//
void CMICR::CommunicationFailure()
{
	CDataCapEntryExit DCE( m_cmDc, _T("CMICR::CommunicationsFailure") );
	CTBDevice::CommunicationFailure();
}

//
// Function:  BeginInsertion
// Purpose:  Implementation of the OPOS BeginInsertion method
// Inputs:  Timeout - Not used in this implementation
// Outputs:  Always OPOS_SUCCESS to keep the calling app happy
// Assumptions:  None
// Comments:  None
//
long CMICR::BeginInsertion(long Timeout)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CMICR::BeginInsertion") );
	// The "device" is placed into insertion mode.
	// Return success to indicate a form was inserted.
	// The TB does not need to be notified.
	this->m_pTBService->SetSlipEmpty( FALSE );
	this->OnStatus( PTR_SUE_SLP_PAPEROK );
	return OPOS_SUCCESS;
}

//
// Function:  BeginRemoval
// Purpose:  Implementation of the OPOS BeginRemoval method
// Inputs:  Timeout - Not used in this implementation
// Outputs:  Always OPOS_SUCCESS to keep the calling app happy
// Assumptions:  None
// Comments:  None
//
long CMICR::BeginRemoval(long Timeout)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CMICR::BeginRemoval") );
	// The "device" is placed into form removal mode
	// Return success to indicate a form was removed
	// The TB does not need to be notified.
	this->m_pTBService->SetSlipEmpty(TRUE);
	this->OnStatus( PTR_SUE_SLP_EMPTY );
	return OPOS_SUCCESS;
}

//
// Function:  EndInsertion
// Purpose:  Implementation of the OPOS EndInsertion method
// Inputs:  None
// Outputs:  Always OPOS_SUCCESS to keep the calling app happy
// Assumptions:  None
// Comments:  None
//
long CMICR::EndInsertion()
{
    CDataCapEntryExit DCE( m_cmDc, _T("CMICR::EndInsertion") );
	// The "device" is taken out of insertion mode
	// Return success to indicate that a form is present
	// The TB does not need to be notified.
	return OPOS_SUCCESS;
}

//
// Function:  EndRemoval
// Purpose:  Implementation of the OPOS EndRemoval method
// Inputs:  None
// Outputs:  Always OPOS_SUCCESS to keep the calling app happy
// Assumptions:  None
// Comments:  None
//
long CMICR::EndRemoval()
{
    CDataCapEntryExit DCE( m_cmDc, _T("CMICR::EndRemoval") );
	// The "device is taken out of removal mode
	// Return success to indicate the form is gone.
	// The TB does not need to be notified.
	return OPOS_SUCCESS;
}

// 
// Function:  NotifyTB
// Purpose:  Sends a message to the TB
// Inputs: pBuffer - buffer containing the message
//         lBufferLen - length of the data to be sent
// Outputs:  OPOS_SUCCESS
// Assumptions:  None
// Comments:  None
//
long CMICR::NotifyTB( BYTE* pBuffer, long lBufferLen )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CMICR::NotifyTB") );

    if (CSocketForTBDevice::GetSpecVersion() > 1) {
      CMicrMsg micr;
      micr.Data(pBuffer);
      int nLen;
      BYTE *pBuf= micr.Serialize(nLen);
      Send(pBuf, nLen);
      delete [] pBuf;
    }
    else {
	   OPOSIO_MICR* pMsg2 = (OPOSIO_MICR*) pBuffer;

	   pMsg2->hdr.deviceID = TBMSG_MICR2;
	   pMsg2->hdr.port = m_nPort;

	   long				lResult = OPOS_E_FAILURE;
	   int					iSent;
	   unsigned int iLength = 0;

	   long SocketMsgLen = lBufferLen + sizeof(tSpecHeaderV01);
	   BYTE *SocketBuffer;
	   SocketBuffer = new BYTE [SocketMsgLen + 1];
	   memset( SocketBuffer, 0, SocketMsgLen + 1);

	   OPOSIO_MICR_SOCKET* pMsg = (OPOSIO_MICR_SOCKET *) SocketBuffer;
	   pMsg->Spec.messageID = TBMSG_MICR;
	   pMsg->Spec.nLen = SocketMsgLen;
	   pMsg->Spec.nVersion = 1;
	   
	   memmove( pMsg->Data, pMsg2, lBufferLen );


	   iSent = Send((BYTE *)SocketBuffer, SocketMsgLen);

	   delete [] SocketBuffer;
	   if (iSent != SOCKET_ERROR)
	   {
		   lResult = OPOS_SUCCESS;
	   }
	   else
	   {
		   m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Failed to write to PipeServer Socket"));
	   }
    }
	 return OPOS_SUCCESS;
}

//
// Function:  ProcessRawData
// Purpose:  Process messages sent from the TB
// Inputs:  pData - buffer containing the message data
//          dwByteLen - the length of the message data
// Outputs:  None
// Assumptions:  the only expected message is the POLL message
// Comments: None
//
void CMICR::ProcessRawData( void* pData, DWORD dwByteLen )
{
	CDataCapEntryExit DCE( m_cmDc, _T("CMICR::ProcessRawData") );
   if (CSocketForTBDevice::GetSpecVersion() > 1) {
      CMicrMsg micr;
      micr.Parse((BYTE *) pData);
      CString csData= micr.Data();
      CTBDevice::ProcessRawData( (void*) (LPCTSTR)csData, csData.GetLength()*sizeof(TCHAR)); 

   } else {
      OPOSIO_HDR* pHdr = (OPOSIO_HDR*) pData;
	   if ( pHdr->deviceID != 0xFF )
	   {
		   OPOSIO_MICR *pMsg = (OPOSIO_MICR*) pData;
		   void* pMICRData;
		   DWORD dwLen;

		   pMICRData = (void*)pMsg->Data;
		   dwLen = pMsg->dataLen;

		   CTBDevice::ProcessRawData( pMICRData, dwLen );
	   }
	   else
	   {
		   m_cmDc.DCPrintf( 0x01, _T("ProcessRawData ignoring POLL request" ));
	   }
   }
}

void CMICR::HandleClose()
{
//	ClientNCRTBMICRSock.Close();
}
