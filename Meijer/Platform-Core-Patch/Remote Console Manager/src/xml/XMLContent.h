#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/xml/XMLContent.h 1     6/20/08 1:14p Sa250050 $
/*
 * @(#)XMLContent.h  $Revision: 1 $ $Date: 6/20/08 1:14p $
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
*  $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/xml/XMLContent.h $
 * 
 * 1     6/20/08 1:14p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 3     5/03/05 4:11p Dm185016
 * Cleaned up tracing
 * 
 * 1     1/26/05 2:34p Dm185016
 * Moved to new Repository.
* 
* 5     10/05/04 8:35a Dm185016
* New base class CBaseObject to support tracing.
* 
* 3     1/27/04 1:48p Dm185016
* DLL Support
* 
* 2     1/16/04 9:35a Dm185016
* Moved property related macros to Property.h
*/

#include "wstrutils.h"
#include "ixmlbuilder.h"
#include "property.h"
#include "utils.h"

using namespace std;



class UTILS_API CXMLContent
    : public IXMLBuilder
    , virtual public CBaseObject
{
public:

    CXMLContent();
    virtual ~CXMLContent(void);

protected:

    ILoggable &loggable();

private:

    CXMLContent(const CXMLContent &);
    CXMLContent &operator =(const CXMLContent &);

    ILoggable *m_pLoggable;
};
