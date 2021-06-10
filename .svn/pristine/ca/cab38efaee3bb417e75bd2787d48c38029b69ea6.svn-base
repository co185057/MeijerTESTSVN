//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmSecurityLogsBase.CPP
//
// TITLE: Class implementation for store mode Coupon Items state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMSmSecurityLogs.h"       // MGV this state
#include "SMSmAuthorization.h"      // MGV auth state
#include "DMProcedures.h"

DLLEXPORT extern DMProcedures dm;             // global DM object

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmSecurityLogsBase")

IMPLEMENT_DYNCREATE(SMSmSecurityLogsBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SmSecurityLogs)


DEFINE_TIMESTAMP
////////////////////////////////////
SMSmSecurityLogsBase::SMSmSecurityLogsBase()
{
  IMPLEMENT_TIMESTAMP
}

/////////////////////////////////////////
SMStateBase  *SMSmSecurityLogsBase::Initialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmSecurityLogs") );	// RFC 330050
	ps.SetFrame(_T("SmSecurityLogs"));

    ps.ShowSMTotal(true);
	ps.ShowSMTBText(csTBMessage2Scot);	//thp
	bStoreModeApprovals = true; //SSCOP-1403

	int nSizeofSecDelayIntervention = SecMgr.GetOutstandingInterventionCount();
	SecDelayIntervention *psecDelayInterventionArray = new SecDelayIntervention[nSizeofSecDelayIntervention];
	SecMgrEventParms parms(psecDelayInterventionArray);

	CString csInterventionList = SecMgr.GetOutstandingInterventionList();
	if(!csInterventionList.IsEmpty())
	{
		parms.ParseRecords(SecMgr.GetOutstandingInterventionList());

		for(int i = 0; i < nSizeofSecDelayIntervention; i++)
		{
		  CString csInstruction;
		  if(psecDelayInterventionArray[i].csSecInterventionType == KEY_OSE_EXC_TYPE_UNEX_INCREASE)
		  {
			  csInstruction.Format(ps.GetPSText(MSG_NEWSEC_UNEX_INCREASE, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_WTINCREASE, SCOT_LANGUAGE_PRIMARY), psecDelayInterventionArray[i].csDeltaWeight, csScaleUnits);
		  }
		  else if(psecDelayInterventionArray[i].csSecInterventionType == KEY_OSE_EXC_TYPE_UNEX_DECREASE)
		  {
			  csInstruction.Format(ps.GetPSText(MSG_NEWSEC_UNEX_DECREASE, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_WTDECREASE, SCOT_LANGUAGE_PRIMARY), psecDelayInterventionArray[i].csDeltaWeight, csScaleUnits);
		  }
		  else if(psecDelayInterventionArray[i].csSecInterventionType == KEY_OSE_EXC_TYPE_MISMATCHED)
		  {
			  csInstruction.Format(ps.GetPSText(MSG_NEWSEC_MISMATCH, SCOT_LANGUAGE_PRIMARY), psecDelayInterventionArray[i].csItemDescription, ps.GetPSText(RA_WTMISMATCH, SCOT_LANGUAGE_PRIMARY), psecDelayInterventionArray[i].csExpectedWeight, csScaleUnits, psecDelayInterventionArray[i].csActualWeight, csScaleUnits);
		  }
		  else if(psecDelayInterventionArray[i].csSecInterventionType == KEY_OSE_EXC_TYPE_SKIP_BAGGING_THRESHOLD)
		  {
			  csInstruction = ps.GetPSText(RA_SKIPBAGGINGTHRESHOLD, SCOT_LANGUAGE_PRIMARY);
		  }
		  else if(psecDelayInterventionArray[i].csSecInterventionType == KEY_OSE_EXC_TYPE_ITEM_REMOVED_THRESHOLD)
		  {
			  csInstruction = ps.GetPSText(RA_REMOVEITMESTHRESHOLD, SCOT_LANGUAGE_PRIMARY);
		  }
		  else if(psecDelayInterventionArray[i].csSecInterventionType == KEY_OSE_EXC_TYPE_BAG_BOX_THRESHOLD)
		  {
			  csInstruction = ps.GetPSText(RA_BAGBOXTHRESHOLD, SCOT_LANGUAGE_PRIMARY);
		  }
		  else if(psecDelayInterventionArray[i].csSecInterventionType == KEY_OSE_EXC_TYPE_NEW_ITEM_THRESHOLD)
		  {
			  csInstruction = ps.GetPSText(RA_NEWWEIGHTITEM, SCOT_LANGUAGE_PRIMARY);
		  }



		  ps.AddDelaySecurityList(csInstruction);

		}
	}
	else
	{
		trace(L6, _T("SMSmSecurityLogsBase::Initialize(), Intervention List is empty"));
		delete [] psecDelayInterventionArray;
		RETURNSTATE(SmAuthorization)
	}
	delete [] psecDelayInterventionArray;

	ps.ShowFrame();

 
  return STATE_NULL;                // stay in SmSecurityLogsBase state
}

