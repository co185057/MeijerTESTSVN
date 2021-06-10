#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/CheckDefaultButton.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)CheckDefaultButton.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/CheckDefaultButton.h $
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
* 1     10/29/04 8:53a Dm185016
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RCMKVActionBase.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

class CCheckDefaultButton
    : public CRCMKVActionBase<CCheckDefaultButton>
    , public CCreateable<CCheckDefaultButton, IAction>
//    , public CPool<CCheckDefaultButton>
{

public:

    CCheckDefaultButton();
	virtual ~CCheckDefaultButton();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    HRESULT ParseOperands();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_MAP();

    DECLARE_KV_HANDLER(OnView);

private:

    // hide copy and assignment
	CCheckDefaultButton(const CCheckDefaultButton&);
	CCheckDefaultButton& operator = (const CCheckDefaultButton &);

    int m_nView;

	StringType m_sViewName;
    StringType m_sVariableName;

    HRESULT m_hr;
};
