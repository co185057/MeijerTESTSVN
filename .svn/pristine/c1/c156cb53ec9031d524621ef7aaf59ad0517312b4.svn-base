// BaseMHGenMsg.cpp: implementation of the CBaseMHGenMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseMHGenMsg.h"
#include "TBGenmsg.h"
#include "tracesystem.h"
#include "CurrentPosState.h"
#include "customerMsgMap.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define T_ID    _T("BaseMHGenMsg")

CBaseMHGenMsg::CBaseMHGenMsg()
{
    
}

CBaseMHGenMsg::~CBaseMHGenMsg()
{
    
}

/*************************************************************************
* ProcessMessage - Base TB just returns TB_IGNORE, implement at customer levels
*
* Parameters: 
*  CTBGenMsg *pGenMsg - the message to be parsed
*
* Returns: TBSTATE - the current state of the host application
*************************************************************************/
TBSTATE CBaseMHGenMsg::ProcessMessage(CTBMsgObj *pMsgObj)
{
    tbtraceIO(_T("ProcessMessage"));
	TBSTATE rc = TB_IGNORE;
	
	CTBGenMsg *pGenMsg= dynamic_cast<CTBGenMsg*> (pMsgObj);

	switch (pGenMsg->GetTBMsgId())
	{
	case TBMSG_POSSTATE:
		CCurrentPosState::instance()->setXML(pGenMsg->getXML());
		rc = CCustomerMsgMap::instance()->ProcessMessage();
		break;
	default:
		tbtrace(TM_WARNING, _T("TBMsgObj not handled by BaseMHGenMsg"));
	}
	
    return rc;
}

