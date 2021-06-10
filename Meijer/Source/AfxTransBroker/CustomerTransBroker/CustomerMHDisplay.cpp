//////////////////////////////////////////////////////////////////////
//
// CustomerMHDisplay.cpp: implementation of the CCustomerMHDisplay class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CustomerMHDisplay.h"
#include "TraceSystem.h"
#include "CustomerCurrentDisplay.h"

#define T_ID    _T("CCustomerMHDisplay")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomerMHDisplay::CCustomerMHDisplay()
{
}

CCustomerMHDisplay::~CCustomerMHDisplay()
{
}

// the base processMessage passes the parameter to the 
// msg map handler class CCustomerMsgMap.  Override here to change.
//TBSTATE CBaseMHDisplay::ProcessMessage(PSMessageElement psMessage)
//{
//}
