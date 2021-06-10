//////////////////////////////////////////////////////////////////////////////
//
// FILE:    SecMgrProcedures.cpp
//
// TITLE:   Smart Scale  (NCR API) Interface
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

#include "stdafx.h"
#include "Common.h"
#include "SecMgrInterface.h"
#include "SecMgrProcedures.h"

#define COMP_ID ID_SECMGR               // used by the trace() calls to identify this component
#define T_ID        _T("SecMgrp")
/////////////////////////////////////////////////////////////////////////////
// SSProcedures

////////////////////////////
SecMgrProcedures::SecMgrProcedures()
{
  doSecMgrInit   = true;
  SecMgrICreated = false;
  SecMgrPCreated = false;
}

/////////////////////////////
SecMgrProcedures::~SecMgrProcedures()
{
  doSecMgrInit   = false;
  SecMgrICreated = false;
  SecMgrPCreated = false;
}

BEGIN_MESSAGE_MAP(SecMgrProcedures, CWnd)
//{{AFX_MSG_MAP(SecMgrProcedures)
// NOTE - the ClassWizard will add and remove mapping macros here.
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////
long SecMgrProcedures::Initialize(CWnd * pWnd)
{
  long rc;
  trace(L0, _T("+")_T(__TIMESTAMP__));
  trace(L4, _T("+Initialize"));
  if ( SecMgrICreated == false )
  {
    if( SecMgrPCreated == false )
    {
      if(pWnd == NULL)
      {
        if( !(rc = Create( NULL, NULL, WS_DISABLED, CRect(0,0,0,0), AfxGetMainWnd(), IDW_SECMGRPWINDOW )))
        {
          ScotError(RESTART,SM_CAT_STARTUP_INIT, SCOTAPP_SEC_MGR_PROCEDURE_MAIN_WINDOW_FAILED);
        }
      }
      else
      {
        if( !(rc = Create( NULL, NULL, WS_DISABLED, CRect(0,0,0,0), pWnd, IDW_SECMGRPWINDOW )))
        {
          ScotError(RESTART,SM_CAT_STARTUP_INIT, SCOTAPP_SEC_MGR_PROCEDURE_WINDOW_FAILED);
        }
      }
      SecMgrPCreated = true;
    }
    if( !(rc = SecMgri.Create( NULL, NULL, WS_DISABLED, CRect(0,0,0,0), this, IDW_SECMGRIWINDOW )))
    {
      ScotError(RESTART,SM_CAT_STARTUP_INIT, SCOTAPP_SEC_MGR_INTERFACE_WINDOW_FAILED );

    }
    SecMgrICreated = true;
  }
  // caller of this routine
	if( doSecMgrInit )
	{
		if (!( SecMgri.InitSecMgr()) )
		{
			rc = false;
		}
		else
		{
			rc = true;
		}
	}
	trace(L4, _T("-Initialize"));
	trace(L0, _T("-")_T(__TIMESTAMP__));
	return rc;
}
/////////////////////////////////
long SecMgrProcedures::UnInitialize()
{
  bool rc;
  trace(L2, _T("+UnInitialize"));
		
  if( rc = SecMgri.UnInitSecMgr() )
  {
  trace(L2, _T("After UnInitialize -- SecMgri:UnInitSecMgr"));
    ScotError(FATAL,SM_CAT_STOP_ABORT, SCOTAPP_SEC_MGR_PROCEDURE_UNINIT);
  }
  if( !DestroyWindow() )					  // destroy this window
  {
  trace(L2, _T("After UnInitialize -- DestroyWindow"));
    ScotError(FATAL,SM_CAT_STOP_ABORT, SCOTAPP_SEC_MGR_PROCEDURE_WINDOW_DESTROY);
  }
  SecMgrPCreated = false;
  SecMgrICreated = false;
  trace(L2, _T("-UnInitialize"));
  return rc;
}
/////////////////////////////////
CString SecMgrProcedures::GetOutstandingInterventionList()
{
  CString strResult;
  trace(L2,_T("+GetOutstandingInterventionList()"));
  strResult = SecMgri.GetOutstandingInterventionList();
  trace(L2,_T("-GetOutstandingInterventionList() return: %s"), strResult);
  return strResult;
}


/////////////////////////////////
CString SecMgrProcedures::GetDevices()
{
  CString strResult;
  trace(L2,_T("+GetDevices()"));
  strResult = SecMgri.GetDevices();
  trace(L2,_T("-GetDevices return: %s"), strResult);
  return strResult;
}

long SecMgrProcedures::GetOutstandingInterventionCount()
{
  long lResult;
  trace(L2,_T("+GetOutstandingInterventionCount()"));
  lResult = SecMgri.GetOutstandingInterventionCount();
  trace(L2,_T("-GetOutstandingInterventionCount return: %d"), lResult);
  return lResult;
}
////////////////////////////////////////////////////////////////////////////

void SecMgrProcedures::SecurityUnadvise(long dwCookie)
{
  trace(L2,_T("+SecurityUnadvise: %d"), dwCookie);
  SecMgri.SecurityUnadvise(dwCookie);
  trace(L2,_T("-SecurityUnadvise"));
  
}
/////////////////////////////////
long SecMgrProcedures::SecurityAdvise(long lFlags)
{
  long r;
  trace(L2,_T("+SecurityAdvise lFlags: %d"), lFlags);
  r = SecMgri.SecurityAdvise(lFlags);
  trace(L2,_T("-SecurityAdvise lFlags: %d"), r);
  return r;
}

////////////////////////////////////////////////////////////////////////////

