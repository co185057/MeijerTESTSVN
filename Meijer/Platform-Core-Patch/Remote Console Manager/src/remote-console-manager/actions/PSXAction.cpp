// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/PSXAction.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)PSXAction.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/PSXAction.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     3/22/05 3:46p Dm185016
 * Added trace.
 * 
 * 2     2/28/05 4:38p Dm185016
 * lint
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
// 
// 11    1/21/05 4:44p Dm185016
// TAR 291218 - Added parameter to EnableControl.  If true then
// ALL_CONTEXTS used as context name.
// 
// 10    1/18/05 3:51p Dm185016
// UNICODE support
// 
// 9     1/17/05 5:55p Tp151000
// Show Total and Item in sign off state.
// 
// 8     1/12/05 6:07p Dm185016
// TAR 289897
// 
// 7     1/05/05 1:57p Tk124825
// TAR 287909 - Changed ClearReceipt() to take 5 parameters in order to
// make total & item controls visible or non-visible. 
// 
// 6     12/31/04 9:18a Dm185016
// TAR 288789 - Added override accepting const _TCHAR * for
// SetTransactionVariable.
// 
// 5     12/16/04 10:10a Dm185016
// Parameter list now a KV list.
// 
// 4     11/30/04 4:31p Dm185016
// Fixed receipt source.
// 
// 3     11/16/04 11:40a Dm185016
// Added transaction variable support
// 
// 2     11/02/04 3:11p Dm185016
// More updates
// 
// 1     10/29/04 9:04a Dm185016
*/
#include "StdAfx.h"

#include "PSXAction.h"
#include "RCManager.h"
#include "PSXMfc.h"
#include "verifypsx.h"
#include "RAPReceipt.h"
#include "RCMKeyValue.h"

using namespace fastlane;


BEGIN_KV_HANDLER_CLASS(CPSXAction::PSXParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CPSXAction::PSXParms, KEY_EV_DATA_SOURCE, DataSource)
    DEFINE_KV_HANDLER_MAP_ENTRY(CPSXAction::PSXParms, KEY_EV_RECEIPT_CONTROL_NAME , ReceiptControlName)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CPSXAction::PSXParms, DataSource)
DEFINE_KV_CSTRING(CPSXAction::PSXParms, ReceiptControlName)


void CPSXAction::PSXParms::Clear()
{
    m_DataSource.Empty();
    m_ReceiptControlName.Empty();
}


CPSXAction::CPSXAction()
{
}


CPSXAction::~CPSXAction()
{
}


// Get all control names from the xml
HRESULT CPSXAction::NextOperands(CParameterList::iterator &iter, CParameterList::const_iterator &end)
{ 
    HRESULT hr = S_OK;

    // Get the data source name for the receipt control
    if (iter != end)
    {    
        m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());

        m_sDataSource = m_parms.GetDataSource();
    }
    else
        m_sDataSource = _T("CMSMReceipt");

    return hr;
}


HRESULT CPSXAction::AddReceiptLine(CReceiptItem &item, bool /* bHilite */, bool bUseParentItem)
{
    HRESULT hr = S_OK;

    ITRACE_DEVELOP(_T(__FUNCTION__) _T("  Using data source:  ") + IString(GetDataSource()));
    if (bUseParentItem)
        hr = item.Create(GetDataSource(), (const _TCHAR *)m_parms.GetReceiptControlName());
    else
        hr = item.Create(GetDataSource());
    if (hr != S_OK)
        return hr;

    hr = item.AddToReceipt(GetDataSource());
    if (hr != S_OK)
        return hr;

    VERIFY_PSX(hr, psx().UpdateReceiptControls(GetDataSource()));

    return hr;
}


