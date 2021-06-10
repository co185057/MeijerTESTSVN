//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmGetSpecialFuncValue.CPP
//
//////////////////////////////////////////////////////////////////////////////////////////////////
// Start of JZB.SSF.5.0.0
//////////////////////////////////////////////////////////////////////////////////////////////////

// This Screen was added to display a key pad and allow a store associate to enter in values.

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // common includes
#include "SMState.h"                // Base state
#include "SMSmGetSpecialFuncValue.h"	// This state
#include "SMSmSystemFunctions.h"
#include "SMSmSpecialStoreFunctions.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmGetSpecialFuncValue")

IMPLEMENT_DYNCREATE(SMSmGetSpecialFuncValue, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

extern int nSpecialFuncNum;

//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
SMSmGetSpecialFuncValue::SMSmGetSpecialFuncValue()
{
  IMPLEMENT_TIMESTAMP
}

//////////////////////////////////////////////
// Initialize
//////////////////////////////////////////////
SMStateBase  *SMSmGetSpecialFuncValue::Initialize(void)
{
  bOkToDisplayBagScaleInfo = false;
  DMScannerDisable();
  ps.SetFrame(STORE_MODE_FRAME);
  ps.SetBackground(BMP_SMBLANK);
  if (nSpecialFuncNum == SPEC_FUNC_ROA)
  {
    ps.SetTitle(LTL_ROASCREEN, PS_BLANK);
    ps.SetLeadthruText(LTT_ROASCREEN, PS_BLANK);
    ps.Prompt(TXT_ROASCREEN);
  }
  else // unknown case, blank everything
  {
    ps.SetTitle(PS_NOTEXT, PS_BLANK);
    ps.SetLeadthruText(PS_NOTEXT, PS_BLANK);
    ps.Prompt(PS_NOTEXT);
  }
  _T("SMButton5")
  ps.SetButtonText(_T("SMButton1"), PS_NOTEXT);
  ps.SetButtonText(_T("SMButton2"), PS_NOTEXT);
  ps.SetButtonText(_T("SMButton3"), PS_NOTEXT);
  ps.SetButtonText(_T("SMButton4"), PS_NOTEXT);
  ps.SetButtonText(_T("SMButton5"), PS_NOTEXT);
  ps.SetButtonText(_T("SMButton6"), PS_NOTEXT);
  ps.SetButtonText(_T("SMButton7"), PS_NOTEXT);
  ps.SetButtonText(_T("SMButton8"), BTT_GOBACK);
  ps.ShowSMMNumericKeypad();
  ps.ShowSMTotal(false);
  ps.ShowFrame();
  ps.AllowInput(MAX_DIGITS,false,false);	// allow the EchoInput() func to accept data
  return STATE_NULL;                // stay in SMSmGetSpecialFuncValue state
}

/////////////////////////////////////////////
// uninitialize
//////////////////////////////////////////////
void SMSmGetSpecialFuncValue::UnInitialize(void)
{
  bOkToDisplayBagScaleInfo = true;
}

/////////////////////////////////////////////
// PSEnterKey - enter on the pinpad
//////////////////////////////////////////////
SMStateBase * SMSmGetSpecialFuncValue::PSEnterKey()
{
  CString csValue;
  long lLen = ps.GetInput(csValue);
  if (lLen==0)		//make sure there is a value in the echo box
  {
    DMSayPhrase(KEYPADINVALID);
    return PSClearKey();
  }

  long lValue = atol(csValue);
  trace(L6,_T("JZB.SSF.5.2.0 (TBHandleReportsFunction %d, %d, NULL)"), nSpecialFuncNum, lValue);
  TBHandleReportsFunction(nSpecialFuncNum, lValue, 0);
  nSpecialFuncNum = 0;  // set back to zero when done
  ps.PSHideSMMNumericKeypad();
  ps.Prompt(PS_NOTEXT);
  PSClearInput();
  RETURNSTATE(SmSystemFunctions);
}

/////////////////////////////////////////////
// PSNumericKey - numbers on the pinpad
///////////////////////////////////////////////////////////////
SMStateBase * SMSmGetSpecialFuncValue::PSNumericKey(const long lKey)
{
  PSEchoInput(lKey);
  return STATE_NULL;
}

/////////////////////////////////////////////
// PSDecimalKey - For now, do n0t allow decimal key
///////////////////////////////////////////////////////////////
SMStateBase * SMSmGetSpecialFuncValue::PSDecimalKey(const long lKey)
{
  trace( L9, _T("+-SMSmGetSpecialFuncValue::PSDecimalKey(%d)"), lKey);

  return STATE_NULL;
}

/////////////////////////////////////////////
// PSClearKey - Clear all input (on the pinpad)
//////////////////////////////////////////////
SMStateBase * SMSmGetSpecialFuncValue::PSClearKey()
{
  PSClearAllInput();
  return STATE_NULL;
}

/////////////////////////////////////////////
// PSButton8 - Go Back
/////////////////////////////////////////////
SMStateBase  *SMSmGetSpecialFuncValue::PSButton8(void)
{
  ps.PSHideSMMNumericKeypad();
  ps.Prompt(PS_NOTEXT);
  PSClearInput();
  RETURNSTATE(SmSpecialStoreFunctions);
}
// end of JZB.SSF.5.0.0
/////////////////////////////////////////////////////////////////////