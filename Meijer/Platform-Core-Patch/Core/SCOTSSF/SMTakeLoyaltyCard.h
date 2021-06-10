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
	
  DECLARE_DYNCREATE(SMTakeLoyaltyCard)// MFC Runtime class/object information
};

#endif
