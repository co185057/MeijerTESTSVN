#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisplayAgeRestrictionInstructionText.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)DisplayAgeRestrictionInstructionText.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisplayAgeRestrictionInstructionText.h $
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
#include "DisplayInstructionText.h"
#include "RCMActionBase.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

using namespace fastlane;
using namespace rap;

class CDisplayAgeRestrictionInstructionText
    : public CDisplayInstructionText
    , public CCreateable<CDisplayAgeRestrictionInstructionText, IAction>
//    , public CPool<CDisplayAgeRestrictionInstructionText>
{

public:

    CDisplayAgeRestrictionInstructionText();
	virtual ~CDisplayAgeRestrictionInstructionText();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    virtual HRESULT FormatInstructionText(StringType &, const _TCHAR *);

private:

    // hide copy and assignment
	CDisplayAgeRestrictionInstructionText(const CDisplayAgeRestrictionInstructionText&);
	CDisplayAgeRestrictionInstructionText& operator = (const CDisplayAgeRestrictionInstructionText &);

    StringType m_sText;
};
