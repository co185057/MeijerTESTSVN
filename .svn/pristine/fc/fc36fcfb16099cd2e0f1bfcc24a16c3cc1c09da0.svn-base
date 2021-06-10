//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMGetFoodStampStateBase.cpp
//
// Description: The GetFoodStampStateBase state is used when we want
//              to get the food stamp total from the host app.
//              Since we are trying to make a synchronous call to the
//              host app we need a state to wait in.
//
//
// Author:  Michael Prather
//
//////////////////////////////////////////////////////////////////////////////////////////////////
 
#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMGetFoodStampState.h"           // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID _T("GetFoodStampStateBase")

IMPLEMENT_DYNCREATE(SMGetFoodStampStateBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(GetFoodStampState)


DEFINE_TIMESTAMP


////////////////////////////////////////////////
SMGetFoodStampStateBase::SMGetFoodStampStateBase()
{
  IMPLEMENT_TIMESTAMP
}

//////////////////////////////////////
SMStateBase  *SMGetFoodStampStateBase::Initialize(void)
{
  //want to disable scanner, etc 
  if (!co.fTenderEBTFSAllowed)
  {
    return createAnchorState();
  }
  if (ps.DisableButtons()<0)
    return STATE_ERROR;

  //TAR218764+ CHU: 110502, if we don't do this, Weight Change event could put ScotApp and TB out of sync.
  trace(L6, _T("SMGetFoodStampStateBase::Initialize()"));
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("GetFoodStamp") );

  if(!co.fLocaleLeaveCashAcceptorsEnabledDuringTransaction) //SSCOADK-836
  {
      DMCashAcceptorDisable();
  }
  // never disable encryptor
  //DMEncryptorDisable();

  DMMSRDisable();
  //DMScaleDisable();
  DMScannerDisable();


  TBRequestFoodStampEligibleTotal();

  // We don't want this screen to show the default (or any) aux screen
  // since it is just a transition state.
  m_bNeedToDisplayAuxScreen = false;
  bUpdateFoodStampTotal = false;
	
  return STATE_NULL;
}

////////////////////////////////////
SMStateBase  *SMGetFoodStampStateBase::TimedOut(void) // timeout
{
  return createAnchorState();
}

//////////////////////////////////
// trap this message just so base doesn't give an error msg,
// nothing else to do here
SMStateBase  *SMGetFoodStampStateBase::DMScanner(void) // scanned an item
{
	
  return STATE_NULL;
}

////////////////////////////////////////
// trap this message just so base doesn't give an error msg,
// nothing else to do here
SMStateBase  *SMGetFoodStampStateBase::DMCardReader(void)
{
  return STATE_NULL;
}
///////////////////////////////////////////////////////////////////////////
SMStateBase *SMGetFoodStampStateBase::OnWtIncreaseNotAllowed()	 
{
	return FinalizationWtIncrease(BES_GETFOODSTAMPSTATE);	
}


///////////////////////////////////////////////////////////////////////////
SMStateBase *SMGetFoodStampStateBase::OnWtDecrease()
{
		if(!g_bIsForgiveUnexpectedDecreaseDuringTendering && !(lBalanceDue < 0.01))
			return FinalizationWtDecreaseNotAllowed(BES_GETFOODSTAMPSTATE);
		else
			return STATE_NULL;
}

//////////////////////////////////////////////////////////////////////////
void SMGetFoodStampStateBase::UnInitialize()
{
	//TAR218764+ CHU 110502
	// if HoldWt was already true before got in here, don't change the value of Hold Weight to false
    trace(L6, _T("+SMGetFoodStampStateBase::UnInitialize()"));
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("GetFoodStamp") );

	trace(L6, _T("-SMGetFoodStampStateBase::UnInitialize()"));
	//TAR218764-

}
