// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/SMErrorHandler.cpp 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)SMErrorHandler.cpp      $Revision: 1 $  $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/SMErrorHandler.cpp $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:27p Dm185016
 * Moved to new Repository.
// 
// 2     6/11/04 10:59a Dm185016
// Minor lint violations.
// 
// 1     4/20/04 4:43p Dm185016
// New error handler for the XML parser.
*/
#include "stdafx.h"
#include "SMConstants.h"
#include "SMErrorHandler.h"
#include "SMError.h"
#include <stdio.h>
#include "XMLError.h"

using namespace STATE_MACHINE_NAMESPACE;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CXMLErrorInfo *CSMErrorHandler::GetErrorInfo ( HRESULT hr, 
							        int nLine, 
							        int nCol, 
							        const _TCHAR *pszFile, 
							        const _TCHAR *pszType )
{
    return new CSMErrorInfo(hr, nLine, nCol, pszFile, pszType);
}
