//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMKeyInCode.CPP
//
// TITLE: Class implementation for Key In Code state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMKeyInCode.h"            // This state

#include "transbrokerssf.h"


// +SSCOI-44479
#include "SMPLAEnterWeight.h"
#include "SMEnterQuantity.h"
// -SSCOI-44479

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("KeyInCode")

IMPLEMENT_DYNCREATE(SMKeyInCode, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMKeyInCode::SMKeyInCode()
{
  IMPLEMENT_TIMESTAMP
}

//SOTF 7670
SMStateBase * SMKeyInCode::Initialize(void)
{
    SMStateBase  *pNextState = SMKeyInCodeBase::Initialize();
	
    CString csOptionName(_T("MaxDigitsAllowCustomerKeyedUPC"));
    CString csOptionValue;
    SendTBCommand(TBCMD_GET_OPTIONS, &csOptionName, &csOptionValue);
    
    if(!ps.RemoteMode())
    {
        long lValue = _ttol(csOptionValue);
        if((lValue <= 0) || (lValue > MAX_DIGITS_ITEMCODE))
        {
            lValue = 5;
        }
        // allow the EchoInput() func to accept up to max digits
		ps.AllowInput(lValue,false,false);
    }
	
    return pNextState;
}
//sotf

void SMKeyInCode::UnInitialize(void)
{
    SMKeyInCodeBase::UnInitialize();
}

bool SMKeyInCode::PSAssistMenuLegalForState()
{
    return SMKeyInCodeBase::PSAssistMenuLegalForState();
}

SMStateBase * SMKeyInCode::PSNumericKey(const long lKey)
{
    return SMKeyInCodeBase::PSNumericKey(lKey);
}

SMStateBase * SMKeyInCode::PSClearKey(void)
{
    return SMKeyInCodeBase::PSClearKey();
}

SMStateBase * SMKeyInCode::PSEnterKey(void)
{
// +SSCOI-44479
#ifndef _CPPUNIT
	long lLen = ps.GetInput(csItemSent);
	if (lLen==0)
	{
		DMSayPhrase(KEYPADINVALID);
		return PSClearAllInput(FALSE);
	}
	
	ps.DisableButtons();	
	
	// REQ 20060728-986 Start Special Transaction
	bool bAllowDefaultProcessing = true;
	tb.PreprocessData(csItemSent, _T("KEYBOARD_DATA"), &bAllowDefaultProcessing);
	if (!bAllowDefaultProcessing)
	{
		trace(L4, _T("PreprocessData() processing returns STATE_NULL"));
		return PSClearAllInput(FALSE);
	}
	
	//+dp185016 tar 288891
	if (InvalidCouponPrefix(csItemSent))
	{
		PSClearKey();
		dm.SaySecurity(SEC_NONE, ps.GetSoundName(COUPONSNOTACCEPTED));
		return STATE_NULL;
	}
	//-dp185016
	//SASendSCOTInput(ITEMIZATION);
	SAWLDBScannerDisable();   // Disable the scanner
	
#endif // _CPPUNIT
	
	
	if (VerifyQtyItem(csItemSent))     //Tar 143309 
	{
		trace(L4, _T("SMKeyInCode::PSEnterKey >>> VerifyQtyItem(csItemSent)"));
		
		SMStateBase::ms_bIsCurrentItemFromPickList = true;
		io.Reset();
		io.csOrigCode = csItemSent;	   // item code keyed
		io.csItemCode = csItemSent;	   // assumed to be the same
		io.d.bPickListItem = true;
		if (!co.IsPicklistAssistEnable()) // dont Send SASendScot when PLA is enable, Let TBItemSale called by PLAEnterWeight do
		{
			SASendSCOTInput( UPC_MANUAL, csItemSent);
		}
		SetNextGenUICurrentItem(io.csOrigCode);
		
		// +SSCOI-44479
		return TBItemSale(false,0,false); 	
		//trace(L4, _T("SMKeyInCode::PSEnterKey >>> VerifyQtyItem(csItemSent) >> TBEnterItem()"));
		//RETURNSTATE(EnterQuantity)
		// -SSCOI-44479
	}
	g_bMultiPickSkipBaggingAllowed = false;// TAR 398289
	// + TAR 399975	
	if(co.IsInAssistMenus())
	{
		setAnchorState(BES_SCANANDBAG);		
	}
	// - TAR 399975
	if (co.IsPicklistAssistEnable() && ps.IsPickListItem(csItemSent))
	{
		io.Reset();
		io.csOrigCode = csItemSent;
		io.csItemCode = csItemSent;
		RETURNSTATE(PLAEnterWeight)
	}
	else
	{
		trace(L4, _T("SMKeyInCode::PSEnterKey >>> return TBItemSale()"));
		
		return TBItemSale(false,0,false);
  }
    //return SMKeyInCodeBase::PSEnterKey();

// -SSCOI-44479
}

SMStateBase * SMKeyInCode::PSBackSpaceKey(void)
{
    return SMKeyInCodeBase::PSBackSpaceKey();
}

SMStateBase * SMKeyInCode::PSButtonGoBack(void)
{
    return SMKeyInCodeBase::PSButtonGoBack();
}

SMStateBase * SMKeyInCode::OnBackToLGW(void)
{
    return SMKeyInCodeBase::OnBackToLGW();
}

SMStateBase * SMKeyInCode::PSButton8(void)
{
    return SMKeyInCodeBase::PSButton8();
}

SMStateBase * SMKeyInCode::DMScanner(void)
{
    return SMKeyInCodeBase::DMScanner();
}

SMStateBase * SMKeyInCode::PSButtonHelp(void)
{
    return SMKeyInCodeBase::PSButtonHelp();
}

SMStateBase * SMKeyInCode::PSSwitchContext(BOOL bRemote, CString csNewContext)
{
    return SMKeyInCodeBase::PSSwitchContext(bRemote, csNewContext);
}
