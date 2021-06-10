//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSecurityAnalysis.h
//
// TITLE: Class header for processing StopLift Analysis status
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SECURITYANALYSISSTATE
#define _SECURITYANALYSISSTATE

#include "SMSecurityAnalysisBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
 SMSecurityAnalysis : public SMSecurityAnalysisBase
{
public:
    SMSecurityAnalysis();

    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize();
    virtual SMStateBase  *TimedOut(void);


    DECLARE_DYNCREATE(SMSecurityAnalysis) // MFC Runtime class/object information
        
};

#endif