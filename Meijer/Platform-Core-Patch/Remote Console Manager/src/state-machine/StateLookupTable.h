#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/StateLookupTable.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)StateLookupTable.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
 *
 * Copyright 2003 by NCR Corporation,
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/StateLookupTable.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:28p Dm185016
 * Moved to new Repository.
* 
* 1     11/10/04 3:20p Dm185016
* 
* 1     11/09/04 3:35p Dm185016
* Support for state lookup tables
*/
#include "statem.h"
#include "SMConstants.h"
#include "StateMachineObject.h"
#include "StateLookupTable.h"
#include <map>

#pragma warning( push )
#pragma warning( disable : 4251 )

namespace STATE_MACHINE_NAMESPACE
{

class STATEM_API IStateLookupTable
    : public IStateMachineObject
{
public:

    virtual ~IStateLookupTable(void){};

    virtual HRESULT Lookup(int &index, const _TCHAR *pszKey)=0;
};

typedef IStateLookupTable *PSTATELOOKUPTABLE;

typedef map<StringType, PSTATELOOKUPTABLE, StringComparator> CStateLookupTablesMap;

};

#pragma warning( pop )
