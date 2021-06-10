//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMInsertGiftCard.h
//
// TITLE:	Class header for InsertGiftCard state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _INSERTGIFTCARDSTATE
#define _INSERTGIFTCARDSTATE

#include "SMInsertGiftCardBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
 SMInsertGiftCard : public SMInsertGiftCardBase
{
public:
	SMInsertGiftCard();
	
  DECLARE_DYNCREATE(SMInsertGiftCard)// MFC Runtime class/object information		
};

#endif


