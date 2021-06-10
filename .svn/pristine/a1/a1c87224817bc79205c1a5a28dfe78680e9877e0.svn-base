#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/ActionExpressionImpl.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)ActionExpressionImpl.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
 *
 * Copyright 2005 by NCR Corporation,
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/ActionExpressionImpl.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 2     4/05/05 12:03p Dm185016
 * TAR 297356 - Added plist for action expression in <if>
 * 
 * 1     3/25/05 10:49a Dm185016
 * Actions as Expression support
*/
#include "ExpressionImpl.h"

namespace STATE_MACHINE_NAMESPACE
{

class IMessage;
class IAction;
class CAbstractStateMachine;

class CActionExpressionImpl
    : public CExpressionImpl
{
public:

    CActionExpressionImpl(HTRACE, int, const _TCHAR *, const _TCHAR *);
    virtual ~CActionExpressionImpl(void);

    virtual bool Evaluate();

    virtual CActionExpressionImpl &SetMessage(IMessage *);

    virtual CActionExpressionImpl &SetStateMachine(CAbstractStateMachine *);

protected:

    IMessage &message();
    IAction &action();
    CAbstractStateMachine &stateMachine();

private:

    CActionExpressionImpl(CActionExpressionImpl &);                     // Hide the Copy construcor
    CActionExpressionImpl &operator =(const CActionExpressionImpl &);   // Hide the assignment operator

    IMessage *m_pMsg;
    IAction *m_pAction;
    StringType m_szActionName;
    StringType m_szPList;
    CAbstractStateMachine *m_pSM;
};

}