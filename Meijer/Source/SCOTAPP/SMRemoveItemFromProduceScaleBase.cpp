//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMRemoveItemFromProduceScaleBase.cpp
//
// TITLE: Class implimentation for Remove Item From Produce Scale State
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#ifndef _CPPUNIT
#include "Common.h"                

#include "SMRemoveItemFromProduceScale.h"     
#else
#include "SMRemoveItemFromProduceScaleBase.h"     
#endif



#define COMP_ID ID_SM               // base state component
#define T_ID _T("RemoveItemFromProduceScaleBase")

IMPLEMENT_DYNCREATE(SMRemoveItemFromProduceScaleBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(RemoveItemFromProduceScale)


DEFINE_TIMESTAMP

////////////////////////////////
SMRemoveItemFromProduceScaleBase::SMRemoveItemFromProduceScaleBase()
{
  IMPLEMENT_TIMESTAMP
}

///////////////////////////////////////
SMStateBase  *SMRemoveItemFromProduceScaleBase::Initialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("RemoveItemFromProduceScale") );	// RFC 330050

    if (lDMScaleWeight == 0) 
    {
        return TBEnterItem();
    }
    
    nTimeOut = co.getTimeOutRemoweItemFromProduceScaleAudio();//Set a timer to play the audio
    // NewUI
	ps.SetCMFrame(_T("RemoveItemFromProduceScale"));

	ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
	ps.ShowCMTBText(csTBMessage2Scot);	//thp

    // End NewUI
    
	ps.ShowCMFrame();
    return STATE_NULL;                // stay in RemoveItemFromProduceScaleBase state
}


////////////////////////////////////
SMStateBase  *SMRemoveItemFromProduceScaleBase::DMScale(void) //
{
    if (lDMScaleWeight>=0)
    {
        ps.Weight(lDMScaleWeight);
    }

    if (lDMScaleWeight == 0) 
    {
        return TBEnterItem();
    }

    return STATE_NULL;
}


void SMRemoveItemFromProduceScaleBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("RemoveItemFromProduceScale") );	
}

SMStateBase  *SMRemoveItemFromProduceScaleBase::TimedOut(void)
{
    DMSayPhrase(REMOVEITEMFROMPRODUCESCALE);
    return STATE_NULL;
}
