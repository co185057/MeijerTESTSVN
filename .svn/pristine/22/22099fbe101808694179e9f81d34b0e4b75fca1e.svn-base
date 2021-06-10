//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMWaitForApproval.CPP
//
// TITLE: Class implementation for wait for approval state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////
// POS329462 Work Request:79417 Name: Alexander Bronola  Date:August 24, 2017
// POS327323 Work Request:79417 Name: Jangala Sunny Date:August 11, 2017

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMWaitForApproval.h"      // MGV this state

#include "transbrokerssf.h"
#include "SMSmEnterBirthdate.h"
#include "SMSmAssistMode.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("WaitForApproval")

IMPLEMENT_DYNCREATE(SMWaitForApproval, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////////////////////////
SMWaitForApproval::SMWaitForApproval(const bool fTV,
                                     const bool fVT, const bool bVerifySig)
		: SMWaitForApprovalBase(fTV, fVT, bVerifySig)
{
  IMPLEMENT_TIMESTAMP
		
}

////////////////////////////////////////
SMWaitForApproval::SMWaitForApproval()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMWaitForApproval::Deliver(const bool fTV,
                                     const bool fVT, const bool bVerifySig)
{
  SMWaitForApprovalBase::Deliver(fTV, fVT, bVerifySig);

  return this;
}

SMStateBase *SMWaitForApproval::Initialize(void)
{
	SendTBCommand(TBCMD_NOTIFY_STOREMODE_PROMPT);	//CRD 338206
    if(GetHookProperty(_T("IsDelayedAgeRestrictedProp")) == _T("1"))
    {
       CString csPOSState = GetTBProperty(_T("SavedLastPosState"));
       trace(L6, _T("Initialize(): Posstate=<%s>"), csPOSState);
       
       if ( csPOSState.Find(_T("AGE_VERIFY")) != -1)
       {
           //set these flags back in order for the base to prompt id/pwd and tell the rap about the intervention.
          lUnapprovedRestrictedItems =1;
          fApprovedForRestrictedItems = false;
		  //(+) POS327323
		  //As per base it takes 2000ms, to disable the cash acceptor. 
		  //If suppose, by any chance cash is inserted during the state transition from ScanAndBag to WaitForApproval
		  //DMCashAcceptorEnable(co.csTenderDenominations) -- This method will accept cash.
		  trace(L6, _T("As cash is inserted during the state tarnsition from ScanAndBag to WaitForApproval we are enabling the cash acceptor."));
		  DMCashAcceptorEnable(co.csTenderDenominations); //Accepts the cash at in WaitForApproval screen
		  //(-) POS327323
       }
       //Start SSCOI-72966 / CRD 407201 Do not reset restricted item approval flags until approved by POS
	   //else if (isRestrictedItemApprovalRequired()) //in case it goes here but not triggered by the POS, usually when 2nd timeout occured.
       //{
       //   lUnapprovedRestrictedItems =0;
       //    fApprovedForRestrictedItems = true;
       //} 
	//End SSCOI-72966
    }

	// (+) VSTS 131568
	SMStateBase  *pRetState = SMWaitForApprovalBase::Initialize();
	
	SetHookProperty(_T("IsInStoreMode"),_T("1"));	//CRD 232267
	
	trace(L6,_T(" SMState::bVoidApprovalDelayed (%s)"), SMState::bVoidApprovalDelayed ? _T("TRUE") : _T("FALSE"));
	if(SMState::bVoidApprovalDelayed)
	{
		trace(L6,_T(" Entered SMState::bVoidApprovalDelayed "));
		CString csWork;
		csWork =  ps.GetPSText(TXT_VOIDEDITEMAPPROVAL);

		ps.Message12(csWork, true);
	}

	//Code changes for recording customer wait time for store assistance - Begin
    SMState::StoreAssistRequestedTime = CTime::GetCurrentTime();

	return pRetState;
   //return SMWaitForApprovalBase::Initialize();
   //(-) VSTS 131568
}

void SMWaitForApproval::UnInitialize(void)
{
	//If cash acceptor is enabled in some condition(...enabled in AGE_VERIFY), we need to disable
	DMCashAcceptorDisable();
    SMWaitForApprovalBase::UnInitialize();
}

SMStateBase *SMWaitForApproval::TimedOut(void)
{
    return SMWaitForApprovalBase::TimedOut();
}

SMStateBase *SMWaitForApproval::OnWtIncreaseNotAllowed(void)
{
    return SMWaitForApprovalBase::OnWtIncreaseNotAllowed();
}

SMStateBase *SMWaitForApproval::OnWtDecrease(void)
{
    return SMWaitForApprovalBase::OnWtDecrease();
}

//Task 251563 - by aab
bool SMWaitForApproval::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
	// +SSCOI-46472
	CString csPOSState = GetTBProperty(_T("SavedLastPosState"));
    trace(L6, _T("DeliverDescriptorList(): Posstate=<%s>"), csPOSState);
    
    //if ( (csPOSState.Find(_T("AGE_VERIFY")) != -1) && GetHookProperty(_T("IsDelayedAgeRestrictedProp")) == _T("1"))
	if( GetTBProperty(_T("IsTriggeredByTB")) == _T("1") ) //POS288667
	{
		SetTBProperty(_T("IsTriggeredByTB"), _T("0"));

        trace(L6, _T("Handling TB request like Age Verify..."));
        Deliver(false, false);

        return true;    
    } // -SSCOI-46472
    else
        return SMWaitForApprovalBase::DeliverDescriptorList(dlParameterList);
}

