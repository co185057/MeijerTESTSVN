//////////////////////////////////////////////////////////////////////////////
//
// FILE:    SAProcedures.cpp
//
// TITLE:   Security Agent (NCR API) Interface
//
//
// AUTHOR:  DcL-
//
//////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "Common.h"

#include "securityagent.h"
#include "SAInterface.h"
#include "SAProcedures.h"

#include "devmgr.h"
#include "ncrdevmgr.h"
#include "DMTriColorLight.h"
//#include "DMInterface.h"
#include "DMProcedures.h"

#define COMP_ID ID_SA               // used by the trace() calls to identify this component
#define T_ID        _T("SAp")

//////////////////////////
ActionEntry::ActionEntry()          // constructor
{
  nConclusion = 0;
  szLogText = NULL;
  for (int j=0;j<MAX_SECLEVEL;j++)
  {
    szBar[j]    = NULL;
    szAudio[j]  = NULL;
    szPopup[j]  = NULL;
  }
}

///////////////////////////
ActionEntry::~ActionEntry()         // destructor
{
  if (szLogText)  delete szLogText;
  for (int j=0;j<MAX_SECLEVEL;j++)
  {
    if (szBar[j])   delete szBar[j];
    if (szAudio[j]) delete szAudio[j];
    if (szPopup[j]) delete szPopup[j];
  }
}

/////////////////////////
long GetTimeStamp( void )
{
  time_t long_time;
  time( &long_time );
  return long_time;
}

/////////////////////////////////////////////////////////////////////////////
// SAProcedures

////////////////////////////
SAProcedures::SAProcedures()
{
  doSAInit   = true;
  SAICreated = false;
  SAPCreated = false;
  LastConclusion;									  // 0 = none
  CurrentActiveConclusion;				  // 0 = none
}

/////////////////////////////
SAProcedures::~SAProcedures()
{
  doSAInit   = false;
  SAICreated = false;
  SAPCreated = false;
  LastConclusion;									  // 0 = none
  CurrentActiveConclusion;				  // 0 = none
}


