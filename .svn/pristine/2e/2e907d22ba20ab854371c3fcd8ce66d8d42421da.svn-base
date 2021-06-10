#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/xthread.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)xthread.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/xthread.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 4     3/15/05 3:40p Dm185016
 * TAR 295676 - AcceptConnection is now handled by a separate thread to
 * prevent hanging the state machine thread.  Added AutoDelete flag to
 * constructor.
 * 
 * 3     3/14/05 8:29p Dm185016
 * Added auto delete parameter.
 * 
 * 2     2/28/05 3:39p Dm185016
 * lint
 * 
 * 1     1/26/05 2:32p Dm185016
 * Moved to new Repository.
* 
* 3     10/05/04 8:30a Dm185016
* Trace object no longer shared.
* 
* 1     1/23/04 3:40p Dm185016
* New DLL Support
*/
#include "utils.h"


typedef unsigned (__stdcall *PTHREADFN)( void* );


class UTILS_API CThreadBase
    : public CBaseObject
{
public:

    /**
    * Default no-argument constructor
    */
    CThreadBase(bool bAutoDelete = false);
    CThreadBase(PTHREADFN m_pThreadFxn, bool bAutoDelete = false);

    virtual ~CThreadBase();

    virtual void SetThreadFxn(PTHREADFN);

    virtual PTHREADFN GetThreadFxn();

    virtual void Start();

    virtual void Stop();

protected:

    bool IsAutoDelete() const { return m_bAutoDelete; }

private:

    static unsigned __stdcall _runit( void * );
    
    HANDLE m_hThread;
    unsigned m_threadID;

    bool m_bAutoDelete;

    PTHREADFN m_pThreadFxn;

};


template < class T >
class CThreadMemberBase
    : public CThreadBase
{
    public:

    CThreadMemberBase ( bool bAutoDelete = false );

    ~CThreadMemberBase ();

protected:

    virtual unsigned Run ( )=0;

    /*----------------------------- Hidden Members -------------------------------*/
private:

    static unsigned __stdcall _run_member_fxn( void * );

}; //


//-----------------------------------------------------------------------------
// Destructor for the base property template
template <typename T>
CThreadMemberBase<T>::CThreadMemberBase(bool bAutoDelete)
: CThreadBase(&_run_member_fxn, bAutoDelete)
{
}


//-----------------------------------------------------------------------------
// Destructor for the base property template
template <typename T>
CThreadMemberBase<T>::~CThreadMemberBase()
{
}


template <typename T>
inline
unsigned __stdcall CThreadMemberBase<T>::_run_member_fxn( void * pArgs)
{
    T *pCTMB = dynamic_cast<T *>((CThreadBase *)pArgs);

    unsigned rc = pCTMB->Run();

    if (pCTMB->IsAutoDelete())
        delete pCTMB;

    return rc;
}
