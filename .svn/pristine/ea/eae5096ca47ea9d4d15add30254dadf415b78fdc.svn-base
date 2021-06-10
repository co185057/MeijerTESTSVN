// DBMInterface.h : main header file for the DBMINTERFACE application
//

#if !defined(AFX_DBMINTERFACE_H__3458A9C3_E510_11D5_AE31_00E02930222C__INCLUDED_)
#define AFX_DBMINTERFACE_H__3458A9C3_E510_11D5_AE31_00E02930222C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


#include "DllDefine.h"
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 

#define IDC_CLIENTDBMGRCTRL1     3068
#define DB_DATA                    3

DBMInterface : public CWnd
{
public:
    DBMInterface();
    virtual ~DBMInterface();
	bool DBMOpened;
	int  DBMCreated;
	bool UnInitDBM();
    bool InitDBM();
	friend class CMainFrame;

	_TCHAR cstr[255];
	
public:
	long StartItemLookup(LPDISPATCH pWTInfoObj);
	long LastChanceLookup(LPDISPATCH pWTInfoObj);
	long GetDeptException(LPCTSTR DeptID, LPDISPATCH pWTInfoObj);
	long SetUp();
	long GetStatus();
	long UpdateException(LPDISPATCH pWTInfoObj);
	long UpdateWLDB(LPDISPATCH pWTInfoObj);
	long TESTStartItemLookup(LPCTSTR csUPC);
	long TESTLastChanceLookup(LPCTSTR csUPC);
	long QuerySMMTol(LPCTSTR csUPC, long lDept, long lTolType, long lPPWU);
	long QuerySMMWLDBwt(LPCTSTR csUPC, long lDept, LPCTSTR csDesc, long lPrice, long lSpecialItem, long lPPWU);
	long UpdateSMMTol(long lAction, LPCTSTR csUPC, long lDept, long lTolType, long lWtExp, long lWtSubCk, long lWtTol, long lPPWU);
	long UpdateSMMWLDBwt(long lAction, LPCTSTR csUPC, long lDept, long lBSwt, long lAppr, long lTally, long lDate, long lInitWt, long lPPWU);
	void AboutBox();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DBMInterface)
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(DBMInterface)
	afx_msg void OnDBMgrStatus(long lType, long lStatusCode);
	afx_msg void OnDeptExceptions();
	afx_msg void OnSMMTol(long lResult, LPCTSTR csUPC, long lDept, long lTolType, long lWtExp, long lWtSubCk, long lWtTol, long lPPWU, long lMinWtTol, long lMaxWtTol);
	afx_msg void OnSMMWLDBwt(long lResult, LPCTSTR csUPC, long lDept, long lInitWt, long lAppr, long lDate, long lTally, long lAvgWt, long lPPWU, long lLastWtEntry);
	afx_msg void OnSMMWorkingStatus(long lResult);
	afx_msg void OnWLDBandItemExceptionInfo();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
  CClientDBMgr dbMgr;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBMINTERFACE_H__3458A9B4_E510_11D5_AE31_00E02930222C__INCLUDED_)
