#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Core/Include/StringComparator.h 1     6/23/08 4:55p Sa250050 $
/*
 * @(#)StringComparator.h    $Revision: 1 $ $Date: 6/23/08 4:55p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Core/Include/StringComparator.h $
 * 
 * 1     6/23/08 4:55p Sa250050
 * FastLane 4.0 E5.3 Build 291 - Core Source
 * 
 * 4     5/31/05 1:26p Dm185016
 * Added operator ==
 * 
 * 1     1/26/05 2:32p Dm185016
 * Moved to new Repository.
* 
* 7     10/14/04 1:52p Dm185016
* Added operator for CString comparisons
* 
* 6     10/07/04 9:44a Dm185016
* Removed StringType definition.
* 
* 5     10/04/04 6:56p Dm185016
* Fixed standalone compile problem
* 
* 3     8/04/04 3:22p Dm185016
* Added additional protection for double inclusion.  If the file is in
* two different directories as is those shared by SecurityManager and
* SCOTAPP, it is possible that both files will be included, depending on
* the search order.  #pragma once does not protect for this situation.
* 
* 2     2/18/04 9:16a Dm185016
* Added operator() for const _TCHAR * operands
* 
* 1     1/23/04 3:40p Dm185016
* Renamed from KeyComparator and moved from state machine
*/

// The following is required since this file is shared by both the SecurityManager
// and SCOTAPP.  It will appear in multiple directories.  #pragma once does not
// protect from this.
#ifndef _STRING_COMPARATOR_
#define _STRING_COMPARATOR_


// Binary operation for comparing std::strings used as keys in a map.
// For some reason, the default implementation does not work correctly in
// library.  The comparison always seems to compare the pointers to the
// strings rather than the values of the strings themselves.  This leads
// to the incorrect construction of the map.
struct StringComparator
{
    bool operator()(const CString& key1, const CString& key2) const
    {
        return (key1.Compare(key2) < 0);
    }


    bool operator()(const StringType& key1, const StringType&key2) const
    {
        const _TCHAR *pKey1 = key1.c_str();
        const _TCHAR *pKey2 = key2.c_str();
        return (_tcscmp(pKey1, pKey2) < 0);
    }


    bool operator()(const _TCHAR *pKey1, const _TCHAR *pKey2) const
    {
        return (_tcscmp(pKey1, pKey2) < 0);
    }
};


inline bool operator==(const StringType& key1, const StringType&key2)
{
    const _TCHAR *pKey1 = key1.c_str();
    const _TCHAR *pKey2 = key2.c_str();
    return (_tcscmp(pKey1, pKey2) == 0);
}

#endif