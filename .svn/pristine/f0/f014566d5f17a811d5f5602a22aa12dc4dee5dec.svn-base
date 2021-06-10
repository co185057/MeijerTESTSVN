// CustomerCurrentTender.cpp: implementation of the CCustomerCurrentTender class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CustomerCurrentTender.h"
#include "TraceSystem.h"

#define T_ID  _T("CCustomerCurrentTender")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomerCurrentTender::CCustomerCurrentTender()
{
}

CCustomerCurrentTender::~CCustomerCurrentTender()
{
    
}

void CCustomerCurrentTender::Invalidate()
{
   tbtrace(TM_ENTRY, _T("Invalidate"));
   Balance((long)INVALID_BALANCE); // the cast is meant to ensure the right overload is selected by the compiler. Not necessary, but sufficient.
}

bool CCustomerCurrentTender::IsValid()
{
   tbtraceIO(_T("IsValid"));
   long lnBalance= BalanceLong();
   bool bRetVal= (INVALID_BALANCE != lnBalance);
   tbtrace(TM_INFO, _T("Returning %d"), bRetVal);
   return bRetVal;
}
