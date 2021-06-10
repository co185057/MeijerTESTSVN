// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/VariableImpl.cpp 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)VariableImpl.cpp    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/VariableImpl.cpp $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 5     5/31/05 11:40a Dm185016
 * String variable type support
 * 
 * 2     2/10/05 5:54p Dm185016
 * Fixed problem with static ids used in SM.
 * 
 * 1     1/26/05 2:29p Dm185016
 * Moved to new Repository.
// 
// 4     1/27/04 2:06p Dm185016
// Action Library support
// 
// 3     1/16/04 9:23a Dm185016
// Namespace support
*/
#include "StdAfx.h"
#include "VariableImpl.h"
#include "VariableHandler.h"

using namespace STATE_MACHINE_NAMESPACE;


CVariableImpl::CVariableImpl(int nId)
	: nId(nId)
    , Type(PT_LONG)
    //, lValue(0)
{
}


CVariableImpl::~CVariableImpl(void)
{
}


DEFINE_STRING_PROPERTY(CVariableImpl, VarName);
DEFINE_STRING_PROPERTY(CVariableImpl, Comment);
DEFINE_LONG_PROPERTY(CVariableImpl, MaxValue);
DEFINE_LONG_PROPERTY(CVariableImpl, MinValue);
DEFINE_ENUM_PROPERTY(CVariableImpl, Type);

//DEFINE_LONG_PROPERTY(CVariableImpl, InitialValue);
HRESULT CVariableImpl::SaveInitialValue(const StringType &szValue)
{
    InitialValue.Convert(szValue);

    m_vValue = InitialValue.Value();

    return S_OK;
}


StringType CVariableImpl::GetXMLOutput(CXMLBuilder *pXMLBuilder)
{
    if (pXMLBuilder == NULL)
        ((CXMLBuilder *)CVariableHandler::instance())->GetXMLOutput(this);
    else
        pXMLBuilder->GetXMLOutput(this);

    return (pXMLBuilder == NULL ? CVariableHandler::instance() : pXMLBuilder)->GetXMLOutput();
}


int CVariableImpl::GetId() const
{
	return nId;
}


const _TCHAR *CVariableImpl::GetComment()
{
    return Comment.Value().c_str();
}


long CVariableImpl::GetValue() const
{ 
    return (long)m_vValue; 
}


const _TCHAR *CVariableImpl::GetName() 
{
    return VarName.Value().c_str();
}


EPropertyType CVariableImpl::GetType()
{
    return Type.Value();
}


long CVariableImpl::GetInitialValue()
{
	return InitialValue.Value();
}


long CVariableImpl::GetMaxValue()
{
	return MaxValue.Value();
}


long CVariableImpl::GetMinValue()
{
	return MinValue.Value();
}


bool CVariableImpl::HasInitialValue()
{
	return InitialValue.IsValid();
}


bool CVariableImpl::HasMaxValue()
{
	return MaxValue.IsValid();
}


bool CVariableImpl::HasMinValue()
{
	return MinValue.IsValid();
}


void CVariableImpl::SetValue(long lNewValue)
{ 
    m_vValue = lNewValue; 
}
