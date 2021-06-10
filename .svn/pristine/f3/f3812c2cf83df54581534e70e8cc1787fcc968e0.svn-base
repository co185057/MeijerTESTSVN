//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMDepositCouponsBase.cpp
//
// TITLE: Class implementation for deposit coupons state
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMFinish.h"               // Finish state
#include "SMDepositCoupons.h"      

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("DepositCouponsBase")

IMPLEMENT_DYNCREATE(SMDepositCouponsBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(DepositCoupons)


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMDepositCouponsBase::SMDepositCouponsBase()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase  *SMDepositCouponsBase::Initialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("DepositCoupons") );	// RFC 330050
	//Update RAP window with SCOT state
	UpdateStateName(ps.GetPSText(LTT_CPNDEPOSIT, SCOT_LANGUAGE_PRIMARY));
	//@@@INFOPOINT

    ps.SetCMFrame(_T("DepositCoupons"));
    ps.CMButtonFlash(_T("CMButton1Med"),true);  

	//Tar 237052
    trace(L6, _T("lBalanceDue = %d, nDMCashInserted = %d, lTotalPaid=%d"), lBalanceDue, nDMCashInserted,lTotalPaid);
    if(lBalanceDue > 0)
        ps.ShowAmountDueAmountPaid(lBalanceDue - nDMCashInserted, lTotalPaid + nDMCashInserted);
    else
    {
        long lChangeAmount = (lBalanceDue - nDMCashInserted) * -1;
        ps.ShowChangeDueAmountPaid(lChangeAmount, lTotalPaid + nDMCashInserted);
    }

	ps.ShowCMTBText(csTBMessage2Scot);	//thp
	//@@@INFOPOINT
		
	setAnchorState(BES_DEPOSITCOUPONS);	  //LP185019 added fix TAR339622	
	DMSayPhrase(DEPOSITCOUPONS);	   // only do this once at entry			
	
	if ((io.d.fDetails) && (io.nLoyaltyCardScanned))
	{
		CString csMessage12, csScannedLoyalty;
		CString csCouponDeposit = ps.GetPSText(MSG_CPNDEPOSIT);
		if (io.nLoyaltyCardScanned == 2)
		{
			// loyalty scanned but this is not the first time
			csScannedLoyalty = ps.GetPSText(MSG_SCANNEDLOYALTY_NTHTIME);
		}
		else
		{
			// loyalty scanned, this is the first time
			csScannedLoyalty = ps.GetPSText(MSG_SCANNEDLOYALTY_FIRSTTIME);
			DMSayPhrase(FREQSHOPACCT);
		}
		io.nLoyaltyCardScanned = false; // do not play this wave file again
		csMessage12 = csScannedLoyalty + csCouponDeposit;
		ps.SetPSText(PS_WORK1, csMessage12);
		ps.Message12(PS_WORK1, true); // clear the image at the background
	}
	else
	{
		ps.Message12(MSG_CPNDEPOSIT,true);
	}
	
  DMCouponLight(DM_MISC_STATE_ONE_HZ);
  ps.ShowCMFrame();
  return STATE_NULL;                // stay in DepositCoupons state
}

void SMDepositCouponsBase::UnInitialize(void)
{
  DMCouponLight(DM_MISC_STATE_OFF);
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("DepositCoupons") );	// RFC 330050
}

SMStateBase  *SMDepositCouponsBase::PSButton1(void)// ok
{
  UpdateDepositedCouponVars();
  //TAR406697+
  if ( !fInSignedOn &&  !fInSignedOff ) //TB_CLOSE has called
  {
	setAnchorState(BES_FINISH);
 	return TBEndOfTransaction();
  }
  //TAR406697-

  return setAndCreateAnchorState(BES_FINISH);
}
