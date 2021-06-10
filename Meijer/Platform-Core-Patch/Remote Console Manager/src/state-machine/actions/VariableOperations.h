#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/VariableOperations.h 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)VariableOperations.h    $Revision: 1 $ $Date: 6/20/08 1:11p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/VariableOperations.h $
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
* 5     4/16/04 4:18p Dm185016
* Fixed assign function
* 
* 4     2/10/04 11:46a Dm185016
* Moved common support to base
* 
* 3     1/16/04 9:17a Dm185016
* Namespace support, Increment and Decrement operations
*/

#include "SMConstants.h"
#include "Createable.h"
#include "ActionBase.h"
#include "Pool.h"
#include "Variable.h"
#include "Property.h"

namespace STATE_MACHINE_NAMESPACE
{

class CAbstractStateMachine;
class IMessage;

class CVariableOperations
    : public CActionBase
    , public CCreateable<CVariableOperations, IAction>
    //, public CPool<CVariableOperations>
{

public:

    enum Operations
    {
        NO_OP  = 0,
        INCREMENT  = 1,
        DECREMENT,
        EVALUATE,
        ASSIGN
    };

    CVariableOperations();
	virtual ~CVariableOperations();
    
    virtual HRESULT Execute(bool *, IMessage &);

    const _TCHAR *GetComment(void);

    void SetCounter(IVariable *);

    bool IsInternal();

protected:

    HRESULT ParseOperands();

    HRESULT ParseOperation();

    bool IncrementVariable();
    bool DecrementVariable();
    bool AssignVariable();

    IVariable &variable();
    IExpression &expression();

private:

    Operations m_nOperation;
    int m_nVariable;

    // hide copy and assignment
	CVariableOperations(const CVariableOperations&);
	CVariableOperations& operator = (const CVariableOperations &);

    IVariable *m_pVariable;
    IExpression *m_pExpression;
};

};