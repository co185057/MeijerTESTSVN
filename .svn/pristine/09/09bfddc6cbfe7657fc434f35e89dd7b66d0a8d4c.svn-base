#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/OtherwiseHandler.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)OtherwiseHandler.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/OtherwiseHandler.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:30p Dm185016
 * Moved to new Repository.
* 
* 1     12/22/04 11:25a Dm185016
* Conditional logic support.
*/

#include "SMConstants.h"
#include "xmlbuilder.h"
#include "XPathHandler.h"
#include "WhenImpl.h"
#include "singleton.h"

namespace STATE_MACHINE_NAMESPACE
{

class CSelectHandler;

class COtherwiseHandler 
    : public CXMLBuilder
    , public CXPathHandler
    , public CSingleton<COtherwiseHandler>
{
public:

    friend class CSingleton<COtherwiseHandler>;

    ~COtherwiseHandler(void);

    virtual void registerHandlers(void);

    DECLARE_START_ELEMENT();
    DECLARE_END_ELEMENT();

    virtual void GetXMLOutput(CXMLContent *, CXMLBuilder *);

    CWhenImpl &otherwise() { return *reinterpret_cast<CWhenImpl *>(m_pContent); }

    void SetOwner(CSelectHandler *pOwner);

protected:
    
    void Reference(std::wstring szValue);

private:
    COtherwiseHandler(void);
    COtherwiseHandler(COtherwiseHandler &);                     // Hidden Copy construcor
    COtherwiseHandler &operator =(const COtherwiseHandler &);   // Hidden assignment operator

    CSelectHandler *m_pOwner;
};

};