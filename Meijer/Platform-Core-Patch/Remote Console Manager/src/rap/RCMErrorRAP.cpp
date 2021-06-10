// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/RCMErrorRAP.cpp 3     1/12/10 11:57p Cu185008 $
/*
 * @(#)RCMErrorRAP.cpp    $Revision: 3 $ $Date: 1/12/10 11:57p $
 *
 * Copyright 2005 by NCR Corporation,
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/RCMErrorRAP.cpp $
 * 
 * 3     1/12/10 11:57p Cu185008
 * G2 Migration.
 * 
 * 1     3/01/05 2:04p Dm185016
 * TAR 294587.  Added application state functionality.
*/
/****************************************************************************/
/* Include Files                                                            */
/****************************************************************************/

#include "StdAfx.h"
#include "RCMErrorRAP.h"


CRCMErrorRAPInfo::CRCMErrorRAPInfo ( HRESULT hr, 
							  int nLine, 
							  int nCol, 
							  const _TCHAR *pszFile, 
							  const _TCHAR *pszType )
{
    Init( lookupError(hr), hr, nLine, nCol, pszFile, pszType );
}


const _TCHAR *CRCMErrorRAPInfo::lookupError( HRESULT RCMErrorRAPId)
{
    const _TCHAR *retValue;

    switch (RCMErrorRAPId)
    {
        case RAP_DUPLICATE_APPLICATION_STATE_ID     : retValue = _T("Duplicate <application-state> Name");             break;
        default :
        {
            retValue = CRCMErrorInfo::lookupError(RCMErrorRAPId);
        }
    }

    return retValue;
}


CRCMErrorRAPInfo::~CRCMErrorRAPInfo ( ) { }
