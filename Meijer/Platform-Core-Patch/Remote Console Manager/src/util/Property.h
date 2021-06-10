#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/Property.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)Property.h  $Revision: 1 $ $Date: 6/20/08 1:13p $
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
*  $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/Property.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 7     6/14/05 9:02a Dm185016
 * Added default no-args constructor.
 * 
 * 5     5/03/05 4:10p Dm185016
 * Cleaned up tracing
 * 
 * 3     3/24/05 7:30p Dm185016
 * UNICODE
 * 
 * 2     2/28/05 3:33p Dm185016
 * lint
 * 
 * 1     1/26/05 2:32p Dm185016
 * Moved to new Repository.
* 
* 9     1/18/05 4:01p Dm185016
* UNICODE support
* 
* 8     10/29/04 9:33a Dm185016
* Added trace
* 
* 6     6/30/04 3:44p Dm185016
* Lint
* New macro to allow enum property to have different name from type.
* 
* 5     5/21/04 10:44a Dm185016
* lint
* 
* 4     3/19/04 3:31p Dm185016
* Fixed problem with valid flag
* 
* 3     1/23/04 3:36p Dm185016
* DLL support
* 
* 2     1/16/04 9:31a Dm185016
* Moved property macros from XMLContent to here
*/

#include "wstrutils.h"


// Need to ensure that these are defined appropriately
#ifndef _L
    #define __L(x) L##x
    #define _L(x) __L(x)
#endif

#define DECLARE_FLOAT_PROPERTY(PROPERTY)\
public :\
    CFloatProperty PROPERTY;\
    HRESULT Save##PROPERTY(const StringType &szValue)

#define DECLARE_BOOLEAN_PROPERTY(PROPERTY)\
public :\
    CBoolProperty PROPERTY;\
    HRESULT Save##PROPERTY(const StringType &szValue)

#define DECLARE_INT_PROPERTY(PROPERTY)\
public :\
    CIntProperty PROPERTY;\
    HRESULT Save##PROPERTY(const StringType &szValue)

#define DECLARE_SHORT_PROPERTY(PROPERTY)\
public :\
    CShortProperty PROPERTY;\
    HRESULT Save##PROPERTY(const StringType &szValue)

#define DECLARE_LONG_PROPERTY(PROPERTY)\
public :\
    CLongProperty PROPERTY;\
    HRESULT Save##PROPERTY(const StringType &szValue)

#define DECLARE_ULONG_PROPERTY(PROPERTY)\
public :\
    CUnsignedLongProperty PROPERTY;\
    HRESULT Save##PROPERTY(const StringType &szValue)

#define DECLARE_STRING_PROPERTY(PROPERTY)\
public :\
    CStringProperty PROPERTY;\
    HRESULT Save##PROPERTY(const StringType &szValue)

#define DECLARE_ENUM_PROPERTY(PROPERTY)\
public :\
    CEnumProperty<PROPERTY##s> PROPERTY;\
    HRESULT Save##PROPERTY(const StringType &szValue)

#define DECLARE_ENUM_PROPERTY_WITH_TYPE(PROPERTY, TYPE)\
public :\
    CEnumProperty<TYPE> PROPERTY;\
    HRESULT Save##PROPERTY(const StringType &szValue)

#define DECLARE_OBJECT_PROPERTY(PROPERTY, OBJECT)\
public :\
    CObjectProperty<C##OBJECT *> PROPERTY;

#define DECLARE_FORM_PROPERTY(PROPERTY)\
    DECLARE_OBJECT_PROPERTY(PROPERTY, Form)

//-----------------------------------------------------------------------
// Macro used to save the data of a given tag.
// @param BASE The name of the enclosing class for this handler.
// @param ID The name of tag.
// @param INSTANCE The name of the method that returns an instance
//      of the data structure associated with this handler.
//-----------------------------------------------------------------------
#ifdef __ALWAYS_LOG_PROPERTY__
    #define CHECK_LOGGING 
#else
    #define CHECK_LOGGING if (loggable().IsDebug())
#endif

#define DEFINE_PROPERTY(BASE, PROPERTY) \
    HRESULT BASE::Save##PROPERTY(const StringType &szValue)\
    {\
        __if_exists(BASE::loggable)\
        {\
            if (loggable().IsDebug()) ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  szValue=") + IString(szValue.c_str()));\
        }\
        __if_not_exists(BASE::loggable)\
        {\
            ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  szValue=") + IString(szValue.c_str()));\
        }\
        PROPERTY.Convert(szValue);\
        return S_OK;\
    }\


    //-----------------------------------------------------------------------
// Macro used to save the data of a given tag as a short variable.
// @param BASE The name of the enclosing class for this handler.
// @param ID The name of tag.
// @param INSTANCE The name of the method that returns an instance
//      of the data structure associated with this handler.
//-----------------------------------------------------------------------
#define DEFINE_INT_PROPERTY(BASE, PROPERTY) \
    DEFINE_PROPERTY(BASE, PROPERTY)


