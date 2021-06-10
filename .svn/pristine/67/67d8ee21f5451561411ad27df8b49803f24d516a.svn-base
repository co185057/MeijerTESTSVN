//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMInSAProgress.cpp
//
// Description: The InSAProgress state is used when the transition from 
//			TB_ITEMSOLD to the next Security event, which is the SASERVER WLBD
//			lookup return, and might take some time.  
//
// Author:  Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////
 
#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMInSAProgress.h"           // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID _T("InSAProgress")

IMPLEMENT_DYNCREATE(SMInSAProgress, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////////////////////
SMInSAProgress::SMInSAProgress()
{
  IMPLEMENT_TIMESTAMP
		
}

SMStateBase * SMInSAProgress::Initialize(void)
{
    return SMInSAProgressBase::Initialize();
}

void SMInSAProgress::UnInitialize(void)
{
    SMInSAProgressBase::UnInitialize();
}

bool SMInSAProgress::inProgress()
{
    return SMInSAProgressBase::inProgress();
}

bool SMInSAProgress::storeMode()
{
    return SMInSAProgressBase::storeMode();
}

bool SMInSAProgress::helpMode()
{
    return SMInSAProgressBase::helpMode();
}

SMStateBase * SMInSAProgress::OnDataBaseInfoComplete(void)
{
    return SMInSAProgressBase::OnDataBaseInfoComplete();
}

SMStateBase * SMInSAProgress::TimedOut(void)
{
    return SMInSAProgressBase::TimedOut();
}

SMStateBase * SMInSAProgress::DMScanner(void)
{
    return SMInSAProgressBase::DMScanner();
}

SMStateBase * SMInSAProgress::DMCardReader(void)
{
    return SMInSAProgressBase::DMCardReader();
}

SMStateBase * SMInSAProgress::OnBackToLGW(void)
{
    return SMInSAProgressBase::OnBackToLGW();
}

SMStateBase * SMInSAProgress::OnWtIncreaseNotAllowed(void)
{
    return SMInSAProgressBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMInSAProgress::OnWtDecrease(void)
{
    return SMInSAProgressBase::OnWtDecrease();
}

SMStateBase * SMInSAProgress::DMdf_EASNoMotion(void)
{
    return SMInSAProgressBase::DMdf_EASNoMotion();
}

SMStateBase * SMInSAProgress::PSButtonHelp(void)
{
    return SMInSAProgressBase::PSButtonHelp();
}
