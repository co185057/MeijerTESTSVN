//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  LaneStateData.CPP
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "LaneStateData.h"
#include "LibraryHelper.h"


#define COMP_ID ID_SM               // base state component
#define T_ID     _T("CLaneStateData")



LPCTSTR CLaneStateData::LOADING=_T("Loading");
LPCTSTR CLaneStateData::OUTOFTRANSACTION=_T("OutOfTransaction");
LPCTSTR CLaneStateData::INTRANSACTION=_T("InTransaction");
LPCTSTR CLaneStateData::INTRANSACTIONVOID=_T("InTransactionVoid");
LPCTSTR CLaneStateData::STORE=_T("Store");
CString CLaneStateData::g_csPreviousNextGenUIAppState = LOADING;
CString CLaneStateData::g_csCurrentNextGenUIAppState = LOADING;
LPCTSTR CLaneStateData::ENDOFTRANSACTION=_T("EndOfTransaction");

bool CLaneStateData::g_bOverride = false;


#ifdef _CPPUNIT
#define SMStateBase FakeSMStateBase
#endif

CLaneStateData::CLaneStateData(CPSX *pPSX) :
   m_pLibraryHelper(new CLibraryHelper())
{
    m_pPSX = pPSX;
}

CLaneStateData::CLaneStateData(CPSX *pPSX, CLibraryHelper &helper) :
    m_pPSX(pPSX),
        m_pLibraryHelper(&helper)
{
}

CLaneStateData::~CLaneStateData()
{
    delete m_pLibraryHelper;
}

CString CLaneStateData::CreateLaneChangeStateData(bool bStoreMode)
{
    CString csData = _T("");
    CString csCurrentLaneState = _T("");

    if(g_bOverride)
    {
        csCurrentLaneState = m_csOverrideLaneState;
    }
    else
    {
        csCurrentLaneState =  m_csLaneState;
    }

    if((csCurrentLaneState.Find(_T("BagAndEAS")) != -1))
    {
        if (co.IsInAssistMenus())
            csData = STORE;
        else
            csData = INTRANSACTION;
    }
    else if(((co.IsInAssistMenus() || bStoreMode)))
    {
        csData = STORE;
    }
    else if((csCurrentLaneState.Find(_T("OutOfService")) != -1) ||
        (csCurrentLaneState.Find(_T("OutOfService2")) != -1)||
        (csCurrentLaneState.Find(_T("SMSystemMessageStartup")) != -1))
    {
        csData = LOADING;
    }
    else if (csCurrentLaneState.Find(_T("Attract")) != -1)
    {
        csData = OUTOFTRANSACTION;
    }
    else if((csCurrentLaneState.Find(_T("ScanAndBag")) != -1))
	{
		csData = INTRANSACTION;
	}
	else if((csCurrentLaneState.Find(_T("VoidItem")) != -1) || (csCurrentLaneState.Find(_T("VoidItemWithReward")) != -1) 
		|| (csCurrentLaneState.Find(_T("ConfirmVoid")) != -1) || (csCurrentLaneState.Find(_T("ConfirmAbort")) != -1)
		|| (csCurrentLaneState.Find(_T("VoidApproval")) != -1))
	{
		csData = INTRANSACTIONVOID;
	}
    else if((csCurrentLaneState.Find(_T("KeyInCode")) != -1) || (csCurrentLaneState.Find(_T("ProduceFavorites")) != -1))
    {
        csData = INTRANSACTION;
    }
	else if (csCurrentLaneState.Find(_T("TakeReceipt")) != -1)
    {
        csData = INTRANSACTION;
    }
    else if (csCurrentLaneState.Find(_T("Finish")) != -1)
    {
        csData = INTRANSACTION;
    }
    else if (csCurrentLaneState.Find(_T("ThankYou")) != -1)
    {
        csData = ENDOFTRANSACTION;
    }
    else if (csCurrentLaneState.Find(_T("SystemMessage1")) != -1 ||
        csCurrentLaneState.Find(_T("SystemMessage2")) != -1 ||
        csCurrentLaneState.Find(_T("SystemMessage3")) != -1)
    {
        csData = STORE;
    }


    return csData;
}

void CLaneStateData::SendLaneStateData(CString csLaneState, CString csOverrideLaneState, bool bOverride, bool bStoreMode)
{
    m_csOverrideLaneState = csOverrideLaneState;
    m_csLaneState = csLaneState;
    g_bOverride = bOverride;

    if(!ps.RemoteMode())
    {
        g_csCurrentNextGenUIAppState = CreateLaneChangeStateData(bStoreMode);
    }
    else
    {
        g_csCurrentNextGenUIAppState = _T("");
    }
}

