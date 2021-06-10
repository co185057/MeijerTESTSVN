//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMUnknownItemBase.cpp
//
// TITLE: Class implementation for Unknown Item State
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header
#ifndef _CPPUNIT
#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMUnknownItem.h"      // This state
#include "DMProcedures.h"
#include "SMPickingUpItems.h"
#else
#include "SMUnknownItemBase.h"

#endif //_CPPUNIT
#ifndef _CPPUNIT
DLLEXPORT extern DMProcedures dm;   // Global DM object
#endif

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("UnknownItemBase")

IMPLEMENT_DYNCREATE(SMUnknownItemBase, CObject)// MFC Runtime class/object information

//pIMPLEMENT_STATE(UnknownItem)
#ifndef _CPPUNIT
bool SMUnknownItemBase::m_bFromEnterCoupons = false; //dp185016 tar 286763
#endif

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMUnknownItemBase::SMUnknownItemBase()
{
  IMPLEMENT_TIMESTAMP
}

///////////////////////////////////////
SMStateBase *SMUnknownItemBase::Initialize(void)
{
#ifndef _CPPUNIT

  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("UnknownItem") );

  //SSCOADK-6076 +
  if (lUnknownItems == 0)
  {
      //no unknown items so return PSButton1()
      return PSButton1();
  }
  //SSCOADK-6076 -
	
  // +TAR 398289
  if (fInMultiSelectPickList)
  {
      g_bMultiPickSkipBaggingAllowed = true;
  }
  // -TAR 398289
  CString csAnchorStateName, csDesc, csInstruction;
  if (getAnchorState() == BES_ENTERCOUPONS)
    {m_bFromEnterCoupons = true;} //dp185016 tar 286763
  //+tar410912
  if( !io.cScanType && 
      (getAnchorState() == BES_SECUNEXPECTEDINCREASE || 
       getAnchorState() == BES_SECUNEXPECTEDDECREASE ||
       getAnchorState() == BES_TRANSPORTITEM) )	
  {
    lUnknownItemsKeyed--;
  }
  //-tar410912
  setAnchorState(BES_UNKNOWNITEM);	//tar 239365
  csAnchorStateName = getSCOTStateName();

  if(io.fNotForSale) // TAR 205236
  {
    csDesc = ps.GetPSText(LTT_UNSELLABLEITEM, SCOT_LANGUAGE_PRIMARY);	  // remote approval
    csAnchorStateName = ps.GetPSText(LTT_UNSELLABLEITEM, SCOT_LANGUAGE_PRIMARY);
    csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_LANEBTT_TIMERESTRICTEDITEM, SCOT_LANGUAGE_PRIMARY), csAnchorStateName, 
		co.IsSecurityDisableOn()==true || co.GetSecurityDisableSubCheckOn() ? ps.GetPSText(RA_SECONDLINE_SECURITYDISABLED, SCOT_LANGUAGE_PRIMARY) : _T(""), ps.GetPSText(RA_LANEBTT_TIMERESTRICTEDITEM, SCOT_LANGUAGE_PRIMARY));
  }
  else
  {
    csDesc = ps.GetPSText(RA_SCANNED_UNKNOWN_ID, SCOT_LANGUAGE_PRIMARY);	  // remote approval
    csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_LANEBTT_RECALLEDITEM, SCOT_LANGUAGE_PRIMARY), csAnchorStateName, 
		co.IsSecurityDisableOn()==true || co.GetSecurityDisableSubCheckOn() ? ps.GetPSText(RA_SECONDLINE_SECURITYDISABLED, SCOT_LANGUAGE_PRIMARY) : _T(""), ps.GetPSText(RA_THIRDLINE_RECALLEDITEM, SCOT_LANGUAGE_PRIMARY));
  }
  DisplayAuxScreen(AUX_SCREEN_USE_TOUCHSCREEN, true);	// TAR 226994
 
  if (io.cScanType!=0 || io.d.bPickListItem)
  {
     if (lUnknownItems > 0) // TAR 243866 and if Unknown item has not been approved
	 {
        //TAR#120988	Keyed unknown item should not turn on red light on 1st occurance
	    //				Scanned should turn the light red, tho.  LPM091499
	    
        // (*) SR898 ====================
        DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                           DM_TRICOLORLIGHT_BLINK_1HZ,
                           DM_TRICOLORLIGHT_NORMALREQUEST);
                           
	    ra.OnUnknownItem(csInstruction, FALSE);
	 } 
  }
  else
  {
	// TAR#120988 Unknown item approval on RAP side needs to reset the globals to zero  LPM111299
	/*SCOTDO_APPROVAL_INFO ApprovalInfo;
	ra.GetApprovalInfo(&ApprovalInfo);
	if ((lUnknownItemsKeyed > 2) && (!ApprovalInfo.bKeyedUnknown))
	{
		lUnknownItemsKeyed = 0;
		lUnknownItems = 1;
	}*///SSCOP-1670
	
	//TAR#120988	Keyed unknown item should not turn on red light on 1st occurance
	lUnknownItemsKeyed++;
    if(co.IsAttendantModeOn() && co.IsImmedInterventionOn())
    {
        lUnknownItemsKeyed = 0;
        lUnknownItems = 0;
    }
    else if(lUnknownItemsKeyed > 1 || io.xd.ExceptionCode == XDONOTPROCESS)
	// keyed exception items will turn on red light on 1st occurance
	{
		// (*) SR898 ====================
        DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                           DM_TRICOLORLIGHT_BLINK_1HZ,
                           DM_TRICOLORLIGHT_NORMALREQUEST);
                           
	    ra.OnUnknownItem(csInstruction, TRUE);
	}
	else if (lUnknownItemsKeyed == 1 && lUnknownItems != 0 )
		lUnknownItems--;

	
  }

  if (co.IsInAssistMenus())
  {
      return PSButton1();
  }
  // Play wave file first, based on certain conditions
  if(io.xd.ExceptionCode == XDONOTPROCESS)
  	DMSayPhrase(EXCEPTIONITEM); 
  else if(io.fNotForSale)
  	DMSayPhrase(SETITEMASIDECONTINUEWITHPURCHASES);
  else if (io.cScanType!=0 || io.d.bPickListItem)
  {
    DMSayPhrase(UNKNOWNITEMSETASIDETOUCHOK);
  }
  else if (lUnknownItemsKeyed == 1)
    DMSayPhrase(UNKNOWNITEM);
  else 
    DMSayPhrase(UNKNOWNITEMSETASIDETOUCHOK);
     
