// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SendInterventionEvent.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SendInterventionEvent.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SendInterventionEvent.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 8     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 5     5/01/05 8:58a Tp151000
 * Performmance
 * 
 * 3     2/23/05 8:37p Dm185016
 * Values for Red and yellow could be 4.  Converted to use LONG instead of
 * BOOL.
 * 
 * 2     2/18/05 11:28a Dm185016
 * 
 * 1     2/17/05 5:55p Dm185016
 * Alert buttons functionality
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "SendInterventionEvent.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"
#include "RCMMessages.h"
#include "RCMKeyValue.h"
#include "RCMLaneXML.h"
#include "RAPUIDefines.h"
#include "PSXMfc.h"
#include "KVParmList.h"
#include "verifypsx.h"


BEGIN_KV_HANDLER_CLASS(CSendInterventionEvent::SIEParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSendInterventionEvent::SIEParms, KEY_CT_OPERATION_SET_LIGHT_STATE_GREEN, Green)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSendInterventionEvent::SIEParms, KEY_CT_OPERATION_SET_LIGHT_STATE_YELLOW, Yellow)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSendInterventionEvent::SIEParms, KEY_CT_OPERATION_SET_LIGHT_STATE_RED, Red)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSendInterventionEvent::SIEParms, KEY_CT_OPERATION, Operation)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CSendInterventionEvent::SIEParms, Operation)
DEFINE_KV_LONG(CSendInterventionEvent::SIEParms, Green)
DEFINE_KV_LONG(CSendInterventionEvent::SIEParms, Yellow)
DEFINE_KV_LONG(CSendInterventionEvent::SIEParms, Red)


void CSendInterventionEvent::SIEParms::Clear()
{
    m_Operation.Empty();
    m_Green = 0;
    m_Yellow = 0;
    m_Red = 0;
}


CSendInterventionEvent::CSendInterventionEvent()
{
    CRCMActionBase::SetName(_T("CSendInterventionEvent"));
}


CSendInterventionEvent::~CSendInterventionEvent()
{
}


HRESULT CSendInterventionEvent::ParseOperands()
{ 
    HRESULT hr = S_OK;

    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();

    if (iter == parms.end())
        return STATEM_MISSING_PARAMETER;
    
    m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());

    return hr;
}


const _TCHAR *CSendInterventionEvent::GetInterventionType()
{
    const _TCHAR *pszInterventionType;
    if (m_parms.GetRed() != 0)
    {
        pszInterventionType = KEY_CT_INTERVENTION_TYPE_IMMEDIATE;
    }
    else if (m_parms.GetYellow() != 0)
    {
        pszInterventionType = KEY_CT_INTERVENTION_TYPE_DELAYED;
    }
    else // if (m_bGreen) || gray
    {
        pszInterventionType = KEY_CT_INTERVENTION_TYPE_NONE;
    }

    return pszInterventionType;
}


const _TCHAR *CSendInterventionEvent::GetOperation()
{
    return (const _TCHAR *)m_parms.GetOperation();
}


bool CSendInterventionEvent::IsOperationRequired() const
{
    return !m_parms.GetOperation().IsEmpty();
}


bool CSendInterventionEvent::IsPreviousInterventionType() const
{
    ETriliteColors newColor;
 
    if (m_parms.GetRed() != 0)
        newColor = rap::Red;
    else if (m_parms.GetYellow() != 0)
        newColor = rap::Yellow;
    else if (m_parms.GetGreen() != 0)
        newColor = rap::Green;
    else
        newColor = rap::Gray;

    bool bRetValue = (rapmgr().GetPreviousInterventionColor() == newColor);

    if (!bRetValue)
        rapmgr().SetPreviousInterventionColor(newColor);

    return bRetValue;
}


HRESULT CSendInterventionEvent::Execute(bool *pResult, IMessage &message)
{
    m_parms.Clear();

    HRESULT hr = m_parms.ParseKeys((_TCHAR *)message.GetData(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

	hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    if (!IsPreviousInterventionType())
        hr = SendEvent();
    else
    {
        ITRACE_DEVELOP(_T("Duplicate event requested.  Ignored!"));
    }

    *pResult = true;

    return hr;
}


template <>
IAction* CCreateable<CSendInterventionEvent, IAction>::NewInstance()
{
    IAction *pAction = new CSendInterventionEvent;

    return pAction;
}


template <>
void CCreateable<CSendInterventionEvent, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(SendInterventionEvent)
DEFINE_ACTION_RELEASE_FXN(SendInterventionEvent)