////////////////////////////////////////////////////////////////////////////
//
// Security Agent Functions
//
/////////////////////////////
long SAProcedures::AboutBox()
{
  trace(L2, _T("+AboutBox"));
  sai.AboutBox();
  trace(L2, _T("-AboutBox"));
  return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BeginCodeWLDBSA11
long SAProcedures::SendSCOTInput(long ID, long Department, LPCTSTR UPCNumber, long Weight, long Price, long Quantity)
{
  BOOL r;
  //TAR#135045 Make trace info easier to read
  trace(L2, _T("+SendSCOTInput: ID:%d Dpt:%d UPC:%s Wt:%d Pr:%d Qty:%d"), ID, Department, UPCNumber, Weight, Price, Quantity);
  if (!SAICreated)// if not created return error.
    return 1;											  // pd980329 DcL-033098 Checked file for other snafues, if not initialized return success Mgg
  if( !(r = sai.SendSCOTInput( ID, Department, UPCNumber, Weight, Price, Quantity )))
  {
    ScotError( RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_SAPROCEDURE_SENDSCOTINPUT);

  }
  trace(L2, _T("-SendSCOTInput rc:%d"), r);	//TAR#135045 Make trace info easier to read
  return (long)r;
}
// EndCodeWLDBSA11

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
long SAProcedures::SendSCOTInputWithOverrides(long ID, long Dept, LPCTSTR UPC, long Weight, long Price, long Quantity, long lRequireSecBagging, long lRequireSecSubstChk)
{
  BOOL r;
  trace(L2, _T("+SendSCOTInputWithOverrides: ID:%d Dpt:%d UPC:%s Wt:%d Pr:%d Qty:%d Bg Req:%d SC Req:%d"), ID, Dept, UPC, Weight, Price, Quantity, lRequireSecBagging, lRequireSecSubstChk);
  if (!SAICreated)// if not created return error.
    return 1;											  // pd980329 DcL-033098 Checked file for other snafues, if not initialized return success Msg
  if( !(r = sai.SendSCOTInputWithOverrides( ID, Dept, UPC, Weight, Price, Quantity, lRequireSecBagging, lRequireSecSubstChk )))
  {
    ScotError( RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_SAPROCEDURE_SENDSCOTINPUT);

  }
  trace(L2, _T("-SendSCOTInputWithOverrides rc:%d"), r);	
  return (long)r;
}

/////////////////////////////////////
long SAProcedures::GetSecurityLevel()
{
  long r;
  trace(L2, _T("+GetSecurityLevel"));
  r = sai.GetSecurityLevel();
  trace(L2, _T("-GetSecurityLevel"));
  return r;
}

//////////////////////////////////
long SAProcedures::GetOnline(void)
{
  BOOL r;
  trace(L2,_T("+GetOnline"));
  r = sai.GetOnline();
  trace(L2,_T("-GetOnline"));
  return (long)r;
}


///////////////////////////////////////////////////////////////
long SAProcedures::VoidRatioOK(long GrossSales, long VoidSales)
{
  long r;
  trace(L2, _T("+SAVoidRatioOK: GrossSale <%d> VoidSales <%d>"), GrossSales, VoidSales);
  r = (long)sai.VoidRatioOK(GrossSales, VoidSales);
  trace(L2, _T("-SAVoidRatioOK: return <%d>"), r);
  return r;
}

/////////////////////////////////////
long SAProcedures::ClearVoidTallies()
{
  long r;
  trace(L2, _T("+SAClearVoidTallies"));
  r = (long)sai.ClearVoidTallies();
  trace(L2, _T("-SAClearVoidTallies: return <%d>"), r);
  return r;
}

/////////////////////////////////////
long SAProcedures::ClearOtherTallies()
{
  long r;
  trace(L2, _T("+SAClearOtherTallies"));
  r = (long)sai.ClearOtherTallies();
  trace(L2, _T("-SAClearOtherTallies: return <%d>"), r);
  return r;
}

///////////////////////////////////////////////////////////////////////////////////
long SAProcedures::SendScaleInput(long Weight, long ReturnCode, long ReturnCodeExt)
{
  long r;
  trace(L2, _T("+SASendScaleInput: Weight <%d> ReturnCode <%d> ReturnCodeExt <%d>"),
    Weight, ReturnCode, ReturnCodeExt);
  r = (long)sai.SendScaleInput(Weight, ReturnCode, ReturnCodeExt);
  trace(L2, _T("-SASendScaleInput: return <%d>"), r);
  return r;
}

//////////////////////////////////////////////////////
long SAProcedures::TransactionAbortOk(long GrossSales)
{
  long r;
  trace(L2, _T("+SATransactionAbortOK: GrossSale <%d>"), GrossSales);
  r = (long)sai.TransactionAbortOk(GrossSales);
  trace(L2, _T("-SATransactionAbortOK: return <%d>"), r);
  return r;
}

///////////////////////////////////////////////////////////////////////////////
long SAProcedures::CouponRatioOK(long GrossSales, long CouponTotals, long Type)
{
  long r;
  trace(L2, _T("+SACouponRatioOK: GrossSales <%d> CouponTotals <%d> Type <%d>"),
    GrossSales, CouponTotals, Type);
  r = (long)sai.CouponRatioOK(GrossSales, CouponTotals, Type);
  trace(L2, _T("-SACouponRatioOK: return <%d>"), r);
  return r;
}

/////////////////////////////////////////////////////////////
long SAProcedures::ClearCouponTallies(long Amount, long Type)
{
  long r;
  trace(L2, _T("+SAClearCouponTallies: Amount <%d> Type <%d>"), Amount, Type);
  r = (long)sai.ClearCouponTallies(Amount, Type);
  trace(L2, _T("-SAClearCouponTallies: return <%d>"), r);
  return r;
}

/////////////////////////////////////////////////////////////
long SAProcedures::UnloadActions(void)
{
  trace(L6,_T("+UnLoadActions"));
  ActionEntry *a;
  POSITION p;	
  int i;

  for (i=0; i < co.GetNumberOfLanguagesSupported(); i++;)
  {
      if (ActionList[i])
      {
          p = ActionList[i]->GetHeadPosition();
          while (p)
          {
              a = (ActionEntry *)ActionList[i]->GetNext(p);
              delete a;
          }
          ActionList[i]->RemoveAll();
          delete ActionList[i];
          ActionList[i] = NULL;
      }
  }

  /*
  if (ActionList)
  {
    p = ActionList->GetHeadPosition();
		
    while (p)
    {
      a = (ActionEntry *)ActionList->GetNext(p);
      delete a;
    }
    ActionList->RemoveAll();
    delete ActionList;
    ActionList = NULL;
  }

  if (SecondActionList)
  {
    p = SecondActionList->GetHeadPosition();
		
    while (p)
    {
      a = (ActionEntry *)SecondActionList->GetNext(p);
      delete a;
    }
    SecondActionList->RemoveAll();
    delete SecondActionList;
    SecondActionList = NULL;
  }
  */
		
  trace(L6,_T("-UnLoadActions"));
  return 0;
}

/////////////////////////////////////////////////////////////
long SAProcedures::LoadActions(LPCTSTR szFileName, CObList *ActionList)
{
  CString csWork;
  int nConc;
  int i,j;
  ActionEntry *a;
  POSITION p;
	
  trace(L6,_T("+LoadActions"));
  i = 0;
	
  	
  // open the text file SASIActions.TXT file for read only access
  // this INI like file has comments and values
  //
  // [Conclusion Number]
  // SecurityLog=Security log text
  // 1.xBar    =Instruction Bar msg text      UIStyle 1.x
  // 1.xAudio=File name for the audio msg text    UIStyle 1.x
  // 2.xBar=Instruction Bar msg text      UIStyle 2.x
  // 2.xPopup=Popup msg texte (OK)        UIStyle 2.x
  // 2.xAudio=File name for the audio msg text    UIStyle 2.x
	
  // each section represents a securyty conclusion
  // each section has the same key values
  // some key values may be missing and will take on default values or
  // are not used in that section
	
  nConc = 0;											  // current conclusion #
	
  try
  {
    CStdioFile csfIn(szFileName,CFile::modeRead);
    // read a complete line
    // each line is a message
	    		
    while (csfIn.ReadString(csWork))
    {
      csWork.TrimRight();
      if (csWork==_T("")) continue;     // ignore blank lines
      if (*csWork==_T(';')) continue;   // ignore comment lines
      if (*csWork==_T(' ')) continue;   // ignore lines with leading spaces
			
      if (*csWork==_T('['))             // start of new section [ ]
      {
        nConc = _ttoi(csWork.Mid(1));
				
        p = ActionList->GetHeadPosition();
        while (p)
        {
          a = (ActionEntry *)ActionList->GetNext(p);
          if (a->nConclusion==nConc) // duplicate
          {
            nConc = 0;              // ignore this dupliacte
            break;                  // exit for loop
          }
        }
        if (nConc==0) continue;
				
        a = new ActionEntry;
        a->nConclusion = nConc;
        ActionList->AddTail(a);
        i++;
        continue;                   // read next record
      }
			
      if (!nConc) continue;         // no current conclusion
			
      j = csWork.Find(_T('='));         // look for = sign
      if (j<0) continue;
			
      CString csKey = csWork.Left(j);	// get key
      CString csValue = csWork.Mid(j+1); // get value
			
      csKey.MakeUpper();
			
      if (csKey==_T("SECURITYLOG"))
      {
        if (a->szLogText) continue;
        a->szLogText = new _TCHAR[csValue.GetLength()+1];
        _tcscpy(a->szLogText,csValue);
        continue;
      }
			
      if (_istdigit(*csKey))
      {
        int nSec = *csKey-_T('0');
        if (nSec<0 || nSec>=MAX_SECLEVEL) continue;
        if (csKey.Mid(1,2)!=_T(".X")) continue;
				
        csKey = csKey.Mid(3);
				
        if (csKey==_T("BAR"))
        {
          if (a->szBar[nSec]) continue;
          a->szBar[nSec] = new _TCHAR[csValue.GetLength()+1];
          _tcscpy(a->szBar[nSec],csValue);
          continue;
        }
        if (csKey==_T("AUDIO"))
        {
          if (a->szAudio[nSec]) continue;
          a->szAudio[nSec] = new _TCHAR[csValue.GetLength()+1];
          _tcscpy(a->szAudio[nSec],csValue);
          continue;
        }
        if (csKey==_T("POPUP"))
        {
          if (a->szPopup[nSec]) continue;
          a->szPopup[nSec] = new _TCHAR[csValue.GetLength()+1];
          _tcscpy(a->szPopup[nSec],csValue);
          continue;
        }
        if (csKey==_T("TONE"))
        {
          csValue.MakeUpper();
          a->fTone[nSec] = (*csValue==_T('Y')?true:false);
          continue;
        }
      }
			
      trace(L6,_T("Unknown ACTION key %s = %s"),csKey,csValue);
      continue;
    }
		
    csfIn.Close();
  }
  catch( CFileException * e )			  //catch any other exceptionn
  {
    eo.FileError(RESTART,_T("Loading Actions"), *e);
    e->Delete();
    return -1;
  }
	
	
  trace(L6,_T("-LoadActions: # loaded: %d, File name: %s"),i, szFileName);
	
  return i;
}

/////////////////////////////////////////////////////////////
ActionEntry *SAProcedures::GetAction(const int nConclusion,
                                     const nLevel, SCOT_LANGUAGE_TYPE language)
{
  ActionEntry *a = NULL;
  POSITION p;
  CObList *List = NULL;

  List = ActionList[language];
  /*
  if (language == SCOT_LANGUAGE_PRIMARY)
  	  List = ActionList;
  else
  	  List = SecondActionList;
  */

  if (List)
  {
     if (nLevel>=0 && nLevel<MAX_SECLEVEL)
	 {
        p = List->GetHeadPosition();
        while (p)
		{
          a = (ActionEntry *)List->GetNext(p);
          if (a->nConclusion==nConclusion) return a;
		}
	 }
  }
  
  return NULL;
}



////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//
// 'Create' this window (sap) then the (sai) window object,
//   load OLE control and update status.
//
//////////////////////////////////////////
long SAProcedures::Initialize(CWnd * pWnd)
{
  long rc;
  int i;
  trace(L0, _T("+")__TIMESTAMP__);
  trace(L4, _T("+Initialize"));
  if ( SAICreated == false )
  {
    if( SAPCreated == false )
    {
      if(pWnd == NULL)
      {
        if( !(rc = Create( NULL, NULL, WS_DISABLED, CRect(0,0,0,0), AfxGetMainWnd(), IDW_SAPWINDOW )))
        {
          ScotError(RESTART,SM_CAT_STARTUP_INIT, SCOTAPP_SAPROCEDURE_MAIN_WINDOW_FAILED);

        }
      }
      else
      {
        if( !(rc = Create( NULL, NULL, WS_DISABLED, CRect(0,0,0,0), pWnd, IDW_SAPWINDOW )))
        {
          ScotError(RESTART,SM_CAT_STARTUP_INIT, SCOTAPP_SAPROCEDURE_WINDOW_FAILED);
        }
      }
			

      // load both English and the other language if dual language turned on
      _TCHAR szActionFile[_MAX_PATH];

      for (i = 0; i < co.GetNumberOfLanguagesSupported(); i++)
      {
          if (i == 0)
          {
              _tmakepath(szActionFile,SCOTDRIVE,SCOTCONFIG, co.csDataSASIActionFile,_T(""));
          }
          else
          {
              _tmakepath(szActionFile,SCOTDRIVE,SCOTCONFIG, co.Getlanguage(i).csSecMessageFile,_T(""));
          }
          ActionList[i] = new CObList;
          if (LoadActions(szActionFile, ActionList[i])<0)
          {
            ScotError(RESTART,SM_CAT_STARTUP_INIT, SCOTAPP_SAPROCEDURE_LOAD_ACTION,_T("%s"),szActionFile);
   
          }
      }
     
/*
      _tmakepath(szActionFile,SCOTDRIVE,SCOTCONFIG, co.csDataSASIActionFile,_T(""));
      ActionList = new CObList;
      if (LoadActions(szActionFile, ActionList)<0)
      {
        ScotError(RESTART,SM_CAT_STARTUP_INIT, SCOTAPP_SAPROCEDURE_LOAD_ACTION,_T("%s"),szActionFile);
   
      }

      if (co.fStateDualLanguage)
      {
         int nLanguage;
         nLanguage = SCOT_LANGUAGE_SECONDARY;
         _tmakepath(szActionFile,SCOTDRIVE,SCOTCONFIG, co.language[nLanguage].csSecMessageFile,_T(""));
         SecondActionList = new CObList;
         if (LoadActions(szActionFile, SecondActionList)<0)
		 {
           ScotError(RESTART,SM_CAT_STARTUP_INIT, SCOTAPP_SAPROCEDURE_LOAD_ACTION,_T("%s"),szActionFile);
		 }
      }
*/

      SAPCreated = true;
    }
    if( !(rc = sai.Create( NULL, NULL, WS_DISABLED, CRect(0,0,0,0), this, IDW_SAIWINDOW )))
    {
      ScotError(RESTART,SM_CAT_STARTUP_INIT, SCOTAPP_SAINTERFACE_WINDOW_FAILED );

    }
    SAICreated = true;
  }
  // caller of this routine
	if( doSAInit )
	{
		rc = sai.InitSA();
		if(rc <= 0)
		{
			// anything special to be done if control creation or initSA fails
		}
	}
	trace(L4, _T("-Initialize"));
	trace(L0, _T("-")__TIMESTAMP__);
	return rc;
}

/////////////////////////////////
long SAProcedures::UnInitialize()
{
  long rc;
  trace(L2, _T("+UnInitialize"));
	
  UnloadActions();
	
  if( rc = sai.UnInitSA() )
  {
  trace(L2, _T("After UnInitialize -- sai:UnIniSA"));
    ScotError(FATAL,SM_CAT_STOP_ABORT, SCOTAPP_SAPROCEDURE_UNINIT);
  }
  if( !DestroyWindow() )					  // destroy this window
  {
  trace(L2, _T("After UnInitialize -- DestroyWindow"));
    ScotError(FATAL,SM_CAT_STOP_ABORT, SCOTAPP_SAPROCEDURE_WINDOW_DESTROY);
  }
  SAPCreated = false;
  SAICreated = false;
  trace(L2, _T("-UnInitialize"));
  return rc;
}

//////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(SAProcedures, CWnd)
//{{AFX_MSG_MAP(SAProcedures)
// NOTE - the ClassWizard will add and remove mapping macros here.
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SAProcedures message handlers

BOOL SAProcedures::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
  sai.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo); // pass it to the DM interface class
  return CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

