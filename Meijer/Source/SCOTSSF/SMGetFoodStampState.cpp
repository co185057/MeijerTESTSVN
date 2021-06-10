//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMGetFoodStampState.cpp
//
// Description: The GetFoodStampState state is used when the transition from a
//        state to the next state might take some time.  GetFoodStampState is a
//        "chameleon" state with respect to Customer Mode, Store Mode,
//        and Help Mode - it becomes the mode of the creating state and
//        therefore does not inherit from the above classes.
//
//
// Author:  Michael Prather
//
//////////////////////////////////////////////////////////////////////////////////////////////////
 
#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMGetFoodStampState.h"           // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID _T("GetFoodStampState")

IMPLEMENT_DYNCREATE(SMGetFoodStampState, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

////////////////////////////////////////////////
SMGetFoodStampState::SMGetFoodStampState()
{
  IMPLEMENT_TIMESTAMP
		
}

SMStateBase * SMGetFoodStampState::Initialize(void)
{
    return SMGetFoodStampStateBase::Initialize();
}

void SMGetFoodStampState::UnInitialize(void)
{
    SMGetFoodStampStateBase::UnInitialize();
}

bool SMGetFoodStampState::GetFoodStampState()
{
    return SMGetFoodStampStateBase::GetFoodStampState();
}

SMStateBase * SMGetFoodStampState::OnWtIncreaseNotAllowed(void)
{
    return SMGetFoodStampStateBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMGetFoodStampState::OnWtDecrease(void)
{
    return SMGetFoodStampStateBase::OnWtDecrease();
}

SMStateBase * SMGetFoodStampState::TimedOut()
{
    return SMGetFoodStampStateBase::TimedOut();
}

SMStateBase * SMGetFoodStampState::DMScanner()
{
    return SMGetFoodStampStateBase::DMScanner();
}

SMStateBase * SMGetFoodStampState::DMCardReader()
{
    return SMGetFoodStampStateBase::DMCardReader();
}
