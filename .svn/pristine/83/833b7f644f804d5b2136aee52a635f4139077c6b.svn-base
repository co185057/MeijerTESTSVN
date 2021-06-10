//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMEnterQuantityBase.CPP
//
// TITLE: Class implementation for enter quantity state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header
#ifndef _CPPUNIT
#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMInProgress.h"           // In progress state
#include "SMRapDataNeeded.h"
#include "SMRapDataNeededBase.h"
#include "SMPLAEnterWeight.h"          //	enter wgt state

#else  // _CPPUNIT
#endif // _CPPUNIT

#include "SMEnterQuantity.h"      

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("EnterQuantityBase")

IMPLEMENT_DYNCREATE(SMEnterQuantityBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(EnterQuantity)


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMEnterQuantityBase::SMEnterQuantityBase()
{
  IMPLEMENT_TIMESTAMP
#ifndef _CPPUNIT
  saMsgFuncPtr = PSProceduresBase::SecurityMessageModal;
#endif // _CPPUNIT
}

#ifndef _CPPUNIT
/////////////////////////////////////////
SMStateBase  *SMEnterQuantityBase::Initialize(void)
{
  if(!fInMultiSelectPickList)//SR679
  {
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("EnterQuantity") );
  }
  //Update RAP window with SCOT state
  UpdateStateName(ps.GetPSText(LTT_HOWMANYBUY, SCOT_LANGUAGE_PRIMARY));
  setAnchorState(BES_ENTERQUANTITY);  //TAR#134884 return to this state from help on way
  //+tar 330491
  m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("EnterQuantity"), UI::PROPERTYSTATE, (long)UI::Normal); 
  //-tar 330491
  ra.RequestEnableAssistMode(false); // TAR 297347
  if (io.fVoid && lVoidQty>0)
  {
    io.lQuantityEntered = lVoidQty;
    lVoidQty = 0;
    return TBEnterItem();
  }
  
  PSTEXT nLtt;
  if (io.fVoid)
	  nLtt = LTT_HOWMANYCANCEL;
  else
	  nLtt = LTT_HOWMANYBUY;

  // if need to have numeric keypad.

    saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;

    ps.SetCMFrame(_T("EnterQuantity"));
    ps.SetCMBackground(BMP_TYPEITEMDESC);
    ps.SetCMLeadthruText(nLtt);
	ps.ShowNumericKeypad();
    ps.Message12(MSG_ENTERQUANTITY, false);


  ps.SetCMMascotImage(BMP_MASCOT_ATMKEY);

  ps.SetCMGobackButtonVisible(true);
  DMSayPhrase(QUANTITYNEEDED);

  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
  ps.ShowCMTBText(csTBMessage2Scot);	//thp

  // allow the EchoInput() func to accept up to max digits
  ps.AllowInput(MAX_DIGITS_QUANTITY,false,false);
  ps.ShowCMFrame();
  return STATE_NULL;                // stay in EnterQuantityBase state
}

////////////////////////////////////////
SMStateBase  *SMEnterQuantityBase::PSButton1(void) // OK
{
  return PSEnterKey();              // same as enter key
}

////////////////////////////////////////
SMStateBase  *SMEnterQuantityBase::PSButton3(void) // cancel all
{
	// BeginCodeLocaleUPC
	SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
	// EndCodeLocaleUPC			


	//return setAndCreateAnchorState(nSellState);	// return to last sell state	

	if (fInMultiSelectPickList)
	{
  // +TAR 407200
		g_bMultiPickSkipBaggingAllowed = true;
  // -TAR 407200
		g_lNumberOfItemsNotBagInMultiPick--;
		return setAndCreateAnchorState( BES_PRODUCEFAVORITES );
	}
	//TAR#117107 return to scan and bag, not last state
	else
	{
		return setAndCreateAnchorState(BES_SCANANDBAG);	// TAR#125791 fix for 117107	LPM081999
	}
}

