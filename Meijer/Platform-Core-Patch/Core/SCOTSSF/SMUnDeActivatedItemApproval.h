//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:   SMUnDeActivatedItemApproval.h
//
// TITLE:  Class header for SMUnDeActivatedItemApproval state
//
// AUTHOR: Anurag Mishra
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SMUNDEACTIVATEDITEMAPPROVAL__
#define __SMUNDEACTIVATEDITEMAPPROVAL__

#include "SMUnDeActivatedItemApprovalBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMUnDeActivatedItemApproval : public SMUnDeActivatedItemApprovalBase
{
public:
	SMUnDeActivatedItemApproval();

  DECLARE_DYNCREATE(SMUnDeActivatedItemApproval)// MFC Runtime class/object information
};

#endif
