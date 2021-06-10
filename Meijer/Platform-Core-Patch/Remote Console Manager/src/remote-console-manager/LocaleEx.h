// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/LocaleEx.h 3     10/21/08 4:09p Sm133076 $
#pragma once
/*
 * @(#)Locale.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/LocaleEx.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 5     6/05/08 8:50a Bs185067
 * TAR 376639
 * 
 * 2     2/08/05 5:31p Tp151000
 * TAR 292777
 * 
 * 1     1/26/05 2:21p Dm185016
 * Moved to new Repository.
* 
* 1     11/16/04 11:28a Dm185016
*/

#include "RCMApi.h"
#include "Singleton.h"

#define MAX_STRING 20


class RCMANAGER_API CLocale
    : public CSingleton<CLocale>
{
public:

    friend class CSingleton<CLocale>;

    static int GetCenturyDigitCount();
    static int GetLocaleShortDateOrdering();
    static int GetDateFormatCount();
    
    static CString DateFormat(int month, int day, int year);
    static CString GetDateFormat(HTRACE hTrace);

private:

    CLocale();
    CLocale(const CLocale &);

    int m_nLocaleShortDateOrdering;
    int m_nCenturyDigitCount;
    StringType m_csLocaleDateSeparator;
    StringType m_csLocaleShortDateFormat;
};
