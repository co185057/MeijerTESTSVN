//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSecurityLevel2.cpp
//
// TITLE: Class header for Level 3 Security agent State.
//
//
// AUTHOR:  DcL-
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>								  // MS MFC always needed first
#include <afxtempl.h>							  // MS MFC template header

#include "Common.h"								  // MGV common includes

#include "SMState.h"							  // MGV base state
#include "SMSecurityLevel2.h"			  // MGV this

#define COMP_ID ID_SM							  // base state component
#define T_ID _T("SecurityLevel2")

IMPLEMENT_DYNCREATE(SMSecurityLevel2, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////////////////////////////
SMSecurityLevel2::SMSecurityLevel2( CString csBar, CString csPop )
: SMSecurityLevel2Base(csBar, csPop)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMSecurityLevel2::SMSecurityLevel2()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMSecurityLevel2::Deliver( CString csBar, CString csPop )
{
  SMSecurityLevel2Base::Deliver(csBar, csPop);

  return this;
}


