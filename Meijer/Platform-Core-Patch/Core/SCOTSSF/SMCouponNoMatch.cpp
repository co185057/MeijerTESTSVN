//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCouponNoMatch.cpp
//
// TITLE: Class impliementation for coupon no match State
//
// This state accepts and handles inputs from:
// PSButton2 :  OK
// PSEnterKey:  like button 2
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// CHANGES: Start
// CHANGES: End
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMCouponNoMatch.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("CouponNoMatch")

IMPLEMENT_DYNCREATE(SMCouponNoMatch, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMCouponNoMatch::SMCouponNoMatch()
{
  IMPLEMENT_TIMESTAMP
}

