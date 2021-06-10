//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSecurityAnalysis.CPP
//
// TITLE: Class implementation for processing StopLift Analysis status
//
// AUTHOR: Rhonah Entize
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header
#ifdef _CPPUNIT
    #include "FakeCommon.h"
#endif


#ifndef _CPPUNIT
#include "Common.h"                 // MGV common includes
#include "SMState.h"                // MGV base state
#endif //CPPUNIT
#include "SMSecurityAnalysis.h"   
#include "SMFinish.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SecurityAnalysisBase")


DLLEXPORT extern HWND hw; 
IMPLEMENT_DYNCREATE(SMSecurityAnalysisBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////////////
SMSecurityAnalysisBase::SMSecurityAnalysisBase()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase  *SMSecurityAnalysisBase::Initialize(void)
{
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SecurityAnalysis") );

    g_bSecurityAnalysisTimerExpires = false;
    bDoNotResetTimeOutOnDMEvent = true;
	
    nTimeOut = g_nSecurityAnalysisTimer;
	setAnchorState(BES_SECURITYANALYSIS);
	UpdateStateName(ps.GetPSText(RA_SECURITYANALYSIS, SCOT_LANGUAGE_PRIMARY));
	ps.SetCMFrame(_T("SecurityAnalysis"));

    ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
    ps.ShowCMFrame();

    return STATE_NULL;
}

void SMSecurityAnalysisBase::UnInitialize(void)
{
    bDoNotResetTimeOutOnDMEvent = false;
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SecurityAnalysis") );
}

SMStateBase  *SMSecurityAnalysisBase::TimedOut(void)
{
	trace(L6, _T("+SMSecurityAnalysisBase::TimedOut"));
	nTimeOut = 0;
	g_bSecurityAnalysisTimerExpires = true;
	RETURNSTATE(Finish)
}

