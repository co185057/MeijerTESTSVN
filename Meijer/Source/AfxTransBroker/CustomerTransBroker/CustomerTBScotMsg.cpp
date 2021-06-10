/**********************************************************************************/
//	CustomerTBScotMsg.cpp   :   implementation  file for CCustomerTBScotMsg  class            
//                                                                                
//                                                                                
// $Header: 
// $Revision:  
//                                                                                
//	CHANGES: Start                                                                
//	CHANGES: End                                                                  
//                                                                                
/***********************************************************************************/
#include "stdafx.h"
#include "CustomerTBScotMsg.h"
#include "TraceSystem.h"
#include "customertrxbroker.h"

#define T_ID    _T("CCustomerTBScotMsg.cpp")


CCustomerTBScotMsg::CCustomerTBScotMsg()
{
    m_bAutoCleanup= false;
}

CCustomerTBScotMsg::~CCustomerTBScotMsg()
{
}

// Send a message to force a return status code
long CCustomerTBScotMsg::SendReturnStateMsg( long lRetStat, RetStateMsgHandlerType i_pfn, const TCHAR *szMessage)
{
    tbtrace(TM_INFO, _T("SendReturnStateMsg %s"), CCustomerTrxBroker::instance()->GetTBSTATEName(lRetStat));
	
	if (lRetStat == TB_COMPLETEPRINTANDCUT)
	{
		// Turn MainReceipt Back on so that we get the header!
	    CCustomerTrxBroker::instance()->TBSetProperty(_T("MainReceipt"), _T("0"));
		// Reset the printing property.
	}
	return CBaseTBScotMsg::SendReturnStateMsg(lRetStat, i_pfn, szMessage);
}