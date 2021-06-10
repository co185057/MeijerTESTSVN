#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/XMLRule.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)XMLRule.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/XMLRule.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 2     3/11/05 1:35p Dm185016
 * <select> support
 * 
 * 1     1/26/05 2:28p Dm185016
 * Moved to new Repository.
* 
* 8     12/22/04 11:20a Dm185016
* Added conditional logic support.
* 
* 7     11/15/04 11:45a Dm185016
* select/if support
* 
* 6     10/04/04 6:43p Dm185016
* Trace object is no longer shared.
* 
* 4     5/19/04 1:34p Dm185016
* lint
* 
* 3     1/27/04 1:56p Dm185016
* DLL Support
* 
* 2     1/16/04 9:13a Dm185016
* Namespace, Action parameters
* 
* 1     1/06/04 8:58a Dm185016
* First version
*/

#include "rule.h"
#include "ModifiableRule.h"
#include "statem.h"

#pragma warning( push )
#pragma warning( disable : 4251 )

namespace STATE_MACHINE_NAMESPACE 
{

class IExpression;

class STATEM_API CXMLRule 
    : public IRule
    , public IModifiableRule
    , virtual public CBaseObject
{

public:
    CXMLRule();
    virtual ~CXMLRule();

	virtual bool IsActive();

    virtual const CRuleKey *GetKey() const;

	virtual int GetMsgID() const;

	virtual int GetStateID() const;

	virtual int GetActionID() const;

    virtual CParameterList &GetActionParms();

	virtual int GetEndStateID() const;

	virtual int GetFalseStateID() const;

	virtual int GetFalseActionID() const;

    virtual CParameterList &GetFalseParms();

	virtual int GetFailureStateID() const;

	virtual int GetFailureActionID() const;

    virtual CParameterList &GetFailureParms();

	virtual void SetMsgID(int nNewID) ;

	virtual void SetStateID(int nNewID) ;

	virtual void SetActionID(int nNewID) ;

    virtual void SetActionParms(const CParameterList &);

	virtual void SetSequenceNumber(int nNewID) ;

	virtual void SetEndStateID(int nNewID) ;

	virtual void SetFalseStateID(int nNewID) ;

	virtual void SetFalseActionID(int nNewID) ;

	virtual void SetFalseParms(const CParameterList &) ;

	virtual void SetFailureStateID(int nNewID) ;

	virtual void SetFailureActionID(int nNewID) ;

	virtual void SetFailureParms(const CParameterList &) ;

	virtual IExpression &GetExpression();
	virtual void SetExpression(IExpression *pExpr);

    virtual CRulesList &GetRulesList();
    virtual bool IsRulesListActive();

    virtual int GetNextRuleIndex();
    virtual void SetNextRuleIndex(int);

private:

    CXMLRule(const CXMLRule &);               // Hide the Copy construcor
    CXMLRule &operator =(const CXMLRule &);   // Hide the assignment operator

    CRuleKey m_Key;
    int  m_nMsgID;
    int  m_nStateID;
    int m_nActionID;
    int m_nSeqNumber;
    int m_nEndStateID;
    int m_nFalseStateID;
    int m_nFalseActionID;
    int m_nFailureStateID;
    int m_nFailureActionID;

    CParameterList m_vActionParms;
	CParameterList m_vFalseParms;
	CParameterList m_vFailureParms;

	IExpression *m_pExpr;

    CRulesList m_vRules;

    int m_nNextRuleIndex;
};

}

#pragma warning( pop )
