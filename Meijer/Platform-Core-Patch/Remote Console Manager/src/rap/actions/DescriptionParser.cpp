// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DescriptionParser.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)DescriptionParser.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DescriptionParser.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 15    11/03/06 9:48a Tn185011
 * Fix TAR 336739: Rap Lane button text does not correspond to actions at
 * Lane. (Multiple instan)
 * 
 * 14    10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 12    9/05/06 9:35a Tn185011
 * Fix TAR 331295 -  Bld601: non text red lane button on RAP, status
 * window showing "help"
 * 
 * 13    9/05/06 9:28a Tn185011
 * Fix TAR 331295 -  Bld601: non text red lane button on RAP, status
 * window showing "help"
 * 
 * 11    1/13/06 2:28p Dm185016
 * TAR 301329
 * 
 * 10    6/28/05 5:43p Dm185016
 * TAR 304xxx - Problem with commas in DESCRIPTION=.
 * 
 * 8     5/01/05 8:57a Tp151000
 * Performmance
 * 
 * 7     4/15/05 9:19a Dm185016
 * Merge from Patch B
 * 
 * 6     4/14/05 10:08p Dm185016
 * TAR 298101
 * 
 * 5     3/25/05 1:07p Dm185016
 * TAR 296505 - Put if statement back.
 * 
 * 4     3/25/05 9:59a Dm185016
 * TAR 296505
 * 
 * 3     3/07/05 4:15p Dm185016
 * TAR 293605.
 * 
 * 2     2/23/05 4:25p Tp151000
 * 293998
 * 
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
// 
// 5     1/18/05 3:44p Dm185016
// UNICODE support
// 
// 4     1/12/05 5:50p Dm185016
// TAR 289897
// 
// 3     12/22/04 10:32a Dm185016
// TAR 288934 - Added override to force updatig the buttons.
// 
// 2     12/14/04 11:18a Dm185016
// TAR 288478.  Changes to correctly display text on buttons
// 
// 1     11/30/04 4:18p Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "DescriptionParser.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"
#include "RCMMessages.h"
#include "RCMKeyValue.h"
#include "RCMLaneXML.h"
#include "RCMError.h"
#include "PSXMfc.h"
#include "KVParmList.h"
#include "RAPReceipt.h"
#include "verifypsx.h"

using namespace rap;

BEGIN_KV_HANDLER_CLASS(CDescriptionParser::DescriptionParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CDescriptionParser::DescriptionParms, KEY_EV_DESCRIPTION_DESCRIPTION, Description)
    DEFINE_KV_HANDLER_MAP_ENTRY(CDescriptionParser::DescriptionParms, KEY_EV_DESCRIPTION_LANE_BUTTON_TEXT, LaneButtonText)
    DEFINE_KV_HANDLER_MAP_ENTRY(CDescriptionParser::DescriptionParms, KEY_EV_DESCRIPTION_LANE_STATUS_LINE_1, LaneStatusLine1)
    DEFINE_KV_HANDLER_MAP_ENTRY(CDescriptionParser::DescriptionParms, KEY_EV_DESCRIPTION_LANE_STATUS_LINE_2, LaneStatusLine2)
    DEFINE_KV_HANDLER_MAP_ENTRY(CDescriptionParser::DescriptionParms, KEY_EV_DESCRIPTION_LANE_STATUS_LINE_3, LaneStatusLine3)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CDescriptionParser::DescriptionParms, Description)
DEFINE_KV_CSTRING(CDescriptionParser::DescriptionParms, LaneButtonText)
DEFINE_KV_CSTRING(CDescriptionParser::DescriptionParms, LaneStatusLine1)
DEFINE_KV_CSTRING(CDescriptionParser::DescriptionParms, LaneStatusLine2)
DEFINE_KV_CSTRING(CDescriptionParser::DescriptionParms, LaneStatusLine3)
BEGIN_KV_HANDLER_CLASS_CLEAR(CDescriptionParser::DescriptionParms)
    CLEAR_KV_CSTRING(Description)
    CLEAR_KV_CSTRING(LaneButtonText)
    CLEAR_KV_CSTRING(LaneStatusLine1)
    CLEAR_KV_CSTRING(LaneStatusLine2)
    CLEAR_KV_CSTRING(LaneStatusLine3)
END_KV_HANDLER_CLASS_CLEAR()


// Get all control names from the xml
HRESULT CDescriptionParser::ParseDescription()
{ 
    HRESULT hr = S_OK;

    m_parms.Clear();

    StringType sDescription = CKeyValueParmList::EscapeValue(GetDescription());
    sDescription = CKeyValueParmList::EscapeValueSeparator(sDescription);

    m_parms.ParseKeys(sDescription.c_str(), GetTraceHandle());

    return hr;
}


