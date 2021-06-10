// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsCurrentTriliteColor.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)IsCurrentTriliteColor.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsCurrentTriliteColor.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 4     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     5/01/05 8:58a Tp151000
 * Performmance
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "IsCurrentTriliteColor.h"
#include "RemoteAPMgr.h"
#include "RCMKeyValue.h"
#include "Message.h"
#include "KVParmList.h"


BEGIN_KV_HANDLER_CLASS(CIsCurrentTriliteColor::ICTCParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CIsCurrentTriliteColor::ICTCParms, KEY_CT_OPERATION_SET_LIGHT_STATE_GREEN, Green)
    DEFINE_KV_HANDLER_MAP_ENTRY(CIsCurrentTriliteColor::ICTCParms, KEY_CT_OPERATION_SET_LIGHT_STATE_YELLOW, Yellow)
    DEFINE_KV_HANDLER_MAP_ENTRY(CIsCurrentTriliteColor::ICTCParms, KEY_CT_OPERATION_SET_LIGHT_STATE_RED, Red)
END_KV_HANDLER_CLASS()
DEFINE_KV_INT(CIsCurrentTriliteColor::ICTCParms, Green)
DEFINE_KV_INT(CIsCurrentTriliteColor::ICTCParms, Yellow)
DEFINE_KV_INT(CIsCurrentTriliteColor::ICTCParms, Red)
BEGIN_KV_HANDLER_CLASS_CLEAR(CIsCurrentTriliteColor::ICTCParms)
    CLEAR_KV_INT(Green, 0)
    CLEAR_KV_INT(Yellow, 0)
    CLEAR_KV_INT(Red, 0)
END_KV_HANDLER_CLASS_CLEAR()


CIsCurrentTriliteColor::CIsCurrentTriliteColor()
{
    CRCMActionBase::SetName(_T("CIsCurrentTriliteColor"));
}


CIsCurrentTriliteColor::~CIsCurrentTriliteColor()
{
}


//-------------------------------------------------------------------------
// Methods from IEvaluateable
//-------------------------------------------------------------------------
long CIsCurrentTriliteColor::GetValue() const { return 0; }
void CIsCurrentTriliteColor::SetValue(long) {}


HRESULT CIsCurrentTriliteColor::Execute(bool *pResult, IMessage &message)
{
    m_parms.Clear();

    HRESULT hr = m_parms.ParseKeys((_TCHAR *)message.GetData(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    ETriliteColors newColor;
 
    if (m_parms.GetRed() != 0)
        newColor = rap::Red;
    else if (m_parms.GetYellow() != 0)
        newColor = rap::Yellow;
    else if (m_parms.GetGreen() != 0)
        newColor = rap::Green;
    else
        newColor = rap::Gray;

    *pResult = rapmgr().IsCurrentTriliteColor(newColor);
    
    return S_OK;
}


template <>
IAction* CCreateable<CIsCurrentTriliteColor, IAction>::NewInstance()
{
    IAction *pAction = new CIsCurrentTriliteColor;

    return pAction;
}


template <>
void CCreateable<CIsCurrentTriliteColor, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(IsCurrentTriliteColor)
DEFINE_ACTION_RELEASE_FXN(IsCurrentTriliteColor)
