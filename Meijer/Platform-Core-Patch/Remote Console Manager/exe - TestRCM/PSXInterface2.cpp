// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/exe - TestRCM/PSXInterface2.cpp 3     10/21/08 4:06p Sm133076 $
/*
 * @(#)PSXInterface2.h    $Revision: 3 $ $Date: 10/21/08 4:06p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/exe - TestRCM/PSXInterface2.cpp $
 * 
 * 3     10/21/08 4:06p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     1/26/05 2:14p Dm185016
 * Moved to new Repository.
// 
// 3     1/07/05 3:49p Dm185016
// 
// 2     11/02/04 3:00p Dm185016
// New functions
// 
// 1     10/29/04 8:48a Dm185016
// Single source with SCOTAPP.
*/

// Debug info is truncated to 255 warning
#pragma warning( disable : 4786 )
#pragma warning( push )

#include <stdafx.h>                 // MS MFC always needed first
#include <afxpriv.h>

#if defined(_SCOT) || defined(_SCOTSSF_)
#include "Common.h"                 // Common includes
#endif

#ifndef EMPTY_STRING
#define		EMPTY_STRING  _T("")
#endif

#include "PSXMfc.h" 
#include "psEvt.h" 
#include "PSXInterface.h"           // This header
#include "KVParmList.h" 
#include "RCMKeyValue.h" 
#include "RCMWrapper.h" 
#include "xerror.h" 

#pragma comment(lib, "comsuppw.lib")


#include "StringComparator.h"


void CPSXInterface::SetControl(CRemoteConsoleMgrWrapper *pControl)
{
	m_pControl = pControl;
}


CRemoteConsoleMgrWrapper &CPSXInterface::control()
{
    return *m_pControl;
}


bool CPSXInterface::ConnectToRemote( PPSXEVENT pEvent )
{
	bool bHandled = true;

    CString strRemoteConnectionName = pEvent->strRemoteConnectionName;
    if (m_connections.find(strRemoteConnectionName) != m_connections.end())
        return bHandled;

    //TODO:  Need to add code here to validate the id and password
	//bool bValidOp = tb.ValidateOperator(LPCTSTR szOperatorID, LPCTSTR szOperatorPw);

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OPERATION, KEY_CT_OPERATION_CONNECT );
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_IE_REMOTE_CONNECTION_NAME, (const _TCHAR *)pEvent->strRemoteConnectionName);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_IE_REMOTE_XML_NAME, _T("wSMKBDIBM4680.xml"));

    _bstr_t vParmList = pEvent->vParam;
    _bstr_t bstrParmList((const _TCHAR *)parmList); 
    bstrParmList += _T(";");
    bstrParmList += vParmList;

    BSTR bstrResult = NULL;

    HRESULT hr = control().OnControl(bstrParmList, &bstrResult);

    ::SysFreeString(bstrResult);

	bHandled = ((hr == S_OK) || (hr == S_FALSE));

    if (bHandled)
        m_connections.insert(strRemoteConnectionName);

    return true;
}


bool CPSXInterface::DisconnectFromRemote( PPSXEVENT pEvent )
{
	bool bHandled = true;

    CString strRemoteConnectionName = pEvent->strRemoteConnectionName;
    if (m_connections.find(strRemoteConnectionName) == m_connections.end())
        return bHandled;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OPERATION, KEY_CT_OPERATION_DISCONNECT );
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_IE_REMOTE_CONNECTION_NAME, (const _TCHAR *)pEvent->strRemoteConnectionName);
    _bstr_t bstrParmList((const _TCHAR *)parmList); 

    BSTR bstrResult = NULL;

    HRESULT hr = control().OnControl(bstrParmList, &bstrResult);

    ::SysFreeString(bstrResult);

	bHandled = (hr == S_OK);

    m_connections.erase(strRemoteConnectionName);

    return true;
}