void SecMgrProcedures::ItemSold(long lCookie, LPCTSTR bstrParms)
{
  // +TAR 279504
  if ( 0 != lCookie )
  {
	trace(L2,_T("+ItemSold lCookie: %d bstrParms: %s"), lCookie, bstrParms );
	SecMgri.ItemSold(lCookie, bstrParms);
	trace(L2,_T("-ItemSold"));
  }
  else
  {
	  trace(L2,_T("+/-ItemSold - invalid cookie %d"),lCookie );
  } 
  // -TAR 279504
}
////////////////////////////////////////////////////////////////////////////
	
void SecMgrProcedures::ItemDetails(long lCookie, LPCTSTR bstrParms)
{
  // +TAR 279504
  if ( 0 != lCookie )
  {
	trace(L2,_T("+ItemDetails lCookie: %d bstrParms: %s"), lCookie, bstrParms );
	SecMgri.ItemDetails(lCookie, bstrParms);
	trace(L2,_T("-ItemDetails"));
  }
  else
  {
	  trace(L2,_T("+/-ItemDetails - invalid cookie %d"),lCookie );
  }   
  // -TAR 279504
}
////////////////////////////////////////////////////////////////////////////
	
void SecMgrProcedures::VoidItem(long lCookie, LPCTSTR bstrParms)
{
  // +TAR 279504
  if ( 0 != lCookie )
  {
	trace(L2,_T("+VoidItem lCookie: %d bstrParms: %s"), lCookie, bstrParms );
	SecMgri.VoidItem(lCookie, bstrParms);
	trace(L2,_T("-VoidItem"));
  }
  else
  {
	  trace(L2,_T("+/-VoidItem - invalid cookie %d"),lCookie );
  }  
  // -TAR 279504
}
////////////////////////////////////////////////////////////////////////////
	
void SecMgrProcedures::TransactionControl(long lCookie, LPCTSTR bstrParms)
{
  // +TAR 279504
  if ( 0 != lCookie )
  {
	trace(L2,_T("+TransactionControl lCookie: %d bstrParms: %s"), lCookie, bstrParms );
	SecMgri.TransactionControl(lCookie, bstrParms);
	trace(L2,_T("-TransactionControl"));
  }
  else
  {
	  trace(L2,_T("+/-TransactionControl - invalid cookie %d"),lCookie );
  }
  // -TAR 279504
}
////////////////////////////////////////////////////////////////////////////

long SecMgrProcedures::AppControl(long lCookie, LPCTSTR bstrParms, BSTR* bstrOutParms)
{
  long r;
  // +TAR 279504
  if ( 0 != lCookie )
  {
	 trace(L2,_T("+AppControl lCookie: %d bstrParms: %s"), lCookie, bstrParms );
	 r = SecMgri.AppControl(lCookie, bstrParms, bstrOutParms);
	 trace(L2,_T("-AppControl"));
	 return r;
  }
  else
  {	  
	  trace(L2,_T("+/-AppControl - invalid cookie %d"),lCookie );
	  return 0;
  }
  // -TAR 279504
}
////////////////////////////////////////////////////////////////////////////
	
void SecMgrProcedures::OverrideException(long lCookie, LPCTSTR bstrParms)
{
  // +TAR 279504
  if ( 0 != lCookie )
  {
	trace(L2,_T("+OverrideException lCookie: %d bstrParms: %s"), lCookie, bstrParms );
	SecMgri.OverrideException(lCookie, bstrParms);
	trace(L2,_T("-OverrideException"));
  }
  else
  {
	  trace(L2,_T("+/-OverrideException - invalid cookie %d"),lCookie );
  }
  // -TAR 279504
}
////////////////////////////////////////////////////////////////////////////
	
long SecMgrProcedures::SecurityMaintenance(long lCookie, LPCTSTR bstrParms)
{
  // +TAR 279504
  if ( 0 != lCookie )
  {
	long r;
	trace(L2,_T("+SecurityMaintenance lCookie: %d bstrParms: %s"), lCookie, bstrParms );
	r = SecMgri.SecurityMaintenance(lCookie, bstrParms);
	trace(L2,_T("-SecurityMaintenance"));
	return r;
  }
  else
  {
	  trace(L2,_T("+/-SecurityMaintenance - invalid cookie %d"),lCookie );
	  return 0;
  }
  // -TAR 279504
}
////////////////////////////////////////////////////////////////////////////

void SecMgrProcedures::SecurityControl(long lCookie, LPCTSTR bstrInParms, BSTR* bstrOutParms)
{
  // +TAR 279504
  if ( 0 != lCookie )
  {
	trace(L2,_T("+SecurityControl lCookie: %d bstrInParms: %s "), lCookie, bstrInParms);
	SecMgri.SecurityControl(lCookie, bstrInParms, bstrOutParms);
	trace(L2,_T("-SecurityControl"));
  }
  else
  {
	  trace(L2,_T("+/-SecurityControl - invalid cookie %d"),lCookie );
  }  
  // -TAR 279504
}
////////////////////////////////////////////////////////////////////////////

long SecMgrProcedures::Initialize(long lCookie, LPCTSTR bstrInParms, BSTR* pbstrValues)
{
  // +TAR 279504
  if ( 0 != lCookie )
  {
	long r;
	_ASSERTE( _CrtCheckMemory() );
	trace(L2,_T("+Initialize lCookie: %d bstrInParms: %s "), lCookie, bstrInParms );
	r = SecMgri.Initialize(lCookie, bstrInParms, pbstrValues);
	trace(L2,_T("-Initialize pbstrValues: %s"), 
	  *pbstrValues ? CString(*pbstrValues) : _T("<null>") );
	_ASSERTE( _CrtCheckMemory() );
	return r;
  }
  else
  {
	  trace(L2,_T("+/-Initialize - invalid cookie %d"),lCookie );
	  return S_OK;
  }  
  // -TAR 279504
}
////////////////////////////////////////////////////////////////////////////

