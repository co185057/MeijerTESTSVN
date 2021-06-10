//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMInvalidBarcodeBase.cpp
//
// TITLE: Class implementation for an Invalid barcode State
//
// AUTHOR:  Rhonah Entize
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMInvalidBarcode.h"      // This state
#include "DMProcedures.h"
#include "SMSmAssistMenu.h"
#include "LaneStateData.h"
DLLEXPORT extern DMProcedures dm;   // Global DM object

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("InvalidBarcodeBase")

IMPLEMENT_DYNCREATE(SMInvalidBarcodeBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMInvalidBarcodeBase::SMInvalidBarcodeBase()
{
  IMPLEMENT_TIMESTAMP
}

///////////////////////////////////////
SMStateBase *SMInvalidBarcodeBase::Initialize(void)
{
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("InvalidBarcode") );

    setAnchorState(BES_INVALIDBARCODE);	
    DisplayAuxScreen(AUX_SCREEN_USE_TOUCHSCREEN, true);

    ps.SetCMFrame(_T("InvalidBarcodeScan"));
    ps.SetCMLeadthruText(LTT_INVALIDBARCODESCAN);
    ps.Message12(MSG_INVALIDBARCODESCAN, true);

    ps.CMButtonFlash(_T("CMButton1Med"), true);
    DMSayPhrase(SETITEMASIDECONTINUEWITHPURCHASES);
    if( BES_SMMEDITADD != nStateBeforeInvalidBarcode )
    {
        m_pAppPSX->SetConfigProperty(_T("ButtonHelp"), _T("InvalidBarcodeScan"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
    }
    else
    {
        CLaneStateData laneStateData(m_pAppPSX);
        laneStateData.SendLaneStateData(_T(""), _T(""), false, true);
        //hide it
        m_pAppPSX->SetConfigProperty(_T("ButtonHelp"), _T("InvalidBarcodeScan"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
    }
    ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
    ps.ShowCMTBText(csTBMessage2Scot);	
    ps.ShowCMFrame();

    return STATE_NULL;          
}


void SMInvalidBarcodeBase::UnInitialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("InvalidBarcode") );
}

///////////////////////////////////////
SMStateBase *SMInvalidBarcodeBase::TimedOut(void)
{
    if (BES_SMMEDITADD == nStateBeforeInvalidBarcode)
    {
        return createState(BES_SMMEDITADD);
    }
    return STATE(State)::TimedOut();
}


/////////////////////////////////////
SMStateBase *SMInvalidBarcodeBase::PSButton1(void) // OK Button
{
    trace(L2, _T("SMInvalidBarcodeBase::PSButton1, nStateBeforeInvalidBarcode = %d"), nStateBeforeInvalidBarcode);

    DMScannerEnable();
    if (BES_SMMEDITADD == nStateBeforeInvalidBarcode)
    {
        return createState(BES_SMMEDITADD);
    }
    return setAndCreateAnchorState(nStateBeforeInvalidBarcode);
}

