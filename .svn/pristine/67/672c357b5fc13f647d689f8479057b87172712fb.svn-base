//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStoreModeState.cpp
//
// TITLE: Base class for Store Mode states
//
// AUTHOR:  Jeff Connelly
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first

#include "Common.h"
#include "SMStoreModeState.h"
#include "SMSmAssistMode.h"
#include "SMSmAssistMenu.h"
#include "transbrokerssf.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("StoreModeState")

IMPLEMENT_DYNCREATE(SMStoreModeState, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////////
SMStoreModeState::SMStoreModeState()
{
  IMPLEMENT_TIMESTAMP
}

bool SMStoreModeState::storeMode()
{
    return SMStoreModeStateBase::storeMode();
}

bool SMStoreModeState::PSRemoteLegalForState()
{
    return SMStoreModeStateBase::PSRemoteLegalForState();
}

bool SMStoreModeState::PSAssistMenuLegalForState()
{
    return SMStoreModeStateBase::PSAssistMenuLegalForState();
}

SMStateBase * SMStoreModeState::createLevel1Message( LPCTSTR szMsg,
                                           PSTEXT pstMsg,
                                           PSTEXT prompt,
                                           long   value,
                                           long   device,
                                           bool   checkHealth,
                                           bool displayHTML)
{
    return SMStoreModeStateBase::createLevel1Message( szMsg, pstMsg, prompt, value, device, checkHealth, displayHTML);
}

SMStateBase * SMStoreModeState::createLevel2Message( LPCTSTR szMsg,
                                           PSTEXT pstMsg,
                                           PSTEXT prompt,
                                           long   value,
                                           long   device,
                                           bool   checkHealth,
                                           bool displayHTML)
{
    return SMStoreModeStateBase::createLevel2Message( szMsg, pstMsg, prompt, value, device, checkHealth, displayHTML);
}

SMStateBase * SMStoreModeState::createLevel3Message( LPCTSTR szMsg,
                                           PSTEXT pstMsg,
                                           PSTEXT prompt,
                                           long   value,
                                           long   device,
                                           bool   checkHealth, 
                                           bool displayHTML )
{
    return SMStoreModeStateBase::createLevel3Message( szMsg, pstMsg, prompt, value, device, checkHealth, displayHTML );
}

bool SMStoreModeState::DMCashRecyclerShouldReject(const CString &csStateName)
{
    return SMStoreModeStateBase::DMCashRecyclerShouldReject(csStateName);
}

SMStateBase * SMStoreModeState::DMAcceptor(void)
{
    return SMStoreModeStateBase::DMAcceptor();
}

SMStateBase *SMStoreModeState::HandleOutstandingInterventions()
{

//Start CRD 407201
	if (isGenericDelayedInterventionApprovalRequired())
	{
		//Go to Assist Menu
		m_GenericDelayedInterventionStatus = GENERIC_DELAYED_STARTING;
		RETURNSTATE(SmAssistMenu);
    }

	if (isRestrictedItemApprovalRequired()  && GetHookProperty(_T("IsDelayedAgeRestrictedProp")) == _T("1"))
    {
		//should go to AM Age Verify
		CString posKey = _T("{PAYMENT}");
		SendTBCommand(TBCMD_SENDKEYEDDATA, posKey);
		RETURNSTATE(SmAssistMode)
    }
	//End CRD 407201
    return SMStoreModeStateBase::HandleOutstandingInterventions();
}