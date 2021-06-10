// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/ActionExpressionImpl.cpp 1     6/20/08 1:12p Sa250050 $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/ActionExpressionImpl.cpp $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 5     5/03/05 4:06p Dm185016
 * Cleaned up tracing
 * 
 * 3     4/05/05 12:03p Dm185016
 * TAR 297356 - Added plist for action expression in <if>
 * 
 * 2     3/28/05 5:10p Dm185016
 * TAR 296771
 * 
 * 1     3/25/05 10:49a Dm185016
 * Actions as Expression support
*/
#include "StdAfx.h"
#include "ActionExpressionImpl.h"
#include "Action.h"
#include "ActionBase.h"
#include "AbstractStateMachine.h"
#include "Expression.h"
#include "XError.h"
#include "SMError.h"

using namespace STATE_MACHINE_NAMESPACE;


//-----------------------------------------------------------------------------
// CActionExpressionImpl
//-----------------------------------------------------------------------------
CActionExpressionImpl::CActionExpressionImpl(HTRACE hTrace
                                           , int nId
                                           , const _TCHAR *pcszActionName
                                           , const _TCHAR *pcszPList)
    : CExpressionImpl(nId)    
    , m_pMsg(NULL)
    , m_pAction(NULL)
    , m_szActionName(pcszActionName)
    , m_szPList(pcszPList)
{
    SetTraceObject(hTrace);
}


CActionExpressionImpl::~CActionExpressionImpl(void)
{
}


bool CActionExpressionImpl::Evaluate()
{
    IMessage *pMessage;
    HRESULT hr = stateMachine().GetCurrentMsg(&pMessage);
    if (hr != S_OK)
        ITHROWSMERROR(hr)

    bool bRetValue = false;

    // Set up the parameters for the action
    CParameterList parmlist;
    parmlist.push_back(m_szPList);

    if (!m_szPList.empty())
        ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  Setting parameter list:  ") + IString(m_szPList.c_str()));

    IAction &theAction = action();

    CActionBase &actionBase = dynamic_cast<CActionBase &>(theAction);
    actionBase.SetParameters(parmlist); 

    // Execute the action.  All we want is the result:  T or F
    hr = theAction.Execute(&bRetValue, *pMessage);
    if (hr != S_OK)
        ITHROWSMERRORTEXT(hr, _T(__FUNCTION__) _T(".  Failure during expression evaluation."));

    ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  Expression evaluates to ") + IString(bRetValue));
    
    return bRetValue;
}


CActionExpressionImpl &CActionExpressionImpl::SetMessage(IMessage *pMsg)
{
    m_pMsg  = pMsg;

    return *this;
}


IMessage &CActionExpressionImpl::message()
{
    _ASSERTE(m_pMsg != NULL);

    return *m_pMsg;
}


CActionExpressionImpl &CActionExpressionImpl::SetStateMachine(CAbstractStateMachine *pSM)
{
    m_pSM  = pSM;

    return *this;
}


CAbstractStateMachine &CActionExpressionImpl::stateMachine()
{
    _ASSERTE(m_pSM != NULL);

    return *m_pSM;
}


IAction &CActionExpressionImpl::action()
{
    if(m_pAction == NULL)
    {
        HRESULT hr = stateMachine().GetAction(&m_pAction, m_szActionName.c_str());
        if (hr != S_OK)
            ITHROWSMERROR(hr)
        try
        {
            IEvaluateable *pEval = dynamic_cast<IEvaluateable *>(m_pAction);
            if (pEval == NULL)
                ITHROWSMERROR(STATEM_INVALID_EXPRESSION_ACTION)
        }
        catch (__non_rtti_object e)
            ITHROWSMERROR(STATEM_INVALID_EXPRESSION_ACTION) 
    }

    return *m_pAction;
}
