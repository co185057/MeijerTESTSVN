//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSecurityLevel3.cpp
//
// TITLE: Implementation File for Level 3 Security agent State.
//
// Notes: This state is used for manager's override confirmation only
//
//
// AUTHOR:  DcL-
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>								  // MS MFC always needed first

#include "Common.h"								  // MGV common includes

#include "SMState.h"							  // MGV base state
#include "SMSecurityLevel3.h"       // MGV security level msg 3 state

#define COMP_ID ID_SM							  // base state component
#define T_ID _T("SecurityLevel3")

IMPLEMENT_DYNCREATE(SMSecurityLevel3, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////////////////////////////
SMSecurityLevel3::SMSecurityLevel3( CString csBar, CString csPop )
: SMSecurityLevel3Base(csBar, csPop)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMSecurityLevel3::SMSecurityLevel3()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMSecurityLevel3::Deliver( CString csBar, CString csPop )
{
  SMSecurityLevel3Base::Deliver(csBar, csPop);

  return this;
}

SMStateBase *SMSecurityLevel3::Initialize(void)
{
    return SMSecurityLevel3Base::Initialize();
}

void SMSecurityLevel3::UnInitialize(void)
{
    SMSecurityLevel3Base::UnInitialize();
}

SMStateBase *SMSecurityLevel3::OnWtIncreaseNotAllowed(void)
{
    return SMSecurityLevel3Base::OnWtIncreaseNotAllowed();
}

SMStateBase *SMSecurityLevel3::OnWtDecrease(void)
{
    return SMSecurityLevel3Base::OnWtDecrease();
}

bool SMSecurityLevel3::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMSecurityLevel3Base::DeliverDescriptorList(dlParameterList);
}

SMStateBase *SMSecurityLevel3::PSButton3(void)
{
    return SMSecurityLevel3Base::PSButton3();
}

SMStateBase *SMSecurityLevel3::PSCancelKey(void)
{
    return SMSecurityLevel3Base::PSCancelKey();
}

void SMSecurityLevel3::showScreen(bool passwordVerified)
{
    SMSecurityLevel3Base::showScreen(passwordVerified);
}

SMStateBase *SMSecurityLevel3::stateAfterPasswordVerify()
{
    return SMSecurityLevel3Base::stateAfterPasswordVerify();
}

SMStateBase *SMSecurityLevel3::PSButtonGoBack(void)
{
    return SMSecurityLevel3Base::PSButtonGoBack();
}
