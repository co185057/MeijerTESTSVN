#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/DataModelTypes.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)DataModelTypes.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/DataModelTypes.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     5/31/05 11:36a Dm185016
 * Data Model Support
*/

#include "property.h"
#include "SMConstants.h"
#include "StringComparator.h"

typedef enum
{
    PT_NONE     = VT_EMPTY,
    PT_BOOL     = VT_BOOL,
    PT_INTEGER  = VT_I4,
    PT_LONG     = VT_I4,
    PT_STRING   = VT_LPSTR
} EPropertyType;


    //CEnumProperty(const T &initialValue);
    //virtual ~CEnumProperty();


template <>
inline void CProperty<EPropertyType>::Convert(const StringType &)
{
}


template <>
inline void CEnumProperty<EPropertyType>::Convert(const StringType &sValue)
{
    m_value = Translate(sValue);
}


template <>
inline StringType CEnumProperty<EPropertyType>::ToString() const
{
    StringType retValue;
    switch (Value())
    {
        case PT_NONE:       retValue = XP_VARIABLE_TYPE_ATTR_NONE;             break; 
        case PT_BOOL:       retValue = XP_VARIABLE_TYPE_ATTR_BOOL;   break;
        case PT_INTEGER:    retValue = XP_VARIABLE_TYPE_ATTR_INT;    break;
        //case PT_LONG:       retValue = XP_VARIABLE_TYPE_ATTR_LONG;    break;
        case PT_STRING:     retValue = XP_VARIABLE_TYPE_ATTR_STRING;    break;
    }

    return retValue;
}


template <>
inline EPropertyType CEnumProperty<EPropertyType>::Translate(const StringType &szValue) const
{
    EPropertyType retValue;
    if(szValue == XP_VARIABLE_TYPE_ATTR_NONE)
        retValue = PT_NONE;
    else if(szValue == XP_VARIABLE_TYPE_ATTR_BOOL)
        retValue = PT_BOOL;
    else if(szValue == XP_VARIABLE_TYPE_ATTR_INT)
        retValue = PT_INTEGER;
    else if(szValue == XP_VARIABLE_TYPE_ATTR_LONG)
        retValue = PT_LONG;
    else if(szValue == XP_VARIABLE_TYPE_ATTR_STRING)
        retValue = PT_STRING;
    else
    {
        ASSERT(0);
    }

    return retValue;
}
