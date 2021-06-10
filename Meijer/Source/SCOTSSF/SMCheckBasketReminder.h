//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCheckBasketReminder.h
//
// TITLE: Class header for Check Basket Reminder State
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CHECKBASKETREMINDERSTATE
#define _CHECKBASKETREMINDERSTATE

#include "SMCheckBasketReminderBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMCheckBasketReminder : public SMCheckBasketReminderBase
{
public:
    SMCheckBasketReminder();

    virtual SMStateBase *Initialize(void);
    virtual void UnInitialize(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);

	static bool bCheckTAB;

protected:
    virtual SMStateBase *TimedOut(void);
    virtual SMStateBase *PSButtonHelp(void);
    virtual SMStateBase *PSButton2(void);
    virtual SMStateBase *PSEnterKey(void);
    virtual SMStateBase *PSButtonGoBack(void);
    virtual SMStateBase *DMScanner(void);
    virtual SMStateBase *OnBackToLGW(void);

  DECLARE_DYNCREATE(SMCheckBasketReminder)// MFC Runtime class/object information

};

#endif