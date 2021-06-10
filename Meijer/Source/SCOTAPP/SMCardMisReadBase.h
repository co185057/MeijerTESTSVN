//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCardMisReadBase.h
//
// TITLE: Class header for mis-read card State
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CARDMISREADBASESTATE
#define _CARDMISREADBASESTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
 SMCardMisReadBase : public SMCustomerModeState
{
public:
	SMCardMisReadBase(LPTSTR, PSTEXT Title = LTT_CARDMISREAD);
	SMCardMisReadBase();
	SMStateBase *Deliver(LPTSTR, PSTEXT Title = LTT_CARDMISREAD);
	SMCardMisReadBase(PSTEXT, PSTEXT Title = LTT_CARDMISREAD);
	SMStateBase *Deliver(PSTEXT, PSTEXT Title = LTT_CARDMISREAD);
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
	virtual SMStateBase  *OnWtIncreaseNotAllowed(void);  //TAR200412
	virtual SMStateBase  *OnWtDecrease(void); //TAR200412


  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize(void);
	
protected:
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *DMCardReader(void);
  virtual SMStateBase  *DMCardRemoved(void);
  virtual SMStateBase  *TimedOut(void);
  virtual SMStateBase  *PSEnterKey(void);
  virtual SMStateBase  *PSButtonHelp(void); //SSCOP-481
  CString csCurContext;    // Tar 238987
	
//private:
  LPTSTR charDisplayMsg;	// not used
  PSTEXT pDisplayMsg;
  bool   fTimedOutOnce;
  bool   fTimedOut; //SSCOP-481
  PSTEXT nTitle;
  BEST nGoToState;     //saves the original state we came from, in order to return there when we're done
	
  DECLARE_DYNCREATE(SMCardMisReadBase)// MFC Runtime class/object information
};

//DECLARE_STATE(CardMisRead)

#endif
