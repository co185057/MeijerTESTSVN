//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMConfirmVoid.h
//
// TITLE: Class header for Confirm Void
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CONFIRMVOIDSTATE
#define _CONFIRMVOIDSTATE

#include "SMConfirmVoidBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMConfirmVoid : public SMConfirmVoidBase
{
public:
	SMConfirmVoid();

    virtual SMStateBase  *Initialize(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual SMStateBase  *OnWtDecrease(void);
    virtual bool PSAssistMenuLegalForState();
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);


  DECLARE_DYNCREATE(SMConfirmVoid)// MFC Runtime class/object information
};

#endif