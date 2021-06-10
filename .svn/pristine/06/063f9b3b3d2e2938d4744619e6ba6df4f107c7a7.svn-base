#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/SelectHandler.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)SelectHandler.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/SelectHandler.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:30p Dm185016
 * Moved to new Repository.
* 
* 3     1/12/05 6:34p Dm185016
* Select support.
* 
* 2     12/22/04 11:25a Dm185016
* 
* 1     10/29/04 9:29a Dm185016
*/
#include "SMConstants.h"
#include "xmlbuilder.h"
#include "XPathHandler.h"
#include "RuleProxy.h"
#include "singleton.h"

namespace STATE_MACHINE_NAMESPACE 
{

class CRuleHandler;
class CStateObjectProxy;
class CSelectImpl;
class CWhenImpl;
class CRuleImpl;
class CMsgImpl;

class CSelectHandler 
    : public CXMLBuilder
    , public CXPathHandler
    , public CSingleton<CSelectHandler>
{
public:

    friend class CSingleton<CSelectHandler>;

    friend class CWhenHandler;

    ~CSelectHandler(void);

    virtual void registerHandlers(void);

    DECLARE_START_ELEMENT();
    DECLARE_CHARACTERS();
    DECLARE_END_ELEMENT();

    virtual void GetXMLOutput(CXMLContent *, CXMLBuilder *);

    CStateObjectProxy &state() { return *reinterpret_cast<CStateObjectProxy *>(m_pContent); }

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

    HRESULT AddWhenClause(CWhenImpl *);

    CWhenImpl &when() { return *m_pCurrentWhen; }

private:
    CSelectHandler(void);
    CSelectHandler(CSelectHandler &);                     // Hidden Copy construcor
    CSelectHandler &operator =(const CSelectHandler &);   // Hidden assignment operator

    CRuleImplBase *m_pCurrentRule;

    CRuleHandler &hndlrRuleHandler;

    int m_nRuleCnt;

    CRuleQ m_vWhenClauses;

    CSelectImpl *m_pSelect;
    CWhenImpl   *m_pCurrentWhen;
};

};