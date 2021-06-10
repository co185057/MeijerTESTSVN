#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/ElseIfHandler.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)ElseIfHandler.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/ElseIfHandler.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 2     3/22/05 5:00p Dm185016
 * <if> support
 * 
 * 1     3/17/05 2:47p Dm185016
 * <if>, <else-if>, <else> support
*/
#include "SMConstants.h"
#include "xmlbuilder.h"
#include "XPathHandler.h"
#include "RuleHandler.h"
#include "StateObjectProxy.h"
#include "singleton.h"
#include "RuleImpl.h"
#include "ElseIfImpl.h"

namespace STATE_MACHINE_NAMESPACE 
{

class CElseIfHandler 
    : public CXMLBuilder
    , public CXPathHandler
    , public CSingleton<CElseIfHandler>
{
public:

    friend class CSingleton<CElseIfHandler>;

    ~CElseIfHandler(void);

    virtual void registerHandlers(void);

    DECLARE_START_ELEMENT();
    DECLARE_END_ELEMENT();

    virtual void GetXMLOutput(CXMLContent *, CXMLBuilder *);

    CElseIfImpl &elseif() { return *m_pElseIf; }

protected:

    CRuleImplBase &currentRule() { return *m_pCurrentRule; }
    
    void Reference(std::wstring szValue);

    DECLARE_HANDLER(Comment);

    void NewRule();
    void SaveRule();
    void CheckNullList();
    void ResetRuleList();

    static HRESULT BeginRule(CRuleImplBase **);
    static HRESULT EndRule();

private:
    CElseIfHandler(void);
    CElseIfHandler(CElseIfHandler &);                     // Hidden Copy construcor
    CElseIfHandler &operator =(const CElseIfHandler &);   // Hidden assignment operator

    CRuleImpl *m_pCurrentRule;

    CElseIfImpl *m_pElseIf;

    CRuleHandler &hndlrRuleHandler;

    int m_nRuleCnt;
};

};