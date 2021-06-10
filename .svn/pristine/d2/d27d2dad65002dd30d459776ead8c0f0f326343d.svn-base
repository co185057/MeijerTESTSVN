//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSecurityAnalysis.cpp
//
// TITLE: Class implementation for processing StopLift Analysis status
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // common includes

#include "SMState.h"                // base state
#include "SMSecurityAnalysis.h"           // this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SecurityAnalysis")
                                                   

IMPLEMENT_DYNCREATE(SMSecurityAnalysis, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

////////////////////////////
SMSecurityAnalysis::SMSecurityAnalysis()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSecurityAnalysis::Initialize(void)
{
    return SMSecurityAnalysisBase::Initialize();
}

void SMSecurityAnalysis::UnInitialize(void)
{
    SMSecurityAnalysisBase::UnInitialize();
}

SMStateBase * SMSecurityAnalysis::TimedOut(void)
{
    return SMSecurityAnalysisBase::TimedOut();
}
