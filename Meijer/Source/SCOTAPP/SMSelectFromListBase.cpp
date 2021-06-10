//
// FILE:  SMSelectFromListBase.cpp
//
// TITLE: Class implementation for Select From List state
//
// AUTHOR:   Feng Zhang
//
//////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#ifndef _CPPUNIT
#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMEnterQuantity.h"
#include "SMPLAEnterWeight.h"          //	enter wgt state

#else  // _CPPUNIT
#endif // _CPPUNIT

#include "SMSelectFromList.h"   // This state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SelectFromList")

IMPLEMENT_DYNCREATE(SMSelectFromListBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SelectFromList)

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSelectFromListBase::SMSelectFromListBase()
{
  IMPLEMENT_TIMESTAMP
#ifndef _CPPUNIT
  saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;
#endif // _CPPUNIT
}

#ifndef _CPPUNIT
///////////////////////////////////////////
SMStateBase  *SMSelectFromListBase::Initialize(void)
{
  //Update RAP window with SCOT state
  UpdateStateName(ps.GetPSText(LTT_SELECTITEM, SCOT_LANGUAGE_PRIMARY));  
  // we need to go to look up first to reach this state
  setAnchorState(BES_SELECTFROMLIST); // TAR 164000
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SelectFromList") );	// RFC 330050

  if(co.fStateGraphicalSelectList)
#ifndef NewUI
//Not sure to remove this
    ps.SetCMFrame(PRODUCE_FRAME);

  else

    ps.SetCMFrame(SCROLL_FRAME); 
#endif
  
  if(!co.fStateGraphicalSelectList)
    ps.SetCMMascotImage(BMP_MASCOT_SELECTFROMLIST);
  
  ps.SetCMGobackButtonVisible(true);
  
  if(co.fStateGraphicalSelectList)
    ps.SetCMBackground(BMP_GRAPHICALSELECT);
  else
	 ps.SetCMBackground(BMP_SELECTFROMLIST);

  ps.SetCMLeadthruText(LTT_SELECTITEM);

  ps.ShowCMFrame();

  ps.DisplaySelectedItem(csLookupItemCode);
  DMSayPhrase(SELECTFROMLIST);

  ps.ShowWideList();
  if(!co.fStateGraphicalSelectList)
  {
    ps.Message12(TXT_SELECTFROMLIST1, false);  
    ps.ShowCMTotal(false ,true);
	ps.ShowCMTBText(csTBMessage2Scot);	//thp
  }
  // allow the EchoInput() func to accept up to max digits
  ps.AllowInput(MAX_DIGITS_ITEMCODE,false,false);

  if(co.fStateGraphicalSelectList)
    ps.ShowVideoProduce();
  else
    ps.ShowVideoSmall();
  SetFocusToVideo();            // TAR #154963. 

  SAWLDBScannerDisable();		// Disable the scanner

  return STATE_NULL;                // stay in SelectFromList state
}

///////////////////////////////////////////
void SMSelectFromListBase::UnInitialize(void)
{
  if(co.fStateGraphicalSelectList)
	  ps.HideVideoProduce();
  else
	  ps.HideVideoSmall();
	// BeginCodeWLDBSA11
	//SAWLDBScannerDisable();			// Disable the scanner
	// EndCodeWLDBSA11
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SelectFromList") );	// RFC 330050
}

///////////////////////////////////////////
SMStateBase  *SMSelectFromListBase::PSButton3(void)// start
{
  return STATE_NULL;                // stay in SelectFromList state
}

///////////////////////////////////////////
SMStateBase  *SMSelectFromListBase::PSButton4(void)// cancel
{
  return setAndCreateAnchorState(nSellState);  //TAR#126112 return to last sell state instead of Scan and Bag  LPM111599
}

///////////////////////////////////////////
SMStateBase  *SMSelectFromListBase::PSButtonGoBack(void)// cancel
{
    //  TAR 164000 -  return createAnchorState(); //TAR# 137321 -- should return to Type Item Description screen
  return setAndCreateAnchorState(BES_LOOKUPITEM); // TAR 164000
}

