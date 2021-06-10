//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCouponTooHigh.cpp
//
// TITLE: Class implimentation for coupon too high
//
//
// AUTHOR:   Gary Miller
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMCouponTooHigh.h"          // MGV this

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("CouponTooHigh")

IMPLEMENT_DYNCREATE(SMCouponTooHigh, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////
SMCouponTooHigh::SMCouponTooHigh()
{
  IMPLEMENT_TIMESTAMP
}

