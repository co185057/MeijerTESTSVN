#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RCMError.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)RCMError.h      $Revision: 3 $  $Date: 10/21/08 4:09p $
 *
 * Copyright 2004, NCR Corporation.
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RCMError.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 5     4/05/05 11:58a Dm185016
 * Added PSX mismatch error
 * 
 * 4     3/01/05 2:03p Dm185016
 * TAR 294587.  Added ctor for RCMErrorRAP derivative,
 * 
 * 3     2/28/05 4:19p Dm185016
 * lint
 * 
 * 2     2/21/05 8:10p Dm185016
 * TAR 293968
 * 
 * 1     1/26/05 2:21p Dm185016
 * Moved to new Repository.
* 
* 4     12/22/04 11:02a Dm185016
* HH support
* 
* 3     11/16/04 11:29a Dm185016
* Context switch support
* 
* 2     10/29/04 8:57a Dm185016
* New error codes
* 
* 1     10/04/04 5:36p Dm185016
* New for 3.2 Patch E/4.0
*/

#include "smerror.h"
#include "rcmapi.h"

// Provides the definitions for all HRESULT values returned by the RCM component
// both internally and externally.
#define RCM_ERROR_BASE                 0xC0060001L

#define RCM_NO_XML_FILE_SPECIFIED               _HRESULT_TYPEDEF_(RCM_ERROR_BASE)
#define RCM_INVALID_ACTION_ID                   _HRESULT_TYPEDEF_((RCM_ERROR_BASE+1))
#define RCM_SM_INIT_FAILED                      _HRESULT_TYPEDEF_((RCM_ERROR_BASE+2))
#define RCM_SM_ALREADY_INITIALIZED              _HRESULT_TYPEDEF_((RCM_ERROR_BASE+3))
#define RCM_SM_NOT_INITIALIZED                  _HRESULT_TYPEDEF_((RCM_ERROR_BASE+4))
#define RCM_SM_MESSAGE_NOT_DEFINED              _HRESULT_TYPEDEF_((RCM_ERROR_BASE+5))
#define RCM_API_FAILURE_INVALID_KEY_VALUE_LIST	_HRESULT_TYPEDEF_((RCM_ERROR_BASE+6))
#define RCM_API_FAILURE_NOT_IMPLEMENTED         _HRESULT_TYPEDEF_((RCM_ERROR_BASE+7))
#define RCM_MISSING_ATTRIBUTE                   _HRESULT_TYPEDEF_((RCM_ERROR_BASE+8))
#define RCM_DUPLICATE_CONTROL_NAME              _HRESULT_TYPEDEF_((RCM_ERROR_BASE+9))
#define RCM_NO_MAIN_CONTEXT_AVAILABLE			_HRESULT_TYPEDEF_((RCM_ERROR_BASE+10))
#define RCM_UNKNOWN_CONTEXT			            _HRESULT_TYPEDEF_((RCM_ERROR_BASE+11))
#define RCM_DUPLICATE_CONTEXT_NAME              _HRESULT_TYPEDEF_((RCM_ERROR_BASE+12))
#define RCM_INVALID_CONTEXT_ID                  _HRESULT_TYPEDEF_((RCM_ERROR_BASE+13))
#define RCM_UNKNOWN_CONTEXT_SWITCH              _HRESULT_TYPEDEF_((RCM_ERROR_BASE+14))
#define RCM_UNKNOWN_REMOTE_CONNECTION           _HRESULT_TYPEDEF_((RCM_ERROR_BASE+15))
#define RCM_NO_REMOTE_CONNECTION_AVAILABLE      _HRESULT_TYPEDEF_((RCM_ERROR_BASE+16))
#define RCM_PSX_VERSION_MISMATCH                _HRESULT_TYPEDEF_((RCM_ERROR_BASE+17))

#pragma warning( push )
#pragma warning( disable : 4251 )

class RCMANAGER_API CRCMErrorInfo
    : public CSMErrorInfo
{
public:
    /*------------------------------- Constructors -------------------------------*/
        CRCMErrorInfo ( HRESULT, int nLine=-1, int nCol=-1, const _TCHAR *pszFile=NULL, const _TCHAR *pszType=NULL );

    virtual
        ~CRCMErrorInfo ( );

protected:

    CRCMErrorInfo();

	const _TCHAR *lookupError(HRESULT);

private:

    CRCMErrorInfo ( const CRCMErrorInfo& errorInformation );
    CRCMErrorInfo
        &operator= ( const CRCMErrorInfo& errorInformation );
}; // CRCMErrorInfo


//IEXCLASSDECLARE(CRCMError,IException);

#pragma warning( pop )
