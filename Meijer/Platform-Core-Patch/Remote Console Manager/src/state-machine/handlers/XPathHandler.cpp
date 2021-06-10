// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/XPathHandler.cpp 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)XPathHandler.cpp    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/XPathHandler.cpp $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:30p Dm185016
 * Moved to new Repository.
// 
// 1     10/04/04 6:48p Dm185016
*/
#include "StdAfx.h"
#include "XPathHandler.h"
#include "Loggable.h"
#include "SMLoader.h"

using namespace STATE_MACHINE_NAMESPACE;


ILoggable &CXPathHandler::xpathLoggable() 
{ 
    if (m_pLoggable == NULL)
        m_pLoggable = &Trace::getLoggable(m_hTrace, eDCXPath);

    return *m_pLoggable; 
}


CXPathHandler::CXPathHandler() 
    : m_pLoggable(NULL)
    , m_hTrace(0)
{
}


void CXPathHandler::SetLoader(CSMLoader *pL) 
{ 
    m_pLoader = pL; 
    m_hTrace = pL->GetTraceHandle();
    SetTraceObject(m_hTrace);
}


CSMLoader *CXPathHandler::GetLoader() { return m_pLoader; }
