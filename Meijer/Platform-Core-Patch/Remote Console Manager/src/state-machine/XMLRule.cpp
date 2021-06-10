// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/XMLRule.cpp 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)XMLRule.cpp    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
*  $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/XMLRule.cpp $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 6     3/28/05 5:06p Dm185016
 * TAR 296771
 * 
 * 5     3/22/05 5:02p Dm185016
 * UNICODE
 * 
 * 4     3/11/05 1:35p Dm185016
 * <select> support
 * 
 * 3     2/28/05 3:54p Dm185016
 * lint
 * 
 * 2     2/10/05 5:42p Dm185016
 * TAR 292915
 * 
 * 1     1/26/05 2:28p Dm185016
 * Moved to new Repository.
// 
// 6     1/12/05 6:29p Dm185016
// Select support.
// 
// 5     12/22/04 11:20a Dm185016
// Added conditional logic support.
// 
// 4     11/15/04 11:45a Dm185016
// select/if support
// 
// 2     1/16/04 9:13a Dm185016
// Namespace, Action parameters
*  
*  1     1/06/04 8:58a Dm185016
*  First version
*/
#include "StdAfx.h"
#include "XMLRule.h"
#include "Expression.h"
#include "xerror.h"

using namespace STATE_MACHINE_NAMESPACE;

/**
 * This class defines the access methods for a rule.
 *
 * @author  David Miller
 * @version $Revision: 1 $
 * @since   JDK1.3
 */
CXMLRule::CXMLRule()
    : m_nMsgID(0)
    , m_nStateID(0)
    , m_nActionID(0)
    , m_nSeqNumber(0)
    , m_nEndStateID(0)
    , m_nFalseStateID(0)
    , m_nFalseActionID(0)
    , m_nFailureStateID(0)
    , m_nFailureActionID(0)
    , m_pExpr(NULL)
    , m_nNextRuleIndex(-1)
{
}


CXMLRule::~CXMLRule()
{
    // Expression deleted by the factory.
    m_pExpr = NULL;
}


//lint --e{774}
IExpression &CXMLRule::GetExpression()
{
    _ASSERT(m_pExpr!=NULL);
    if (m_pExpr==NULL)
        ITHROWNEW(IException, _T("Missing expression in rule"));

    return *m_pExpr;
}


void CXMLRule::SetExpression(IExpression *pExpr)
{
    _ASSERTE(pExpr!=NULL);
    delete m_pExpr;
    m_pExpr = pExpr;
}


bool CXMLRule::IsActive() 
{ 
    IExpression &expr = GetExpression();
    bool bRetValue = expr.Evaluate();

    return bRetValue; 
}


const CRuleKey * CXMLRule::GetKey() const
{
    return &m_Key;
}


int CXMLRule::GetMsgID() const
{ 
    return m_nMsgID;
}


int CXMLRule::GetStateID() const
{ 
    return m_nStateID;
}


int CXMLRule::GetActionID() const
{ 
    return m_nActionID;
}


int CXMLRule::GetEndStateID() const
{ 
    return m_nEndStateID;
}


int CXMLRule::GetFalseStateID() const
{ 
    return m_nFalseStateID;
}


int CXMLRule::GetFalseActionID() const
{ 
    return m_nFalseActionID;
}


int CXMLRule::GetFailureStateID() const
{ 
    return m_nFailureStateID;
}


int CXMLRule::GetFailureActionID() const
{ 
    return m_nFailureActionID;
}


void CXMLRule::SetMsgID(int nNewID)
{
//  IMODTRACE_DEVELOP();
//  ITRACE_DEVELOP("nNewID=" + IString(nNewID));
    m_nMsgID = nNewID;

    m_Key.SetMsgID(nNewID);
}


void CXMLRule::SetStateID(int nNewID)
{
//  IMODTRACE_DEVELOP();
//  ITRACE_DEVELOP("nNewID=" + IString(nNewID));
    m_nStateID = nNewID;

    m_Key.SetStateID(nNewID);
}


void CXMLRule::SetActionID(int nNewID)
{
//  IMODTRACE_DEVELOP();
//  ITRACE_DEVELOP("nNewID=" + IString(nNewID));
    m_nActionID = nNewID;
}


CParameterList &CXMLRule::GetActionParms() { return m_vActionParms; }       //lint !e1536


CParameterList &CXMLRule::GetFalseParms() { return m_vFalseParms; }         //lint !e1536


CParameterList &CXMLRule::GetFailureParms() { return m_vFailureParms; }     //lint !e1536


void CXMLRule::SetActionParms(const CParameterList &vParms)
{
    m_vActionParms.assign(vParms.begin(), vParms.end());
}


void CXMLRule::SetSequenceNumber(int nNewID)
{
    m_nSeqNumber = nNewID;
}


void CXMLRule::SetEndStateID(int nNewID)
{
    m_nEndStateID = nNewID;
}


void CXMLRule::SetFalseStateID(int nNewID)
{
    m_nFalseStateID = nNewID;
}


void CXMLRule::SetFalseActionID(int nNewID)
{
    m_nFalseActionID = nNewID;
}


void CXMLRule::SetFalseParms(const CParameterList &vParms)
{
    m_vFalseParms.assign(vParms.begin(), vParms.end());
}


void CXMLRule::SetFailureStateID(int nNewID)
{
    m_nFailureStateID = nNewID;
}


void CXMLRule::SetFailureActionID(int nNewID)
{
    m_nFailureActionID = nNewID;
}


void CXMLRule::SetFailureParms(const CParameterList &vParms)
{
    m_vFailureParms.assign(vParms.begin(), vParms.end());
}


CRulesList &CXMLRule::GetRulesList() { return m_vRules; }              //lint !e1536
bool CXMLRule::IsRulesListActive() { return !m_vRules.empty(); }


int CXMLRule::GetNextRuleIndex() { return m_nNextRuleIndex; }
void CXMLRule::SetNextRuleIndex(int nNextRuleIndex) { m_nNextRuleIndex = nNextRuleIndex; }

