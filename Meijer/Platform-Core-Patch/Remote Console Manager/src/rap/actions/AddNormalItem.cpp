// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/AddNormalItem.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)AddNormalItem.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/AddNormalItem.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 9     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 7     1/26/06 5:51p Dm185016
 * TAR 316749
 * 
 * 5     1/13/06 2:27p Dm185016
 * TAR 301329
 * 
 * 2     3/15/05 3:33p Dm185016
 * UNICODE
 * 
 * 1     3/11/05 2:39p Dm185016
 * 
 * 4     2/28/05 4:29p Dm185016
 * lint
 * 
 * 3     2/23/05 4:29p Tp151000
 * 293998
 * 
 * 2     2/15/05 11:26a Dm185016
 * Removed creation of receipt item for alert messages.
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
// 
// 11    1/20/05 10:38a Dm185016
// TAR 290434
// 
// 10    1/18/05 3:48p Dm185016
// UNICODE support
// 
// 9     1/12/05 5:57p Dm185016
// TAR 289987.
// 
// 8     12/14/04 11:25a Dm185016
// TAR 288478.  Factored out common code.  AddNormalItem now inherits from
// DescriptionParser.
// 
// 7     12/07/04 4:20p Dm185016
// Corrected lane state window(x) display
// 
// 6     12/02/04 3:43p Dm185016
// Change SetConfigProperty to SetControlProperty for redraw per Dan.
// 
// 5     11/30/04 4:29p Dm185016
// Added hilited items only on RAP display.
// 
// 4     11/16/04 11:35a Dm185016
// New transaction variables
// 
// 3     11/02/04 3:09p Dm185016
// More updates
// 
// 2     10/29/04 9:07a Dm185016
// Moved PSX fxns to PSXAction base class
// 
// 1     10/14/04 1:40p Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "AddNormalItem.h"
#include "RCManager.h"
#include "RCMStateMachine.h"
#include "RCMMessages.h"
#include "RCMKeyValue.h"
#include "PSXMfc.h"
#include "KVParmList.h"
#include "RAPReceipt.h"
#include "verifypsx.h"


BEGIN_KV_HANDLER_CLASS(CAddNormalItem::ANIParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CAddNormalItem::ANIParms, KEY_EV_DESCRIPTION, Description)
    DEFINE_KV_HANDLER_MAP_ENTRY(CAddNormalItem::ANIParms, KEY_EV_LANE_BUTTON_CONTROL_NAME     , LaneButtonControlName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CAddNormalItem::ANIParms, KEY_EV_LANE_STATUS_BUTTON_CONTROL_NAME, LaneStatusButtonControlName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CAddNormalItem::ANIParms, KEY_EV_ASSIST_MODE_BUTTON_CONTROL_NAME, AssistModeButtonControlName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CAddNormalItem::ANIParms, KEY_EV_ITEM_COUNT_TEXT_CONTROL_NAME , ItemCountTextControlName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CAddNormalItem::ANIParms, KEY_EV_TOTAL_AMOUNT_TEXT_CONTROL_NAME , TotalAmountTextControlName)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CAddNormalItem::ANIParms, Description)
DEFINE_KV_CSTRING(CAddNormalItem::ANIParms, LaneButtonControlName)
DEFINE_KV_CSTRING(CAddNormalItem::ANIParms, LaneStatusButtonControlName)
DEFINE_KV_CSTRING(CAddNormalItem::ANIParms, AssistModeButtonControlName)
DEFINE_KV_CSTRING(CAddNormalItem::ANIParms, ItemCountTextControlName)
DEFINE_KV_CSTRING(CAddNormalItem::ANIParms, TotalAmountTextControlName)
BEGIN_KV_HANDLER_CLASS_CLEAR(CAddNormalItem::ANIParms)
    CLEAR_KV_CSTRING(Description)
    CLEAR_KV_CSTRING(LaneButtonControlName)
    CLEAR_KV_CSTRING(LaneStatusButtonControlName)
    CLEAR_KV_CSTRING(AssistModeButtonControlName)
    CLEAR_KV_CSTRING(ItemCountTextControlName)
    CLEAR_KV_CSTRING(TotalAmountTextControlName)
