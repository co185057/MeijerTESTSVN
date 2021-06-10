//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMEnterWeight.CPP
//
// TITLE: Class implementation for enter weight state
//
//
// AUTHOR:    Peter Denhoed
//
// POS39476/POS53487 WORK REQUEST:52635 Name: Robert Susanto Date: January 16, 2013
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMEnterWeight.h"          // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("EnterWeight")

IMPLEMENT_DYNCREATE(SMEnterWeight, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMEnterWeight::SMEnterWeight()
{
  IMPLEMENT_TIMESTAMP
}

//POS39476/53487 - Display the message 488 - Place Item On Scale
SMStateBase * SMEnterWeight::Initialize(void)
{
    SMStateBase *returnState = SMEnterWeightBase::Initialize();
	
    ps.Message12(MSG_PLACEITEMONSCALE, true);
    ps.ShowCMFrame();    
	
    return returnState;
}

void SMEnterWeight::UnInitialize(void)
{
    SMEnterWeightBase::UnInitialize();
}

bool SMEnterWeight::PSAssistMenuLegalForState()
{
    return SMEnterWeightBase::PSAssistMenuLegalForState();
}

SMStateBase * SMEnterWeight::PSButton3(void)
{
    return SMEnterWeightBase::PSButton3();
}

SMStateBase * SMEnterWeight::PSCancelKey(void)
{
    return SMEnterWeightBase::PSCancelKey();
}

SMStateBase * SMEnterWeight::DMScale(void)
{
    return SMEnterWeightBase::DMScale();
}

SMStateBase * SMEnterWeight::PSButtonGoBack(void)
{
    return SMEnterWeightBase::PSButtonGoBack();
}

SMStateBase * SMEnterWeight::PSButton1(void)
{
    return SMEnterWeightBase::PSButton1();
}

SMStateBase * SMEnterWeight::ProcessWeightOnScale()
{
    return SMEnterWeightBase::ProcessWeightOnScale();
}

void SMEnterWeight::HandleEnterWeightTimedOut()
{
    SMEnterWeightBase::HandleEnterWeightTimedOut();
}

SMStateBase * SMEnterWeight::TimedOut(void)
{
    return SMEnterWeightBase::TimedOut();
}

bool SMEnterWeight::ShowAcceptWeightBtnAndStartEnterWeightTimer(bool bWeightChange )
{
    return SMEnterWeightBase::ShowAcceptWeightBtnAndStartEnterWeightTimer(bWeightChange);
}
