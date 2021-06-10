#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/IfHandler.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)IfHandler.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/IfHandler.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 3     3/22/05 5:00p Dm185016
 * <if> support
 * 
 * 2     3/17/05 2:46p Dm185016
 * <if>, <else-if>, <else> support
 * 
 * 1     1/26/05 2:30p Dm185016
 * Moved to new Repository.
* 
* 2     1/12/05 6:34p Dm185016
* Select support.
* 
* 1     10/04/04 6:48p Dm185016
*/
#include "SMConstants.h"
#include "xmlbuilder.h"
#include "XPathHandler.h"
#include "RuleHandler.h"
#include "StateObjectProxy.h"
#include "singleton.h"
#include "RuleImpl.h"
#include "IfImpl.h"

namespace STATE_MACHINE_NAMESPACE 
{

class CIfHandler 
    : public CXMLBuilder
    , public CXPathHandler
    , public CSingleton<CIfHandler>
{
public:

    friend class CSingleton<CIfHandler>;

    ~CIfHandler(void);

    virtual void registerHandlers(void);

    DECLARE_START_ELEMENT();
    DECLARE_END_ELEMENT();

    virtual void GetXMLOutput(CXMLContent *, CXMLBuilder *);

    CIfImpl &ifclause() { return *m_pIf; }

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

    CIfHandler(void);
    CIfHandler(CIfHandler &);                     // Hidden Copy construcor
    CIfHandler &operator =(const CIfHandler &);   // Hidden assignment operator

    CRuleImpl *m_pCurrentRule;

    CRuleHandler &hndlrRuleHandler;

    CIfImpl *m_pIf;

    int m_nRuleCnt;
};

};