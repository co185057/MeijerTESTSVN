// SolutionTBProperties.cpp: implementation of the CSolutionTBProperties class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolutionTBProperties.h"
#include "SolutionTBConnectPOS.h"
#include "CustomerTBConnectPOS.h"
#include "TraceSystem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CSolutionTBProperties::SetHookProperty(LPCTSTR szPropertyName, LPCTSTR szValue)
{
	CSolutionTBConnectPOS & conPos = *CCustomerTBConnectPOS::instance();


	conPos.SetProperty(szPropertyName, szValue);
	
}

CString CSolutionTBProperties::GetHookProperty(LPCTSTR szPropertyName)
{
	CSolutionTBConnectPOS & conPos = *CCustomerTBConnectPOS::instance();

	return conPos.GetProperty(szPropertyName);
}
