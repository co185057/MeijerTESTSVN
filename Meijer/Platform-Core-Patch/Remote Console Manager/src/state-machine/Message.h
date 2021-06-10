#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/Message.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)Message.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/Message.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 4     3/24/05 7:25p Dm185016
 * UNICODE
 * 
 * 3     3/22/05 4:08p Dm185016
 * UNICODE
 * 
 * 2     2/28/05 3:53p Dm185016
 * lint
 * 
 * 1     1/26/05 2:27p Dm185016
 * Moved to new Repository.
* 
* 10    11/15/04 11:41a Dm185016
* Moved return code from action to message.
* 
* 8     8/26/04 9:58p Dm185016
* TAR 278069 - Added trace macro for messages.
* 
* 7     5/19/04 1:33p Dm185016
* lint
* 
* 6     3/26/04 11:28a Dm185016
* Added accessor for data length.
* 
* 5     2/24/04 3:32p Dm185016
* Added data field
* 
* 4     2/10/04 11:41a Dm185016
* Added synchronous support
* 
* 3     1/27/04 1:56p Dm185016
* DLL Support
* 
* 2     1/16/04 9:06a Dm185016
* Namespace support
*/

#include "StateMachineObject.h"
//#include <hash_map>
#include <map>

#define MSG_TIMEOUT_SUFFIX _T("_timeout");

#define TRACE_MSG(MSG_PTR)\
    try\
    {\
        CA2T sName(typeid(*MSG_PTR).name());\
        ITRACE_DEVELOP(IString(_T("Message:  ")) + (const _TCHAR *)sName);\
    }\
    catch( __non_rtti_object  &e )\
        { ITRACE_EXCEPTION(e); }\
    catch( bad_typeid &e )\
        { ITRACE_EXCEPTION(e); }\
    catch( ... )\
        { ITRACE_DOT_ERROR(); }\
    ITRACE_DEVELOP(_T("Message address:  ") + IString((long)MSG_PTR).d2x());

namespace STATE_MACHINE_NAMESPACE
{

class STATEM_API IMessage
    : public IStateMachineObject
{

public:
    virtual ~IMessage() {};

    virtual const _TCHAR *GetName()=0;

    virtual long GetTimeout()=0;

    virtual bool IsSynchronous() const=0;

    virtual BSTR GetText() const=0;

    virtual HRESULT GetRC() const=0;

    virtual HRESULT Wait() const=0;

    virtual void Post() const=0;

    virtual void* GetData() const=0;

    virtual size_t GetDataLength() const=0;

protected:

    IMessage() {};
};

typedef IMessage *PMESSAGE;

typedef map<StringType, PMESSAGE, StringComparator> CMessagesMap;

}