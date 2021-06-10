#include "StdAfx.h"
#include "QueuedActionShowMessageScreen.h"

#define COMP_ID ID_SM
#define T_ID _T("ShowMessage")

#ifndef _CPPUNIT
#include "Common.h"                 // Common includes
#include "SMCustomMessage.h"
#include "PSConfigData.h"
#else
#include "FakeSMStateBase.h"
#include "FakeDataEntry.h"
#include "FakeSCOTDataEntry.h"
#include "FakeSMCustomMessageBase.h"
#include "FakePSConfigData.h"
#define CDataEntry CFakeDataEntry
#define CSCOTDataEntry CFakeSCOTDataEntry
#define SMCustomMessage FakeSMCustomMessage
#define CPSConfigData CFakePSConfigData
#endif //_CPPUNIT



/*  
 *  Queued Action class for delayed message screen. 
 *  csData contains dataneeded view string.  Message screen text, title, and wave
 *  are extracted and used to fill in the CustomMessage object,  after which
 *  the CustomMessage state is returned.
 *  Note:QUEUED_MESSAGESCREENITEMNEEDCANCEL and QUEUED_MESSAGESCREENGOBACKSCANANDBAG
 *  are handled when this action is created,  in SMStateBase::OnGenericDelayedIntervention
 */
CQueuedActionShowMessageScreen::CQueuedActionShowMessageScreen(const CString &csData)
{
    trace(L6, _T("CQueuedActionShowMessageScreen"));
    m_csActionString = csData;
    CDataEntry dataEntry;
    dataEntry.FromString(m_csActionString);

    m_csID = dataEntry.ExtractString(_T("ID"));

    m_csMessageScreenTitle = dataEntry.ExtractString(QUEUED_MESSAGESCREENTITLE);
    m_csMessageScreenText = dataEntry.ExtractString(QUEUED_MESSAGESCREENTEXT);
    m_csPlayWave = dataEntry.ExtractString(QUEUED_MESSAGESCREENWAVEFILE);
}

CQueuedActionShowMessageScreen::~CQueuedActionShowMessageScreen()
{
}

SMStateBase* CQueuedActionShowMessageScreen::HandleQueuedAction(SMStateBase* pCurrentState)
{
    if (m_csMessageScreenTitle.IsEmpty() && m_csMessageScreenText.IsEmpty())
    {
        if(m_csPlayWave)
        {
            pCurrentState->DMSayPhrase(m_csPlayWave);
        }
        return STATE_NULL;
    }
    CPSConfigData cd;
    CString csMessageScreenTitle = m_csMessageScreenTitle;
    CString csMessageScreenText = m_csMessageScreenText;
    CString csPlayWave = m_csPlayWave;

    cd.FormatString(csMessageScreenTitle);
    cd.FormatString(csMessageScreenText);
    cd.FormatString(csPlayWave);

    CustomMessageObj.csLeadThruText 	= csMessageScreenTitle;
	CustomMessageObj.csScreenText		= csMessageScreenText;
	CustomMessageObj.csPlayWave 		= csPlayWave;

	CustomMessageObj.csE_ReceiptText.Empty();
	CustomMessageObj.csRAP_MessageText.Empty();
	CustomMessageObj.bHiliteReceiptText = false;
	CustomMessageObj.bApprovalNeeded	= false;
	CustomMessageObj.bNeedItemCancel	= false;
	CustomMessageObj.bGoBackScanAndBag	= false;
    CustomMessageObj.bHiliteRAPText     = false;
       
    RETURNSTATE(CustomMessage);
}

