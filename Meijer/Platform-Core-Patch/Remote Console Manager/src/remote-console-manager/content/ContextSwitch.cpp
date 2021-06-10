// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/content/ContextSwitch.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ContextSwitch.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
 *
 * Copyright 2004 by NCR Corporation,
 *
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of NCR Corporation. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with NCR.
 */
/*
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/content/ContextSwitch.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     2/10/05 5:33p Dm185016
 * Fixed problem with static ids used in SM.
 * 
 * 1     1/26/05 2:24p Dm185016
 * Moved to new Repository.
// 
// 2     1/12/05 6:12p Dm185016
// Select support
// 
// 1     11/16/04 11:45a Dm185016
*/
#include "StdAfx.h"
#include "ContextSwitch.h"
#include "ContextSwitchHnd.h"
#include "RuleImpl.h"
#include "Message.h"

using namespace STATE_MACHINE_NAMESPACE;
using namespace fastlane;

int CContextSwitch::m_nContextCounter = 0;


CContextSwitch::CContextSwitch(void)
    : CStateObjectProxy(++m_nContextCounter)
    , m_nExitMsgID(-1)
    , m_nEntryMsgID(-1)
    , m_bInputEventsEnabled(true)
{
//  IMODTRACE_DEVELOP();
}


CContextSwitch::~CContextSwitch(void)
{
//  IMODTRACE_DEVELOP();

    try
    {
        for (CRuleQIterator iter = m_qEntryActions.begin();
            iter != m_qEntryActions.end();
            iter++)
        {
            delete *iter;
        }

        for (CRuleQIterator exititer = m_qExitActions.begin();
            exititer != m_qExitActions.end();
            exititer++)
        {
            delete *exititer;
        }

        m_qExitActions.clear();
        m_qEntryActions.clear();
    }
    catch (...)
        { ITRACE_ERROR(_T("Exception caught in CContextSwitch::~CContextSwitch()")); }
}


DEFINE_BOOLEAN_PROPERTY(CContextSwitch, PushInputControlSettings);
DEFINE_BOOLEAN_PROPERTY(CContextSwitch, DisableInputControl);
DEFINE_BOOLEAN_PROPERTY(CContextSwitch, EnableInputControl);
DEFINE_STRING_PROPERTY(CContextSwitch, Name);
DEFINE_STRING_PROPERTY(CContextSwitch, Comment);


void CContextSwitch::AddExitRule(CRuleImplBase *pRule)
{
//  IMODTRACE_DEVELOP();
    m_qExitActions.push_back(pRule);
}


void CContextSwitch::AddEntryRule(CRuleImplBase *pRule)
{
//  IMODTRACE_DEVELOP();
    m_qEntryActions.push_back(pRule);
}


StringType CContextSwitch::GetXMLOutput(CXMLBuilder *pXMLBuilder)
{
    if (pXMLBuilder == NULL)
        ((CXMLBuilder *)CContextSwitchHandler::instance())->GetXMLOutput(this);
    else
        pXMLBuilder->GetXMLOutput(this);

    return (pXMLBuilder == NULL ? CContextSwitchHandler::instance() : pXMLBuilder)->GetXMLOutput();
}


int CContextSwitch::GetId() const
{
    return m_nId;
}

int CContextSwitch::GetExitMsgID() const             { return m_nExitMsgID; }
void CContextSwitch::SetExitMsgID(int nExitMsgID)    { m_nExitMsgID = nExitMsgID; }

int CContextSwitch::GetEntryMsgID() const            { return m_nEntryMsgID; }
void CContextSwitch::SetEntryMsgID(int nEntryMsgID)  { m_nEntryMsgID = nEntryMsgID; }


const _TCHAR *CContextSwitch::GetComment() const
{
    return Comment.Value().c_str();
}


const _TCHAR *CContextSwitch::GetName() const
{
    return Name.Value().c_str();
}

bool CContextSwitch::IsPushInputControlSettings() const { return PushInputControlSettings.IsValid(); }
bool CContextSwitch::IsDisableInputControl() const      { return DisableInputControl.IsValid(); }
bool CContextSwitch::IsEnableInputControl() const       { return EnableInputControl.IsValid(); }


void CContextSwitch::SetInputEventsEnabled(bool lValue) { m_bInputEventsEnabled = lValue; }
bool CContextSwitch::IsInputEventsEnabled() const { return m_bInputEventsEnabled; }
