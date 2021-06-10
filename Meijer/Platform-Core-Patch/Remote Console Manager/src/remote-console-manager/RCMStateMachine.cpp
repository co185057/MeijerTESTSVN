// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/remote-console-manager/RCMStateMachine.cpp 4     1/25/10 8:21p Cu185008 $
/*
 * @(#)RCMStateMachine.cpp      $Revision: 4 $  $Date: 1/25/10 8:21p $
 *
 * Copyright 2004, NCR Corporation.
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/remote-console-manager/RCMStateMachine.cpp $
 * 
 * 4     1/25/10 8:21p Cu185008
 * G2 Migration.
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 15    5/30/08 10:25a Bc185047
 * RCM Patch G Issue – Eliminate RCM’s dependence on XML format
 * 
 * 14    5/27/08 10:50a Bc185047
 * RCM Patch G Issue – Eliminate RCM’s dependence on XML format
 * 
 * 13    3/07/08 1:31p Bc185047
 * New custom data handling. RCM was parsing through LaneRAP.xml to
 * retrieve the View and DefaultButton values. This stopped working after
 * the new XML schema took effect.
 * 
 * After this change, we are now setting the View and DefaultButton values
 * by using the PSX API.
 * 
 * 12    2/15/08 10:50a Cs185088
 * TAR369684 - RCM not releasing all action
 * 
 * 11    6/27/05 2:10p Dm185016
 * Fixed _non_rtti_object exception being thrown when attempting to use
 * the last action defined in the XML.
 * 
 * 8     3/15/05 3:36p Dm185016
 * UNICODE
 * 
 * 7     3/14/05 8:27p Dm185016
 * 
 * 6     3/11/05 1:35p Dm185016
 * <select> support
 * 
 * 5     2/28/05 4:22p Dm185016
 * lint
 * 
 * 4     2/21/05 8:09p Dm185016
 * Added debugging information to new and delete for pooled objects.
 * 
 * 3     2/10/05 5:30p Dm185016
 * TAR 292915
 * 
 * 2     2/08/05 3:36p Dm185016
 * Removed trace.
 * 
 * 1     1/26/05 2:21p Dm185016
 * Moved to new Repository.
// 
// 6     1/25/05 3:46p Dm185016
// TAR 287899 - Memory leak during termination.
// 
// 5     1/18/05 3:47p Dm185016
// UNICODE support
// 
// 4     12/22/04 11:05a Dm185016
// 
// 3     11/16/04 11:33a Dm185016
// Context switch support
// 
// 2     10/29/04 8:59a Dm185016
// Added LaneRAP.xml processing
// 
// 1     10/04/04 5:36p Dm185016
// New for 3.2 Patch E/4.0
*/

/*lint -e766 */         // Lint keeps complaining about SMTimerManager

#include "StdAfx.h"
#include "RCMStateMachine.h"
#include "SMTimerManager.h"
#include "statem.h"
#include "SMLoader.h"
#include "RCMError.h"
#include "RCMLoader.h"
//#include "RCMConstants.h"
//#include "RCMErrorHandler.h"
#include "State.h"
#include "StateImpl.h"
#include "RuleImpl.h"
#include "MsgImpl.h"
#include "Loggable.h"
#include "verifypsx.h"
#include "PSXMfc.h"

namespace fastlane {}
using namespace fastlane;
using namespace STATE_MACHINE_NAMESPACE;


ILoggable &CRCMStateMachine::loggable() 
{ 
    if (pLoggable == NULL)
        pLoggable = &Trace::getLoggable(GetTraceHandle(), _T("StateMachine"));

    return *pLoggable; 
}


/**
* constructor
*/
 CRCMStateMachine::CRCMStateMachine(const TCHAR *pFn, HTRACE hTrace)
     : CXMLStateMachine(pFn, hTrace)
	 , m_pRCMLoader(NULL)
     , m_pRCMActions(NULL)
	 , m_pContexts(NULL)
     , pLoggable(NULL)
     , m_nMaxContextID(0)
{
    CBaseObject::SetTraceObject(hTrace);

	// BC185047 - New XML data handling by using PSX methods
	HRESULT hr;
	hr = AddControlsAndContexts();	
}


