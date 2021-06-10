// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/RuleImpl.cpp 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)RuleImpl.cpp    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/RuleImpl.cpp $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 2     3/11/05 1:20p Dm185016
 * <select> support
 * 
 * 1     1/26/05 2:29p Dm185016
 * Moved to new Repository.
// 
// 9     1/18/05 3:58p Dm185016
// UNICODE support
// 
// 8     1/12/05 6:31p Dm185016
// Select support.
// 
// 7     12/22/04 11:23a Dm185016
// Factored out base functionality of RuleImpl into RuleImplBase.
// 
// 6     11/15/04 11:43a Dm185016
// Added TrueInstance and FalseInstance for select/if
// 
// 4     3/19/04 3:29p Dm185016
// Application state support
// 
// 3     1/27/04 2:04p Dm185016
// Action Library support
// 
// 2     1/16/04 9:20a Dm185016
// Namespace support, Action parameters
*/
#include "StdAfx.h"
#include "RuleImpl.h"
#include "RulesHandler.h"
#include "Expression.h"

using namespace STATE_MACHINE_NAMESPACE;

int CRuleImplBase::counter = 1;


CRuleImpl::CRuleImpl(void)
{
}


CRuleImpl::~CRuleImpl(void)
{
}


_RuleTypes CRuleImpl::GetRuleType() const { return SimpleRule; }


CRuleImplBase::CRuleImplBase(void)
	: nId(++counter)
    , m_pExpr(NULL)
{
}


CRuleImplBase::~CRuleImplBase(void)
{
    //-------------------------------------------------------------------------
    // Do not want to delete this pointer because it is passed off to the 
    // XMLRule implementation
    //-------------------------------------------------------------------------
    //delete m_pExpr;
}


DEFINE_STRING_PROPERTY(CRuleImplBase, Comment);
DEFINE_STRING_PROPERTY(CRuleImplBase, MsgName);
DEFINE_STRING_PROPERTY(CRuleImplBase, ActionName);
DEFINE_STRING_PROPERTY(CRuleImplBase, StateName);
DEFINE_STRING_PROPERTY(CRuleImplBase, EndStateName);
DEFINE_STRING_PROPERTY(CRuleImplBase, FalseActionName);
DEFINE_STRING_PROPERTY(CRuleImplBase, FalseStateName);
DEFINE_STRING_PROPERTY(CRuleImplBase, FailureActionName);
DEFINE_STRING_PROPERTY(CRuleImplBase, FailureStateName);
DEFINE_INT_PROPERTY(CRuleImplBase, SequenceNumber);


HRESULT CRuleImplBase::SaveActionParm(const StringType &szValue)
{
    m_vActionParms.push_back(szValue);

    return S_OK;
}


HRESULT CRuleImplBase::SaveFalseParm(const StringType &szValue)
{
    m_vFalseParms.push_back(szValue);

    return S_OK;
}


HRESULT CRuleImplBase::SaveFailureParm(const StringType &szValue)
{
    m_vFailureParms.push_back(szValue);

    return S_OK;
}


StringType CRuleImplBase::GetXMLOutput(CXMLBuilder *pXMLBuilder)
{
    if (pXMLBuilder == NULL)
        ((CXMLBuilder *)CRulesHandler::instance())->GetXMLOutput(this);
    else
        pXMLBuilder->GetXMLOutput(this);

    return (pXMLBuilder == NULL ? CRulesHandler::instance() : pXMLBuilder)->GetXMLOutput();
}


IExpression *CRuleImplBase::GetExpression()
{
    return m_pExpr;
}


void CRuleImplBase::SetExpression(IExpression *pExpr)
{
    m_pExpr = pExpr;
}


CRuleQ &CRuleImplBase::GetRulesList() { return m_vRules; }


int CRuleImplBase::GetId() const
{
	return nId;
}


const _TCHAR *CRuleImplBase::GetComment()
{
	return Comment.Value().c_str();
}


const _TCHAR *CRuleImplBase::GetMsgName()
{
	return MsgName.Value().c_str();
}


const _TCHAR *CRuleImplBase::GetActionName()
{
	return ActionName.Value().c_str();
}


CParameterList &CRuleImplBase::GetActionParms()
{
	return m_vActionParms;
}


const _TCHAR *CRuleImplBase::GetStateName()
{
	return StateName.Value().c_str();
}


const _TCHAR *CRuleImplBase::GetEndStateName()
{
	return EndStateName.Value().c_str();
}


const _TCHAR *CRuleImplBase::GetFalseActionName()
{
	return FalseActionName.Value().c_str();
}


const _TCHAR *CRuleImplBase::GetFalseStateName()
{
	return FalseStateName.Value().c_str();
}


CParameterList &CRuleImplBase::GetFalseParms()
{
	return m_vFalseParms;
}


const _TCHAR *CRuleImplBase::GetFailureActionName()
{
	return FailureActionName.Value().c_str();
}


const _TCHAR *CRuleImplBase::GetFailureStateName()
{
	return FailureStateName.Value().c_str();
}


CParameterList &CRuleImplBase::GetFailureParms()
{
	return m_vFailureParms;
}


int CRuleImplBase::GetSequenceNumber()
{
	return SequenceNumber.Value();
}


StringType CRuleImplBase::ToString()
{
    IStringBuffer sbRetValue(125);
    sbRetValue += _T("Rule:  ");
    sbRetValue += GetStateName();
    sbRetValue += _T(",");
    sbRetValue += GetMsgName();
    sbRetValue += _T(",");
    FormatAction(_T(""), GetActionName(), m_vActionParms, sbRetValue);
    sbRetValue += _T(",");
    FormatAction(_T("False:"), GetFalseActionName(), m_vFalseParms, sbRetValue);
    sbRetValue += _T(",");
    FormatAction(_T("Failure:"), GetFailureActionName(), m_vFailureParms, sbRetValue);

    StringType sRetValue = (const _TCHAR *)sbRetValue;

    return sRetValue;
}


void CRuleImplBase::FormatAction(const _TCHAR *pszDesc, const _TCHAR *pszName, const CParameterList &plist, IStringBuffer &sbRetValue)
{
    if (_tcslen(pszName) == 0)
        return;

    sbRetValue += pszDesc;
    sbRetValue += pszName;

    FormatParmaterList(plist, sbRetValue);
}


void CRuleImplBase::FormatParmaterList(const CParameterList &plist, IStringBuffer &sbValue)
{
    if (plist.empty())
        return;

    sbValue += _T("(");
    CParameterList::const_iterator iter = plist.begin();
    for ( ;iter!=plist.end(); iter++)
    {
        sbValue += (*iter).c_str();
    }
    sbValue += _T(")");
}
