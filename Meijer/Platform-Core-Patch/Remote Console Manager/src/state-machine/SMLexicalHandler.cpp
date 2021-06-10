// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/SMLexicalHandler.cpp 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)SMLexicalHandler.cpp      $Revision: 1 $  $Date: 6/20/08 1:13p $
 *
 * Copyright 2004, NCR Corporation.
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/SMLexicalHandler.cpp $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:27p Dm185016
 * Moved to new Repository.
// 
// 3     10/29/04 9:23a Dm185016
// Made trace optional
// 
// 1     6/30/04 3:26p Dm185016
// Added handler for comments.
*/
#include "stdafx.h"
#include "SMConstants.h"
#include "SMLexicalHandler.h"
#include "XMLUtilities.h"

using namespace STATE_MACHINE_NAMESPACE;


HRESULT STDMETHODCALLTYPE CSMLexicalHandler::comment (
    /*[in]*/ unsigned short * pwchChars,
    /*[in]*/ int cchChars )
{
    StringType szElement = XMLUtilities::getString(pwchChars, cchChars);

    if (loggable().IsTracing())
        ITRACE_DEVELOP(_T("Comment:  \"") + IString(szElement.c_str()) + _T("\"."));

    if (szElement[0] == '$')
    {
        ITRACE_DEVELOP(_T("Found StateMachine processing element!"));
    }

	return S_OK;
}


ILoggable &CSMLexicalHandler::loggable() const
{ 
    if (m_pLoggable == NULL)
        m_pLoggable = &Trace::getLoggable(GetTraceHandle(), eDCContentHandler);

    return *m_pLoggable; 
}
