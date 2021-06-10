//Global function for parsing file path with environment variable


#ifndef _INC_FASTLANEUTILS
#define _INC_FASTLANEUTILS

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <tchar.h> 
#include <stdlib.h> 
bool GetPartitionPath( LPCTSTR sSrc, LPTSTR sDest );
bool ExpandPartitionStrings( LPCTSTR sSrc, LPTSTR sDest );

// Convenience macro for GetPartitionPath that logs
// failed path resolutions.
#if defined(_SCOT) || defined(_SCOTSSF_)
#define GET_PARTITION_PATH(src, dst) \
do \
{ \
	TCHAR _szTmp[_MAX_PATH]; \
	if(! GetPartitionPath((src), _szTmp) ) \
		trace(L6, _T("ERROR:  GetPartitionPath() could not resolve '%s'"), (src)); \
	_tcscpy((dst), _szTmp); \
} \
while(0)
#else
#define GET_PARTITION_PATH(src, dst) \
do \
{ \
	TCHAR _szTmp[_MAX_PATH]; \
	GetPartitionPath((src), _szTmp); \
	_tcscpy((dst), _szTmp); \
} \
while(0)
#endif

#endif// _INC_FASTLANEUTILS