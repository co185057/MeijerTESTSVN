#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/LookupTableHandler.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)LookupTableHandler.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/LookupTableHandler.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     2/14/05 7:05p Dm185016
 * Release Exclusive Access Event support
*/
#include "SMConstants.h"
#include "xmlbuilder.h"
#include "XPathHandler.h"
#include "singleton.h"
#include "MsgImpl.h"

namespace STATE_MACHINE_NAMESPACE 
{
class CLookupTableImpl;

class CLookupTableHandler 
    : public CXMLBuilder
    , public CXPathHandler
    , public CSingleton<CLookupTableHandler>
{
public:

    typedef CLookupTableImpl *content_type;

    friend class CSingleton<CLookupTableHandler>;

    ~CLookupTableHandler(void);

    virtual void registerHandlers(void);

    DECLARE_START_ELEMENT();
    DECLARE_END_ELEMENT();

    virtual void GetXMLOutput(CXMLContent *, CXMLBuilder *);

protected:

    DECLARE_HANDLER_BEGIN(Element);
    DECLARE_HANDLER_START_ELEMENT();
    DECLARE_HANDLER_END(Element);

private:
    CLookupTableHandler(void);
    CLookupTableHandler(CLookupTableHandler &);                     // Hidden Copy construcor
    CLookupTableHandler &operator =(const CLookupTableHandler &);   // Hidden assignment operator
};

};