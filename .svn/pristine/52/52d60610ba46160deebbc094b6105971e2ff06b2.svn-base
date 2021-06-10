//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMEnterIDBase.CPP
//
// TITLE: Class implementation for Enter ID state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header
#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMEnterID.h"      
#include "SMEnterPassword.h"      

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("EnterIDBase")

IMPLEMENT_DYNCREATE(SMEnterIDBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(EnterID)

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMEnterIDBase::SMEnterIDBase()
{
  IMPLEMENT_TIMESTAMP
  saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;
}

/////////////////////////////////////////
SMStateBase  *SMEnterIDBase::Initialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("EnterID") );	// RFC 330050
    SMStateBase *sReturnState = CkStateStackAndReturn();
    if (sReturnState != STATE_NULL)
      return sReturnState;
	ps.SetCMFrame( _T("EnterID") );
	ps.SetCMLeadthruText( TXT_ENTER_ID );
	ps.Message12( MSG_ENTERID_PRESSENTER, false );
	ps.AllowInput( MAX_DIGITS_OPERATOR, false, false );
	
	ps.ShowCMTBText(csTBMessage2Scot);	//thp
	
	ps.ShowCMFrame( false );   
	
	return STATE_NULL;    
}

////////////////////////////////////////
SMStateBase  *SMEnterIDBase::PSButtonGoBack(void) // Previous 
{
  return setAndCreateAnchorState(BES_SCANANDBAG);	
    
}

//////////////////////////////////////////////////////
SMStateBase  *SMEnterIDBase::PSNumericKey(const long lKey)
{
  PSEchoInput(lKey);
  return STATE_NULL;
}

/////////////////////////////////////////
SMStateBase  *SMEnterIDBase::PSClearKey(void)
{

    PSClearAllInput(true);

  return STATE_NULL;
}

/////////////////////////////////////////
SMStateBase  *SMEnterIDBase::PSEnterKey(void)// enter key pressed, sell this item
{
  
  ps.GetInput(csOperator);
  if (csOperator != _T(""))
  {
    return 0; //RETURNSTATE(EnterPassword);
  }
  else
  {
    // Nothing entered - leave and continue to prompt for operator
    DMSayPhrase(KEYPADINVALID);
    return false;
  } 
}
// + RFC 330050
void SMEnterIDBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("EnterID") );	
}
// - RFC 330050
