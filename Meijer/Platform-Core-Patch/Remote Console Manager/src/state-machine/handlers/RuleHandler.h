#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/RuleHandler.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)RuleHandler.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/RuleHandler.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 2     2/10/05 5:57p Dm185016
 * Fixed problem with static ids used in SM.
 * 
 * 1     1/26/05 2:30p Dm185016
 * Moved to new Repository.
* 
* 10    1/12/05 6:34p Dm185016
* Select support.
* 
* 9     10/29/04 9:30a Dm185016
* 
* 8     10/04/04 6:48p Dm185016
* New base class:  XPathHandler
* 
* 6     6/30/04 3:32p Dm185016
* Added validation of action names.
* 
* 5     3/19/04 3:36p Dm185016
* Split into 2 pieces - this and RulesHandler.  This now only handles a
* single <rule> section.
* 
* 3     2/10/04 11:50a Dm185016
* New action format
* 
* 2     1/16/04 9:26a Dm185016
* New rule format, Action parameters, Entry and Exit lists, Global state
*/
#include "SMConstants.h"
#include "statem.h"
#include "xmlbuilder.h"
#include "XPathHandler.h"
#include "StateObjectProxy.h"
#include "singleton.h"
#include "RuleImpl.h"
#include "MsgImpl.h"
#include <stack>

namespace STATE_MACHINE_NAMESPACE 
{

class STATEM_API CRuleHandler 
    : public CXMLBuilder
    , public CXPathHandler
    , public CSingleton<CRuleHandler>
{
public:

    friend class CSingleton<CRuleHandler>;

    typedef HRESULT (*PRULE_BEGIN_FXN)(CRuleImplBase **);
    typedef HRESULT (*PRULE_END_FXN)();

	typedef pair<PRULE_BEGIN_FXN, PRULE_END_FXN> COwner;

    typedef stack<COwner> COwnerStack;

    enum ActionDest
    {
        NO_DEST = 0,
        EXIT_LIST = 1,
        ENTRY_LIST,
        MSG_LIST
    };

    enum ParmType
    {
        NO_PARM = 0,
        ACTION_PARM = 1,
        FALSE_PARM,
        FAILURE_PARM
    };

    ~CRuleHandler(void);

    void PushOwner(PRULE_BEGIN_FXN, PRULE_END_FXN);
    const COwner &GetOwner();
    void PopOwner();

    virtual void registerHandlers(void);

    DECLARE_START_ELEMENT();
    DECLARE_END_ELEMENT();

    virtual void GetXMLOutput(CXMLContent *, CXMLBuilder *);

    CRuleImpl &rule() { return *reinterpret_cast<CRuleImpl *>(m_pContent); }

protected:
    
    void Reference(std::wstring szValue);

//  DECLARE_HANDLER_WITH_OWNER(Comment, CRuleHandler);
    DECLARE_HANDLER_BEGIN(Comment) \
    DECLARE_HANDLER_CHARACTERS() \
    DECLARE_HANDLER_OWNER(Comment, CRuleHandler)\
    DECLARE_HANDLER_END(Comment);

    DECLARE_HANDLER_WITH_OWNER(ActionName, CRuleHandler);
    DECLARE_HANDLER_WITH_OWNER(ParmValue, CRuleHandler);
    DECLARE_HANDLER_WITH_OWNER(FailureAction, CRuleHandler);
    DECLARE_HANDLER_WITH_OWNER(FailureTransition, CRuleHandler);
    DECLARE_HANDLER_WITH_OWNER(ExceptionAction, CRuleHandler);
    DECLARE_HANDLER_WITH_OWNER(ExceptionTransition, CRuleHandler);

    DECLARE_HANDLER_BEGIN(Parameter);
    DECLARE_HANDLER_START_ELEMENT();
    DECLARE_HANDLER_END_ELEMENT();
    DECLARE_HANDLER_OWNER(Parameter, CRuleHandler)
    DECLARE_HANDLER_END(Parameter);

    void SetDestination(ActionDest eActionDest) { m_eActionDest = eActionDest; }
    void SetParmType(ParmType eParmType) { m_eParmType = eParmType; }
    void NewParameter()  { m_szCurrentParameter.clear(); }
    void SaveParameter();
    void SetParmValue(const StringType &szValue);
    void AddParmValue(const StringType &szValue); 

private:

    CRuleHandler();
    CRuleHandler(CRuleHandler &);                     // Hidden Copy construcor
    CRuleHandler &operator =(const CRuleHandler &);   // Hidden assignment operator

    ActionDest m_eActionDest;
    ParmType m_eParmType;
    StringType m_szCurrentParameter;

	COwner m_owner;
    COwnerStack m_owners;
    //PRULE_BEGIN_FXN m_pBeginFxn;
    //PRULE_END_FXN m_pEndFxn;

    bool m_bRegistered;
};

};