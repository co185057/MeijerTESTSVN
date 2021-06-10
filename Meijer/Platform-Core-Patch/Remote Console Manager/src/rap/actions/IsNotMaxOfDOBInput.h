#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsNotMaxOfDOBInput.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)IsNotMaxOfDOBInput.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/IsNotMaxOfDOBInput.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     2/08/05 5:31p Tp151000
 * TAR 292777
 * 
 * 1     2/08/05 11:03a Tp151000
 * 290952
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
* 
* 1     11/16/04 11:23a Dm185016
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RCMActionBase.h"
#include "ActionBaseRAP.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;
using namespace fastlane;
using namespace rap;

class CIsNotMaxOfDOBInput
    : public CRCMActionBase
    , public CActionBaseRAP
    , public CCreateable<CIsNotMaxOfDOBInput, IAction>
//    , public CPool<CIsNotMaxOfDOBInput>
{

public:

    CIsNotMaxOfDOBInput();
	virtual ~CIsNotMaxOfDOBInput();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    HRESULT ParseOperands();
    bool IsMaxValue();

private:

    // hide copy and assignment
	CIsNotMaxOfDOBInput(const CIsNotMaxOfDOBInput&);
	CIsNotMaxOfDOBInput& operator = (const CIsNotMaxOfDOBInput &);
    IVariable *m_pVariable;
};
