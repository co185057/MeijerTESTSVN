//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMRestrictedNotAllowedBase.cpp
//
// TITLE: Class implementation for Restricted Item State
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMRestrictedNotAllowed.h"        
#include "DMProcedures.h"
DLLEXPORT extern DMProcedures dm;   // Global DM object

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("RestrictedNotAllowed")

IMPLEMENT_DYNCREATE(SMRestrictedNotAllowedBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(RestrictedNotAllowed)

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMRestrictedNotAllowedBase::SMRestrictedNotAllowedBase(const PSTEXT nMsg)
:nMessage(nMsg)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMRestrictedNotAllowedBase::SMRestrictedNotAllowedBase()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMRestrictedNotAllowedBase::Deliver(const PSTEXT nMsg)
{
  nMessage = nMsg;

  return this;
}
//////////////////////////////////////////
bool SMRestrictedNotAllowedBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    // There should be one parameter, a dword
    if (dlParameterList.GetDescriptorCount() == 1)
    {
        trace(L6, _T("+SMRestrictedNotAllowedBase::DeliverDescriptorList() - Parameter Count of 1"));
        Deliver((PSTEXT)DLParamDWORD(dlParameterList));    // nTyp
        return true;
    }
    else
    {
        trace(L2, _T("+SMRestrictedNotAllowedBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}


///////////////////////////////////////
SMStateBase *SMRestrictedNotAllowedBase::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("RestrictedNotAllowed") );	// RFC 330050


  // +TAR 407200
  if (fInMultiSelectPickList)
  {
      g_bMultiPickSkipBaggingAllowed = true;
  }
  // -TAR 407200


  DMSayPhrase(SECURITYMED);


// NewUI
  ps.SetCMFrame(_T("RestrictedNotAllowed"));
  ps.CMButtonFlash(_T("CMButton1Med"), true);
// NewUI
  
  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
  ps.ShowCMTBText(csTBMessage2Scot);//thp
  ps.Message12(TXT_RESTRICTEDNOTALLOW, true); 
  if (!co.IsInAssistMenus())
  {
	  // remote approval
	  CString csDesc, csInstruction;
	  csDesc = ps.GetPSText(RA_THIRDLINE_RESTRICTEDNOTALLOWED, SCOT_LANGUAGE_PRIMARY);
	  csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_LANEBTT_TIMERESTRICTEDITEM, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(LTT_RESTRICTEDITEM, SCOT_LANGUAGE_PRIMARY), co.IsSecurityDisableOn()==true ? ps.GetPSText(RA_SECONDLINE_SECURITYDISABLED, SCOT_LANGUAGE_PRIMARY) : _T(""), ps.GetPSText(RA_THIRDLINE_RESTRICTEDNOTALLOWED, SCOT_LANGUAGE_PRIMARY));
	  trace(L7, _T("+ra.OnRestrictedNotAllowed, Message=%s"), csInstruction);
	  ra.OnRestrictedNotAllowed(csInstruction);
	  trace(L7, _T("-ra.OnRestrictedNotAllowed"));

      dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
          DM_TRICOLORLIGHT_BLINK_1HZ,
          DM_TRICOLORLIGHT_NORMALREQUEST);
      ps.Message12(TXT_RESTRICTEDNOTALLOW, true); 
  }
  else
  {
      lRestrictedNotAllowedItems = 0;
  }
  
  ps.ShowCMFrame();
  return STATE_NULL;    
}

//////////////////////////////////////
SMStateBase *SMRestrictedNotAllowedBase::PSButton1(void) // OK Button
{
  // always return to caller
	// BeginCodeLocaleUPC
  // AKR SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1); // tar 125389
  // -old-code-
	  // SASendSCOTInput(CANCEL_ITEM, io.csOrigCode, 0, 0, 1);
  // EndCodeLocaleUPC			

  if (fInMultiSelectPickList)
	  return setAndCreateAnchorState( BES_PRODUCEFAVORITES );
  else
  {
   
		  return setAndCreateAnchorState(BES_SCANANDBAG);

  }
}

////////////////////////////////////////
SMStateBase *SMRestrictedNotAllowedBase::PSEnterKey(void) // OK for scanned items
{
  // BeginCodeLocaleUPC
  // AKR SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1); // tar 125389
  // -old-code-
	  // SASendSCOTInput(CANCEL_ITEM, io.csOrigCode, 0, 0, 1);
  // EndCodeLocaleUPC			
  DMSayPhrase(KEYPADVALID);
  return PSButton1();             // usually same as button 1
}
// + RFC 330050
void SMRestrictedNotAllowedBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("RestrictedNotAllowed") );	
}
// - RFC 330050