////////////////////////////////////////
SMStateBase  *SMSmSecurityLogsBase::PSButton1(void) // clear security log
{
  lSAYellowNeedingApproval = 0;
	
//  SAClearOtherTallies();

  // remote approval
  trace(L7, _T("+ra.OnSecurityApproval"));
  ra.OnSecurityApproval();
  trace(L7, _T("-ra.OnSecurityApproval"));

  lSALoggedMsgs  = 0;	
  CKeyValueParmList parmList;
  CKeyValue<const _TCHAR*>::Put(parmList, KEY_OSE_EXCEPTION_ID, _T("*"));
  CKeyValue<const _TCHAR*>::Put(parmList, KEY_OSM_OPERATION, KEY_EXCEPTION_OPERATION_CLEAR_DELAYED);
  _bstr_t bstrParmList((const _TCHAR *)parmList); 
  SecMgr.OverrideException(lSecMgrCookie, bstrParmList);
  ps.ResetDelaySecurityList();
  g_bClearAllDelayedSecIntervention = true;
  
  //sscoadk-5431
  //return STATE_NULL; // wait for WMRA_SEND_INTERVENTION_APPROVED
  trace(L6, _T("Going to SmAuthorization to wait on WMRA_SEND_INTERVENTION_APPROVED"));
  RETURNSTATE(SmAuthorization)  //wait for WMRA_SEND_INTERVENTION_APPROVED at SmSmAuthorization state
}

////////////////////////////////////////
SMStateBase  *SMSmSecurityLogsBase::PSButton8(void) // cancel
{
  bStoreModeGoBack = true;
  bStoreModeApprovals = false; //sscoadk-5431
  ps.ResetDelaySecurityList();
  RETURNSTATE(SmAuthorization)
}
// + RFC 330050
void SMSmSecurityLogsBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmSecurityLogs") );	
}
// - RFC 330050

/*
SMStateBase *SMSmSecurityLogsBase::RAParse(MessageElement *me)
{
    trace(L4,_T("SMSmSecurityLogsBase::RAParse: me->raInfo.msg = %d on thread 0x%08.8x"), me->raInfo.msg, GetCurrentThreadId());

    if (me->raInfo.msg == WMRA_SEND_INTERVENTION_APPROVED )
    {
        if (!isAnyApprovalRequired())
        {
            g_bInterventionApproval = false;
            if( g_bInterventionsOnStoreModeEntry )
            {
                g_bInterventionsOnStoreModeEntry = false;
                rp.ClearOperator();
                return createAnchorState();
            }
            else
                RETURNSTATE(SmAuthorization)
        }
        else
            RETURNSTATE(SmAuthorization)
    }

    return STATE(StoreModeState)::RAParse(me);
}
*/
