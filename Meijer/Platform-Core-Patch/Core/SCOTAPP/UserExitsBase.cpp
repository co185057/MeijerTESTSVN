//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  UserExitsBase.CPP
//
// TITLE: User Exits for SCOT
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV abstract base state


#define COMP_ID ID_UE               // base state component
#define T_ID    _T("UEBase")

//////////////////////////////////////////////////////////////////////////////////////////////////
// additional Initialization
long SMStateBase::UserExit00(void)
{
  return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// additional TB Display Message analysis for user mods or features
SMStateBase *SMStateBase::UserExit01(void)// before SCOT
{
  return STATE_NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// additional TB Display Message analysis for user mods or features
SMStateBase *SMStateBase::UserExit02(void)  // unknown msg, after SCOT
{
  return STATE_NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Customer Reciept print top line exit
long SMStateBase::UserExit15(CString& csWork)
{
	long nUserExit = 0;	// return = 0 or anything else execute normal processing
	//        = 1 continue processing without printing 

	return nUserExit;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Before TBITEMSOLD 
SMStateBase *SMStateBase::UserExit16(const bool fVoid,
														 const long lTares,
														 const CString csItemCode)
{
	return STATE_NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// additional UnInitialization
void SMStateBase::UserExit99(void)
{
  return;
}