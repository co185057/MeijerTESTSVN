#ifdef _TESTSSF
#include <stdafx.h> 				// MS MFC always needed first
//				// MS MFC template header

#include "Common.h" 				// Common includes

#include "SMState.h"				// Base state
#include "SMSSFCardDispenserReadFailed.h"
#include "SMScanAndBag.h"

#define COMP_ID ID_SM				// Base state component
#define T_ID	_T("SSFCardDispenserReadFailed")

IMPLEMENT_DYNCREATE(SMSSFCardDispenserReadFailed, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSSFCardDispenserReadFailed::SMSSFCardDispenserReadFailed()

{
	IMPLEMENT_TIMESTAMP

}

////////////////////////////////////
SMStateBase *SMSSFCardDispenserReadFailed::Initialize(void)
{
	trace(L6, _T("+SMSSFCardDispenserReadFailed::Initialize"));
	//+setup UI
	ps.SetCMFrame(_T("SystemMessage")); 
	m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("SystemMessage"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
	ps.Message12(_T("\n\nCard not inserted or inserted improperly"), true); 
	ps.SetButtonText(_T("CMButton1Med"),BTT_CONTINUE);
	ps.ShowCMFrame();
	//check if card is in the TRANSPORT
	if ( DM_CD_CARD_IN_TRANSPORT == dm.CardDispenserGetCardPosition())
	{
		dm.CardDispenserMoveToGate(); 
	}
	//-setup UI
	trace(L6, _T("-SMSSFCardDispenserReadFailed::Initialize"));
	return STATE_NULL;
}

///////////////////////////////////
SMStateBase *SMSSFCardDispenserReadFailed::PSButton1(void)
{
  	RETURNSTATE(ScanAndBag);
}
SMStateBase *SMSSFCardDispenserReadFailed::TimedOut(void)
{
  	RETURNSTATE(ScanAndBag);
}
#endif

