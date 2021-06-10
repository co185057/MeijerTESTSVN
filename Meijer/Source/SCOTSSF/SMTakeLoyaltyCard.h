//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTakeLoyaltyCard.h
//
// TITLE: Class header for Take LoyaltyCard State
//
//
// AUTHOR:    SCOT CORE TEAM, NCR Future Retail, ATLANTA
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAKELOYALTYCARD_STATE
#define _TAKELOYALTYCARD_STATE

#include "SMTakeLoyaltyCardBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMTakeLoyaltyCard : public SMTakeLoyaltyCardBase
{
	
public:
    SMTakeLoyaltyCard(TBLOYALTYCARDTYPE LoyaltyCardType = LC_INVALID);
    SMStateBase *Deliver(TBLOYALTYCARDTYPE LoyaltyCardType = LC_INVALID);

    virtual SMStateBase *Initialize(void);
    virtual void     UnInitialize(void);
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
protected:
    virtual SMStateBase *TimedOut(void);
    virtual SMStateBase *DMCardRemoved(void);
    virtual SMStateBase *OnBackToLGW(void);
    virtual SMStateBase *OnWtIncreaseNotAllowed();

    DECLARE_DYNCREATE(SMTakeLoyaltyCard)// MFC Runtime class/object information
};

#endif
