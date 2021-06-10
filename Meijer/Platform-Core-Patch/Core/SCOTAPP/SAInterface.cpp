//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  FILE:      SAInterface.cpp
//
//  TITLE:     Security Agent (NCR API) Interface
//
//
//  AUTHOR:    DcL-
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Common.h"


#include "SAInterface.h"

#define COMP_ID ID_SA

#define T_ID _T("SAi")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PROCESS_EXCEPTIONS(a)                                 \
	catch(COleDispatchException * e)                              \
{                                                             \
  ScotError(RETURN,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_SAINTERFACE_OLEDISPATCH_EXCEPTION, \
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
  ScotError(RETURN,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_SAINTERFACE_OLE_EXCEPTION,_T("%s|%s|%x"),        \
	msg,                                                      \
	cstr,                                                     \
	e->m_sc);                                                 \
  e->Delete();                                               \
  return a;                                                   \
}                                                             \
	catch(CException* e)                                          \
{                                                             \
  e->GetErrorMessage( cstr, 255 );                           \
  ScotError(RETURN,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_SAINTERFACE_C_EXCEPTION,_T("%s|%s"),                   \
	msg,                                                      \
	cstr);                                                    \
  e->Delete();                                               \
  return a;                                                   \
}                                                             \
	catch(...)                                                    \
{                                                             \
  long le = GetLastError();                                   \
  ScotError(RETURN,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_SAINTERFACE_EXCEPTION,_T("%s|%d"),msg,le);            \
  return a;                                                   \
}


/////////////////////////////////////////////////////////////////////////////
// SAInterface

SAInterface::SAInterface()
{
  SAOpened  = false;
  SACreated = false;
}

SAInterface::~SAInterface()
{
  SAOpened  = false;
  SACreated = false;
}

//////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Open the Security Agent
//
bool SAInterface::InitSA()
{
  _TCHAR msg[] = T_ID _T("::InitSA");
	
  if ( SACreated == false )
  {
    if( !sa.Create(NULL, NULL, CRect(0,0,0,0), this, IDC_SECURITYAGENTCTRL1 ))
    {
	  ScotError(INFO,SM_CAT_STOP_ABORT, SCOTAPP_SAINTERFACE_STOP);
	  return false;
    }
	else
	{
      SACreated = true;
	}
		
    try
    {
      if (SAOpened == false )
      {
        // sa.SetOnline(true);				  // not needed in this revision
        // if( !sa.GetOnline() )
        //  ScotError( RETURN, "SA Not Online" );
        SAOpened = true;
      }
    }
    PROCESS_EXCEPTIONS(false);
		
  }// end - if ( SACreated == false )
  return true;
}


////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// Close Security Agent
//
bool SAInterface::UnInitSA()
{
  trace(L2, _T("+UnInitSA"));
  _TCHAR msg[] = T_ID _T("::UnInitSA");
  bool rc;
  try
  {
    if (SAOpened == true )
    {
	  sa.SendSCOTInput(50, 0,0,0,0,0);
      trace(L2, _T("UnInitSA -- before SetOnline"));
      sa.SetOnline(false);	// use after updated version
      trace(L2, _T("after SetOnline"));
	  try
	  {
      rc = !sa.DestroyWindow();	// true - if succesful
	  }
	  catch(CException* e)
	  {
        _TCHAR szBuff[300];
		e->GetErrorMessage(szBuff,300);
		trace(L2, _T("Got an exception in sa.DestroyWindow() :Error text:%s"), szBuff);
		e->Delete(); //TAR247967

	  }
       trace(L2, _T("after DestroyWindow"));
     SAOpened = false;
      SACreated = false;
    }
  trace(L2, _T("UnInitSA -- before PROCESS_EXCEPTIONS"));
  }
  PROCESS_EXCEPTIONS(false);
  trace(L2, _T("-UnInitSA"));
  return rc;
}


////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// Security Agent Functions
//
void SAInterface::AboutBox(void)
{
  _TCHAR msg[] = T_ID _T("::AboutBox");
	
  try
  {
    sa.AboutBox();
  }
  PROCESS_EXCEPTIONS(;);
	
  return;
}


//////////////////////////////////////////////////////////////////////////
long SAInterface::GetSecurityLevel()
{
  _TCHAR msg[] = T_ID _T("::GetSecurityLevel");
  long r = 0;
	
  try
  {
    r = sa.GetSecurityLevel();
  }
  PROCESS_EXCEPTIONS(r);
  return r;
}


///////////////////////////////////////////////////////////////////////////
bool SAInterface::GetOnline()
{
  _TCHAR msg[] = T_ID _T("::GetOnline");
  bool r = false;
	
  try
  {
    r = (sa.GetOnline() ? true:false);
  }
  PROCESS_EXCEPTIONS(false);
  return r;
}

//////////////////////////////////////////////////////////////////////////
// BeginCodeWLDBSA11
bool SAInterface::SendSCOTInput(long ID, long Department, LPCTSTR UPCNumber, long Weight, long Price, long Quantity)
{
  _TCHAR msg[] = T_ID _T("::SendSCOTInput");
  bool r = false;
  try
  {
    r = (sa.SendSCOTInput( ID, Department, UPCNumber, Weight, Price, Quantity) ? true:false);
  }
  PROCESS_EXCEPTIONS(false);
  return r;
}
// EndCodeWLDBSA11

// A Mgg
//////////////////////////////////////////////////////////////////////////////
bool SAInterface::VoidRatioOK(long GrossSales, long VoidSales)
{
  _TCHAR msg[] = T_ID _T("::VoidRatioOK");
  bool r = false;
	
  try
  {
    // Bad luck, VoidSales is neg...
    r = (sa.VoidRatioOK(GrossSales, abs(VoidSales)) ? true:false);
  }
  PROCESS_EXCEPTIONS(false);
  return r;
}


//////////////////////////////////////////////////////////////////////////////
bool SAInterface::ClearVoidTallies()
{
  _TCHAR msg[] = T_ID _T("::ClearVoidTallies");
  bool r = false;
	
  try
  {
    r = (sa.ClearVoidTallies() ? true:false);
  }
  PROCESS_EXCEPTIONS(false);
  return r;
}


//////////////////////////////////////////////////////////////////////////////
bool SAInterface::ClearOtherTallies()
{
  _TCHAR msg[] = T_ID _T("::ClearOtherTallies");
  bool r = false;
	
  try
  {
    r = (sa.ClearOtherTallies() ? true:false);
  }
  PROCESS_EXCEPTIONS(false);
  return r;
}


//////////////////////////////////////////////////////////////////////////////
bool SAInterface::SendScaleInput(long Weight, long ReturnCode, long ReturnCodeExt)
{
  _TCHAR msg[] = T_ID _T("::SendScaleInput");
  bool r = false;
	
  try
  {
    r = (sa.SendScaleInput(Weight, ReturnCode, ReturnCodeExt) ? true:false);
  }
  PROCESS_EXCEPTIONS(false);
  return r;
}


//////////////////////////////////////////////////////////////////////////////
bool SAInterface::TransactionAbortOk(long GrossSales)
{
  _TCHAR msg[] = T_ID _T("::TransactionAbortOk");
  bool r = false;
	
  try
  {
    r = (sa.TransactionAbortOk(GrossSales) ? true:false);
  }
  PROCESS_EXCEPTIONS(false);
  return r;
}


//////////////////////////////////////////////////////////////////////////////
bool SAInterface::CouponRatioOK(long GrossSales, long CouponTotals, long Type)
{
  _TCHAR msg[] = T_ID _T("::CouponRatioOK");
  bool r = false;
	
  try
  {
    r = (sa.CouponRatioOK(GrossSales, CouponTotals, Type) ? true:false);
  }
  PROCESS_EXCEPTIONS(false);
  return r;
}


//////////////////////////////////////////////////////////////////////////////
bool SAInterface::ClearCouponTallies(long Amount, long Type)
{
  _TCHAR msg[] = T_ID _T("::ClearCouponTallies");
  bool r = false;
	
  try
  {
    r = (sa.ClearCouponTallies(Amount, Type) ? true:false);
  }
  PROCESS_EXCEPTIONS(false);
  return r;
}

//////////////////////////////////////////////////////////////////////////////
bool SAInterface::SendSCOTInputWithOverrides(long ID, long Dept, LPCTSTR UPC, long Weight, long Price, long Quantity, long lRequireSecBagging, long lRequireSecSubstChk)
{
  _TCHAR msg[] = T_ID _T("::SendSCOTInputWithOverrides");
  bool r = false;
	
  try
  {
    r = (sa.SendSCOTInputWithOverrides(ID, Dept, UPC, Weight, Price, Quantity, lRequireSecBagging, lRequireSecSubstChk) ? true:false);
  }
  PROCESS_EXCEPTIONS(false);
  return r;
}

// E MGG1


BEGIN_MESSAGE_MAP(SAInterface, CWnd)
//{{AFX_MSG_MAP(SAInterface)
// NOTE - the ClassWizard will add and remove mapping macros here.
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// Application shuting down, Close Security Agent so that it does not remain
// in memory.
//
void SAInterface::OnDestroy()
{
  UnInitSA();
  CWnd::OnDestroy();							  // now finish up with the rest of the termination.
}


/////////////////////////////////////////////////////////////////////////////
// DMInterface message handlers
BOOL SAInterface::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
  sa.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
  return CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// BeginCodeWLDBSA11
BEGIN_EVENTSINK_MAP(SAInterface, CWnd)
//{{AFX_EVENTSINK_MAP(DMInterface)
	ON_EVENT(SAInterface, IDC_SECURITYAGENTCTRL1, 1 /* SecurityOutput */, OnSecurityOutputSecurityagentctrl1, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(SAInterface, IDC_SECURITYAGENTCTRL1, 2 /* SMMWorkingStatus */, OnSMMWorkingStatusSecurityagentctrl1, VTS_I4)
	ON_EVENT(SAInterface, IDC_SECURITYAGENTCTRL1, 3 /* SMMBagScaleWt */, OnSMMBagScaleWtSecurityagentctrl1, VTS_I4)
	ON_EVENT(SAInterface, IDC_SECURITYAGENTCTRL1, 4 /* SMMTol */, OnSMMTolSecurityagentctrl1, VTS_I4 VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(SAInterface, IDC_SECURITYAGENTCTRL1, 5 /* SMMWLDBwt */, OnSMMWLDBwtSecurityagentctrl1, VTS_I4 VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// SAInterface message handlers
void SAInterface::OnSecurityOutputSecurityagentctrl1(long OutputConclusion, long OutputLevel, long OutputType, long InputID)
{
  if ((OutputConclusion == 0) && (OutputType == 0) ||	// Security says No Conclusion - no need to process		TAR165402  LPM031501
	  (OutputConclusion == -1) && (OutputType == 4) ||	// Security says dont reenable s/s - no need to process
	  (OutputConclusion == -2) && (OutputType == 4) )	// Security says dont reenable s/s - no need to process
  {
	//166544 Preformance enhancement: Do Not Remove this trace: Still want to trace this information even tho not processing it.  LPM031501
	trace(L2, _T("Ignoring non-relevant Security output messages. Concl/Context=%d, Level=%d, Type=%d, InputID=%d"), OutputConclusion, OutputLevel, OutputType, InputID);
  }
  else
  {
	mo.PostSA( SA_DATA, OutputConclusion, OutputLevel, OutputType, InputID );
  }
}

//////////////////////////////////////
//									//
// begin Security Maintenance Mode  //
//									//
//////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Query the SMM Weight Tolerance info
/////////////////////////////////////////////////////////////////
bool SAInterface::QuerySMMTol( LPCTSTR csUPCNumber, long lDepartment,  long lTolType, long lPPWU )
{
  _TCHAR msg[] = T_ID _T("::QuerySMMTol");
  bool r = false;
  try
  {
    r = (sa.QuerySMMTol( csUPCNumber, lDepartment, lTolType, lPPWU ) ? true:false);
  }
  PROCESS_EXCEPTIONS(false);
  return r;
}

/////////////////////////////////////////////////////////////////
// Update the SMM Weight Tolerance info
/////////////////////////////////////////////////////////////////
bool SAInterface::UpdateSMMTol( long lAction, LPCTSTR csUPCNumber, long lDepartment,  long lTolType, long lWtExpectation, long lWtSubCk, long lWtTolerance, long lPPWU )
{
  _TCHAR msg[] = T_ID _T("::UpdateSMMTol");
  bool r = false;
  try
  {
    r = (sa.UpdateSMMTol( lAction, csUPCNumber, lDepartment, lTolType, lWtExpectation, lWtSubCk, lWtTolerance, lPPWU ) ? true:false);
  }
  PROCESS_EXCEPTIONS(false);
  return r;
}

/////////////////////////////////////////////////////////////////
// Query the SMM Weight Tolerance info
/////////////////////////////////////////////////////////////////
bool SAInterface::QuerySMMWLDBwt( LPCTSTR csUPCNumber, long lDepartment, LPCTSTR csDescription, long lPrice, long lSpecialItem, long lPPWU )
{
  _TCHAR msg[] = T_ID _T("::QuerySMMWLDBwt");
  bool r = false;
  try
  {
    r = (sa.QuerySMMWLDBwt( csUPCNumber, lDepartment, csDescription, lPrice, lSpecialItem, lPPWU ) ? true:false);
  }
  PROCESS_EXCEPTIONS(false);
  return r;
}

/////////////////////////////////////////////////////////////////
// Update the SMM Weight Tolerance info
/////////////////////////////////////////////////////////////////
bool SAInterface::UpdateSMMWLDBwt( long lAction, LPCTSTR csUPCNumber, long lDepartment, long lBagScaleWt, long lApproval, long lTally, long lDate, long lInitialWt, long lPPWU )
{
  _TCHAR msg[] = T_ID _T("::UpdateSMMWLDBwt");
  bool r = false;
  try
  {
    r = (sa.UpdateSMMWLDBwt( lAction, csUPCNumber, lDepartment, lBagScaleWt, lApproval, lTally, lDate, lInitialWt, lPPWU ) ? true:false);
  }
  PROCESS_EXCEPTIONS(false);
  return r;
}

/////////////////////////////////////////////////////////////////
// Fire results of Query-ing the SMM Weight Tolerance info
/////////////////////////////////////////////////////////////////
void SAInterface::OnSMMTolSecurityagentctrl1(long lResult, LPCTSTR csUPC, long lDept, long lTolType, long lWtExp, long lWtSubCk, long lWtTol, long lPPWU, long lMinWtTol, long lMaxWtTol)
{
	mo.PostSASMMWtTol( SA_SMMTOLDATA, lResult, csUPC, lDept, lTolType, lWtExp, lWtSubCk, lWtTol, lPPWU, lMinWtTol, lMaxWtTol );
}

/////////////////////////////////////////////////////////////////
// Fire results of Query-ing the Security working status
/////////////////////////////////////////////////////////////////
void SAInterface::OnSMMWorkingStatusSecurityagentctrl1(long lResult) 
{
	mo.PostSASMMWorkingStatus( SA_SMMSTATUSDATA, lResult );
}

/////////////////////////////////////////////////////////////////
// Fire results of a bag scale increase
/////////////////////////////////////////////////////////////////
void SAInterface::OnSMMBagScaleWtSecurityagentctrl1(long lBagScaleWt) 
{
	mo.PostSASMMBSwt( SA_SMMBSDATA, lBagScaleWt );
}

/////////////////////////////////////////////////////////////////
// Fire results of Query-ing or Updating the WLDB for a specific UPC
/////////////////////////////////////////////////////////////////
void SAInterface::OnSMMWLDBwtSecurityagentctrl1(long lResult, LPCTSTR csUPC, long lDept, long lInitWt, long lAppr, long lDate, long lTally, long lAvgWt, long lPPWU, long lLastWtEntry) 
{
	mo.PostSASMMWLDBwt( SA_SMMWLDBDATA, lResult, csUPC, lDept, lInitWt, lAppr, lDate, lTally, lAvgWt, lPPWU, lLastWtEntry );
}
