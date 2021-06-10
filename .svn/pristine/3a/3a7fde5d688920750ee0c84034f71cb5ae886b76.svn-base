// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/MessageObject.cpp 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)CMessageObject.cpp    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/MessageObject.cpp $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 3     2/28/05 3:53p Dm185016
 * lint
 * 
 * 2     2/17/05 5:59p Dm185016
 * Fixed UNICODE problem
 * 
 * 1     1/26/05 2:27p Dm185016
 * Moved to new Repository.
// 
// 17    1/18/05 3:55p Dm185016
// UNICODE support
// 
// 16    11/22/04 4:01p Dm185016
// When making a copy, need to also copy the trace handle.
// 
// 15    11/15/04 11:41a Dm185016
// Moved return code from action to message.
// 
// 14    10/29/04 9:21a Dm185016
// Corrected error message
// 
// 13    10/04/04 6:39p Dm185016
// Trace object is no longer shared.
// 
// 11    8/26/04 9:58p Dm185016
// TAR 278069
// 
// 10    6/30/04 3:25p Dm185016
// Lint
// 
// 9     4/01/04 1:50p Dm185016
// Fixed AppControl
// 
// 8     3/31/04 4:01p Dm185016
// Allow copying of synchronous message
// 
// 7     3/26/04 11:28a Dm185016
// Added accessor for data length.
// 
// 6     3/19/04 3:03p Dm185016
// Memory Leak fix
// 
// 5     2/24/04 3:32p Dm185016
// Added data field
// 
// 4     2/10/04 11:41a Dm185016
// Added synchronous support
// 
// 3     1/27/04 1:59p Dm185016
// Added copy constructor and assignment operator.
*/
//lint -e1506

#include "stdafx.h"
#include "MessageObject.h"

using namespace STATE_MACHINE_NAMESPACE;

CMessageObject::CMessageObject()
    : m_szName(NULL)
    , m_szComment(NULL)
    , m_lTimeout(-1)
    , m_nID(0)
    , m_pData(NULL)
    , m_nDataLength(0)
    , m_bstrText(NULL)
    , m_bSynchronous(false)
    , m_pSemaphore(NULL)
    , m_hr(S_OK)
{
    //ITRACE_DEVELOP(__FUNCTION__ _T(".  this:  ") + IString((long)this).d2x());
}


CMessageObject::CMessageObject(const CMessageObject &theOther)
    : IMessage(theOther)
    , CBaseObject(theOther)
    , CPool<CMessageObject>(theOther)
    , m_szName(NULL)
    , m_szComment(NULL)
    , m_lTimeout(theOther.m_lTimeout)
    , m_nID(theOther.m_nID)
    , m_pData(NULL)
    , m_nDataLength(0)
    , m_bstrText(theOther.m_bstrText)           //lint !e1554 Want to copy pointer
    , m_bSynchronous(theOther.m_bSynchronous)
    , m_pSemaphore(NULL)
    , m_hr(S_OK)
{
    //ITRACE_DEVELOP(__FUNCTION__ _T(".  this:  ") + IString((long)this).d2x());
    SetComment(theOther.m_szComment);
    SetName(theOther.m_szName);
    SetData(m_pData, m_nDataLength);
}


CMessageObject &CMessageObject::AsynchCopy()
{
    //ITRACE_DEVELOP(__FUNCTION__ _T(".  this:  ") + IString((long)this).d2x());
    if (m_bSynchronous)
        throw exception("Cannot copy synchronous message");
    CMessageObject *pCopy   = new CMessageObject;
    pCopy->m_nID            = m_nID;
    pCopy->m_lTimeout       = m_lTimeout;
    pCopy->m_bSynchronous   = m_bSynchronous;
    pCopy->m_pSemaphore     = NULL;
    pCopy->m_bstrText       = m_bstrText;
    pCopy->SetComment(m_szComment);
    pCopy->SetName(m_szName);
    pCopy->SetTraceObject(GetTraceHandle());

    if (m_nDataLength != 0)
        pCopy->SetData(m_pData, m_nDataLength);

    return *pCopy;
}


/// <summary>SynchCopy makes a copy of this message.  However, the synchronous
/// capabilities are passed on to the copy.  This message is changed to asynchronous
/// and ownership of the allocated semaphore is passed to the copy.
/// </summary>
/*
CMessageObject &CMessageObject::SynchCopy()
{
    if (!m_bSynchronous)
        throw exception(_T("Cannot copy asynchronous message"));
    CMessageObject *pCopy   = new CMessageObject;
    pCopy->m_nID            = m_nID;
    pCopy->m_lTimeout       = m_lTimeout;
    pCopy->m_bSynchronous   = true;
    m_bSynchronous          = false;
    pCopy->m_pSemaphore     = m_pSemaphore;
    m_pSemaphore            = NULL;
    pCopy->m_bstrText       = m_bstrText;
    pCopy->SetComment(m_szComment);
    pCopy->SetName(m_szName);

    if (m_nDataLength != 0)
        pCopy->SetData(m_pData, m_nDataLength);

    return *pCopy;
}
*/


