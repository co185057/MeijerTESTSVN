//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCheckBasketReminderBase.cpp
//
// TITLE: Class implementation for Check Basket Reminder State
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMCheckBasketReminder.h"      // This state
#include "SMFinish.h"
#include "SMSecUnExpectedIncrease.h"
#include "DMProcedures.h"
DLLEXPORT extern DMProcedures dm;   // Global DM object

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("CheckBasketReminderBase")

IMPLEMENT_DYNCREATE(SMCheckBasketReminderBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(CheckBasketReminder)


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMCheckBasketReminderBase::SMCheckBasketReminderBase()
{
  IMPLEMENT_TIMESTAMP
}

///////////////////////////////////////
SMStateBase *SMCheckBasketReminderBase::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("CheckBasketReminder") );
  //Update RAP window with SCOT state
  UpdateStateName(ps.GetPSText(TTL_CHECKYOURCART, SCOT_LANGUAGE_PRIMARY));
  nTimeOut = co.getTimeOutCheckBasketReminder();
  DisplayAuxScreen(AUX_SCREEN_USE_TOUCHSCREEN, true);	// 227527 - change txt on Aux display to "Use touchscreen to continue". LPM021203

  ps.SetCMFrame(_T("CheckBasketReminder"));
  // TAR 228760 - Need to take into account the cash inserted
  ps.ShowAmountDueAmountPaid(lBalanceDue - nDMCashInserted, lTotalPaid + nDMCashInserted);
  // TAR 228760
  ps.ShowCMTBText(csTBMessage2Scot);	//thp
  ps.Message12(MSG_CHECKYOURCART, true);
  DMSayPhrase(CHECKBASKETREMINDER);

  trace(L2, _T("::Initialize - bEASReEnableScanner is set to true."));
  bEASReEnableScanner = true;
  SAWLDBScannerEnable();	// enable the scanner
  ps.ShowCMFrame();
  return STATE_NULL;                // stay in CheckBasketReminder state
}


void SMCheckBasketReminderBase::UnInitialize(void)
{
  SAWLDBScannerDisable();			   // disable the scanner
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("CheckBasketReminder") );
}

///////////////////////////////////////
SMStateBase *SMCheckBasketReminderBase::TimedOut(void) // go forward if reaching the timeout
{
	return PSButton2();
}

//////////////////////////////////////////
SMStateBase  *SMCheckBasketReminderBase::PSButtonHelp(void)
{
	return SMStateBase::PSButtonHelp();
}

/////////////////////////////////////
SMStateBase *SMCheckBasketReminderBase::PSButton2(void) // OK Button
{
  bIsInFinalization = true; //tar 223051
  return setAndCreateAnchorState(BES_FINISH);  
}

////////////////////////////////////////
SMStateBase *SMCheckBasketReminderBase::PSEnterKey(void) // 
{
  DMSayPhrase(KEYPADVALID);
  return PSButton2();          
}

/////////////////////////////////////
SMStateBase *SMCheckBasketReminderBase::PSButtonGoBack(void) // Go Back Button
{
  //+SR713
  if(co.fOperationsRepeatSellBags && co.fOperationsSellBagsAtFinalize)
  {
	bDoNotShowSellBags = false;
	
  }	
  bIsInPreTenderState = false;
  //-SR713
  return setAndCreateAnchorState(BES_SCANANDBAG);
}

///////////////////////////////////////
SMStateBase  *SMCheckBasketReminderBase::DMScanner(void)// scanned item
{
  csItemSent = csDMLastBarCode;
  return TBItemSale(false,0,true);
}

///////////////////////////////////////////////////////////////////////////
//tar 201569
SMStateBase *SMCheckBasketReminderBase::OnWtIncreaseNotAllowed()	 
{
	trace(L2, _T("SMStateBase::OnWtIncreaseNotAllowed, PrevState = %d, NextState = %d"),getAnchorState(), getAnchorState());
	// text here is just the name of the section in scotdataentry.ini.  Don't need to be in SCOTMSGE.DAT
	DMSaySecurity( SEC_NONE, _T("blank.wav") );
	CString csCurrentStateName;
	try
	{
		csCurrentStateName = this->GetRuntimeClass()->m_lpszClassName;
	}
	catch(...){};
	if(csCurrentStateName.Find(_T("SystemMessage")) != -1)
	{
        if ((getAnchorState() == BES_FINISH) && (SMFinishBase::issuedCashAcceptorError))
        {
            // weight increase causes to leave current system message which is waiting to be addressed
            // so, make sure operator login is required to see any current cash acceptor error once weight is fixed
            // TAR: 196868
            SMFinishBase::issuedCashAcceptorError = false;
        }
    }
	/* TAR 364552 removed if else statement
	if (nDMCashInserted>0)
	{
		CREATE_AND_DISPENSE(SecUnExpectedIncrease)(BES_CASHPAYMENT, BES_CASHPAYMENT);
	}
	else
	{
		CREATE_AND_DISPENSE(SecUnExpectedIncrease)(BES_FINISH, BES_FINISH);
	}*/
	CREATE_AND_DISPENSE(SecUnExpectedIncrease)(BES_FINISH, BES_FINISH);//TAR 364552 - moved this line out of if else
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMCheckBasketReminderBase::OnBackToLGW(void)
{
	trace(L2, _T("SMCheckBasketReminderBase::OnBackToLGW, returning state null"));

	// TAB re-enable scanner and clear Wait For Approval State
	TABReEnableScannerAndClearWaitForApprovalState();

	return STATE_NULL;
}
