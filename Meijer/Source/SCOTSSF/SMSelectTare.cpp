//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSelectTare.cpp
//
// TITLE: Class implementation for select Tare state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMSelectTare.h"           // MGV this state


#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SelectTare")

IMPLEMENT_DYNCREATE(SMSelectTare, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSelectTare::SMSelectTare()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSelectTare::Initialize(void)
{
    return SMSelectTareBase::Initialize();
}

void SMSelectTare::UnInitialize(void)
{
    SMSelectTareBase::UnInitialize();
}

bool SMSelectTare::PSAssistMenuLegalForState()
{
    return SMSelectTareBase::PSAssistMenuLegalForState();
}

SMStateBase * SMSelectTare::PSButton1(void)
{
    return SMSelectTareBase::PSButton1();
}

SMStateBase * SMSelectTare::PSButton4(void)
{
    return SMSelectTareBase::PSButton4();
}

SMStateBase * SMSelectTare::PSListLookup(CString cItemCode)
{
    // Override the base implementation to allow tare code = 0
  long lTare = _ttoi(cItemCode);	
  io.lTareCode = lTare;
  m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("SelectTare"), UI::PROPERTYVISIBLE, VARIANT_FALSE); //tar 330491
  
  return TBEnterItem(false);
}

SMStateBase * SMSelectTare::PSCancelKey(void)
{
    return SMSelectTareBase::PSCancelKey();
}

SMStateBase * SMSelectTare::PSListScroll(const long lScroll)
{
    return SMSelectTareBase::PSListScroll(lScroll);
}

SMStateBase * SMSelectTare::PSButtonGoBack(void)
{
    return SMSelectTareBase::PSButtonGoBack();
}

SMStateBase * SMSelectTare::PSReceiptUp(void)
{
    return SMSelectTareBase::PSReceiptUp();
}

SMStateBase * SMSelectTare::PSReceiptDown(void)
{
    return SMSelectTareBase::PSReceiptDown();
}
