#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/PSXAction.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)PSXAction.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/PSXAction.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     2/28/05 4:38p Dm185016
 * lint
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
* 
* 9     1/21/05 4:44p Dm185016
* TAR 291218 - Added parameter to EnableControl.  If true then
* ALL_CONTEXTS used as context name.
* 
* 8     1/17/05 5:55p Tp151000
* Show Total and Item in sign off state.
* 
* 7     1/12/05 6:07p Dm185016
* TAR 289897
* 
* 6     1/05/05 1:57p Tk124825
* TAR 287909 - Changed ClearReceipt() to take 5 parameters in order to
* make total & item controls visible or non-visible. 
* 
* 5     12/31/04 9:18a Dm185016
* TAR 288789 - Added override accepting const _TCHAR * for
* SetTransactionVariable.
* 
* 4     12/16/04 10:10a Dm185016
* Parameter list now a KV list.
* 
* 3     11/30/04 4:31p Dm185016
* Fixed receipt source.
* 
* 2     11/16/04 11:40a Dm185016
* Added transaction variable support
* 
* 1     10/29/04 9:04a Dm185016
*/

#include "RCMActionBase.h"
#include "KeyValueHandler.h"
#include <map>

namespace rap
{
    class CReceiptItem;
}
using namespace rap;

namespace fastlane
{

class CPSXAction
	: public CRCMActionBase
{

public:

    CPSXAction();
	virtual ~CPSXAction();

    /// <summary>
    /// Clears the receipt control.
    /// </summary>
    HRESULT ClearReceipt(const _TCHAR *pszItemCountTextControlName = _T(""),const _TCHAR *pszTotalAmountTextControlName = _T(""),const _TCHAR *pszItemCountStaticTextControlName = _T(""),const _TCHAR *pszTotalAmountStaticTextControlName = _T(""), const bool bVisible = false);

    /// <summary>
    /// Adds receipt line to the receipt control specified in the action parameters.
    /// </summary>
    HRESULT AddReceiptLine(CReceiptItem &, bool, bool bUseParentItem = true);

    /// <summary>
    /// Enables/Disables the given control.
    /// </summary>
	HRESULT EnableControl(const StringType &controlName, bool bEnable, bool bUseAllContexts = true);

    /// <summary>
    /// Makes the given control visible/invisible.
    /// </summary>
	HRESULT SetControlVisible(const StringType &controlName, bool bShow, bool bUsedCurrentContext = true);

    /// <summary>
    /// Sets the value of the given transaction variable.
    /// </summary>
	HRESULT SetTransactionVariable(const _TCHAR *pszVariableName, COleVariant& varValue);
	HRESULT SetTransactionVariable(const _TCHAR *pszVariableName, const _TCHAR *pszValue);

    /// <summary>
    /// Returns the value of the given translated string as maintained within the PSX
    /// string tables.
    /// </summary>
    CString GetString(const _TCHAR *pszStringTableId);

protected:

    virtual HRESULT ParseOperands()=0;
    virtual HRESULT NextOperands(CParameterList::iterator &iter, CParameterList::const_iterator &end);

    const _TCHAR *GetDataSource() const { return m_sDataSource.c_str(); };

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(PSXParms)
        DECLARE_KV_CSTRING(DataSource)
        DECLARE_KV_CSTRING(ReceiptControlName)
    DECLARE_KV_HANDLER_CLASS_END

private:

    // hide copy and assignment
	CPSXAction(const CPSXAction&);
	CPSXAction& operator = (const CPSXAction &);

    PSXParms m_parms;       //lint !e1516

    StringType m_sDataSource;
};

}
