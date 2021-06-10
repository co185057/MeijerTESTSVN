#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/remote-console-manager/receipt-content/TopListItem.h 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)TopListItem.h    $Revision: 1 $ $Date: 6/20/08 1:11p $
 *
 * Copyright 2003 by NCR Corporation,
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/remote-console-manager/receipt-content/TopListItem.h $
 * 
 * 1     6/20/08 1:11p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:25p Dm185016
 * Moved to new Repository.
* 
* 2     1/12/05 6:15p Dm185016
* TAR 289897.
* 
* 1     10/29/04 9:18a Dm185016
*/

#include "RAPReceipt.h"

class CPSX;

namespace rap
{


class CTopListItem
    : public CReceiptItem
{
public:

    CTopListItem(HTRACE hTrace, const _TCHAR *pszMsg);

    virtual ~CTopListItem() {};

    HRESULT Create(const StringType &sSource, const StringType &szParentControlName = _T("") );
    HRESULT AddToReceipt(const StringType &sSource);

private:

    // hide copy and assignment
	CTopListItem(const CTopListItem&);
	CTopListItem& operator = (const CTopListItem &);

    StringType m_sMsg;
};


}
