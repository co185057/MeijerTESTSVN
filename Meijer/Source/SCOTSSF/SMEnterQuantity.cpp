//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMEnterQuantity.CPP
//
// TITLE: Class implementation for enter quantity state
//
//
// AUTHOR:    Peter Denhoed
//
// POS11024 Work Request:16632 Name: Sandy Reedy Date:July 14, 2011
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMEnterQuantity.h"        // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("EnterQuantity")

IMPLEMENT_DYNCREATE(SMEnterQuantity, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMEnterQuantity::SMEnterQuantity()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMEnterQuantity::Initialize(void)
{
    nTimeOut = 0;//TAR352193

	// +CRD 140001 - Auto process of QTY when a quantity required items recevied
	if ( g_bInRecallMode ) 
	{
		trace(L3, _T("*_* Quantity required received while transferring items"));
	
		if(!fInMultiSelectPickList)//SR679
		{
			TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("EnterQuantity") );
		}
		//Update RAP window with SCOT state
		UpdateStateName(ps.GetPSText(LTT_HOWMANYBUY, SCOT_LANGUAGE_PRIMARY));
		setAnchorState(BES_ENTERQUANTITY);  //TAR#134884 return to this state from help on way
		//+tar 330491
		m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("EnterQuantity"), UI::PROPERTYSTATE, (long)UI::Normal); 
		//-tar 330491
		ra.RequestEnableAssistMode(false); // TAR 297347

		// CS sent quantity required items as a single quantity item and SCO will send a quantity of "1" to the POS.
		io.lQuantityEntered = 1;
		fItemOverride = true;

		io.d.fQuantityRequired = true;
		io.d.fQuantityConfirmed = false;

		if (io.fVoid && lVoidQty>0)
		{
			io.lQuantityEntered = lVoidQty;
			lVoidQty = 0;
		}

		trace(L6, _T("*_* Quantity required sending QTY in transferring items"));
		SMStateBase *ReturnState= TBEnterItem();
		trace(L6, _T("*_* Quantity required sent completed in transferring items"));

		return ReturnState;
	}
	// -CRD 140001 - Auto process of QTY when a quantity required items recevied

	//POS11024 Mobile Shopper. Display ItemDesc in EnterQuantity screen
	SMStateBase *ReturnState=SMEnterQuantityBase::Initialize();
	CString ItemDesc= GetHookProperty(_T("PropItemDesc"));
	ItemDesc.TrimLeft();
	COleVariant v;
	v= ItemDesc;
	m_pAppPSX->SetConfigProperty(_T("Title2Area"), ps.GetCurrentContext(),UI::PROPERTYTEXTFORMAT, v);
	
	//POS11024 return ReturnState
  return ReturnState;
}

void SMEnterQuantity::UnInitialize(void)
{
    SMEnterQuantityBase::UnInitialize();
}

bool SMEnterQuantity::PSAssistMenuLegalForState()
{
    return SMEnterQuantityBase::PSAssistMenuLegalForState();
}

SMStateBase * SMEnterQuantity::PSButton1(void)
{
    return SMEnterQuantityBase::PSButton1();
}

SMStateBase * SMEnterQuantity::PSButton3(void)
{
    return SMEnterQuantityBase::PSButton3();
}

SMStateBase * SMEnterQuantity::PSNumericKey(const long lKey)
{
    return SMEnterQuantityBase::PSNumericKey(lKey);
}

SMStateBase * SMEnterQuantity::PSClearKey(void)
{
    return SMEnterQuantityBase::PSClearKey();
}

SMStateBase * SMEnterQuantity::PSEnterKey(void)
{
    return SMEnterQuantityBase::PSEnterKey();
}

SMStateBase * SMEnterQuantity::PSCancelKey(void)
{
    return SMEnterQuantityBase::PSCancelKey();
}

SMStateBase * SMEnterQuantity::PSBackSpaceKey(void)
{
    return SMEnterQuantityBase::PSBackSpaceKey();
}

SMStateBase * SMEnterQuantity::PSButtonGoBack(void)
{
    return SMEnterQuantityBase::PSButtonGoBack();
}
