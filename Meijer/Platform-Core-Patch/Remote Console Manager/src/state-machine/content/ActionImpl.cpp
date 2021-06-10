// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/ActionImpl.cpp 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)ActionImpl.cpp    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/ActionImpl.cpp $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
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
// 5     1/12/05 6:31p Dm185016
// Select support.
// 
// 3     4/30/04 1:01p Dm185016
// Removed unused tags
// 
// 2     1/16/04 9:19a Dm185016
// Namespace support, Action parameters
*/
#include "StdAfx.h"
#include "ActionImpl.h"
#include "ActionHandler.h"

using namespace STATE_MACHINE_NAMESPACE;


CActionImpl::CActionImpl(int nId)
	: nId(nId)
{
}


CActionImpl::~CActionImpl(void)
{
}


bool CActionImpl::IsInternal() 
{
    return false; 
}


DEFINE_STRING_PROPERTY(CActionImpl, Name);
DEFINE_STRING_PROPERTY(CActionImpl, Comment);


StringType CActionImpl::GetXMLOutput(CXMLBuilder *pXMLBuilder)
{
    if (pXMLBuilder == NULL)
        ((CXMLBuilder *)CActionHandler::instance())->GetXMLOutput(this);
    else
        pXMLBuilder->GetXMLOutput(this);

    return (pXMLBuilder == NULL ? CActionHandler::instance() : pXMLBuilder)->GetXMLOutput();
}


int CActionImpl::GetId() const
{
	return nId;
}


const _TCHAR *CActionImpl::GetComment()
{
	return Comment.Value().c_str();
}


const _TCHAR *CActionImpl::GetName()
{
	return Name.Value().c_str();
}


HRESULT CActionImpl::Execute(bool *pResult, IMessage &)
{
	*pResult = true;

	return S_OK;
}


void CActionImpl::AddParameter(const StringType & /* szType */, const StringType &szValue)
{
    m_vParms.push_back(szValue);
}


void CActionImpl::ClearParameters()
{
    m_vParms.clear();
}


StringType CActionImpl::ToString()
{
    StringType sRetValue = Name.Value();

    return sRetValue;
}