CRCMStateMachine::~CRCMStateMachine()
{
    IMODTRACE_DEVELOP();

    try
    {
        CRCMStateMachine::Stop();

        // Delete all instances of CControlImpl
        CControlsMap::iterator controlsIter;
        for (controlsIter = m_controls.begin();
            controlsIter != m_controls.end();
            controlsIter++)
        {
            delete controlsIter->second;
        }

        // Delete all instances of Contexts
        CContextsMap::iterator iter;
        for (iter = m_contexts.begin();
            iter != m_contexts.end();
            iter++)
        {
            delete iter->second;
        }
        delete [] m_pContexts;

        // Delete all instances of Context Switches
        CContextSwitchMap::iterator contextSwitchIter;
        for (contextSwitchIter = m_contextSwitches.begin();
            contextSwitchIter != m_contextSwitches.end();
            contextSwitchIter++)
        {
            delete contextSwitchIter->second;
        }

        //CApplicationStatesMap::iterator iter = appstates.begin();
        //for (;iter != appstates.end(); iter ++)
        //{
        //    delete iter->second;
        //}

        // At this point all actions should have been released.
        delete m_pRCMActions;
    }
    catch (...)
        { ITRACE_DOT_ERROR(); }

    m_pRCMLoader    = NULL;
    pLoggable       = NULL;
}


void CRCMStateMachine::UnregisterAll()
{
    IMODTRACE_DEVELOP();

    _ASSERTE(m_pRCMActions!=NULL);
    IAction *pAction;
    HRESULT hr;
    for (unsigned i=0; i<=m_nMaxActionID; i++)
    {
        pAction = m_pRCMActions[i];

        m_pRCMActions[i] = NULL;

        try
        {

            // Action was not used yet so ignore this slot.
            if (pAction == NULL)
            {
                ITRACE_DEVELOP(_T("Ignoring action slot:  ") + IString(i));
            }

            // Do not release internal actions
            //else if (pAction->IsInternal())
            //{
            //    ITRACE_DEVELOP("Skipping action release:  " + IString(pAction->GetName()));
            //}

            // All other actions are fair game
            else 
            {
                ITRACE_DEVELOP(_T("Releasing action:  ") + IString(pAction->GetName()));

                hr = CXMLStateMachine::ReleaseAction(pAction) ;
                if (hr != S_OK)
                {
                    CRCMErrorInfo errorInfo(hr);
                    ITRACE_ERROR(errorInfo.text());
                }
            }
        }
        catch (...)
        { ITRACE_DOT_ERROR(); }
    }

    CXMLStateMachine::UnregisterAll();
}

void CRCMStateMachine::Init()
{
    IMODTRACE_DEVELOP();

    CXMLStateMachine::Init();

    _ASSERTE( _CrtCheckMemory() );
    InitContexts();
    _ASSERTE( _CrtCheckMemory() );
}


void CRCMStateMachine::InitContexts()
{
    const unsigned maxID = m_nMaxContextID+1;
    m_pContexts  = new PCONTEXT[maxID];
    memset(m_pContexts, 0, sizeof(PCONTEXT)*maxID);

    CContextsMap::iterator iter;
    PCONTEXT pContext;
    for (iter = m_contexts.begin();
         iter != m_contexts.end();
         iter++)
    {
        pContext = iter->second;

        if (pContext->HasView())
        {
            int nIndex = pContext->GetView();
            m_pContexts[nIndex] = pContext;
        }
    }
}


void CRCMStateMachine::Start()
{
    GetTimerMgr().SetTraceObject(GetTraceHandle());

    CXMLStateMachine::Start();
}


const utils::IErrorInfo &CRCMStateMachine::GetErrorInfo(HRESULT hr) 
{
    CRCMErrorInfo *pErrorInfo = new CRCMErrorInfo(hr);

    SetErrorInfo(pErrorInfo);

    return *pErrorInfo; 
}


void CRCMStateMachine::Stop()
{
    CAbstractStateMachine::Stop();
}


const TCHAR *CRCMStateMachine::GetIdentifier() const
{
    return _T("RCMStateMachine");
}


void CRCMStateMachine::InitActions()
{
    CXMLStateMachine::InitActions();

    m_pRCMActions = new PACTION[m_nMaxActionID+1];

    memset(m_pRCMActions, 0, sizeof(IAction*) * (m_nMaxActionID+1));
}


