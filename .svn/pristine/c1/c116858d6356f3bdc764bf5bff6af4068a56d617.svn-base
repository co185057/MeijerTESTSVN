// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/content/ContextImpl.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)ContextImpl.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/content/ContextImpl.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
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
#include "ContextImpl.h"
#include "RAViews.h"

using namespace STATE_MACHINE_NAMESPACE;
using namespace fastlane;

int CContextImpl::m_nContextCounter = 0;


CContextImpl::CContextImpl(HTRACE hTrace, const StringType &name)
    : CBaseObject(hTrace)
    , Name(_T(""))
    , Comment(_T(""))
    , DefaultButton(_T(""))
    , View(DATANEEDED_NOVIEW)
{
    m_nId = ++m_nContextCounter;

    SaveName(name);
}


CContextImpl::~CContextImpl(void) throw()
{
}


DEFINE_STRING_PROPERTY(CContextImpl, Name);
DEFINE_STRING_PROPERTY(CContextImpl, Comment);
DEFINE_STRING_PROPERTY(CContextImpl, DefaultButton);
DEFINE_INT_PROPERTY(CContextImpl, View);


int CContextImpl::GetId() const
{
    return m_nId;
}


const _TCHAR *CContextImpl::GetComment()
{
    return Comment.Value().c_str();
}


const _TCHAR *CContextImpl::GetName() const throw()
{
    return Name.Value().c_str();
}


bool CContextImpl::HasDefaultButton() const throw()
{
    return DefaultButton.IsValid();
}


const _TCHAR *CContextImpl::GetDefaultButton() const throw()
{
    _ASSERT(DefaultButton.IsValid());
    return DefaultButton.Value().c_str();
}


bool CContextImpl::HasView() const throw()
{
    return View.IsValid();
}


int CContextImpl::GetView() const throw()
{
    _ASSERT(View.IsValid());
    return View.Value();
}


StringType CContextImpl::GetXMLOutput(CXMLBuilder *)
{
    //if (pXMLBuilder == NULL)
    //    ((CXMLBuilder *)CApplicationStateHandler::instance())->GetXMLOutput(this);
    //else
    //    pXMLBuilder->GetXMLOutput(this);

    //return (pXMLBuilder == NULL ? CApplicationStateHandler::instance() : pXMLBuilder)->GetXMLOutput();
    return _T("");
}
