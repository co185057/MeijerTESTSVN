//////////////////////////////////////////////////////////////////////////////////////////////////
//  
// 
#ifndef _USSFUTILITY
#define _USSFUTILITY

#include "USSFUtilityBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
 USSFUtility : public USSFUtilityBase
{
public:
	USSFUtility();

};
#endif	