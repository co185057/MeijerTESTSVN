// SMTypeJobInfo.cpp: implementation of the SMTypeJobInfo class.
//
//////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header
#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMEnterAlphaNumericInfo.h"      // This state

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("AlphaNumericInformation")

#include "PSInterface.h"            

#include "DMProcedures.h"
DLLEXPORT extern DMProcedures dm;   // Global DM object

static bool fAlpha = false;
static bool fDigit = false;

IMPLEMENT_DYNCREATE(SMEnterAlphaNumericInfoBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(EnterAlphaNumericInfo)

DEFINE_TIMESTAMP

////////////////////////////
SMEnterAlphaNumericInfoBase::SMEnterAlphaNumericInfoBase()
{
  IMPLEMENT_TIMESTAMP
  saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;
}

///////////////////////////////////////
SMStateBase  *SMEnterAlphaNumericInfoBase::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("EnterAlphaNumericInfo") );	// RFC 330050
  SMStateBase *sReturnState = CkStateStackAndReturn();
  if (sReturnState != STATE_NULL)
  {
	  setAnchorState(BES_ENTERALPHANUMERICINFO);
	  return sReturnState;
  }

  DMSayPhrase(TYPEJOBINFO);  //play typejobinformation.wav
  
  SAWLDBScannerDisable();   // Disable the scanner

  ps.SetCMFrame(_T("EnterAlphaNumericInfo"));  

  _TCHAR szWork[256] = _T("");
  _sntprintf(szWork,sizeof(szWork),ps.GetPSText(MSG_TYPEJOBINFORMATION));  //807 in scotmsge.dat
  ps.SetAlphaNumericMessage(ps.GetPSText(MSG_TYPEJOBINFORMATION));
	
  // allow the EchoInput() func to accept up to max digits
  fAlpha = false;
  fDigit = false;
  dm.PinPadDisable();       // TAR 125995

  ps.AllowInput(20,false,false);
  ps.ShowCMFrame();

  return STATE_NULL;        // stay in SelectItem state
}

/////////////////////////////////////
void SMEnterAlphaNumericInfoBase::UnInitialize(void)
{
  
  ps.PSHideAlphaNumericKeypad();
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("EnterAlphaNumericInfo") );	// RFC 330050
  
}


//////////////////////////////////////
/* @@@INFOPOINT*/
SMStateBase  *SMEnterAlphaNumericInfoBase::PSButton1(void)// cancel
{
	return STATE_NULL;
}

//////////////////////////////////////
SMStateBase  *SMEnterAlphaNumericInfoBase::PSButtonGoBack(void)// cancel
{
  long lLen = ps.GetInput(csItemSent);
	
  if (lLen==0)
  {
	fAlpha = false;
	fDigit = false;
    return PSClearAllInput();
  }
  //TBSendJobInformation(csItemSent);
  return STATE_NULL;
 
}

//////////////////////////////////////////////////
SMStateBase  *SMEnterAlphaNumericInfoBase::PSNumericKey(const long lKey)
{

	PSEchoInput(lKey);

	long lLen = ps.GetInput(csItemSent);
	
	return STATE_NULL;
}

//////////////////////////////////////////////////
SMStateBase  *SMEnterAlphaNumericInfoBase::PSAlphaKey(const long lKey)
{

	ps.EchoInput((_TCHAR)lKey, true);	// allow alpha chars in input

	long lLen = ps.GetInput(csItemSent);

    return STATE_NULL;		  	    //remain in same state
}

//////////////////////////////////////////////////
//TAR247048
SMStateBase  *SMEnterAlphaNumericInfoBase::PSAlphaNumericKey(const long lKey)
{

	ps.EchoInput((_TCHAR)lKey, true);	// allow alpha chars in input

	long lLen = ps.GetInput(csItemSent);

    return STATE_NULL;		  	    //remain in same state
}

///////////////////////////////////////
SMStateBase  *SMEnterAlphaNumericInfoBase::PSEnterKey(void) // enter key pressed, sell this item
{
  long lLen = ps.GetInput(csItemSent);
	
  if (lLen==0)
  {
	fAlpha = false;
	fDigit = false;
    DMSayPhrase(KEYPADINVALID);
    return PSClearAllInput();
  }
	 
  //TBSendJobInformation(csItemSent);
  return STATE_NULL;

}

///////////////////////////////////////
SMStateBase  *SMEnterAlphaNumericInfoBase::PSClearKey(void)
{
  PSBackSpaceInput();

  return STATE_NULL;
}

SMStateBase  *SMEnterAlphaNumericInfoBase::PSSpaceKey(void)
{
	ps.EchoInput((_TCHAR)(0x20), true);

	long lLen = ps.GetInput(csItemSent);

	return STATE_NULL;
}

/////////////////////////////////////
SMStateBase  *SMEnterAlphaNumericInfoBase::DMScanner(void)// scanned item
{
  //csItemSent = csDMLastBarCode;
  //return TBItemSale(false,0,true);
	return STATE_NULL;
}
