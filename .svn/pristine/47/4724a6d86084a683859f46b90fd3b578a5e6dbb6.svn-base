#pragma once
/*
 * @(#)LibraryImpl.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/LibraryImpl.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 2     2/10/05 5:50p Dm185016
 * Fixed problem with static ids used in SM.
 * 
 * 1     1/26/05 2:29p Dm185016
 * Moved to new Repository.
* 
* 5     1/18/05 3:58p Dm185016
* UNICODE support
* 
* 4     10/04/04 6:47p Dm185016
* Changed base class
* 
* 2     3/19/04 3:28p Dm185016
* Memory leak fix
* 
* 1     1/27/04 2:06p Dm185016
* New support for DLL libraries of actions
*/
#include "XMLContent.h"
#include "Library.h"
#include "statem.h"

#pragma warning( push )
#pragma warning( disable : 4251 )

namespace STATE_MACHINE_NAMESPACE
{

class STATEM_API  CLibraryImpl
    : public CXMLContent
    , public ILibrary
    , public CPoolObject<CLibraryImpl>
{
public:

    CLibraryImpl(int);
    virtual ~CLibraryImpl(void);

    virtual int GetId() const;

    virtual const _TCHAR *GetComment();

    virtual const _TCHAR *GetName();

    DECLARE_STRING_PROPERTY(Name);
    DECLARE_STRING_PROPERTY(Comment);

    FARPROC FindFunction(LPCTSTR pcsFxnName);

    virtual FARPROC GetCreateFunction(LPCTSTR pcsFxnName);

    virtual FARPROC GetReleaseFunction(LPCTSTR pcsFxnName);

public: 

    StringType GetXMLOutput(CXMLBuilder *pXMLBuilder=NULL);

private:

    CLibraryImpl(CLibraryImpl &);                     // Hide the Copy construcor
    CLibraryImpl &operator =(const CLibraryImpl &);   // Hide the assignment operator

    HMODULE m_hModule;

    FARPROC m_pCreateFxn;

    FARPROC m_pReleaseFxn;

    int nId;
};

};

#pragma warning( pop )
