//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTakeLoyaltyCard.cpp
//
// TITLE: Class impliementation file for Take LoyaltyCard State
//
//
//
// AUTHOR:    SCOT CORE TEAM, NCR Future Retail, ATLANTA
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMTakeLoyaltyCard.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("TakeLoyaltyCard")

DEFINE_TIMESTAMP

IMPLEMENT_DYNCREATE(SMTakeLoyaltyCard, CObject)// MFC Runtime class/object information


////////////////////////////
SMTakeLoyaltyCard::SMTakeLoyaltyCard(TBLOYALTYCARDTYPE LoyaltyCardType)
: SMTakeLoyaltyCardBase(LoyaltyCardType)

{
  IMPLEMENT_TIMESTAMP
}


////////////////////////////////////////
SMStateBase  *SMTakeLoyaltyCard::Deliver(TBLOYALTYCARDTYPE LoyaltyCardType)
{
  SMTakeLoyaltyCardBase::Deliver(LoyaltyCardType);


  return this;
}


