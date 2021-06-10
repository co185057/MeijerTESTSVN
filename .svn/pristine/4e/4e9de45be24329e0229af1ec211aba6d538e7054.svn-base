// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsTriliteColorGray.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)IsTriliteColorGray.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsTriliteColorGray.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
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

#include "IsTriliteColorGray.h"
#include "RemoteAPMgr.h"
#include "RCMKeyValue.h"
#include "Message.h"
#include "KVParmList.h"


BEGIN_KV_HANDLER_CLASS(CIsTriliteColorGray::ICTCParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CIsTriliteColorGray::ICTCParms, KEY_CT_OPERATION_SET_LIGHT_STATE_GREEN, Green)
    DEFINE_KV_HANDLER_MAP_ENTRY(CIsTriliteColorGray::ICTCParms, KEY_CT_OPERATION_SET_LIGHT_STATE_YELLOW, Yellow)
    DEFINE_KV_HANDLER_MAP_ENTRY(CIsTriliteColorGray::ICTCParms, KEY_CT_OPERATION_SET_LIGHT_STATE_RED, Red)
END_KV_HANDLER_CLASS()
DEFINE_KV_INT(CIsTriliteColorGray::ICTCParms, Green)
DEFINE_KV_INT(CIsTriliteColorGray::ICTCParms, Yellow)
DEFINE_KV_INT(CIsTriliteColorGray::ICTCParms, Red)
BEGIN_KV_HANDLER_CLASS_CLEAR(CIsTriliteColorGray::ICTCParms)
    CLEAR_KV_INT(Green, 0)
    CLEAR_KV_INT(Yellow, 0)
    CLEAR_KV_INT(Red, 0)
END_KV_HANDLER_CLASS_CLEAR()


CIsTriliteColorGray::CIsTriliteColorGray()
{
    CRCMActionBase::SetName(_T("CIsTriliteColorGray"));
}


CIsTriliteColorGray::~CIsTriliteColorGray()
{
}


//-------------------------------------------------------------------------
// Methods from IEvaluateable
//-------------------------------------------------------------------------
long CIsTriliteColorGray::GetValue() const { return 0; }
void CIsTriliteColorGray::SetValue(long) {}


HRESULT CIsTriliteColorGray::Execute(bool *pResult, IMessage &message)
{
    m_parms.Clear();

    HRESULT hr = m_parms.ParseKeys((_TCHAR *)message.GetData(), GetTraceHandle());
    if (hr != S_OK)
        return hr;
 
    if (m_parms.GetRed() != 0)
        *pResult = false;
    else if (m_parms.GetYellow() != 0)
        *pResult = false;
    else if (m_parms.GetGreen() != 0)
        *pResult = false;
    else
        *pResult = true;
    
    return S_OK;
}


template <>
IAction* CCreateable<CIsTriliteColorGray, IAction>::NewInstance()
{
    IAction *pAction = new CIsTriliteColorGray;

    return pAction;
}


template <>
void CCreateable<CIsTriliteColorGray, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(IsTriliteColorGray)
DEFINE_ACTION_RELEASE_FXN(IsTriliteColorGray)
