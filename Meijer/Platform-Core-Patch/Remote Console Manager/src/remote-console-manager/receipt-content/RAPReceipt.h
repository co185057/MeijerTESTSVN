#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/remote-console-manager/receipt-content/RAPReceipt.h 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)RAPReceipt.h    $Revision: 1 $ $Date: 6/20/08 1:11p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/remote-console-manager/receipt-content/RAPReceipt.h $
 * 
 * 1     6/20/08 1:11p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:25p Dm185016
 * Moved to new Repository.
* 
* 2     1/12/05 6:14p Dm185016
* TAR 289897.
* 
* 1     10/29/04 9:18a Dm185016
*/

class CPSX;

namespace rap
{

class CRemoteAPMgr;

class CReceiptItem
    : public CBaseObject
{
public:

    CReceiptItem(HTRACE hTrace);
    virtual ~CReceiptItem() {};

    virtual HRESULT Create(const StringType &sSource, const StringType &szParentControlName = _T("") )=0;
    virtual HRESULT AddToReceipt(const StringType &sSource)=0;

protected:

    const _TCHAR *GetId() { return m_sId.c_str(); }

    CRemoteAPMgr &rapmgr();
    CPSX &psx();

private:

    // hide copy and assignment
	CReceiptItem(const CReceiptItem&);
	CReceiptItem& operator = (const CReceiptItem &);

    StringType m_sId;
};


class CMessageReceiptItem
    : public CReceiptItem
{
public:

    CMessageReceiptItem(HTRACE hTrace, const _TCHAR *pszMsg);

    virtual ~CMessageReceiptItem() {};

    HRESULT Create(const StringType &sSource, const StringType &szParentControlName = _T("") );
    HRESULT AddToReceipt(const StringType &sSource);

private:

    // hide copy and assignment
	CMessageReceiptItem(const CMessageReceiptItem&);
	CMessageReceiptItem& operator = (const CMessageReceiptItem &);

    StringType m_sMsg;
};


}
