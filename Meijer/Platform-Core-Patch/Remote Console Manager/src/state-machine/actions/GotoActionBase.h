#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/GotoActionBase.h 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)GotoActionBase.h    $Revision: 1 $ $Date: 6/20/08 1:11p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/GotoActionBase.h $
 * 
 * 1     6/20/08 1:11p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 2     2/28/05 3:50p Dm185016
 * lint
 * 
 * 1     1/26/05 2:28p Dm185016
 * Moved to new Repository.
* 
* 1     11/09/04 3:37p Dm185016
* Support for state lookup tables
*/

#include "SMConstants.h"
#include "ActionBase.h"

namespace STATE_MACHINE_NAMESPACE 
{

class CStateLookupTable;

class STATEM_API CGotoActionBase
    : virtual public CActionBase
{

public:

    CGotoActionBase();
    virtual ~CGotoActionBase();

protected:

    HRESULT SetNextState(const _TCHAR *pszStateTableName,
                         const _TCHAR *pszKey);
    HRESULT SetNextState(int);

    virtual HRESULT ParseOperands()=0;
    virtual HRESULT NextOperands(CParameterList::iterator &iter, CParameterList::iterator &end);

    const _TCHAR *GetKey() { return m_sKey.c_str(); }
    const _TCHAR *GetStateTableName() { return m_sStateTableName.c_str(); }

private:

    HRESULT StateLookup(int &index,
                        const _TCHAR *pszStateTableName,
                        const _TCHAR *pszKey);

    // hide copy and assignment
    CGotoActionBase(const CGotoActionBase&);
    CGotoActionBase& operator = (const CGotoActionBase &);

    StringType m_sKey;
    StringType m_sStateTableName;
};

};