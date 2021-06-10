#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/LookupTableImpl.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)LookupTableImpl.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/LookupTableImpl.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 4     3/22/05 4:34p Dm185016
 * .000 merge support
 * 
 * 3     3/03/05 3:18p Dm185016
 * TAR 291700 - Changed type of value-type to integer from string.
 * 
 * 2     2/28/05 3:46p Dm185016
 * lint
 * 
 * 1     2/14/05 7:06p Dm185016
 * Release Exclusive Access Event support
*/
#include "statem.h"
#include "SMConstants.h"
#include "XMLContent.h"
#include "LookupTable.h"
#include <map>

#pragma warning( push )
#pragma warning( disable : 4251 )

namespace STATE_MACHINE_NAMESPACE
{


typedef std::map<StringType, StringType, StringComparator> CStringStringLookupTable;

class STATEM_API CLookupTableImpl
    : public ILookupTable
    , public CXMLContent
    , public CPoolObject<CLookupTableImpl>
{
public:

    static const int TYPE_STRING     = 1;
    static const int TYPE_INTEGER    = 2;

    typedef CStringStringLookupTable map_t;

    CLookupTableImpl(int);
    virtual ~CLookupTableImpl(void);

    DECLARE_STRING_PROPERTY(Name);
    DECLARE_STRING_PROPERTY(Key);
    DECLARE_STRING_PROPERTY(Value);
    DECLARE_STRING_PROPERTY(Comment);
    DECLARE_STRING_PROPERTY(KeyType);
    DECLARE_INT_PROPERTY(ValueType);

	virtual int GetId() const;

	virtual const _TCHAR *GetName();

	virtual const _TCHAR *GetComment();

    void AddEntry();

    virtual HRESULT Lookup(const _TCHAR *pszKey);
    virtual HRESULT Lookup(int nKey);

    virtual int GetInteger() const;
    virtual const _TCHAR *GetString() const;

    virtual void Merge(const CLookupTableImpl *);

public: 

    StringType GetXMLOutput(CXMLBuilder *pXMLBuilder=NULL);

protected:

    map_t m_mapLookups;

private:

    CLookupTableImpl();
    CLookupTableImpl(const CLookupTableImpl &);               // Hide the Copy construcor
    CLookupTableImpl &operator =(const CLookupTableImpl &);   // Hide the assignment operator

    StringType m_value;

    int m_nId;
};

}

#pragma warning( pop )
