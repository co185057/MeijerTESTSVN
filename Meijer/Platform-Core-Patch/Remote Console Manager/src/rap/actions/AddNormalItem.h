#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/AddNormalItem.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)AddNormalItem.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/AddNormalItem.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 8     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 6     1/26/06 5:51p Dm185016
 * TAR 316749
 * 
 * 4     1/13/06 2:27p Dm185016
 * TAR 301329
 * 
 * 1     3/11/05 2:39p Dm185016
 * 
 * 3     2/28/05 4:29p Dm185016
 * lint
 * 
 * 2     2/23/05 4:29p Tp151000
 * 293998
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
* 
* 6     1/20/05 10:38a Dm185016
* TAR 290434
* 
* 5     1/12/05 5:57p Dm185016
* TAR 289987.
* 
* 4     12/14/04 11:25a Dm185016
* TAR 288478.  Factored out common code.  AddNormalItem now inherits from
* DescriptionParser.
* 
* 3     12/07/04 4:20p Dm185016
* Corrected lane state window(x) display
* 
* 2     10/29/04 9:07a Dm185016
* Moved PSX fxns to PSXAction base class
* 
* 1     10/14/04 1:40p Dm185016
*/

/*lint -save -e1516 */

#include "SMConstants.h"
#include "Createable.h"
#include "PSXAction.h"
#include "KeyValueHandler.h"
#include "DisplayControlParser.h"
#include "RAEvents.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

const _TCHAR SPECIAL_TEXT_ALERT[]               = RAP_RECEIPT_LINE_ALERT;
const _TCHAR SPECIAL_TEXT_NUM_ITEMS[]           = _T("[NumItems]"); 
const _TCHAR SPECIAL_TEXT_DELAY_COUNT[]         = _T("[DelayCount]");
const _TCHAR SPECIAL_TEXT_HIGHLIGHT_EX[]        = RAP_RECEIPT_LINE_HIGHLIGHT;
const _TCHAR SPECIAL_TEXT_HIGHLIGHT_SUBITEM_EX[]= RAP_RECEIPT_LINE_HIGHLIGHT_SUBITEM;
const _TCHAR SPECIAL_TEXT_SUMMARY_EX[]          = RAP_RECEIPT_LINE_SUMMARY;
const _TCHAR SPECIAL_TEXT_HIGHLIGHT[]           = RAP_RECEIPT_LINE_HIGHLIGHT_OLD;
const _TCHAR SPECIAL_TEXT_SUMMARY[]             = RAP_RECEIPT_LINE_SUMMARY_OLD;
const _TCHAR SPECIAL_TEXT_TOTAL[]               = _T("[Total]");
const _TCHAR SPECIAL_TEXT_ERROR[]               = _T("[Error]");

const _TCHAR ALERT_POS_MESSAGE[]                = _T("POS Message:");

const _TCHAR NULL_TEXT[] = _T("");

const size_t SPECIAL_TEXT_ALERT_LENGTH          = _tcslen(SPECIAL_TEXT_ALERT);
const size_t SPECIAL_TEXT_NUM_ITEMS_LENGTH      = _tcslen(SPECIAL_TEXT_NUM_ITEMS);
const size_t SPECIAL_TEXT_TOTAL_LENGTH          = _tcslen(SPECIAL_TEXT_TOTAL);
const size_t SPECIAL_TEXT_SUMMARY_EX_LENGTH     = _tcslen(SPECIAL_TEXT_SUMMARY_EX);
const size_t SPECIAL_TEXT_HIGHLIGHT_EX_LENGTH   = _tcslen(SPECIAL_TEXT_HIGHLIGHT_EX);
const size_t SPECIAL_TEXT_HIGHLIGHT_SUBITEM_EX_LENGTH   = _tcslen(SPECIAL_TEXT_HIGHLIGHT_SUBITEM_EX);
const size_t SPECIAL_TEXT_SUMMARY_LENGTH        = _tcslen(SPECIAL_TEXT_SUMMARY);
const size_t SPECIAL_TEXT_HIGHLIGHT_LENGTH      = _tcslen(SPECIAL_TEXT_HIGHLIGHT);

class CAddNormalItem
    : public CDisplayControlParser
    , public CCreateable<CAddNormalItem, IAction>
//    , public CPool<CAddNormalItem>
{


public:

    CAddNormalItem();
	virtual ~CAddNormalItem();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    ///<summary>Obtains all the controls names from the xml file.</summary>
    HRESULT ParseOperands();
    HRESULT ParseSpecial(IMessage &);
    HRESULT ParseAlert(const StringType &, IMessage &);

    const _TCHAR *GetDescription() const;

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(ANIParms)
        DECLARE_KV_CSTRING(Description)
        DECLARE_KV_CSTRING(LaneButtonControlName)
        DECLARE_KV_CSTRING(LaneStatusButtonControlName)
        DECLARE_KV_CSTRING(AssistModeButtonControlName)
        DECLARE_KV_CSTRING(ItemCountTextControlName)
        DECLARE_KV_CSTRING(TotalAmountTextControlName)
    DECLARE_KV_HANDLER_CLASS_END

    ANIParms m_parms;           

private:

    // hide copy and assignment
	CAddNormalItem(const CAddNormalItem&);
	CAddNormalItem& operator = (const CAddNormalItem &);
};

/*lint -restore */
