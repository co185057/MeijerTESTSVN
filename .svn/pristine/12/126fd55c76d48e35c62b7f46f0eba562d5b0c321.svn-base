#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/LibraryHandler.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)SMConstants.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/LibraryHandler.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:30p Dm185016
 * Moved to new Repository.
* 
* 3     10/04/04 6:48p Dm185016
* New base class:  XPathHandler
* 
* 1     1/27/04 1:53p Dm185016
* New support for DLL libraries containing actions
*/
#include "xmlbuilder.h"
#include "XPathHandler.h"
#include "LibraryImpl.h"
#include "singleton.h"

namespace STATE_MACHINE_NAMESPACE 
{

class CLibraryHandler 
    : public CXMLBuilder
    , public CXPathHandler
    , public CSingleton<CLibraryHandler>
{
public:

    friend class CSingleton<CLibraryHandler>;

    ~CLibraryHandler(void);

    virtual void registerHandlers(void);

    DECLARE_START_ELEMENT();
    DECLARE_END_ELEMENT();

    virtual void GetXMLOutput(CXMLContent *, CXMLBuilder *);

    CLibraryImpl &library() { return *reinterpret_cast<CLibraryImpl *>(m_pContent); }

protected:

    DECLARE_HANDLER(Name);
    DECLARE_HANDLER(Comment);

private:

    CLibraryHandler(void);                                // Hidden constructor
    CLibraryHandler(CLibraryHandler &);                     // Hidden Copy construcor
    CLibraryHandler &operator =(const CLibraryHandler &);   // Hidden assignment operator

};

};