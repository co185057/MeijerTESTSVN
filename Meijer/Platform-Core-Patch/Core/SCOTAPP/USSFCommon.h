#include "USSFUtility.h"

#ifdef _BUILDLIB_
#ifdef _SCOTSSF_
DLLIMPORT extern USSFUtilityBase &USSFutil;			// global PS object used by many
#else 
DLLEXPORT extern USSFUtilityBase &USSFutil;           // global PS object used by many
#endif

#else

#ifdef _SCOTSSF_
DLLEXPORT extern USSFUtility &USSFutil;			// global PS object used by many
#else 
DLLIMPORT extern USSFUtility &USSFutil;           // global PS object used by many
#endif

#endif