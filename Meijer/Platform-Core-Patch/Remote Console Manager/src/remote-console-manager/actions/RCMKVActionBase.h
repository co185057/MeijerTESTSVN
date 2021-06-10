#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/RCMKVActionBase.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)RCMKVActionBase.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/actions/RCMKVActionBase.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     2/28/05 4:40p Dm185016
 * lint
 * 
 * 1     1/26/05 2:24p Dm185016
 * Moved to new Repository.
* 
* 2     1/12/05 6:07p Dm185016
* Cleaned up trace log.
* 
* 1     10/04/04 5:36p Dm185016
* New for 3.2 Patch E/4.0
*/

#include "KVParmList.h"
#include "KeyValueHandler.h"
#include "RCMActionBase.h"

namespace fastlane
{

template<class T>
class CRCMKVActionBase
    : public CRCMActionBase
    , public CKeyValueHandler<T>
{
public:

    CRCMKVActionBase() {};
    virtual ~CRCMKVActionBase() {};

protected:
    
    inline HRESULT ParseMessageKeys(const IMessage &message);
    inline HRESULT ParseStringKeys(const _TCHAR *pszKeys);

    virtual void UnknownKey(CKeyValueParmList::iterator &);

};


template <class T>
void CRCMKVActionBase<T>::UnknownKey(CKeyValueParmList::iterator &keys)
{
    const _TCHAR *pszValue = CKeyValue<const _TCHAR *>::Get(keys, _T("<Unknown>"));
    ITRACE_DEVELOP(_T("Unknown key detected:  ") + IString((*keys).first) 
                   + _T(".  value=\"") + IString(pszValue) + _T("\""));
}


template <class T>
HRESULT CRCMKVActionBase<T>::ParseMessageKeys(const IMessage &message)
{
    return ParseKeys((_TCHAR *)message.GetData(), GetTraceHandle());
}


template <class T>
HRESULT CRCMKVActionBase<T>::ParseStringKeys(const _TCHAR *pszKeys)
{
    return ParseKeys(pszKeys, GetTraceHandle());
}


}
