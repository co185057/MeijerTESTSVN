// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/remote-console-manager/receipt-content/RAPReceipt.cpp 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)RAPReceipt.cpp    $Revision: 1 $ $Date: 6/20/08 1:11p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/remote-console-manager/receipt-content/RAPReceipt.cpp $
 * 
 * 1     6/20/08 1:11p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 4     3/29/05 8:45a Dm185016
 * TAR 296269
 * 
 * 3     2/21/05 8:07p Dm185016
 * Added trace info.
 * 
 * 2     2/04/05 9:56a Dm185016
 * TAR 292311 - item created no longer added to receipt as selectable
 * item.
 * 
 * 1     1/26/05 2:25p Dm185016
 * Moved to new Repository.
// 
// 6     1/18/05 3:53p Dm185016
// UNICODE support
// 
// 5     1/13/05 1:43p Dm185016
// Remove leading white space.
// 
// 4     1/12/05 6:14p Dm185016
// TAR 289897.
// 
// 3     1/10/05 9:32a Rm124623
// Fix TAR 288199:  ji/32e54 - text printing on rap e-receipt not reading
// the \n as line feeds
// 
// 2     11/30/04 4:32p Dm185016
// Added tracing.
// 
// 1     10/29/04 9:18a Dm185016
*/
#include "StdAfx.h"
#include "RAPReceipt.h"
#include "RCMLaneXML.h"
#include "RemoteAPMgr.h"
#include "PSXMFC.h"
#include "PSXError.h"
#include "verifypsx.h"

using namespace fastlane;
using namespace rap;


CReceiptItem::CReceiptItem(HTRACE hTrace)
    : CBaseObject(hTrace)
{   
    CString sItemID;
    sItemID.Format(_T("RCM%d"), rapmgr().GetNextItem());

    m_sId = sItemID.GetBuffer();

    ITRACE_DEVELOP(_T("Adding item id:  ") + IString(m_sId.c_str()));
}

CRemoteAPMgr &CReceiptItem::rapmgr() { return *CSingleton<CRemoteAPMgr>::instance(); }
CPSX &CReceiptItem::psx() { return rapmgr().psx(); }


const _TCHAR* NEWLINE = _T("\n");
const _TCHAR* TAB = _T("\t");
CMessageReceiptItem::CMessageReceiptItem(HTRACE hTrace, const _TCHAR *pszMsg)
    : CReceiptItem(hTrace)
{
    m_sMsg = StringType(pszMsg);
    size_t pos = 0;

    // Remove leading whitespace
    pos = m_sMsg.find_first_not_of(_T(" \t\n"));
    if (pos != 0)
        m_sMsg.erase(0, pos);

    // Convert "\n" to newline character
    while (pos != StringType::npos)
    {
        pos = m_sMsg.find(_T("\\n"),pos);
        if (pos != StringType::npos)
        {
            m_sMsg.replace(pos,2,NEWLINE);
        }
    }
    
    // Convert "\t" to tab characters
    while (pos != StringType::npos)
    {
        pos = m_sMsg.find(_T("\\t"),pos);
        if (pos != StringType::npos)
        {
            m_sMsg.replace(pos,2,TAB);
        }
    }
}


HRESULT CMessageReceiptItem::Create(const StringType &szSource, const StringType &szParentControlName)
{
    HRESULT hr = S_OK;

    CString sParentId = _T("");

    if (szParentControlName != _T(""))
    {
        VERIFY_PSX(hr, psx().GetSelectedReceiptItem(szParentControlName.c_str(), sParentId) );
    }
    ITRACE_DEVELOP(_T(__FUNCTION__) 
                   _T("  Item Id:  ") + IString((const _TCHAR *)GetId()) + 
                   _T("  Parent Item Id:  ") + IString((const _TCHAR *)sParentId));

    VERIFY_PSX(hr, psx().CreateReceiptItem( szSource.c_str(), GetId(), sParentId ));

    return hr;
}


HRESULT CMessageReceiptItem::AddToReceipt(const StringType &szSource)
{
    HRESULT hr = S_OK;

    ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  szSource:  ") + IString(szSource.c_str())
                 + _T(".  Text:  \"") + IString(m_sMsg.c_str()) + _T("\""));

	VERIFY_PSX(hr, psx().SetReceiptItemVariable( szSource.c_str()
                                           , GetId()
                                           , RECEIPT_VAR_ITEM_SELECTABLE
                                           , VARIANT_FALSE));
    _variant_t varText = m_sMsg.c_str();
    VERIFY_PSX(hr, psx().SetReceiptItemVariable( szSource.c_str()
                                           , GetId()
                                           , RECEIPT_VAR_ITEM_MSG
                                           , varText ));

    return hr;
}
