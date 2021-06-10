// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/LocaleEx.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)Locale.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/LocaleEx.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 6     6/05/08 8:50a Bs185067
 * TAR 376639
 * 
 * 3     2/08/05 7:12p Tp151000
 * TAR 292777
 * 
 * 2     2/08/05 5:31p Tp151000
 * TAR 292777
 * 
 * 1     1/26/05 2:21p Dm185016
 * Moved to new Repository.
// 
// 2     11/30/04 4:22p Dm185016
// 
// 1     11/16/04 11:28a Dm185016
*/

#include "StdAfx.h"
#include "LocaleEx.h"
#include "PSXConnectionThread.h"

#include "psxmfc.h"
#include "RCManager.h"
#include "verify.h"
#include "XTrace.h"


using namespace fastlane;

//int Locale::m_nLocaleShortDateOrdering = 0;
//int Locale::m_nCenturyDigitCount = 0 ;
//StringType Locale::m_csLocaleDateSeparator = _T("");
//StringType Locale::m_csLocaleShortDateFormat = _T("");

//int CLocale::GetCenturyDigitCount() { return m_nCenturyDigitCount; }
//int CLocale::GetLocaleShortDateOrdering() { return m_nCenturyDigitCount; }
int CLocale::GetCenturyDigitCount() { return CLocale::instance()->m_nCenturyDigitCount; }
int CLocale::GetLocaleShortDateOrdering() { return CLocale::instance()->m_nLocaleShortDateOrdering; }

CLocale::CLocale()
{
    _TCHAR lpLCData[MAX_STRING];
    LCTYPE LCType;
    LCID Locale=LOCALE_USER_DEFAULT;    // Specifies the locale for which the currency string is to be formatted
    int cchData=MAX_STRING;

    // Read Date Short Format
    LCType = LOCALE_IDATE;                                  // Short date format-ordering specifier.
    ::GetLocaleInfo( Locale,                                    // locale identifier
                     LCType,                                    // type of information
                     lpLCData,          // address of buffer for information
                     cchData);                              // size of buffer
    m_nLocaleShortDateOrdering = _ttoi(lpLCData);

    // Read Century
    LCType = LOCALE_ICENTURY;                               // Specifier for full 4-digit century.
    ::GetLocaleInfo( Locale,                                    // locale identifier
                     LCType,                                    // type of information
                     lpLCData,          // address of buffer for information
                     cchData);                              // size of buffer
    int nCenturyCount = _ttoi(lpLCData);
    m_nCenturyDigitCount = nCenturyCount;

    // Read Character Separator
    LCType = LOCALE_SDATE;                                  // Character(s) for the date separator.
    ::GetLocaleInfo( Locale,                                    // locale identifier
                     LCType,                                    // type of information
                     lpLCData,          // address of buffer for information
                     cchData);                              // size of buffer
    m_csLocaleDateSeparator = lpLCData;

    // Read Short date formatting    (not used)
    LCType = LOCALE_SSHORTDATE;                             // Character(s) for the date separator.
    ::GetLocaleInfo( Locale,                                    // locale identifier
                     LCType,                                    // type of information
                     lpLCData,          // address of buffer for information
                     cchData);                              // size of buffer
    m_csLocaleShortDateFormat = lpLCData;
}


///////////////////////////////////////////////////////////////////////
// DateFormat - return date format
///////////////////////////////////////////////////////////////////////
CString CLocale::DateFormat(int month, int day, int year)
{
    CString date_format;
    CString year_format;

    CLocale &locale = *instance();

    year_format.Format(_T("%04d"), year);

    int nNoTotalDigit = 0;
    if (locale.m_nCenturyDigitCount == 0)
        nNoTotalDigit = 2;
    else
        nNoTotalDigit = 4;

        
    switch (locale.m_nLocaleShortDateOrdering)
    {
        case 0: // Month-Day-Year                
            date_format.Format(_T("%02d%02d%s")
                             , month, day, year_format.Right(nNoTotalDigit));
            break;
        case 1: // Day-Month-Year
            date_format.Format(_T("%02d%02d%s")
                             , day, month, year_format.Right(nNoTotalDigit));
            break;
        case 2: // Year-Month-Day
            date_format.Format(_T("%s%02d%02d")
                              , year_format.Right(nNoTotalDigit), month, day);
            break;
        default:
            break;
    }      

    return date_format;
    //return _T("");
}

///////////////////////////////////////////////////////////////////////
// DateFormat - return date format
///////////////////////////////////////////////////////////////////////
CString CLocale::GetDateFormat(HTRACE hTrace)
{
    CLocale &locale = *instance();
    CRCManager &rcm = *CSingleton<CRCManager>::instance();        
    Trace &traceObject = Trace::GetTraceObject(hTrace);
    CString csDateFormat;
    PSXRC rc = PSX_E_FAIL;
    
    switch (locale.m_nLocaleShortDateOrdering)
    {
        case 0: // Month-Day-Year
            if (locale.m_nCenturyDigitCount)
            {
               rc = rcm.psx().GetString(_T("DateFormat1"), csDateFormat);
               if (IsRCError(rc))
               {
                   traceObject.write(_T("psx().GetString(DateFormat1) error - ") + IString(RCToString(rc)), ITRACE_LEVEL_ERROR);
               }
            }       
            else
            {
               rc = rcm.psx().GetString(_T("DateFormat2"), csDateFormat);
               if (IsRCError(rc))
               {
                    traceObject.write(_T("psx().GetString(DateFormat2) error - ") + IString(RCToString(rc)), ITRACE_LEVEL_ERROR);
              }
            }       
            break;
        case 1: // Day-Month-Year
            if (locale.m_nCenturyDigitCount)
            {
               rc = rcm.psx().GetString(_T("DateFormat3"), csDateFormat);
               if (IsRCError(rc))
               {
                     traceObject.write(_T("psx().GetString(DateFormat3) error - ") + IString(RCToString(rc)), ITRACE_LEVEL_ERROR);
               }
            }
            else
            {
               rc = rcm.psx().GetString(_T("DateFormat4"), csDateFormat);
               if (IsRCError(rc))
               {
                    traceObject.write(_T("psx().GetString(DateFormat4) error - ") + IString(RCToString(rc)), ITRACE_LEVEL_ERROR);
              }
            }       
            break;
        case 2: // Year-Month-Day
            if (locale.m_nCenturyDigitCount)
            {
               rc = rcm.psx().GetString(_T("DateFormat5"), csDateFormat);
               if (IsRCError(rc))
               {
                   traceObject.write(_T("psx().GetString(DateFormat5) error - ") + IString(RCToString(rc)), ITRACE_LEVEL_ERROR);
               }
            }
            else
            {
               rc = rcm.psx().GetString(_T("DateFormat6"), csDateFormat);
               if (IsRCError(rc))
               {
                  traceObject.write(_T("psx().GetString(DateFormat6) error - ") + IString(RCToString(rc)), ITRACE_LEVEL_ERROR);
               }
            }       
            break;
        default:
                  traceObject.write(_T("Invalid locale.m_nLocaleShortDateOrdering") + IString(RCToString(rc)), ITRACE_LEVEL_ERROR);
            break;
    }
    return csDateFormat;
}

///////////////////////////////////////////////////////////////////////
// GetDateFormatCount - return the size of the date format string
///////////////////////////////////////////////////////////////////////
int CLocale::GetDateFormatCount()
{
    CString date_format;
    int iRet = 0;
    date_format = GetDateFormat(0);
    iRet = date_format.GetLength();
    iRet = iRet - 2;
    return iRet;
}
