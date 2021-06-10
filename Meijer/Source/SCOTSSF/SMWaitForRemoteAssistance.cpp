//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMWaitForRemoteAssistance.CPP
//
// TITLE: Class implementation for RAP Data Needed state
//
//
// AUTHOR:    Erick Kobres
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMWaitForRemoteAssistance.h"      // MGV this state

#include "transbrokerssf.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("WaitForRemoteAssistance")

IMPLEMENT_DYNCREATE(SMWaitForRemoteAssistance, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////////////
SMWaitForRemoteAssistance::SMWaitForRemoteAssistance()
{
  IMPLEMENT_TIMESTAMP
}

SMWaitForRemoteAssistance::SMWaitForRemoteAssistance( LPCTSTR sMachineName, LPCTSTR sRemoteContext ) :
	SMWaitForRemoteAssistanceBase( sMachineName, sRemoteContext )
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase  *SMWaitForRemoteAssistance::Deliver( LPCTSTR sMachineName, LPCTSTR sRemoteContext )
{
  SMWaitForRemoteAssistanceBase::Deliver( sMachineName, sRemoteContext );

  return this;
}

SMStateBase *SMWaitForRemoteAssistance::Initialize(void)
{
    return SMWaitForRemoteAssistanceBase::Initialize();
}

void SMWaitForRemoteAssistance::UnInitialize(void)
{
    SMWaitForRemoteAssistanceBase::UnInitialize();
}

SMStateBase *SMWaitForRemoteAssistance::TimedOut(void)
{
    return SMWaitForRemoteAssistanceBase::TimedOut();
}

SMStateBase *SMWaitForRemoteAssistance::OnWtIncreaseNotAllowed(void)
{
    return SMWaitForRemoteAssistanceBase::OnWtIncreaseNotAllowed();
}

SMStateBase *SMWaitForRemoteAssistance::OnWtDecrease(void)
{
    return SMWaitForRemoteAssistanceBase::OnWtDecrease();
}

bool SMWaitForRemoteAssistance::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMWaitForRemoteAssistanceBase::DeliverDescriptorList(dlParameterList);
}

bool SMWaitForRemoteAssistance::IsLoginBypassAllowed()
{
    return SMWaitForRemoteAssistanceBase::IsLoginBypassAllowed();
}

void SMWaitForRemoteAssistance::showScreen(bool passwordVerified)
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
		
        // Determine the leadthru text to show
        int iMessage = MSG_ENTERID_PRESSENTER;
        COleVariant v = VARIANT_TRUE;
        bool bEmplid = (SendTBCommand(TBCMD_IS_EMPLID_ENABLED) == TB_SUCCESS_IMMEDIATE);
        if( bEmplid )
        {
            bool bAllowHandKey = (SendTBCommand(TBCMD_IS_EMPLID_ALLOWHANDKEY) == TB_SUCCESS_IMMEDIATE);
            iMessage = bAllowHandKey ? MSG_ENTERID_SCANIDORKEY : MSG_ENTERID_SCANIDONLY;
            v = bAllowHandKey ? VARIANT_TRUE : VARIANT_FALSE;
//(+)POS108118
            if(!bAllowHandKey)
            {
                CString csNewPosition;
                csNewPosition = _T("400,262,520,65");
                m_pAppPSX->SetConfigProperty(_T("LeadthruText"), _T("EnterID"), UI::PROPERTYPOSITION, csNewPosition);

                csNewPosition = _T("265,396,510,250");
                m_pAppPSX->SetConfigProperty(_T("MessageBoxNoEcho"), _T("EnterID"), UI::PROPERTYPOSITION, csNewPosition);

                m_pAppPSX->SetConfigProperty(_T("LeadthruText"),  _T("EnterID"),UI::PROPERTYTEXTALIGNMENT, _T("0"));
                m_pAppPSX->SetConfigProperty(_T("MessageBoxNoEcho"),  _T("EnterID"),UI::PROPERTYTEXTALIGNMENT, _T("0"));
            }
//(-)POS108118
        }

        m_pAppPSX->SetConfigProperty(_T("NumericP1"), _T("EnterID"), UI::PROPERTYVISIBLE, v);
        m_pAppPSX->SetConfigProperty(_T("NumericP2"), _T("EnterID"), UI::PROPERTYVISIBLE, v);
        m_pAppPSX->SetConfigProperty(_T("NumericP3"), _T("EnterID"), UI::PROPERTYVISIBLE, v);
        m_pAppPSX->SetConfigProperty(_T("NumericP4"), _T("EnterID"), UI::PROPERTYVISIBLE, v);
//(+)POS108118
        m_pAppPSX->SetConfigProperty(_T("Echo"), _T("EnterID"), UI::PROPERTYVISIBLE, v);
        m_pAppPSX->SetConfigProperty(_T("EchoField"), _T("EnterID"), UI::PROPERTYVISIBLE, v);
//(-)POS108118
        
        ps.Message12(iMessage, false);
        // Leadthru text message set

		ps.AllowInput( co.GetnOperationsOperatorIDMaxDigit(), false, false );
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
  ps.Message12(csWork, true);
  ps.ShowCMFrame();
  trace(L6, _T("-SMWaitForRemoteAssistanceBase::showScreen()"));
}

SMStateBase *SMWaitForRemoteAssistance::stateAfterPasswordVerify()
{
    return SMWaitForRemoteAssistanceBase::stateAfterPasswordVerify();
}

SMStateBase *SMWaitForRemoteAssistance::PSConnectRemote( CString& strMachineName, CString& strRemoteContext )
{
    return SMWaitForRemoteAssistanceBase::PSConnectRemote(strMachineName, strRemoteContext );
}