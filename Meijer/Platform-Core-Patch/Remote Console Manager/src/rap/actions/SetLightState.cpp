// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SetLightState.cpp 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SetLightState.cpp    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SetLightState.cpp $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 10    10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 8     1/13/06 3:46p Tk124825
 * TAR 301329
 * 
 * 6     5/02/05 11:20a Dm185016
 *
 * 5     5/01/05 8:57a Tp151000
 * Performmance
 *
 * 2     3/22/05 2:48p Dm185016
 * UNICODE
 *
 * 1     3/11/05 2:45p Dm185016
 *
 * 2     2/28/05 4:40p Dm185016
 * lint
 *
 * 1     1/26/05 2:24p Dm185016
 * Moved to new Repository.
//
// 9     1/12/05 6:09p Dm185016
// Flash support (disabled)
//
// 8     12/22/04 11:15a Dm185016
// Added parameter to set the lane button text to the proper color.
//
// 7     12/14/04 11:27a Dm185016
// Change per Dan.
//
// 6     12/07/04 4:24p Dm185016
// Corrected lane name display on lane button
//
// 5     12/02/04 3:43p Dm185016
// Change SetConfigProperty to SetControlProperty for redraw per Dan.
//
// 4     11/19/04 1:39p Dm185016
// Added trace information
//
// 3     11/02/04 3:11p Dm185016
//
// 2     10/14/04 1:42p Dm185016
// Added actual implementation
//
// 1     10/04/04 5:36p Dm185016
// New for 3.2 Patch E/4.0
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "SetLightState.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"
#include "RCMKeyValue.h"
#include "RCMLaneXML.h"
#include "PSXMfc.h"
#include "KVParmList.h"
#include "verifypsx.h"

BEGIN_KV_HANDLER_MAP(CSetLightState)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSetLightState, KEY_CT_OPERATION_SET_LIGHT_STATE_GREEN, OnGreen)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSetLightState, KEY_CT_OPERATION_SET_LIGHT_STATE_YELLOW, OnYellow)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSetLightState, KEY_CT_OPERATION_SET_LIGHT_STATE_RED, OnRed)
END_KV_HANDLER_MAP()


BEGIN_KV_HANDLER_CLASS(CSetLightState::SLSParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSetLightState::SLSParms, KEY_EV_LANE_BUTTON_TEXT_CONTROL_NAME, LaneButtonTextControlName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSetLightState::SLSParms, KEY_EV_LANE_BUTTON_CONTROL_NAME     , LaneButtonControlName)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CSetLightState::SLSParms, LaneButtonTextControlName)
DEFINE_KV_CSTRING(CSetLightState::SLSParms, LaneButtonControlName)
BEGIN_KV_HANDLER_CLASS_CLEAR(CSetLightState::SLSParms)
    CLEAR_KV_CSTRING(LaneButtonTextControlName)
    CLEAR_KV_CSTRING(LaneButtonControlName)
END_KV_HANDLER_CLASS_CLEAR()


CSetLightState::CSetLightState()
    : m_pMessage(NULL)
    , m_hr(S_OK)
    , m_bGreen(false)
    , m_bYellow(false)
    , m_bRed(false)
    , m_bFlash(false)
{
    CRCMActionBase::SetName(_T("CSetLightState"));
}


CSetLightState::~CSetLightState()
{
    m_pMessage = NULL;
}


DEFINE_KV_HANDLER(CSetLightState, OnGreen, keys)
{
    m_hr = S_OK;

    int nLightState = CKeyValue<int>::Get(keys, 0);
    m_bGreen        = (nLightState != 0);
    m_bFlash        = (nLightState != 1);
    ITRACE_DEVELOP(_T("Green:  ") + IString(m_bGreen) + _T(".  Blinking:  ") + IString(m_bFlash));
}


DEFINE_KV_HANDLER(CSetLightState, OnYellow, keys)
{
    m_hr = S_OK;

    int nLightState = CKeyValue<int>::Get(keys, 0);
    m_bYellow       = (nLightState != 0);
    m_bFlash        = (nLightState != 1);
    ITRACE_DEVELOP(_T("Yellow:  ") + IString(m_bYellow) + _T(".  Blinking:  ") + IString(m_bFlash));
}


DEFINE_KV_HANDLER(CSetLightState, OnRed, keys)
{
    m_hr        = S_OK;

    int nLightState = CKeyValue<int>::Get(keys, 0);
    m_bRed          = (nLightState != 0);
    m_bFlash        = (nLightState != 1);
    ITRACE_DEVELOP(_T("Red:  ") + IString(m_bRed) + _T(".  Blinking:  ") + IString(m_bFlash));
}


HRESULT CSetLightState::ParseOperands()
{
    HRESULT hr = S_OK;

    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();

    if (iter == parms.end())
        return STATEM_MISSING_PARAMETER;

    m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());

    return hr;
}


bool CSetLightState::IsExistingColor(rap::ETriliteColors newColor)
{
    if (rapmgr().GetCurrentTriliteColor() == newColor)
        return true;

    rapmgr().SetCurrentTriliteColor(newColor);

    return false;
}


