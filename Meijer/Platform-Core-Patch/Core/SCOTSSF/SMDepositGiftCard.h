//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMDepositGiftCard.h
//
// TITLE: Class header for DepositGiftCard State
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DEPOSITGIFTCARDSTATE
#define _DEPOSITGIFTCARDSTATE

#include "SMDepositGiftCardBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMDepositGiftCard : public SMDepositGiftCardBase
{
public:
	SMDepositGiftCard();
	
  DECLARE_DYNCREATE(SMDepositGiftCard)// MFC Runtime class/object information
};

#endif