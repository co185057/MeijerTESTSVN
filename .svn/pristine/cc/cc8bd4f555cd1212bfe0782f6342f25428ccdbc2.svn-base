// CustomerCurrentSigCap.cpp: implementation of the CCustomerCurrentSigCap class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CustomerCurrentSigCap.h"
#include "TraceSystem.h"

#define T_ID  _T("CCustomerCurrentSigCap")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomerCurrentSigCap::CCustomerCurrentSigCap()
{
}

CCustomerCurrentSigCap::~CCustomerCurrentSigCap()
{
    
}

TBRC CCustomerCurrentSigCap::PreprocessSignatureData(LPCTSTR lpFileName,BSTR bstrSignatureData, LPCTSTR lpReceiptType, BOOL* bProcessData)
{
	tbtraceIO( _T("PreprocessSignatureData") );
    //this solve the problem about fastlane shutdown during print signature by setting the bProcessData to false.
	DWORD dwLen = SysStringByteLen( bstrSignatureData );
	if ( dwLen <= 0 )
	{
		*bProcessData = false;
		tbtrace( TM_INFO, _T("Signature Data length is zero."));
	}

	return CSolutionCurrentSigCap::PreprocessSignatureData(lpFileName, bstrSignatureData, lpReceiptType,bProcessData);
}