// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/MsgImpl.cpp 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)MsgImpl.cpp    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/MsgImpl.cpp $
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
// 7     11/15/04 11:42a Dm185016
// Moved return code from action to message.
// 
// 5     3/26/04 11:28a Dm185016
// Added accessor for data length.
// 
// 4     2/24/04 3:41p Dm185016
// Added data field
// 
// 3     2/10/04 11:49a Dm185016
// Added Synchronous message support.
// 
// 2     1/16/04 9:22a Dm185016
// Namespace support, Removed <class> tag
*/
#include "StdAfx.h"
#include "MsgImpl.h"
#include "MsgHandler.h"

using namespace STATE_MACHINE_NAMESPACE;


CMsgImpl::CMsgImpl()
	: nId(-1)
{
}


CMsgImpl::CMsgImpl(int nId)
	: nId(nId)
{
}


CMsgImpl::~CMsgImpl(void)
{
}


DEFINE_STRING_PROPERTY(CMsgImpl, Name);
DEFINE_STRING_PROPERTY(CMsgImpl, Comment);
//DEFINE_STRING_PROPERTY(CMsgImpl, Class);
DEFINE_LONG_PROPERTY(CMsgImpl, Timeout);
DEFINE_LONG_PROPERTY(CMsgImpl, Value);


StringType CMsgImpl::GetXMLOutput(CXMLBuilder *pXMLBuilder)
{
    if (pXMLBuilder == NULL)
        ((CXMLBuilder *)CMsgHandler::instance())->GetXMLOutput(this);
    else
        pXMLBuilder->GetXMLOutput(this);

    return (pXMLBuilder == NULL ? CMsgHandler::instance() : pXMLBuilder)->GetXMLOutput();
}


int CMsgImpl::GetId() const
{
	return nId;
}


const _TCHAR *CMsgImpl::GetComment()
{
	return Comment.Value().c_str();
}


const _TCHAR *CMsgImpl::GetName()
{
	return Name.Value().c_str();
}


long CMsgImpl::GetTimeout()
{
	return Timeout.Value();
}


bool CMsgImpl::IsSynchronous() const
{
    return false;
}


void CMsgImpl::SetSynchronous(bool /* bSynchronous */ )
{
    // TODO:  Implement or delete this method
}


BSTR CMsgImpl::GetText() const
{
    return NULL;
}


void CMsgImpl::SetText(BSTR /* bstrText */ )
{
    // TODO:  Implement or delete this method
}


HRESULT CMsgImpl::GetRC() const
{
    return S_OK;
}


HRESULT CMsgImpl::Wait() const
{
    return S_OK;
}


void CMsgImpl::Post() const
{
}


void* CMsgImpl::GetData() const
{ 
    return NULL;
}


size_t CMsgImpl::GetDataLength() const
{ 
    return 0;
}