// BeginCodeWLDBSA11
// -------------------------------------------------------------------------------------------
// Trace WLDB
// This code will send a message to the Application called TestWDLB
// TestWLDB is just a debug test to control WLDB 

void DisplayError()
{
	LPTSTR lpMsgBuf;
	FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER | 
						FORMAT_MESSAGE_FROM_SYSTEM |     
						FORMAT_MESSAGE_IGNORE_INSERTS,    
						NULL,
						GetLastError(),
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
						(LPTSTR) &lpMsgBuf,    
						0,    
						NULL );
	// Display the string.
	AfxMessageBox(lpMsgBuf, MB_OK, 0 );
	// Free the buffer.
	LocalFree( lpMsgBuf );
}

// BeginCodeWLDBSA11
bool SAProcedures::TraceWLDB(int nConclusion, int nLevel, int nType, int InProcess, int nInputType)
{
	UINT Msg=WM_USER+1;	// message to post  
	WPARAM wParam=0;		// first message parameter
	LPARAM lParam=0;		// second message parameter);
	static HWND hWTestWLDB=0;
	static BOOL RegistryRead=FALSE;
	static BOOL Debug=FALSE;
	static BOOL DebugError=FALSE;

	if (!RegistryRead)
	{
		// Read configuration info from the registry
		LONG     lRC;
		HKEY     hKey;

		lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTSASI"), 0, KEY_QUERY_VALUE, &hKey);
		if (lRC==ERROR_SUCCESS) 
		{ 
			CString  cValue;
			DWORD    dwRegType;
			DWORD    dwDataSize;
			
			#define DEBUGKEY _T("Debug")
			#define DEBUGERROR _T("DebugError")

			lRC = ::RegQueryValueEx(hKey, DEBUGKEY, 0, &dwRegType, NULL, &dwDataSize);
			if (lRC==ERROR_SUCCESS && dwRegType==REG_SZ) 
			{
				LPTSTR pBuffer = cValue.GetBuffer(dwDataSize);
				RegQueryValueEx(hKey, DEBUGKEY, 0, &dwRegType, (LPBYTE)pBuffer, &dwDataSize);
				cValue.MakeUpper();
				if (cValue.Find(_T("YES"))==0) 
				{
					Debug=TRUE;
				}
				cValue.ReleaseBuffer(0);				
			}
			lRC = ::RegQueryValueEx(hKey, DEBUGERROR, 0, &dwRegType, NULL, &dwDataSize);
			if (lRC==ERROR_SUCCESS && dwRegType==REG_SZ) 
			{
				LPTSTR pBuffer = cValue.GetBuffer(dwDataSize);
				RegQueryValueEx(hKey, DEBUGERROR, 0, &dwRegType, (LPBYTE)pBuffer, &dwDataSize);
				cValue.MakeUpper();
				if (cValue.Find(_T("YES"))==0) 
				{
					DebugError=TRUE;
				}
				cValue.ReleaseBuffer(0);				
			}
            RegCloseKey(hKey);
        }
		RegistryRead=TRUE;   
	}

	if (Debug)
	{
		wParam = (nConclusion + 1000) * 1000 + nLevel * 10 + nType;
		lParam = nInputType * 10 + InProcess;

		hWTestWLDB = ::FindWindow( NULL, _T("TestWLDB") );

		if (hWTestWLDB==NULL) return FALSE;

		if ( ::PostMessage(  hWTestWLDB, Msg, wParam, lParam) == 0)
		{
			if (DebugError) DisplayError();
		}
	}							
	return TRUE;	
}
// EndCodeWLDBSA11

