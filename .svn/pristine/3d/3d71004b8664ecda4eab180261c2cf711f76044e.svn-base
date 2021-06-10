#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/ConstantImpl.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)ConstantImpl.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/ConstantImpl.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 5     5/31/05 11:40a Dm185016
 * String variable type support
 * 
 * 2     2/10/05 5:50p Dm185016
 * Fixed problem with static ids used in SM.
 * 
 * 1     1/26/05 2:29p Dm185016
 * Moved to new Repository.
* 
* 5     10/04/04 6:47p Dm185016
* Changed base class
* 
* 3     3/19/04 3:28p Dm185016
* Memory leak fix
* 
* 2     1/27/04 2:05p Dm185016
* Added header
*/
#include "XMLContent.h"
#include "XMLContent.h"
#include "Constant.h"
#include "DataModelTypes.h"

namespace STATE_MACHINE_NAMESPACE
{

class CConstantImpl
    : public CXMLContent
    , public IConstant
    , public CPoolObject<CConstantImpl>
{
public:

    CConstantImpl(int);
    virtual ~CConstantImpl(void);

	virtual int GetId() const;

	virtual const _TCHAR *GetComment();

    virtual const _TCHAR *GetName();

    virtual EPropertyType GetType();

    virtual long GetMaxValue();

    virtual long GetMinValue();

    virtual long GetInitialValue();

    virtual bool HasInitialValue();

    virtual bool HasMaxValue();

    virtual bool HasMinValue();

    virtual long GetValue() const;

    virtual void SetValue(long lNewValue);

    DECLARE_STRING_PROPERTY(Name);
    DECLARE_STRING_PROPERTY(Comment);
    DECLARE_LONG_PROPERTY(Value);
    DECLARE_ENUM_PROPERTY_WITH_TYPE(Type, EPropertyType);

public: 

    StringType GetXMLOutput(CXMLBuilder *pXMLBuilder=NULL);

private:

    CConstantImpl(CConstantImpl &);                     // Hide the Copy construcor
    CConstantImpl &operator =(const CConstantImpl &);   // Hide the assignment operator

    _variant_t m_vValue;

	int nId;
};

};