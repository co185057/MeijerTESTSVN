//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  FILE:      SAInterface.h
//
//  TITLE:     Device Manager (NCR API) Interface
//
//
//  AUTHOR:    DcL-
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAINTERFACE_H__D5F90611_8CFC_11D1_8B17_644513000000__INCLUDED_)
#define AFX_SAINTERFACE_H__D5F90611_8CFC_11D1_8B17_644513000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// SAInterface window

#define IDC_SECURITYAGENTCTRL1     1068
#define SA_DATA                    2
#define SA_SMMTOLDATA              3
#define SA_SMMSTATUSDATA           4
#define SA_SMMBSDATA               5
#define SA_SMMWLDBDATA             6

#include "DllDefine.h"
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
  SAInterface : public CWnd
{
  // Construction
public:
  SAInterface();
  virtual ~SAInterface();
	
  friend class CmainFrame;

  _TCHAR cstr[255];
	
  bool SAOpened;
  int  SACreated;
	
  bool UnInitSA();
  bool InitSA();	// create instance of Security Agent OCX, open and setup
	
  bool GetOnline();
  void SetOnline(bool);
  long GetStatus();
  void SetStatus(long);
  bool Get_Online();
  void Set_Online(bool);
  long GetSchedule();
  void SetSchedule(long);
  long GetSecurityLevel();
  void SetSecurityLevel(long);
  long GetState();
  void SetState(long);
  short GetMode();
  void SetMode(short);
  long GetTimers();
  void SetTimers(long);
  bool GetTimersEnabled();
  void SetTimersEnabled(bool);
  bool GetSimulated();
  void SetSimulated(bool);
  bool GetServerEnabled();
  void SetServerEnabled(bool);
	
  bool VoidRatioOK(long GrossSales, long VoidSales);
  bool ClearVoidTallies();
  bool ClearOtherTallies();
  bool SendSCOTInput(long ID, long Time, LPCTSTR UPCNumber, long Weight, long Price, long Quantity);
  bool SendScaleInput(long Weight, long ReturnCode, long ReturnCodeExt);
  bool TransactionAbortOk(long GrossSales);
  bool CouponRatioOK(long GrossSales, long CouponTotals, long Type);
  bool ClearCouponTallies(long Amount, long Type);
  bool SendSCOTInputWithOverrides(long ID, long Dept, LPCTSTR UPC, long Weight, long Price, long Quantity, long lRequireSecBagging, long lRequireSecSubstChk);
  void AboutBox(void);

  //Security maintenance mode
  bool QuerySMMTol(LPCTSTR csUPCNumber, long lDepartment, long lTolType, long lPPWU);
  bool UpdateSMMTol(long lAction, LPCTSTR csUPCNumber, long lDepartment,  long lTolType, long lWtExpectation, long lWtSubCk, long lWtTolerance, long lPPWU);
  bool QuerySMMWLDBwt(LPCTSTR csUPCNumber, long lDepartment, LPCTSTR csDescription, long lPrice, long lSpecialItem, long lPPWU);
  bool UpdateSMMWLDBwt(long lAction, LPCTSTR csUPCNumber, long lDepartment, long lBagScaleWt, long lApproval, long lTally, long lDate, long lInitialWt, long lPPWU);
  /////////////////////////////////////////////////////////////////////////////
	
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(SAInterface)
public:
  virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
  //}}AFX_VIRTUAL
	
	
  // Generated message map functions
protected:
    //{{AFX_MSG(SAInterface)
	afx_msg void OnSecurityOutputSecurityagentctrl1(long OutputConclusion, long OutputLevel, long OutputType, long InputID);
	afx_msg void OnSMMWorkingStatusSecurityagentctrl1(long lResult);
	afx_msg void OnSMMBagScaleWtSecurityagentctrl1(long lBagScaleWt);
	afx_msg void OnSMMTolSecurityagentctrl1(long lResult, LPCTSTR csUPC, long lDept, long lTolType, long lWtExp, long lSubCk, long lWtTol, long lPPWU, long lMinWtTol, long lMaxWtTol);
	afx_msg void OnSMMWLDBwtSecurityagentctrl1(long lResult, LPCTSTR csUPC, long lDept, long lInitWt, long lAppr, long lDate, long lTally, long lAvgWt, long lPPWU, long lLastWtEntry);
    afx_msg void OnDestroy();
	DECLARE_EVENTSINK_MAP()
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
  CSecurityAgent sa;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAINTERFACE_H__D5F90611_8CFC_11D1_8B17_644513000000__INCLUDED_)
