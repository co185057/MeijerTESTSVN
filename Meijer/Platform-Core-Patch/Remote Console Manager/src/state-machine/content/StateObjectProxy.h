#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/StateObjectProxy.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)StateObjectProxy.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/StateObjectProxy.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 3     2/28/05 3:46p Dm185016
 * lint
 * 
 * 2     2/10/05 5:54p Dm185016
 * Fixed problem with static ids used in SM.
 * 
 * 1     1/26/05 2:29p Dm185016
 * Moved to new Repository.
* 
* 12    1/12/05 6:32p Dm185016
* Select support.
* 
* 11    12/22/04 11:23a Dm185016
* Conditional logic support.
* 
* 10    11/15/04 11:43a Dm185016
* Added TrueInstance and FalseInstance for select/if
* 
* 9     10/04/04 6:47p Dm185016
* Changed base class
* 
* 7     5/21/04 10:36a Dm185016
* lint
* 
* 6     4/06/04 1:53p Dm185016
* Suppressed warning 4251
* 
* 5     3/31/04 10:22a Dm185016
* Exported class for new derivatives
* 
* 4     3/19/04 3:28p Dm185016
* Memory leak fix
* 
* 3     2/10/04 11:50a Dm185016
* New action format
* 
* 2     1/27/04 2:06p Dm185016
* Action Library support
* 
* 1     1/16/04 9:21a Dm185016
* Support for new rule format
*/

/*lint -save -e1536 */

#include "statem.h"
#include "XMLContent.h"
#include "RuleImpl.h"
#include <vector>
#include <map>

#pragma warning( push )
#pragma warning( disable : 4251 )

namespace STATE_MACHINE_NAMESPACE
{

class CMsgImpl;

typedef std::map<int, CRuleQ *> CRuleQMap;
typedef CRuleQMap::iterator CRuleQMapIterator;

class STATEM_API CStateObjectProxy
    : public CXMLContent
    , public CPoolObject<CStateObjectProxy>
{
public:

    enum ActionDest
    {
        EXIT_LIST = 1,
        ENTRY_LIST,
        MSG_LIST
    };

    CStateObjectProxy(int);
    virtual ~CStateObjectProxy(void);

    HRESULT SaveSuccessTransition(const StringType &);
    HRESULT SaveFailureTransitionDefault(const StringType &);
    HRESULT SaveExceptionTransitionDefault(const StringType &);
    HRESULT SaveFailureActionDefault(const StringType &);
    HRESULT SaveExceptionActionDefault(const StringType &);
    DECLARE_STRING_PROPERTY(Comment);
    DECLARE_STRING_PROPERTY(StateName);
    DECLARE_BOOLEAN_PROPERTY(Global);

//  CStateObject &state() { return *m_pStateObject; }

    void AddExitRule(CRuleImplBase *);
    void AddEntryRule(CRuleImplBase *);
    void AddMsgRule(CMsgImpl *pMsg, CRuleImplBase *);
    void ClearRules();

    CRuleQ *GetEntryList() { return &m_qEntryActions; }
    CRuleQ *GetExitList() { return &m_qExitActions; }
    CRuleQ *GetMsgList(int);
    CRuleQMap *GetMsgMap() { return &m_mapMsgActions; }

public: 

    StringType GetXMLOutput(CXMLBuilder *pXMLBuilder=NULL);

protected:

	int m_nId;

    CRuleQ m_qExitActions;
    CRuleQ m_qEntryActions;
    CRuleQ m_qMsgActions;
    CRuleQMap m_mapMsgActions;

private:

    CStateObjectProxy();
    CStateObjectProxy(const CStateObjectProxy &);               // Hide the Copy construcor
    CStateObjectProxy &operator =(const CStateObjectProxy &);   // Hide the assignment operator
};

}

#pragma warning( pop )

/*lint -restore */