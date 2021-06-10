//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCollectGiftCard.h
//
// TITLE: Class header for CollectGiftCard state
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _COLLECTGIFTCARDSTATE
#define _COLLECTGIFTCARDSTATE

#include "SMCollectGiftCardBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMCollectGiftCard : public SMCollectGiftCardBase
{
public:
	SMCollectGiftCard();
	
  DECLARE_DYNCREATE(SMCollectGiftCard)// MFC Runtime class/object information
};

#endif
