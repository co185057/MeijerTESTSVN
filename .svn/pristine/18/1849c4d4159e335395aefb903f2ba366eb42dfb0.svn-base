#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SetTransactionState.h 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)SetTransactionState.h    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/SetTransactionState.h $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     3/11/05 2:45p Dm185016
 * 
 * 2     3/01/05 2:03p Dm185016
 * TAR 294587.  Added application state functionality.
 * 
 * 1     1/26/05 2:24p Dm185016
 * Moved to new Repository.
* 
* 1     10/04/04 5:36p Dm185016
* New for 3.2 Patch E/4.0
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RCMActionBase.h"
#include "ActionBaseRAP.h"
#include "KeyValueHandler.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

namespace rap
{

class CApplicationState;

class CSetTransactionState
    : public CRCMActionBase
    , public CActionBaseRAP
    , public CCreateable<CSetTransactionState, IAction>
//    , public CPool<CSetTransactionState>
{

public:

    CSetTransactionState();
	virtual ~CSetTransactionState();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    virtual void ExitCurrentState();
    virtual void EnterNewState();
    
    virtual void EnableAssistModeButton(bool);

    CApplicationState &GetAppState() const throw();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(STSParms)
        DECLARE_KV_CSTRING(State)
    DECLARE_KV_HANDLER_CLASS_END

    STSParms m_parms;

private:

    // hide copy and assignment
	CSetTransactionState(const CSetTransactionState&);
	CSetTransactionState& operator = (const CSetTransactionState &);

    CApplicationState *m_pState;
};

}