//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMHelpOnWayBase.CPP
//
// TITLE: Class implementation for Help Coming state
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMHelpOnWay.h"            // MGV this state
#include "DMProcedures.h"
#include "SMContinueTrans.h"
#include "SMContextHelp.h"			//TAR119740	LPM100199  must return to system msg after call for help
//#include "SMStateBase.h"				//TAR119740	LPM100199  must return to system msg after call for help


DLLEXPORT extern DMProcedures dm;             // global DM object

#define COMP_ID ID_SM               // base state component
#define T_ID _T("HelpOnWayBase")

IMPLEMENT_DYNCREATE(SMHelpOnWayBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(HelpOnWay)


DEFINE_TIMESTAMP

//////////////////////////
SMHelpOnWayBase::SMHelpOnWayBase()
{
  IMPLEMENT_TIMESTAMP
}

//////////////////////////////////////
// Initialize
//////////////////////////////////////
SMStateBase *SMHelpOnWayBase::Initialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("HelpOnWay") );	// RFC 330050

   //Tar 120846
  //setAnchorState(BES_HELPSCREEN);

	if (fFromRequestSignature) 
	{
		nTimeOut = 0;					 //TAR#140156 -- turn timeout feature off  5/9/00 LMP and YW
		fFromRequestSignature = false;
	}
    // NewUI
    ps.SetCMFrame(_T("HelpOnWay"));
    ps.CMButtonFlash(_T("CMButton1"),true);  
	ps.ShowCMTBText(csTBMessage2Scot);	//thp
	ps.Message12(MSG_HELPONWAY, true);
    // End NewUI

	DMSayPhrase(HELPONTHEWAY);

	if (!fCantScanCoupon)	// TAR 238422 only change light and help request event when not coming from Can't Scan Coupon
	{
		dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
						  DM_TRICOLORLIGHT_BLINK_1HZ,
						  DM_TRICOLORLIGHT_NORMALREQUEST);

		// remote approval
		CString csAnchorStateName = getSCOTStateName();
		CString csDesc, csInstruction;
		csDesc = ps.GetPSText(RA_CALL_FOR_HELP_ID, SCOT_LANGUAGE_PRIMARY);
		csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(BTT_HELP, SCOT_LANGUAGE_PRIMARY), csAnchorStateName, co.IsSecurityDisableOn()==true ? ps.GetPSText(RA_SECONDLINE_SECURITYDISABLED, SCOT_LANGUAGE_PRIMARY) : _T(""), ps.GetPSText(RA_THIRDLINE_CALLFORHELP, SCOT_LANGUAGE_PRIMARY));
		trace(L7, _T("+ra.OnCallForHelp, Message=%s"), csInstruction);
		ra.OnCallForHelp(csInstruction);	  
		trace(L7, _T("-ra.OnCallForHelp"));
	}
	ps.ShowCMFrame(false);
    return STATE_NULL; // stay in HelpOnWay state
}

////////////////////////////////////////
SMStateBase *SMHelpOnWayBase::PSButtonHelp(void)
{
  return STATE_RETURN;              // return to prev state
}

/////////////////////////////////////
SMStateBase *SMHelpOnWayBase::PSButton1(void) // ok
{
  bPressHelpOnWay = true;   //Tar 164037 return to Help need screen, then return to the previous screen
  if (fSAInItemSale)
  {
	// sale pending, cancel the pending item sale
	if ((anchorState != BES_SELECTCONTAINER) && 
		(anchorState != BES_SELECTTARE)      &&
		(anchorState != BES_ENTERQUANTITY)   &&
		(anchorState != BES_ENTERWEIGHT)	 &&
		(anchorState != BES_CRATEABLEITEM)	 &&
		(anchorState != BES_ENTERWTFORPRICEEMBEDDED))     
		//Tar 154147
		SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
  }

  
  return STATE_RETURN;
	
}

//////////////////////////////////////
SMStateBase *SMHelpOnWayBase::PSEnterKey(void)// Yes
{
  DMSayPhrase(KEYPADVALID);
  return PSButton1();
}

/////////////////////////////////////////////
SMStateBase *SMHelpOnWayBase::TimedOut(void)
{
   // Fix for TAR 153465
  if (SCOTIsMinimized())
    return STATE_NULL;
  else
    RETURNSTATE(ContinueTrans)
}
// + RFC 330050
void SMHelpOnWayBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("HelpOnWay") );	
}
// - RFC 330050
