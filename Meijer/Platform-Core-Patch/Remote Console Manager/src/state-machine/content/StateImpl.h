#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/StateImpl.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)StateImpl.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/StateImpl.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 4     3/16/05 4:03p Dm185016
 * Fixed mismatch for parameter types.
 * 
 * 3     3/11/05 1:20p Dm185016
 * <select> support
 * 
 * 2     2/10/05 5:54p Dm185016
 * Fixed problem with static ids used in SM.
 * 
 * 1     1/26/05 2:29p Dm185016
 * Moved to new Repository.
* 
* 9     12/07/04 4:29p Dm185016
* Reworking CRulesList for <select>
* 
* 8     10/04/04 6:47p Dm185016
* Changed base class
* 
* 6     6/30/04 3:31p Dm185016
* Lint
* 
* 5     4/06/04 1:53p Dm185016
* Suppressed warning 4251
* 
* 4     3/31/04 10:22a Dm185016
* Exported class for new derivatives
* 
* 3     3/19/04 3:28p Dm185016
* Memory leak fix
* 
* 2     1/16/04 9:23a Dm185016
* Namespace support, Removed <class>, Global state, Entry and Exit lists
*/

using namespace std;

#include "XMLContent.h"
#include "State.h"
#include "Rule.h"
#include "RuleProxy.h"
#include "RuleTypes.h"
#include "Action.h"
#include <map>
#include <vector>

#pragma warning( push )
#pragma warning( disable : 4251 )

namespace STATE_MACHINE_NAMESPACE
{

typedef std::map<int, CRulesList *> CRulesListMap;


class STATEM_API CStateImpl
    : public CXMLContent
	, public IState
    , public CPoolObject<CStateImpl>
{
public:

    CStateImpl(int);
    virtual ~CStateImpl(void);

	virtual int GetId() const;

	virtual const _TCHAR *GetComment();

	virtual const _TCHAR *GetName();

	virtual long GetTimeout();

	virtual bool IsInitial();

	virtual bool IsGlobal();

    DECLARE_BOOLEAN_PROPERTY(Initial);
    DECLARE_BOOLEAN_PROPERTY(Global);
    DECLARE_STRING_PROPERTY(Name);
    DECLARE_STRING_PROPERTY(Comment);
    DECLARE_STRING_PROPERTY(Entry);
    DECLARE_STRING_PROPERTY(Exit);
    DECLARE_LONG_PROPERTY(Timeout);


    void AddExitRule(IRule *);
    void AddEntryRule(IRule *);
    void AddMsgRule(const IMessage *, IRule *);
    void AddMsgRule(int, IRule *, _RuleTypes );

    CRulesList *GetEntryList() { return &m_EntryActions; }
    CRulesList *GetExitList() { return &m_ExitActions; }
    CRulesList *GetMsgList(int);

    StringType GetXMLOutput(CXMLBuilder *pXMLBuilder=NULL);     //lint !e1735

    friend class CXMLStateMachine;

private:

    CStateImpl(const CStateImpl &);                     // Hide the Copy construcor
    CStateImpl &operator =(const CStateImpl &);   // Hide the assignment operator

    void FixupSelect(size_t nNextRuleIndex);

    CRulesList m_ExitActions;
    CRulesList m_EntryActions;
    CRulesList m_WhenActions;

    CRulesListMap m_MsgMap;
    
    _TCHAR * m_szName;
	_TCHAR * m_szComment;
	long m_lTimeout;
    int m_nID;
    bool m_bInitial;
    bool m_bGlobal;
};

typedef map<int, PSTATE> CStateIntMap;
typedef vector<PSTATE> CStatesVector;

}

#pragma warning( pop )
