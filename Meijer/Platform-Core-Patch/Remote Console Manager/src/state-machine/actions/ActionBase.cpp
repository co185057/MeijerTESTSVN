// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/ActionBase.cpp 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)AbstractStateMachine.cpp    $Revision: 1 $ $Date: 6/20/08 1:11p $
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

#include "stdafx.h"
#include "ActionBase.h"

using namespace STATE_MACHINE_NAMESPACE;

CActionBase::CActionBase()
    : m_nID(-1)
{
    m_sName.erase();
}


CActionBase:: ~CActionBase() 
{
    m_pSM = NULL;
}


DEFINE_STRING_PROPERTY(CActionBase, Comment);


const _TCHAR *CActionBase::GetComment(void) 
{
    return Comment.Value().c_str();
}


bool CActionBase::IsInternal() 
{
    return false; 
}


int CActionBase::GetId() const
{
    return m_nID;
}


void CActionBase::SetId(int nID)
{
    m_nID = nID;
}


void CActionBase::SetSynchronousReturnCode(IMessage &msg, HRESULT hr)
{
    stateMachine().SetSynchronousReturnCode(msg, hr);
}


void CActionBase::AddParameter(const StringType &, const StringType &szValue)
{
    m_vParms.push_back(szValue);
}


void CActionBase::SetParameters(const CParameterList &newParms)
{
    m_vParms.clear();
    m_vParms.insert(m_vParms.begin(), newParms.begin(), newParms.end());
}


void CActionBase::ClearParameters()
{
    m_vParms.clear();
}


/*lint -save -e1536 */
CParameterList &CActionBase::GetParameters() 
{ 
    return m_vParms; 
}
/*lint -restore */


void CActionBase::SetStateMachine(CAbstractStateMachine *pSM) 
{ 
    m_pSM = pSM; 
}


CAbstractStateMachine &CActionBase::stateMachine() 
{ 
    _ASSERTE(m_pSM != NULL); 
    return *m_pSM; 
}

const _TCHAR *CActionBase::GetName()
{
    return m_sName.c_str();
}


void CActionBase::SetName(const _TCHAR *sName)
{
    m_sName = sName;
}


HRESULT CActionBase::ParseOperands()
{
    HRESULT hr = S_OK;

    // TODO:  Need to implement this

    //CParameterList &parms = GetParameters();
    //CParameterList::iterator iter = parms.begin();
    //if (iter != parms.end())
    //{
    //    for ( ;iter != parms.end(); iter++)
    //    {
    //        POPERANDFXN pfxn = GetNextOperandHandler();
            //StringType &sVariableName = *iter;
            //hr = stateMachine().GetVariable(&pVariable, sVariableName.c_str());

            //m_bDelayAllowed &= (pVariable->GetValue() != 0);    //lint !e514

            //ITRACE_DEVELOP(_T("Checking variable:  ")
            //    + IString(sVariableName.c_str())
            //    + _T(".  Value:  ")
            //    + IString(pVariable->GetValue()));
    //    }
    //}

    return hr;
}


CActionBase::POPERANDFXN CActionBase::GetNextOperandHandler()
{
    return NULL;
}


COperandTable *CActionBase::GetOperandTable()
{
    return NULL;
}


void CActionBase::DefaultOperand(CParameterList::iterator &)
{
}
