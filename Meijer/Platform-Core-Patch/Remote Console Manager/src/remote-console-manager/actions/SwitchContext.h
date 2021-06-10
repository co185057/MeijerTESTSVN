#pragma once
/*
 * @(#)SwitchContext.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/SwitchContext.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     1/26/05 2:24p Dm185016
 * Moved to new Repository.
* 
* 1     10/04/04 5:36p Dm185016
* New for 3.2 Patch E/4.0
*/
#include "Createable.h"
#include "RCMActionBase.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

class CSwitchContext
    : public CRCMActionBase
    , public CCreateable<CSwitchContext, IAction>
//    , public CPool<CSwitchContext>
{

public:
	CSwitchContext();
	virtual ~CSwitchContext();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    HRESULT ParseOperands();

private:

    // hide copy and assignment
	CSwitchContext(const CSwitchContext&);
	CSwitchContext& operator = (const CSwitchContext &);

    StringType m_sNewContext;
};
