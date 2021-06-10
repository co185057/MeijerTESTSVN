// BaseMHPINPad.cpp: implementation of the CBaseMHPINPad class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BaseMHPINPad.h"
#include "TraceSystem.h"
#include "TBUtilities.h"
#include "BaseCurrentPINPad.h"
#include "BaseObjectManager.h"


#define T_ID _T("CBaseMHPINPad")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseMHPINPad::CBaseMHPINPad()
{

}

CBaseMHPINPad::~CBaseMHPINPad()
{

}

// This is required functionality for all MH classes
TBSTATE CBaseMHPINPad::ProcessMessage(CTBMsgObj *pMsgObj)
{
    TBSTATE rc = TB_IGNORE;
    tbtrace( TM_ENTRY, _T("+-ProcessMessage"));
    return rc;
}

TBRC CBaseMHPINPad::GetPINWorkingKey(PSPINWorkingKey psPINWorkingKey)
{
    TBRC rc = TB_SUCCESS;
    LPCTSTR  lpThisName = _T("CBaseMHPINPad::GetPINWorkingKey()");
    tbtrace( TM_ENTRY, _T("+%s"), lpThisName);

    CBaseCurrentPINPad* pCurrPINPad = CCustomerCurrentPINPad::instance();

    CopyString(psPINWorkingKey->szAccountNumber, pCurrPINPad->Get_PWK_AccountNumber());
    psPINWorkingKey->nAccountNumberLength = pCurrPINPad->Get_PWK_AccountNumberLength();
    CopyString(psPINWorkingKey->szWorkingKey, pCurrPINPad->Get_PWK_WorkingKey());
    psPINWorkingKey->nWorkingKeyLength = pCurrPINPad->Get_PWK_WorkingKeyLength();
    psPINWorkingKey->nPinBlockFormat = pCurrPINPad->Get_PWK_PinBlockFormat();
    psPINWorkingKey->nPinpadSystem = pCurrPINPad->Get_PWK_PinpadSystem();
    psPINWorkingKey->nPinBlockEncoding = pCurrPINPad->Get_PWK_PinBlockEncoding();
    psPINWorkingKey->lUnused3 = pCurrPINPad->Get_PWK_Unused3();

    tbtrace( TM_ENTRY, _T("-%s"), lpThisName);
    return rc;
}

void CBaseMHPINPad::SetPINWorkingKey(PSPINWorkingKey psPINWorkingKey)
{
    LPCTSTR  lpThisName = _T("CBaseMHPINPad::SetPINWorkingKey()");
    tbtrace( TM_ENTRY, _T("+%s"), lpThisName);

    CBaseCurrentPINPad* pCurrPINPad = CCustomerCurrentPINPad::instance();

    pCurrPINPad->Set_PWK_AccountNumber(psPINWorkingKey->szAccountNumber);
    pCurrPINPad->Set_PWK_AccountNumberLength(psPINWorkingKey->nAccountNumberLength);
    pCurrPINPad->Set_PWK_WorkingKey(psPINWorkingKey->szWorkingKey);
    pCurrPINPad->Set_PWK_WorkingKeyLength(psPINWorkingKey->nWorkingKeyLength);
    pCurrPINPad->Set_PWK_PinBlockFormat(psPINWorkingKey->nPinBlockFormat);
    pCurrPINPad->Set_PWK_PinpadSystem(psPINWorkingKey->nPinpadSystem);
    pCurrPINPad->Set_PWK_PinBlockEncoding(psPINWorkingKey->nPinBlockEncoding);
    pCurrPINPad->Set_PWK_Unused3(psPINWorkingKey->lUnused3);

    tbtrace( TM_ENTRY, _T("-%s"), lpThisName);
}


