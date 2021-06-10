//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMWaitForRemoteAssistance.CPP
//
// TITLE: Class implementation for RAP Data Needed state
//
//
// AUTHOR:    Erick Kobres
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMWaitForRemoteAssistance.h"      // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID _T("WaitForRemoteAssistance")

IMPLEMENT_DYNCREATE(SMWaitForRemoteAssistance, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////////////
SMWaitForRemoteAssistance::SMWaitForRemoteAssistance()
{
  IMPLEMENT_TIMESTAMP
}

SMWaitForRemoteAssistance::SMWaitForRemoteAssistance( LPCTSTR sMachineName, LPCTSTR sRemoteContext ) :
	SMWaitForRemoteAssistanceBase( sMachineName, sRemoteContext )
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase  *SMWaitForRemoteAssistance::Deliver( LPCTSTR sMachineName, LPCTSTR sRemoteContext )
{
  SMWaitForRemoteAssistanceBase::Deliver( sMachineName, sRemoteContext );

  return this;
}


