// SolutionMHTotals.cpp: implementation of the CSolutionMHItem class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "SolutionMHTotals.h"
#include "CustomerMHItem.h"
#include "CustomerCurrentTotals.h"

#define T_ID _T("CSolutionMHTotals")

///////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////
CSolutionMHTotals::CSolutionMHTotals()
{
}

CSolutionMHTotals::~CSolutionMHTotals()
{
}

/*************************************************************************
* ProcessMessage - Process the message posted by TB. TB analyzes the message 
*       and returns the state of the host application, based on the presentation 
*       data in the message.
*
* Parameters: 
*  CTBMsgObj *pMsg - Pointer to the TB message
*
* Returns: TBSTATE - the current state of the host application
*************************************************************************/
TBSTATE CSolutionMHTotals::ProcessMessage(CTBMsgObj *pMsgObj)
{
    TBSTATE rc = CBaseMHTotals::ProcessMessage(pMsgObj);
	int nNumberOfItemsSold = CCustomerMHItem::instance()->GetNumberOfItemsSold();

	if(nNumberOfItemsSold == 0)
	{
		CCustomerCurrentTotals::instance()->ResetTotalDetails();
	}
    
	return rc;
}