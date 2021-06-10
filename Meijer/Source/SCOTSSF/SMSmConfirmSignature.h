//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmConfirmSignature.h
//
// TITLE: Class header for Confirm Signature
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CONFIRMSIGNATURESTATE
#define _CONFIRMSIGNATURESTATE

#include "SMSmConfirmSignatureBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmConfirmSignature : public SMSmConfirmSignatureBase
{
public:
    SMSmConfirmSignature();
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual SMStateBase  *OnWtDecrease(void);

protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);
    virtual SMStateBase  *TimedOut(void);
    virtual SMStateBase  *PSButtonHelp(void);

    DECLARE_DYNCREATE(SMSmConfirmSignature)// MFC Runtime class/object information
};

#endif