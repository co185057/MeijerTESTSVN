#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/RuleImpl.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)RuleImpl.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/RuleImpl.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 3     3/11/05 1:20p Dm185016
 * <select> support
 * 
 * 2     2/28/05 3:46p Dm185016
 * lint
 * 
 * 1     1/26/05 2:29p Dm185016
 * Moved to new Repository.
* 
* 9     1/12/05 6:31p Dm185016
* Select support.
* 
* 8     12/22/04 11:23a Dm185016
* Factored out base functionality of RuleImpl into RuleImplBase.
* 
* 7     11/15/04 11:43a Dm185016
* Added TrueInstance and FalseInstance for select/if
* 
* 6     10/04/04 6:47p Dm185016
* Changed base class
* 
* 4     4/06/04 1:53p Dm185016
* Suppressed warning 4251
* 
* 3     3/19/04 3:29p Dm185016
* Application state support
* 
* 2     1/16/04 9:20a Dm185016
* Namespace support, Action parameters
*/
#include "SMConstants.h"
#include "XMLContent.h"
#include "RuleProxy.h"
#include "rule.h"

#pragma warning( push )
#pragma warning( disable : 4251 )

namespace STATE_MACHINE_NAMESPACE
{

class IExpression;

#ifndef DEF_CPARAMETERLIST
#define DEF_CPARAMETERLIST
typedef vector<StringType> CParameterList;
#endif

class STATEM_API CRuleImplBase
    : public CXMLContent
	, public IRuleProxy
{
public:

    CRuleImplBase();
    virtual ~CRuleImplBase(void);

	virtual IExpression * GetExpression();
	virtual void SetExpression(IExpression *);

    virtual CRuleQ &GetRulesList();

	virtual int GetId() const;

    const _TCHAR *GetComment();

    const _TCHAR *GetMsgName();

    const _TCHAR *GetActionName();

    CParameterList &GetActionParms();

    const _TCHAR *GetStateName();

    const _TCHAR *GetEndStateName();

    const _TCHAR *GetFalseActionName();

    const _TCHAR *GetFalseStateName();

    CParameterList &GetFalseParms();

    const _TCHAR *GetFailureActionName();

    const _TCHAR *GetFailureStateName();

    CParameterList &GetFailureParms();

    int GetSequenceNumber();


    DECLARE_STRING_PROPERTY(Comment);
    DECLARE_STRING_PROPERTY(MsgName);
    DECLARE_STRING_PROPERTY(ActionName);
    DECLARE_STRING_PROPERTY(ActionParm);
    DECLARE_STRING_PROPERTY(StateName);
    DECLARE_STRING_PROPERTY(EndStateName);
    DECLARE_STRING_PROPERTY(FalseActionName);
    DECLARE_STRING_PROPERTY(FalseStateName);
    DECLARE_STRING_PROPERTY(FalseParm);
    DECLARE_STRING_PROPERTY(FailureActionName);
    DECLARE_STRING_PROPERTY(FailureStateName);
    DECLARE_STRING_PROPERTY(FailureParm);
    DECLARE_INT_PROPERTY(SequenceNumber);

    virtual StringType ToString();

public: 

    StringType GetXMLOutput(CXMLBuilder *pXMLBuilder=NULL);

private:

    void FormatAction(const _TCHAR *, const _TCHAR *, const CParameterList &, IStringBuffer &);
    void FormatParmaterList(const CParameterList &, IStringBuffer &);

    CRuleImplBase(const CRuleImplBase &);                // Hide the Copy construcor
    CRuleImplBase &operator =(const CRuleImplBase &);   // Hide the assignment operator

	static int counter;

	int nId;
	CParameterList m_vActionParms;
	CParameterList m_vFalseParms;
	CParameterList m_vFailureParms;

	IExpression *m_pExpr;

    CRuleQ  m_vRules;
};

class STATEM_API CRuleImpl
    : public CRuleImplBase
    , public CPoolObject<CRuleImpl>
{
public:

    CRuleImpl();
    virtual ~CRuleImpl(void);

    virtual _RuleTypes GetRuleType() const;

private:

    CRuleImpl(const CRuleImpl &);               // Hide the Copy construcor
    CRuleImpl &operator =(const CRuleImpl &);   // Hide the assignment operator
};

}

#pragma warning( pop )
