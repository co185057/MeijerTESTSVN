// DBMInterface.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Common.h"
#include "SMState.h" 
#include "DBMInterface.h"

#define COMP_ID ID_SADBM

#define T_ID _T("dbi")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define PROCESS_EXCEPTIONS(a)                                 \
	catch(COleDispatchException * e)                              \
{                                                             \
  ScotError(RETURN,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_DBMINTERFACE_OLEDISPATCH_EXCEPTION, \
  _T("%s|%s|%x"),   \
	msg,                                                      \
	e->m_strDescription,                                      \
	e->m_scError);                                            \
  e->Delete();                                               \
  return a;                                                   \
}                                                             \
	catch(COleException* e)                                       \
{                                                             \
  e->GetErrorMessage( cstr, 255 );                           \
  ScotError(RETURN,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_DBMINTERFACE_OLE_EXCEPTION,_T("%s|%s|%x"),        \
	msg,                                                      \
	cstr,                                                     \
	e->m_sc);                                                 \
  e->Delete();                                               \
  return a;                                                   \
}                                                             \
	catch(CException* e)                                          \
{                                                             \
  e->GetErrorMessage( cstr, 255 );                           \
  ScotError(RETURN,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_DBMINTERFACE_C_EXCEPTION,_T("%s|%s"),                   \
	msg,                                                      \
	cstr);                                                    \
  e->Delete();                                               \
  return a;                                                   \
}                                                             \
	catch(...)                                                    \
{                                                             \
  long le = GetLastError();                                   \
  ScotError(RETURN,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_DBMINTERFACE_EXCEPTION,_T("%s|%d"),msg,le);            \
  return a;                                                   \
}

//////////////////////////////////////////////////////////////////////////////
//
// Database Manager Functions 
//
DBMInterface::DBMInterface()
{
  DBMOpened =  false;
  DBMCreated = false;
}

DBMInterface::~DBMInterface()
{
  DBMOpened =  false;
  DBMCreated = false;
}
 /////////////////////////////////////////////////////////////////////////////
//
// Open the Database Management
//
bool DBMInterface::InitDBM()
{
  _TCHAR msg[] = T_ID _T("::InitDBM");
	
  if ( DBMCreated == false )
  {
    if( !dbMgr.Create(NULL, NULL, CRect(0,0,0,0), this, IDC_CLIENTDBMGRCTRL1 ))
    {
	  ScotError(INFO,SM_CAT_STOP_ABORT, SCOTAPP_DBMINTERFACE_STOP);//TD 05/17/02 TAR 204873
	  return false;
    }
	else
	{
      DBMCreated = true;
	}
	try
	{
      if (DBMOpened == false )
      {
        DBMOpened = true;
      }
    }	
    PROCESS_EXCEPTIONS(false);
    }// end - if ( SACreated == false )
  return true;
}

