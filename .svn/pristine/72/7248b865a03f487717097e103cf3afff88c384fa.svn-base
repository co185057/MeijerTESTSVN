// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/RuleHandler.cpp 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)RuleHandler.cpp    $Revision: 1 $ $Date: 6/20/08 1:12p $
 *
 * Copyright 2003 by NCR Corporation,
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/RuleHandler.cpp $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 5     4/21/05 7:27p Dm185016
 * Cleaned up tracing
 * 
 * 3     2/28/05 3:44p Dm185016
 * lint
 * 
 * 2     2/10/05 5:57p Dm185016
 * Fixed problem with static ids used in SM.
 * 
 * 1     1/26/05 2:30p Dm185016
 * Moved to new Repository.
// 
// 14    1/18/05 3:58p Dm185016
// UNICODE support
// 
// 13    1/13/05 2:15p Dm185016
// TAR 287359 - cApprove&Update string was causing error when parsed.
// RuleHandler was changed to escape the '&' to "&amp;" in the text save
// for parameters.
// 
// 12    1/12/05 6:34p Dm185016
// Select support.
// 
// 11    10/29/04 9:30a Dm185016
// 
// 10    10/04/04 6:49p Dm185016
// Trace object is no longer shared.
// 
// 8     6/30/04 3:32p Dm185016
// Added validation of action names.
// 
// 7     3/19/04 3:36p Dm185016
// Split into 2 pieces - this and RulesHandler.  This now only handles a
// single <rule> section.
// 
// 5     2/24/04 3:42p Dm185016
// Cleaned up observed problems
// 
// 4     2/10/04 11:50a Dm185016
// New action format
// 
// 3     1/27/04 1:50p Dm185016
// DLL Support
// 
// 2     1/16/04 9:26a Dm185016
// New rule format, Action parameters, Entry and Exit lists, Global state
*/
#include "StdAfx.h"
#include "Rulehandler.h"
#include "SMObjectRepository.h"
#include "SMLoader.h"
#include "SMConstants.h"
#include "SMError.h"
#include "StateImpl.h"
#include "KVParmList.h"

// Do not care about unreferenced formal parameters
#pragma warning( disable : 4100 )

using namespace STATE_MACHINE_NAMESPACE;

CRuleHandler::CRuleHandler() 
    : CXPathHandler()
    , m_eParmType(NO_PARM)
    , m_eActionDest(NO_DEST)
    , m_bRegistered(false)
{
	m_owner.first		= NULL;
	m_owner.second	= NULL;
}


void CRuleHandler::PushOwner(PRULE_BEGIN_FXN pBeginFxn, PRULE_END_FXN pEndFxn) 
{
    m_owner.first	= pBeginFxn;
    m_owner.second  = pEndFxn;

    m_owners.push(m_owner);
}


void CRuleHandler::PopOwner() 
{
    m_owners.pop();

    if (!m_owners.empty())
        m_owner = m_owners.top();
}


CRuleHandler::~CRuleHandler(void)
{
}


void CRuleHandler::registerHandlers(void)
{
    IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);

    if (!m_bRegistered)
    {
        m_bRegistered = true;
     
        SETUP_NULL_HANDLER    ( XP_RULE_STATE_MSG_ACTION );
        SETUP_DECLARED_HANDLER( XP_RULE_STATE_MSG_ACTION_NAME,                      ActionName );
        SETUP_DECLARED_HANDLER( XP_RULE_STATE_MSG_ACTION_PARM,                      Parameter );
        SETUP_DECLARED_HANDLER( XP_RULE_STATE_MSG_ACTION_PARM_VALUE,                ParmValue );
        SETUP_DECLARED_HANDLER( XP_RULE_STATE_MSG_ACTION_FAILURE_TRANS,             FailureTransition );
        SETUP_NULL_HANDLER( XP_RULE_STATE_MSG_ACTION_FAILURE_ACTION );
        SETUP_DECLARED_HANDLER( XP_RULE_STATE_MSG_ACTION_EXC_TRANS,                 ExceptionTransition );
        SETUP_NULL_HANDLER( XP_RULE_STATE_MSG_ACTION_EXC_ACTION );

        SETUP_DECLARED_HANDLER( XP_RULE_STATE_MSG_RULE_EXC_ACTION_NAME,             ExceptionAction );
        SETUP_DECLARED_HANDLER( XP_RULE_STATE_MSG_RULE_EXC_ACTION_PARM,             Parameter );
        SETUP_DECLARED_HANDLER( XP_RULE_STATE_MSG_RULE_EXC_ACTION_PARM_VALUE,       ParmValue );

        SETUP_DECLARED_HANDLER( XP_RULE_STATE_MSG_RULE_FAILURE_ACTION_NAME,         FailureAction );
        SETUP_DECLARED_HANDLER( XP_RULE_STATE_MSG_RULE_FAILURE_ACTION_PARM,         Parameter );
        SETUP_DECLARED_HANDLER( XP_RULE_STATE_MSG_RULE_FAILURE_ACTION_PARM_VALUE,   ParmValue );
    }
}


DEFINE_START_ELEMENT(CRuleHandler)
{
    HRESULT hr = (*m_owner.first)((CRuleImplBase**)&m_pContent);
    return hr;
}
DEFINE_END_ELEMENT(CRuleHandler)
{
    HRESULT hr = (*m_owner.second)();
    return hr;
}


