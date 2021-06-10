//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSelectTareBase.cpp
//
// TITLE: Class implementation for Select Tare state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header
#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMSelectTare.h"       // This state

#include "SMThankYou.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SelectTare")

IMPLEMENT_DYNCREATE(SMSelectTareBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SelectTare)


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSelectTareBase::SMSelectTareBase()
: csCurContext(EMPTY_STRING)
{
  IMPLEMENT_TIMESTAMP
  saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;

}

//////////////////////////////////////////
SMStateBase  *SMSelectTareBase::Initialize(void)
{
	if(!fInMultiSelectPickList)//SR679
	{
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SelectTare") );	// RFC 330050
	}
	//Update RAP window with SCOT state
	UpdateStateName(ps.GetPSText(LTT_SELECTITEM, SCOT_LANGUAGE_PRIMARY));
	DMSayPhrase(SELECTCONTAINER);
	setAnchorState(BES_SELECTTARE);

    csCurContext = _T("SelectTare");  // Tar 238987
	m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("SelectTare"), UI::PROPERTYVISIBLE, VARIANT_TRUE); //tar 330491
//*** NewUI ***//
	ps.SetCMFrame(_T("SelectTare"));
	if (co.IsInAssistMenus())
	{
		ps.ResetAMTareList();
		ps.SetLanguage(SCOT_LANGUAGE_PRIMARY);
		ps.AssignAMTareList();
		COleVariant vReturn, vIndex = (long)UI::ScrollTop;
		m_pAppPSX->SendCommand( _T("AMTareList"), _T("ScrollSelection"), vReturn, 1, vIndex );
   
	}
	else
		ps.DisplaySelectedItem(_T("   "));
	ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
	ps.ShowCMTBText(csTBMessage2Scot);	//thp
    //RFC 236396 050703
    m_pAppPSX->SetTransactionVariable( _T("LeadthruText"), io.csOrgItemDescription );
	ps.ShowCMFrame();
	return STATE_NULL;// stay in SelectTare state
}

//////////////////////////////////////////
SMStateBase  *SMSelectTareBase::PSButton4(void)// cancel
{
  SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
  //+tar 389019
  if(fInMultiSelectPickList)
  {
     //if(ps.GetPicklistItemsSold()==0)
	 //{
	 //   SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
	 //}
  // +TAR 407200
	 g_bMultiPickSkipBaggingAllowed = true;
  // -TAR 407200

	 g_lNumberOfItemsNotBagInMultiPick--;
     return setAndCreateAnchorState( BES_PRODUCEFAVORITES );
  }
  //-tar 389019
  else
  {
     // BeginCodeLocaleUPC
     //SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
     // EndCodeLocaleUPC			  
     return setAndCreateAnchorState(BES_SCANANDBAG);	// go back to scan and bag TAR#126112 LPM111699
  }
}

//////////////////////////////////////////
SMStateBase  *SMSelectTareBase::PSListLookup(CString cItemCode) // tare selection
{
  // NewUI
  long lTare = _ttoi(cItemCode);
  if (lTare==0)
  {
    DMSayPhrase(KEYPADINVALID);
    return STATE_NULL;
  }
// NewUI
	
  io.lTareCode = lTare;
  m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("SelectTare"), UI::PROPERTYVISIBLE, VARIANT_FALSE); //tar 330491
  
  return TBEnterItem(false);
}

//////////////////////////////////////////
SMStateBase  *SMSelectTareBase::PSCancelKey(void)/// override where needed
{
  DMSayPhrase(KEYPADVALID);
  return PSButton4();               // usually same as button 4
}