////////////////////////////////////////
//@@@INFOPOINT
SMStateBase  *SMEnterQuantityBase::PSButtonGoBack(void) // cancel all
{
  return PSButton3();     // TAR#125791 fix for 117107	LPM081999	
}
//@@@INFOPOINT
//////////////////////////////////////////////////////
SMStateBase  *SMEnterQuantityBase::PSNumericKey(const long lKey)
{
  PSEchoInput(lKey);

  // Show OK key if qty > 0.
  CString csQty;
  long lLen = ps.GetInput(csQty);
  long lQty = _ttol(csQty);

    if (lQty==0)
      ps.EnterClearKeys(true, false);  // disable Enter, enable Clear key.  

   
  return STATE_NULL;
}

/////////////////////////////////////////
SMStateBase  *SMEnterQuantityBase::PSClearKey(void)
{


    PSClearAllInput(true);

  return STATE_NULL;
}

//+SR828
/////////////////////////////////////////////
SMStateBase  *SMEnterQuantityBase::PSBackSpaceKey(void)
{
    PSClearInput();

    bool bDisableButtons = false; //will disable/enable clear and backspace buttons
    CString csValue;
    long lLen = ps.GetInput(csValue);
      
    if (lLen == 0)
    {
        ps.ClearInput();
        bDisableButtons = true;
    }

    long lQty = _ttol(csValue);

    // disable Enter if qty is 0. 
    if (lQty==0)
        ps.EnterClearKeys(true, bDisableButtons); 

    return STATE_NULL;
}
//-SR828

#endif // _CPPUNIT
/////////////////////////////////////////
SMStateBase  *SMEnterQuantityBase::PSEnterKey(void)// enter key pressed, sell this item
{
  CString csQty;
  long lLen = ps.GetInput(csQty);

#ifndef _CPPUNIT
  if (lLen==0)
  {
    DMSayPhrase(KEYPADINVALID);
    return PSClearKey();
  }
#endif // _CPPUNIT

  long lQty = _ttol(csQty);
#ifndef _CPPUNIT
  if (lQty == 0)
  {

    DMSayPhrase(QUANTITYNEEDED);
    return PSClearKey();
  }
	
  ps.DisableButtons();
#endif // _CPPUNIT

  io.lQuantityEntered = lQty;
  io.d.fQuantityRequired = true;
  io.d.fQuantityConfirmed = false;
  if (io.lUpdateFields)
  {
#ifndef _CPPUNIT
	  itemDetailNew.lQtySold = lQty;
	   //TAR400230+
	  if (lVoidBarcodeType!=0)
	  { 
		 return TBItemSale(false, 0, true, SMStateBase::ms_bIsCurrentItemFromPickList, io.lUpdateFields);
		
      }
	  else
	  {
		 return TBItemSale(false, 0, false, SMStateBase::ms_bIsCurrentItemFromPickList, io.lUpdateFields);
		
	  }
	  //TAR400230-
#endif // _CPPUNIT
	}
  else
	  //return TBEnterItem();
  { 
	  m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("EnterQuantity"), UI::PROPERTYSTATE, (long)UI::Disabled);  //tar 330491


      if (co.IsPicklistAssistEnable() && ps.IsPickListItem(csItemSent))
      {
          g_lQty = lQty; //save qty here.
          RETURNSTATE(PLAEnterWeight)
      }
      else
      {
          return TBEnterItem();
      }
  }
}
#ifndef _CPPUNIT

//////////////////////////////////////////
SMStateBase  *SMEnterQuantityBase::PSCancelKey(void) /// override where needed
{
  DMSayPhrase(KEYPADINVALID);		//TAR#116928	LPM081199
  return STATE_NULL;
}

/////////////////////////////////////////
void SMEnterQuantityBase::UnInitialize()
{
  if(!fInMultiSelectPickList)//SR679
  {
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("EnterQuantity") );
  }
}

#endif // _CPPUNIT