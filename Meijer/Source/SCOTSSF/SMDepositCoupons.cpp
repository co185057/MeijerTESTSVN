//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMDepositCoupons.cpp
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
#include "SMDepositCoupons.h"       // MGV this state

#include "transbrokerssf.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("DepositCoupons")

IMPLEMENT_DYNCREATE(SMDepositCoupons, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMDepositCoupons::SMDepositCoupons()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMDepositCoupons::Initialize(void)
{
    CString strOptionName = _T("FastLaneConfiguration");
	CString m_csFLConfig;
	SendTBCommand(TBCMD_GET_OPTIONS, &strOptionName, &m_csFLConfig);
	//TAR336633
	if ( m_csFLConfig == _T("SuperMini"))
		return PSButton1();
	else
		return SMDepositCouponsBase::Initialize();
}

void SMDepositCoupons::UnInitialize(void)
{
    SMDepositCouponsBase::UnInitialize();
}

SMStateBase * SMDepositCoupons::PSButton1(void)
{
    return SMDepositCouponsBase::PSButton1();
}

