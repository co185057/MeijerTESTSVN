#include "StdAfx.h"
#include "QueuedActionRecallItem.h"

#define COMP_ID ID_SM
#define T_ID _T("RecallItem")

#ifndef _CPPUNIT
#include "Common.h"                 // Common includes
#else
#include "FakeSMStateBase.h"
#endif //_CPPUNIT

#include "MobileConstantsSSF.h"
/*  
 *  Queued Action class for delayed message screen. 
 *  csData contains dataneeded view string.  Message screen text, title, and wave
 *  are extracted and used to fill in the CustomMessage object,  after which
 *  the CustomMessage state is returned.
 *  Note:QUEUED_MESSAGESCREENITEMNEEDCANCEL and QUEUED_MESSAGESCREENGOBACKSCANANDBAG
 *  are handled when this action is created,  in SMStateBase::OnGenericDelayedIntervention
 */

CQueuedActionRecallItem::CQueuedActionRecallItem(CString csID, CString csDMLastBarLabel, CString csDMLastBarCode, long lBarcodeType)
{
    m_csID = csID;
    trace(L6, _T("CQueuedActionShowMessageScreen"));
    //m_cDMLastData = cDMLastData;
    m_csDMLastBarLabel = csDMLastBarLabel;
    m_csDMLastBarCode = csDMLastBarCode;
    m_lBarcodeType = lBarcodeType;
    m_bIsLoyaltyID = false;
    m_csDescription = _T("");
    m_lItemProperty = MOBILE_ITEMPROP_NORMAL;
    m_bAgeRestricted = false;
    m_lOrigin = MOBILE_ORIGIN_MYSCAN;
    m_lRescanItem = MOBILE_RESCAN_NORESCAN;
    m_lQty = 0;
}

CQueuedActionRecallItem::~CQueuedActionRecallItem()
{
}

SMStateBase* CQueuedActionRecallItem::HandleQueuedAction(SMStateBase* pCurrentState)
{
    SMStateBase* pState = (SMStateBase*) pCurrentState;
    return STATE_NULL;
}

void CQueuedActionRecallItem:: SetActionID(CString csID)  //Used to change the ID to csEntryID of item for Upload Ticket.
{
    m_csID = csID;
}