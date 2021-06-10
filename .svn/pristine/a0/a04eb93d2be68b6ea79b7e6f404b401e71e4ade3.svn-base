#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/remote-console-manager/RCMStateMachine.h 4     1/25/10 8:21p Cu185008 $
/*
 * @(#)RCMStateMachine.h      $Revision: 4 $  $Date: 1/25/10 8:21p $
 *
 * Copyright 2003, NCR Corporation.
 *
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of NCR Corporation. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with msi.
 */
/*
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/remote-console-manager/RCMStateMachine.h $
 * 
 * 4     1/25/10 8:21p Cu185008
 * G2 Migration.
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 8     5/30/08 10:25a Bc185047
 * RCM Patch G Issue – Eliminate RCM’s dependence on XML format
 * 
 * 7     5/27/08 10:47a Bc185047
 * RCM Patch G Issue – Eliminate RCM’s dependence on XML format
 * 
 * Modified GetCustomData, added AddControlsAndContexts and the necessary
 * variables.
 * 
 * 6     3/07/08 1:31p Bc185047
 * New custom data handling. RCM was parsing through LaneRAP.xml to
 * retrieve the View and DefaultButton values. This stopped working after
 * the new XML schema took effect.
 * 
 * After this change, we are now setting the View and DefaultButton values
 * by using the PSX API.
 * 
 * 3     2/28/05 4:22p Dm185016
 * lint
 * 
 * 2     2/10/05 5:30p Dm185016
 * TAR 292915
 * 
 * 1     1/26/05 2:21p Dm185016
 * Moved to new Repository.
* 
* 4     1/25/05 3:46p Dm185016
* TAR 287899 - Memory leak during termination.
* 
* 3     11/16/04 11:33a Dm185016
* Context switch support
* 
* 2     10/29/04 8:59a Dm185016
* Added LaneRAP.xml processing
* 
* 1     10/04/04 5:36p Dm185016
* New for 3.2 Patch E/4.0
*/
#include "XMLStateMachine.h"
#include "ControlImpl.h"
#include "ContextImpl.h"
#include "ContextSwitch.h"
#include "rcmapi.h"
#include "PSXAction.h"
#include "RCManager.h"

#pragma warning( push )
#pragma warning( disable : 4251 )

const _TCHAR MSG_ENTRY_SUFFIX[] = _T("_entry_msg");
const _TCHAR MSG_EXIT_SUFFIX[]  = _T("_exit_msg");

namespace STATE_MACHINE_NAMESPACE
{
    class CStateImpl;
}
//class STATE_MACHINE_NAMESPACE::CSMLoader;

namespace fastlane
{

class CRCMLoader;

class RCMANAGER_API CRCMStateMachine
    : public CXMLStateMachine
{

public: 

    CRCMStateMachine(const _TCHAR *pFn, HTRACE hTrace);
    virtual ~CRCMStateMachine();

    HRESULT Add(CControlImpl *);

    HRESULT Add(CContextImpl *);

    HRESULT Add(CContextSwitch *);

    //HRESULT GetAppState(fastlane::CApplicationState **, const _TCHAR *) const; 

    void Start();

    void Stop();

    virtual const utils::IErrorInfo &GetErrorInfo(HRESULT hr);

    virtual bool IsControl(const _TCHAR *) const;
    virtual bool IsValidContextSwitch(const _TCHAR *) const;

    virtual HRESULT GetContext(fastlane::PCONTEXT *ppContext, const _TCHAR *);
    virtual HRESULT GetContext(fastlane::PCONTEXT *ppContext, int m_nView);

    virtual HRESULT GetViewName(StringType &m_sViewName, int m_nView);

    virtual HRESULT GetContextSwitch(fastlane::PCONTEXTSWITCH *ppContextSwitch, const _TCHAR *);

protected:

    ILoggable &loggable();

    //virtual SAXErrorHandlerImpl *GetErrorHandler();
    virtual CSMLoader *GetLoader();
    virtual void ReleaseLoader();

    HRESULT GetAction(IAction **, int);

    virtual HRESULT ReleaseAction(IAction *);

    virtual void InitActions();

    const TCHAR * GetIdentifier() const;

    virtual void Init();

    virtual void InitContexts();

    HRESULT AddExitList(CContextSwitch &cswitch);
    HRESULT AddEntryList(CContextSwitch &cswitch);

    CStateImpl &GetApplicationAnyState();

    virtual void UnregisterAll();

	// BC185047 - New custom data handling
	virtual void GetCustomData(const _TCHAR *pContextName, StringType &sDataNeededView, StringType &sDefaultButton);
	inline CRCManager &rcm() 
	{
		return *CSingleton<CRCManager>::instance(); 
	}

	inline CPSX &psx() 
	{ 
		return rcm().psx(); 
	}

	// BC185047 - New XML data handling by using PSX methods
	virtual HRESULT AddControlsAndContexts();

private:

    CRCMStateMachine();

//  CApplicationStatesMap appstates;
	CRCMLoader *m_pRCMLoader;

    PACTION *   m_pRCMActions;
    PCONTEXT *   m_pContexts;

    ILoggable *pLoggable;

    CControlsMap m_controls;

    CContextsMap m_contexts;

    CContextSwitchMap m_contextSwitches;

	int m_nMaxContextID;

	StringType sDataNeededViewValue;
	StringType sDefaultButtonValue;

};

}

#pragma warning( pop )

