//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMApplyPromotionalItems.cpp
//
// TITLE: Class implementation for applying promotional items (POS19072)
//
// CHANGE HISTORY
//
// POS19072 WORK REQUEST:16656 Name: Matt Condra Date: November 29, 2011
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h> 				// MS MFC always needed first
#include <afxtempl.h>				// MS MFC template header

#include "Common.h" 				// Common includes

#include "SMState.h"				// Base state
#include "SMApplyPromotionalItems.h"
#include "SMContinueTrans.h"
#include "PSIntList.h"
#include "TransBrokerSSF.h"
#include "SMInProgress.h"

#define COMP_ID ID_SM				// Base state component
#define T_ID	_T("ApplyPromotionalItems")

IMPLEMENT_DYNCREATE(SMApplyPromotionalItems, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMApplyPromotionalItems::SMApplyPromotionalItems()
{
	IMPLEMENT_TIMESTAMP
}

////////////////////////////////////
SMStateBase *SMApplyPromotionalItems::Initialize(void)
{
  CString sLTT, sMessage;
	
  SAWLDBScannerDisable();   // Disable the scanner
  ps.SetCMFrame(_T("ApplyPromotionalItems"));
  ps.ShowCMTBText(csTBMessage2Scot);	//thp

  ps.ButtonState(_T("ButtonHelp"), true, true);

  // Disable 'Yes' button if no items have been rung-up yet.
  COleVariant v (lItems? (long)UI::Normal : (long)UI::Disabled);
  m_pAppPSX->SetConfigProperty (_T("CMButton1Med"), _T("ApplyPromotionalItems"), UI::PROPERTYSTATE, v);

  // Make sure 'No' button is enabled.
  m_pAppPSX->SetConfigProperty (_T("CMButton2Med"), _T("ApplyPromotionalItems"), UI::PROPERTYSTATE, (long)UI::Normal);

  // Display 'Apply Promotional Items' screen title with a message of 'Are you finished entering your WIC eligible items?'.
  sLTT = ps.GetPSText(SSF_LTT_APPLYPROMOTIONALITEMS);
  sMessage = ps.GetPSText(SSF_MSG_APPLYPROMOTIONALITEMS);
  ps.SetLeadthruText(sLTT, _T(""));
  ps.Message12(sMessage, true);
  
  ps.ShowCMFrame();

  // Indicate that customer has promotional/free items to apply.
  SetHookProperty(_T("CustomerHasWICFreeItems"), _T("1"));

  return STATE_NULL;
}

///////////////////////////////////
void SMApplyPromotionalItems::UnInitialize(void)
{
	SAWLDBScannerDisable();		  // disable the scanner
	ps.HideButtons();
}

///////////////////////////////////
SMStateBase *SMApplyPromotionalItems::PSButton1(void)
{	
	// Yes, the customer is finished entering WIC eligible items.
	trace(L6, _T("+SMApplyPromotionalItems::PSButton1 (Yes)"));

	// Disable buttons while waiting for WIC Free Item phase to become active.
	m_pAppPSX->SetConfigProperty (_T("CMButton1Med"), _T("ApplyPromotionalItems"), UI::PROPERTYSTATE, (long)UI::Disabled);
	m_pAppPSX->SetConfigProperty (_T("CMButton2Med"), _T("ApplyPromotionalItems"), UI::PROPERTYSTATE, (long)UI::Disabled);

	// Force sales app into the WIC Free Item phase.
	SendTBCommand(TBCMD_ENTERING_WIC_FREE_ITEM_PHASE, true);

	// Wait until sales app enters the WIC Free Item phase, at which time the Scan&Bag's 
	// initialization routine will take care of setting up the 'Scan Promotional Items' screen.
	RETURNSTATE(InProgress)
}

///////////////////////////////////
SMStateBase *SMApplyPromotionalItems::PSButton2(void)
{	
	trace(L6, _T("+SMApplyPromotionalItems::PSButton2 (No)"));

	// No, the customer is not finished entering WIC eligible items, so return back to Scan&Bag state.
	return STATE_RETURN;
}

///////////////////////////////////
SMStateBase *SMApplyPromotionalItems::TimedOut(void)
{	
	trace(L6, _T("+SMApplyPromotionalItems::TimedOut"));
	RETURNSTATE(ContinueTrans)
}