bool CPSXInterface::SignOn( PPSXEVENT pEvent )
{
	bool bHandled = false;

    CString strRemoteConnectionName = pEvent->strRemoteConnectionName;
    if (m_connections.find(strRemoteConnectionName) == m_connections.end())
        return bHandled;

	CString vParam = (const _TCHAR *)(_bstr_t)pEvent->vParam;
	SignonParms parms;

	parms.ParseKeys((const _TCHAR *)vParam, GetTraceHandle());

	bool bValidOperator = true;

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OPERATION, KEY_CT_OPERATION_SIGNON );
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_IE_REMOTE_CONNECTION_NAME, (const _TCHAR *)pEvent->strRemoteConnectionName);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OP_ID, (const _TCHAR *)parms.m_csUserId);
    CKeyValue<bool>::Put(parmList, KEY_CT_OP_VALID, bValidOperator);
    _bstr_t bstrParmList((const _TCHAR *)parmList);

    BSTR bstrResult = NULL;

    HRESULT hr = control().OnControl(bstrParmList, &bstrResult);

    ::SysFreeString(bstrResult);

	bHandled = (hr == S_OK);

    return bValidOperator;
}


BEGIN_KV_HANDLER_MAP(CPSXInterface::SignonParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CPSXInterface::SignonParms, _T("UserId"), UserId)
    DEFINE_KV_HANDLER_MAP_ENTRY(CPSXInterface::SignonParms, _T("UserPwd"), UserPwd)
END_KV_HANDLER_MAP()

////////////////////////////////////////////////////////////////////////////////////////////
void CPSXInterface::SignonParms::UserId(CKeyValueParmList::iterator &keys)
{
    m_csUserId = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}
////////////////////////////////////////////////////////////////////////////////////////////
void CPSXInterface::SignonParms::UserPwd(CKeyValueParmList::iterator &keys)
{
    m_csUserPwd = CKeyValue<const _TCHAR *>::Get(keys, NULL);
}
////////////////////////////////////////////////////////////////////////////////////////////
void CPSXInterface::SignonParms::TraceThis(const _TCHAR *pPairs) throw()
{
}


bool CPSXInterface::SignOff( PPSXEVENT pEvent )
{
	bool bHandled = false;

    CString strRemoteConnectionName = pEvent->strRemoteConnectionName;
    if (m_connections.find(strRemoteConnectionName) == m_connections.end())
        return bHandled;


	CString vParam = (const _TCHAR*)(_bstr_t)pEvent->vParam;
	SignonParms parms;

	parms.ParseKeys((const _TCHAR *)vParam, GetTraceHandle());

    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OPERATION, KEY_CT_OPERATION_SIGNOFF );
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_CT_OP_ID, (const _TCHAR *)parms.m_csUserId);
    _bstr_t bstrParmList((const _TCHAR *)parmList);

    BSTR bstrResult = NULL;

    HRESULT hr = control().OnControl(bstrParmList, &bstrResult);

    ::SysFreeString(bstrResult);

	bHandled = (hr == S_OK);

    return bHandled;
}


bool CPSXInterface::ForwardInput( PPSXEVENT pEvent )
{
    IMODTRACE_DEVELOP();

    bool bHandled = false;

    CString strRemoteConnectionName = pEvent->strRemoteConnectionName;
    if (strRemoteConnectionName == _T(""))
        return bHandled;

    if (m_connections.find(strRemoteConnectionName) == m_connections.end())
        return bHandled;

    CKeyValueParmList parmList;
    CKeyValue<long>::Put(parmList, KEY_IE_LPARAM, pEvent->lParam);
    CKeyValue<long>::Put(parmList, KEY_IE_WPARAM, pEvent->wParam);
    CKeyValue<long>::Put(parmList, KEY_IE_EVENT_ID, pEvent->nEvent);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_IE_CONTROL_NAME, (const _TCHAR *)pEvent->strControlName);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_IE_CONTEXT_NAME, (const _TCHAR *)pEvent->strContextName);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_IE_EVENT_NAME, (const _TCHAR *)pEvent->strEventName);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_IE_REMOTE_CONNECTION_NAME, (const _TCHAR *)pEvent->strRemoteConnectionName);
    _bstr_t bstrParmList((const _TCHAR *)parmList); 

    HRESULT hr = control().OnInputEvent(bstrParmList);
	switch (hr)
	{
		case S_OK :
			bHandled = true;
			break;
		case S_FALSE :
			break;
		default :
		    trace( L6, _T("CPSXInterface::EventHandler failure to handle event.  hr=%d"), hr);
			bHandled = true;
			break;
	}

    return bHandled;
}
