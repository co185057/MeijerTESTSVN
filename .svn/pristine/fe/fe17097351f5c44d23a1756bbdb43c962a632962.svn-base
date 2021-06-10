// Module copied directly from OPOS for MICR support
/*************************************************************************
 *
 * Functions at file scope.
 * 
 * $Workfile:: GlobalFunctions.cpp                                       $
 *
 * $Revision:: 3                                                         $
 *
 * $Archive:: /OPOS/Rel.2_3/Services/NCR71xx/GlobalFunctions.cpp         $
 *
 * $Date:: 8/09/02 5:13p                                                 $
 *
 *************************************************************************
 *
 * Copyright (c) NCR Corporation 1997.  All rights reserved.
 *
 *************************************************************************
 *
 * $History:: GlobalFunctions.cpp                                        $
 * 
 * *****************  Version 3  *****************
 * User: Barlom       Date: 8/09/02    Time: 5:13p
 * Updated in $/OPOS/Rel.2_3/Services/NCR71xx
 * Check-In changes made by O.Nakada for TAR # 209978, 209979
 * 
 * *****************  Version 16  *****************
 * User: Standw       Date: 3/24/00    Time: 9:42p
 * Updated in $/OPOS/Rel.1_8/Services/NCR71xx
 * Added support for Windows CE
 * 
 * *****************  Version 12  *****************
 * User: Duffem       Date: 11/04/98   Time: 6:51p
 * Updated in $/OPOS/Rel.1_5/Services/NCR71xx
 * Store Minder 2.0 integration.
 * 
 * *****************  Version 10  *****************
 * User: Admin        Date: 7/28/98    Time: 10:25a
 * Updated in $/OPOS/Rel.1_4/Services/NCR71xx
 * 
 * *****************  Version 9  *****************
 * User: Admin        Date: 7/26/98    Time: 9:03p
 * Updated in $/OPOS/Rel.1_4/Services/NCR71xx
 * MND, moved private data security member variables to public.
 * Was getting protection violations in CMClass lib.
 * 
 * *****************  Version 8  *****************
 * User: Admin        Date: 5/05/98    Time: 5:17p
 * Updated in $/OPOS/Rel.1_4/Services/NCR71xx
 * KBase Q173065
 * 
 * *****************  Version 6  *****************
 * User: Duffem       Date: 9/15/97    Time: 11:06a
 * Updated in $/OPOS/Rel.1_2/Services/NCR71xx
 * Added VSS file header
 * 
 *
 ************************************************************************/
 

#include "StdAfx.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// WTS: Windows CE Updates (4-99)
// Prototype for binary search routine
#ifdef _WIN32_WCE
int MyBSearch(int LowerBound, int UpperBound, long Number, const LookupTable *pLT);
#endif

/////////////////////////////////////////////////////////////////////////////
// Perform linear search for a number:
//   If found, returns corresponding string.
//   Else returns string in first element.
//   -> In either case, a "format" is performed using the number, so that
//       the number can optionally be formatted into the string.

CString LookupNumber( long nNumber, const LookupTable* pLT, long nCount )
{
    LPCTSTR pszString = pLT->pszText;

    while ( pLT++, nCount-- )
    {
        if ( pLT->nNumber == nNumber )
        {
            pszString = pLT->pszText;
            break;
        }
    }
    CString s;
    s.Format( pszString, nNumber );
    return s;
}


/////////////////////////////////////////////////////////////////////////////
// Perform binary search for a number.
//   (The LookupTable elements must be arranged with nNumber values from
//    smallest to biggest.)
//   If found, returns corresponding string.
//   Else returns string in first element.

static int CompareLT(
    const void* p1,             // First "pointer" is the number we are looking for.
    const void* p2 )            // Second pointer points to LookupTable element.
{
    return (long) p1 - ( (LookupTable*) p2 )->nNumber;
}

