//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMPLAHandInView.h
//
// TITLE: Class header for lookup item(type item description) state
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PLAHANDINVIEWSTATE
#define _PLAHANDINVIEWSTATE

#include "SMPLAHandInViewBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
 SMPLAHandInView : public SMPLAHandInViewBase
{
public:
    SMPLAHandInView();
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize();
    virtual SMStateBase  *TimedOut(void);
    virtual SMStateBase *OnPLATransition();


    DECLARE_DYNCREATE(SMPLAHandInView) // MFC Runtime class/object information
        
};

#endif