#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/ExpressionFactory.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)CExpressionFactory.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
 *
 * Copyright 2004, NCR Corporation.
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/ExpressionFactory.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 8     4/07/05 1:54p Dm185016
 * TAR 297347
 * 
 * 7     4/07/05 1:52p Dm185016
 * 
 * 6     4/05/05 12:01p Dm185016
 * TAR 297356 - Added plist for action expression in <if>
 * 
 * 5     3/28/05 5:07p Dm185016
 * TAR 296771
 * 
 * 4     3/22/05 4:33p Dm185016
 * <if> support
 * UNICODE
 * 
 * 3     2/28/05 3:53p Dm185016
 * lint
 * 
 * 2     2/10/05 5:39p Dm185016
 * TAR 292915
 * 
 * 1     1/26/05 2:27p Dm185016
 * Moved to new Repository.
* 
* 7     1/12/05 6:26p Dm185016
* Select support.
* 
* 6     11/15/04 11:44a Dm185016
* Added TrueInstance and FalseInstance for select/if
* 
* 5     10/04/04 6:39p Dm185016
* Trace object is no longer shared.
* 
* 3     5/22/04 11:16a Dm185016
* lint
* 
* 2     4/16/04 4:17p Dm185016
* Added value expressions
* 
* 1     2/10/04 11:39a Dm185016
* New Expression evaluation support
*/

//lint -esym(*, CEvaluateableList::*)

#include "SMConstants.h"
#include "AbstractStateMachine.h"
#include "statem.h"
#include "Expression.h"
#include "CPPTokenizer.h"
#include "ExpressionImpl.h"

namespace STATE_MACHINE_NAMESPACE
{

typedef int CTokenType;
typedef struct _Token
{
    StringType token; 
    int ttype; 
    StringType ttypeName;
    IExpression *tpexpr;

    const _TCHAR *GetToken() { return token.c_str(); }
    const _TCHAR *GetTokenTypeName() { return ttypeName.c_str(); }
} CToken, *PTOKEN;
typedef std::list<CToken> CTokenList;


class CExpressionImpl;

//lint --e{1725}        Want a reference
class STATEM_API CExpressionFactory
    : public CBaseObject
{
    static const int DONE = -1;

public:
        
    enum CTokenTypes 
    {
        Expression=CCPPTokenizer::MaxCppToken+1
        , PrimaryExpression
        , PostfixExpression
        , KVPlist
        , UnaryExpression
        , NultiplicativeExpression
        , AdditiveExpression
        , RelationalExpression
        , EqualityExpression
        , AndExpression
        , ExclusiveOrExpression
        , InclusiveOrExpression
        , LogicalAndExpression
        , LogicalOrExpression
        , ConditionalExpression
    };

    CExpressionFactory(CAbstractStateMachine *);
	virtual ~CExpressionFactory();

    IExpression * NewInstance(const _TCHAR *);

    IExpression * TrueInstance();
    IExpression * FalseInstance();
    
    IExpression * IfInstance(const _TCHAR *, IExpression *);

    enum EExprType
    {
        eNone = 0,
        eLogical,
        eArithmetic
    };

protected:

    CAbstractStateMachine &stateMachine() { return *m_pStateMachine; }
    
    CToken GetNextToken();

private:
    
    ILoggable &loggable();

    CExpressionFactory();

    void State1 (CToken &);
    void State2 (CToken &);
    void State3 (CToken &);
    void State4 (CToken &);
    void State5 (CToken &);

    void PushbackToken(const CToken &);

    void ReduceExpression();
    void ReducePrimaryExpression();
    void ReducePostfixExpression();
    void ReduceKVPlist();
    void ReduceUnaryExpression();
    void ReduceNultiplicativeExpression();
    void ReduceAdditiveExpression();
    void ReduceRelationalExpression();
    void ReduceEqualityExpression();
    void ReduceAndExpression();
    void ReduceExclusiveOrExpression();
    void ReduceInclusiveOrExpression();
    void ReduceLogicalAndExpression();
    void ReduceLogicalOrExpression();
    void ReduceConditionalExpression();

    IEvaluateable *GetInteger(const StringType &);

    CExpressionImpl *ReduceBinaryExpression();
    CExpressionImpl *ReduceValueExpression();
    CExpressionImpl *ReduceActionExpression();
    CExpressionImpl *ReduceLogicalNotExpression(CExpressionImpl *);

    CExpressionFactory(const CExpressionFactory &);
    CExpressionFactory &operator =(const CExpressionFactory &);

    int m_operation;
    bool m_bNegateIt;
    IEvaluateable *m_pLH;
    IEvaluateable *m_pRH;
    
    StringType m_szActionName;
    int m_nState;
    int m_counter;

    EExprType m_eExprType;

    CExpressionImpl *m_pExpression;

    ILoggable *m_pLoggable;

    CAbstractStateMachine *m_pStateMachine;

    CEvaluateableList m_evaluateables;

    CTokenList m_TokenStack;

    CCPPTokenizer tokenizer;

    CToken  m_token;

    StringType  m_temp;
};


class IOperator
{
public:
    
    virtual bool Evaluate()=0;
};


class LtOperator
    : public IOperator
{
public:
    
    bool Evaluate()=0;
};

}
