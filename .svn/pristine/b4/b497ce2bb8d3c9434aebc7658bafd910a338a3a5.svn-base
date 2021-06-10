//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMPLAEnterWeight.cpp
//
// TITLE: Class implementation for select item state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // common includes

#include "SMState.h"                // base state
#include "SMPLAEnterWeight.h"           // this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("PLAEnterWeight")
								                   

IMPLEMENT_DYNCREATE(SMPLAEnterWeight, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

////////////////////////////
SMPLAEnterWeight::SMPLAEnterWeight()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMPLAEnterWeight::Initialize(void)
{
    return SMPLAEnterWeightBase::Initialize();
}

void SMPLAEnterWeight::UnInitialize(void)
{
    SMPLAEnterWeightBase::UnInitialize();
}

SMStateBase * SMPLAEnterWeight::TimedOut(void)
{
    return SMPLAEnterWeightBase::TimedOut();
}

SMStateBase *SMPLAEnterWeight::OnPLATransition()
{
    return SMPLAEnterWeightBase::OnPLATransition();
}

bool SMPLAEnterWeight::WaitForScaleRequired(bool bWeightChange)
{
    return SMPLAEnterWeightBase::WaitForScaleRequired(bWeightChange);
}

SMStateBase * SMPLAEnterWeight::DMScale(void)
{
    return SMPLAEnterWeightBase::DMScale();
}

SMStateBase * SMPLAEnterWeight::PSButton1(void)
{
    return SMPLAEnterWeightBase::PSButton1();
}

SMStateBase * SMPLAEnterWeight::PSButton3(void)
{
    return SMPLAEnterWeightBase::PSButton3();
}

SMStateBase * SMPLAEnterWeight::ProcessWeightOnScale(void)
{
    return SMPLAEnterWeightBase::ProcessWeightOnScale();
}

SMStateBase * SMPLAEnterWeight::PSButtonGoBack(void)
{
    return SMPLAEnterWeightBase::PSButtonGoBack();
}

SMStateBase * SMPLAEnterWeight::PSCancelKey(void)
{
    return SMPLAEnterWeightBase::PSCancelKey();
}
