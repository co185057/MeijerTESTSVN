// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/actions/SignonCommand.cpp 5     2/05/10 3:01p Sm133076 $
/*
 * @(#)SignonCommand.cpp    $Revision: 5 $ $Date: 2/05/10 3:01p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/actions/SignonCommand.cpp $
 * 
 * 5     2/05/10 3:01p Sm133076
 * TAR398785
 * 
 * 4     5/04/09 10:43a Sm133076
 * 381428 Meijer lab 4.32.6 FL Reports: Daily Summary Report by Cashier
 * are reporting incorrect operator ID
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 6     11/27/07 5:42p Lp185019
 * SR629 (TAR 334800) - Resolve CISP Issue: RCM is tracing password by
 * default
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     3/11/05 2:48p Dm185016
 * 
 * 2     2/28/05 4:26p Dm185016
 * lint
 * 
 * 1     1/26/05 2:24p Dm185016
 * Moved to new Repository.
// 
// 7     1/18/05 3:52p Dm185016
// Fix for correctly setting time
// 
// 6     1/15/05 6:54p Dm185016
// Support for signon from scanner data.
// 
// 5     1/13/05 6:49p Dm185016
// Timer Server Support
// 
// 4     1/12/05 6:10p Dm185016
// Cleaned up trace log.
// 
// 3     11/16/04 11:44a Dm185016
// Added saving operator values
// 
// 2     11/02/04 3:17p Dm185016
// More updates
// 
// 1     10/29/04 9:04a Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "SignonCommand.h"
#include "RCManager.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"
#include "RCMKeyValue.h"
#include "RCMError.h"
#include "KVParmList.h"
#include "Operator.h"
#include "PSXMfc.h"
#include "RAPUIDefines.h"
#include "verifypsx.h"

BEGIN_KV_HANDLER_MAP(CSignonCommand)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSignonCommand, KEY_CT_OP_ID, OnOperatorID)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSignonCommand, KEY_CT_OP_PASSWORD, OnOperatorPwd)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSignonCommand, KEY_CT_OP_VALID, OnValid)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSignonCommand, KEY_CT_TIME_REQUEST, OnTimeRequest)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSignonCommand, KEY_CT_REMOTE_CONNECTION_NAME, OnRemoteConnectionName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSignonCommand, KEY_AUX_DATA, OnAuxDataRequest)	//POS81634
	DEFINE_KV_HANDLER_MAP_ENTRY(CSignonCommand, KEY_AP_APPROVAL_CODE, OnApprovalCode)	//POS81634
END_KV_HANDLER_MAP()


CSignonCommand::CSignonCommand()
    : m_nValid(false)
    , m_bTimeRequest(false)
{
    CRCMActionBase::SetName(_T("CSignonCommand"));
}


CSignonCommand::~CSignonCommand()
{
}


DEFINE_KV_HANDLER(CSignonCommand, OnRemoteConnectionName, keys)
{
    m_szRemoteConnectionName = CKeyValue<const _TCHAR *>::Get(keys, _T(""));
    ITRACE_DEVELOP(_T("remote connection name:  ") + IString(m_szRemoteConnectionName.c_str()));
}


DEFINE_KV_HANDLER(CSignonCommand, OnOperatorID, keys)
{
    m_szOpId = CKeyValue<const _TCHAR *>::Get(keys, _T(""));
    ITRACE_DEVELOP(_T("m_szOpId:  ") + IString(m_szOpId.c_str()));
}


DEFINE_KV_HANDLER(CSignonCommand, OnOperatorPwd, keys)
{
    m_szOpPwd = CKeyValue<const _TCHAR *>::Get(keys, _T(""));
    ITRACE_DEVELOP(_T("m_szOpPwd: \"****\""));
}


DEFINE_KV_HANDLER(CSignonCommand, OnTimeRequest, keys)
{
    m_bTimeRequest = CKeyValue<bool>::Get(keys, false);
    ITRACE_DEVELOP(_T("m_bTimeRequest:  ") + IString(m_nValid));
}


DEFINE_KV_HANDLER(CSignonCommand, OnValid, keys)
{
     m_nValid = CKeyValue<BOOL>::Get(keys, false);
    ITRACE_DEVELOP(_T("m_nValid:  ") + IString(m_nValid));
}

//POS81634 - Support 2 new fields in the SignOn command
DEFINE_KV_HANDLER(CSignonCommand, OnAuxDataRequest, keys)
{
    m_szAuxData = CKeyValue<const _TCHAR *>::Get(keys, _T(""));
    ITRACE_DEVELOP(_T("m_szAuxData:  ") + IString(m_szAuxData.c_str()));
}

DEFINE_KV_HANDLER(CSignonCommand, OnApprovalCode, keys)
{
    m_nApprovalCode = CKeyValue<int>::Get(keys, (int)0);
    ITRACE_DEVELOP(_T("m_nApprovalCode:  ") + IString(m_nApprovalCode));
}
//e POs81634

HRESULT CSignonCommand::Execute(bool *pResult, IMessage &message)
{
    IMODTRACE_DEVELOP();

    m_nValid        = 0;
    m_bTimeRequest  = true;

    HRESULT hr = S_OK;

    ParseKeys((_TCHAR *)message.GetData(), GetTraceHandle());

    if (m_szRemoteConnectionName.empty())
    {
        ITRACE_ERROR(_T("Missing parameter:  ") + IString(KEY_IE_REMOTE_CONNECTION_NAME));
        return RCM_API_FAILURE_INVALID_KEY_VALUE_LIST;
    }

    if (m_szOpId.empty())
    {
        ITRACE_ERROR(_T("Missing parameter:  ") + IString(KEY_CT_OP_ID));
        return RCM_API_FAILURE_INVALID_KEY_VALUE_LIST;
    }

	COperator *pOperator;
    CKeyValueParmList parmlist;
    CKeyValue<const _TCHAR*>::Put(parmlist, KEY_CT_OPERATION, KEY_CT_OPERATION_SIGNON );
    switch (m_nValid)
	{



		case 1:
    
			ITRACE_DEVELOP(_T("Operator Id:  ") + IString(m_szOpId.c_str()) + _T(" Valid"));
			CKeyValue<int>::Put(parmlist, KEY_CT_SUCCESS, m_nValid);

			if( m_szAuxData.empty() )
			{
				ITRACE_DEVELOP(_T("Set Operator Id:  ") + IString(m_szOpId.c_str()) + _T(" Valid"));
				pOperator = new COperator(m_szOpId.c_str(), m_szOpPwd.c_str());
				rapmgr().SetOperator(pOperator);
			}
			break;
    
		case 2:
		
			ITRACE_DEVELOP(_T("Operator Id:  ") + IString(m_szOpId.c_str()) + _T(" unknown"));
			CKeyValue<int>::Put(parmlist, KEY_CT_SUCCESS, m_nValid);
			break;
    
		default:
    
			ITRACE_DEVELOP(_T("Operator Id:  ") + IString(m_szOpId.c_str()) + _T(" Not valid"));
			CKeyValue<int>::Put(parmlist, KEY_CT_SUCCESS, m_nValid);
    }

    CKeyValue<const _TCHAR*>::Put(parmlist, KEY_CT_USERID, m_szOpId.c_str() );

    // If the RAP requested the time, then retrieve it and add to the parameter list.
    if (m_bTimeRequest)
    {
        SYSTEMTIME time;
        ::GetSystemTime(&time);//TAR398785;

        CKeyValue<int>::Put(parmlist, KEY_CT_TIME_REQUEST, (int)time.wYear);
        CKeyValue<int>::Put(parmlist, (int)time.wMonth);
        CKeyValue<int>::Put(parmlist, (int)time.wDay);
        CKeyValue<int>::Put(parmlist, (int)time.wHour);
        CKeyValue<int>::Put(parmlist, (int)time.wMinute);
        CKeyValue<int>::Put(parmlist, (int)time.wSecond);
    }

	//POS81634 - Populate the new field for Check in/oout command - rrs
	if( !m_szAuxData.empty() )
	{
		//ITRACE_DEVELOP(_T("Aux Data:  ") + IString(m_szAuxData.c_str()) + _T(" Valid"));
		//ITRACE_DEVELOP(_T("Approval Code:  ") + IString(m_nApprovalCode) );

		CKeyValue<const _TCHAR*>::Put(parmlist, KEY_AUX_DATA, m_szAuxData.c_str() );
		CKeyValue<int>::Put(parmlist, KEY_AP_APPROVAL_CODE, m_nApprovalCode );
	}
	// e POS81634

    ITRACE_DEVELOP(_T("parmlist:  ") + IString((const _TCHAR *)parmlist));

    _variant_t varParam = (const _TCHAR *)parmlist;

    VERIFY_PSX(hr, psx().GenerateEvent(_T(""),
                                       _T(""),
                                       UI::EVENTREMOTEDATA,
                                       varParam,
                                       m_szRemoteConnectionName.c_str()));

    *pResult = (m_nValid == 1);

    SetSynchronousReturnCode(message, hr);

    return hr;
}

template <>
IAction* CCreateable<CSignonCommand, IAction>::NewInstance()
{
    IAction *pAction = new CSignonCommand;

    return pAction;
}


template <>
void CCreateable<CSignonCommand, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(SignonCommand)
DEFINE_ACTION_RELEASE_FXN(SignonCommand)
