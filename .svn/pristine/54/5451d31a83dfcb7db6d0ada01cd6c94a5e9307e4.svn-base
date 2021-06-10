#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/HideControl.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)HideControl.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/HideControl.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     1/26/05 2:23p Dm185016
 * Moved to new Repository.
* 
* 1     10/29/04 9:04a Dm185016
*/

#include "SMConstants.h"
#include "Createable.h"
#include "PSXAction.h"
#include "KeyValueHandler.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

class CHideControl
    //: public CRCMKVActionBase<CTransactionBegin>
    : public CPSXAction
    , public CCreateable<CHideControl, IAction>
//    , public CPool<CHideControl>
{

public:

    CHideControl();
	virtual ~CHideControl();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    HRESULT ParseOperands();
    HRESULT NextOperands(CParameterList::iterator &iter, CParameterList::const_iterator &end);

private:

    // hide copy and assignment
	CHideControl(const CHideControl&);
	CHideControl& operator = (const CHideControl &);

    StringType m_sControl;
};
