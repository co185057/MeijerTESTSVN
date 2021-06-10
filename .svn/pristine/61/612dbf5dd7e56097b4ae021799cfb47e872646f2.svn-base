// SecMgrInterface.h : main header file for the SecMgrINTERFACE application
//
#if !defined(AFX_SECMGRINTERFACE_H__5419D06F_AB11_11D6_B67F_0090276FBD2B__INCLUDED_)
#define AFX_SECMGRINTERFACE_H__5419D06F_AB11_11D6_B67F_0090276FBD2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include _T('stdafx.h') before including this file for PCH
#endif


/////////////////////////////////////////////////////////////////////////////
// SecMgrInterface.h:
// See SecMgrInterface.cpp for the implementation of this class
//
#define IDC_SECURITYMANAGERCTRL1    2068
#define SM_DATA                    2

#include "DllDefine.h"
#include "SecurityManager.h"
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 

/////////////////////////////////////////////////////////////////////////////
// SSInterface window
SecMgrInterface : public CWnd
{
public:
	SecMgrInterface();
	 virtual ~SecMgrInterface();
	int  SecMgrCreated;
	bool UnInitSecMgr();
    bool InitSecMgr();
   
	friend class CMainFrame;
	_TCHAR cstr[255];

// Attributes
public:
	CString GetOutstandingInterventionList();
	CString GetDevices();
    long GetOutstandingInterventionCount();

// Operations
public:
	void SecurityUnadvise(long lCookie);
	long SecurityAdvise(long lFlags);
	void ItemSold(long lCookie, LPCTSTR bstrParms);
	void ItemDetails(long lCookie, LPCTSTR bstrParms);
	void VoidItem(long lCookie, LPCTSTR bstrParms);
	void TransactionControl(long lCookie, LPCTSTR bstrParms);
	long AppControl(long lCookie, LPCTSTR bstrParms, BSTR* bstrOutParms);
	void OverrideException(long lCookie, LPCTSTR bstrParms);
	long SecurityMaintenance(long lCookie, LPCTSTR bstrParms);
	void SecurityControl(long lCookie, LPCTSTR bstrInParms, BSTR* bstrOutParms);
	long SecMgrInterface::Initialize(long lCookie, LPCTSTR bstrInParms, BSTR* pbstrValues);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SecMgrInterface)
	//}}AFX_VIRTUAL

	
  // Generated message map functions
protected:
	//{{AFX_MSG(SecMgrInterface)
	afx_msg void OnItemOK(LPCTSTR sParms);
	afx_msg void OnTransactionComplete();
	afx_msg void OnSecurityException(LPCTSTR sParms);
	afx_msg void OnSecurityExceptionCleared(LPCTSTR sParms);
	afx_msg void OnSecurityMaintenance(LPCTSTR sParms);
	afx_msg void OnDeviceFailure(LPCTSTR sParms);
	afx_msg void OnInitializationComplete(LPCTSTR sParms);
	afx_msg void OnAppControl(LPCTSTR sParms);
	afx_msg void OnSecurityControl(LPCTSTR sParms);
	afx_msg void OnItemPickFinish();//SR679

	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
  CSecurityManager SecMgr;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SECMGRINTERFACE_H__5419D06F_AB11_11D6_B67F_0090276FBD2B__INCLUDED_)
