//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMScanCard.h
//
// TITLE: Class header for Confirn Signature
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ScanCardSTATE
#define _ScanCardSTATE

#include "SMScanCardBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMScanCard : public SMScanCardBase
{

public:

	SMScanCard(const TBTENDERTYPE, const long);
	SMScanCard();
	SMStateBase *Deliver(const TBTENDERTYPE, const long);

  DECLARE_DYNCREATE(SMScanCard)// MFC Runtime class/object information
};


#endif