HRESULT CSetLightState::Execute(bool *pResult, IMessage &message)
{
    m_pMessage  = &message;
    m_bGreen    = false;
    m_bYellow   = false;
    m_bRed      = false;
    m_bFlash    = false;

	HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    ParseKeys((_TCHAR *)message.GetData(), GetTraceHandle());

    ETriliteColors newColor;
    StringType sTextColor;
    StringType sBackgroundColor;
    if (m_bRed)
    {
        sTextColor          = COLOR_TEXT_WHITE;
        sBackgroundColor    = COLOR_BACKGROUND_RED;
     //   if (m_pDialogMain->GetTopPriorityMultiLane() == nLane) //Check to see if this is the top lane.
	    //{
     //       SetButtonColor(IDC_LANE_BUTTON, CSCOTConfig::REDTOPPRIORITY);
	    //}
     //   else // Not the top lane so just set it to standard red.
	    //{
     //       SetButtonColor(IDC_LANE_BUTTON, CSCOTConfig::RED);
	    //}
        ITRACE_DEVELOP(_T("Setting light Red"));

        newColor = rap::Red;
    }
    else if (m_bYellow)
    {
        sTextColor          = COLOR_TEXT_BLACK;
        sBackgroundColor    = COLOR_BACKGROUND_YELLOW;
        ITRACE_DEVELOP(_T("Setting light Yellow"));

        newColor = rap::Yellow;
    }
    else if (m_bGreen)
    {
        sTextColor          = COLOR_TEXT_BLACK;
        sBackgroundColor    = COLOR_BACKGROUND_GREEN;
        ITRACE_DEVELOP(_T("Setting light Green"));

        newColor = rap::Green;
    }
    else //Grey button.
    {
        sTextColor          = COLOR_TEXT_BLACK;
        sBackgroundColor    = COLOR_BACKGROUND_GREY;
        ITRACE_DEVELOP(_T("Setting light Grey"));

        newColor = rap::Gray;
    }

    if (IsExistingColor(newColor))
        return S_OK;

    COleVariant varTextColor;
    COleVariant varOldColor;
    COleVariant varBackgroundColor;

    const _TCHAR *pszButtonName = m_parms.GetLaneButtonControlName();

    ITRACE_DEVELOP(_T("Text color:  ") + IString(sTextColor.c_str()));

    VERIFY_PSX_AND_LOG(psx().SetControlProperty(UI::CONTROLDISPLAY, UI::PROPERTYREDRAW, VARIANT_FALSE));

    VERIFY_PSX_AND_LOG(psx().GetConfigColor(sBackgroundColor.c_str(), varBackgroundColor));
    VERIFY_PSX_AND_LOG(psx().GetConfigProperty(pszButtonName, UI::ALL_CONTEXTS, UI::PROPERTYBACKGROUNDCOLOR, varOldColor));
    if (((LPVARIANT)varOldColor)->lVal != ((LPVARIANT)varBackgroundColor)->lVal)
    {
        ITRACE_DEVELOP(_T("New Background color:  ") + IString(sBackgroundColor.c_str()));
        VERIFY_PSX_AND_LOG(psx().SetConfigProperty(pszButtonName, UI::ALL_CONTEXTS, UI::PROPERTYBACKGROUNDCOLOR, varBackgroundColor));
    }

    VERIFY_PSX_AND_LOG(psx().GetConfigColor(sTextColor.c_str(), varTextColor));
    VERIFY_PSX_AND_LOG(psx().GetConfigProperty(pszButtonName, UI::ALL_CONTEXTS, UI::PROPERTYTEXTCOLOR, varOldColor));
    if (((LPVARIANT)varOldColor)->lVal != ((LPVARIANT)varTextColor)->lVal)
    {
        ITRACE_DEVELOP(_T("New Text color:  ") + IString(sTextColor.c_str()));
        VERIFY_PSX_AND_LOG(psx().SetConfigProperty(pszButtonName, UI::ALL_CONTEXTS, UI::PROPERTYTEXTCOLOR, varTextColor));
        VERIFY_PSX_AND_LOG(psx().SetConfigProperty((const _TCHAR *)m_parms.GetLaneButtonTextControlName(), UI::ALL_CONTEXTS, UI::PROPERTYTEXTCOLOR, varTextColor));
    }

    VERIFY_PSX_AND_LOG(psx().SetControlProperty(UI::CONTROLDISPLAY, UI::PROPERTYREDRAW, VARIANT_TRUE));

    *pResult = true;

    return m_hr;
}


IMessage &CSetLightState::GetMessage()
{
    _ASSERTE(m_pMessage!=NULL);
    return *m_pMessage;
}

template <>
IAction* CCreateable<CSetLightState, IAction>::NewInstance()
{
    IAction *pAction = new CSetLightState;

    return pAction;
}


template <>
void CCreateable<CSetLightState, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(SetLightState)
DEFINE_ACTION_RELEASE_FXN(SetLightState)