////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// Close Database Management
//
bool DBMInterface::UnInitDBM()
{
  trace(L2, _T("+UnInitDBM"));
  _TCHAR msg[] = T_ID _T("::UnInitDBM");
  bool rc;
  try
  {
 
    if (DBMOpened == true )
    {
      trace(L2, _T("UnInitDBM -- before Closing Window"));
	  //Tar 214239,  Dr. Watson shutting down the app using UpCon
      //dbMgr.CloseWindow();
	  //End Tar 214239  
      trace(L2, _T("Close Window"));
	  try
	  {
      rc = !dbMgr.DestroyWindow();	// true - if succesful
	  }
	  catch(CException* e)
	  {
        _TCHAR szBuff[300];
		e->GetErrorMessage(szBuff,300);
		trace(L2, _T("Got an exception in dbMgr.DestroyWindow() :Error text:%s"), szBuff);
		e->Delete(); //TAR247967

	  }
       trace(L2, _T("after DestroyWindow"));
	   DBMOpened = false;
       DBMCreated = false;
    }
	trace(L2, _T("UnInitDBM -- before PROCESS_EXCEPTIONS"));
  }
  PROCESS_EXCEPTIONS(false);
  trace(L2, _T("-UnInitDBM"));
  return rc;
}
////////////////////////////////////////////////////////////////////////////
long DBMInterface::SetUp()
{
  _TCHAR msg[] = T_ID _T("::SetUp");
  long r = 0;	
  try
  {
    r = dbMgr.SetUp();
  }
  PROCESS_EXCEPTIONS(r);
  return r;
}
////////////////////////////////////////////////////////////////////////////
long DBMInterface::StartItemLookup(LPDISPATCH pWTInfoObj)
{
  _TCHAR msg[] = T_ID _T("::StartItemLookup");
  long r = 0;	
  try
  {
    r = dbMgr.StartItemLookup(pWTInfoObj);
  }
  PROCESS_EXCEPTIONS(r);
  return r;
}
////////////////////////////////////////////////////////////////////////////
long DBMInterface::GetDeptException(LPCTSTR DeptID, LPDISPATCH pWTInfoObj)
{
	//tar 193806
	IWTInfoPtr pTempWTInfoObj = pWTInfoObj;

	//get dept exception info
	long lwtTol, lbDefFlag;
	int iwtReq, iSubstCk, iTolUsedFlag;
	 pTempWTInfoObj->GetExceptionInfo(&lwtTol, &iwtReq, &iSubstCk, &lbDefFlag, &iTolUsedFlag);
	trace(L2,_T("Dept Excep: lwtTol: %d, iwtReq: %d, iSubstCk: %d, lbDefFlag: %d, DeptID: %s"), lwtTol, iwtReq, iSubstCk, lbDefFlag, DeptID);
    //end of tar 193806

  _TCHAR msg[] = T_ID _T("::GetDeptException");
  long r = 0;	
  try
  {
    r = dbMgr.GetDeptException(DeptID,pWTInfoObj);
  }
  PROCESS_EXCEPTIONS(r);
  return r;
}
////////////////////////////////////////////////////////////////////////////
long DBMInterface::LastChanceLookup(LPDISPATCH pWTInfoObj)
{
  _TCHAR msg[] = T_ID _T("::LastChanceLookup");
  long r = 0;	
  try
  {
    r = dbMgr.LastChanceLookup(pWTInfoObj);
  }
  PROCESS_EXCEPTIONS(r);
  return r;
}
////////////////////////////////////////////////////////////////////////////
long DBMInterface::GetStatus()
{
  _TCHAR msg[] = T_ID _T("::GetStatus");
  long r = 0;	
  try
  {
    r = dbMgr.GetStatus();
  }
  PROCESS_EXCEPTIONS(r);
  return r;
}
////////////////////////////////////////////////////////////////////////////
long DBMInterface::UpdateException(LPDISPATCH pWTInfoObj)
{
  _TCHAR msg[] = T_ID _T("::UpdateException");
  long r = 0;	
  try
  {
    r = dbMgr.UpdateException(pWTInfoObj);
  }
  PROCESS_EXCEPTIONS(r);
  return r;
}
////////////////////////////////////////////////////////////////////////////
long DBMInterface::UpdateWLDB(LPDISPATCH pWTInfoObj)
{
  _TCHAR msg[] = T_ID _T("::UpdateWLDB");
  long r = 0;	
  try
  {
    r = dbMgr.UpdateWLDB(pWTInfoObj);
  }
  PROCESS_EXCEPTIONS(r);
  return r;
}
////////////////////////////////////////////////////////////////////////////
long DBMInterface::TESTStartItemLookup(LPCTSTR csUPC)
{
  _TCHAR msg[] = T_ID _T("::TESTStartItemLookup");
  long r = 0;	
  try
  {
    r = dbMgr.TESTStartItemLookup(csUPC);
  }
  PROCESS_EXCEPTIONS(r);
  return r;
}
////////////////////////////////////////////////////////////////////////////
long DBMInterface::TESTLastChanceLookup(LPCTSTR csUPC)
{
  _TCHAR msg[] = T_ID _T("::TESTLastChanceLookup");
  long r = 0;	
  try
  {
    r = dbMgr.TESTLastChanceLookup(csUPC);
  }
  PROCESS_EXCEPTIONS(r);
  return r;
}
////////////////////////////////////////////////////////////////////////////
long DBMInterface::QuerySMMTol(LPCTSTR csUPC, long lDept, long lTolType, long lPPWU)
{
	_TCHAR msg[] = T_ID _T("::QuerySMMTol");
	long r = 0;	
	try
	{
    r = dbMgr.QuerySMMTol(csUPC,lDept,lTolType,lPPWU);
	}
	PROCESS_EXCEPTIONS(r);
	return r;
}
////////////////////////////////////////////////////////////////////////////
long DBMInterface::QuerySMMWLDBwt(LPCTSTR csUPC, long lDept, LPCTSTR csDesc, long lPrice, long lSpecialItem, long lPPWU)
{
	_TCHAR msg[] = T_ID _T("::QuerySMMWLDBwt");
	long r = 0;	
	try
	{
    r = dbMgr.QuerySMMWLDBwt(csUPC,lDept,csDesc,lPrice,lSpecialItem,lPPWU);
	}
	PROCESS_EXCEPTIONS(r);
	return r;

}
////////////////////////////////////////////////////////////////////////////

