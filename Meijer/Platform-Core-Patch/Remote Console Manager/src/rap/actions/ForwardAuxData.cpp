// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ForwardAuxData.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ForwardAuxData.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ForwardAuxData.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 4     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     4/18/05 7:18p Dm185016
 * Toggle support
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "ForwardAuxData.h"
#include "RCManager.h"
#include "RCMEvent.h"
#include "Message.h"
#include "RCMKeyValue.h"


BEGIN_KV_HANDLER_CLASS(CForwardAuxData::FADParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CForwardAuxData::FADParms, KEY_EV_AUX_DATA, AuxData)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CForwardAuxData::FADParms, AuxData)
BEGIN_KV_HANDLER_CLASS_CLEAR(CForwardAuxData::FADParms)
    CLEAR_KV_CSTRING(AuxData)
END_KV_HANDLER_CLASS_CLEAR()


CForwardAuxData::CForwardAuxData()
{
    CRCMActionBase::SetName(_T("CForwardAuxData"));
}


CForwardAuxData::~CForwardAuxData()
{
}


const _TCHAR *CForwardAuxData::GetUnsolicitedData()
{
    IMODTRACE_DEVELOP();

    return (const _TCHAR *)m_parms.GetAuxData();
}


HRESULT CForwardAuxData::ParseMessage(IMessage &message)
{
    return m_parms.ParseKeys((const _TCHAR *)message.GetData(), GetTraceHandle());
}


// Get data string from XML
HRESULT CForwardAuxData::ParseOperands()
{ 
    return S_OK;
}


template <>
IAction* CCreateable<CForwardAuxData, IAction>::NewInstance()
{
    IAction *pAction = new CForwardAuxData;

    return pAction;
}


template <>
void CCreateable<CForwardAuxData, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(ForwardAuxData)
DEFINE_ACTION_RELEASE_FXN(ForwardAuxData)
