//////////////////////////////////////////////////////////////////////////////
//
// FILE:    DBMProcedures.cpp
//
// TITLE:   Database Manager (NCR API) Interface
//
//
// AUTHOR:  
//
//////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "Common.h"
#include "SMState.h" 
#include "dbmgrclient.h"
#include "DBMInterface.h"
#include "DBMProcedures.h"
#include "devmgr.h"
#include "ncrdevmgr.h"
#include "DMTriColorLight.h"
//#include "DMInterface.h"
#include "DMProcedures.h"


#define COMP_ID ID_SADBM               // used by the trace() calls to identify this component
#define T_ID        _T("DBMp")

/////////////////////////////////////////////////////////////////////////////
// SSProcedures

////////////////////////////
DBMProcedures::DBMProcedures()
{
  doDBMInit   = true;
  DBMICreated = false;
  DBMPCreated = false;
}

/////////////////////////////
DBMProcedures::~DBMProcedures()
{
  doDBMInit   = false;
  DBMICreated = false;
  DBMPCreated = false;
}

BEGIN_MESSAGE_MAP(DBMProcedures, CWnd)
//{{AFX_MSG_MAP(DBMProcedures)
// NOTE - the ClassWizard will add and remove mapping macros here.
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////
//
// 'Create' this window (dbmp) then the (dbmi) window object,
//   load OLE control and update status.
//
//////////////////////////////////////////
long DBMProcedures::Initialize(CWnd * pWnd)
{
  long rc;
  trace(L0, _T("+")_T(__TIMESTAMP__));
  trace(L4, _T("+Initialize"));
  if ( DBMICreated == false )
  {
    if( DBMPCreated == false )
    {
      if(pWnd == NULL)
      {
        if( !(rc = Create( NULL, NULL, WS_DISABLED, CRect(0,0,0,0), AfxGetMainWnd(), IDW_DBMPWINDOW )))
        {
          ScotError(RESTART,SM_CAT_STARTUP_INIT, SCOTAPP_DBMPROCEDURE_MAIN_WINDOW_FAILED);

        }
      }
      else
      {
        if( !(rc = Create( NULL, NULL, WS_DISABLED, CRect(0,0,0,0), pWnd, IDW_DBMPWINDOW )))
        {
          ScotError(RESTART,SM_CAT_STARTUP_INIT, SCOTAPP_SAPROCEDURE_WINDOW_FAILED);
        }
      }
			

      // load both English and the other language if dual language turned on
    /*  _TCHAR szActionFile[_MAX_PATH];
     
      _tmakepath(szActionFile,SCOTDRIVE,SCOTCONFIG, co.csDataSASIActionFile,"");
      ActionList = new CObList;
      if (LoadActions(szActionFile, ActionList)<0)
      {
        ScotError(RESTART,SM_CAT_STARTUP_INIT, SCOTAPP_SAPROCEDURE_LOAD_ACTION,"%s",szActionFile);
   
      }

      if (co.fStateDualLanguage)
      {
         int nLanguage;
         nLanguage = SCOT_LANGUAGE_SECONDARY;
         _tmakepath(szActionFile,SCOTDRIVE,SCOTCONFIG, co.language[nLanguage].csSecMessageFile,"");
         SecondActionList = new CObList;
         if (LoadActions(szActionFile, SecondActionList)<0)
		 {
           ScotError(RESTART,SM_CAT_STARTUP_INIT, SCOTAPP_SAPROCEDURE_LOAD_ACTION,"%s",szActionFile);
		 }
      }
*/
      DBMPCreated = true;
    }
    if( !(rc = dbi.Create( NULL, NULL, WS_DISABLED, CRect(0,0,0,0), this, IDW_DBMIWINDOW )))
    {
      ScotError(RESTART,SM_CAT_STARTUP_INIT, SCOTAPP_DBMINTERFACE_WINDOW_FAILED );

    }
    DBMICreated = true;
  }
  // caller of this routine
	if( doDBMInit )
	{
		rc = dbi.InitDBM();
		if(rc <= 0)
		{
			// anything special to be done if control creation or initSA fails
		}
	}
	trace(L4, _T("-Initialize"));
    trace(L0, _T("-")_T(__TIMESTAMP__));
	return rc;
}
/////////////////////////////////
long DBMProcedures::UnInitialize()
{
  long rc;
  trace(L2, _T("+UnInitialize"));
	
  if( rc = dbi.UnInitDBM() )
  {
  trace(L2, _T("After UnInitialize -- dbi:UnIniDBM"));
    ScotError(FATAL,SM_CAT_STOP_ABORT, SCOTAPP_DBMPROCEDURE_UNINIT);
  }
  if( !DestroyWindow() )					  // destroy this window
  {
  trace(L2, _T("After UnInitialize -- DestroyWindow"));
    ScotError(FATAL,SM_CAT_STOP_ABORT, SCOTAPP_DBMPROCEDURE_WINDOW_DESTROY);
  }
  DBMPCreated = false;
  DBMICreated = false;
  trace(L2, _T("-UnInitialize"));
  return rc;
}
////////////////////////////////////////////////////////////////////////////
long DBMProcedures::SetUp()
{
  BOOL r;
  trace(L2,_T("+SetUp"));
  r = dbi.SetUp();
  trace(L2,_T("-SetUp return: %d"), r);
  return (long)r;
}
////////////////////////////////////////////////////////////////////////////
long DBMProcedures::StartItemLookup(LPDISPATCH pWTInfoObj)
{
  BOOL r;

  trace(L2,_T("+StartItemLookup() UPC: %s. (Raw UPC)"), io.csItemCode);
  r = dbi.StartItemLookup(pWTInfoObj);
  trace(L2,_T("-StartItemLookup() return: %d"), r);
  return (long)r;
}
////////////////////////////////////////////////////////////////////////////
long DBMProcedures::GetDeptException(LPCTSTR DeptID, LPDISPATCH pWTInfoObj)
{
  BOOL r;
  trace(L2,_T("+GetDeptException DeptID: %s"), DeptID);
  r = dbi.GetDeptException(DeptID,pWTInfoObj);
  trace(L2,_T("-GetDeptException return: %d"), r);
  return (long)r;
}
////////////////////////////////////////////////////////////////////////////
long DBMProcedures::LastChanceLookup(LPDISPATCH pWTInfoObj)
{
  BOOL r;
  trace(L2,_T("+LastChanceLookup UPC: %s"), io.csItemCode);
  r = dbi.LastChanceLookup(pWTInfoObj);
  trace(L2,_T("-LastChanceLookup return: %d"), r);
  return (long)r;
}
////////////////////////////////////////////////////////////////////////////
long DBMProcedures::UpdateException(LPDISPATCH pWTInfoObj)
{
  BOOL r;
  trace(L2,_T("+UpdateException"));
  r = dbi.UpdateException(pWTInfoObj);
  trace(L2,_T("-UpdateException return: %d"), r);
  return (long)r;
}
////////////////////////////////////////////////////////////////////////////
long DBMProcedures::GetStatus()
{
  BOOL r;
  trace(L2,_T("+GetStatus"));
  r = dbi.GetStatus();
  trace(L2,_T("-GetStatus return: %d"), r);
  return (long)r;
}
////////////////////////////////////////////////////////////////////////////
long DBMProcedures::UpdateWLDB(LPDISPATCH pWTInfoObj)
{
  BOOL r;
  //tar 193806
  IWTInfoPtr pTempWTInfoObj = pWTInfoObj;
  CString csUPC;
  BSTR upc = NULL; //TAR 199035 SysAllocStringLen(NULL, 100);
  long litemWt, lwtIndex, lTally; 
  pTempWTInfoObj->get_csUPC(&upc);
  pTempWTInfoObj->get_lItemWT(&litemWt);
  pTempWTInfoObj->get_lExpectedWTIndex(&lwtIndex);
  pTempWTInfoObj->get_Tally(&lTally);
  csUPC = upc;
  //TAR 199035
  trace(L6,_T("+UpdateWLDB UPC: %s, litemWt: %d, lwtIndex: %d, lTally: %d"), csUPC, litemWt, lwtIndex, lTally);
  //SysFreeString(upc);Moving down to end of function
  //end of tar 193806

  r = dbi.UpdateWLDB(pWTInfoObj);
  pTempWTInfoObj->get_csUPC(&upc);
  csUPC = upc;
  trace(L6,_T("-UpdateWLDB return: %d, UPC after update %s"), r, csUPC );
  SysFreeString(upc);
  return (long)r;
}
////////////////////////////////////////////////////////////////////////////
long DBMProcedures::TESTStartItemLookup(LPCTSTR csUPC)
{
  BOOL r;
  trace(L2,_T("+TESTStartItemLookup csUPC: %s"),csUPC);
  r = dbi.TESTStartItemLookup(csUPC);
  trace(L2,_T("-TESTStartItemLookup rc=%d"), r);
  return (long)r;
}
////////////////////////////////////////////////////////////////////////////
long DBMProcedures::TESTLastChanceLookup(LPCTSTR csUPC)
{
  BOOL r;
  trace(L2,_T("+TESTLastChanceLookup csUPC: %s"), csUPC);
  r = dbi.TESTLastChanceLookup(csUPC);
  trace(L2,_T("-TESTLastChanceLookup"));
  return (long)r;
}
////////////////////////////////////////////////////////////////////////////
long DBMProcedures::QuerySMMTol(LPCTSTR csUPC, long lDept, long lTolType, long lPPWU)
{
  BOOL r;
  CString upc = csUPC;
  trace(L2,_T("+QuerySMMTol csUPC: %s, lDept: %d, lTolType: %d, lPPWU: %d"), upc,lDept,lTolType,lPPWU);
  r = dbi.QuerySMMTol(csUPC,lDept,lTolType,lPPWU);
  trace(L2,_T("-QuerySMMTol return: %d"), r);
  return (long)r;
}
////////////////////////////////////////////////////////////////////////////
long DBMProcedures::QuerySMMWLDBwt(LPCTSTR csUPC, long lDept, LPCTSTR csDesc, long lPrice, long lSpecialItem, long lPPWU)
{
  BOOL r;

  if (lPPWU)
	csUPC = io.csItemCodeWOPrice;

  trace(L2,_T("+QuerySMMWLDBwt csUPC: %s, lDept: %d ,csDesc: %s, lPrice: %d, lSpecialItem: %d, lPPWU: %d"), csUPC,lDept,csDesc,lPrice,lSpecialItem,lPPWU);
  r = dbi.QuerySMMWLDBwt(csUPC,lDept,csDesc,lPrice,lSpecialItem,lPPWU);
  trace(L2,_T("-QuerySMMWLDBwt return: %d"), r);
  return (long)r;
}
////////////////////////////////////////////////////////////////////////////
long DBMProcedures::UpdateSMMTol(long lAction, LPCTSTR csUPC, long lDept, long lTolType, long lWtExp, long lWtSubCk, long lWtTol, long lPPWU)
{
  BOOL r;
  trace(L2,_T("+UpdateSMMTol lAction: %d,csUPC: %s, lDept: %d, lTolType: %d, lWtExp: %d, lWtSubCk: %d, lWtTol: %d,lPPWU: %d"), lAction,csUPC,lDept,lTolType,lWtExp,lWtSubCk,lWtTol,lPPWU);
  r = dbi.UpdateSMMTol(lAction,csUPC,lDept,lTolType,lWtExp,lWtSubCk,lWtTol,lPPWU);
  trace(L2,_T("-UpdateSMMTol return: %d"), r);
  return (long)r;
}
////////////////////////////////////////////////////////////////////////////
long DBMProcedures::UpdateSMMWLDBwt(long lAction, LPCTSTR csUPC, long lDept, long lBSwt, long lAppr, long lTally, long lDate, long lInitWt, long lPPWU)
{
  BOOL r;
  trace(L2,_T("+UpdateSMMWLDBwt lAction: %d, csUPC: %s, lDept: %d, lBSwt: %d, lAppr: %d, lTally: %d, lDate: %d, lInitWt: %d, lPPWU: %d"), lAction,csUPC,lDept,lBSwt,lAppr,lTally,lDate,lInitWt,lPPWU);
  r = dbi.UpdateSMMWLDBwt(lAction,csUPC,lDept,lBSwt,lAppr,lTally,lDate,lInitWt,lPPWU);
  trace(L2,_T("-UpdateSMMWLDBwt return: %d"), r);
  return (long)r;
}
////////////////////////////////////////////////////////////////////////////
void DBMProcedures::AboutBox()
{
	dbi.AboutBox();
}