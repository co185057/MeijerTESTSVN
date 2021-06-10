// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/ElseIfImpl.cpp 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)ElseIfImpl.cpp    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/ElseIfImpl.cpp $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 2     3/17/05 2:57p Dm185016
 * UNICODE fix
 * 
 * 1     3/17/05 2:46p Dm185016
 * <if>, <else-if>, <else> support
*/
#include "StdAfx.h"
#include "ElseIfImpl.h"

using namespace STATE_MACHINE_NAMESPACE;

int CElseIfImpl::counter = 0;


CElseIfImpl::CElseIfImpl()
    : nId(++counter)
{}


CElseIfImpl::~CElseIfImpl(void)
{}


int CElseIfImpl::GetId() const { return nId; }


//const _TCHAR *CElseIfImpl::GetComment() { return Comment.Value().c_str(); }


StringType CElseIfImpl::ToString()
{
    IStringBuffer sbRetValue(125);
    sbRetValue += _T("ElseIf(");
    sbRetValue += ExpressionValue.Value().c_str();
    sbRetValue += _T(")");

    StringType sRetValue = (const _TCHAR *)sbRetValue;

    return sRetValue;
}


_RuleTypes CElseIfImpl::GetRuleType() const { return ElseIfRule; }


StringType CElseIfImpl::GetXMLOutput(CXMLBuilder * /* pXMLBuilder */ )
{
    //if (pXMLBuilder == NULL)
    //    ((CXMLBuilder *)CWhenHandler::instance())->GetXMLOutput(this);
    //else
    //    pXMLBuilder->GetXMLOutput(this);

    //return (pXMLBuilder == NULL ? CWhenHandler::instance() : pXMLBuilder)->GetXMLOutput();
    return _T("");
}