CMessageObject &CMessageObject::operator =(const CMessageObject &theOther)
{
    //ITRACE_DEVELOP(__FUNCTION__ _T(".  this:  ") + IString((long)this).d2x());
    if (&theOther == this)
        return *this;

    m_nID           = theOther.m_nID;
    m_lTimeout      = theOther.m_lTimeout;
    m_bSynchronous  = theOther.m_bSynchronous;
    m_pSemaphore    = NULL;

    SetComment(theOther.m_szComment);
    SetName(theOther.m_szName);

    return *this;
}


CMessageObject:: ~CMessageObject() 
{
    //ITRACE_DEVELOP(__FUNCTION__ _T(".  this:  ") + IString((long)this).d2x());

    try
    {
        delete [] m_szName;
        delete [] m_szComment;
        delete [] m_pData;
        delete m_pSemaphore;

        m_bstrText = NULL;
    }
    catch (...)
        { ITRACE_ERROR(_T("Exception caught in CMessageObject::~CMessageObject")); }
}


int CMessageObject::GetId() const
{
    return m_nID;
}


const _TCHAR *CMessageObject::GetName() 
{ 
    return m_szName; 
}


const _TCHAR *CMessageObject::GetComment(void) 
{ 
    return m_szComment; 
}


long CMessageObject::GetTimeout() 
{ 
    return m_lTimeout; 
}


void CMessageObject::SetId(int nID)
{
    m_nID = nID;
}


void CMessageObject::SetName(const _TCHAR *szName)
{
    delete [] m_szName;

    size_t length = 0;
    if (szName != NULL)
        length = _tcslen(szName)+1;
    else
        length = 1;
    m_szName = new _TCHAR[length];
    if (szName != NULL)
        _tcscpy(m_szName, szName); 
    else
        m_szName[0] = 0;
}


void CMessageObject::SetComment(const _TCHAR *szComment) 
{ 
    delete [] m_szComment;

    size_t length = 0;
    if (szComment != NULL)
        length = _tcslen(szComment)+1;
    else
        length = 1;
    m_szComment = new _TCHAR[length];
    if (szComment != NULL)
        _tcscpy(m_szComment, szComment); 
    else
        m_szComment[0] = 0;
}


void CMessageObject::SetTimeout(long lTimeout) 
{ 
    m_lTimeout = lTimeout; 
}


bool CMessageObject::IsSynchronous() const
{
    return m_bSynchronous;
}


void CMessageObject::SetSynchronous(bool bSynchronous)
{
    m_bSynchronous = bSynchronous;

    delete m_pSemaphore;

    m_pSemaphore = new utils::CSemaphore(GetTraceHandle());
}


void CMessageObject::SetRC(HRESULT hr) { m_hr = hr; }
HRESULT CMessageObject::GetRC() const { return m_hr; }


//lint -e1763
BSTR CMessageObject::GetText() const
{
    return m_bstrText;
}


void CMessageObject::SetText(BSTR bstrText)
{
    if (m_bstrText != NULL)
        ::SysFreeString(m_bstrText);

    m_bstrText = bstrText;
}


void* CMessageObject::GetData() const
{ 
    return m_pData;
}


size_t CMessageObject::GetDataLength() const
{ 
    return m_nDataLength;
}


void CMessageObject::SetData(const void* pData, size_t nDataLength)
{
    delete [] m_pData;

    if ((nDataLength == 0) || (pData == NULL))
    {
        m_pData = NULL;
        m_nDataLength = 0;
        return;
    }

    m_pData = new _TCHAR[nDataLength+1];
    memcpy(m_pData, pData, nDataLength*sizeof(_TCHAR));
    ((_TCHAR *)m_pData)[nDataLength]=0;
    m_nDataLength = nDataLength;
}


HRESULT CMessageObject::Wait() const
{
    _ASSERTE(m_pSemaphore!=NULL);

    HRESULT hr = S_OK;
    unsigned long ulPostCnt;

    (*m_pSemaphore).Wait();
    (*m_pSemaphore).Reset(ulPostCnt);

    return hr;
}


void CMessageObject::Post() const
{
    _ASSERTE(m_pSemaphore!=NULL);

    (*m_pSemaphore).Post();
}
