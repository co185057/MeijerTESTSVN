//
// FILE:  SMSelectFromList.cpp
//
// TITLE: Class implementation for Select From List state
//
// TIME:  07/12/98
//
// AUTHOR:   Feng Zhang
//
//////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMSelectFromList.h"       // MGV this state

// +SSCOI-44479
#include "SMEnterQuantity.h"		
#include "SMPLAEnterWeight.h"
// -SSCOI-44479

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SelectFromList")

IMPLEMENT_DYNCREATE(SMSelectFromList, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSelectFromList::SMSelectFromList()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSelectFromList::Initialize(void)
{
    return SMSelectFromListBase::Initialize();
}

void SMSelectFromList::UnInitialize(void)
{
    SMSelectFromListBase::UnInitialize();
}

SMStateBase * SMSelectFromList::PSButton3(void)
{
    return SMSelectFromListBase::PSButton3();
}

SMStateBase * SMSelectFromList::PSButton4(void)
{
    return SMSelectFromListBase::PSButton4();
}

SMStateBase * SMSelectFromList::PSListLookup(CString csItemInfo)
{
// +SSCOI-44479
#ifndef _CPPUNIT
	// GetListWide returns an int, not a UINT and -1 means failure which is 
	// completely lost if we cast the return to UINT  ncr_barn
	int nSelect = ps.GetWideList(csItemSent); //Selected item //Feng
	
	csItemSent.TrimRight();
	
	// Check for failure of nSelect also (<=)  ncr_barn
	if (nSelect<=0 || csItemSent==_T("")) //fail in picking list
	{
		DMSayPhrase(PICKLISTINVALID);
		return STATE_NULL;
	}
#endif //_CPPUNIT
	//double boink pd DMSayPhrase("PickListValid");   //Play a "PickListValid" wav file. --Feng
	if (VerifyQtyItem(csItemSent))     //Tar 143309 
	{
		io.Reset();
		io.csOrigCode = csItemSent;	   // item code keyed
		io.csItemCode = csItemSent;	   // assumed to be the same
		if (!co.IsPicklistAssistEnable()) // dont Send SASendScot when PLA is enable, Let TBItemSale called by PLAEnterWeight do
		{
			SASendSCOTInput( UPC_MANUAL, csItemSent);
		}
		SetNextGenUICurrentItem(io.csOrigCode);
		// +SSCOI-44479
		return TBItemSale(FALSE,0,false); 	
		//RETURNSTATE(EnterQuantity)
		// -SSCOI-44479
	}
	if (co.IsPicklistAssistEnable() && ps.IsPickListItem(csItemSent))
	{
		io.Reset();
		io.csOrigCode = csItemSent;
		io.csItemCode = csItemSent;
		RETURNSTATE(PLAEnterWeight)
	}
	else
	{
		return TBItemSale(FALSE,0,false);         //Sell item
  }
    //return SMSelectFromListBase::PSListLookup(csItemInfo);
	// 	//SSCOI-44479 -
}

SMStateBase * SMSelectFromList::PSNumericKey(const long lKey)
{
    return SMSelectFromListBase::PSNumericKey(lKey);
}

SMStateBase * SMSelectFromList::PSEnterKey(void)
{
    return SMSelectFromListBase::PSEnterKey();
}

SMStateBase * SMSelectFromList::PSClearKey(void)
{
    return SMSelectFromListBase::PSClearKey();
}

SMStateBase * SMSelectFromList::PSCancelKey(void)
{
    return SMSelectFromListBase::PSCancelKey();
}

SMStateBase * SMSelectFromList::DMScanner(void)
{
    return SMSelectFromListBase::DMScanner();
}

SMStateBase * SMSelectFromList::PSButtonGoBack(void)
{
    return SMSelectFromListBase::PSButtonGoBack();
}
