#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/RCMStateMachineRAP.h 3     1/12/10 11:59p Cu185008 $
/*
 * @(#)RCMStateMachineRAP.h      $Revision: 3 $  $Date: 1/12/10 11:59p $
 *
 * Copyright 2005, NCR Corporation.
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/RCMStateMachineRAP.h $
 * 
 * 3     1/12/10 11:59p Cu185008
 * G2 Migration.
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     3/01/05 2:04p Dm185016
 * TAR 294587.  Added application state functionality.
*/
#include "RCMStateMachine.h"
#include "AppState.h"
#include "rcmapi.h"

#pragma warning( push )
#pragma warning( disable : 4251 )


namespace STATE_MACHINE_NAMESPACE
{
    class CStateImpl;
}
//class STATE_MACHINE_NAMESPACE::CSMLoader;

using namespace fastlane;

namespace rap
{

class CRCMLoaderRAP;

class RCMANAGER_API CRCMStateMachineRAP
    : public CRCMStateMachine
{

public: 

    CRCMStateMachineRAP(const _TCHAR *pFn, HTRACE hTrace);
    virtual ~CRCMStateMachineRAP();

    HRESULT Add(CApplicationState *);

    HRESULT GetAppState(rap::CApplicationState **, const _TCHAR *) const; 

protected:

    //virtual SAXErrorHandlerImpl *GetErrorHandler();
    virtual CSMLoader *GetLoader();
    virtual void ReleaseLoader();

    HRESULT AddExitList(CApplicationState &cswitch);
    HRESULT AddEntryList(CApplicationState &cswitch);
    
    const utils::IErrorInfo &GetErrorInfo(HRESULT);

    const TCHAR * GetIdentifier() const;

private:

    CRCMStateMachineRAP();

//  CApplicationStatesMap appstates;
	CRCMLoaderRAP *m_pRCMLoader;

    PAPPSTATE *   m_pAppStates;

    CApplicationStatesMap m_appstates;

    int m_nMaxAppStateID;
};

}

#pragma warning( pop )

