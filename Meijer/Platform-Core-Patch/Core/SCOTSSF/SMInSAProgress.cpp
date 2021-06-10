//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMInSAProgress.cpp
//
// Description: The InSAProgress state is used when the transition from 
//			TB_ITEMSOLD to the next Security event, which is the SASERVER WLBD
//			lookup return, and might take some time.  
//
// Author:  Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////
 
#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMInSAProgress.h"           // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID _T("InSAProgress")

IMPLEMENT_DYNCREATE(SMInSAProgress, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////////////////////
SMInSAProgress::SMInSAProgress()
{
  IMPLEMENT_TIMESTAMP
		
}