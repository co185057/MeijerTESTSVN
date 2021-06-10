#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/xresource.h 1     6/20/08 1:13p Sa250050 $
/*
* @(#)Resource.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/xresource.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:32p Dm185016
 * Moved to new Repository.
* 
* 6     10/05/04 8:29a Dm185016
* Trace object no longer shared.
* 
* 4     8/26/04 10:03p Dm185016
* TAR 278069 - Added Loggable to turn off trace
* 
* 3     8/23/04 10:19p Dm185016
* Added better tracing information.
* 
* 2     6/30/04 3:51p Dm185016
* Lint.
* 
* 1     5/21/04 9:24a Dm185016
* Had to rename - conflict with generated file name
* 
* 1     1/23/04 3:39p Dm185016
* New DLL Support
*/
#include "Utils.h"

class ILoggable;


//lint --e{1735}
class UTILS_API CResource  
    : public CBaseObject
{
public:

    /*------------------------------- Constructors -------------------------------*/
    CResource( HTRACE hTrace, const _TCHAR *name = NULL );
    CResource( const _TCHAR *name = NULL );
    virtual
    ~CResource( );

    /*----------------------------- Resource Locking -----------------------------*/
    virtual CResource
    &Lock           ( unsigned long timeOut = 0xffffffff );
    virtual CResource
    &Unlock         ( );

    void SetName( const _TCHAR * );

private:

    /*------------------------------ Hidden Members ------------------------------*/
    CResource    ( const CResource& resource );
    CResource
    &operator=    ( const CResource& resource );

    void Initialize( HTRACE hTrace, const _TCHAR *name );

    /*--------------------------------- Private ----------------------------------*/
    HANDLE hMutex;

    IString keyName;

    ILoggable &loggable();

    ILoggable *m_pLoggable;

}; // CResource


class UTILS_API CSharedResource : public CResource
{
public:
    /*------------------------------- Constructors -------------------------------*/
    CSharedResource   ( HTRACE hTrace, const _TCHAR* keyName );
    virtual
    ~CSharedResource   ( );

    /*---------------------------- Resource Information --------------------------*/
    IString
    keyName           ( ) const;

private:
    /*------------------------------ Hidden Members ------------------------------*/
    CSharedResource   (  );
    CSharedResource    ( const CSharedResource& sharedResource );
    CSharedResource
    &operator=          ( const CSharedResource& sharedResource );

}; // CSharedResource
