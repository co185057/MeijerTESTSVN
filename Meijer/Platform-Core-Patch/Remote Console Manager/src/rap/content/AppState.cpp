// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/content/AppState.cpp 3     10/21/08 4:06p Sm133076 $
/*
 * @(#)ApplicationState.cpp    $Revision: 3 $ $Date: 10/21/08 4:06p $
 *
 * Copyright 2005 by NCR Corporation,
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/content/AppState.cpp $
 * 
 * 3     10/21/08 4:06p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     3/01/05 2:05p Dm185016
 * TAR 294587.  Added application state functionality.
*/
#include "StdAfx.h"
#include "AppState.h"
#include "AppStateHnd.h"
#include "Message.h"

using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;

int CApplicationState::m_nAppCounter = 0;


CApplicationState::CApplicationState(void)
    : CStateObjectProxy(++m_nAppCounter)
    , m_bSavedAssistModeEnabled(false)
    , m_nExitMsgID(-1)
    , m_nEntryMsgID(-1)
{
//  IMODTRACE_DEVELOP();
}


CApplicationState::~CApplicationState(void)
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
        { ITRACE_ERROR(_T("Exception caught in CApplicationState::~CApplicationState()")); }
}


DEFINE_BOOLEAN_PROPERTY(CApplicationState, AssistModeEnabled);
DEFINE_BOOLEAN_PROPERTY(CApplicationState, PushAssistModeEnabled);
DEFINE_BOOLEAN_PROPERTY(CApplicationState, AssistModeDisabled);
DEFINE_STRING_PROPERTY(CApplicationState, Name);
DEFINE_STRING_PROPERTY(CApplicationState, Comment);


void CApplicationState::AddExitRule(CRuleImpl *pRule)
{
//  IMODTRACE_DEVELOP();
    m_qExitActions.push_back(pRule);
}


void CApplicationState::AddEntryRule(CRuleImpl *pRule)
{
//  IMODTRACE_DEVELOP();
    m_qEntryActions.push_back(pRule);
}


StringType CApplicationState::GetXMLOutput(CXMLBuilder *pXMLBuilder)
{
    if (pXMLBuilder == NULL)
        ((CXMLBuilder *)CApplicationStateHandler::instance())->GetXMLOutput(this);
    else
        pXMLBuilder->GetXMLOutput(this);

    return (pXMLBuilder == NULL ? CApplicationStateHandler::instance() : pXMLBuilder)->GetXMLOutput();
}


int CApplicationState::GetId() const
{
    return m_nId;
}

int CApplicationState::GetExitMsgID() const             { return m_nExitMsgID; }
void CApplicationState::SetExitMsgID(int nExitMsgID)    { m_nExitMsgID = nExitMsgID; }

int CApplicationState::GetEntryMsgID() const            { return m_nEntryMsgID; }
void CApplicationState::SetEntryMsgID(int nEntryMsgID)  { m_nEntryMsgID = nEntryMsgID; }


const _TCHAR *CApplicationState::GetComment() const
{
    return Comment.Value().c_str();
}


const _TCHAR *CApplicationState::GetName() const
{
    return Name.Value().c_str();
}


void CApplicationState::SetSavedAssistModeEnabled(bool bAssistModeEnabled) { m_bSavedAssistModeEnabled = bAssistModeEnabled; }
bool CApplicationState::IsSavedAssistModeEnabled() const { return m_bSavedAssistModeEnabled; }

bool CApplicationState::IsAssistModeEnabled() const            {   return AssistModeEnabled.IsValid(); }
bool CApplicationState::IsAssistModeDisabled() const           {   return AssistModeDisabled.IsValid(); }
bool CApplicationState::IsPushAssistModeEnabled() const        {   return PushAssistModeEnabled.IsValid(); }
