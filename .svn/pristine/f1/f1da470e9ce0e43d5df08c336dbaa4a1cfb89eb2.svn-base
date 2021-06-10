//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTakeLoyaltyCardBase.h
//
// TITLE: Class header for Take LoyaltyCard State
//
// $Header: /Fastlane/4.0 Patch B/Core/Source/SCOTAPP/SMTakeLoyaltyCardBase.h 1     1/26/05 2:10p Bd123405 $
//
// AUTHOR:  
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAKELOYALTYCARDBASESTATE_
#define _TAKELOYALTYCARDBASESTATE_

#include "SMCustomerModeState.h"


#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
 SMTakeLoyaltyCardBase : public SMCustomerModeState
{
public:
	SMTakeLoyaltyCardBase(TBLOYALTYCARDTYPE LoyaltyCardType = LC_INVALID);
	SMStateBase *Deliver(TBLOYALTYCARDTYPE LoyaltyCardType = LC_INVALID);
	virtual SMStateBase *Initialize(void);
	virtual void     UnInitialize(void);
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
protected:
	bool    fTimedOutOnce;
	virtual SMStateBase *TimedOut(void);
	virtual SMStateBase *DMCardRemoved(void);
	virtual SMStateBase *OnBackToLGW(void); //TAR256942: Smart Scale reported that the unmatched weight was removed
	virtual SMStateBase *OnWtIncreaseNotAllowed(); //TAR256942

//private:
	TBLOYALTYCARDTYPE     fCardType;
	bool	bUnexpectedIncHappened;
	
  DECLARE_DYNCREATE(SMTakeLoyaltyCardBase)	// MFC Runtime class/object information
};

//DECLARE_STATE(TakeLoyaltyCard)

#endif // _TAKELOYALTYCARDBASESTATE_
