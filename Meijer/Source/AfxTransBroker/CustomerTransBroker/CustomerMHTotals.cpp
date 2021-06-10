// CustomerMHTotals.cpp: implementation of the CCustomerMHTotals class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CustomerMHTotals.h"
#include "traceSystem.h"
#include "CustomerTrxBroker.h"
#include "CustomerCurrentTotals.h"
#include "CustomerCurrentTender.h"
#include "CustomerMHOptions.h"



#define T_ID    _T("CCustomerMHTotals")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCustomerMHTotals::CCustomerMHTotals()
{

}

CCustomerMHTotals::~CCustomerMHTotals()
{

}

TBSTATE CCustomerMHTotals::ProcessMessage(CTBMsgObj *pMsgObj)
{
	/*CCustomerMHOptions* pOpts = CCustomerMHOptions::instance();
	CString csFLConfig = pOpts->OptionValue(CString(_T("FastLaneConfiguration")));
	
	if ( csFLConfig != _T("Standard"))
		return CBaseMHTotals::ProcessMessage(pMsgObj);

	else
		return CSolutionMHTotals::ProcessMessage(pMsgObj);*/
	
	//this necessary for buffering the items during resume
	return CBaseMHTotals::ProcessMessage(pMsgObj);

}