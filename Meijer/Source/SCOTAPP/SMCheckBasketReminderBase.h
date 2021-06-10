//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCheckBasketReminderBase.h
//
// TITLE: Class header for Check Basket Reminder State
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CheckBasketReminderBaseSTATE
#define _CheckBasketReminderBaseSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMCheckBasketReminderBase : public SMCustomerModeState
{
public:
	SMCheckBasketReminderBase();
  virtual SMStateBase *Initialize(void);
  virtual void UnInitialize(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void); //tar 201569

protected:
  virtual SMStateBase *TimedOut(void);
  virtual SMStateBase *PSButtonHelp(void);
  virtual SMStateBase *PSButton2(void);
  virtual SMStateBase *PSEnterKey(void);
  virtual SMStateBase *PSButtonGoBack(void);
  virtual SMStateBase *DMScanner(void);
  virtual SMStateBase *OnBackToLGW(void);				// Smart Scale reported that the unmatched weight was removed
	
  DECLARE_DYNCREATE(SMCheckBasketReminderBase)// MFC Runtime class/object information
		
};

//DECLARE_STATE(CheckBasketReminder)

#endif