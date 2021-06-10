// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SendAllLaneStatusSummary.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SendAllLaneStatusSummary.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SendAllLaneStatusSummary.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 4     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     4/12/05 2:53p Dm185016
 * Changes for AllLaneStatusSummary
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "SendAllLaneStatusSummary.h"
#include "RCManager.h"
#include "RCMEvent.h"
#include "Message.h"
#include "RCMKeyValue.h"


BEGIN_KV_HANDLER_CLASS(CSendAllLaneStatusSummary::SALSSParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSendAllLaneStatusSummary::SALSSParms, KEY_EV_AUX_DATA, AuxData)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CSendAllLaneStatusSummary::SALSSParms, AuxData)
BEGIN_KV_HANDLER_CLASS_CLEAR(CSendAllLaneStatusSummary::SALSSParms)
    CLEAR_KV_CSTRING(AuxData)
END_KV_HANDLER_CLASS_CLEAR()


CSendAllLaneStatusSummary::CSendAllLaneStatusSummary()
{
    CRCMActionBase::SetName(_T("CSendAllLaneStatusSummary"));
}


CSendAllLaneStatusSummary::~CSendAllLaneStatusSummary()
{
}


const _TCHAR *CSendAllLaneStatusSummary::GetUnsolicitedData()
{
    IMODTRACE_DEVELOP();

    return (const _TCHAR *)m_parms.GetAuxData();
}


HRESULT CSendAllLaneStatusSummary::ParseMessage(IMessage &message)
{
    return m_parms.ParseKeys((const _TCHAR *)message.GetData(), GetTraceHandle());
}


// Get data string from XML
HRESULT CSendAllLaneStatusSummary::ParseOperands()
{ 
    return S_OK;
}


template <>
IAction* CCreateable<CSendAllLaneStatusSummary, IAction>::NewInstance()
{
    IAction *pAction = new CSendAllLaneStatusSummary;

    return pAction;
}


template <>
void CCreateable<CSendAllLaneStatusSummary, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(SendAllLaneStatusSummary)
DEFINE_ACTION_RELEASE_FXN(SendAllLaneStatusSummary)
