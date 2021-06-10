#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/DataModel.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)DataModel.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/DataModel.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     5/31/05 11:36a Dm185016
 * Data Model Support
*/

#include <map>
#include "DataModelTypes.h"
#include "statem.h"

#define BEGIN_PROPERTIES_TABLE(FQCLASS)\
    SDMEntry FQCLASS::_dm[] = {

#define DEFINE_BOOL_PROPERTY(PROPNAME)\
    { NULL,NULL, PT_BOOL, NULL, NULL },

#define END_PROPERTIES_TABLE()\
    { NULL,NULL, PT_NONE, NULL, NULL } };


#define BEGIN_DATA_MODEL(FQCLASS, OBJNAME)\
static int _dm_property_count_##FQCLASS##_##OBJNAME = 0;

#define DATA_MODEL_BOOL_PROPERTY(FQCLASS, OBJNAME, PROPNAME)\
COleVariant FQCLASS::_dm_property_Is##PROPNAME(const _TCHAR *pszPropName) { COleVariant var = VARIANT_TRUE; return var; }\
void FQCLASS::_dm_property_Set##PROPNAME(const _TCHAR *pszPropName, const COleVariant &vNewValue) { }\
static CPropInitializer _dm_property_init_##FQCLASS##OBJNAME##PROPNAME(\
    &FQCLASS::_dm[_dm_property_count_##FQCLASS##_##OBJNAME++],\
    QUOTE(OBJNAME),\
    QUOTE(PROPNAME),\
    PT_BOOL,\
    &FQCLASS::_dm_property_Is##PROPNAME,\
    &FQCLASS::_dm_property_Set##PROPNAME);


#define END_DATA_MODEL()

#define DECLARE_DATA_MODEL_BEGIN(FQNAME)\
public:\
    static SDMEntry _dm[];

#define DECLARE_DATA_MODEL_BOOL_PROPERTY(PROPNAME)\
public:\
    static COleVariant _dm_property_Is##PROPNAME(const _TCHAR *);\
    static void _dm_property_Set##PROPNAME(const _TCHAR *, const COleVariant &);

#define DECLARE_DATA_MODEL_END()

typedef COleVariant (*PDMGETTER)(const _TCHAR *);
typedef void (*PDMSETTER)(const _TCHAR *, const COleVariant &);


typedef struct _DM_Entry
{
    const _TCHAR   *pszObjectName;
    const _TCHAR   *pszPropertyName;
    EPropertyType   eType;
    PDMGETTER       pGetter;
    PDMSETTER       pSetter;
} SDMEntry;


class CPropInitializer
{
public:
    CPropInitializer(SDMEntry *pEntry, const _TCHAR   *pszObjectName,
        const _TCHAR   *pszPropertyName,
        EPropertyType   eType,
        PDMGETTER       pGetter,
        PDMSETTER       pSetter)
    {
        pEntry->pszObjectName   = pszObjectName;
        pEntry->pszPropertyName = pszPropertyName;
        pEntry->eType           = eType;
        pEntry->pGetter         = pGetter;
        pEntry->pSetter         = pSetter;
    }
};

class CDataModelBase;


// the iterator
template <typename _Tp>
class _dm_iterator 
    : public std::iterator<std::forward_iterator_tag, _Tp>
{
    friend class CKeyValueParmList;

public:

    typedef SDMEntry _node;
    
    friend class CDataModelBase;

    inline const _node &operator->()
    {
        return *m_pNode;
    }
    

    inline const _node &operator*()
    {
        return *m_pNode;
    }
    

    /// <summary>Postfix operator ++ is used to get the next table entry.  The
    /// int parameter is not used.  It is just a placeholder to force the postfix</summary>
    inline _dm_iterator<_Tp> & operator ++(int)
    {
        m_pNode++;

        return *this;
    }
    

    /// <summary>Prefix operator ++ is used to get the next value using 
    /// the same key as the previous usage</summary>
    inline _dm_iterator<_Tp> & operator ++()
    {
        m_pNode++;

        return *this;
    }


    /// <summary>Operator != returns true if the two objects are not equal</summary>
    inline bool operator!=(const _dm_iterator<_Tp> & rhs) const
    {
        return (m_pNode != rhs.m_pNode);
    }
    
private:
    
    // Constructor
    _dm_iterator(CDataModelBase *pDataModel)
        : m_pDataModel(pDataModel)
    {
        m_pNode = m_pDataModel->GetDataModel();
    }
    
    const _node *m_pNode;
    CDataModelBase *m_pDataModel;
};


class CDataModelMgr;


class STATEM_API CDataModelBase
    : virtual public CBaseObject
{
public:

    CDataModelBase();

    HRESULT Register(CDataModelMgr &);

    typedef _dm_iterator<CDataModelBase> iterator;

    virtual const _TCHAR *GetDataModelName() const=0;

    virtual const SDMEntry* GetDataModel() const=0;


    /// <summary>Obtains an iterator used to step over the key/value pairs in the source string</summary>
    /// <return>Returns an iterator</return>
    iterator begin() { return iterator(this); }

    /// <summary>Obtains an iterator that points past the last key/value pair in the source string</summary>
    /// <return>Returns an iterator</return>
    iterator end() 
    { 
        iterator iter = iterator(this); 
        while ((*iter).eType != PT_NONE)
            iter++;

        return iter; 
    }
};


class CDataModelMgr
{
public:

    typedef struct _PropNode
    {
        PDMGETTER pGetter;
        PDMSETTER pSetter;
    } SPropNode;

    typedef std::map<StringType, SPropNode *, StringComparator> CObjectPropertyMap;

    typedef struct _ObjNode
    {
        CDataModelBase *pObj;
        CObjectPropertyMap propertyMap;
    } SObjNode;

    typedef std::map<StringType, SObjNode *, StringComparator> CObjectMap;

    HRESULT Register(const _TCHAR *, CDataModelBase *pObject);
    HRESULT Register(const _TCHAR *, const _TCHAR *, PDMGETTER, PDMSETTER);

    template <typename T> T GetProperty(const _TCHAR *, const _TCHAR *, T);
    template <typename T> void SetProperty(const _TCHAR *, const _TCHAR *, T);

protected:

    HRESULT GetObjectNode(const _TCHAR *, SObjNode **);
    HRESULT GetPropertyNode(const _TCHAR *, const _TCHAR *, SPropNode **);

private:

    CObjectMap m_Objects;
    CObjectPropertyMap m_Properties;

};


template<class T>
class CDataModel
    : public CDataModelBase
{
public:
    
    //typedef void (T::*PVALUEFXN)(CKeyValueParmList::iterator &);
    typedef const _TCHAR *key_t;
    //typedef std::map<key_t, PVALUEFXN, StringComparator> CValueMap;

public:

    CDataModel() {};
    virtual ~CDataModel() {};

    template <class R> R GetProperty(const _TCHAR *, R);

    template <class R> void SetProperty(const _TCHAR *, R);

#if defined(_SCOT) || defined(_SCOTSSF_)
    typedef long HTRACE;
    #define INVALID_HANDLE (-1)
#endif

protected:

    virtual const SDMEntry* GetDataModel() const;

private:

    // hide copy and assignment
    CDataModel(const CDataModel&);
    CDataModel& operator = (const CDataModel &);

    SDMEntry *pDataModel;
    //const CValueMap &GetValueMap();

    //static CValueMap values;

};


template <class T>
const typename SDMEntry* CDataModel<T>::GetDataModel() const
{
    return pDataModel;
}


template <class T> template <class R>
R CDataModel<T>::GetProperty(const _TCHAR *, R defValue)
{
    return defValue;
}


template <class T> template <class R>
void CDataModel<T>::SetProperty(const _TCHAR *, R newValue)
{
}
