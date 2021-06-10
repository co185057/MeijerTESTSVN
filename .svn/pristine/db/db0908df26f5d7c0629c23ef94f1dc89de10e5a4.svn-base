// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/WhenImpl.cpp 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)WhenImpl.cpp    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/WhenImpl.cpp $
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
// 2     1/12/05 6:32p Dm185016
// Select support.
// 
// 1     10/29/04 9:28a Dm185016
*/
#include "StdAfx.h"
#include "WhenImpl.h"
#include "WhenHandler.h"

using namespace STATE_MACHINE_NAMESPACE;

int CWhenImpl::counter = 0;


CWhenImpl::CWhenImpl()
    : nId(++counter)
{}


CWhenImpl::~CWhenImpl(void)
{}


DEFINE_STRING_PROPERTY(CWhenImpl, ExpressionValue);


int CWhenImpl::GetId() const { return nId; }


//const _TCHAR *CWhenImpl::GetComment() { return Comment.Value().c_str(); }


StringType CWhenImpl::ToString()
{
    IStringBuffer sbRetValue(125);
    sbRetValue += _T("When:  ");
    sbRetValue += ExpressionValue.Value().c_str();

    StringType sRetValue = (const _TCHAR *)sbRetValue;

    return sRetValue;
}


_RuleTypes CWhenImpl::GetRuleType() const { return WhenRule; }


StringType CWhenImpl::GetXMLOutput(CXMLBuilder * /* pXMLBuilder */ )
{
    //if (pXMLBuilder == NULL)
    //    ((CXMLBuilder *)CWhenHandler::instance())->GetXMLOutput(this);
    //else
    //    pXMLBuilder->GetXMLOutput(this);

    //return (pXMLBuilder == NULL ? CWhenHandler::instance() : pXMLBuilder)->GetXMLOutput();
    return _T("");
}
