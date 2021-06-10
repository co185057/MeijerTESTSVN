//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  FILE:      SAProcedures.h
//
//  TITLE:     Security Agent (NCR API) Interface
//
//
//  AUTHOR:    DcL-
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAPROCEDURES_H__76363AB0_8EA8_11D1_8B18_86BA39000000__INCLUDED_)
#define AFX_SAPROCEDURES_H__76363AB0_8EA8_11D1_8B18_86BA39000000__INCLUDED_

// API for SaySecurity cases
#define SEC_NONE 0								  // case 0:No signal
#define SEC_LOW  1								  // case 1:S_LOW
#define SEC_MED  2								  // case 2:S_MED
#define SEC_HIGH 3								  //case 3:S_HIGH

// API for SaySignal cases
#define SIG_LOW  1 //case 1: S_LOW
#define SIG_MED  3								  //case 2: case 3: S_MED
#define SIG_HIGH 6								  //case 4:case 5:case 6: S_HIGH
#define SIGSEC_LOW  100 //case SECURITY_LOW:S_LOW
#define SIGSEC_MED  101 //case SECURITY_MED:S_MED
#define SIGSEC_HIGH 102 //case SECURITY_HIGH:S_HIGH

#define NO_TONE (0x1000);

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

long GetTimeStamp( void );

////////////////////////////////////////////////////////////////////////////
// SAProcedures action class

#define MAX_SECLEVEL  4
#include "DllDefine.h"
//#include "ConfigObject.h"    


#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
  ActionEntry : public CObject  // data from SASIActions.txt
{                                   // loaded by LoadActions
public:
  int  nConclusion;
  LPTSTR szLogText;
  LPTSTR szBar[MAX_SECLEVEL];
  LPTSTR szAudio[MAX_SECLEVEL];
  LPTSTR szPopup[MAX_SECLEVEL];
  BOOL fTone[MAX_SECLEVEL];
  ActionEntry();                    // constructor
  ~ActionEntry();                   // destructor
};

////////////////////////////////////////////////////////////////////////////
// SAProcedures window

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
  SAProcedures : public CWnd
{
public:
          SAProcedures();
  virtual ~SAProcedures();

  long Initialize(CWnd* pWnd = NULL);
  long UnInitialize(void);

  ActionEntry *GetAction(const int,const int, SCOT_LANGUAGE_TYPE language); // get an action or NULL

  CObList *ActionList[SCOT_LANGUAGE_MAX];
  /*
  CObList *ActionList;         // Primary language list.
  CObList *SecondActionList;   // Secondary language list.
  */
  long LoadActions(LPCTSTR szFileName,CObList *ActionList); // load actions from SASIActions.TXT
	
  long GetOnline(void);
  long SetOnline(BOOL);
  long GetStatus(void);
  long SetStatus(long);
  long Get_Online(void);
  long Set_Online(BOOL);
  long GetSchedule(void);
  long SetSchedule(long);
  long GetSecurityLevel(void);
  long SetSecurityLevel(long);
  long GetState(void);
  long SetState(long);
  long GetMode(void);
  long SetMode(short);
  long GetTimers(void);
  long SetTimers(long);
  long GetTimersEnabled(void);
  long SetTimersEnabled(BOOL);
  long GetSimulated(void);
  long SetSimulated(BOOL);
  long VoidRatioOK(long GrossSales, long VoidSales);
  long ClearVoidTallies();
  long ClearOtherTallies();
  long SendSCOTInput(long ID, long Time, LPCTSTR UPCNumber, long Weight, long Price, long Quantity);
  long SendScaleInput(long Weight, long ReturnCode, long ReturnCodeExt);
  long TransactionAbortOk(long GrossSales);
  long CouponRatioOK(long GrossSales, long CouponTotals, long Type);
  long ClearCouponTallies(long Amount, long Type);
  long SendSCOTInputWithOverrides(long ID, long Dept, LPCTSTR UPC, long Weight, long Price, long Quantity, long lRequireSecBagging, long lRequireSecSubstChk);
  long AboutBox();

  //Security maintenance mode
  bool QuerySMMTol(LPCTSTR UPCNumber, long Department, long TolType, long lPPWU);
  bool UpdateSMMTol(long lAction, LPCTSTR csUPCNumber, long lDepartment,  long lTolType, long lWtExpectation, long lWtSubCk, long lWtTolerance, long lPPWU);
  bool QuerySMMWLDBwt(LPCTSTR csUPCNumber, long lDepartment, LPCTSTR csDescription, long lPrice, long lSpecialItem, long lPPWU);
  bool UpdateSMMWLDBwt(long lAction, LPCTSTR csUPCNumber, long lDepartment, long lBagScaleWt, long lApproval, long lTally, long lDate, long lInitialWt, long lPPWU);

  // BeginCodeWLDBSA11
  bool SAProcedures::TraceWLDB(int nConclusion, int nLevel, int nType, int InProcess, int InputType);
  // EndCodeWLDBSA11

  int  LastConclusion;					 // 0 = none
  int  CurrentActiveConclusion;	 // 0 = none
  bool doSAInit;
  int  SAICreated;
  bool SAPCreated;
	
	
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(SAProcedures)
public:
  virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
  //}}AFX_VIRTUAL
	
protected:
  SAInterface sai;
  //{{AFX_MSG(SAProcedures)
  // NOTE - the ClassWizard will add and remove member functions here.
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

private:
  long UnloadActions(void);         // free actions

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAPROCEDURES_H__76363AB0_8EA8_11D1_8B18_86BA39000000__INCLUDED_)
