// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/ConstantImpl.cpp 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)ConstantImpl.cpp    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/ConstantImpl.cpp $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 6     5/31/05 11:40a Dm185016
 * String variable type support
 * 
 * 3     2/28/05 3:46p Dm185016
 * lint
 * 
 * 2     2/10/05 5:50p Dm185016
 * Fixed problem with static ids used in SM.
 * 
 * 1     1/26/05 2:29p Dm185016
 * Moved to new Repository.
// 
// 2     1/27/04 2:04p Dm185016
// Action Library support
// 
// 1     1/16/04 9:21a Dm185016
// <constant> support
*/
#include "StdAfx.h"
#include "ConstantImpl.h"
#include "ConstantHandler.h"

using namespace STATE_MACHINE_NAMESPACE;


CConstantImpl::CConstantImpl(int nId)
    : nId(nId)
    , Type(PT_LONG)
{
}


CConstantImpl::~CConstantImpl(void)
{
}


DEFINE_STRING_PROPERTY(CConstantImpl, Name);
DEFINE_STRING_PROPERTY(CConstantImpl, Comment);

//DEFINE_LONG_PROPERTY(CConstantImpl, Value);
HRESULT CConstantImpl::SaveValue(const StringType &szValue)
{
    Value.Convert(szValue);
    m_vValue = Value.Value();

    return S_OK;
}


StringType CConstantImpl::GetXMLOutput(CXMLBuilder *pXMLBuilder)
{
    if (pXMLBuilder == NULL)
        ((CXMLBuilder *)CConstantHandler::instance())->GetXMLOutput(this);
    else
        pXMLBuilder->GetXMLOutput(this);

    return (pXMLBuilder == NULL ? CConstantHandler::instance() : pXMLBuilder)->GetXMLOutput();
}


int CConstantImpl::GetId() const
{
	return nId;
}


const _TCHAR *CConstantImpl::GetComment()
{
    return Comment.Value().c_str();
}


long CConstantImpl::GetValue() const { return (long)m_vValue; }


const _TCHAR *CConstantImpl::GetName() 
{
    return Name.Value().c_str();
}


EPropertyType CConstantImpl::GetType()
{
    return Type.Value();
}


long CConstantImpl::GetInitialValue() { return Value.Value(); }


long CConstantImpl::GetMaxValue() { return Value.Value(); }


long CConstantImpl::GetMinValue() { return Value.Value(); } 


bool CConstantImpl::HasInitialValue() { return false; }


bool CConstantImpl::HasMaxValue() { return false; }


bool CConstantImpl::HasMinValue() { return false; }


void CConstantImpl::SetValue(long) {}
