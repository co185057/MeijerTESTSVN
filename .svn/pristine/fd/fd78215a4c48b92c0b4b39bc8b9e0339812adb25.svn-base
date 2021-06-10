#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/VariableImpl.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)VariableImpl.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/VariableImpl.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 5     5/31/05 11:40a Dm185016
 * String variable type support
 * 
 * 2     2/10/05 5:54p Dm185016
 * Fixed problem with static ids used in SM.
 * 
 * 1     1/26/05 2:29p Dm185016
 * Moved to new Repository.
* 
* 6     10/04/04 6:47p Dm185016
* Changed base class
* 
* 4     3/19/04 3:28p Dm185016
* Memory leak fix
* 
* 3     1/27/04 2:06p Dm185016
* Added header
*/
#include "XMLContent.h"
#include "Variable.h"
#include "DataModelTypes.h"

namespace STATE_MACHINE_NAMESPACE
{

class CVariableImpl
    : public CXMLContent
    , public IVariable
    , public CPoolObject<CVariableImpl>
{
public:

    CVariableImpl(int);
    virtual ~CVariableImpl(void);

	virtual int GetId() const;

	virtual const _TCHAR *GetComment();

    virtual const _TCHAR *GetName();

    virtual EPropertyType GetType();

    virtual bool HasInitialValue();

    virtual bool HasMaxValue();

    virtual bool HasMinValue();

    virtual long GetInitialValue();

    virtual long GetMaxValue();

    virtual long GetMinValue();

    virtual long GetValue() const;

    virtual void SetValue(long lNewValue);

    template <typename T>
    HRESULT GetInitialValue(T &);

    template <typename T>
    HRESULT GetMaxValue(T &);

    template <typename T>
    HRESULT GetMinValue(T &);

    template <typename T>
    HRESULT GetValue(T &) const;

    template <typename T>
    HRESULT SetValue(T lNewValue);

    DECLARE_STRING_PROPERTY(VarName);
    DECLARE_STRING_PROPERTY(Comment);
    DECLARE_LONG_PROPERTY(InitialValue);
    DECLARE_LONG_PROPERTY(MaxValue);
    DECLARE_LONG_PROPERTY(MinValue);
    DECLARE_ENUM_PROPERTY_WITH_TYPE(Type, EPropertyType);

public: 

    StringType GetXMLOutput(CXMLBuilder *pXMLBuilder=NULL);

private:

    CVariableImpl(CVariableImpl &);                     // Hide the Copy construcor
    CVariableImpl &operator =(const CVariableImpl &);   // Hide the assignment operator

    _variant_t m_vValue;

	int nId;
};


//-----------------------------------------------------------------------------
// Specialization for long variables
template <>
inline HRESULT CVariableImpl::GetInitialValue(long &pLong)
{
    pLong = 0;

    return S_OK;
}


template <>
inline HRESULT CVariableImpl::GetMaxValue(long &pLong)
{
    pLong = 0;

    return S_OK;
}

template <>
inline HRESULT CVariableImpl::GetMinValue(long &pLong)
{
    pLong = 0;

    return S_OK;
}

template <>
inline HRESULT CVariableImpl::GetValue(long &pLong) const
{
    pLong = 0;

    return S_OK;
}

template <>
inline HRESULT CVariableImpl::SetValue(long lNewValue)
{
    m_vValue = lNewValue;

    return S_OK;
}


//-----------------------------------------------------------------------------
// Specialization for string variables
template <>
inline HRESULT CVariableImpl::GetInitialValue(_bstr_t &pString)
{
    pString = _T("");

    return S_OK;
}


template <>
inline HRESULT CVariableImpl::GetMaxValue(_bstr_t &pLong)
{
    return E_NOTIMPL;
}

template <>
inline HRESULT CVariableImpl::GetMinValue(_bstr_t &pLong)
{
    return E_NOTIMPL;
}

template <>
inline HRESULT CVariableImpl::GetValue(_bstr_t &pLong) const
{
    pLong = (_bstr_t)m_vValue;

    return S_OK;
}

template <>
inline HRESULT CVariableImpl::SetValue(_bstr_t bstrNewValue)
{
    m_vValue = bstrNewValue;

    return S_OK;
}

};