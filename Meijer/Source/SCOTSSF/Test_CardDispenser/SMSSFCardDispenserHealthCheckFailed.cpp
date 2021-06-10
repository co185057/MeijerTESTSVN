#ifdef _TESTSSF
#include <stdafx.h> 				// MS MFC always needed first
//				// MS MFC template header

#include "Common.h" 				// Common includes

#include "SMState.h"				// Base state
#include "SMSSFCardDispenserHealthCheckFailed.h"
#include "SMSSFSelectGiftCardActivity.h"

#define COMP_ID ID_SM				// Base state component
#define T_ID	_T("SSFCardDispenserHealthCheckFailed")

IMPLEMENT_DYNCREATE(SMSSFCardDispenserHealthCheckFailed, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSSFCardDispenserHealthCheckFailed::SMSSFCardDispenserHealthCheckFailed()
{
	IMPLEMENT_TIMESTAMP

}

////////////////////////////////////
SMStateBase *SMSSFCardDispenserHealthCheckFailed::Initialize(void)
{
	trace(L6, _T("+SMSSFCardDispenserHealthCheckFailed::Initialize"));
	nTimeOut = 20;		// set 20sec timeout
	//+setup UI
	ps.SetCMFrame(_T("SystemMessage")); 
	m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("SystemMessage"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
	ps.Message12(_T("\n\nCard dispenser is not currently available"), true);
	ps.SetButtonText(_T("CMButton1Med"),BTT_CONTINUE);
	ps.ShowCMFrame();
	//-setup UI
	trace(L6, _T("-SMSSFCardDispenserHealthCheckFailed::Initialize"));
	return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Function:    PSButton1()
// Description: 
///////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMSSFCardDispenserHealthCheckFailed::PSButton1(void) 
{
  RETURNSTATE(SSFSelectGiftCardActivity);	
}
///////////////////////////////////////////////////////////////////////////////
// Function:    TimedOut
// Description: 
///////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMSSFCardDispenserHealthCheckFailed::TimedOut(void)
{
  RETURNSTATE(SSFSelectGiftCardActivity);	
}

void SMSSFCardDispenserHealthCheckFailed::UnInitialize()
{
	m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("SystemMessage"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
}
#endif

