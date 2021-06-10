// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/remote-console-manager/receipt-content/TopListItem.cpp 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)TopListItem.cpp    $Revision: 1 $ $Date: 6/20/08 1:11p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/remote-console-manager/receipt-content/TopListItem.cpp $
 * 
 * 1     6/20/08 1:11p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:25p Dm185016
 * Moved to new Repository.
// 
// 3     1/18/05 3:53p Dm185016
// UNICODE support
// 
// 2     1/12/05 6:15p Dm185016
// TAR 289897.
// 
// 1     10/29/04 9:18a Dm185016
*/
#include "StdAfx.h"
#include "TopListItem.h"
#include "RCMLaneXML.h"
#include "RemoteAPMgr.h"
#include "PSXMFC.h"
#include "PSXError.h"
#include "verifypsx.h"

using namespace fastlane;
using namespace rap;


CTopListItem::CTopListItem(HTRACE hTrace, const _TCHAR *pszMsg)
    : CReceiptItem(hTrace)
    , m_sMsg(pszMsg)
{
}


HRESULT CTopListItem::Create(const StringType &szSource, const StringType &szParentControlName )
{
    HRESULT hr = S_OK;

    ITRACE_DEVELOP(_T(__FUNCTION__) _T("  szSource:  ") + IString(szSource.c_str()));

    CString sParentId = _T("");

    if (szSource != _T(""))
    {
        VERIFY_PSX(hr, psx().GetSelectedReceiptItem(szParentControlName.c_str(), sParentId) );
        ITRACE_DEVELOP(_T(__FUNCTION__) _T("  Parent Item Id:  ") + IString((const _TCHAR *)sParentId));
    }

    VERIFY_PSX(hr, psx().CreateReceiptItem( szSource.c_str(), GetId(), sParentId ) );

    return hr;
}


HRESULT CTopListItem::AddToReceipt(const StringType &szSource)
{
    HRESULT hr = S_OK;

    ITRACE_DEVELOP(_T(__FUNCTION__) _T("  szSource:  ") + IString(szSource.c_str()));
    ITRACE_DEVELOP(_T(__FUNCTION__) _T("  m_sMsg:  ") + IString(m_sMsg.c_str()));

    _variant_t varText = m_sMsg.c_str();
    VERIFY_PSX(hr, psx().SetReceiptItemVariable( szSource.c_str()
                                           , GetId()
                                           , TOP_LIST_ITEM_MSG
                                           , varText ));

    return hr;
}
