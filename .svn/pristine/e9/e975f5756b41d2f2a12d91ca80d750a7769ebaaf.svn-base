//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMState.CPP
//
// TITLE: Class implementation file for base class SMState
//
// All states ultimately inherit from SMState.
// This class is an abstract class (it has at least one
// pure virtual function and can't be instantiated.)
//
// This class provides default processing for
// all possible messages to be parsed. Message
// handlers are usually implemented in deriving state
// but if not they end up here and are usually
// ignored.
//
// The functions in SMState are implemented here and in
// SMStatePS.cpp, SMStateDM.cpp, SMStateTB.cpp and SMStateSA.cpp
//
//
// AUTHOR:  Peter Denhoed, Updated DcL-
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"

#include "SMAttract.h"
#include "SMScanAndBag.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SMState")

IMPLEMENT_DYNCREATE(SMState, CObject)// MFC Runtime class/object information

DLLEXPORT SMState *GetStartState(void)  //Get the start screen. The default screen is SMAttract
{
  CRuntimeClass* pStateAttract = RUNTIME_CLASS(SMAttract);
  CObject* pSMState = pStateAttract->CreateObject();
  ASSERT( pSMState->IsKindOf( RUNTIME_CLASS(SMAttract) ) );
  return (SMState*) pSMState;
}

//
//
// IMPORTANT CONSIDERATIONS FOR INTERMEDIATE AND BASE SSF STATES!
//
// 
//  You cannot add new VIRTUAL methods/functions to any
//    intermediate or base state
//
// ·  You cannot add new non-static properties/data members
//    to any intermediate or base state
//
// ·  You can override existing virtual methods using an
//    identical prototype
//
// ·  You can add new non-virtual methods
//
// ·  You can add new static properties/data members.
//
// The reason for these restrictions is that the core is pre-built
// using a set of default intermediate and base states.  At the
// time the code is built, the virtual method/function entry point
// table becomes fixed and the space for non-static properties/data
// members becomes fixed.  Any attempt to extend the virtual table
// or properties in SSF DLL will compile and will link but the
// resulting tables will not mesh with the pre-built tables in
// the core EXE.  This causes unpredictable serious errors at run
// time, usually access violations or application hangs. 


/////////////////////////////////////////
// Constructor
/////////////////////////////////////////
SMState::SMState()
: SMStateBase()
{
			 

}


SMStateBase *SMState::TBParse(MessageElement* mg)// same parse for all
{
  return SMStateBase::TBParse(mg);
}

TBSTATE SMState::TBProcessMessage(MessageElement* me)
{
  return SMStateBase::TBProcessMessage(me);
}
#ifdef _TESTSSF
const TCHAR SCOTAPPSHOPPINGCARD[] = _T("^SCOTAPP SHOPPING CARD^"); //gift card key
///////////////////////////////////////////////////////////////////////////////
// Function:    IsCardDispenserOK
// Description: Check the health of card dispenser. 
// Return Value: bool - true  - if card dispense is ok
//						false - if not ok
///////////////////////////////////////////////////////////////////////////////////
bool SMState::IsCardDispenserOK() 
{
	long lCountBin,lCountReject;
	bool bReturn = true;
	dm.GetAdjustCardDispenserBinCounts(lCountBin, lCountReject, 0);
	if (dm.GetCardDispenserEmpty() || dm.GetCardDispenserGateOpen () \
		|| lCountReject == co.getDispenserRejectBinFull() )
	{
		bReturn = false;
	}
	return bReturn;
}
///////////////////////////////////////////////////////////////////////////////
// Function:    GetAccountNumber
// Description: Extract the account number from m_csGiftCardTrackData
// Return Value: CString - accout number
///////////////////////////////////////////////////////////////////////////////////
CString SMState::GetAccountNumber() 
{
	CString csRet;
	int nLastCaret = m_csGiftCardTrackData.Find(_T("^"));
	csRet = m_csGiftCardTrackData.Mid(1, (nLastCaret - 1) );	
	return csRet;
}