#endif //_CPPUNIT
///////////////////////////////////////////
SMStateBase  *SMSelectFromListBase::DMScanner(void)// scanned item
{
  csItemSent = csDMLastBarCode;
  if (co.IsPicklistAssistEnable() && ps.IsPickListItem(csDMLastBarLabel))
  {
      io.Reset();
      csItemSent = csDMLastBarLabel;
      io.csOrigCode = csItemSent;
      io.csItemCode = csItemSent;
      RETURNSTATE(PLAEnterWeight)
  }
  else
  {
      return TBItemSale(FALSE,0,true);
  }
}
///////////////////////////////////////////
#ifndef _CPPUNIT
SMStateBase  *SMSelectFromListBase::PSNumericKey(const long lKey)
{
  PSEchoInput(lKey);
  return STATE_NULL;
}

///////////////////////////////////////////
SMStateBase  *SMSelectFromListBase::PSEnterKey(void) // enter key pressed, sell this item
{
  long lLen = ps.GetInput(csItemSent);
	
  if (lLen==0)
  {
    DMSayPhrase(KEYPADINVALID);
    return PSClearKey();
  }
	
  DMSayPhrase(KEYPADVALID);

 	// BeginCodeWLDBSA11
	SAWLDBScannerDisable();							  // Disable the scanner--Feng
	// EndCodeWLDBSA11

  return TBItemSale(FALSE,0,false);
}

///////////////////////////////////////////
SMStateBase  *SMSelectFromListBase::PSClearKey(void)
{
  //PSClearInput();
  long lKey=0x08;
  PSEchoInput(lKey);	//Delete one character once--Feng
  return STATE_NULL;
}

#endif //_CPPUNIT
///////////////////////////////////////////
//This is the new lookup --Feng
SMStateBase  *SMSelectFromListBase::PSListLookup(CString cItemCode) // list or lookup selection
{
#ifndef _CPPUNIT
  // GetListWide returns an int, not a UINT and -1 means failure which is 
  // completely lost if we cast the return to UINT  ncr_barn
  int nSelect = ps.GetWideList(csItemSent); //Selected item //Feng
	
  csItemSent.TrimRight();
	
  // Check for failure of nSelect also (<=)  ncr_barn
  if (nSelect<=0 || csItemSent==_T("")) //fail in picking list
  {
    DMSayPhrase(PICKLISTINVALID);
    return STATE_NULL;
  }
#endif //_CPPUNIT
  //double boink pd DMSayPhrase("PickListValid");   //Play a "PickListValid" wav file. --Feng
  if (VerifyQtyItem(csItemSent))     //Tar 143309 
  {
      io.Reset();
	  io.csOrigCode = csItemSent;	   // item code keyed
	  io.csItemCode = csItemSent;	   // assumed to be the same
      if (!co.IsPicklistAssistEnable()) // dont Send SASendScot when PLA is enable, Let TBItemSale called by PLAEnterWeight do
      {
          SASendSCOTInput( UPC_MANUAL, csItemSent);
      }
      SetNextGenUICurrentItem(io.csOrigCode);
	  RETURNSTATE(EnterQuantity)
  }
  if (co.IsPicklistAssistEnable() && ps.IsPickListItem(csItemSent))
  {
      io.Reset();
      io.csOrigCode = csItemSent;
      io.csItemCode = csItemSent;
      RETURNSTATE(PLAEnterWeight)
  }
  else
  {
      return TBItemSale(FALSE,0,false);         //Sell item
  }
}

#ifndef _CPPUNIT
///////////////////////////////////////////
SMStateBase  *SMSelectFromListBase::PSCancelKey(void)/// override where needed
{
  DMSayPhrase(KEYPADVALID);
  return PSButton4();               // usually same as button 4
}

#endif // _CPPUNIT