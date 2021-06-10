//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmSecurityLogs.CPP
//
// TITLE: Class implementation for store mode Coupon Items state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMSmSecurityLogs.h"       // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmSecurityLogs")

IMPLEMENT_DYNCREATE(SMSmSecurityLogs, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////////
SMSmSecurityLogs::SMSmSecurityLogs()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSmSecurityLogs::Initialize(void)
{
    return SMSmSecurityLogsBase::Initialize();
}

void SMSmSecurityLogs::UnInitialize(void)
{
    SMSmSecurityLogsBase::UnInitialize();
}

SMStateBase * SMSmSecurityLogs::PSButton1(void)
{
    return SMSmSecurityLogsBase::PSButton1();
}

SMStateBase * SMSmSecurityLogs::PSButton8(void)
{
    return SMSmSecurityLogsBase::PSButton8();
}

/*sscoadk-5431
SMStateBase * SMSmSecurityLogs::RAParse(MessageElement* me)
{
    return SMSmSecurityLogsBase::RAParse(me);
}
*/