#pragma warning( push )
#pragma warning( disable : 4311 )
HRESULT CRCMStateMachine::GetAction(IAction **ppAction, int _actionID)
{
    HRESULT retValue = S_OK;

    _ASSERTE(m_pRCMActions!=NULL);

    *ppAction = NULL;

    if (!((_actionID >= 0) && (_actionID <= (int)m_nMaxActionID)))
        return RCM_INVALID_ACTION_ID;

    *ppAction = m_pRCMActions[_actionID];
    if (*ppAction == NULL)
    {
        retValue = CXMLStateMachine::GetAction(ppAction, _actionID) ;

        m_pRCMActions[_actionID] = *ppAction;

        ITRACE_DEVELOP(_T(__FUNCTION__) _T("  _actionID=") + IString(_actionID) + 
                       _T(".  name=") + IString((*ppAction)->GetName()) +
                       _T(".  address=") + IString((long)(*ppAction)->GetName()).d2x());
    }
        
    return retValue;
}
#pragma warning( pop )


HRESULT CRCMStateMachine::ReleaseAction(IAction *)
{
    return S_OK;
}


CSMLoader *CRCMStateMachine::GetLoader()
{
	if (m_pRCMLoader == NULL)
		m_pRCMLoader = new CRCMLoader(GetTraceHandle());

    return m_pRCMLoader;
}


void CRCMStateMachine::ReleaseLoader()
{
	IMODTRACE_DEVELOP();

    delete m_pRCMLoader;
	m_pRCMLoader = NULL;
}
//
//
//HRESULT CRCMStateMachine::GetAppState(fastlane::CApplicationState **ppState, const _TCHAR *pszStateName) const
//{
//    *ppState = NULL;
//
//    CApplicationStatesMap::const_iterator iter = appstates.find(pszStateName);
//    if (iter != appstates.end())
//        *ppState = iter->second;
//
//    return *ppState != NULL ? S_OK : E_FAIL;
//}
//
//


HRESULT CRCMStateMachine::Add(CControlImpl *pControl)
{
    HRESULT hr = S_OK;

    _ASSERTE(pControl!=NULL);
    
    if (loggable().IsDebug()) 
        ITRACE_DEVELOP(_T("Adding CControl:  ") 
                      + IString(pControl->GetName()) 
                      + _T("(") + IString(pControl->GetId()) + _T(") ")
                      + IString(pControl->GetType()) );

    const _TCHAR *pName = pControl->GetName();

    CControlsMap::iterator iter = m_controls.find(pName);
    if (iter != m_controls.end())
        return RCM_DUPLICATE_CONTROL_NAME;

    m_controls.insert(std::make_pair(pName, pControl));

    IMessage *pControlMsg;
    if (S_OK == (hr = ConstructMessage(&pControlMsg, pName)))
        return CXMLStateMachine::Add(pControlMsg);

    return hr;
}


HRESULT CRCMStateMachine::Add(CContextImpl *pContext)
{
    HRESULT hr = S_OK;

    _ASSERTE(pContext!=NULL);
    
    if (loggable().IsDebug()) 
    {
        ITRACE_DEVELOP(_T("Adding CContext:  ") 
                     + IString(pContext->GetName()));
    }

	const _TCHAR *pName = pContext->GetName();

	// BC185047 - New custom data handling. Pass the context name and variables to be set.
	GetCustomData(pName, sDataNeededViewValue, sDefaultButtonValue);

	// BC185047 - New custom data handling. Only modify the pContext attributes if the returned
	// variables are not empty. Also trace.
	if (!sDataNeededViewValue.empty())
	{
		pContext->View.Value(_ttoi(sDataNeededViewValue.c_str()));
		ITRACE_DEVELOP(_T("Adding View: \"") + IString(sDataNeededViewValue.c_str())
			+ _T("\" to CContext: \"" + IString(pName) + _T("\"")));
	}
	if (!sDefaultButtonValue.empty())
	{
		pContext->DefaultButton.Value(sDefaultButtonValue.c_str());
		ITRACE_DEVELOP(_T("Adding DefaultButton: \"") + IString(sDefaultButtonValue.c_str())
			+ _T("\" to CContext: \"" + IString(pName) + _T("\"")));
	}

    CContextsMap::iterator iter = m_contexts.find(pName);
    if (iter != m_contexts.end())
        return RCM_DUPLICATE_CONTEXT_NAME;

	m_contexts.insert(std::make_pair(pName, pContext));
    if (pContext->HasView())
        m_nMaxContextID = max(m_nMaxContextID, pContext->GetView());

    return hr;
}


