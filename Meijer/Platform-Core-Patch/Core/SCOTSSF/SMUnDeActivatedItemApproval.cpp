//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:     SMUnDeActivatedItemApproval.CPP
//
// TITLE:    Class implementation for SMUnDeActivatedItemApproval
//
// AUTHOR:   Anurag Mishra
//
//////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMUnDeActivatedItemApproval.h"

#define COMP_ID ID_SM                        // Base state component
#define T_ID    _T("UnDeActivatedItemApproval")


IMPLEMENT_DYNCREATE(SMUnDeActivatedItemApproval, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

 

//////////////////////////////////////////
SMUnDeActivatedItemApproval::SMUnDeActivatedItemApproval()
{
  IMPLEMENT_TIMESTAMP
}