//////////////////////////////////
//	Security Maintenance Mode	//
//////////////////////////////////

// Query the SMM Weight Tolerance info	
//////////////////////////////////////////////
bool SAProcedures::QuerySMMTol(LPCTSTR csUPCNumber, long lDepartment, long lTolType, long lPPWU)
{
  bool bRet;
  //TAR#135045 Make trace info easier to read
  trace(L2, _T("+QuerySMMTol UPC:%s, Dpt:%d, TolType:%d, PPWU:%d"), csUPCNumber, lDepartment, lTolType, lPPWU);
  if (!SAICreated)// if not created return error.
    return false;		
  if( !(bRet = sai.QuerySMMTol( csUPCNumber, lDepartment, lTolType, lPPWU  )))
  {
	ScotError(RETURN,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_SAINTERFACE_EXCEPTION, _T("SMM RC:%d"), bRet);
  }
  trace(L2, _T("-QuerySMMTol rc:%d"), bRet);
  return bRet;
}

// Update the SMM Weight Tolerance info	
//////////////////////////////////////////////
bool SAProcedures::UpdateSMMTol( long lAction, LPCTSTR csUPCNumber, long lDepartment,  long lTolType, long lWtExpectation, long lWtSubCk, long lWtTolerance, long lPPWU )
{
  bool bRet;
  //TAR#135045 Make trace info easier to read
  trace(L2, _T("+UpdateSMMTol Action:%d, UPC:%s, Dpt:%d, TolType:%d, WtExp:%d, WtSC:%d, WtTol:%d, PPWU:%d"), lAction, csUPCNumber, lDepartment,  lTolType, lWtExpectation, lWtSubCk, lWtTolerance, lPPWU);
  if (!SAICreated)// if not created return error.
    return false;		
  if( !(bRet = sai.UpdateSMMTol( lAction, csUPCNumber, lDepartment,  lTolType, lWtExpectation, lWtSubCk, lWtTolerance, lPPWU  )))
  {
	ScotError(RETURN,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_SAINTERFACE_EXCEPTION, _T("SMM RC:%d"), bRet);
  }
  trace(L2, _T("-UpdateSMMTol rc:%d"), bRet);
  return bRet;
}

