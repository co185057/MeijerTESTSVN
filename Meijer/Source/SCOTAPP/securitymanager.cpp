// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "securitymanager.h"

/////////////////////////////////////////////////////////////////////////////
// CSecurityManager

IMPLEMENT_DYNCREATE(CSecurityManager, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CSecurityManager properties

CString CSecurityManager::GetOutstandingInterventionList()
{
	CString result;
	GetProperty(0xa, VT_BSTR, (void*)&result);
	return result;
}

void CSecurityManager::SetOutstandingInterventionList(LPCTSTR propVal)
{
	SetProperty(0xa, VT_BSTR, propVal);
}

CString CSecurityManager::GetDevices()
{
	CString result;
	GetProperty(0xb, VT_BSTR, (void*)&result);
	return result;
}

void CSecurityManager::SetDevices(LPCTSTR propVal)
{
	SetProperty(0xb, VT_BSTR, propVal);
}

long CSecurityManager::GetOutstandingInterventionCount()
{
	long result;
	GetProperty(0xe, VT_I4, (void*)&result);
	return result;
}

void CSecurityManager::SetOutstandingInterventionCount(long propVal)
{
	SetProperty(0xe, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CSecurityManager operations

void CSecurityManager::SecurityUnadvise(long dwCookie)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCookie);
}

long CSecurityManager::SecurityAdvise(long lFlags)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lFlags);
	return result;
}

void CSecurityManager::ItemSold(long lCookie, LPCTSTR bstrParms)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lCookie, bstrParms);
}

void CSecurityManager::ItemDetails(long lCookie, LPCTSTR bstrParms)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lCookie, bstrParms);
}

void CSecurityManager::VoidItem(long lCookie, LPCTSTR bstrParms)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lCookie, bstrParms);
}

void CSecurityManager::TransactionControl(long lCookie, LPCTSTR bstrParms)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lCookie, bstrParms);
}

long CSecurityManager::AppControl(long lCookie, LPCTSTR bstrParms, BSTR* bstrOutParms)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_BSTR VTS_PBSTR;
	InvokeHelper(0x7, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lCookie, bstrParms, bstrOutParms);
	return result;
}

void CSecurityManager::OverrideException(long lCookie, LPCTSTR bstrParms)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR;
	InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lCookie, bstrParms);
}

long CSecurityManager::SecurityMaintenance(long lCookie, LPCTSTR bstrParms)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_BSTR;
	InvokeHelper(0x9, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lCookie, bstrParms);
	return result;
}

void CSecurityManager::SecurityControl(long lCookie, LPCTSTR bstrInParms, BSTR* bstrOutParms)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR VTS_PBSTR;
	InvokeHelper(0xc, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lCookie, bstrInParms, bstrOutParms);
}

long CSecurityManager::Initialize(long lCookie, LPCTSTR bstrInParms, BSTR* pbstrValues)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_BSTR VTS_PBSTR;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lCookie, bstrInParms, pbstrValues);
	return result;
}
