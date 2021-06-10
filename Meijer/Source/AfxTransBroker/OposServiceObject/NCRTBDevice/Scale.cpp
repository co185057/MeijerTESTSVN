// Scale.cpp: implementation of the CScale class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Scale.h"
#include "OposScal.h"
#include "TBService.h"
#include "TBOposIO.h"
#include "scaleMsg.h"

#include "SocketForTBDevice.h"

#define NCRDIO_SCAL_LIVE_WEIGHT 604

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScale::CScale(int nPort, int nId,  CmDataCapture& cmDc )
					   :CTBDevice(OPOS_CLASSKEY_SCAL,nId,nPort, cmDc)
{
	CDataCapEntryExit DCE( m_cmDc, _T("CScale::CScale") );
}

CScale::~CScale()
{
	CDataCapEntryExit DCE( m_cmDc, _T("CScale::~CScale") );
}


//
// Function:  Open
// Purpose:  Opens the Scale device, loads configuration from the registry
// Inputs:  pService - pointer to the Service object controlling the device
//          oRegistry - pointer to the registry settings for the device
// Outputs:  OPOS_SUCCESS on success
// Assumptions:  The registry settings are located in the key created for the 
//               OPOS profile for the device. The following values can be defined
//                   CapDisplay - (default TRUE) - Specifies whether the scale
//                      has an integrated weight display.
//                   MaximumWeight - (default 999999) - Specifies the maximum weight
//                      that can be handled by the scale.
//                   WeightUnit - (default 4) - Specifies the unit of weight for the
//                      the scale data (See OPOS_Scal.h for additional values)
// Comments:  None
//  
long CScale::Open( CTBService* pService, COposRegistry& oRegistry )
{
	CDataCapEntryExit DCE( m_cmDc, _T("CScale::Open") );
	BOOL bCapDisplay;
	long lResult;
	long lMaxWeight;
	long lWeightUnit;

	bCapDisplay = oRegistry.ReadInt(_T("CapDisplay"), 0, TRUE );
	lMaxWeight = oRegistry.ReadInt(_T("MaximumWeight"), 0, 999999 );
	lWeightUnit = oRegistry.ReadInt(_T("WeightUnit"), 0, SCAL_WU_POUND );

	pService->InitializeScale( bCapDisplay, lMaxWeight, lWeightUnit );
	
	lResult = CTBDevice::Open( pService, oRegistry);

	return lResult;
}

//
// Function:  Close
// Purpose:  Closes the Scale device and cleans up resources related
//           to the device.
// Inputs:  pService - pointer to the Service object controlling the device
// Outputs:  None
// Assumptions:  None
// Comments:  None
//
void CScale::Close( CTBService* pService )
{
	CDataCapEntryExit DCE( m_cmDc, _T("CScale::Close") );
	pService->CloseScale();

	CTBDevice::Close( pService );
}


// 
// Function:  ProcessRawData
// Purpose:  Processes messages received from the TB.
// Inputs:  pData - pointer to the message contents.  The message format is 
//                  defined in TBOposIO.h.
//          dwByteLen - Length of the entire message.
// Outputs:  None
// Assumptions:  The TB sends a message containing a non-zero stable weight.
// Comments:  None
//
void CScale::ProcessRawData( void* pData, DWORD dwByteLen )
{
	CDataCapEntryExit DCE( m_cmDc, _T("CScale::ProcessRawData") );
   if (CSocketForTBDevice::GetSpecVersion() > 1) {
      CScaleMsg scale;
      scale.Parse((BYTE *) pData);
      m_pTBService->SetScaleWeight(scale.Weight());
   }
   else {
	   OPOSIO_HDR* pHdr = (OPOSIO_HDR*) pData;

	   if ( pHdr->deviceID != 0xFF )
	   {
		   m_cmDc.DCPrintf( 0x01, _T("Got a scale message"));
		   OPOSIO_SCALE *pMsg = (OPOSIO_SCALE*) pData;
		   m_pTBService->SetScaleWeight( pMsg->weight );
	   }
	   else
	   {
		   m_cmDc.DCPrintf( 0x01, _T("ProcessRawData ignoring POLL request" ));
	   }
   }
}


// 
// Function:  CommunicationFailure
// Purpose:  Handle a failure of the communication mechanism with the TB
// Inputs:  None
// Outputs:  None
// Assumptions:  None
// Comments:  None
//
void CScale::CommunicationFailure( )
{
	CDataCapEntryExit DCE( m_cmDc, _T("CScale::CommunicationsFailure") );
	CTBDevice::CommunicationFailure();
}

// 
// Function:  ReadWeight
// Purpose:  Obtains the current non-zero stable weight from the scale.
// Inputs:  pWeight - storage for the weight
//          Timeout - amount of time in milliseconds to wait for a stable
//                    non-zero weight
// Outputs:  OPOS_SUCCESS on success
//       OPOS_E_TIMEOUT on timeout
//       OPOS_E_EXTENDED when weight is over max weight
//       OPOS_E_FAILURE on an internal failure
// Assumptions:  None
// Comments:  None
//
long CScale::ReadWeight( long* pWeight, long Timeout )
{
	*pWeight = 0;
	CDataCapEntryExit DCE( m_cmDc, _T("CScale::ReadWeight") );
	return m_pTBService->GetScaleWeight( pWeight, Timeout );
}

long CScale::DirectIO(long nCommand, long *plData, CString& sData)
{
   CDataCapEntryExit DCE( m_cmDc, _T("CScale::DirectIO") );
   if (nCommand == NCRDIO_SCAL_LIVE_WEIGHT)
   {
		*plData = m_pTBService->m_lScaleWeight;
   }
   return SetRC( OPOS_SUCCESS );
}