//-----------------------------------------------------------------------
// Macro used to save the data of a given tag as a long variable.
// @param BASE The name of the enclosing class for this handler.
// @param ID The name of tag.
// @param INSTANCE The name of the method that returns an instance
//      of the data structure associated with this handler.
//-----------------------------------------------------------------------
#define DEFINE_LONG_PROPERTY(BASE, PROPERTY) \
    DEFINE_PROPERTY(BASE, PROPERTY)

//-----------------------------------------------------------------------
// Macro used to save the data of a given tag as a long variable.
// @param BASE The name of the enclosing class for this handler.
// @param PROPERTY The name of tag.
// @param INSTANCE The name of the method that returns an instance
//      of the data structure associated with this handler.
//-----------------------------------------------------------------------
#define DEFINE_ULONG_PROPERTY(BASE, PROPERTY) \
    DEFINE_PROPERTY(BASE, PROPERTY)

//-----------------------------------------------------------------------
// Macro used to save the data of a given tag as a short variable.
// @param BASE The name of the enclosing class for this handler.
// @param PROPERTY The name of tag.
// @param INSTANCE The name of the method that returns an instance
//      of the data structure associated with this handler.
//-----------------------------------------------------------------------
#define DEFINE_SHORT_PROPERTY(BASE, PROPERTY) \
    DEFINE_PROPERTY(BASE, PROPERTY)

//-----------------------------------------------------------------------
// Macro used to save the data of a given tag as a boolean variable.
// @param BASE The name of the enclosing class for this handler.
// @param PROPERTY The name of tag.
// @param INSTANCE The name of the method that returns an instance
//      of the data structure associated with this handler.
//-----------------------------------------------------------------------
#define DEFINE_BOOLEAN_PROPERTY(BASE, PROPERTY) \
    DEFINE_PROPERTY(BASE, PROPERTY)

//-----------------------------------------------------------------------
// Macro used to save the data of a given tag as a CStdString variable.
// @param BASE The name of the enclosing class for this handler.
// @param PROPERTY The name of tag.
// @param INSTANCE The name of the method that returns an instance
//      of the data structure associated with this handler.
//-----------------------------------------------------------------------
#define DEFINE_STRING_PROPERTY(BASE, PROPERTY) \
    DEFINE_PROPERTY(BASE, PROPERTY)

//-----------------------------------------------------------------------
// Macro used to save the data of a given tag as a CStdString variable.
// @param BASE The name of the enclosing class for this handler.
// @param PROPERTY The name of tag.
// @param INSTANCE The name of the method that returns an instance
//      of the data structure associated with this handler.
//-----------------------------------------------------------------------
#define DEFINE_ENUM_PROPERTY(BASE, PROPERTY) \
    HRESULT BASE::Save##PROPERTY(const StringType &szValue)\
    {\
        PROPERTY.Value(PROPERTY.Translate(szValue));\
        return S_OK;\
    }\

//-----------------------------------------------------------------------
// Macro used to save the data of a given tag as a CStdString variable.
// @param BASE The name of the enclosing class for this handler.
// @param PROPERTY The name of tag.
// @param INSTANCE The name of the method that returns an instance
//      of the data structure associated with this handler.
//-----------------------------------------------------------------------
#define DEFINE_FLOAT_PROPERTY(BASE, PROPERTY) \
    DEFINE_PROPERTY(BASE, PROPERTY)

#define DEFINE_FORM_PROPERTY(BASE, PROPERTY)

#define DEFINE_OBJECT_PROPERTY(BASE, PROPERTY, OBJECT)


//-----------------------------------------------------------------------------
// Interface class providing the means of determining whether a property has
// been set to any value other than the default value.
class IValidatable
{
public:
    virtual ~IValidatable(){};
    virtual bool IsValid() const=0;
};


//-----------------------------------------------------------------------------
// main template class for encapsulating a property of an object.
template <typename T>
class CProperty
    : public IValidatable
{
public:
    CProperty();
    inline CProperty(const T &initial);
    inline virtual ~CProperty();
    inline const T &Value() const;
    inline void Value(const T & value);
    inline virtual void Convert(const StringType &szValue);
    inline bool IsValid() const;

protected:
    T m_value;
    bool m_bValid;
};


//-----------------------------------------------------------------------------
// typedefs for the various properties
typedef CProperty<short> CShortProperty;
typedef CProperty<long> CLongProperty;
typedef CProperty<unsigned long> CUnsignedLongProperty;
typedef CProperty<int> CIntProperty;
typedef CProperty<bool> CBoolProperty;
typedef CProperty<float> CFloatProperty;
typedef CProperty<StringType> CStringProperty;


