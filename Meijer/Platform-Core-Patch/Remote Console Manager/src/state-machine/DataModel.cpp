#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/DataModel.cpp 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)DataModel.cpp    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/DataModel.cpp $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     5/31/05 11:36a Dm185016
 * Data Model Support
*/

#include "stdafx.h"
#include "DataModel.h"
#include "SMError.h"


CDataModelBase::CDataModelBase()
{
}


HRESULT CDataModelBase::Register(CDataModelMgr &dm)
{
    dm.Register(GetDataModelName(), this);

    iterator iter = begin();
    for (;iter != end(); iter++)
    {
       dm.Register((*iter).pszObjectName,
                   (*iter).pszPropertyName,
                   (*iter).pGetter,
                   (*iter).pSetter);
    }

    return S_OK;
}


HRESULT CDataModelMgr::Register(const _TCHAR *pszObjName, CDataModelBase *pObject)
{
    CObjectMap::iterator iter = m_Objects.find(pszObjName);
    if (iter != m_Objects.end())
        return STATEM_DM_OBJECT_ALREADY_REGISTERED;

    SObjNode *pObjNode      = new SObjNode;
    pObjNode->pObj          = pObject;
    m_Objects[pszObjName]   = pObjNode;

    return S_OK;
}


HRESULT CDataModelMgr::Register(const _TCHAR *pszObjName
                              , const _TCHAR *pszPropertyName
                              , PDMGETTER pGetter
                              , PDMSETTER pSetter)
{
    SObjNode *pObject;

    HRESULT hr = GetObjectNode(pszObjName, &pObject);
    if (hr != S_OK)
        return hr;

    CObjectPropertyMap &propertyMap = pObject->propertyMap;
    CObjectPropertyMap::iterator iter = propertyMap.find(pszPropertyName);
    if (iter != propertyMap.end())
        return STATEM_DM_PROPERTY_ALREADY_REGISTERED;

    SPropNode *pPropNode    = new SPropNode;
    pPropNode->pGetter      = pGetter;
    pPropNode->pSetter      = pSetter;
    propertyMap[pszPropertyName] = pPropNode;

    return hr;
}


HRESULT CDataModelMgr::GetObjectNode(const _TCHAR *pszObjName, SObjNode **ppObject)
{
    CObjectMap::iterator iter = m_Objects.find(pszObjName);
    if (iter == m_Objects.end())
        return STATEM_DM_OBJECT_NOT_REGISTERED;

    *ppObject = iter->second;

    return S_OK;
}


HRESULT CDataModelMgr::GetPropertyNode(const _TCHAR *pszObjName
                                     , const _TCHAR *pszPropName
                                     , SPropNode **ppProperty)
{
    SObjNode *pObject;

    HRESULT hr = GetObjectNode(pszObjName, &pObject);
    if (hr != S_OK)
        return hr;

    CObjectPropertyMap &propertyMap = pObject->propertyMap;
    CObjectPropertyMap::iterator iter = propertyMap.find(pszPropName);
    if (iter == propertyMap.end())
        return STATEM_DM_PROPERTY_NOT_REGISTERED;

    *ppProperty = iter->second;

    return S_OK;
}


template <> long CDataModelMgr::GetProperty(const _TCHAR *pszObjName
                                  , const _TCHAR *pszPropertyName
                                  , long lDefault)
{
    return lDefault;
}


template <> void CDataModelMgr::SetProperty(const _TCHAR *pszObjName
                                  , const _TCHAR *pszPropertyName
                                  , long lNewValue)
{
}


template <> bool CDataModelMgr::GetProperty(const _TCHAR *pszObjName
                                  , const _TCHAR *pszPropertyName
                                  , bool bDefault)
{
    return bDefault;
}


template <> void CDataModelMgr::SetProperty(const _TCHAR *pszObjName
                                  , const _TCHAR *pszPropertyName
                                  , bool bDefault)
{
}