long DBMInterface::UpdateSMMTol(long lAction, LPCTSTR csUPC, long lDept, long lTolType, long lWtExp, long lWtSubCk, long lWtTol, long lPPWU)
{
	_TCHAR msg[] = T_ID _T("::UpdateSMMTol");
	long r = 0;	
	try
	{
    r = dbMgr.UpdateSMMTol(lAction,csUPC,lDept,lTolType,lWtExp,lWtSubCk,lWtTol,lPPWU);
	}
	PROCESS_EXCEPTIONS(r);
	return r;
 
}
////////////////////////////////////////////////////////////////////////////
long DBMInterface::UpdateSMMWLDBwt(long lAction, LPCTSTR csUPC, long lDept, long lBSwt, long lAppr, long lTally, long lDate, long lInitWt, long lPPWU)
{
	_TCHAR msg[] = T_ID _T("::UpdateSMMWLDBwt");
	long r = 0;	
	try
	{
    r = dbMgr.UpdateSMMWLDBwt(lAction,csUPC,lDept,lBSwt,lAppr,10,lDate,lInitWt,lPPWU);//using approval field for tolerance info, will set approval to true in ClientDBMgr LPM090302
	}
	PROCESS_EXCEPTIONS(r);
	return r;
}
////////////////////////////////////////////////////////////////////////////
void DBMInterface::AboutBox()
{
	dbMgr.AboutBox();
}
////////////////////////////////////////////////////////////////////////////
BEGIN_EVENTSINK_MAP(DBMInterface, CWnd)
    //{{AFX_EVENTSINK_MAP(DBMInterface)
	ON_EVENT(DBMInterface, IDC_CLIENTDBMGRCTRL1, 1 /* DBMgrStatus */, OnDBMgrStatus, VTS_I4 VTS_I4)
	ON_EVENT(DBMInterface, IDC_CLIENTDBMGRCTRL1, 2 /* WLDBandItemExceptionInfo */, OnWLDBandItemExceptionInfo, VTS_NONE)
	ON_EVENT(DBMInterface, IDC_CLIENTDBMGRCTRL1, 3 /* DeptExceptions */, OnDeptExceptions, VTS_NONE)
	ON_EVENT(DBMInterface, IDC_CLIENTDBMGRCTRL1, 4 /* SMMWorkingStatus */, OnSMMWorkingStatus, VTS_I4)
	ON_EVENT(DBMInterface, IDC_CLIENTDBMGRCTRL1, 5 /* SMMTol */, OnSMMTol, VTS_I4 VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(DBMInterface, IDC_CLIENTDBMGRCTRL1, 6 /* SMMWLDBwt */, OnSMMWLDBwt, VTS_I4 VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()
/////////////////////////////////////////////////////////////////////////////
// DBMInterface message handlers

void DBMInterface::OnDBMgrStatus(long lType, long lStatusCode)
{
	trace(L6,_T("PostDB DBM_STATUS lType: %d, lStatusCode: %d"),lType,lStatusCode);
	mo.PostDB(DB_DATA, lType, lStatusCode, DBM_STATUS);
}

////////////////////////////////////////////////////////////////////////////
void DBMInterface::OnDeptExceptions() 
{
	trace(L6,_T("PostDB DBM_DEPTINFOSUCCESS"));
	mo.PostDB(DB_DATA, -1, -1, DBM_DEPTINFOSUCCESS);
}
////////////////////////////////////////////////////////////////////////////
void DBMInterface::OnSMMTol(long lResult, LPCTSTR csUPC, long lDept, long lTolType, long lWtExp, long lWtSubCk, long lWtTol, long lPPWU, long lMinWtTol, long lMaxWtTol) 
{
	trace(L6,_T("PostSASMMWtTol Res=%d,upc=%s,Dept=%d,Type=%d,WtExp=%d,SubCk=%d,wtTol=%d,PPWU=%d,MinwtTol&MaxWtTol=unknown"),lResult,csUPC,lDept,lTolType,lWtExp,lWtSubCk,lWtTol,lPPWU); //,lMinWtTol,lMaxWtTol); dont know em CMal012802
	mo.PostSASMMWtTol(SA_SMMTOLDATA,lResult,csUPC,lDept,lTolType,lWtExp,lWtSubCk,lWtTol,lPPWU,lMinWtTol,lMaxWtTol);
}
////////////////////////////////////////////////////////////////////////////
void DBMInterface::OnSMMWLDBwt(long lResult, LPCTSTR csUPC, long lDept, long lInitWt, long lAppr, long lDate, long lTally, long lAvgWt, long lPPWU, long lLastWtEntry) 
{
	
	if (lLastWtEntry)
	{
	trace(L6,_T("PostSASMMWLDBwt Res=%d,upc=%s,Dept=%d,InitWt=%d,Approved=%d,Date=%d,Tally=%d,AvgWt=%d,PPWU=%d,LASTWtEntry"),
			lResult,csUPC,lDept,lInitWt,lAppr,lDate,lTally,lAvgWt,lPPWU, lLastWtEntry);
	}
	else
	{
	trace(L6,_T("PostSASMMWtTol Res=%d,upc=%s,dpt=%d,Init=%d,Appr=%d,Date=%d,Tally=%d,Avg=%d,PPWU=%d,NotLastEntry"),
			lResult,csUPC,lDept,lInitWt,lAppr,lDate,lTally,lAvgWt,lPPWU);
	}
	mo.PostSASMMWLDBwt(SA_SMMWLDBDATA,lResult,csUPC,lDept,lInitWt,lAppr,lDate,lTally,lAvgWt,lPPWU,lLastWtEntry);

			
}
////////////////////////////////////////////////////////////////////////////
void DBMInterface::OnSMMWorkingStatus(long lResult) 
{
	trace(L6,_T("PostSASMMWorkingStatus Result: %d"),lResult );
	mo.PostSASMMWorkingStatus(SA_SMMSTATUSDATA,lResult);	
	
}
////////////////////////////////////////////////////////////////////////////
void DBMInterface::OnWLDBandItemExceptionInfo() 
{
	trace(L2, _T("+DBMInterface::OnWLDBandItemExceptionInfo")); //tar 193806
	trace(L6,_T("PostDB DBM_DBLOOKUPSUCCESS "));
	SMStateBase::TraceWtInfo(); //tar 193806
	mo.PostDB(DB_DATA, -1, -1, DBM_DBLOOKUPSUCCESS);
	trace(L2, _T("-DBMInterface::OnWLDBandItemExceptionInfo")); //tar 193806
}

////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(DBMInterface, CWnd)
	//{{AFX_MSG_MAP(DBMInterface)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDBMInterfaceApp construction

/////////////////////////////////////////////////////////////////////////////


