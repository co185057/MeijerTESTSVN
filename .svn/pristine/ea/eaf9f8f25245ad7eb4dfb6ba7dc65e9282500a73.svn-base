#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/ConstantHandler.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)ConstantHandler.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/ConstantHandler.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 4     5/31/05 11:40a Dm185016
 * String variable type support
 * 
 * 1     1/26/05 2:30p Dm185016
 * Moved to new Repository.
* 
* 4     10/04/04 6:48p Dm185016
* New base class:  XPathHandler
* 
* 2     1/27/04 1:51p Dm185016
* DLL Support
*/
#include "xmlbuilder.h"
#include "XPathHandler.h"
#include "ConstantImpl.h"
#include "singleton.h"

namespace STATE_MACHINE_NAMESPACE
{

class CConstantHandler 
    : public CXMLBuilder
    , public CXPathHandler
    , public CSingleton<CConstantHandler>
{
public:

    typedef CConstantImpl *content_type;

    friend class CSingleton<CConstantHandler>;

    ~CConstantHandler(void);

    virtual void registerHandlers(void);

    DECLARE_START_ELEMENT();
    DECLARE_END_ELEMENT();

    virtual void GetXMLOutput(CXMLContent *, CXMLBuilder *);

    CConstantImpl &constant() { return *reinterpret_cast<CConstantImpl *>(m_pContent); }

protected:
    
    void Reference(std::wstring szValue);

    DECLARE_HANDLER(Name);
    DECLARE_HANDLER(Comment);
    DECLARE_HANDLER(Value);

private:
    CConstantHandler(void);
    CConstantHandler(CConstantHandler &);                     // Hidden Copy construcor
    CConstantHandler &operator =(const CConstantHandler &);   // Hidden assignment operator

};

};