HRESULT CRCMStateMachine::Add(CContextSwitch *pContextSwitch)
{
    HRESULT hr = S_OK;

    _ASSERTE(pContextSwitch!=NULL);
    
    if (loggable().IsDebug()) 
    {
        ITRACE_DEVELOP(_T("Adding ContextSwitch:  ") 
                      + IString(pContextSwitch->GetName()));
    }

    const _TCHAR *pName = pContextSwitch->GetName();

    CContextSwitchMap::iterator iter = m_contextSwitches.find(pName);
    if (iter != m_contextSwitches.end())
        return RCM_DUPLICATE_CONTEXT_NAME;

    m_contextSwitches.insert(std::make_pair(pName, pContextSwitch));

    hr = AddExitList(*pContextSwitch);

    if (hr == S_OK)
        hr = AddEntryList(*pContextSwitch);

    return hr;
}


HRESULT CRCMStateMachine::AddExitList(CContextSwitch &cswitch)
{
    StringType sName(cswitch.GetName());

    CRuleQ *pExitList = cswitch.GetExitList();
    if (pExitList->empty())
        return S_OK;

    CStateImpl &state = GetApplicationAnyState();

    IRule *pRule;
    CRuleImplBase *pRuleProxy;
    _ASSERTE( _CrtCheckMemory() );
 
    // Create the exit message
    sName += MSG_EXIT_SUFFIX;
    CMsgImpl *pMsg = dynamic_cast<CMsgImpl *>(CreateNextMessage());
    pMsg->Name.Value(sName);
    HRESULT hr = CXMLStateMachine::Add(pMsg);
    VERIFY(hr==S_OK);

    int nMsgID = GetMsgID(sName.c_str());

    cswitch.SetExitMsgID(nMsgID);

    CRuleQ &exitList = *pExitList;
    for (CRuleQIterator iter = exitList.begin();
         iter != exitList.end();
         iter++)
    {
        pRuleProxy = *iter;
        pRule = CreateRule(pRuleProxy);
        state.AddMsgRule(nMsgID, pRule, pRuleProxy->GetRuleType());
    }

    return S_OK;
}


HRESULT CRCMStateMachine::AddEntryList(CContextSwitch &cswitch)
{
    StringType sName(cswitch.GetName());

    CRuleQ *pEntryList = cswitch.GetEntryList();
    if (pEntryList->empty())
        return S_OK;

    CStateImpl &state = GetApplicationAnyState();

    IRule *pRule;
    CRuleImplBase *pRuleProxy;
    _ASSERTE( _CrtCheckMemory() );

    // Create the entry message
    sName += MSG_ENTRY_SUFFIX;
    CMsgImpl *pMsg = dynamic_cast<CMsgImpl *>(CreateNextMessage());
    pMsg->Name.Value(sName);
    HRESULT hr = CXMLStateMachine::Add(pMsg);
    VERIFY(hr==S_OK);

    int nMsgID = GetMsgID(sName.c_str());

    cswitch.SetEntryMsgID(nMsgID);

    CRuleQ &entryList = *pEntryList;
    for (CRuleQIterator iter = entryList.begin();
         iter != entryList.end();
         iter++)
    {
        pRuleProxy = *iter;
        pRule = CreateRule(pRuleProxy);
        state.AddMsgRule(nMsgID, pRule, pRuleProxy->GetRuleType());
    }

    return S_OK;
}


CStateImpl &CRCMStateMachine::GetApplicationAnyState()
{
    CStateImpl *pState = dynamic_cast<CStateImpl*>(GetAnyState());
    
    _ASSERTE(pState != NULL);

    return *pState;
}


bool CRCMStateMachine::IsControl(const _TCHAR *pszControlName) const
{
    CControlsMap::const_iterator iter = m_controls.find(pszControlName);

    return (iter != m_controls.end());
}

// RFC 393276 - Check if context is declared under RCMgr.xml's context switch list.
bool CRCMStateMachine::IsValidContextSwitch(const _TCHAR *pszContextSwitchName) const
{
  CContextSwitchMap::const_iterator iter = m_contextSwitches.find(pszContextSwitchName);

  return (iter != m_contextSwitches.end());
}


