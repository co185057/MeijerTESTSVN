//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMProcessingAnalysis.H
//
// TITLE: Class header for processing StopLift Analysis status
//
// AUTHOR: Rhonah Entize
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SECURITYANALYSISBASESTATE
#define _SECURITYANALYSISBASESTATE

#ifdef _CPPUNIT
   #include "FakeSMCustomerModeState.h"
#else
   #include "SMCustomerModeState.h"
#endif // _CPPUNIT

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif

SMSecurityAnalysisBase : public SMCUSTOMERMODESTATE
{

public:
	SMSecurityAnalysisBase();
	virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize();
    virtual SMStateBase  *TimedOut(void);

protected:

	DECLARE_DYNCREATE(SMSecurityAnalysisBase)// MFC Runtime class/object information
};

#endif
