//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMEnterCouponValueBase.CPP
//
// TITLE: Class implementation for Enter Coupon Value state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMEnterCouponValue.h" 
#include "SMCouponTooHigh.h" 
#include "SMEnterCoupons.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("EnterCouponValueBase")

IMPLEMENT_DYNCREATE(SMEnterCouponValueBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(EnterCouponValue)


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMEnterCouponValueBase::SMEnterCouponValueBase()
{
  IMPLEMENT_TIMESTAMP
  saMsgFuncPtr = PSProceduresBase::SecurityMessageModal;
}

////////////////////////////////////////////
SMStateBase  *SMEnterCouponValueBase::Initialize(void)
{
  if(!fInMultiSelectPickList)//SR679
  {

	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("EnterCouponValue") );	// RFC 330050
  }
  if (fInMultiSelectPickList)
  {
      g_bMultiPickSkipBaggingAllowed = true;
  }

  //Update RAP window with SCOT state
  UpdateStateName(ps.GetPSText(LTT_CPNVALUE, SCOT_LANGUAGE_PRIMARY));
//  setAnchorState(BES_ENTERCOUPONVALUE); // TAR 164014
 //+tar 330491
  m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("EnterCouponValue"), UI::PROPERTYSTATE, (long)UI::Normal);
  //-tar 330491

    saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;

    ra.RequestEnableAssistMode(false); 

    ps.SetCMFrame(_T("EnterCouponValue"));
    ps.SetCMBackground(BMP_TYPEITEMDESC);
    ps.Message12(MSG_KEYCPNVALUE, false);
 

  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
  ps.ShowCMTBText(csTBMessage2Scot);	//thp
  DMSayPhrase(ENTERCOUPONVALUE);	
  fSAInItemSale = true;  // TAR 164014
	
  // allow the EchoInput() func to accept up to max digits
  ps.AllowInput(MAX_DIGITS_CPNAMT,false,true);
  ps.ShowCMFrame();
  return STATE_NULL;                // stay in EnterCouponValue state
}

////////////////////////////////////////
SMStateBase  *SMEnterCouponValueBase::PSButton1(void) // OK
{
  return PSEnterKey();              // same as enter key
}

///////////////////////////////////////////
SMStateBase  *SMEnterCouponValueBase::PSButton3(void)// cancel
{
  return createAnchorState();
}

////////////////////////////////////////
SMStateBase  *SMEnterCouponValueBase::PSButtonGoBack(void) // cancel all
{
	SASendSCOTInput( COUPON_REFUSED, io.csItemCode, 0, 0, 1);
	//TAR 192707
    //! Needed to be commented out due to double sound play from coupon context 
	//DMSayPhrase(BTTNVALID);     

    //TAR#116831 always return to scan and bag, unless
    //we came from from Scan Coupons state
/*	if (getAnchorState() != BES_ENTERCOUPONS) 
	{
		return setAndCreateAnchorState(BES_SCANANDBAG); // TAR 164014 - should go where originated.
	}
	setAnchorState(BES_EMPTY_STATE); // need to set this to play wave file on EnterCoupons, EnterCoupons resets this base state value
	RETURNSTATE(EnterCoupons)*/
	//TAR 192705 Go back to prev state
	if (fInMultiSelectPickList)
		return setAndCreateAnchorState( BES_PRODUCEFAVORITES );
	else
		return STATE_RETURN;
}

/////////////////////////////////////////////////////////
SMStateBase  *SMEnterCouponValueBase::PSNumericKey(const long lKey)
{
  CString csVal = _T("");
  long lLen = ps.GetInput(csVal);

  long lVal = 0;
  if(lLen > 0 )
  {
      lVal = _ttol(csVal);
  }

  if(0 == lLen) //this is the first character
  {
	 if(lKey == '0') //throw leading 0 keys away
	 {
		 DMSayPhrase(KEYPADINVALID);
		 return STATE_NULL;
	 }
  }

  PSEchoInput(lKey);
  
  //Commented out. No more chance to get 0 value.
  //TAR#116743	LPM081199  Show OK key if qty > 0.
  //if (lVal==0)
  //    ps.EnterClearKeys(true, false);  // disable Enter, enable Clear key.  
   
  return STATE_NULL;
}

////////////////////////////////////////////
SMStateBase  *SMEnterCouponValueBase::PSClearKey(void)
{

    PSClearAllInput(true);
  
  return STATE_NULL;
}

//+SR828
/////////////////////////////////////////////
SMStateBase *SMEnterCouponValueBase::PSBackSpaceKey()
{
    PSClearInput();

    CString csValue;
    long lLen = ps.GetInput(csValue);
    long lVal = _ttol(csValue);
    
    // Clear input and disable Enter, Clear and Backspace buttons if val is 0.
    if (lVal==0)
    {
        ps.ClearInput();
        ps.EnterClearKeys(true, true);
    }

    return STATE_NULL;
}
//-SR828

////////////////////////////////////////////
SMStateBase  *SMEnterCouponValueBase::PSEnterKey(void) // enter key pressed, sell this coupon
{
  CString csValue;
  long lLen = ps.GetInput(csValue);
	
  if (lLen==0)
  {
    return PSClearKey();
  }
  
  //TAR#116743	LPM081199  
  long lCouponValue = _ttol(csValue);
  lCouponValue *= -1;
  ASSERT(lCouponValue);   //shouldn't be 0 because enter key is disabled until it's not 0

  if (lCouponValue == 0)      //still handle the error case
  {

    return PSClearKey();
  }
	
  if (-lCouponValue>co.nOperationsMaxCoupon)
  {
 	RETURNSTATE(CouponTooHigh)
  }

  ps.DisableButtons();
  io.lUnitPriceEntered = lCouponValue;
  io.lExtPriceEntered = lCouponValue; //Tar 137876 add extended price
  io.lPriceEntered = lCouponValue;
  m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("EnterCouponValue"), UI::PROPERTYSTATE, (long)UI::Disabled); //tar 330491
  SMStateBase *retval = TBEnterItem();
  //TAR 192679
  //If coupon entry originated at Scan & Bag, go to Scan & Bag and display coupon pop-up.
  //If coupon entry originated at Select Payment Type, go to Scan & Bag and display coupon pop-up.
  //If coupon entry originated at Coupon Entry, go to Coupon Entry screen.
  if(getAnchorState() != BES_ENTERCOUPONS)
  {
      if (fInMultiSelectPickList)
          setAnchorState( BES_PRODUCEFAVORITES );
      else
          setAnchorState(BES_SCANANDBAG);
  }
  return retval;
}

/////////////////////////////////////////////
SMStateBase  *SMEnterCouponValueBase::PSCancelKey(void)/// override where needed
{
  //TAR#116743	LPM081199
  DMSayPhrase(KEYPADINVALID);		
  return STATE_NULL;

}

/////////////////////////////////////////////
void SMEnterCouponValueBase::UnInitialize()
{
  if(!fInMultiSelectPickList)//SR679
  {
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("EnterCouponValue") );	// RFC 330050
  }
    ra.RequestEnableAssistMode(true); 
}
