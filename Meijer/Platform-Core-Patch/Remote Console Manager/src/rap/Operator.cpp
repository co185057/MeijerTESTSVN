#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/Operator.cpp 3     1/12/10 11:55p Cu185008 $
/*
 * @(#)Operator.cpp    $Revision: 3 $ $Date: 1/12/10 11:55p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/Operator.cpp $
 * 
 * 3     1/12/10 11:55p Cu185008
 * G2 Migration.
 * 
 * 1     1/26/05 2:17p Dm185016
 * Moved to new Repository.
// 
// 1     11/11/04 10:04a Dm185016
*/

#include "StdAfx.h"
#include "Operator.h"

using namespace rap;

COperatorKeyType COperator::m_lCount = 1;


COperator::COperator(const _TCHAR *pszOpId, const _TCHAR *pszOpPwd)
    : m_Id(m_lCount++) 
    , m_sOperatorId(pszOpId)
    , m_sOperatorPwd(pszOpPwd)
{}


COperatorKeyType COperator::GetId() { return m_Id; }


const _TCHAR *COperator::GetOperatorId() const { return m_sOperatorId.c_str(); }
const _TCHAR *COperator::GetOperatorPwd() const { return m_sOperatorPwd.c_str(); }


