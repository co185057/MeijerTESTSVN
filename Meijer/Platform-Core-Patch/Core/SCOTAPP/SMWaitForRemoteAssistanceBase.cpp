//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMWaitForRemoteAssistanceBase.CPP
//
// TITLE: Class implementation for Wait for Approval state
//
// TAR#102019: LPM120999 - AllowCoupons has 3 options now.  I reformated this entire file! It was very hard to read
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMWaitForRemoteAssistance.h"  // This state
#include "SMSmAuthorizationBase.h"
#include "SMSmAssistMode.h"
#include "SMSmAssistMenu.h"
#include "SMSmAbort.h"				// Tar 194454 RF020702
#include "SMSmAuthorization.h"		// Tar 194454 RF020702
#include "SMSmReportsMenu.h"
#include "SMSmDataEntry.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID _T("WaitForRemoteAssistanceBase")

IMPLEMENT_DYNCREATE(SMWaitForRemoteAssistanceBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP


////////////////////////////////////////
SMWaitForRemoteAssistanceBase::SMWaitForRemoteAssistanceBase()
: csCurContext(EMPTY_STRING)
{
	IMPLEMENT_TIMESTAMP
}

SMWaitForRemoteAssistanceBase::SMWaitForRemoteAssistanceBase( LPCTSTR sMachineName, LPCTSTR sRemoteContext ) :
	m_sMachineName( sMachineName ),
	m_sRemoteContext( sRemoteContext ), csCurContext(EMPTY_STRING)
{
	IMPLEMENT_TIMESTAMP
}

SMStateBase  *SMWaitForRemoteAssistanceBase::Deliver( LPCTSTR sMachineName, LPCTSTR sRemoteContext )
{
	m_sMachineName = sMachineName;
	m_sRemoteContext = sRemoteContext;
	return this;
}
bool SMWaitForRemoteAssistanceBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    // There should be two parameters, both strings, a machine name and a context
    if (dlParameterList.GetDescriptorCount() == 2)
    {
        trace(L6, _T("+SMWaitForRemoteAssistanceBase::DeliverDescriptorList() - Parameter Count of 2"));
        Deliver(DLParamCString(dlParameterList),    // Machine Name
                DLParamCString(dlParameterList));   // Remote Context
        return true;
    }
    else
    {
        trace(L2, _T("+SMWaitForRemoteAssistanceBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}

/////////////////////////////////////////
SMStateBase  *SMWaitForRemoteAssistanceBase::TimedOut(void)
{
  if (nSaidCount<10)
  {
    DMSayPhrase(WAITFORASSISTANCE);
    nSaidCount++;
  }
    
  return STATE_NULL;
}

///////////////////////////////////////////
SMStateBase  *SMWaitForRemoteAssistanceBase::Initialize(void)
{
   trace(L6, _T("+SMWaitForRemoteAssistanceBase::Initialize() - MachineName: %s, RemoteContext: %s"), m_sMachineName, m_sRemoteContext );
   if (dm.fStateTakeawayBelt)	// do not run belt in store mode  LPM091802 215283
   {
	csItemSent = EMPTY_STRING;       // forget last item code we TRIED to sell 
   }
   TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("WaitForRemoteAssistance") );

   SMStateBase *pNextState = STATE_RETURN; 
   nTimeOut = co.getTimeOutHelp();   // ring alarm at timeout
   pNextState = STATE(OperatorPasswordState)::Initialize();
   DisplayAuxScreen(AUX_SCREEN_USE_TOUCHSCREEN, true);	// TAR 228153

   if( pNextState == STATE_NULL )
   {
		pNextState = STATE_RETURN;
		
		ps.SetServerName( m_sMachineName );

        ra.AcceptConnection( m_sMachineName, m_sRemoteContext );

	    // Rel 3.2 Requirement 4.5.1 - Single Mode RAP
        long lConnMode = ra.GetConnectionMode();
		
		if( !dm.SoundConnectToServer( NCRRC_REMOTEFOCUS, NCRRC_BOTHDISPLAY ) )
        {
			trace( L1, _T("SMWaitForRemoteAssistanceBase::Initialize() dm.SoundConnectToServer() failed.") );
        }

		// Check to see the remoteconnection is handheld or not
        CString csRemoteConnectionName = ra.GetRemoteConnectionName();
		bool m_bIsHandHeld = ra.IsRemoteConnectionHandheld(csRemoteConnectionName);

		switch( lConnMode )
		{
			case RA_CONNECTION_MODE_ASSIST:
                m_pAppPSX->GenerateEvent( _T(""), _T(""), _T("RemoteData"), _T("operation=assist-acquire;"), csRemoteConnectionName );
				if (co.fStateAssistMenuFromRAPAllowed && (!bIsInFinalization || m_bIsHandHeld))
				{
					RETURNSTATE(SmAssistMenu);
				}
				else
				{
                    RETURNSTATE(SmAssistMode);
                }
			case RA_CONNECTION_MODE_REPORTING:
				RETURNSTATE(SmReportsMenu);
			case RA_CONNECTION_MODE_STORE:
		    	RETURNSTATE(SmAuthorization); 
            case RA_CONNECTION_MODE_DATA_NEEDED:
                RETURNSTATE(SmDataEntry);
			default :
                
				
				ASSERT(FALSE);
		}
    }
	else
	{
		trace(L0,_T("SMWaitForRemoteAssistanceBase::Initialize() Base state Initialize() did not return STATE_NULL.  Exiting state."));	
	}

	ps.Button(_T("RemoteButton1"), PS_NOTEXT, false);	// Tar 213430

	trace(L6,_T("-SMWaitForRemoteAssistanceBase::Initialize()"));
	return pNextState;
}

SMStateBase *SMWaitForRemoteAssistanceBase::PSConnectRemote( CString&, CString& )
{
	ASSERT( FALSE );
	return STATE_NULL;
}

/////////////////////////////////////////////////////////
void SMWaitForRemoteAssistanceBase::UnInitialize()
{

	SMOperatorPasswordStateBase::UnInitialize();
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("WaitForRemoteAssistance") );
}

/////////////////////////////////////////////////////////
void SMWaitForRemoteAssistanceBase::showScreen(bool passwordVerified)
{
  
  saMsgFuncPtr = PSProceduresBase::SecurityMessageModal;
  int nNumApprovals = 0;

  // This state does not need to reshow if the password
  // was verified, since we'll be leaving now
  if (passwordVerified)
  { 
	trace(L6,_T("+/-SMWaitForRemoteAssistanceBase::showScreen(passwordVerified = true)"));
    return;
  }
  trace(L6, _T("+SMWaitForRemoteAssistanceBase::showScreen(passwordVerified = false)"));
  saMsgFuncPtr = PSProceduresBase::SecurityMessageModal;

  CString csWork;


	if (inputMode == PROMPTING_FOR_OPERATOR)
	{
        saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;
		
		STATE(OperatorPasswordState)::ShowEnterID();  //SR93 Biometrics support		
		ps.SetCMLeadthruText( TXT_ENTER_ID );
		ps.AllowInput( MAX_DIGITS_OPERATOR, false, false );
		ps.ShowCMFrame( false );   

		trace(L6, _T("-SMWaitForRemoteAssistanceBase::showScreen() - returned after prompting for Operator."));
				
		return;	}
	else
	{  // Display Wait for Approval.... 
        // NewUI
		ps.SetFrame(_T("WaitForRemoteAssistance"));

		// Begin TAR 208331 play the wave file when using remote assist mode
		DMSayPhrase(WAITFORASSISTANCE);
		// End TAR 208331

		// Begin TAR 408888
		ps.SetCMLeadthruText( LTT_ASSISTANCEINPROGRESS );
		// END TAR 408888
	}
 
  m_pAppPSX->ClearReceipt(_T("CMSmallReceipt")); //*** NewUI ***//
  csCurContext = _T("WaitForRemoteAssistance");  // Tar 238987
  COleVariant v;
  v = VARIANT_FALSE;

  if (co.fOperationsAllowAMDoneBtn) 	// RFC 209075 RJF 7/31/02
  {
	  v = VARIANT_TRUE;
	  ps.Button(_T("RemoteButton1"), BTT_DONE, true);	// Tar 213430
  }
  else
	  ps.Button(_T("RemoteButton1"), PS_NOTEXT, false);	// Tar 213430
  m_pAppPSX->SetConfigProperty(_T("RemoteButton1"), csCurContext, UI::PROPERTYVISIBLE, v);

  ps.CMButton(_T("ButtonHelp"), BTT_HELP, false);  //Tar 143857 Disable the help button.
  // End NewUI

  csWork = ps.GetPSText(MSG_ASSISTANCEPLEASEWAIT);   //RFC 282281
//  csWork += ps.GetPSText(MSG_REMOTE_ASSISTANCE);   //RFC 282281
  ps.Message12(csWork, true);
  ps.ShowCMFrame();
  trace(L6, _T("-SMWaitForRemoteAssistanceBase::showScreen()"));
}
 
//////////////////////////////////////////////////////
SMStateBase * SMWaitForRemoteAssistanceBase::stateAfterPasswordVerify()
{ 	 
  RETURNSTATE(SmAuthorization)
}

//////////////////////////////////////////////////////////////
SMStateBase *SMWaitForRemoteAssistanceBase::OnWtIncreaseNotAllowed() //tar 197857
{
	return VoidingWtIncrease(BES_WAITFORREMOTEASSIST, BES_WAITFORREMOTEASSIST);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMWaitForRemoteAssistanceBase::OnWtDecrease() //tar 197857
{
	return ThresholdViolWtDecrease(BES_WAITFORREMOTEASSIST, BES_WAITFORREMOTEASSIST);
}
