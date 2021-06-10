// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RCMError.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)RCMError.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RCMError.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 4     4/05/05 11:58a Dm185016
 * Added PSX mismatch error
 * 
 * 3     3/01/05 2:03p Dm185016
 * TAR 294587.  Added ctor for RCMErrorRAP derivative,
 * 
 * 2     2/21/05 8:10p Dm185016
 * TAR 293968
 * 
 * 1     1/26/05 2:21p Dm185016
 * Moved to new Repository.
// 
// 4     12/22/04 11:02a Dm185016
// HH support
// 
// 3     11/16/04 11:29a Dm185016
// Context switch support
// 
// 2     10/29/04 8:57a Dm185016
// New error codes
// 
// 1     10/04/04 5:36p Dm185016
// New for 3.2 Patch E/4.0
*/
/****************************************************************************/
/* Include Files                                                            */
/****************************************************************************/

#include "StdAfx.h"
#include "RCMError.h"


CRCMErrorInfo::CRCMErrorInfo (  )
{}


CRCMErrorInfo::CRCMErrorInfo ( HRESULT hr, 
							  int nLine, 
							  int nCol, 
							  const _TCHAR *pszFile, 
							  const _TCHAR *pszType )
{
    Init( lookupError(hr), hr, nLine, nCol, pszFile, pszType );
}


const _TCHAR *CRCMErrorInfo::lookupError( HRESULT RCMErrorId)
{
    const _TCHAR *retValue;

    switch (RCMErrorId)
    {
        case RCM_NO_XML_FILE_SPECIFIED      		: retValue = _T("No XML File specified");             break;
        case RCM_INVALID_ACTION_ID          		: retValue = _T("Invalid action id specified");             break;
        case RCM_SM_INIT_FAILED             		: retValue = _T("Initialize failed");             break;
        case RCM_SM_ALREADY_INITIALIZED     		: retValue = _T("State Machine already initialized");             break;
        case RCM_SM_NOT_INITIALIZED         		: retValue = _T("State Machine not initialized");             break;
        case RCM_SM_MESSAGE_NOT_DEFINED     		: retValue = _T("SM message not defined in XML");             break;
        case RCM_API_FAILURE_INVALID_KEY_VALUE_LIST	: retValue = _T("API failure.  Invalid KV list");             break;
        case RCM_API_FAILURE_NOT_IMPLEMENTED	    : retValue = _T("API failure.  Function not implemented");             break;
        case RCM_MISSING_ATTRIBUTE	                : retValue = _T("Attribute missing from <Control> definition");             break;
        case RCM_DUPLICATE_CONTROL_NAME             : retValue = _T("Duplicate <Control> Name");             break;
        case RCM_NO_MAIN_CONTEXT_AVAILABLE          : retValue = _T("No main context available");             break;
        case RCM_UNKNOWN_CONTEXT                    : retValue = _T("Unknown context");             break;
        case RCM_DUPLICATE_CONTEXT_NAME             : retValue = _T("Duplicate <Context> Name");             break;
        case RCM_INVALID_CONTEXT_ID                 : retValue = _T("Invalid context id");             break;
        case RCM_UNKNOWN_CONTEXT_SWITCH             : retValue = _T("Unknown context switch");             break;
        case RCM_UNKNOWN_REMOTE_CONNECTION          : retValue = _T("Unknown Remote Connection ");             break;
        case RCM_NO_REMOTE_CONNECTION_AVAILABLE     : retValue = _T("No Remote Connection available");             break;
        case RCM_PSX_VERSION_MISMATCH               : retValue = _T("PSX version mismatch");             break;
        default :
        {
            retValue = CSMErrorInfo::lookupError(RCMErrorId);
        }
    }

    return retValue;
}


CRCMErrorInfo::~CRCMErrorInfo ( ) { }
