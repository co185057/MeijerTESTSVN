
#ifndef _CSCOTDataEntry
#define _CSCOTDataEntry



//#include "stdafx.h"
//
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
      bool FromConfigData(CString sSource, CString sSection, bool bPrimLang = true, bool bTranslate = true);
//RFQ 7477
	  CString GetSectionName() const;
      void SetSectionName(CString);
//RFQ 7477
   };
#endif