HRESULT CPSXAction::ClearReceipt(const _TCHAR *pszItemCountTextControlName
                               , const _TCHAR *pszTotalAmountTextControlName
                               , const _TCHAR *pszItemCountStaticTextControlName
                               , const _TCHAR *pszTotalAmountStaticTextControlName
                               , const bool bVisible)
{
    IMODTRACE_DEVELOP();

    HRESULT hr = S_OK;

    VERIFY_PSX(hr, psx().ClearReceipt(GetDataSource()));
    if ( StringType(pszItemCountTextControlName) != _T("") )
    {
        if ( bVisible)
        {
            SetControlVisible(pszItemCountTextControlName, true, false);
        }
        else
        {
            SetControlVisible(pszItemCountTextControlName, false, false);
        }
    }
    if ( StringType(pszTotalAmountTextControlName) != _T("") )
    {
        if ( bVisible) 
        {
            SetControlVisible(pszTotalAmountTextControlName, true, false);
        }
        else
        {
            SetControlVisible(pszTotalAmountTextControlName, false, false);
        }
    }
    if ( StringType(pszItemCountStaticTextControlName) != _T("") )
    {
        if ( bVisible) 
        {
            SetControlVisible(pszItemCountStaticTextControlName, true, false);
        }
        else
        {
            SetControlVisible(pszItemCountStaticTextControlName, false, false);
        }
    }
    if ( StringType(pszTotalAmountStaticTextControlName) != _T("") )
    {
        if ( bVisible) 
        {
            SetControlVisible(pszTotalAmountStaticTextControlName, true, false);
        }
        else
        {
            SetControlVisible(pszTotalAmountStaticTextControlName, false, false);
        }
    }

    return hr;
}


CString CPSXAction::GetString(const _TCHAR *pszName)
{
    CString strValue;
    HRESULT hr = S_OK;

    VERIFY_PSX(hr, psx().GetString( pszName, strValue ));
    if (hr == S_OK)
        ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  String id:  ") + IString(pszName)
                     + _T(".  Value:  ") + IString((const _TCHAR *)strValue));
    else
        ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  String id:  ") + IString(pszName)
                     + _T(" not found."));

    return strValue;
}


HRESULT CPSXAction::EnableControl(const StringType &controlName, bool bEnable, bool bUseAllContexts)
{
    HRESULT hr = S_OK;

    long state = (long) (bEnable ? UI::Normal : UI::Disabled);
    const _TCHAR *pszContextName = (bUseAllContexts ? UI::ALL_CONTEXTS : rcm().GetCurrentContext());

    ITRACE_DEVELOP(_T(__FUNCTION__) 
                   _T("  controlName=") + IString(controlName.c_str()) +
                   _T("  contextName=") + IString(pszContextName) + 
                   _T(".  bEnable=") + IString(bEnable));

    VERIFY_PSX(hr, psx().SetConfigProperty(controlName.c_str(), pszContextName, UI::PROPERTYSTATE, state));

    return hr;
}


HRESULT CPSXAction::SetTransactionVariable(const _TCHAR *pszVariableName, const _TCHAR *pszValue)
{
    COleVariant varValue = pszValue;
    return SetTransactionVariable(pszVariableName, varValue);
}


HRESULT CPSXAction::SetTransactionVariable(const _TCHAR *pszVariableName, COleVariant& varValue)
{
    ITRACE_DEVELOP(_T(__FUNCTION__) _T("  pszVariableName=") + IString(pszVariableName));

    HRESULT hr = S_OK;

    VERIFY_PSX(hr, psx().SetTransactionVariable(pszVariableName, varValue));

    return hr;
}


HRESULT CPSXAction::SetControlVisible(const StringType &controlName, bool bEnable, bool bUsedCurrentContext)
{
    HRESULT hr = S_OK;

    _variant_t vShow = (bEnable ? VARIANT_TRUE : VARIANT_FALSE);
	const _TCHAR *pszCurrentContext;

	if(bUsedCurrentContext)
	{
		pszCurrentContext = rcm().GetCurrentContext();
	}
	else
	{
		pszCurrentContext = UI::ALL_CONTEXTS;
	}
	VERIFY_PSX(hr, psx().SetConfigProperty(controlName.c_str(), pszCurrentContext, UI::PROPERTYVISIBLE, vShow));

    return hr;
}
