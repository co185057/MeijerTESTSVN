//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSelectTareBase.cpp
//
// TITLE: Class implementation for Select Tare state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header
#include "SMSelectTare.h"       // This state
#ifndef _CPPUNIT
#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state

#include "SMThankYou.h"
#include "SMRemoveItemFromProduceScale.h"


#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SelectTare")

#endif //_CPPUNIT 

IMPLEMENT_DYNCREATE(SMSelectTareBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SelectTare)


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSelectTareBase::SMSelectTareBase()
#ifndef _CPPUNIT
: csCurContext(EMPTY_STRING)
#endif //_CPPUNIT
{
   #ifndef _CPPUNIT
  IMPLEMENT_TIMESTAMP
  saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;
   #endif

}


//////////////////////////////////////////
SMStateBase  *SMSelectTareBase::Initialize(void)
{
#ifndef _CPPUNIT
	if(!fInMultiSelectPickList)//SR679
	{
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SelectTare") );	// RFC 330050
	}
	//Update RAP window with SCOT state
	UpdateStateName(ps.GetPSText(LTT_SELECTITEM, SCOT_LANGUAGE_PRIMARY));
	//DMSayPhrase(SELECTCONTAINER);
	setAnchorState(BES_SELECTTARE);
//*** NewUI ***//
	ps.SetCMFrame(_T("SelectTare"));
#endif //_CPPUNIT   
    csCurContext = _T("SelectTare");  // Tar 238987
    COleVariant v;
    if(g_bInRecallMode)
    {
        v=VARIANT_FALSE;
        ps.CMButton(_T("ButtonHelp"),BTT_HELP,false); 
    }
    else
    {
        v=VARIANT_TRUE;
    }
    m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), csCurContext, UI::PROPERTYVISIBLE, v);
#ifndef _CPPUNIT

	ps.ShowCMFrame(); //SSCOADK-6740 - additional fix - moved this here to set language before tares are displayed
	DMSayPhrase(SELECTCONTAINER);
	
	if (co.IsInAssistMenus())
	{
		ps.ResetAMTareList();
		ps.SetLanguage(SCOT_LANGUAGE_PRIMARY);
		ps.AssignAMTareList();
		COleVariant vReturn, vIndex = (long)UI::ScrollTop;
		m_pAppPSX->SendCommand( _T("AMTareList"), UI::COMMANDSCROLL, vReturn, 1, vIndex );  //_T("ScrollSelection") //SSCOP-1991
        ps.ScrollUp(_T("AMTareList"));   //SSCOP-1991
		
	}
	else
		ps.DisplaySelectedItem(_T("   "));
	ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
	ps.ShowCMTBText(csTBMessage2Scot);	//thp
    //RFC 236396 050703
    m_pAppPSX->SetTransactionVariable( _T("LeadthruText"), io.csOrgItemDescription );
	//ps.ShowCMFrame(); SSCOADK-6740 - additional fix - moved this up to set language before tares are displayed
#endif //_CPPUNIT   
	return STATE_NULL;// stay in SelectTare state
}
#ifndef _CPPUNIT
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
#endif //_CPPUNIT   
//////////////////////////////////////////
SMStateBase  *SMSelectTareBase::PSListLookup(CString cItemCode) // tare selection
{
#ifndef _CPPUNIT
  // NewUI
  long lTare = _ttoi(cItemCode);
  if (lTare==0)
  {
    DMSayPhrase(KEYPADINVALID);
    return STATE_NULL;
  }
// NewUI
	
  io.lTareCode = lTare;
  m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("SelectTare"), UI::PROPERTYSTATE, (long)UI::Disabled); //tar 330491
#endif //_CPPUNIT   
  if(g_bInRecallMode)
  {
      RETURNSTATE(RemoveItemFromProduceScale); 
  }
  return TBEnterItem(false);
}
#ifndef _CPPUNIT
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

#endif //_CPPUNIT 

SMStateBase  *SMSelectTareBase::PSReceiptDown(void)
{
  // NextGen //
  if(co.IsInAssistMenus())
  {
      if (0 != nPSLastParm)
      {
	      CString csItemId;
	      csItemId.Format(_T("%d"), nPSLastParm);
	      ps.GetPSXObject()->SetSelectedReceiptItem(_T("AMTareList"), csItemId);
      }
  }

//  ps.ScrollDown(_T("AMTareList"));
  return STATE_NULL;
}

SMStateBase *SMSelectTareBase::PSButton1(void)
{
#ifndef _CPPUNIT
  USES_CONVERSION;
  CString csCode;
  COleVariant vValue;
  csCode = ps.GetSelection(_T("AMTareList"));
  m_pAppPSX->GetReceiptItemVariable(_T("AMTareList"), csCode, _T("ITEM_SMTARECODE"), vValue);
  vValue.ChangeType( VT_INT );
  long lTare = vValue.lVal;
  io.lTareCode = lTare;
#endif //_CPPUNIT 
  if(g_bInRecallMode)
  {
      RETURNSTATE(RemoveItemFromProduceScale); 
  }
  return TBEnterItem(false);
}
#ifndef _CPPUNIT
// + RFC 330050
void SMSelectTareBase::UnInitialize(void)
{
	if(!fInMultiSelectPickList)//SR679
	{
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SelectTare") );	
	}
}
// - RFC 330050
#endif //_CPPUNIT 
