#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisplayInstructionText.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)DisplayInstructionText.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisplayInstructionText.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
* 
* 1     1/07/05 2:27p Dm185016
* TAR 287222, TAR 288789
*/

#include "SMConstants.h"
#include "Createable.h"
#include "PSXAction.h"
#include "KeyValueHandler.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;
using namespace fastlane;

class CDisplayInstructionText
    : public CPSXAction
{

public:

    CDisplayInstructionText() {};
    virtual ~CDisplayInstructionText() {};

protected:

    HRESULT ParseOperands();

    virtual const _TCHAR *GetInstructionText();

    virtual HRESULT FormatInstructionText(StringType &, const _TCHAR *)=0;

    virtual HRESULT DisplayInstructionText();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(DITParms)
        DECLARE_KV_CSTRING(InstructionTextVariable);
        DECLARE_KV_CSTRING(ContextName);
    DECLARE_KV_HANDLER_CLASS_END

private:

    // hide copy and assignment
	CDisplayInstructionText(const CDisplayInstructionText&);
	CDisplayInstructionText& operator = (const CDisplayInstructionText &);

    DITParms m_parms;
    CString m_sText;
};
