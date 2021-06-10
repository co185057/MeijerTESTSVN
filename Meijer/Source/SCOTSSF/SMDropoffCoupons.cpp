//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMDropoffCoupons.cpp
//
// TITLE: Class implimentation for deposit coupons state
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMDropoffCoupons.h"       // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("DropoffCoupons")

IMPLEMENT_DYNCREATE(SMDropoffCoupons, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMDropoffCoupons::SMDropoffCoupons()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMDropoffCoupons::Initialize(void)
{
    return SMDropoffCouponsBase::Initialize();
}

void SMDropoffCoupons::UnInitialize(void)
{
    SMDropoffCouponsBase::UnInitialize();
}

SMStateBase * SMDropoffCoupons::PSButton1(void)
{
    return SMDropoffCouponsBase::PSButton1();
}