///////////////////////////////////////////////////////////////////////////////
// Function:    IsValidShoppingCard
// Description: Validate if card inserted is a valid gift card
// Paremeter:   [out] CString& csCardData - Card Data
// Return Value: bool - true if valid gift card
///////////////////////////////////////////////////////////////////////////////////
bool SMState::IsValidShoppingCard(CString& cspCardData) 
{
	trace(L6, _T("+SMState::IsValidShoppingCard"));
	cspCardData = _T("");
	//+codes here are copied from SMStateBase::GetAccountNum
	//use LPTSTR SMStateBase::cDMLastTrack to get the track data
	//get track and validate
	short int nT1, nT2, nT3, nHeaderLen;
	short int nShortLen = sizeof(short);

    nT1 = nT2 = nT3 = 0;
	nT1 = (short ) *(  (const char *)cDMLastTrack);						//length of Track 1
	nT2 = (short ) *( ((const char *)cDMLastTrack) + nShortLen);		//length of Track 2
	nT3 = (short ) *( ((const char *)cDMLastTrack) + (nShortLen * 2));	//length of Track 3
  	nHeaderLen = nT1 + nT2 + nT3;

	LPTSTR szTrackData = new _TCHAR[nHeaderLen+1];
	memcpy(szTrackData, ((const char *)cDMLastTrack + (3*nShortLen)), nHeaderLen*sizeof(_TCHAR));
	szTrackData[nHeaderLen] = '\0';
	cspCardData = szTrackData;
	//-codes here are copied from SMStateBase::GetAccountNum
	trace(L6, _T("-SMState::IsValidShoppingCard = %s"),cspCardData );
	return (cspCardData.Find(SCOTAPPSHOPPINGCARD) > 0 ? true:false);
}
///////////////////////////////////////////////////////////////////////////////
// Function:    WriteGiftCard
// Description: Create Gift Card
// Paremeter:   [in] bNewCard - true if create new card
//							  - false if update existing card	
// Return Value: bool - true if write to card dispenser is successfull
///////////////////////////////////////////////////////////////////////////////////
bool SMState::WriteGiftCard( bool bNewCard ) 
{
	bool bReturn = false;
	CString csAccountNo, csAmount;
	//create new account number if true
	if (  bNewCard )
	{
		COleDateTime dt = COleDateTime::GetCurrentTime();
		csAccountNo.Format( _T("%s%s"), dt.Format(_T("%m%d%y%H%M%S")), _T("0000") );
	}else
	{
		csAccountNo = GetAccountNumber();
		//+extract current card amount and add the amount selected from the shopping card
		int nLastCaretLocation = m_csGiftCardTrackData.Find(_T("^"));
		nLastCaretLocation = m_csGiftCardTrackData.Find(_T("^"), (nLastCaretLocation + 1));
		m_SSFMoneyCenterGiftCardAmount += (_ttoi(m_csGiftCardTrackData.Mid((nLastCaretLocation + 1), 7))/100);
		//-extract current card amount and add the amount selected from the shopping card
	}
	//format amount;
	csAmount.Format(_T("%07d"), (m_SSFMoneyCenterGiftCardAmount * 100));
	//Track 1: Bmmddyyhhmmss0000^SCOTAPP SHOPPING CARD^aaaaaaa
	//Track 2:mmddyyhhmmss0000=aaaaaaa
	//Where mmddyy is the month day year, hhmmss is the hour minute and second 
	//and aaaaaaa is the current value of the card in dollars and cents 
	//(so $25 would have a value of 0002500 for this field).  

	CString csTrack1, csTrack2;
	csTrack1.Format(_T("B%s%s%s"), csAccountNo,  SCOTAPPSHOPPINGCARD,  csAmount); 
	csTrack2.Format(_T("%s=%s"),csAccountNo, csAmount);
	m_csGiftCardTrackData.Format(_T("%s%s"), csTrack1, csTrack2); 
	if (dm.CardDispenserWrite( DM_MSR_TRACK_1 | DM_MSR_TRACK_2, csTrack1, csTrack2, _T("")) )
	{
		bReturn = true;
	}
	return bReturn;
}
#endif _TESTSSF
