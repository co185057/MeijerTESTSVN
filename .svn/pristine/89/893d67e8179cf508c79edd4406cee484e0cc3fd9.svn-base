#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/ExpressionImpl.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)ExpressionImpl.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/ExpressionImpl.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 7     5/03/05 4:06p Dm185016
 * Cleaned up tracing
 * 
 * 6     4/07/05 1:55p Dm185016
 * TAR 297347
 * 
 * 5     4/07/05 1:51p Dm185016
 * TAR 297347
 * 
 * 4     3/28/05 5:10p Dm185016
 * TAR 296771
 * 
 * 3     3/22/05 4:35p Dm185016
 * <if> support
 * 
 * 2     2/10/05 5:50p Dm185016
 * Fixed problem with static ids used in SM.
 * 
 * 1     1/26/05 2:29p Dm185016
 * Moved to new Repository.
* 
* 7     1/12/05 6:31p Dm185016
* Select support.
* 
* 6     11/15/04 11:43a Dm185016
* Added TrueInstance and FalseInstance for select/if
* 
* 5     10/04/04 6:47p Dm185016
* Changed base class
* 
* 3     4/16/04 4:18p Dm185016
* Added value expressions
* 
* 2     3/19/04 3:28p Dm185016
* Memory leak fix
* 
* 1     2/10/04 11:49a Dm185016
* New support for expressions.
*/
#include "XMLContent.h"
#include "Expression.h"
#include "CPPTokenizer.h"

#define MAP_BOOL(expr) (expr ? _T("true") : _T("false"))

namespace STATE_MACHINE_NAMESPACE
{

class CAbstractStateMachine;

class CExpressionImpl
    : public CXMLContent
    , public IExpression
    , public CPoolObject<CExpressionImpl>
{
public:

    CExpressionImpl(int);
    virtual ~CExpressionImpl(void);

	virtual int GetId() const;

    virtual long GetValue() const;

    virtual void SetValue(long lNewValue);

    virtual bool Evaluate()=0;

    virtual StringType ToString();

    virtual void SetValue(const StringType &);

public: 

    StringType GetXMLOutput(CXMLBuilder *pXMLBuilder=NULL);

protected:

    long lValue;

private:

    CExpressionImpl(CExpressionImpl &);                     // Hide the Copy construcor
    CExpressionImpl &operator =(const CExpressionImpl &);   // Hide the assignment operator

	int nId;

    StringType m_sValue;
};


typedef CExpressionImpl *PEXPRESSIONIMPL;
typedef std::list<PEXPRESSIONIMPL> CExpressionImplList;


class CValueExpressionImpl
    : public CExpressionImpl
{
public:

    CValueExpressionImpl(int nId);
    virtual ~CValueExpressionImpl(void);

    virtual bool Evaluate();

    void SetLHS(IEvaluateable *);

    virtual long GetValue() const;

private:

    CValueExpressionImpl(CValueExpressionImpl &);                     // Hide the Copy construcor
    CValueExpressionImpl &operator =(const CValueExpressionImpl &);   // Hide the assignment operator

    IEvaluateable *m_pLH;
};


//-----------------------------------------------------------------------------
// CIfExpressionImpl
//-----------------------------------------------------------------------------
class CIfBlock;
class CIfExpressionImpl
    : public CExpressionImpl
{
public:

    CIfExpressionImpl(HTRACE, int, IExpression *, IExpression *);
    virtual ~CIfExpressionImpl(void);

    virtual bool Evaluate();

    virtual void SetExpression(IExpression *);

    virtual CIfExpressionImpl &SetStateMachine(CAbstractStateMachine *);

    virtual StringType ToString();

    virtual void SetString(const _TCHAR *);

protected:

    bool IsBlockOwner() const;
    CAbstractStateMachine &stateMachine();

private:

    CIfExpressionImpl(CIfExpressionImpl &);                     // Hide the Copy construcor
    CIfExpressionImpl &operator =(const CIfExpressionImpl &);   // Hide the assignment operator

    IExpression *m_pExpr;

    CIfBlock    *m_pIfBlock;
  
    bool m_bBlockOwner;
    CAbstractStateMachine *m_pSM;

    StringType m_sExpression;
};
class CIfBlock
    : public CBaseObject
{
public: 

    CIfBlock(CIfExpressionImpl *pOwner);

    bool CanContinue() const;

    void SetContinue(bool);

private:

    CIfExpressionImpl *m_pOwner;

    bool m_bContinue;

};


class CBinaryExpressionImpl
    : public CExpressionImpl
{
public:

    CBinaryExpressionImpl(int nId);
    virtual ~CBinaryExpressionImpl(void);

    virtual bool Evaluate();

    void SetLHS(IEvaluateable *);
    void SetRHS(IEvaluateable *);
    void SetOperator(int);

protected:

    int m_operation;
    IEvaluateable *m_pLH;
    IEvaluateable *m_pRH;

private:

    CBinaryExpressionImpl(CBinaryExpressionImpl &);                     // Hide the Copy construcor
    CBinaryExpressionImpl &operator =(const CBinaryExpressionImpl &);   // Hide the assignment operator
};


class CLogicalExpressionImpl
    : public CBinaryExpressionImpl
{
public:

    CLogicalExpressionImpl(int nId);
    virtual ~CLogicalExpressionImpl(void);

    virtual bool Evaluate();

    void SetLHS(IExpression *);
    void SetRHS(IExpression *);

private:

    CLogicalExpressionImpl(CLogicalExpressionImpl &);                     // Hide the Copy construcor
    CLogicalExpressionImpl &operator =(const CLogicalExpressionImpl &);   // Hide the assignment operator

    IExpression *pLHExpression;
    IExpression *pRHExpression;
};


class CUnaryExpressionImpl
    : public CExpressionImpl
{
public:

    CUnaryExpressionImpl(int nId);
    virtual ~CUnaryExpressionImpl(void);

    virtual bool Evaluate();

    void SetOperator(int operation);
    void SetExpression(IExpression *);

private:

    CUnaryExpressionImpl(CUnaryExpressionImpl &);                     // Hide the Copy construcor
    CUnaryExpressionImpl &operator =(const CUnaryExpressionImpl &);   // Hide the assignment operator

    int m_operation;
    IExpression *m_pExpression;
};


class CFalseExpressionImpl
    : public CExpressionImpl
{
public:

    CFalseExpressionImpl(int nId);
    virtual ~CFalseExpressionImpl();

    virtual bool Evaluate();

    virtual StringType ToString();

private:

    CFalseExpressionImpl(CFalseExpressionImpl &);                     // Hide the Copy construcor
    CFalseExpressionImpl &operator =(const CFalseExpressionImpl &);   // Hide the assignment operator
};


class CTrueExpressionImpl
    : public CExpressionImpl
{
public:

    CTrueExpressionImpl(int nId);
    virtual ~CTrueExpressionImpl();

    virtual bool Evaluate();

    virtual StringType ToString();

private:

    CTrueExpressionImpl(CTrueExpressionImpl &);                     // Hide the Copy construcor
    CTrueExpressionImpl &operator =(const CTrueExpressionImpl &);   // Hide the assignment operator
};

};