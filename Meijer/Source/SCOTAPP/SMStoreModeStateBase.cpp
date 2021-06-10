//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStoreModeStateBase.cpp
//
// TITLE: Base class for Store Mode states
//
// AUTHOR:  Jeff Connelly
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first

#include "Common.h"
#include "SMStoreModeState.h"
#include "SMSystemMessage1.h"
#include "SMSystemMessage2.h"
#include "SMSystemMessage3.h"
#include "SMState.h"                //SSCOB-257
#include <assert.h> 
#include "CMInterface.h"
#include "SMSmEnterBirthdate.h"
#include "SMSmRestrictedItems.h"
#include "SMSmCouponItems.h"
#include "SMSmVisualItems.h"
#include "SMSmSecurityLogs.h"


#define COMP_ID ID_SM               // base state component
#define T_ID _T("StoreModeStateBase")

IMPLEMENT_DYNCREATE(SMStoreModeStateBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(StoreModeState)


DEFINE_TIMESTAMP

////////////////////////////////////
SMStoreModeStateBase::SMStoreModeStateBase()
{
  IMPLEMENT_TIMESTAMP
}

///////////////////////////////////////////////////////////////////
SMStateBase * SMStoreModeStateBase::createLevel1Message( LPCTSTR szMsg,
		                                            PSTEXT pstMsg,
		                                            PSTEXT prompt,
		                                            long   value,
		                                            long   device,
		                                            bool   checkHealth,
													bool displayHTML) //SR672
{
  CREATE_AND_DISPENSE(SystemMessage1)(szMsg,pstMsg,prompt,value,device,checkHealth,displayHTML);
}

///////////////////////////////////////////////////////////////////
SMStateBase * SMStoreModeStateBase::createLevel2Message( LPCTSTR szMsg,
		                                            PSTEXT pstMsg,
		                                            PSTEXT prompt,
		                                            long   value,
		                                            long   device,
		                                            bool   checkHealth,
													bool displayHTML) //SR672
{
  CREATE_AND_DISPENSE(SystemMessage2)(szMsg,pstMsg,prompt,value,device,checkHealth,displayHTML);
}

///////////////////////////////////////////////////////////////////
SMStateBase * SMStoreModeStateBase::createLevel3Message( LPCTSTR szMsg,
		                                            PSTEXT pstMsg,
		                                            PSTEXT prompt,
		                                            long   value,
		                                            long   device,
		                                            bool   checkHealth,
													bool displayHTML) //SR672
{
  CREATE_AND_DISPENSE(SystemMessage3)(szMsg,pstMsg,prompt,value,device,checkHealth,displayHTML);
}

// Assist mode may be run legally from the RAP station
bool SMStoreModeStateBase::PSRemoteLegalForState() //awk.10.22.2
{
	return true;
}
// Assist mode may be run legally from the RAP station
bool SMStoreModeStateBase::PSAssistMenuLegalForState() //awk.10.22.2
{
	return true;
}

//SSCOB-257+
SMStateBase *SMStoreModeStateBase::PSParse(const MessageElement *me)
{
  
  // save all the PS parameters
  nPSLastEvt = (PSEVT)me->psInfo.nEvt;
  nPSLastParm = me->psInfo.lParm;
  nPSLastWParam = me->psInfo.wParm;
  CString csContext;

  csContext = me->psInfo.pszContext; 

  trace(L5,_T("Parse PS %d, wparam:0x%x, lparam:0x%x, csContext: %s"),nPSLastEvt,me->psInfo.wParm, nPSLastParm, csContext);

  if ( csContext.Find(_T("Attract")) != -1 )
  {
	 trace(L5, _T("Ignore any button press from Attract"));
	 return STATE_NULL;
  }

  return STATE(State)::PSParse(me);
}
//SSCOB-257-


SMStateBase *SMStoreModeStateBase::DMAcceptor()
{
    trace( L6, _T("SMStoreModeStateBase::DMAcceptor()") );

    CString csInserted;
    csInserted.Format(_T("%d"), nDMLastAcceptor);

    // Determine if the last deposit was a coin or bill.
    assert(nDMLastDev == DMCLASS_CASHACCEPTOR ||
           nDMLastDev == DMCLASS_COINACCEPTOR);

    bool bNote;     
    switch(nDMLastDev)
    {
    case DMCLASS_CASHACCEPTOR:
        bNote = true;
        break;
    case DMCLASS_COINACCEPTOR:
        bNote = false;
        break;
    default:
        trace(L6, _T("Event not supported. Return to base."));
        return STATE(State)::DMAcceptor();
    }

	//SSCOB - 17583
	if (dm.m_csCashRecyclerModel.Find(_CASHDISPENSER_GLORY) != -1){
		m_cmMgr.CurrencyDeposited((LPCTSTR)csInserted, bNote);
	}

    return STATE_NULL;
}

// +SSCOB-2631
// Add Glory CM Support
bool SMStoreModeStateBase::DMCashRecyclerShouldReject(const CString &csStateName)
{
    trace(L6,_T("SMStoreModeStateBase::DMCashRecyclerShouldReject"));

    CString csCashRecyclerModel = dm.m_csCashRecyclerModel;
    dm.m_csCashRecyclerModel.Empty();

    bool bRet = STATE(State)::DMCashRecyclerShouldReject(csStateName);
    dm.m_csCashRecyclerModel = csCashRecyclerModel;

    return bRet;
}
// -SSCOB-2631

SMStateBase *SMStoreModeStateBase::HandleOutstandingInterventions()
{
    //USSF Start
	if (co.fOperationsEnableUSSF)
		USSFEVENT(_T("USSFApproval"), if (nextState != STATE_NULL){return nextState;}, ; );
    //USSF End
    if (isRestrictedItemApprovalRequired())
    {
        //+ Personalization age prompt
        CString csDOB = custInfo.GetPreference(_T("Birthdate"));
        if((custInfo.GetOption(_T("PersonalizedAgePrompt")).CompareNoCase(_T("y"))==0) &&
            (csDOB.GetLength() > 0))
        {
            bStoreModeApprovals = true; //SSCOP-4153
            RETURNSTATE(SmEnterBirthdate)
        }
        else
        {
            g_bCustomerLanguageChanged = true; //SSCOADK-5989
            RETURNSTATE(SmRestrictedItems)
        }
        //- Personalization age prompt
    }
    if (isCouponRatioApprovalRequired())
    {
        
        //+SSCOADK-1365
        //tell RAP about the intervention in case hasn't been done: 
        CString csAnchorStateName = getSCOTStateName();
        CString csDesc, csInstruction;
        csDesc = ps.GetPSText(TXT_COUPON_THRESHOLD_EXCEEDED, SCOT_LANGUAGE_PRIMARY);
        csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(TXT_COUPON, SCOT_LANGUAGE_PRIMARY), csAnchorStateName, 
            co.IsSecurityDisableOn()==true || co.GetSecurityDisableSubCheckOn() ? ps.GetPSText(RA_SECONDLINE_SECURITYDISABLED, SCOT_LANGUAGE_PRIMARY) : _T(""), ps.GetPSText(RA_THIRDLINE_QUANTITYRESTRICTEDITEM, SCOT_LANGUAGE_PRIMARY));
        
        trace(L7, _T("+ra.OnCouponItem, csInstruction=%s, lTotalCouponAmount=%d"), csInstruction, lTotalCouponAmount);
        ra.OnCouponItem(csInstruction, lTotalCouponAmount, true);
        trace(L7, _T("-ra.OnCouponItem"));
        //-SSCOADK-1365
        RETURNSTATE(SmCouponItems)
    }
    
    if (isVisualItemApprovalRequired())
        RETURNSTATE(SmVisualItems)
        
    if (isSecurityApprovalRequired() && !g_bClearAllDelayedSecIntervention)
        RETURNSTATE(SmSecurityLogs)

    //bStoreModeApprovals = false; //sscoadk-5431

    return STATE_NULL;
            
}