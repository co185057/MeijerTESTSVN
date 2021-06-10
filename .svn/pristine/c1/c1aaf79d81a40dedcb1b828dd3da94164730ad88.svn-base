// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/content/ControlImpl.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ControlImpl.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/content/ControlImpl.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     2/28/05 4:24p Dm185016
 * lint
 * 
 * 1     1/26/05 2:24p Dm185016
 * Moved to new Repository.
// 
// 2     11/16/04 11:45a Dm185016
// Cleaned up code
// 
// 1     10/29/04 9:17a Dm185016
*/
#include "StdAfx.h"
#include "ControlImpl.h"
#include "SMConstants.h"

using namespace STATE_MACHINE_NAMESPACE;
using namespace fastlane;

int CControlImpl::m_nControlCounter = 0;


CControlImpl::CControlImpl(HTRACE hTrace, const StringType &name, const StringType &type)
    : CBaseObject(hTrace)
    , m_bIgnored(false)
{
//  IMODTRACE_DEVELOP();

    m_nId = ++m_nControlCounter;

    SaveName(name);
    SaveType(type);
}


CControlImpl::~CControlImpl(void) throw()
{
}


DEFINE_STRING_PROPERTY(CControlImpl, Name);
DEFINE_STRING_PROPERTY(CControlImpl, Type);
DEFINE_STRING_PROPERTY(CControlImpl, Comment);


int CControlImpl::GetId() const
{
    return m_nId;
}


const _TCHAR *CControlImpl::GetComment() const throw()
{
    return Comment.Value().c_str();
}


const _TCHAR *CControlImpl::GetName() const throw()
{
    return Name.Value().c_str();
}


const _TCHAR *CControlImpl::GetType() const throw()
{
    return Type.Value().c_str();
}


void CControlImpl::SetIgnored(bool bIgnore) { m_bIgnored = bIgnore; }
bool CControlImpl::IsIgnored() const throw() { return m_bIgnored; }


StringType CControlImpl::GetXMLOutput(CXMLBuilder *)
{
    //if (pXMLBuilder == NULL)
    //    ((CXMLBuilder *)CApplicationStateHandler::instance())->GetXMLOutput(this);
    //else
    //    pXMLBuilder->GetXMLOutput(this);

    //return (pXMLBuilder == NULL ? CApplicationStateHandler::instance() : pXMLBuilder)->GetXMLOutput();
    return _T("");
}
