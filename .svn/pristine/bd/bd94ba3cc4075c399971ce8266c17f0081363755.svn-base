//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMScanAndBag.CPP
//
// TITLE: Class header for Scan and Bag State
//
// $Header:
//
// AUTHOR:    Feng Zhang
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes
#include "SMState.h"                // MGV base state
#include "SMScanAndBag.h"           // MGV this state


#define COMP_ID ID_SM               // base state component
#define T_ID    _T("ScanAndBag")

IMPLEMENT_DYNCREATE(SMScanAndBag, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

////////////////////////////
SMScanAndBag::SMScanAndBag()
{
  IMPLEMENT_TIMESTAMP
	//+4.2 card dispenser
		#ifdef _TESTSSF
		  m_pAppPSX->SetConfigProperty( _T("SMButton6"), _T("ScanAndBag"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
		#endif
	 //-4.2 card dispenser
}

	//+4.2 card dispenser
	#ifdef _TESTSSF
	#include "Test_CardDispenser\SMScanAndBagTest.cpp"
	#endif
	//-4.2 card dispenser
