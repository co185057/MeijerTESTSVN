//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmVoidedItemsBase.CPP
//
// TITLE: Class implementation for store mode voided Items state
//
////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMSmVoidedItems.h"    // This state
#include "SMSmAuthorization.h"      // Auth state

#define COMP_ID ID_SM               // Base state component
#define T_ID _T("SmVoidedItemsBase")

IMPLEMENT_DYNCREATE(SMSmVoidedItemsBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SmVoidedItems)


DEFINE_TIMESTAMP

//////////////////////////////////
SMSmVoidedItemsBase::SMSmVoidedItemsBase()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase  *SMSmVoidedItemsBase::Initialize(void)
{
  CString csBtt1;
	
    csBtt1.Format(_T("%s\n(%d/%d)"),
    ps.GetPSText(BTT_APPCANCELS),
    lUnapprovedVoidedItems,
    lVoidedItems);
	
    ps.SetPSText(PS_WORK1,csBtt1);
// NewUI
	ps.SetFrame(_T("SmVoidedItems"));
	ps.SetButtonText(_T("SMButton1"), PS_WORK1);
// NewUI

    ps.SMShowVoidedItemReceipt(true);
 	ps.ShowSMTotal(true);
	ps.ShowSMTBText(csTBMessage2Scot);	//thp
	ps.ShowFrame();

  return STATE_NULL;
}


SMStateBase  *SMSmVoidedItemsBase::PSButton1(void) // approve voids
{
  isVoidRatioApprovalRequired();	//Fix for TAR# 106570 LPM071499
  lUnapprovedVoidedItems = 0;

  // remote approval
  trace(L7, _T("+ra.OnVoidApproval"));
  ra.OnVoidApproval();
  trace(L7, _T("-ra.OnVoidApproval"));
  RETURNSTATE(SmAuthorization)
}

SMStateBase  *SMSmVoidedItemsBase::PSButton8(void) // cancel
{
  RETURNSTATE(SmAuthorization)
}

//SMStateBase  *SMSmVoidedItemsBase::PSCancelKey(void) // cancel ----> removed per tar 121487
//{
//  DMSayPhrase("KeyPadValid");
//  return PSButton8();
//}
