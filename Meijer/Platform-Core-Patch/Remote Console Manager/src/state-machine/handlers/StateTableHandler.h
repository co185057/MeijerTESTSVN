#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/StateTableHandler.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)StateTableHandler.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/StateTableHandler.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:30p Dm185016
 * Moved to new Repository.
* 
* 1     11/09/04 3:38p Dm185016
* Support for state lookup tables
*/
#include "SMConstants.h"
#include "xmlbuilder.h"
#include "XPathHandler.h"
#include "singleton.h"
#include "MsgImpl.h"

namespace STATE_MACHINE_NAMESPACE 
{
class CStateLookupTableImpl;

class CStateTableHandler 
    : public CXMLBuilder
    , public CXPathHandler
    , public CSingleton<CStateTableHandler>
{
public:

    typedef CStateLookupTableImpl *content_type;

    friend class CSingleton<CStateTableHandler>;

    ~CStateTableHandler(void);

    virtual void registerHandlers(void);

    DECLARE_START_ELEMENT();
    DECLARE_END_ELEMENT();

    virtual void GetXMLOutput(CXMLContent *, CXMLBuilder *);

protected:

    DECLARE_HANDLER_BEGIN(State);
    DECLARE_HANDLER_START_ELEMENT();
    DECLARE_HANDLER_END(State);

private:
    CStateTableHandler(void);
    CStateTableHandler(CStateTableHandler &);                     // Hidden Copy construcor
    CStateTableHandler &operator =(const CStateTableHandler &);   // Hidden assignment operator
};

};