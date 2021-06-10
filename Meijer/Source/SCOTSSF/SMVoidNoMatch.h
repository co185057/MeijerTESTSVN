//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMVoidNoMatch.h
//
// TITLE: Class header for void item no match State
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _VOIDNOMATCHSTATE
#define _VOIDNOMATCHSTATE

#include "SMVoidNoMatchBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMVoidNoMatch : public SMVoidNoMatchBase
{
public:
    SMVoidNoMatch();
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize();
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual SMStateBase  *OnWtDecrease(void);
    virtual bool PSAssistMenuLegalForState();
protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSEnterKey(void);

    DECLARE_DYNCREATE(SMVoidNoMatch)// MFC Runtime class/object information

};

#endif