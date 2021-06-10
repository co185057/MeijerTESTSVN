#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/RuleKey.cpp 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)XMLRule.java    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
// $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/RuleKey.cpp $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 2     2/28/05 3:53p Dm185016
 * lint
 * 
 * 1     1/26/05 2:27p Dm185016
 * Moved to new Repository.
// 
// 5     1/18/05 3:55p Dm185016
// UNICODE support
// 
// 3     8/17/04 6:23p Dm185016
// 
// 2     1/16/04 9:06a Dm185016
// Namespace support
// 
// 1     1/06/04 8:58a Dm185016
// First version
*/
#include "StdAfx.h"
#include "ruleKey.h"

using namespace STATE_MACHINE_NAMESPACE;

CRuleKey::CRuleKey(int _nMsgID, int _nStateID)
    : m_nMsgID(_nMsgID)
    , m_nStateID(_nStateID)
{
    UpdateKey();
}


CRuleKey::CRuleKey() 
    : m_nMsgID(-1)
    , m_nStateID(-1)
{
    UpdateKey();
}


CRuleKey::~CRuleKey() 
{
}


void CRuleKey::SetMsgID(int nNewID)
{
    m_nMsgID = nNewID;

    UpdateKey();
}


void CRuleKey::SetStateID(int nNewID)
{
    m_nStateID = nNewID;

    UpdateKey();
}


void CRuleKey::UpdateKey()
{
    _stprintf(m_Key
            , _T("%04.4x:%04.4x")
            , (m_nStateID & 0x0000FFFF)
            , (m_nMsgID & 0x0000FFFF) );
//  ITRACE_DEVELOP("CRuleKey key=" + IString((_TCHAR *)&m_Key));
}


CRuleKey:: operator const _TCHAR * ( ) const
{
    return const_cast<const _TCHAR *>(m_Key);
}
