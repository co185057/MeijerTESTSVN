#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/strutils.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)strutils.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/strutils.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 3     3/11/05 1:15p Dm185016
 * 
 * 2     3/03/05 5:25p Dm185016
 * 
 * 1     3/03/05 4:30p Dm185016
 * TAR 291700
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
static inline  bool __cdecl operator==(
        const char * _Left,
        const std::string& _Right)
{   // test for NTCS vs. string equality
    return (strcmp(_Left, _Right.c_str()) == 0);
}


static inline  bool __cdecl operator==(
        const wchar_t * _Left,
        const std::string& _Right)
{   // test for NTCS vs. string equality
    CW2A theLeft(_Left);

    return (strcmp((const char *)theLeft, _Right.c_str()) == 0);
}


//lint --e{732}
static inline  bool __cdecl operator==(
        const std::string& _Left,
        const char *_Right)
{   // test for string vs. NTCS equality
    return (strcmp(_Left.c_str(), _Right) == 0);
}


static inline  bool __cdecl operator==(
        const std::string& _Left,
        const wchar_t *_Right)
{   // test for string vs. NTCS equality
    CW2A theRight(_Right);

    return (strcmp(_Left.c_str(), (const char *)theRight) == 0);
}
