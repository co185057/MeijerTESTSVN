//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMUSSFManager.CPP 
//
// TITLE: Class implementation for this state
//
// AUTHOR:	Christopher Swift
//  
//            
//////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"                 // MS MFC always needed first
#include <afxtempl.h>                     // MS MFC template header
#include <afxpriv.h>                // for USES_CONVERSION

#include "Common.h"                       // MGV common includes
#include "PSIntList.h"                    // MGV common includes
#include "SMUSSFManager.h"    // MGV this state
#include "SMCardMisRead.h"
#include "SMFinish.h"
#include "SMTakeCard.h"
#include "USSFCommon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
#endif


#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SMUSSFManager")

IMPLEMENT_DYNCREATE(SMUSSFManager, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
// CONTEXTS THAT USE THIS STATE:
//
//  SMUSSFManager...
//
//////////////////////////////////////////
SMUSSFManager::SMUSSFManager()
{
  IMPLEMENT_TIMESTAMP
}

/////////////////////////////////////////////////
SMStateBase *SMUSSFManager::Initialize(void)
{
  trace(L7, _T("SMUSSFManager::Initialize(void)"));
  CString tempstr = _T("");
  long rc = 0;

  nTimeOut = 30;
  
  SMStateBase *returnState = SMUSSFManagerBase::Initialize();
  if (returnState == STATE_NULL)
  {
    // Context Dependent Stuff

	CString currentContext = GetDisplayContext();
    returnState = ShowDetails(currentContext);
    if (returnState == STATE_NULL)
    {
      returnState = ShowContext();
    }
  }
  
  return returnState;
}

SMStateBase *SMUSSFManager::TBIdleTimer(void)
{
  return SMUSSFManagerBase::TBIdleTimer();
}

SMStateBase *SMUSSFManager::MakeNextMove(CString altContext)
{
  trace(L7,_T("SMUSSFManager::MakeNextMove(CString altContext)"));

  //bool rc = TBStateIsReadyToSell();
  //if (rc == true)
  //{
	//  SetTransactionVariable(_T("GA_ITEM"), _T("TRUE"));
  //}

  return SMUSSFManagerBase::MakeNextMove(altContext);
}

SMStateBase *SMUSSFManager::PerformAction(CString thisAction, bool &bActionTaken)
{
  trace(L7,_T("SMUSSFManager::PerformAction(CString thisAction)"));

  trace(L6, _T("*** PERFORMING ACTION=<%s>."), thisAction);
  bActionTaken = false;

  CString csTraceStr;
  SMStateBase *returnState = STATE_NULL;

  try
  {
	USSF_LOCAL_ACTION(_T("SAMPLE"), FullReset);
  }
  catch(...)
  {
	trace(L7, _T("Exception Within Customer Action(%s)."), thisAction);
  }

  if (bActionTaken == false)
  {
	  return SMUSSFManagerBase::PerformAction(thisAction, bActionTaken);
  }

  return returnState;
}