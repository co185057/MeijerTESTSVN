#ifdef _TESTSSF
#include <stdafx.h> 				// MS MFC always needed first
#include <afxtempl.h>				// MS MFC template header

#include "Common.h" 				// Common includes

#include "SMState.h"				// Base state
#include "SMSSFIssueRecharge.h"
#include "SMSSFCardDispenserReadFailed.h"
#include "SMSSFCardRechargeDispenserTakeCard.h"

#define COMP_ID ID_SM				// Base state component
#define T_ID	_T("SSFIssueRecharge")

IMPLEMENT_DYNCREATE(SMSSFIssueRecharge, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP


//////////////////////////////////////////
SMSSFIssueRecharge::SMSSFIssueRecharge()

{
	IMPLEMENT_TIMESTAMP

}

////////////////////////////////////
SMStateBase *SMSSFIssueRecharge::Initialize(void)
{
	trace(L6, _T("+SMSSFIssueRecharge::Initialize"));
	
	nTimeOut = 20; //set 20sec timeout
	ps.SetCMFrame(_T("SystemMessage")); 
	m_pAppPSX->SetConfigProperty( _T("CMButton1Med"), _T("SystemMessage"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
	m_pAppPSX->SetConfigProperty( _T("ButtonGoBack"), _T("SystemMessage"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
	ps.Message12Modal(_T("\n\nShopping Card Dispenser Processing"));
	ps.ShowFrame();


	if (WriteGiftCard(false))
	{
		RETURNSTATE(SSFCardRechargeDispenserTakeCard);
	}else
	{
		RETURNSTATE(SSFCardDispenserReadFailed);
	}
	trace(L6, _T("-SMSSFIssueRecharge::Initialize"));
	return STATE_NULL;
}

SMStateBase *SMSSFIssueRecharge::TimedOut(void)
{
	RETURNSTATE(SSFCardDispenserReadFailed);
}

void SMSSFIssueRecharge::UnInitialize(void)
{
	m_pAppPSX->SetConfigProperty( _T("CMButton1Med"), _T("SystemMessage"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
	m_pAppPSX->SetConfigProperty( _T("ButtonGoBack"), _T("SystemMessage"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
}

#endif