END_KV_HANDLER_CLASS_CLEAR()


CAddNormalItem::CAddNormalItem()
{
    CRCMActionBase::SetName(_T("CAddNormalItem"));
}


CAddNormalItem::~CAddNormalItem()
{
}


// Get all control names from the xml
HRESULT CAddNormalItem::ParseOperands()
{ 
    CParameterList &parms           = GetParameters();
    CParameterList::iterator iter   = parms.begin();
    CParameterList::iterator end    = parms.end();

    if (iter == parms.end())
        return STATEM_MISSING_PARAMETER;
    
    HRESULT hr = m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    return CDisplayControlParser::ParseOperands();
}


const _TCHAR *CAddNormalItem::GetDescription() const { return m_parms.GetDescription(); }


HRESULT CAddNormalItem::Execute(bool *pResult, IMessage &message)
{
    m_parms.Clear();

	HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    VERIFY_PSX_AND_LOG(psx().SetControlProperty((const _TCHAR *)UI::CONTROLDISPLAY, UI::PROPERTYREDRAW, VARIANT_FALSE));

    hr = m_parms.ParseKeys((_TCHAR *)message.GetData());
    if (hr != S_OK)
        return hr;

    StringType description = m_parms.GetDescription();
    if (description[0] == _T('['))
        hr = ParseSpecial(message);
    else if (description.find(KEY_EV_DESCRIPTION_LANE_STATUS_LINE_1))
    {
        hr = ParseDescription();
        if (hr != S_OK)
            return hr;

        hr = UpdateDescription();
        if (hr != S_OK)
            return hr;
    }

    VERIFY_PSX_AND_LOG(psx().SetControlProperty(CString(UI::CONTROLDISPLAY), CString(UI::PROPERTYREDRAW), VARIANT_TRUE));
    //VERIFY_PSX_AND_LOG(psx().UpdateReceiptControls( GetDataSource() ));
    *pResult = true;

    return hr;
}


