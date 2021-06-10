//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMInSAProgress.h
//
// Description: The InSAProgress state is used when the transition from 
//			TB_ITEMSOLD to the next Security event, which is the SASERVER WLBD
//			lookup return, and might take some time.  
//
// Author:  Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _INSAPROGRESSSTATE
#define _INSAPROGRESSSTATE

#include "SMInSAProgressBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMInSAProgress : public SMInSAProgressBase
{
public:
  // Constructor
  SMInSAProgress();
 	
  DECLARE_DYNCREATE(SMInSAProgress)// MFC Runtime class/object information
};

#endif
