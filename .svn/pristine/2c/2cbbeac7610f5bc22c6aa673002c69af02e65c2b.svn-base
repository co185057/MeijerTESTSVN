// BaseMHMSR.cpp: implementation of the CBaseMHMSR class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BaseMHMSR.h"
#include "TraceSystem.h"
#include "MsrMsg.h"
#include "basecurrentMSR.h"

#include "BaseObjectManager.h"

#define T_ID  _T("CBaseMHMSR")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseMHMSR::CBaseMHMSR()
{

}

CBaseMHMSR::~CBaseMHMSR()
{

}

/*************************************************************************
* ProcessMessage - Process the message posted by TB. TB analyzes the message 
*       and returns the state of the host application, based on the presentation 
*       data in the message. Dynamic cast to a MSRMsg and then call Refresh()
*       tp get data into CurrentMSR class
*
* Parameters: 
*  PSMessageElement psMessage - Pointer to the TB message
*
* Returns: TBSTATE - the current state of the host application
*************************************************************************/
TBSTATE CBaseMHMSR::ProcessMessage(CTBMsgObj *pMsgObj)
{
    tbtraceIO(_T("ProcessMessage"));
    TBSTATE rc = TB_IGNORE;
	 CMSRMsg *pMsg= dynamic_cast<CMSRMsg *> (pMsgObj);
	 ASSERT(pMsg);
	 if (pMsg) {
	    CBaseCurrentMSR* pCurr = CCustomerCurrentMSR::instance();
		 pCurr->Refresh(pMsg);
	 }
    return rc;
}

