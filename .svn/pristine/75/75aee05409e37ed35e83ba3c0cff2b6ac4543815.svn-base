//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMEnterPasswordBase.CPP
//
// TITLE: Class implementation for Enter Password state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header
#ifndef _CPPUNIT
#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMEnterID.h"      
#else
#include "FakePSProcedures.h"
#define PSProceduresBase CFakePSProcedures
#endif //#ifndef _CPPUNIT

#include "SMEnterPassword.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("EnterPasswordBase")

IMPLEMENT_DYNCREATE(SMEnterPasswordBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(EnterPassword)

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMEnterPasswordBase::SMEnterPasswordBase()
{
  IMPLEMENT_TIMESTAMP
  saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;
}

/////////////////////////////////////////
SMStateBase  *SMEnterPasswordBase::Initialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("EnterPassword") );	// RFC 330050
	ps.SetCMFrame( _T("EnterID") ); // EnterPassword //
	ps.SetCMLeadthruText( TXT_ENTER_PASSWORD );
	ps.Message12( MSG_ENTERPASSWORD_PRESSENTER, false );
	ps.AllowInput( co.GetnOperationsOperatorPWMaxDigit(), true, false );

	// ++ NextGenUI: Fix issue on pressing Go Back will not go back to EnterID context
	m_pAppPSX->SetTransactionVariable( _T("NextGenData"), COleVariant(_T("EnterPassword")) );
	
	ps.ShowNumericKeypad();
	ps.ShowCMTBText( csTBMessage2Scot );	//thp
	
	ps.ShowCMFrame( false );
		
	return STATE_NULL;
}

////////////////////////////////////////
SMStateBase  *SMEnterPasswordBase::PSButtonGoBack(void) // previous
{
   return STATE_NULL;    
    
}

//////////////////////////////////////////////////////
SMStateBase  *SMEnterPasswordBase::PSNumericKey(const long lKey)
{
  PSEchoInput(lKey);
  return STATE_NULL;
}

/////////////////////////////////////////
SMStateBase  *SMEnterPasswordBase::PSClearKey(void)
{

    PSClearAllInput(true);

  return STATE_NULL;
}

/////////////////////////////////////////
SMStateBase  *SMEnterPasswordBase::PSEnterKey(void)// enter key pressed, sell this item
{
   return STATE_NULL;

}

// + RFC 330050
void SMEnterPasswordBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("CollectGiftCard") );	
}
// - RFC 330050
