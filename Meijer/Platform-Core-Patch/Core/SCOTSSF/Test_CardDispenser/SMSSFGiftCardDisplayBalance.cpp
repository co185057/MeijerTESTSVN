#ifdef _TESTSSF
#include <stdafx.h> 				// MS MFC always needed first
#include <afxtempl.h>				// MS MFC template header

#include "Common.h" 				// Common includes

#include "SMState.h"				// Base state
#include "SMSSFGiftCardDisplayBalance.h"
#include "SMScanAndBag.h"

#define COMP_ID ID_SM				// Base state component
#define T_ID	_T("SSFGiftCardDisplayBalance")

IMPLEMENT_DYNCREATE(SMSSFGiftCardDisplayBalance, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP


//////////////////////////////////////////
SMSSFGiftCardDisplayBalance::SMSSFGiftCardDisplayBalance()

{
	IMPLEMENT_TIMESTAMP

}

////////////////////////////////////
SMStateBase *SMSSFGiftCardDisplayBalance::Initialize(void)
{
	trace(L6, _T("+SMSSFGiftCardDisplayBalance::Initialize"));
	ps.SetCMFrame(_T("SystemMessage")); 
	m_pAppPSX->SetConfigProperty( _T("ButtonGoBack"), _T("SystemMessage"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
	ps.SetButtonText(_T("CMButton1Med"),BTT_CONTINUE); 
	ps.SetLeadthruText(_T("Shopping Card Balance"), _T(""));
	//extract amount from m_csGiftCardTrackData
	int nLastCaretLocation = m_csGiftCardTrackData.Find(_T("^"));
	nLastCaretLocation = m_csGiftCardTrackData.Find(_T("^"), (nLastCaretLocation + 1)); //get the last caret(^)

	CString csMessage;
	csMessage.Format(_T("\n\n Your balance is $%d"),\
		_ttoi(m_csGiftCardTrackData.Mid((nLastCaretLocation + 1), 7))/100 ); 
	ps.Message12Modal(csMessage);
	ps.ShowFrame();
	trace(L6, _T("-SMSSFGiftCardDisplayBalance::Initialize"));
	return STATE_NULL;
}

void SMSSFGiftCardDisplayBalance::UnInitialize(void)
{
	ps.SetLeadthruText(_T(""),_T(""));
	m_pAppPSX->SetConfigProperty( _T("ButtonGoBack"), _T("SystemMessage"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
}
SMStateBase  *SMSSFGiftCardDisplayBalance::PSButton1(void)
{
	dm.CardDispenserMoveToGate();
	RETURNSTATE(ScanAndBag);
}
#endif

