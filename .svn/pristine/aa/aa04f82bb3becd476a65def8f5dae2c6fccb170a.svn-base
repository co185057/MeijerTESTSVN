//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMProcessing.cpp
//
// TITLE: Class impliementation for check signature state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes
#include "SMState.h"                // base state

#include "SMProcessing.h"     // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("Processing")

IMPLEMENT_DYNCREATE(SMProcessing, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////////////
SMProcessing::SMProcessing()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMProcessing::Initialize(void)
{
    SMStateBase *returnState = SMProcessingBase::Initialize();
	
    //POS36318 - change the logo to mperks logo if its for mperks screen - rrs
    if( GetHookProperty(_T("__POSState")).Find(_T("DIGITAL_COUPON")) != -1 )
    {
        m_pAppPSX->SetConfigProperty(_T("SmallmperksLogo"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_TRUE);
    }
    else
    {        
        m_pAppPSX->SetConfigProperty(_T("SmallmperksLogo"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_FALSE);
    }
    //e POS36318 - Reset the small logo - rrs
	
    return returnState;
}