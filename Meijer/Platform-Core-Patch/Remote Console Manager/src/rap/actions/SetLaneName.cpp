// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SetLaneName.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SetLaneName.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SetLaneName.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     2/28/05 4:15p Dm185016
 * lint
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
// 
// 5     1/18/05 3:45p Dm185016
// UNICODE support
// 
// 4     1/07/05 10:15a Dm185016
// TAR 288479 - Lane name on button was not updating until transaction
// start.
// 
// 3     12/14/04 11:20a Dm185016
// Changed per Dan
// 
// 2     12/07/04 4:17p Dm185016
// Added trace information
// 
// 1     11/18/04 1:37p Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "SetLaneName.h"
#include "RemoteAPMgr.h"
#include "RCMMessages.h"
#include "RCMKeyValue.h"
#include "KVParmList.h"
#include "RCMLaneXML.h"
#include "PSXMfc.h"
#include "UIDefines.h"


CSetLaneName::CSetLaneName()
{
    CRCMActionBase::SetName(_T("CSetLaneName"));
}


CSetLaneName::~CSetLaneName()
{
}


BEGIN_KV_HANDLER_MAP(CSetLaneName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSetLaneName, KEY_LANE_NAME, OnLaneName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSetLaneName, KEY_BUTTON_NAME, OnButtonName)
END_KV_HANDLER_MAP()


DEFINE_KV_HANDLER(CSetLaneName, OnButtonName, keys)
{
    m_sButtonName = CKeyValue<const _TCHAR *>::Get(keys, _T(""));
}


DEFINE_KV_HANDLER(CSetLaneName, OnLaneName, keys)
{
    m_sLaneName = CKeyValue<const _TCHAR *>::Get(keys, _T(""));
}


// Get data string from XML
HRESULT CSetLaneName::ParseOperands()
{ 
    HRESULT hr = S_OK;

    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();

    if (iter != parms.end())
    {
        StringType sKeyString = *iter;
        ParseStringKeys(sKeyString.c_str());
    }

    return hr;
}


HRESULT CSetLaneName::Execute(bool *pResult, IMessage &)
{
    *pResult = true;

    m_sLaneName.clear();
    m_sButtonName.clear();

    HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    if (m_sLaneName.empty())
        m_sLaneName = rapmgr().GetLaneName();

    _ASSERTE(!m_sLaneName.empty());
    if (m_sLaneName.empty())
        ITRACE_WARNING(_T("No lane name specified"));

    ITRACE_DEVELOP(_T(__FUNCTION__) _T("  m_sLaneName:  \"") + IString(m_sLaneName.c_str()) + _T("\""));

    COleVariant vValue = m_sLaneName.c_str();
    psx().SetConfigProperty(m_sButtonName.c_str()
                          , UI::ALL_CONTEXTS
                          , UI::PROPERTYTEXTFORMAT
                          , vValue);

    return S_OK;
}


template <>
IAction* CCreateable<CSetLaneName, IAction>::NewInstance()
{
    IAction *pAction = new CSetLaneName;

    return pAction;
}


template <>
void CCreateable<CSetLaneName, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(SetLaneName)
DEFINE_ACTION_RELEASE_FXN(SetLaneName)
