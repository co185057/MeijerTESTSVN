#ifndef _OVERRIDESCOTSTRINGLIST_H_
#define _OVERRIDESCOTSTRINGLIST_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//
#include "DllDefine.h"
#include "SCOTStringList.h"


#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 

  COverrideSCOTStringList : public CSCOTStringList  
{
public:
	//A 000 message
    BOOL RefreshList(CString m_csFileName, int iFileID, int tablesize);
    //E 000 message

};

#endif
