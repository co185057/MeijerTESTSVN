//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	 SMElectronicBenefitsTransferBase.CPP
//
// TITLE:	Class implementation for Electronic Benefits Transfer State
//
//
// AUTHOR:   Michael Prather
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>				 // MS MFC always needed first
//			   // MS MFC template header

#include "Common.h"				 // MGV common includes

#include "SMState.h"				// MGV base state
#include "SMElectronicBenefitsTransfer.h"	  // MGV this state
#include "SMEnterPin.h"			 // MGV enter PIN state
#include "SMSelectCardType.h"	   // MGV select card type
#include "SMConfirmEBTAmount.h"	   //
#include "SMGetFoodStampState.h"	   //
#include "SMSelectPayment.h"	   //
#include "SMTakeCard.h"             // Take your card

#define COMP_ID ID_SM			   // base state component
#define T_ID	_T("ElectronicBenefitsTransferBase")

IMPLEMENT_DYNCREATE(SMElectronicBenefitsTransferBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(ElectronicBenefitsTransfer)

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMElectronicBenefitsTransferBase::SMElectronicBenefitsTransferBase(const bool fCshBck)
: fCashBack(fCshBck),
  bShowFoodStamps(false)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMElectronicBenefitsTransferBase::SMElectronicBenefitsTransferBase()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMElectronicBenefitsTransferBase::Deliver(const bool fCshBck)
{
  fCashBack = fCshBck;
  bShowFoodStamps = false;

  return this;
}

////////////////////////////////////////
bool SMElectronicBenefitsTransferBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    // There should be one parameter, a bool (word)
    if (dlParameterList.GetDescriptorCount() == 1)
    {
        trace(L6, _T("+SMElectronicBenefitsTransferBase::DeliverDescriptorList() - Parameter Count of 1"));
        Deliver((bool)DLParamWORD(dlParameterList));            // fCshBck
        return true;
    }
    else
    {
        trace(L2, _T("+SMElectronicBenefitsTransferBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}

//////////////////////////////////////////
SMStateBase  *SMElectronicBenefitsTransferBase::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("ElectronicBenefitsTransfer") );	// RFC 330050
  //Update RAP window with SCOT state
  UpdateStateName(ps.GetPSText(LTT_EBTTRANSFER, SCOT_LANGUAGE_PRIMARY));
  ResetAllTenderValues();
  DMSayPhrase(SELECTTYPE);	//TAR#117134	LPM081799

  if((lFoodStampsDue > 0) && (co.fTenderEBTFSAllowed))
	bShowFoodStamps = true;
	//TAR#117136	LPM081899  not used 
	//PSTEXT Btn1 = (bShowFoodStamps?BTT_FOOD_STAMPS:PS_NOTEXT);	
	//PSTEXT Btn2 = (co.fTenderEBTCBAllowed?BTT_EBTCASH:PS_NOTEXT);
	PSTEXT MsgText = (bShowFoodStamps?MSG_EBTTYPE:MSG_EBTNOFSTYPE);

	//@@@INFOPOINT

	ps.SetCMFrame(_T("ElectronicBenefitsTransfer"));
		//begin		TAR#117136	button should be visible and disabled, not hidden
    if (!bShowFoodStamps) 
		ps.CMButton(_T("CMButton1Med"), BTT_FOOD_STAMPS, false);  
	else
		ps.CMButton(_T("CMButton1Med"), BTT_FOOD_STAMPS, true);  

	if (!co.fTenderEBTCBAllowed) 
		ps.CMButton(_T("CMButton1Med"), BTT_EBTCASH, false);  
	else
		ps.CMButton(_T("CMButton1Med"), BTT_EBTCASH, true);  
	//end		TAR#117136	LPM081899

    ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
	//@@@INFOPOINT
	
  DisplayAuxScreen(bShowFoodStamps ? AUX_SCREEN_EBT_TYPE : AUX_SCREEN_EBT_NO_FS_TYPE);

	ps.Message12(ps.GetPSText(MsgText), true);
	ps.ShowCMFrame();
	return STATE_NULL;
}

/////////////////////////////////////////
SMStateBase  *SMElectronicBenefitsTransferBase::PSButton1(void)// Food Stamps
{
  	if( co.fOperationsPartialIntegratedPINPad && !co.fOperationsFoodStampsMSR)   //PIP
	{
		if (!co.fOperationsPIPSCOTPromptConfirmTotal) 			 
		{
			long nSubmitTotal = lFoodStampsDue;
			if( lBalanceDue < lFoodStampsDue) // customer has already paid part of balance off  //Tar 154631
			{
				// safeway fix, lFoodStampsDue = lBalanceDue; //gets passed in as the available EBT balance
				if (!co.fTenderSubmitFSTtotalIfMorethanTrxTotal)
				{
					// do not submit food stamp total, instead, submit balancedue to not let customer
					// get change for his/her coupons
					nSubmitTotal = lBalanceDue;
				}
			}
			return TBEnterTender(TB_TENDER_PIP_FOODSTAMPS,nSubmitTotal);		  
		}
		else
		{
			CREATE_AND_DISPENSE(ConfirmEBTAmount)(TB_TENDER_PIP_FOODSTAMPS,lFoodStampsDue,0 );
		}
	}


  if(co.fTenderEBTFSNeedsPIN && (!co.fOperationsPartialIntegratedPINPad || !co.fOperationsFoodStampsMSR))  //Tar 154518
  {
	CREATE_AND_DISPENSE(EnterPin)(TB_TENDER_FOODSTAMPS,false); // false used to be co.fTenderEBTFSCashBack
  }

//TAR#117347 removing food stamps cash back option.  
//if (co.fTenderEBTFSCashBack)
//  CREATE_AND_DISPENSE(SelectPayment)(TB_TENDER_FOODSTAMPS);		  //With cash back
//else
  CREATE_AND_DISPENSE(ConfirmEBTAmount)(TB_TENDER_FOODSTAMPS,lFoodStampsDue,0 );

}

/////////////////////////////////////////
SMStateBase  *SMElectronicBenefitsTransferBase::PSButton2(void)// EBT Cash Benefit
{
   if(co.fOperationsPartialIntegratedPINPad && !co.fOperationsEBTMSR)   //PIP
	{
		if (co.fTenderEBTCBCashBack && co.fOperationsPIPSCOTPromptCashBack)
		{
			CREATE_AND_DISPENSE(SelectPayment)(TB_TENDER_EBT);
		}
			
		if(!co.fOperationsPIPSCOTPromptConfirmTotal)			 
		{
			return TBEnterTender(TB_TENDER_PIP_EBT,lBalanceDue);		  
		}
		else
		{
			CREATE_AND_DISPENSE(ConfirmEBTAmount)(TB_TENDER_EBT,lBalanceDue,0 );
		}
	}


  if (co.fTenderEBTCBNeedsPIN && (!co.fOperationsPartialIntegratedPINPad || !co.fOperationsEBTMSR))
  {
    CREATE_AND_DISPENSE(EnterPin)(TB_TENDER_EBT,co.fTenderEBTCBCashBack);
  }

  if (co.fTenderEBTCBCashBack)
  {
    CREATE_AND_DISPENSE(SelectPayment)(TB_TENDER_EBT);		  //With cash back
  }
  else
  {
    CREATE_AND_DISPENSE(ConfirmEBTAmount)(TB_TENDER_EBT,lBalanceDue,0 );
  }
}

/////////////////////////////////////////
SMStateBase  *SMElectronicBenefitsTransferBase::PSButtonGoBack(void)// Cancel
{
    setAnchorState(BES_FINISH);
	//Tar 146185 goto take card, to eject the card and return to the base state
	CREATE_AND_DISPENSE(TakeCard)(false) ;
 // RETURNSTATE(GetFoodStampState);
}

///////////////////////////////////////////
SMStateBase  *SMElectronicBenefitsTransferBase::PSCancelKey(void)// cancel
{
  DMSayPhrase(KEYPADVALID);
  return PSButtonGoBack();			  
}
///////////////////////////////////////////////////////////////////////////
SMStateBase *SMElectronicBenefitsTransferBase::OnWtIncreaseNotAllowed()	 
{
	return FinalizationWtIncrease(BES_STATERETURN);	
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMElectronicBenefitsTransferBase::OnWtDecrease()
{
	if(!g_bIsForgiveUnexpectedDecreaseDuringTendering && !(lBalanceDue < 0.01))
		return FinalizationWtDecreaseNotAllowed(BES_STATERETURN);
	else
		return STATE_NULL;
}
// + RFC 330050
void SMElectronicBenefitsTransferBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ElectronicBenefitsTransfer") );	
}
// - RFC 330050
