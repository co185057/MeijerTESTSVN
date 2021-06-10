#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/RCMErrorRAP.h 3     1/12/10 11:58p Cu185008 $
/*
 * @(#)RCMErrorRAP.h      $Revision: 3 $  $Date: 1/12/10 11:58p $
 *
 * Copyright 2005, NCR Corporation.
 *
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of NCR Corporation. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with msi.
 */
/*
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/RCMErrorRAP.h $
 * 
 * 3     1/12/10 11:58p Cu185008
 * G2 Migration.
 * 
 * 1     3/01/05 2:04p Dm185016
 * TAR 294587.  Added application state functionality.
*/

#include "rcmerror.h"
#include "rcmapi.h"

// Provides the definitions for all HRESULT values returned by the RCM component
// both internally and externally.
#define RAP_ERROR_BASE                          0xC0070001L

#define RAP_DUPLICATE_APPLICATION_STATE_ID      _HRESULT_TYPEDEF_(RCM_ERROR_BASE)

#pragma warning( push )
#pragma warning( disable : 4251 )

class RCMANAGER_API CRCMErrorRAPInfo
    : public CRCMErrorInfo
{
public:
    /*------------------------------- Constructors -------------------------------*/
        CRCMErrorRAPInfo ( HRESULT, int nLine=-1, int nCol=-1, const _TCHAR *pszFile=NULL, const _TCHAR *pszType=NULL );

    virtual
        ~CRCMErrorRAPInfo ( );

protected:

	const _TCHAR *lookupError(HRESULT);

private:

    CRCMErrorRAPInfo();
    CRCMErrorRAPInfo ( const CRCMErrorRAPInfo& errorInformation );

    CRCMErrorRAPInfo
        &operator= ( const CRCMErrorRAPInfo& errorInformation );
}; // CRCMErrorRAPInfo


//IEXCLASSDECLARE(CRCMErrorRAP,IException);

#pragma warning( pop )