SMStateBase *SMWaitForApproval::OnCouponSensor(void)
{
    return SMWaitForApprovalBase::OnCouponSensor();
}

SMStateBase *SMWaitForApproval::PSButtonHelp(void)
{
    return SMWaitForApprovalBase::PSButtonHelp();
}

SMStateBase *SMWaitForApproval::PSCancelKey(void)
{
    return SMWaitForApprovalBase::PSCancelKey();
}

SMStateBase *SMWaitForApproval::PSButton1()
{
    return SMWaitForApprovalBase::PSButton1();
}

SMStateBase *SMWaitForApproval::OnBackToLGW(void)
{
    //SOTF 7004
    bool bNeedApprovalState = false;
	
    if ((!isAnyApprovalRequired()) && 
        (g_bgVoidTrx))
	{
        bNeedApprovalState = true;
		
    }    
	
    SMStateBase *pRet = SMWaitForApprovalBase::OnBackToLGW();
	
    if(bNeedApprovalState)
    {
        
        delete getRealStatePointer( pRet); 
        return STATE_NULL;
    }
	
    return pRet;
    //sotf 7004
}

void SMWaitForApproval::showScreen(bool passwordVerified)
{
    SMWaitForApprovalBase::showScreen(passwordVerified);
    
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
    }

	// (+) VSTS 131568
	trace(L6,_T(" SMState::bVoidApprovalDelayed  (%s)"), SMState::bVoidApprovalDelayed ? _T("TRUE") : _T("FALSE"));
	if(SMState::bVoidApprovalDelayed)
	{
		trace(L6,_T(" Entered SMState::bVoidApprovalDelayed "));
		//ps.LeadThruText(TXT_VOIDEDITEMAPPROVAL);
	    int iMessage = TXT_VOIDEDITEMAPPROVAL;
		ps.Message12(iMessage, false);

	}

   ps.ShowCMFrame( false );

   // (-) VSTS 131568
	

     
   ps.ClearInput();
}

SMStateBase *SMWaitForApproval::stateAfterPasswordVerify()
{
    //TAR306722, preventing from going into SMSmAuthorization that would cause
	//to neglect the added bagging scale weight  
   
   if (isRestrictedItemApprovalRequired())
   {
       if(GetHookProperty(_T("IsDelayedAgeRestrictedProp")) == _T("1"))
       {
         //Start SSCOI-72966 / CRD 407201 Do not reset restricted item approval flags until approved by POS
		   //lUnapprovedRestrictedItems =0;
           //fApprovedForRestrictedItems = true;
		   //End SSCOI-72966 / CRD 407201
       	
          RETURNSTATE(SmAssistMode)
       }
       else
          RETURNSTATE(SmEnterBirthdate)	//single Age Restricted Approval screen (Meijer DG30)

   }	
   return SMWaitForApprovalBase::stateAfterPasswordVerify();
}

SMStateBase *SMWaitForApproval::OnUnDeActivatedTheftTag(void)
{
    return SMWaitForApprovalBase::OnUnDeActivatedTheftTag();
}
