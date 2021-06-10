#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/KeyMessageHandler.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)KeyMessageHandler.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/KeyMessageHandler.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 2     3/07/05 6:57p Dm185016
 * Added trace message.
 * 
 * 1     1/26/05 2:27p Dm185016
 * Moved to new Repository.
* 
* 2     1/18/05 3:55p Dm185016
* UNICODE support
* 
* 1     10/04/04 6:36p Dm185016
* New base handler for messages that get mapped to other messages
*/

#include "StringComparator.h"
#include "Message.h"
#include <map>

#define BEGIN_KEY_MSG_MAP(CNAME)\
    CNAME::SKeyMsgEntry\
        CNAME::m_keyMsgHandlerMap[] = {

#define KEY_MSG_MAP_ENTRY(KEY, MSG)\
    { KEY, MSG },

#define END_KEY_MSG_MAP()\
    { NULL, NULL } };

#define DECLARE_KEY_MSG_MAP()\
private:\
    static SKeyMsgEntry m_keyMsgHandlerMap[];\
protected: \
    virtual const SKeyMsgEntry* GetKeyMsgTable() const { return m_keyMsgHandlerMap; }

template<class T>
class CKeyMessageHandler
{
public:
    
    typedef const _TCHAR *key_t;
    typedef std::map<key_t, key_t, StringComparator> CKeyMsgMap;

    struct SKeyMsgEntry
    {
        const _TCHAR *pszKeyName;
        const _TCHAR *pszMessageName;
    };

public:

    CKeyMessageHandler() {};
    virtual ~CKeyMessageHandler() {};

    HRESULT PostKeyMsg(T *, IMessage &message, const _TCHAR *) throw();

protected:

    virtual const SKeyMsgEntry* GetKeyMsgTable() const=0;

    virtual void UnknownKey(const _TCHAR *);

private:

    // hide copy and assignment
    CKeyMessageHandler(const CKeyMessageHandler&);
    CKeyMessageHandler& operator = (const CKeyMessageHandler &);

    const CKeyMsgMap &GetKeyMsgMap();

    static CKeyMsgMap keymsgMappings;

};


template <class T>
const typename CKeyMessageHandler<T>::CKeyMsgMap &CKeyMessageHandler<T>::GetKeyMsgMap()
{
    if (keymsgMappings.empty())
    {
        const SKeyMsgEntry *pKeyMsgMap = GetKeyMsgTable();
        for (int i=0; pKeyMsgMap->pszKeyName != NULL; pKeyMsgMap++, i++)
        {
            keymsgMappings[pKeyMsgMap->pszKeyName] = pKeyMsgMap->pszMessageName;
        }
    };

    return keymsgMappings;
}


template <class T>
void CKeyMessageHandler<T>::UnknownKey(const _TCHAR *)
{
    // Override this method to report the unknown key here
}


template <class T>
HRESULT CKeyMessageHandler<T>::PostKeyMsg(T *thisAction, IMessage &message, const _TCHAR *pszKeyName) throw()
{
    HRESULT hr = STATEM_MSG_CREATION_FAILED;

    ASSERT(pszKeyName!=NULL);
    if (pszKeyName == NULL)
    {
        hr = STATEM_INVALID_PARAMETER;
        thisAction->GetTraceObject().XTRACE_DEVELOP(_T(__FUNCTION__) _T(" returns:  ") + IString(hr).d2x());
        return hr;
    }

    try
    {
        const CKeyMsgMap &keyMsgMap = GetKeyMsgMap();
        CKeyMsgMap::const_iterator iter = keyMsgMap.find(pszKeyName);
        if (iter == keyMsgMap.end())
        {
            UnknownKey(pszKeyName);
            hr = STATEM_NO_KEY_MAPPING;
            thisAction->GetTraceObject().XTRACE_DEVELOP(_T(__FUNCTION__) _T(" returns:  ") + IString(hr).d2x());
            return hr;
        }
        else
        {
            const _TCHAR *pszNewMsgName = iter->second;

            thisAction->GetTraceObject().XTRACE_DEVELOP(_T(__FUNCTION__) _T(" sending msg:  ") + IString(pszNewMsgName));

            IMessage &newMsg 
                = thisAction->stateMachine().MakeAsynchCopyEx(message, pszNewMsgName);
            hr = thisAction->stateMachine().PostMsg(newMsg);
        }
    }
    catch (const exception &exc)
    { 
		thisAction->GetTraceObject().XTRACE_EXCEPTION(exc); 
    }
    catch (...)
    { 
        thisAction->GetTraceObject().XTRACE_DOT_ERROR(); 
    }

    return hr;
}


template<class T>
typename CKeyMessageHandler<T>::CKeyMsgMap CKeyMessageHandler<T>::keymsgMappings;
