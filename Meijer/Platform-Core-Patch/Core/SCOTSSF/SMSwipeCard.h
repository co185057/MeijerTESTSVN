//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSwipeCard.h
//
// TITLE: Class header for Confirn Signature
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SwipeCardSTATE
#define _SwipeCardSTATE

#include "SMSwipeCardBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSwipeCard : public SMSwipeCardBase
{

public:

	SMSwipeCard(const TBTENDERTYPE, const long);
	SMSwipeCard();
	SMStateBase *Deliver(const TBTENDERTYPE, const long);
	
  DECLARE_DYNCREATE(SMSwipeCard)// MFC Runtime class/object information
};


#endif