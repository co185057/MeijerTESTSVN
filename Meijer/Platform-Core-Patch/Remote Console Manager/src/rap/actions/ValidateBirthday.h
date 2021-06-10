#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ValidateBirthday.h 3     10/21/08 4:08p Sm133076 $
/*
 * @(#)ValidateBirthday.h    $Revision: 3 $ $Date: 10/21/08 4:08p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ValidateBirthday.h $
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
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

class CValidateBirthday
    : public CRCMActionBase
    , public CActionBaseRAP
    , public CCreateable<CValidateBirthday, IAction>
//    , public CPool<CValidateBirthday>
{

public:

    CValidateBirthday();
	virtual ~CValidateBirthday();
    
    virtual HRESULT Execute(bool *, IMessage &);

private:

    // hide copy and assignment
	CValidateBirthday(const CValidateBirthday&);
	CValidateBirthday& operator = (const CValidateBirthday &);
};
