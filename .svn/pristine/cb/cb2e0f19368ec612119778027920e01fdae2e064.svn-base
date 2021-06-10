#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/StateLookupTableImpl.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)StateLookupTableImpl.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/StateLookupTableImpl.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 4     3/22/05 4:34p Dm185016
 * .000 merge support
 * 
 * 3     2/28/05 3:46p Dm185016
 * lint
 * 
 * 2     2/10/05 5:54p Dm185016
 * Fixed problem with static ids used in SM.
 * 
 * 1     1/26/05 2:29p Dm185016
 * Moved to new Repository.
* 
* 1     11/09/04 3:38p Dm185016
* Support for state lookup tables
*/
#include "statem.h"
#include "SMConstants.h"
#include "XMLContent.h"
#include "StateLookupTable.h"
#include <map>

#pragma warning( push )
#pragma warning( disable : 4251 )

namespace STATE_MACHINE_NAMESPACE
{

typedef std::map<StringType, int, StringComparator> CLookupTable;

class STATEM_API CStateLookupTableImpl
    : public IStateLookupTable
    , public CXMLContent
    , public CPoolObject<CStateLookupTableImpl>
{
public:

    typedef CLookupTable map_t;

    CStateLookupTableImpl(int);
    virtual ~CStateLookupTableImpl(void);

    DECLARE_STRING_PROPERTY(Name);
    DECLARE_STRING_PROPERTY(Key);
    DECLARE_STRING_PROPERTY(StateName);
    DECLARE_STRING_PROPERTY(Comment);
    DECLARE_INT_PROPERTY(StateId);

	virtual int GetId() const;

	virtual const _TCHAR *GetName();

	virtual const _TCHAR *GetComment();

    void AddEntry();

    HRESULT Lookup(int &index,
                const _TCHAR *pszKey);

    virtual void Merge(const CStateLookupTableImpl *);

public: 

    StringType GetXMLOutput(CXMLBuilder *pXMLBuilder=NULL);

protected:

    map_t m_mapLookups;

private:

    CStateLookupTableImpl();
    CStateLookupTableImpl(const CStateLookupTableImpl &);                     // Hide the Copy construcor
    CStateLookupTableImpl &operator =(const CStateLookupTableImpl &);   // Hide the assignment operator

	int m_nId;
};

}

#pragma warning( pop )