HRESULT CRCMStateMachine::GetViewName(StringType &m_sViewName, int m_nView)
{
    // TODO:  need to implement this function
    PCONTEXT pContext;

    HRESULT hr = GetContext(&pContext, m_nView);
    if (hr == S_OK)
        m_sViewName = pContext->GetName();

    return hr;
}


HRESULT CRCMStateMachine::GetContextSwitch(fastlane::PCONTEXTSWITCH *ppContextSwitch, const _TCHAR *pszContextName)
{
    CContextSwitchMap::const_iterator iter = m_contextSwitches.find(pszContextName);

    //ITRACE_DEVELOP(_T("m_contextSwitches.size()=") + IString((int)m_contextSwitches.size()));
    //CContextSwitchMap::const_iterator tempiter = m_contextSwitches.begin();
    //for (int i=0;tempiter!=m_contextSwitches.end(); tempiter++, i++)
    //{
    //    ITRACE_DEVELOP(_T("m_contextSwitches[") + IString(i) + _T("]=") + tempiter->second->GetName());
    //}

    if (iter == m_contextSwitches.end())
        return RCM_UNKNOWN_CONTEXT_SWITCH;

    *ppContextSwitch = iter->second;

    return S_OK;
}


HRESULT CRCMStateMachine::GetContext(fastlane::PCONTEXT *ppContext, const _TCHAR *pszContextName)
{
    CContextsMap::const_iterator iter = m_contexts.find(pszContextName);

    if (iter == m_contexts.end())
        return RCM_UNKNOWN_CONTEXT;

    *ppContext = iter->second;

    return S_OK;
}


HRESULT CRCMStateMachine::GetContext(fastlane::PCONTEXT *ppContext, int m_nView)
{
    _ASSERTE(m_pContexts!=NULL);

    if (!((m_nView >= 0) && (m_nView <= (int)m_nMaxContextID)))
        return RCM_INVALID_CONTEXT_ID;

    *ppContext = m_pContexts[m_nView];

    return S_OK;
}

// BC185047 - New custom data handling, takes in the current context name and sets the custom data variables
// used by RCM. Sets a variable to the empty string if it does not exist within the current context.
void CRCMStateMachine::GetCustomData(const _TCHAR *pContextName, StringType &sDataNeededView, StringType &sDefaultButton)
{   
    HRESULT hr;
    CString sText;

	VERIFY_PSX(hr, psx().GetCustomDataVar( _T("DataNeededView")
                                        , sText
                                        , _T("")
										, pContextName));
	sDataNeededView = sText.GetString();
	
	VERIFY_PSX(hr, psx().GetCustomDataVar( _T("DefaultButton")
										, sText
										, _T("")
										, pContextName));
    sDefaultButton = sText.GetString();
}

// BC185047 - New XML data handling by using PSX methods
HRESULT CRCMStateMachine::AddControlsAndContexts()
{
	HRESULT hr;
	CStringArray strContextNameArray;
	CStringArray strControlNameArray;
	StringType szName;
	StringType szType;
	CControlImpl *m_pXMLControl;
	CContextImpl *m_pXMLContext;
	
	m_pXMLControl = NULL;
	m_pXMLContext = NULL;

	psx().GetContextNameList(strContextNameArray);
	for(int i = 0; i < strContextNameArray.GetSize(); i++)
	{
		psx().GetControlNameList(strControlNameArray, strContextNameArray[i]);
		
		for(int j = 0; j < strControlNameArray.GetSize(); j++)
		{
			szName = strControlNameArray[j];
			// BC185047 - m_pXMLControl is deleted as part of the RCMStateMachine destructor
			m_pXMLControl = new CControlImpl(GetTraceHandle(), szName, szType);
			// BC185047 - Must set the control object name before calling Add
			m_pXMLControl->Name.Value(szName);
			hr = Add(m_pXMLControl);
		}
		szName = strContextNameArray[i];
		// BC185047 - m_pXMLContext is deleted as part of the RCMStateMachine destructor
		m_pXMLContext = new CContextImpl(GetTraceHandle(), szName);
		// BC185047 - Must set the context object name before calling Add
		m_pXMLContext->Name.Value(szName);
		hr = Add(m_pXMLContext);
		m_nMaxContextID += 1;
	}
	return hr;
}