//////////////////////////////////////////
SMStateBase  *SMSelectTareBase::PSButtonGoBack(void)/// override where needed
{
  return PSButton4();               // TAR#126112  return to Scan&Bag LPM111699
}
//*** New UI - aiqin ***//
SMStateBase  *SMSelectTareBase::PSListScroll(const long lScroll) // list scroll
{
    if (!co.IsInAssistMenus())
    {
        COleVariant vReturn, vReturn1, v = lScroll; // 0 goes up 1 row and 1 goes down one row
        m_pAppPSX->SendCommand( _T("TareList"), _T("Scroll"), vReturn, 1, v );
        vReturn = VARIANT_FALSE;
        v = (long)UI::PageDown;
        m_pAppPSX->SendCommand( _T("TareList"), UI::COMMANDCANSCROLL, vReturn, 1, v );
        vReturn1 = VARIANT_FALSE;
        v = (long)UI::PageUp;
        m_pAppPSX->SendCommand( _T("TareList"), UI::COMMANDCANSCROLL, vReturn1, 1, v );
        // if can scroll up and down, then make the buttons visible and enabled
        if( vReturn.boolVal == VARIANT_TRUE && vReturn1.boolVal == VARIANT_TRUE )
        {
            m_pAppPSX->SetConfigProperty(_T("CMTareUp"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
            m_pAppPSX->SetConfigProperty(_T("CMTareDown"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
            v = (long) UI::Normal;
            m_pAppPSX->SetConfigProperty(_T("CMTareUp"), csCurContext, UI::PROPERTYSTATE, v);
            m_pAppPSX->SetConfigProperty(_T("CMTareDown"),csCurContext, UI::PROPERTYSTATE, v);
        }
        // if can scroll down, but not up, then disable up and enable down
        else if( vReturn.boolVal == VARIANT_TRUE && vReturn1.boolVal == VARIANT_FALSE )
        {
            m_pAppPSX->SetConfigProperty(_T("CMTareUp"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
            m_pAppPSX->SetConfigProperty(_T("CMTareDown"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
            v = (long) UI::Disabled;
            m_pAppPSX->SetConfigProperty(_T("CMTareUp"), csCurContext, UI::PROPERTYSTATE, v);
            v = (long) UI::Normal;
            m_pAppPSX->SetConfigProperty(_T("CMTareDown"), csCurContext, UI::PROPERTYSTATE, v);
        }
        // if can scroll up, but not down, then disable down and enable up
        else if( vReturn.boolVal == VARIANT_FALSE && vReturn1.boolVal == VARIANT_TRUE )
        {
            m_pAppPSX->SetConfigProperty(_T("CMTareUp"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
            m_pAppPSX->SetConfigProperty(_T("CMTareDown"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
            v = (long) UI::Normal;
            m_pAppPSX->SetConfigProperty(_T("CMTareUp"), csCurContext, UI::PROPERTYSTATE, v);
            v = (long) UI::Disabled;
            m_pAppPSX->SetConfigProperty(_T("CMTareDown"), csCurContext,UI::PROPERTYSTATE, v);
        }
    }
	return STATE_NULL;
} 
//*** end of NewUI ***//
SMStateBase  *SMSelectTareBase::PSReceiptUp(void)
{
//  ps.ScrollUp(_T("AMTareList"));
  return STATE_NULL;
}

SMStateBase  *SMSelectTareBase::PSReceiptDown(void)
{
//  ps.ScrollDown(_T("AMTareList"));
  return STATE_NULL;
}

SMStateBase *SMSelectTareBase::PSButton1(void)
{
  USES_CONVERSION;
  CString csCode;
  COleVariant vValue;
  csCode = ps.GetSelection(_T("AMTareList"));
  m_pAppPSX->GetReceiptItemVariable(_T("AMTareList"), csCode, _T("ITEM_SMTARECODE"), vValue);
  vValue.ChangeType( VT_INT );
  long lTare = vValue.lVal;
  io.lTareCode = lTare;
  return TBEnterItem(false);
}
// + RFC 330050
void SMSelectTareBase::UnInitialize(void)
{
	if(!fInMultiSelectPickList)//SR679
	{
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SelectTare") );	
	}
}
// - RFC 330050
