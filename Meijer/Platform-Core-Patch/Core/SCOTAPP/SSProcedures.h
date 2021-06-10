//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  FILE:      SSProcedures.h
//
//  TITLE:     Smart Scale (NCR API) Interface
//
//
//  AUTHOR:    
//
//////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_SSPROCEDURES_H__5419D067_AB11_11D6_B67F_0090276FBD2B__INCLUDED_)
#define AFX_SSPROCEDURES_H__5419D067_AB11_11D6_B67F_0090276FBD2B__INCLUDED_



// API for SaySecurity cases
#define SEC_NONE 0								  // case 0:No signal
#define SEC_LOW  1								  // case 1:S_LOW
#define SEC_MED  2								  // case 2:S_MED
#define SEC_HIGH 3								  // case 3:S_HIGH

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
// SSProcedures action class

#define MAX_SECLEVEL  4
#include "DllDefine.h"
//#include "ConfigObject.h"    

////////////////////////////////////////////////////////////////////////////
// SSProcedures window
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
  SSProcedures : public CWnd
{
public:
          SSProcedures();
  virtual ~SSProcedures();
    long Initialize(CWnd* pWnd = NULL);
    long UnInitialize(void);
	int  SSICreated;
    bool SSPCreated;
	bool doSSInit;
// Attributes
public:
	long GetSlowConclusionTimeout();
	void SetSlowConclusionTimeout(long);
	long GetSlowConclusionTimeoutInAttract();
	void SetSlowConclusionTimeoutInAttract(long);
	long GetSlowConclTimeoutInNotExpecting();
	void SetSlowConclTimeoutInNotExpecting(long);
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
	BOOL GetInAttract();
	void SetInAttract(BOOL);



// Operations
public:
	void GetStatus(BSTR* bstrDeviceName, BSTR* bstrCheckHealthText, long* lStatus);
	void ResetTimers(long nTimer);
	void SetSoldItemInfo(LPDISPATCH lpdispWeightInfo);
	
	
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(SAProcedures)
  //}}AFX_VIRTUAL
	
protected:
  SSInterface ssi;
  //{{AFX_MSG(SSProcedures)
  // NOTE - the ClassWizard will add and remove member functions here.
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()


};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SSPROCEDURES_H__5419D067_AB11_11D6_B67F_0090276FBD2B__INCLUDED_)