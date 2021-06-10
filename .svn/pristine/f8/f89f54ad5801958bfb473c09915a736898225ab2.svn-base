#include "StdAfx.h"
#include "QueuedActionRescanItem.h"

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

CQueuedActionRescanItem::CQueuedActionRescanItem(CString csID, CString csDMLastBarLabel, CString csDMLastBarCode, 
                                                 CString csResponseItemCode, CString csResponseErrorString, long lBarcodeType):
CQueuedActionRecallItem(csID, csDMLastBarLabel, csDMLastBarCode, lBarcodeType)
{
    m_csResponseItemCode = csResponseItemCode;
    m_csResponseErrorString = csResponseErrorString;
    m_bAddItemToTransaction = false;
}

CQueuedActionRescanItem::~CQueuedActionRescanItem()
{
}

