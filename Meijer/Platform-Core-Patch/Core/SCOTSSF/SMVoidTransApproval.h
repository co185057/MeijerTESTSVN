//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMVoidTransApproval.h
//
// TITLE: Class header for VoidTransApproval
//
// AUTHOR:    Aiqin Hou
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _VOIDTRANSAPPROVALSTATE
#define _VOIDTRANSAPPROVALSTATE

#include "SMVoidTransApprovalBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMVoidTransApproval : public SMVoidTransApprovalBase
{
public:
	SMVoidTransApproval();
	
  DECLARE_DYNCREATE(SMVoidTransApproval)// MFC Runtime class/object information
};

#endif
