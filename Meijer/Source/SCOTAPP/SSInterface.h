// SSInterface.h : main header file for the SSINTERFACE application
//
#if !defined(AFX_SSINTERFACE_H__5419D06F_AB11_11D6_B67F_0090276FBD2B__INCLUDED_)
#define AFX_SSINTERFACE_H__5419D06F_AB11_11D6_B67F_0090276FBD2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include _T('stdafx.h') before including this file for PCH
#endif


/////////////////////////////////////////////////////////////////////////////
// SSInterface.h:
// See SSInterface.cpp for the implementation of this class
//
#define IDC_SMARTSCALECTRL1    2068
#define SS_DATA                    2

#include "DllDefine.h"
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 

/////////////////////////////////////////////////////////////////////////////
// SSInterface window
SSInterface : public CWnd
{
public:
	SSInterface();
	 virtual ~SSInterface();
	bool SSOpened;
	int  SSCreated;
	bool UnInitSS();
    bool InitSS();
   
	friend class CMainFrame;
	_TCHAR cstr[255];

// Attributes
public:
	long GetSlowConclusionTimeout();
	void SetSlowConclusionTimeout(long);
	long GetSCWatchdogTimeout();
	void SetSCWatchdogTimeout(long);
	long GetZeroWeightTimeout();
	void SetZeroWeightTimeout(long);
	long GetLastGoodWeight();
	void SetLastGoodWeight(long);
	BOOL GetHoldWeight();
	void SetHoldWeight(BOOL);
	BOOL GetSkipBagging();
	void SetSkipBagging(BOOL);
	long GetHeavyWeightLimit();
	void SetHeavyWeightLimit(long);
	long GetMeduimWeightLimit();
	void SetMeduimWeightLimit(long);
	long GetHeavyWeightTol();
	void SetHeavyWeightTol(long);
	long GetMediumWeightTol();
	void SetMediumWeightTol(long);
	long GetLightWeightTol();
	void SetLightWeightTol(long);
	long GetCurrentTotalWeight();
	void SetCurrentTotalWeight(long);
	long GetNewItemZeroWeightTimeout();
	void SetNewItemZeroWeightTimeout(long);
	long GetSlowConclusionTimeoutInAttract();
	void SetSlowConclusionTimeoutInAttract(long);
	long GetSlowConclTimeoutInNotExpecting();
	void SetSlowConclTimeoutInNotExpecting(long);
	BOOL GetInAttract();
	void SetInAttract(BOOL);



// Operations
public:
	void GetStatus(BSTR* bstrDeviceName, BSTR* bstrCheckHealthText, long* lStatus);
	void ResetTimers(long nTimer);
	void SetSoldItemInfo(LPDISPATCH lpdispWeightInfo);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SSInterface)
	//}}AFX_VIRTUAL

	
  // Generated message map functions
protected:
	//{{AFX_MSG(SSInterface)
	afx_msg void OnBackToLGW();
	afx_msg void OnDeviceFailure(long nFailType);
	afx_msg void OnLateMatchedWeight(long lLastGoodWeight, long lCurrentTotalWeight, long lIndex, long lItemWt);
	afx_msg void OnMatchedWeight(long lID, long lLastGoodWeight, long lCurrentTotalWeight);
	afx_msg void OnUnmatchedWeight(long lLastGoodWeight, long lCurrentTotalWeight);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
  CSmartScale ss;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SSINTERFACE_H__5419D06F_AB11_11D6_B67F_0090276FBD2B__INCLUDED_)
