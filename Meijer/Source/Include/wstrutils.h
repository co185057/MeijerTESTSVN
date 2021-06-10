#pragma once
// $Header: /Fastlane/4.0 Patch B/Component Utilities/src/util/wstrutils.h 1     1/26/05 2:32p Dm185016 $
/*
 * @(#)wstrutils.h    $Revision: 1 $ $Date: 1/26/05 2:32p $
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
* $Log: /Fastlane/4.0 Patch B/Component Utilities/src/util/wstrutils.h $
 * 
 * 1     1/26/05 2:32p Dm185016
 * Moved to new Repository.
* 
* 3     6/30/04 3:49p Dm185016
* Lint.
* 
* 2     1/23/04 3:42p Dm185016
* Added hdr
*/

using namespace std;

//lint -emacro(*, A2W)


//-----------------------------------------------------------------------
// Equality operators for comparing wstring to char *, eg.
//
//      wstring someString;
//      if (someString == "some ascii string" )
//           ...
//
//      if ( "some ascii string" == someString )
//           ...
//
// Neither of these operators is defined in the string.h headers.
//lint --e{732}
inline  bool __cdecl operator==(
        const char * _Left,
        const std::wstring& _Right)
{   // test for NTCS vs. string equality
    USES_CONVERSION;
    LPWSTR theLeft = A2W(_Left);

    return (_Right.compare(theLeft) == 0);
}


//lint --e{732}
inline  bool __cdecl operator==(
        const std::wstring& _Left,
        const char *_Right)
{   // test for string vs. NTCS equality
    USES_CONVERSION;
    LPWSTR theRight = A2W(_Right);

    return (_Left.compare(theRight) == 0);
}


inline  bool __cdecl operator==(
        const std::wstring& _Left,
        const wchar_t *_Right)
{   // test for string vs. NTCS equality
    return (_Left.compare(std::wstring(_Right)) == 0);
}
