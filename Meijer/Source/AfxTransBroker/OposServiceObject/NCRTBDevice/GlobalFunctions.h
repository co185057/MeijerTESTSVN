// Module copied directly from OPOS for MICR support
/*************************************************************************
 *
 * Functions at file scope.
 * 
 * $Workfile:: GlobalFunctions.h                                         $
 *
 * $Revision:: 3                                                         $
 *
 * $Archive:: /OPOS/Rel.2_3/Services/NCR71xx/GlobalFunctions.h           $
 *
 * $Date:: 8/09/02 5:21p                                                 $
 *
 *************************************************************************
 *
 * Copyright (c) NCR Corporation 1997.  All rights reserved.
 *
 *************************************************************************
 *
 * $History:: GlobalFunctions.h                                          $
 * 
 * *****************  Version 3  *****************
 * User: Barlom       Date: 8/09/02    Time: 5:21p
 * Updated in $/OPOS/Rel.2_3/Services/NCR71xx
 * Check-In change made by O.Nakada for TAR # 209978, 209979
 * 
 * *****************  Version 15  *****************
 * User: Standw       Date: 3/24/00    Time: 9:42p
 * Updated in $/OPOS/Rel.1_8/Services/NCR71xx
 * Added support for Windows CE
 * 
 * *****************  Version 11  *****************
 * User: Duffem       Date: 11/04/98   Time: 6:51p
 * Updated in $/OPOS/Rel.1_5/Services/NCR71xx
 * Store Minder 2.0 integration.
 * 
 * *****************  Version 9  *****************
 * User: Admin        Date: 7/28/98    Time: 10:25a
 * Updated in $/OPOS/Rel.1_4/Services/NCR71xx
 * 
 * *****************  Version 8  *****************
 * User: Admin        Date: 7/26/98    Time: 9:03p
 * Updated in $/OPOS/Rel.1_4/Services/NCR71xx
 * MND, moved private data security member variables to public.
 * Was getting protection violations in CMClass lib.
 * 
 * *****************  Version 7  *****************
 * User: Admin        Date: 5/05/98    Time: 5:17p
 * Updated in $/OPOS/Rel.1_4/Services/NCR71xx
 * KBase Q173065
 * 
 * *****************  Version 5  *****************
 * User: Duffem       Date: 9/15/97    Time: 11:06a
 * Updated in $/OPOS/Rel.1_2/Services/NCR71xx
 * Added VSS file header
 * 
 *
 ************************************************************************/
 

#ifndef _GlobalFunctions_H
#define _GlobalFunctions_H


/////////////////////////////////////////////////////////////////////////////
// Preprocessor macro functions...

// Convert an expression to a boolean value.
#define TOBOOL(x) ((x) != 0)            // Returns TRUE or FALSE.

// Form a bitmask, based on a bit number.
#define BIT(x) (1<<(x))

// Lock and unlock a CmMutex:
//   Debugging version captures info. Release version doesn't.
#ifdef _DEBUG
    #define LockMutex(x)   \
        ( dc.DCPrintf( DCM_MUTEX, _T(" ..Lock   ") _T(#x) _T(": (%d) %s"), __LINE__, THIS_FILE ), \
          x.Lock() )
    #define UnlockMutex(x) \
        ( dc.DCPrintf( DCM_MUTEX, _T(" ..Unlock ") _T(#x) _T(": (%d) %s"), __LINE__, THIS_FILE ), \
          x.Unlock() )
#else
    #define LockMutex(x)   x.Lock()
    #define UnlockMutex(x) x.Unlock()
#endif


/////////////////////////////////////////////////////////////////////////////
// Search functions...

// Both functions use a lookup table, consisting of an array of "LookupTable"
//   elements.
//   - The first element contains the string to use if not found;
//      the number is not used.
//   - The second and succeeding elements contain a number and its corresponding
//      strings.
class LookupTable
{
public:
    long    nNumber;
    LPCTSTR pszText;
};

// Calculate the number of elements in a LookupTable array.
#define LTCount(aLT) (sizeof(aLT)/sizeof(LookupTable)-1)

// Perform linear search for a number:
//   If found, returns corresponding string.
//   Else returns string in first element.
//   -> In either case, a "format" is performed using the number, so that
//       the number can optionally be formatted into the string.
CString LookupNumber( long nNumber, const LookupTable* pLT, long nCount );

// Perform binary search for a number.
//   (The LookupTable elements must be arranged with nNumber values from
//    smallest to biggest.)
//   If found, returns corresponding string.
//   Else returns string in first element.
//   -> In either case, a "format" is performed using the number, so that
//       the number can optionally be formatted into the string.
CString LookupNumberBinary( long nNumber, const LookupTable* pLT, long nCount );

// Added 2-byte code page support - 07/2002, TAR 209978, 209979
// Test a character type.
//   If a data is two-byte character, returns a none zero value.
//   Else returns 0.
//   This function only support under Windows CE.
//   If a system is MBCS (no UNICODE), returns 0.
BOOL IsTwoByteChar(unsigned int nUnicode, long nCP, BOOL b2ByteMode);

#endif

//..end..
