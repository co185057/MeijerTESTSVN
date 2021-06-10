//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMRAPDataNeeded.h
//
// TITLE: Class header for wait for approval state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _RAPDataNeededSTATE
#define _RAPDataNeededSTATE


#include "SMRAPDataNeededBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMRAPDataNeeded : public SMRAPDataNeededBase
{
public:
   typedef enum
      {
      RAPDATA_LASTVIEW           // Highest element in the list, place others before it.
      } RAPDATANEEDEDVIEW;

	SMRAPDataNeeded(LPCTSTR sDesc, int View);
	SMRAPDataNeeded();
	SMStateBase *Initialize();
	SMStateBase *Deliver(LPCTSTR sDesc, int View);
	SMStateBase *Deliver(LPCTSTR sDesc, int View, LPCTSTR pPtr, const bool storeMode);


  DECLARE_DYNCREATE(SMRAPDataNeeded)// MFC Runtime class/object information
};

#endif
