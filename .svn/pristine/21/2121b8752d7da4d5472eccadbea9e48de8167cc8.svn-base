#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/RulesHandler.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)RulesHandler.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/RulesHandler.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 3     3/17/05 2:45p Dm185016
 * <if>, <else-if>, <else> support
 * 
 * 2     3/17/05 10:51a Dm185016
 * <if>, <else-if>, <else> support
 * 
 * 1     1/26/05 2:30p Dm185016
 * Moved to new Repository.
* 
* 5     1/12/05 6:34p Dm185016
* Select support.
* 
* 4     10/04/04 6:48p Dm185016
* New base class:  XPathHandler
* 
* 2     5/22/04 8:13a Dm185016
* Added code to handle null definition of msg-rule-list.  Generates a
* single null action if no list specified.
* 
* 1     3/19/04 3:35p Dm185016
* New handler split from RuleHandler
*/
#include "SMConstants.h"
#include "xmlbuilder.h"
#include "XPathHandler.h"
#include "RuleHandler.h"
#include "StateObjectProxy.h"
#include "singleton.h"
#include "RuleImpl.h"
#include "MsgImpl.h"

namespace STATE_MACHINE_NAMESPACE 
{

class CRulesHandler 
    : public CXMLBuilder
    , public CXPathHandler
    , public CSingleton<CRulesHandler>
{
public:

    friend class CSelectHandler;
    friend class CIfHandler;
    friend class CElseIfHandler;
    friend class CElseHandler;

    enum ActionDest
    {
        NO_DEST = 0,
        EXIT_LIST = 1,
        ENTRY_LIST,
        MSG_LIST
    };

    friend class CSingleton<CRulesHandler>;

    ~CRulesHandler(void);

    virtual void registerHandlers(void);

    DECLARE_START_ELEMENT();
    DECLARE_END_ELEMENT();

    virtual void GetXMLOutput(CXMLContent *, CXMLBuilder *);

    CStateObjectProxy &state() { return *reinterpret_cast<CStateObjectProxy *>(m_pContent); }

protected:

    CRuleImplBase &currentRule() { return *m_pCurrentRule; }
//  CMsgImpl &currentMsg() { return *m_pCurrentMsg; }
    CMsgImpl &currentMsg() { return m_CurrentMsg; }
    
    void Reference(std::wstring szValue);

    DECLARE_HANDLER(Comment);
    DECLARE_HANDLER(MsgName);
    DECLARE_HANDLER(StateName);
    DECLARE_HANDLER(FailureTransitionDefault);
    DECLARE_HANDLER(ExceptionTransitionDefault);
    DECLARE_HANDLER(FailureActionDefault);
    DECLARE_HANDLER(ExceptionActionDefault);

    DECLARE_HANDLER_BEGIN(ExitList);
    DECLARE_HANDLER_START_ELEMENT();
    DECLARE_HANDLER_END_ELEMENT();
    DECLARE_HANDLER_END(ExitList);

    DECLARE_HANDLER_BEGIN(EntryList);
    DECLARE_HANDLER_START_ELEMENT();
    DECLARE_HANDLER_END_ELEMENT();
    DECLARE_HANDLER_END(EntryList);

    DECLARE_HANDLER_BEGIN(Message);
    DECLARE_HANDLER_START_ELEMENT();
    DECLARE_HANDLER_END_ELEMENT();
    DECLARE_HANDLER_END(Message);

    DECLARE_HANDLER_BEGIN(ActionList);
    DECLARE_HANDLER_START_ELEMENT();
    DECLARE_HANDLER_END_ELEMENT();
    DECLARE_HANDLER_END(ActionList);

    DECLARE_HANDLER_BEGIN(Global); 
    DECLARE_HANDLER_START_ELEMENT();
    DECLARE_HANDLER_CHARACTERS();
    DECLARE_HANDLER_END(Global);

    DECLARE_HANDLER_BEGIN(SuccessTransition); 
    DECLARE_HANDLER_START_ELEMENT();
    DECLARE_HANDLER_CHARACTERS();
    DECLARE_HANDLER_END(SuccessTransition);

    void SetDestination(ActionDest eActionDest) { m_eActionDest = eActionDest; }
    void NewRule();
    void SaveRule();
    void SaveRule(CRuleImplBase *);
    void NewMessage();
    void SetMsgName(const StringType &);
    HRESULT CreateGlobalState();
    void CheckNullList();
    void ResetRuleList();

    static HRESULT BeginRule(CRuleImplBase **);
    static HRESULT EndRule();

private:
    CRulesHandler(void);
    CRulesHandler(CRulesHandler &);                     // Hidden Copy construcor
    CRulesHandler &operator =(const CRulesHandler &);   // Hidden assignment operator

    CRuleImplBase *m_pCurrentRule;
//  CMsgImpl *m_pCurrentMsg;
    CMsgImpl m_CurrentMsg;

    ActionDest m_eActionDest;

    CRuleHandler &hndlrRuleHandler;

    int m_nRuleCnt;
};

};