//-----------------------------------------------------------------------------
// Interface class providing the means of converting the enumeration into a
// string.
class IEnum
{
public:
    virtual ~IEnum(){};
    virtual StringType ToString() const=0;
};


//-----------------------------------------------------------------------------
// template class for managing an enumerated property
template <typename T>
class CEnumProperty 
    : public CProperty<T>
    , public IEnum
{
public:
    CEnumProperty();
    CEnumProperty(const T &initialValue);
    virtual ~CEnumProperty();
    void Convert(const StringType &szValue);
    T Translate(const StringType &szValue) const;
    StringType ToString() const;
};


//-----------------------------------------------------------------------------
// template class for encapsulating a object pointer.
template <typename T>
class CObjectProperty : public CProperty<T>
{
public:
    CObjectProperty();
    virtual ~CObjectProperty();
};


//-----------------------------------------------------------------------------
// Constructor for the base property template
//lint -e{1401}
template <typename T>
inline
CProperty<T>::CProperty()
    : m_bValid(false)
{
}


//-----------------------------------------------------------------------------
// Constructor for the base property template
template <typename T>
inline
CProperty<T>::CProperty(const T & initial)
    : m_value(initial)
    , m_bValid(false)
{
}


//-----------------------------------------------------------------------------
// Destructor for the base property template
template <typename T>
inline
CProperty<T>::~CProperty()
{
}


//-----------------------------------------------------------------------------
// Getter method to return the value of the property
template <typename T>
inline
const T &CProperty<T>::Value() const
{
    return m_value;
}


//-----------------------------------------------------------------------------
// Setter method to set the value of the property
template <typename T>
inline
void CProperty<T>::Value(const T & value)
{
    m_value = value;
    m_bValid = true;
}


//-----------------------------------------------------------------------------
// Returns true if the property has been set to some value other than the
// default set during creation.  Returns false otherwise.
template <typename T>
inline
bool CProperty<T>::IsValid() const
{
    return m_bValid;
}


//-----------------------------------------------------------------------------
// Specialization of Convert() for string property
template <>
inline CProperty<StringType>::CProperty<StringType>()
    : m_value(_T(""))
	, m_bValid(false)
{
}


//-----------------------------------------------------------------------------
// Specialization of Convert() for string property
template <>
inline void CStringProperty::Convert(const StringType &szValue)
{
    Value(szValue);
}


//-----------------------------------------------------------------------------
// Specialization of Convert() for int property
template <>
inline void CIntProperty::Convert(const StringType &szValue)
{
    Value(_tstoi(szValue.c_str()));
}


//-----------------------------------------------------------------------------
// Specialization of Convert() for long property
template <>
inline void CLongProperty::Convert(const StringType &szValue)
{
    Value(_tstol(szValue.c_str()));
}


//-----------------------------------------------------------------------------
// Specialization of Convert() for unsigned long property
template <>
inline void CUnsignedLongProperty::Convert(const StringType &szValue)
{
    Value(_tcstoul(szValue.c_str(), NULL, 10));
}


//-----------------------------------------------------------------------------
// Specialization of Convert() for short property
template <>
inline void CShortProperty::Convert(const StringType &szValue)
{
    Value((short)_tstoi(szValue.c_str()));
}


//-----------------------------------------------------------------------------
// Specialization of Convert() for bool property
template <>
inline void CBoolProperty::Convert(const StringType &szValue)
{
	Value((_tstol(szValue.c_str()) != 0) && ((_tcsicmp(szValue.c_str(), _T("true")) == 0) || (szValue.c_str()[0]+0x20 ==  'y')) );
}


//-----------------------------------------------------------------------------
// Specialization of Convert() for float property
template <>
inline void CFloatProperty::Convert(const StringType &szValue)
{
    Value((float)_tstof(szValue.c_str()));
}


//-----------------------------------------------------------------------------
// Constructor for enum property
template <typename T>
inline
CEnumProperty<T>::CEnumProperty()
{
//  m_value = initialValue;
}


//-----------------------------------------------------------------------------
// Constructor for enum property
template <typename T>
inline
CEnumProperty<T>::CEnumProperty(const T &initialValue)
    : CProperty<T>(initialValue)
{
//  m_value = initialValue;
}


//-----------------------------------------------------------------------------
// Destructor for enum property
template <typename T>
inline
CEnumProperty<T>::~CEnumProperty()
{}


template <typename T>
inline void CEnumProperty<T>::Convert(const StringType &szValue)
{
    Value(Translate(szValue));
}


//-----------------------------------------------------------------------------
// Constructor for generic object property that manages a pointer to the
// created object
template <typename T>
inline
CObjectProperty<T>::CObjectProperty()
{
    Value(NULL);
}


//-----------------------------------------------------------------------------
// Destructor for generic object property
template <typename T>
inline
CObjectProperty<T>::~CObjectProperty()
{
    delete m_value;
}
