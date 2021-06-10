//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  FILE:      DBMProcedures.h
//
//  TITLE:     Database Manager (NCR API) Interface
//
//
//  AUTHOR:    
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBMPROCEDURES_H__76363AB0_8EA8_11D1_8B18_86BA39000000__INCLUDED_)
#define AFX_DBMPROCEDURES_H__76363AB0_8EA8_11D1_8B18_86BA39000000__INCLUDED_


////////////////////////////////////////////////////////////////////////////
// DBMProcedures

#include "DllDefine.h"
//#include "ConfigObject.h"    

////////////////////////////////////////////////////////////////////////////
// DBMProcedures window
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
  DBMProcedures : public CWnd
{
public:
  DBMProcedures();
  virtual ~DBMProcedures();

  long Initialize(CWnd* pWnd = NULL);
  long UnInitialize(void);
  int  DBMICreated;
  bool DBMPCreated;
  bool doDBMInit;

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

	
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(DBMProcedures)
  //}}AFX_VIRTUAL
	
protected:
  DBMInterface dbi;
  //{{AFX_MSG(DBMProcedures)
  // NOTE - the ClassWizard will add and remove member functions here.
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBMPROCEDURES_H__76363AB0_8EA8_11D1_8B18_86BA39000000__INCLUDED_)
