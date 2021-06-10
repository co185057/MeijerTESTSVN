/////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmCardManagement.CPP
//
// TITLE: Class implementation for store mode CardManagement state
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"
#include "SMSmCardManagement.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmCardManagement")

IMPLEMENT_DYNCREATE(SMSmCardManagement, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////
SMSmCardManagement::SMSmCardManagement()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSmCardManagement::Initialize(void)
{
    return SMSmCardManagementBase::Initialize();
}

void SMSmCardManagement::UnInitialize(void)
{
    SMSmCardManagementBase::UnInitialize();
}

SMStateBase * SMSmCardManagement::PSButton1(void)
{
    return SMSmCardManagementBase::PSButton1();
}

SMStateBase * SMSmCardManagement::PSButton8(void)
{
    return SMSmCardManagementBase::PSButton8();
}

SMStateBase * SMSmCardManagement::PSEnterKey(void)
{
    return SMSmCardManagementBase::PSEnterKey();
}

SMStateBase * SMSmCardManagement::PSClearKey(void)
{
    return SMSmCardManagementBase::PSClearKey();
}

SMStateBase * SMSmCardManagement::PSNumericKey(const long lKey)
{
    return SMSmCardManagementBase::PSNumericKey(lKey);
}

SMStateBase * SMSmCardManagement::PSOtherSpecialKeys(const long ch)
{
    return SMSmCardManagementBase::PSOtherSpecialKeys(ch);
}

SMStateBase * SMSmCardManagement::PSKeystroke(const long wParam, const long lParam)
{
    return SMSmCardManagementBase::PSKeystroke(wParam, lParam);
}

void SMSmCardManagement::showScreen()
{
    SMSmCardManagementBase::showScreen();
}

void SMSmCardManagement::ShowTextData(void)
{
    SMSmCardManagementBase::ShowTextData();
}

void SMSmCardManagement::ShowImgData(void)
{
    SMSmCardManagementBase::ShowImgData();
}

bool SMSmCardManagement::LoadAmt(const long lValue)
{
    return SMSmCardManagementBase::LoadAmt(lValue);
}

CString SMSmCardManagement::GetNewCylinderPosition(CString csDefRect, long lCount, long lCapacity)
{
    return SMSmCardManagementBase::GetNewCylinderPosition(csDefRect, lCount, lCapacity);
}

void SMSmCardManagement::ChangeAndShowContext(void)
{
    SMSmCardManagementBase::ChangeAndShowContext();
}