CString LookupNumberBinary( long nNumber, const LookupTable* pLT, long nCount )
{
    LPCTSTR pszString = pLT->pszText;   // Default string.

// WTS: Windows CE Updates (3-99)
// bsearch is not supported under Windows CE
#ifndef _WIN32_WCE

    LookupTable* pElement = (LookupTable*) bsearch(
        (const void *) nNumber,         // Key is the number (cast a "long" to a "pointer").
        (const void *) (pLT + 1 ),      // Pointer to start of array.
        nCount,                         // Elements in the array.
        sizeof(LookupTable),            // Size of each element.
        &CompareLT );                   // Pointer to compare function.

#else

    // Implement binary search myself
    int Index;      // Index of the matching entry

    // Perform the search
    Index = MyBSearch(0, nCount-1, nNumber, pLT);

    // pElement points to the matching entry, if found.
    // If no match, pElement is NULL;
    LookupTable *pElement;

    if (Index == -1)
        pElement = NULL;
    else
        pElement = (LookupTable *) &pLT[Index];

#endif


    if ( pElement != NULL )
        pszString = pElement->pszText;

    CString s;
    s.Format( pszString, nNumber );
    return s;
}

// WTS: Windows CE Updates (4-99)
// Add a binary search routine, since bsearch is not supported under CE
#ifdef _WIN32_WCE

// ****************************************************************************
//
// Routine:     MyBSearch
//
// Purpose:     Performs a binary search
//              Note that this is a recursive routine
//
//              NOTE:   Lower and upper bounds must be signed.
//                      If they are unsigned, and we subtract 1 from lower
//                      bound 0, we get a big positive number and don't break out.
//
// Parameters:  
//              LowerBound      Lower bound of the portion of the array to search
//              UpperBound      Upper bound of the portion of the array to search
//              Number          The number to search for
//              pLT             Pointer to lookup table
//
// Returns:     
//              The index of the number, if found
//              -1 otherwise
//
// ****************************************************************************
int MyBSearch(int LowerBound, int UpperBound, long Number, const LookupTable *pLT)
{
    int     Mid;    // Midpoint of the section of the array we are searching

    // If the lower bound is more than the upper bound, then we have
    // run out of elements to search, and the number is not present.
    if (LowerBound > UpperBound)
        return(-1);

    // Calculate midpoint
    Mid = (LowerBound + UpperBound) / 2;

    // Check for a match
    if (pLT[Mid].nNumber == Number)
        return(Mid);

    // If no match, search the half of the array where the number must reside
    else if (pLT[Mid].nNumber > Number)
        return( MyBSearch(LowerBound, Mid-1, Number, pLT) );
    else
        return( MyBSearch(Mid+1, UpperBound, Number, pLT) );
}

#endif // _WIN32_WCE


// Added 2-byte code page support - 07/2002, TAR 209978, 209979
/////////////////////////////////////////////////////////////////////////////
// Test a character type.
//   If a data is two-byte character, returns a none zero value.
//   Else returns 0.
//   This function only support under Windows CE.
//   If a system is MBCS (no UNICODE), returns 0.

BOOL IsTwoByteChar(unsigned int nUnicode, long nCP, BOOL b2ByteMode)
{
#ifdef _WIN32_WCE
	BOOL	b2Byte;
	BYTE	nHData, nLData;

	b2Byte = TRUE;
	nHData = HIBYTE(nUnicode);
	nLData = LOBYTE(nUnicode);

	switch (nCP)
	{
	case 932:
		if (! b2ByteMode)
		{
			b2Byte = FALSE;
		}
		else if (nHData == 0x00)
		{
			b2Byte = FALSE;
		}
		else if (0xFF == nHData)
		{
			if (0x61 <= nLData && nLData <= 0x9F)
			{
				b2Byte = FALSE;
			}
		}
		break;
	case 936:
	case 949:
	case 950:
		if (nHData == 0x00)
		{
			b2Byte = FALSE;
		}
		break;
	default:
		b2Byte = FALSE;
	}

	return b2Byte;
#else
	return FALSE;
#endif
}


//..end..
