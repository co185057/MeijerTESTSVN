//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMScanAndBag.h
//
// TITLE: Class header for Scan and Bag State
//
// $Header:
//
// AUTHOR:  Feng Zhang
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SCANANDBAGSTATE
#define _SCANANDBAGSTATE

#include "SMScanAndBagBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
 SMScanAndBag : public SMScanAndBagBase
{
public:
	SMScanAndBag();
	//+4.2 - Card Dispenser
	#ifdef _TESTSSF
	protected:
	#include "Test_CardDispenser\SMScanAndBagTest.h"
	#endif
	//-4.2 - Card Dispenser
  DECLARE_DYNCREATE(SMScanAndBag)// MFC Runtime class/object information


		
};

#endif