HRESULT CDescriptionParser::UpdateDescription(bool bImmediate, bool bShowDescription)
{
    HRESULT hr;

    VERIFY_PSX(hr, psx().SetControlProperty(UI::CONTROLDISPLAY, UI::PROPERTYREDRAW, VARIANT_FALSE));

    CString sDescription = m_parms.GetDescription();
    if (bShowDescription && !sDescription.IsEmpty())
    {
        ITRACE_DEVELOP(_T(__FUNCTION__) _T("  Creating receipt item:  ") 
                     + IString((const _TCHAR *)sDescription));
        CMessageReceiptItem item(GetTraceHandle(), sDescription);
        hr = AddReceiptLine(item, false, false);
    }
    SetTransactionVariable(GetInstructionTextVariableName(), sDescription);

    CString sLaneStatusLine1 = m_parms.GetLaneStatusLine1();
    if (!sLaneStatusLine1.IsEmpty())
        SetTransactionVariable(GetLaneStatus1ControlName(), (const _TCHAR *)sLaneStatusLine1);

    bool bUpdateButtons = bImmediate;
    if (bUpdateButtons)
    {
        ITRACE_DEVELOP(_T(__FUNCTION__) _T("  Unconditional button update requested"));
    }
    else if (rapmgr().GetCurrentIntervention() != NULL)
    {
        ITRACE_DEVELOP(_T(__FUNCTION__) _T("  Intervention in progress already.  Cannot update buttons!"));
    }
    else if (!rapmgr().IsDelayedInterventionPending() || rapmgr().IsImmediateInterventionPending())
    {
        ITRACE_DEVELOP(_T(__FUNCTION__) _T("  No Delayed interventions pending ..."));
        bUpdateButtons = true;
    }
    if (bUpdateButtons)
    {
        ITRACE_DEVELOP(_T(__FUNCTION__) _T("  Updating buttons ..."));
        SetText(GetLaneButtonTextControlName(), m_parms.GetLaneButtonText());
        SetTransactionVariable(GetLaneStatus2ControlName(), m_parms.GetLaneStatusLine2());
        SetTransactionVariable(GetLaneStatus3ControlName(), m_parms.GetLaneStatusLine3());
    }

    VERIFY_PSX(hr, psx().SetControlProperty(UI::CONTROLDISPLAY, UI::PROPERTYREDRAW, VARIANT_TRUE));

    return hr;
}


HRESULT CDescriptionParser::SetText(const _TCHAR *pszControlName, const _TCHAR *pszText)
{
    HRESULT hr = S_OK;

    ITRACE_DEVELOP(_T(__FUNCTION__) _T("  pszControlName:  \"") + IString(pszControlName) 
                 + _T("\"") + _T("  pszText:  \"") + IString(pszText) + _T("\""));

    if ((pszControlName == NULL) || (pszControlName[0]==0))
        return STATEM_MISSING_PARAMETER;

    //if ((pszText == NULL) || (pszText[0]==0))
    //    return S_OK;

    COleVariant varOldText;
    VERIFY_PSX(hr, psx().GetConfigProperty(pszControlName, UI::ALL_CONTEXTS, UI::PROPERTYTEXTFORMAT, varOldText));

    _variant_t varText = pszText;
    if ((_bstr_t)_variant_t((LPVARIANT)varOldText) != (_bstr_t)varText)
        VERIFY_PSX(hr, psx().SetConfigProperty(pszControlName, UI::ALL_CONTEXTS, UI::PROPERTYTEXTFORMAT, varText));
    else
        ITRACE_DEVELOP(_T(__FUNCTION__) _T("  Same text.  Ignoring"));

    return hr;
}


HRESULT CDescriptionParser::SetTransactionVariable(const _TCHAR *pszVarName, const _TCHAR *pszText)
{
    HRESULT hr = S_OK;

    ITRACE_DEVELOP(_T(__FUNCTION__) _T("  pszVarName:  \"") + IString(pszVarName) + _T("\"") 
                 + _T("  pszText:  \"") + IString(pszText) + _T("\""));

    if ((pszVarName == NULL) || (pszVarName[0]==0))
        return STATEM_MISSING_PARAMETER;

    COleVariant varOldText;
    VERIFY_PSX(hr, psx().GetTransactionVariable(pszVarName, varOldText));

    _variant_t varValue = pszText;
    if ((_bstr_t)_variant_t((LPVARIANT)varOldText) != (_bstr_t)varValue)
        VERIFY_PSX(hr, psx().SetTransactionVariable(pszVarName, varValue));
    else
        ITRACE_DEVELOP(_T(__FUNCTION__) _T("  Same text.  Ignoring"));

    return hr;
}


CRemoteAPMgr &CDescriptionParser::rapmgr() { return *CSingleton<CRemoteAPMgr>::instance(); }


CPSX &CDescriptionParser::psx() { return CSingleton<CRCManager>::instance()->psx(); }
