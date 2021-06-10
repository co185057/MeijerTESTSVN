//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmRestrictedItemsBase.CPP
//
// TITLE: Class implementation for store mode Restricted Items state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMSmRestrictedItems.h"    // This state
#include "SMSmVoidedItems.h"        // Voided items auth
#include "SMSmAuthorization.h"      // Auth state
#include "SMSmEnterBirthdate.h"     // 
#include "SMSmAssistMenu.h"      // MGV auth state
#include "SMSmAssistMode.h"         // TAR 391593

#define COMP_ID ID_SM               // Base state component
#define T_ID _T("SmRestrictedItemsBase")


IMPLEMENT_DYNCREATE(SMSmRestrictedItemsBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SmRestrictedItems)

DEFINE_TIMESTAMP

//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
SMSmRestrictedItemsBase::SMSmRestrictedItemsBase()
: csCurContext(EMPTY_STRING)
{
  IMPLEMENT_TIMESTAMP
}

//////////////////////////////////////////////
// Initialize
//////////////////////////////////////////////
SMStateBase  *SMSmRestrictedItemsBase::Initialize(void)
{
  bStoreModeApprovals = true;
  csBirthDate = "";                 // Birthdate input by user
  csCurContext = _T("SmRestrictedItems");  // Tar 238987
  ps.SetFrame(csCurContext);

  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmRestrictedItems") );	// RFC 330050
  COleVariant v = VARIANT_TRUE;
  if (!co.fStateDOBNotNeededButton)
  {
	  v = VARIANT_FALSE;
	  m_pAppPSX->SetConfigProperty(_T("SMButton2"), csCurContext, UI::PROPERTYVISIBLE, v);
  }
  else
  {
	  //TAR408730
	  ps.ButtonEnable(_T("SMButton2"), true);
	  v = VARIANT_TRUE;
	  m_pAppPSX->SetConfigProperty(_T("SMButton2"), csCurContext, UI::PROPERTYVISIBLE, v);
  }
   
  ps.ShowSMTotal(true);
  ps.ShowSMTBText(csTBMessage2Scot);	//thp
  
  // BeginCodeLocale
  int nNoTotalDigit = 4; // By definition: 2 digits for month and for days
  if (co.nLocaleNoCenturyDigit==0)
	 nNoTotalDigit += 2;
  if (co.nLocaleNoCenturyDigit==1) 
	 nNoTotalDigit += 4;  
  ps.AllowInput(nNoTotalDigit,false,false);

	//SR81 begin
	if (!co.fOperationsImmediateInterventionForRestrictedItems)
	{
		ps.ButtonState( _T("SMButton3"), false, false);
		//TAR 408485+
		//ps.ButtonEnable(_T("SMButton8"), true);
		if (ps.RemoteMode())
		{
			ps.Button(_T("SMButton8"), BTT_GOBACK, true);
		}
		else
		{
			ps.Button(_T("SMButton8"), TTL_STOREAUTH, true);
		}
		//TAR 408485-
	}
	else
	{
		ps.ButtonState( _T("SMButton3"), true, true);
		
		//TAR 408485+
		//ps.ButtonEnable(_T("SMButton8"), false);
		if (ps.RemoteMode())
		{
			ps.Button(_T("SMButton8"), BTT_GOBACK, false);
		}
		else
		{
			ps.Button(_T("SMButton8"), TTL_STOREAUTH, false);
		}
		//TAR 408485-
	}
	//SR81 end

  // EndCodeLocale
  ps.ShowFrame();

  //Tar 229455 Show green light
  dm.turnOffTriColorLight();
  dm.setTriColorLight(DM_TRICOLORLIGHT_GREEN,
                         DM_TRICOLORLIGHT_ON,
                         DM_TRICOLORLIGHT_NORMALREQUEST);

  return STATE_NULL;                // stay in SmRestrictedItemsBase state
}

/////////////////////////////////////////////
// PSButton1 - birth date not needed
/////////////////////////////////////////////
SMStateBase  *SMSmRestrictedItemsBase::PSButton1(void)
{
  bInApprovalNeeded = false; //TAR 279092
  RETURNSTATE(SmEnterBirthdate)
}

//SR81 begin
/////////////////////////////////////////////
// PSButton3 - assist mode button
/////////////////////////////////////////////
SMStateBase  *SMSmRestrictedItemsBase::PSButton3(void)
{	  
	bForceApprovalBeforeSuspend = true;
	lUnapprovedRestrictedItems = 1;

	CString csDateNotNeeded = ps.GetPSText(RA_AGE_RESTRICTED_ID, SCOT_LANGUAGE_PRIMARY);
	csDateNotNeeded +=  _T("\n") + ps.GetPSText(BTT_BIRTHDATENOTREQUIRED, SCOT_LANGUAGE_PRIMARY);
	trace(L7, _T("+ra.OnNormalItem, Message=%s"), csDateNotNeeded);
	ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + csDateNotNeeded); // Display valid birthday on RAP
	trace(L7, _T("-ra.OnNormalItem"));  

	// remote approval
	CString csDesc = ps.GetPSText(RA_AGE_RESTRICTED_ID);

	//TAR 196910 start
	while (m_lAgerestrictQueuedtoRAP > 0)
	{
		trace(L7, _T("+ra.OnAgeApproval"));
		ra.OnAgeApproval();
		trace(L7, _T("-ra.OnAgeApproval"));
		m_lAgerestrictQueuedtoRAP--;
	}
	m_lAgerestrictQueuedtoRAP = 0;

	if (co.fStateAssistMenuFromStoreModeAllowed)
	{
		RETURNSTATE(SmAssistMenu)
	}

	RETURNSTATE(SmAssistMode)  // TAR 391593
}
//SR81 end