void CLaneStateData::SendNextGenUIAppState()
{
    COleVariant v;

    m_pPSX->GetTransactionVariable(_T("NextGenUIApplicationState"), v);

    CString csCurrentData = v.bstrVal;
    trace (L6, _T("+SendNextGenUIAppState(): csCurrentData = %s, g_csCurrentNextGenUIAppState = %s, g_csPreviousNextGenUIAppState = %s, g_bOverride = %d, co.IsInAssistMenus() = %d"),
       csCurrentData, g_csCurrentNextGenUIAppState, g_csPreviousNextGenUIAppState, g_bOverride, co.IsInAssistMenus());
    if(!g_csCurrentNextGenUIAppState.IsEmpty())
    {
        if (csCurrentData.Compare(g_csCurrentNextGenUIAppState) != 0)
        {
            g_csPreviousNextGenUIAppState = csCurrentData;
            v = g_csCurrentNextGenUIAppState;
            m_pPSX->SetTransactionVariable(_T("NextGenUIApplicationState"), v);
            v = g_csPreviousNextGenUIAppState;
            m_pPSX->SetTransactionVariable(_T("NextGenUIPreviousApplicationState"), v);
        }
    }
    else
    {
        if ((csCurrentData.Find(STORE) != -1) && !co.IsInAssistMenus())
        {
            v = g_csPreviousNextGenUIAppState;
            m_pPSX->SetTransactionVariable(_T("NextGenUIApplicationState"), v);
            g_csPreviousNextGenUIAppState = csCurrentData;
            v = g_csPreviousNextGenUIAppState;
            m_pPSX->SetTransactionVariable(_T("NextGenUIPreviousApplicationState"), v);
        }
    }
}

void CLaneStateData::SendNextGenUIData(CString csTransactionVariableName, COleVariant vValue)
{
    m_pPSX->SetTransactionVariable(csTransactionVariableName, vValue);
}

void CLaneStateData::SendNextGenUIData(CString csTransactionVariableName, bool bValue)
{
    if (bValue)
    {
        m_pPSX->SetTransactionVariable(csTransactionVariableName, COleVariant((long)VARIANT_TRUE));
    }
    else
    {
        m_pPSX->SetTransactionVariable(csTransactionVariableName, COleVariant((long)VARIANT_FALSE));
    }
}

void CLaneStateData::SendNextGenUIData(CString csTransactionVariableName, CString csValue)
{
    COleVariant v;
    v = csValue;
    m_pPSX->SetTransactionVariable(csTransactionVariableName, v);
}

void CLaneStateData::SendNextGenUIData(CString csTransactionVariableName, long lValue)
{
    COleVariant v;
    v = lValue;
    m_pPSX->SetTransactionVariable(csTransactionVariableName, v);
}

CString CLaneStateData::CreateNextGenUIModeData(bool bAttendantMode, bool bTrainingMode, long lDegradedMode)
{
    CKEYVALUEPARMLIST parmList;
    CString csValue = _T("");

    CKEYVALUE<bool>::Put(parmList, _T("attendant"), bAttendantMode, NULL);
    CKEYVALUE<bool>::Put(parmList, _T("training"), bTrainingMode, NULL);

    if(lDegradedMode == DEGRADEDMODE_CASH_ONLY)
    {
        csValue = _T("cash");
    }
    else if(lDegradedMode == DEGRADEDMODE_CARD_ONLY)
    {
        csValue = _T("credit");
    }
    else
    {
        csValue = _T("false");
    }

    CKEYVALUE<const _TCHAR*>::Put(parmList, _T("degraded"), csValue, NULL);

    CString csParmList((const _TCHAR *)parmList);

    trace (L6, _T("-CreateNextGenUIModeData(): csParmList = %s"), csParmList);

    return csParmList;

}

CString CLaneStateData::CreateNextGenUIHotKeyState(LPCTSTR HKState, bool bShiftOn)
{
    CKEYVALUEPARMLIST parmList;
    CString csValue = _T("");

    CKEYVALUE<const _TCHAR*>::Put(parmList, _T("HKState"), HKState, NULL);
    CKEYVALUE<bool>::Put(parmList, _T("ShiftOn"), bShiftOn, NULL);

    CString csParmList((const _TCHAR *)parmList);

    trace (L6, _T("-CreateNextGenUIHotKeyState(): csParmList = %s"), csParmList);

    return csParmList;

}

void CLaneStateData::ResetNextGenDataTrxnVariables(void)
{
    SendNextGenUIData(_T("IsQuickPick"), (bool)false);
    SendNextGenUIData(_T("NextGenUIChangeDue"),0L);
    SendNextGenUIData(_T("NextGenUIRefundDue"),0L);
    SendNextGenUIData(_T("NextGenUIAmountDue"),0L);
    SendNextGenUIData(_T("NextGenUIAmountPaid"),0L);
    SendNextGenUIData(_T("NextGenUITBLastState"), 0L );
    SendNextGenUIData(_T("NextGenUIVolumeControlIncLevel"), 0L);
    SendNextGenUIData(_T("NextGenData"), (CString)EMPTY_STRING );    
    SendNextGenUIData(_T("NextGenUISetFrame"), (CString)EMPTY_STRING );
    SendNextGenUIData(_T("NextGenUIEchoField"), (CString)EMPTY_STRING );
    SendNextGenUIData(_T("NextGenUICurrentItem"), (CString)EMPTY_STRING );
    SendNextGenUIData(_T("NextGenUIHotKeyState"), (CString)EMPTY_STRING );
    SendNextGenUIData(_T("NextGenUIInterventionMode"),(CString)_T("none"));
    SendNextGenUIData(_T("NextGenUIApplicationState"),(CString)EMPTY_STRING);
    SendNextGenUIData(_T("NextGenUIPreviousApplicationState"),(CString)EMPTY_STRING);
    SendNextGenUIData(_T("NextGenUIPicklistDisplayLevels"), (CString)EMPTY_STRING);
	SendNextGenUIData(_T("TerminalType"), (CString)EMPTY_STRING);
    SendNextGenUIData(_T("NextGenUIMode"), (CString)_T("attendant=false;training=false;degraded=false") );
}

