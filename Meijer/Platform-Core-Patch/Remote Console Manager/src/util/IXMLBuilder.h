#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/IXMLBuilder.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)IXMLBuilder.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/IXMLBuilder.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:32p Dm185016
 * Moved to new Repository.
* 
* 3     6/30/04 3:40p Dm185016
* Lint.
* 
* 2     1/23/04 3:36p Dm185016
* DLL support
*/
#include "Utils.h"

class CXMLBuilder;

class UTILS_API IXMLBuilder 
{
public:

    virtual ~IXMLBuilder(void){};
	virtual StringType GetXMLOutput(CXMLBuilder *pXMLBuilder)=0;
};