// NewUI
  ps.SetCMFrame(_T("UnknownItem"));
  // Set lead thru based on certain conditions
  if(io.xd.ExceptionCode == XDONOTPROCESS)
	ps.SetLeadthruText(LTT_EXCEPTIONITEM);
  else if(io.fNotForSale)
	ps.SetCMLeadthruText(LTT_UNSELLABLEITEM);
  else if (io.cScanType!=0 || io.d.bPickListItem)
  {
    ps.SetCMLeadthruText(LTT_UNKNOWNITEM);
  }
  else if (lUnknownItemsKeyed == 1)
	ps.SetCMLeadthruText(LTT_UNKNOWNITEMCODE);
  else 
	ps.SetCMLeadthruText(LTT_UNKNOWNITEM);
  ps.CMButtonFlash(_T("CMButton1Med"),true);
// NewUI

  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
  ps.ShowCMTBText(csTBMessage2Scot);	//thp
		  
  if(io.xd.ExceptionCode == XDONOTPROCESS)
  {
    CString csMsg;
	csMsg.Format(ps.GetPSText(MSG_SETASIDEEXCEPTION),io.xd.csCategory);
	ps.Message12(csMsg, true);
  }
  else if(io.fNotForSale)
	ps.Message12(MSG_UNSELLABLEITEM, true);
  else if (io.cScanType!=0 || io.d.bPickListItem)
  {
    ps.Message12(MSG_UKITEMSCANNED, true);
  }
  else if (lUnknownItemsKeyed == 1)
	ps.Message12(MSG_UKITEMKEYED, true);
  else
	ps.Message12(MSG_UKITEMSCANNED, true);

#endif// _CPPUNIT
  if (g_bInRecallMode)
  {
      if(io.xd.ExceptionCode == XDONOTPROCESS)
          SetInRecalledModeReceiptMessage(LTT_EXCEPTIONITEM, MSG_SETASIDEEXCEPTION);
      else if(io.fNotForSale)
          SetInRecalledModeReceiptMessage(LTT_UNSELLABLEITEM, MSG_UNSELLABLEITEM);
      else
          SetInRecalledModeReceiptMessage(LTT_UNKNOWNITEMCODE, MSG_UKITEMSCANNED);

      ps.ButtonState(_T("ButtonHelp"), false, false);
  }
  ps.ShowCMFrame();

  return STATE_NULL;                // stay in UnknownItem state
}

#ifndef _CPPUNIT

void SMUnknownItemBase::UnInitialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("UnknownItem") );
	// don't need to do this. It creats problems for the derived classes  
	// ps.CMButtonFlash(IDC_BUTTON1,false);
}

///////////////////////////////////////
SMStateBase *SMUnknownItemBase::TimedOut(void)
{
    if (g_bInRecallMode)
    {
        trace(L6, _T("SMUnknownItemBase::TimedOut in recall mode, pressing OK button"));
        return PSButton1();
    }

    if (lUnknownItemsKeyed > 0)  //check to be sure we haven't cleared via RAP
	    lUnknownItemsKeyed--;	// If the screen times out, it will return to this initialize, so this var has to be decremented LPM030201
    return STATE(State)::TimedOut();
}

//////////////////////////////////////////
SMStateBase  *SMUnknownItemBase::PSButtonHelp(void)
{
    if (lUnknownItemsKeyed > 0)  //check to be sure we haven't cleared via RAP
    	lUnknownItemsKeyed--;	// If the help is press, it will return to this initialize, so this var has to be decremented LPM030201
	return STATE(State)::PSButtonHelp();
}

/////////////////////////////////////
SMStateBase *SMUnknownItemBase::PSButton1(void) // OK Button
{
  //+dp185016 tar 286763
  if( m_bFromEnterCoupons )
  {
    m_bFromEnterCoupons = false;
    return setAndCreateAnchorState(BES_ENTERCOUPONS);
  }
  //-dp185016

  if (fInMultiSelectPickList)
	  return setAndCreateAnchorState( BES_PRODUCEFAVORITES );
  else
  {

	  return setAndCreateAnchorState(BES_SCANANDBAG);
  }
}

////////////////////////////////////////
SMStateBase *SMUnknownItemBase::PSEnterKey(void) // OK for scanned items
{
  DMSayPhrase(KEYPADVALID);
  return PSButton1();          
}

#endif
SMStateBase *SMUnknownItemBase::OnWtDecrease()
{
	if (co.IsSecurityBypassOn()	)
	{
		OverrideSecurityException(_T("Lane"), KEY_EXCEPTION_OPERATION_CLEAR);
	}
	else
	{
		RETURNSTATE(PickingUpItems)
	}
    return STATE_NULL;
}