HRESULT CAddNormalItem::ParseSpecial(IMessage &msg)
{
    HRESULT hr = S_OK;

    StringType description = m_parms.GetDescription();
    if (description.find(SPECIAL_TEXT_ALERT) != StringType::npos)
    {
        ITRACE_DEVELOP(IString(_T("Found ")) + SPECIAL_TEXT_ALERT);
        hr = ParseAlert(description.erase(0, SPECIAL_TEXT_ALERT_LENGTH), msg);
    }

    else if (description.find(SPECIAL_TEXT_NUM_ITEMS) != StringType::npos)
    {
        ITRACE_DEVELOP(IString(_T("Found ")) + SPECIAL_TEXT_NUM_ITEMS);
        ITRACE_DEVELOP(IString(_T("Text ")) + IString(description.substr(SPECIAL_TEXT_NUM_ITEMS_LENGTH).c_str()));
        COleVariant varValue = description.substr(SPECIAL_TEXT_NUM_ITEMS_LENGTH).c_str();
        VERIFY_PSX(hr, psx().SetTransactionVariable((const _TCHAR *)m_parms.GetItemCountTextControlName(), varValue));
    }

    else if (description.find(SPECIAL_TEXT_DELAY_COUNT) != StringType::npos)
    {
        ITRACE_DEVELOP(IString(_T("Found ")) + SPECIAL_TEXT_DELAY_COUNT);
    }

   else if (description.find(SPECIAL_TEXT_HIGHLIGHT_SUBITEM_EX) != StringType::npos)
    {
        ITRACE_DEVELOP(IString(_T("Found ")) + SPECIAL_TEXT_HIGHLIGHT_SUBITEM_EX);

        CMessageReceiptItem item(GetTraceHandle(), description.substr(SPECIAL_TEXT_HIGHLIGHT_SUBITEM_EX_LENGTH).c_str());
        hr = AddReceiptLine(item, false);
    }

    else if (description.find(SPECIAL_TEXT_HIGHLIGHT_EX) != StringType::npos)
    {
        ITRACE_DEVELOP(IString(_T("Found ")) + SPECIAL_TEXT_HIGHLIGHT_EX);

        CMessageReceiptItem item(GetTraceHandle(), description.substr(SPECIAL_TEXT_HIGHLIGHT_EX_LENGTH).c_str());
        hr = AddReceiptLine(item, false, false);
    }

    else if (description.find(SPECIAL_TEXT_SUMMARY_EX) != StringType::npos)
    {
        ITRACE_DEVELOP(IString(_T("Found ")) + SPECIAL_TEXT_SUMMARY_EX);

        CMessageReceiptItem item(GetTraceHandle(), description.substr(SPECIAL_TEXT_SUMMARY_EX_LENGTH).c_str());
        hr = AddReceiptLine(item, false, false);
    }

    else if (description.find(SPECIAL_TEXT_HIGHLIGHT) != StringType::npos)
    {
        ITRACE_DEVELOP(IString(_T("Found ")) + SPECIAL_TEXT_HIGHLIGHT);

        CMessageReceiptItem item(GetTraceHandle(), description.substr(SPECIAL_TEXT_HIGHLIGHT_LENGTH).c_str());
        hr = AddReceiptLine(item, false);
    }

    else if (description.find(SPECIAL_TEXT_SUMMARY) != StringType::npos)
    {
        ITRACE_DEVELOP(IString(_T("Found ")) + SPECIAL_TEXT_SUMMARY);

        CMessageReceiptItem item(GetTraceHandle(), description.substr(SPECIAL_TEXT_SUMMARY_LENGTH).c_str());
        hr = AddReceiptLine(item, false, false);
    }

    else if (description.find(SPECIAL_TEXT_TOTAL) != StringType::npos)
    {
        ITRACE_DEVELOP(IString(_T("Found ")) + SPECIAL_TEXT_TOTAL);
        ITRACE_DEVELOP(IString(_T("Text ")) + IString(description.substr(SPECIAL_TEXT_TOTAL_LENGTH).c_str()));
        COleVariant varValue = description.substr(SPECIAL_TEXT_TOTAL_LENGTH).c_str();
        VERIFY_PSX(hr, psx().SetTransactionVariable((const _TCHAR *)m_parms.GetTotalAmountTextControlName(), varValue));
    }

    else if (description.find(SPECIAL_TEXT_ERROR) != StringType::npos)
    {
        ITRACE_DEVELOP(IString(_T("Found ")) + SPECIAL_TEXT_ERROR);
    }

    return hr;
}


HRESULT CAddNormalItem::ParseAlert(const StringType &alertText, IMessage &msg)
{
    HRESULT hr = S_OK;

    ITRACE_DEVELOP(_T(__FUNCTION__) _T("  Checking alert:  \"") + IString(alertText.c_str()) + IString(_T("\"")));
    if (alertText.find(ALERT_POS_MESSAGE) != StringType::npos)
    {
        ITRACE_DEVELOP(IString(_T("Found ")) + SPECIAL_TEXT_ALERT + ALERT_POS_MESSAGE);
        IMessage &newMsg = stateMachine().MakeAsynchCopyEx(msg, MSG_NORMAL_ITEM_EVENT_POS_MESSAGE);
        hr = stateMachine().PostMsg(newMsg, true);
    }
    else
    {
        ITRACE_DEVELOP(IString(_T("Found only ")) + SPECIAL_TEXT_ALERT);

        //CMessageReceiptItem item(GetTraceHandle(), m_sText.substr(SPECIAL_TEXT_SUMMARY_LENGTH).c_str());
        //hr = AddReceiptLine(item, false, false);
    }

    return hr;
}
                                                                                 

template <>
IAction* CCreateable<CAddNormalItem, IAction>::NewInstance()
{
    IAction *pAction = new CAddNormalItem;

    return pAction;
}


template <>
void CCreateable<CAddNormalItem, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(AddNormalItem)
DEFINE_ACTION_RELEASE_FXN(AddNormalItem)