/////////////////////////////////////////////
// PSButton8 - Cancel
/////////////////////////////////////////////
SMStateBase  *SMSmRestrictedItemsBase::PSButton8(void)
{
  bForceApprovalBeforeSuspend = false; //TAR 364905
  bInApprovalNeeded = false; //TAR 279092
  trace(L7, _T("Cashier selected Store Authorization button instead of approving age restricted item(s)"));
  bStoreModeGoBack = true;
  // They are cancelling - nothing entered here will be
  // saved.  Note that if they have entered valid information
  // to authorized all unapproved items, they will have already
  // left via PSEnterKey and won't have a chance to get here.
  //TAR 290433
  if (co.IsInAssistMenus())
  {
	  RETURNSTATE(SmAssistMenu)
  }
  RETURNSTATE(SmAuthorization)
}

///////////////////////////////////////////////////////////
// PSNumericKey
///////////////////////////////////////////////////////////
SMStateBase  *SMSmRestrictedItemsBase::PSNumericKey(const long lKey)
{
  PSEchoInput(lKey);
  return STATE_NULL;
}

///////////////////////////////////////////////////////////
// PSButton2
///////////////////////////////////////////////////////////
SMStateBase  *SMSmRestrictedItemsBase::PSButton2(void)
{
  // On the cashier's authority, consider all tobacco and alcohol items
  // approved now, and proceed as if a valid legal age for any restricted
  // item has been obtained.
  lUnapprovedRestrictedItems =0;
  lRegularYellowNeedingApproval = 0;
  bInApprovalNeeded = false; //TAR 279092
  fApprovedForRestrictedItems = true;
  fAgeObtained = true;
  nCustomerAge = 99;
  
  CString csDateNotNeeded = ps.GetPSText(RA_AGE_RESTRICTED_ID, SCOT_LANGUAGE_PRIMARY);
  csDateNotNeeded +=  _T("\n") + ps.GetPSText(BTT_BIRTHDATENOTREQUIRED, SCOT_LANGUAGE_PRIMARY);
  trace(L7, _T("+ra.OnNormalItem, Message=%s"), csDateNotNeeded);
  ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + csDateNotNeeded); // Display valid birthday on RAP
  trace(L7, _T("-ra.OnNormalItem"));  
  
  // remote approval
  CString csDesc = ps.GetPSText(RA_AGE_RESTRICTED_ID);

  //TAR 196910 start
  while (m_lAgerestrictQueuedtoRAP > 0)
  {
	  trace(L7, _T("+ra.OnAgeApproval"));
	  ra.OnAgeApproval();
	  trace(L7, _T("-ra.OnAgeApproval"));
	  m_lAgerestrictQueuedtoRAP--;
  }
  m_lAgerestrictQueuedtoRAP = 0;
  //TAR 196910 end

  RETURNSTATE(SmAuthorization)
}

//////////////////////////////////////////////
// PSClearKey
//////////////////////////////////////////////
SMStateBase  *SMSmRestrictedItemsBase::PSClearKey(void)
{
  PSClearInput();
  return STATE_NULL;
}

//////////////////////////////////////////////
// PSEnterKey
//////////////////////////////////////////////
SMStateBase  *SMSmRestrictedItemsBase::PSEnterKey(void)
{
  return VerifyBirthDate();
}
////////////////////////////////////////
// NewUI
SMStateBase  *SMSmRestrictedItemsBase::PSReceiptUp(void)
{
  ps.ScrollUp(_T("SMReceiptRestricted"));
  return STATE_NULL;

}
SMStateBase  *SMSmRestrictedItemsBase::PSReceiptDown(void)
{
  ps.ScrollDown(_T("SMReceiptRestricted"));
  return STATE_NULL;

}
// NewUI
//////////////////////////////////////////////
SMStateBase *SMSmRestrictedItemsBase::OnRAPUnsolicitedDataReceived(LPCTSTR sData) 
   {
   trace (L6, _T("+SMSmRestrictedItemsBase::OnRAPUnsolicitedDataReceived(%s)."), sData);
   trace (L6, _T("-SMSmRestrictedItemsBase::OnRAPUnsolicitedDataReceived() -- Ignore remote scanned input"));
   return STATE_NULL;
   }
// + RFC 330050
void SMSmRestrictedItemsBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmRestrictedItems") );	
}
// - RFC 330050
