
#ifndef _CSCOTDataEntry
#define _CSCOTDataEntry



//#include "stdafx.h"
#include "afxtempl.h"
#include "DataEntry.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#elif defined _SCOT
class DLLEXPORT
#else
class 
#endif 
CSCOTDataEntry : public CDataEntry
   {
   public:
      // Mutators.
      // Populate all fields from an INI file section or Registry key.
      bool FromConfigData(CString sSource, CString sSection, bool bPrimLang = true);
	  CString GetSectionName() const;
      void SetSectionName(CString);
   };
#endif