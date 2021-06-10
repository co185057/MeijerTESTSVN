#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/LookupTable.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)LookupTable.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/LookupTable.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     2/14/05 7:12p Dm185016
 * Release Exclusive Access Event support
*/
#include "statem.h"
#include "SMConstants.h"
#include "StateMachineObject.h"
#include "LookupTable.h"
#include <map>

#pragma warning( push )
#pragma warning( disable : 4251 )

namespace STATE_MACHINE_NAMESPACE
{

class STATEM_API ILookupTable
    : public IStateMachineObject
{
public:

    virtual ~ILookupTable(void){};

    virtual HRESULT Lookup(const _TCHAR *pszKey)=0;
    virtual HRESULT Lookup(int nKey)=0;

    virtual int GetInteger() const=0;
    virtual const _TCHAR *GetString() const=0;
};

typedef ILookupTable *PLOOKUPTABLE;

typedef map<StringType, PLOOKUPTABLE, StringComparator> CLookupTablesMap;

};

#pragma warning( pop )
