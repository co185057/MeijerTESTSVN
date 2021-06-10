#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/XPathHandler.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)XPathHandler.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/XPathHandler.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:30p Dm185016
 * Moved to new Repository.
* 
* 1     10/04/04 6:48p Dm185016
*/
#include "statem.h"
#include "SMConstants.h"
#include "ContentHandler.h"

namespace STATE_MACHINE_NAMESPACE 
{

class CSMLoader;

class STATEM_API CXPathHandler 
    : public CContentHandler
{
public:

    CXPathHandler();

    virtual ~CXPathHandler() {};

    ILoggable &xpathLoggable();

    void SetLoader(CSMLoader *pL);

    CSMLoader *GetLoader();

private:

    ILoggable *m_pLoggable;

    HTRACE m_hTrace;

    CSMLoader *m_pLoader;

};

};