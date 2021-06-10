// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/SelectImpl.cpp 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)SelectImpl.cpp    $Revision: 1 $ $Date: 6/20/08 1:12p $
 *
 * Copyright 2005 by NCR Corporation,
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/SelectImpl.cpp $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 3     3/11/05 1:20p Dm185016
 * <select> support
 * 
 * 2     2/28/05 3:46p Dm185016
 * lint
 * 
 * 1     1/26/05 2:29p Dm185016
 * Moved to new Repository.
// 
// 1     1/07/05 3:45p Dm185016
// Select support
*/
#include "StdAfx.h"
#include "SelectImpl.h"
#include "WhenHandler.h"

using namespace STATE_MACHINE_NAMESPACE;

int CSelectImpl::counter = 0;


CSelectImpl::CSelectImpl()
    : nId(++counter)
{}


CSelectImpl::~CSelectImpl(void)
{}


int CSelectImpl::GetId() const { return nId; }


//const _TCHAR *CSelectImpl::GetComment() { return Comment.Value().c_str(); }


StringType CSelectImpl::ToString()
{
    IStringBuffer sbRetValue(125);
    sbRetValue += _T("Select End  ");

    StringType sRetValue = (const _TCHAR *)sbRetValue;

    return sRetValue;
}


_RuleTypes CSelectImpl::GetRuleType() const { return SelectRule; }


StringType CSelectImpl::GetXMLOutput(CXMLBuilder * /* pXMLBuilder */ )
{
    //if (pXMLBuilder == NULL)
    //    ((CXMLBuilder *)CWhenHandler::instance())->GetXMLOutput(this);
    //else
    //    pXMLBuilder->GetXMLOutput(this);

    //return (pXMLBuilder == NULL ? CWhenHandler::instance() : pXMLBuilder)->GetXMLOutput();
    return _T("");
}
