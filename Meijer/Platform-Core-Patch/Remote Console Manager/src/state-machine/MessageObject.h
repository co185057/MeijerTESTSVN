#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/MessageObject.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)CMessageObject.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
 *
 * Copyright 2004, NCR Corporation.
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/MessageObject.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:27p Dm185016
 * Moved to new Repository.
* 
* 12    11/15/04 11:41a Dm185016
* Moved return code from action to message.
* 
* 11    10/04/04 6:39p Dm185016
* Trace object is no longer shared.
* 
* 9     6/30/04 3:25p Dm185016
* Lint
* 
* 8     4/01/04 1:50p Dm185016
* Fixed AppControl
* 
* 7     3/26/04 11:28a Dm185016
* Added accessor for data length.
* 
* 6     3/19/04 3:03p Dm185016
* Memory Leak fix
* 
* 5     2/24/04 3:32p Dm185016
* Added data field
* 
* 4     2/10/04 11:41a Dm185016
* Added synchronous support
* 
* 3     1/27/04 1:59p Dm185016
* Added copy constructor and assignment operator.
*/

#include "StateMachineObject.h"
//#include <hash_map>
#include <map>
#include "Message.h"
#include "semaphore.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE
{

class CMessageObject
    : public IMessage
    , virtual public CBaseObject
    , public CPool<CMessageObject>
{

public:
    friend class CXMLStateMachine;

	CMessageObject();
	virtual ~CMessageObject();

	virtual int GetId() const;

    virtual const _TCHAR *GetName();

    virtual const _TCHAR *GetComment();

	virtual long GetTimeout();

    virtual void SetId(int);

    virtual void SetName(const _TCHAR *);

	virtual void SetComment(const _TCHAR *);

	virtual void SetTimeout(long);

    virtual bool IsSynchronous() const;

    virtual void SetSynchronous(bool);

    virtual BSTR GetText() const;

    virtual void SetText(BSTR);

    virtual HRESULT Wait() const;

    virtual void SetRC(HRESULT);

    virtual HRESULT GetRC() const;

    virtual void Post() const;

    virtual void* GetData() const;

    virtual size_t GetDataLength() const;

    virtual void SetData(const void*, size_t nDataLength);

protected:

    CMessageObject &AsynchCopy();

//  CMessageObject &SynchCopy();

private:

    CMessageObject(const CMessageObject &);
    CMessageObject &operator =(const CMessageObject &);
    
    _TCHAR * m_szName;
	_TCHAR * m_szComment;
	long m_lTimeout;
    int m_nID;
    void* m_pData;
    size_t m_nDataLength;

    BSTR m_bstrText;

    bool m_bSynchronous;
    mutable utils::CSemaphore *m_pSemaphore;
    HRESULT m_hr;
};

}