// Query the SMM Weight Tolerance info
/////////////////////////////////////////////////////////////////
bool SAProcedures::QuerySMMWLDBwt( LPCTSTR csUPCNumber, long lDepartment, LPCTSTR csDescription, long lPrice, long lSpecialItem, long lPPWU )
{
  bool bRet;
  //TAR#135045 Make trace info easier to read
  trace(L2, _T("+QuerySMMWLDBwt UPC:%s, Dpt:%d, Desc:%s, Pr:%d, Special:%d, PPWU:%d"), csUPCNumber, lDepartment, csDescription, lPrice, lSpecialItem, lPPWU);
  if (!SAICreated)// if not created return error.
    return false;		
  if( !(bRet = sai.QuerySMMWLDBwt( csUPCNumber, lDepartment, csDescription, lPrice, lSpecialItem, lPPWU )))
  {
	ScotError(RETURN,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_SAINTERFACE_EXCEPTION, _T("SMM RC:%d"), bRet);
  }
  trace(L2, _T("-QuerySMMWLDBwt rc:%d"), bRet);
  return bRet;
}

// Update the SMM Weight Tolerance info
/////////////////////////////////////////////////////////////////
bool SAProcedures::UpdateSMMWLDBwt( long lAction, LPCTSTR csUPCNumber, long lDepartment, long lBagScaleWt, long lApproval, long lTally, long lDate, long lInitialWt, long lPPWU )
{
  bool bRet;
  //TAR#135045 Make trace info easier to read
  trace(L2, _T("+UpdateSMMWLDBwt Action:%d, UPC:%s, Dpt:%d, Wt:%d, Appr:%d, Tally:%d, Date:%d, InitWt:%d, PPWU:%d"), lAction, csUPCNumber, lDepartment, lBagScaleWt, lApproval, lTally, lDate, lInitialWt, lPPWU);
  if (!SAICreated)// if not created return error.
    return false;		
  if( !(bRet = sai.UpdateSMMWLDBwt( lAction, csUPCNumber, lDepartment, lBagScaleWt, lApproval, lTally, lDate, lInitialWt, lPPWU  )))
  {
	ScotError(RETURN,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_SAINTERFACE_EXCEPTION, _T("SMM RC:%d"), bRet);
  }
  trace(L2, _T("-UpdateSMMWLDBwt rc:%d"), bRet);
  return bRet;
}

