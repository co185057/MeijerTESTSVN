#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/EvaluateExpression.h 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)EvaluateExpression.h    $Revision: 1 $ $Date: 6/20/08 1:11p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/EvaluateExpression.h $
 * 
 * 1     6/20/08 1:11p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 2     2/10/05 5:48p Dm185016
 * TAR 292915
 * 
 * 1     1/26/05 2:28p Dm185016
 * Moved to new Repository.
* 
* 3     10/29/04 9:26a Dm185016
* Removed reference to MessageObject.
* 
* 1     2/10/04 11:47a Dm185016
* New actions for evaluating expressions
*/

#include "SMConstants.h"
#include "Createable.h"
#include "ActionBase.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE
{

class IMessage;
class IExpression;

class CEvaluateExpression
    : public CActionBase
    , public CCreateable<CEvaluateExpression, IAction>
{

public:
	CEvaluateExpression();
	virtual ~CEvaluateExpression();
    
    virtual HRESULT Execute(bool *, IMessage &);

    virtual bool IsInternal();

private:

    // hide copy and assignment
	CEvaluateExpression(const CEvaluateExpression&);
	CEvaluateExpression& operator = (const CEvaluateExpression &);
    
    HRESULT ParseOperands();

    IExpression *m_pExpression;
};

};
