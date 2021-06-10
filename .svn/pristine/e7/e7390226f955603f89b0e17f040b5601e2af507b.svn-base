//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  PLAHandInViewBase.h
//
// TITLE: Class header for VegiCam enter weight state
//
//
// AUTHOR:  Charl Subingsubing
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PLAHANDINVIEWBASESTATE
#define _PLAHANDINVIEWBASESTATE

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
  SMPLAHandInViewBase : public SMCUSTOMERMODESTATE
{
public:
    SMPLAHandInViewBase();
    virtual SMStateBase  *Initialize(void);
    SMStateBase *Deliver(const BEST argPrevState);
    virtual void UnInitialize();
    virtual SMStateBase  *TimedOut(void);

    virtual SMStateBase *OnPLATransition();

protected:

    BEST m_PrevState;


  DECLARE_DYNCREATE(SMPLAHandInViewBase)// MFC Runtime class/object information
        
};

#endif