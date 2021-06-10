//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  FILE:      SecMgrProcedures.h
//
//  TITLE:     Security Manager(NCR API) Interface
//
//
//  AUTHOR:    
//
//////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_SECMGRPROCEDURES_H__5419D067_AB11_11D6_B67F_0090276FBD2B__INCLUDED_)
#define AFX_SECMGRPROCEDURES_H__5419D067_AB11_11D6_B67F_0090276FBD2B__INCLUDED_

#include "SecMgrProceduresConstants.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

long GetTimeStamp( void );

////////////////////////////////////////////////////////////////////////////
// SSProcedures action class

#include "DllDefine.h"
#include "SecMgrInterface.h"
#include "ISecMgrProcedures.h"

////////////////////////////////////////////////////////////////////////////
// SSProcedures window
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
  SecMgrProcedures : public CWnd, public ISecMgrProcedures
{
public:
          SecMgrProcedures();
  virtual ~SecMgrProcedures();
    long Initialize(CWnd* pWnd = NULL);
    long UnInitialize(void);
	bool SecMgrICreated;
    bool SecMgrPCreated;
	bool doSecMgrInit;
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
	long Initialize(long lCookie, LPCTSTR bstrInParms, BSTR* pbstrValues);

	
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(SecMgrProcedures)
  //}}AFX_VIRTUAL
	
protected:
  SecMgrInterface SecMgri;
  //{{AFX_MSG(SecMgrProcedures)
  // NOTE - the ClassWizard will add and remove member functions here.
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()


};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SECMGRPROCEDURES_H__5419D067_AB11_11D6_B67F_0090276FBD2B__INCLUDED_)