//DEFINE_HANDLER_LOCAL(CRuleHandler, Comment, rule);
HRESULT CRuleHandler::CComment::characters (
    /* [in] */ wchar_t __RPC_FAR *pwchChars,
    /* [in] */ int cchChars)
{
    StringType szValue = XMLUtilities::getString(pwchChars, cchChars);
    CRuleHandler::instance()->rule().SaveComment(szValue);
    return S_OK;
}


DEFINE_HANDLER_START_ELEMENT(CRuleHandler, Parameter)
{
    CRuleHandler::instance()->NewParameter();

    return S_OK;
}
DEFINE_HANDLER_END_ELEMENT(CRuleHandler, Parameter)
{
    CRuleHandler::instance()->SaveParameter();

    return S_OK;
}
void CRuleHandler::SaveParameter()
{
    switch(m_eParmType)
    {
        case FAILURE_PARM :
            rule().SaveFailureParm(m_szCurrentParameter);
            break;
        case FALSE_PARM :
            rule().SaveFalseParm(m_szCurrentParameter);
            break;
        case ACTION_PARM :
            rule().SaveActionParm(m_szCurrentParameter);
            break;
    }
}


DEFINE_CHARACTERS(CRuleHandler, ParmValue)
{
    StringType szValue = XMLUtilities::getString(pwchChars, cchChars);

    IStringBuffer sbValue;
    _TCHAR c;
    for (unsigned i=0; i<szValue.size(); i++)
    {
        c = szValue[i];
        switch(c)
        {
            case '&' :
                sbValue += _T("&") ENTITY_AMP _T(";");
                break;
            default :
                sbValue += (char)c;
                break;
        }
    }

    CRuleHandler::instance()->AddParmValue((const _TCHAR *)sbValue);

    return S_OK;
}


DEFINE_CHARACTERS(CRuleHandler, ActionName)
{
    StringType szValue = XMLUtilities::getString(pwchChars, cchChars);

    if (!CRuleHandler::instance()->rule().ActionName.Value().empty())
        return SECURITY_MULTIPLE_ACTIONS_DEFINED;

    if (CRuleHandler::instance()->GetLoader()->GetObjectRepository()->GetActionID(szValue.c_str()) == -1)
        return SECURITY_UNDEFINED_ACTION_ID;

    CRuleHandler::instance()->rule().ActionName.Value(szValue);

    CRuleHandler::instance()->SetParmType(ACTION_PARM);

    return S_OK;
}


DEFINE_CHARACTERS(CRuleHandler, FailureAction)
{
    StringType szValue = XMLUtilities::getString(pwchChars, cchChars);

    if (CRuleHandler::instance()->GetLoader()->GetObjectRepository()->GetActionID(szValue.c_str()) == -1)
        return SECURITY_UNDEFINED_ACTION_ID;

    CRuleHandler::instance()->rule().FalseActionName.Value(szValue);

    CRuleHandler::instance()->SetParmType(FALSE_PARM);

    return S_OK;
}


DEFINE_CHARACTERS(CRuleHandler, FailureTransition)
{
    StringType szValue = XMLUtilities::getString(pwchChars, cchChars);

    if (CRuleHandler::instance()->GetLoader()->GetObjectRepository()->GetStateID(szValue.c_str()) == -1)
        return SECURITY_UNDEFINED_STATE_ID;

    CRuleHandler::instance()->rule().FalseStateName.Value(szValue);
    return S_OK;
}


DEFINE_CHARACTERS(CRuleHandler, ExceptionAction)
{
    StringType szValue = XMLUtilities::getString(pwchChars, cchChars);

    if (CRuleHandler::instance()->GetLoader()->GetObjectRepository()->GetActionID(szValue.c_str()) == -1)
        return SECURITY_UNDEFINED_ACTION_ID;

    CRuleHandler::instance()->rule().FailureActionName.Value(szValue);

    CRuleHandler::instance()->SetParmType(FAILURE_PARM);

    return S_OK;
}


DEFINE_CHARACTERS(CRuleHandler, ExceptionTransition)
{
    StringType szValue = XMLUtilities::getString(pwchChars, cchChars);

    if (CRuleHandler::instance()->GetLoader()->GetObjectRepository()->GetStateID(szValue.c_str()) == -1)
        return SECURITY_UNDEFINED_STATE_ID;

    CRuleHandler::instance()->rule().FailureStateName.Value(szValue);
    return S_OK;
}



void CRuleHandler::SetParmValue(const StringType &szValue) 
{ 
    m_szCurrentParameter = szValue; 
}


void CRuleHandler::AddParmValue(const StringType &szValue) 
{ 
    m_szCurrentParameter += szValue; 
}


void CRuleHandler::GetXMLOutput(CXMLContent *, CXMLBuilder *)
{
    //CXMLBuilder &builder = *((pTheBuilder == NULL) ? this : pTheBuilder);

//  CRuleImpl &state = *(CRuleImpl *)pContent;

/*
    builder.StartElement(_L(REFUND_IN));
    
    builder.AddAttribute(ATTR_ID, refundIn.Reason__Id);
    builder.AddElement(_L(REASON), refundIn.Reason);    
    builder.AddAttribute(ATTR_ID, refundIn.SuperNum__Id);
    builder.AddElement(_L(SUPER_NUM), refundIn.SuperNum);    
    builder.AddAttribute(ATTR_ID, refundIn.SuperPass__Id);
    builder.AddElement(_L(SUPER_PASS), refundIn.SuperPass);    
    builder.AddAttribute(ATTR_ID, refundIn.Price__Id);
    builder.AddElement(_L(PRICE), refundIn.Price);    

    builder.EndElement(_L(REFUND_IN));
*/
}
