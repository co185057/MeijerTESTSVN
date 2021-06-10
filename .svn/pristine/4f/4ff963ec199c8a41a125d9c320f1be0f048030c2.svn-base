#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/xml/ContentHandlerOwner.h 1     6/20/08 1:14p Sa250050 $
/*
 * @(#)ContentHandlerOwner.h      $Revision: 1 $  $Date: 6/20/08 1:14p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/xml/ContentHandlerOwner.h $
 * 
 * 1     6/20/08 1:14p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 2     2/28/05 3:23p Dm185016
 * lint
 * 
 * 1     1/26/05 2:33p Dm185016
 * Moved to new Repository.
* 
* 1     10/14/04 1:53p Dm185016
* 
* 6     6/30/04 3:54p Dm185016
* Lint.
* Added Lexical handler for processing comments.
* 
* 5     4/20/04 4:47p Dm185016
* Expanded the error reporting mechanism
* 
* 4     2/10/04 11:56a Dm185016
* Cleaned up error handling
*/

#include "XMLError.h"
#include "utils.h"

class UTILS_API IContentHandlerOwner
{
public:

    virtual ~IContentHandlerOwner(void) {}

    virtual HRESULT AddEnvVariable(const _TCHAR *pszVarName, const _TCHAR *pszValue)=0;
    virtual HRESULT AddEnvVariable(const StringType &pszVarName, const StringType &pszValue)=0;
    virtual const _TCHAR * GetEnvVariable(const StringType &sVarName)=0;

protected:

    IContentHandlerOwner() {};

private:

    IContentHandlerOwner(const IContentHandlerOwner &);               // Hide the Copy construcor
    IContentHandlerOwner &operator =(const IContentHandlerOwner &);   // Hide the assignment operator
};
