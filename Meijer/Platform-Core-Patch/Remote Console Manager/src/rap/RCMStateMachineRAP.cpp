// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/RCMStateMachineRAP.cpp 3     1/12/10 11:58p Cu185008 $
/*
 * @(#)RCMStateMachineRAP.cpp      $Revision: 3 $  $Date: 1/12/10 11:58p $
 *
 * Copyright 2005, NCR Corporation.
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/RCMStateMachineRAP.cpp $
 * 
 * 3     1/12/10 11:58p Cu185008
 * G2 Migration.
 * 
 * 8     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 4     3/22/05 3:47p Dm185016
 * UNICODE
 * 
 * 3     3/14/05 8:27p Dm185016
 * 
 * 2     3/11/05 1:35p Dm185016
 * <select> support
 * 
 * 1     3/01/05 2:04p Dm185016
 * TAR 294587.  Added application state functionality.
*/

/*lint -e766 */         // Lint keeps complaining about SMTimerManager

#include "StdAfx.h"
#include "RCMStateMachineRAP.h"
#include "statem.h"
#include "RCMErrorRAP.h"
#include "RCMLoaderRAP.h"
//#include "RCMConstants.h"
//#include "RCMErrorHandler.h"
#include "MsgImpl.h"
#include "StateImpl.h"
#include "AppState.h"
#include "Loggable.h"

using namespace rap;
using namespace STATE_MACHINE_NAMESPACE;


/**
* constructor
*/
 CRCMStateMachineRAP::CRCMStateMachineRAP(const TCHAR *pFn, HTRACE hTrace)
     : CRCMStateMachine(pFn, hTrace)
	 , m_pRCMLoader(NULL)
	 , m_pAppStates(NULL)
     , m_nMaxAppStateID(0)
{
    IMODTRACE_DEVELOP();
}


CRCMStateMachineRAP::~CRCMStateMachineRAP()
{
    IMODTRACE_DEVELOP();

    try
    {
        CApplicationStatesMap::iterator iter = m_appstates.begin();
        for (;iter != m_appstates.end(); iter ++)
        {
            delete iter->second;
        }
        delete [] m_pAppStates;
    }
    catch (...)
        { ITRACE_DOT_ERROR(); }

    m_pRCMLoader    = NULL;
}


const utils::IErrorInfo &CRCMStateMachineRAP::GetErrorInfo(HRESULT hr) 
{
    CRCMErrorRAPInfo *pErrorInfo = new CRCMErrorRAPInfo(hr);

    SetErrorInfo(pErrorInfo);

    return *pErrorInfo; 
}


const TCHAR *CRCMStateMachineRAP::GetIdentifier() const
{
    return _T("RCMStateMachineRAP");
}


CSMLoader *CRCMStateMachineRAP::GetLoader()
{
	if (m_pRCMLoader == NULL)
		m_pRCMLoader = new CRCMLoaderRAP(GetTraceHandle());

    return m_pRCMLoader;
}


void CRCMStateMachineRAP::ReleaseLoader()
{
	IMODTRACE_DEVELOP();

    delete m_pRCMLoader;
	m_pRCMLoader = NULL;
}


HRESULT CRCMStateMachineRAP::GetAppState(rap::CApplicationState **ppState, const _TCHAR *pszStateName) const
{
    *ppState = NULL;

    CApplicationStatesMap::const_iterator iter = m_appstates.find(pszStateName);
    if (iter != m_appstates.end())
        *ppState = iter->second;

    return *ppState != NULL ? S_OK : E_FAIL;
}


HRESULT CRCMStateMachineRAP::Add(CApplicationState *pAppstate)
{
    _ASSERTE(pAppstate!=NULL);
    
    if (loggable().IsDebug()) 
        ITRACE_DEVELOP(_T("Adding CApplicationState:  ") 
                      + IString(pAppstate->GetName()) 
                      + (pAppstate->IsAssistModeEnabled() ? _T("<enable-assist-mode/>") : _T("")) 
                      + (pAppstate->IsAssistModeDisabled() ? _T("<disable-assist-mode/>") : _T("")) 
                      + (pAppstate->IsPushAssistModeEnabled() ? _T("<push-assist-mode/>") : _T("")) 
                      + _T("(") + IString(pAppstate->GetId()) + _T(")"));

    CApplicationStatesMap::iterator iter = m_appstates.find(pAppstate->GetName());
    if (iter != m_appstates.end())
        return RAP_DUPLICATE_APPLICATION_STATE_ID;

    //lint -e{534} Don't care about return
    m_appstates.insert(std::make_pair(pAppstate->GetName(), pAppstate));  

    HRESULT hr = AddExitList(*pAppstate);

    if (hr == S_OK)
        hr = AddEntryList(*pAppstate);

    return hr;
}


HRESULT CRCMStateMachineRAP::AddExitList(CApplicationState &appstate)
{
    StringType sName(appstate.GetName());

    CRuleQ *pExitList = appstate.GetExitList();
    if (pExitList->empty())
        return S_OK;

    CStateImpl &state = GetApplicationAnyState();

    IRule *pRule;
    CRuleImpl *pRuleProxy;
    _ASSERTE( _CrtCheckMemory() );
 
    // Create the exit message
    sName += MSG_EXIT_SUFFIX;
    CMsgImpl *pMsg = dynamic_cast<CMsgImpl *>(CreateNextMessage());
    pMsg->Name.Value(sName);
    HRESULT hr = CXMLStateMachine::Add(pMsg);
    VERIFY(hr==S_OK);

    int nMsgID = GetMsgID(sName.c_str());

    appstate.SetExitMsgID(nMsgID);

    CRuleQ &exitList = *pExitList;
    for (CRuleQIterator iter = exitList.begin();
         iter != exitList.end();
         iter++)
    {
        pRuleProxy = dynamic_cast<CRuleImpl *>(*iter);
        pRule = CreateRule(pRuleProxy);
        state.AddMsgRule(nMsgID, pRule, pRuleProxy->GetRuleType());
    }

    return S_OK;
}


HRESULT CRCMStateMachineRAP::AddEntryList(CApplicationState &appstate)
{
    StringType sName(appstate.GetName());

    CRuleQ *pEntryList = appstate.GetEntryList();
    if (pEntryList->empty())
        return S_OK;

    CStateImpl &state = GetApplicationAnyState();

    IRule *pRule;
    CRuleImpl *pRuleProxy;
    _ASSERTE( _CrtCheckMemory() );

    // Create the entry message
    sName += MSG_ENTRY_SUFFIX;
    CMsgImpl *pMsg = dynamic_cast<CMsgImpl *>(CreateNextMessage());
    pMsg->Name.Value(sName);
    HRESULT hr = CXMLStateMachine::Add(pMsg);
    VERIFY(hr==S_OK);

    int nMsgID = GetMsgID(sName.c_str());

    appstate.SetEntryMsgID(nMsgID);

    CRuleQ &entryList = *pEntryList;
    for (CRuleQIterator iter = entryList.begin();
         iter != entryList.end();
         iter++)
    {
        pRuleProxy = dynamic_cast<CRuleImpl *>(*iter);
        pRule = CreateRule(pRuleProxy);
        state.AddMsgRule(nMsgID, pRule, pRuleProxy->GetRuleType());
    }

    return S_OK;
}
