//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmConfirmSignatureBase.cpp
//
// TITLE: Class implementation for check signature state
//
// TAR#127527	122299 LPM this is a store mode screen.  Had to add the SaveSigReceiptData method.
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMSmConfirmSignature.h" 
#include "SMTakeCard.h"             // Take Card state
#include "smRequestSig.h"
#include "SigReceiptDataFile.h"
#include "smtakeReceipt.h"


#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SmConfirmSignatureBase")

IMPLEMENT_DYNCREATE(SMSmConfirmSignatureBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(ConfirmSignature)

DEFINE_TIMESTAMP

////////////////////////////////////////
SMSmConfirmSignatureBase::SMSmConfirmSignatureBase()
: csCurContext(EMPTY_STRING)
{
	IMPLEMENT_TIMESTAMP
}

/////////////////////////////////////////////
SMStateBase  *SMSmConfirmSignatureBase::Initialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmConfirmSignature") );	// RFC 330050
   // CANNOT enter Remote Assist Mode at this state
   //ra.OnSetTransactionState(RA_STATE_ASSISTNOTOK);
   ra.RequestEnableAssistMode(false);   //TAR 213249
   
   //Update RAP window with SCOT state
   UpdateStateName(ps.GetPSText(LTT_SIGNCONFIRM, SCOT_LANGUAGE_PRIMARY));
   ps.SetLanguage(SCOT_LANGUAGE_PRIMARY);
   
   csCurContext = _T("SmConfirmSignature");     // Tar 238987
   ps.SetFrame(csCurContext); //*** NewUI ***//
   if (co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].bCaptureSignature)
   {
      ps.Button(_T("SMButton1"), BTT_OK, true);
      ps.Button(_T("SMButton2"), BTT_SIGNAGAIN, true);
      COleVariant vValue = VARIANT_TRUE;
      m_pAppPSX->SetConfigProperty(_T("CMSigCap"), csCurContext, UI::PROPERTYVISIBLE, vValue);
      ps.SetLeadthruText(MSG_SIGSIGNCONFIRM, PS_NOTEXT);
   }
   else
   {
      ps.Button(_T("SMButton1"), BTT_DONE, true);
      ps.SetButtonText(_T("SMButton2"), PS_NOTEXT);
      COleVariant vValue = VARIANT_FALSE;
      m_pAppPSX->SetConfigProperty(_T("CMSigCap"), csCurContext, UI::PROPERTYVISIBLE, vValue);
      ps.SetLeadthruText(MSG_SIGNCONFIRM, PS_NOTEXT);
   }   
   ps.SetTitle(LTT_SIGNCONFIRM);
   ps.Weight(lDMScaleWeight);
   ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);

   
   if (fDMReceiptError)
   {
      DMReceiptReprint();
   }
   if (fDMJournalError)
   {
      DMJournalReprint();
   }
   ps.ShowFrame(false);
   return STATE_NULL;                // stay here
}

//////////////////////////////////////////    
SMStateBase  *SMSmConfirmSignatureBase::PSButton1(void)  // Signature is OK.
{
/*	if (co.m_bOperationsSignatureCapture || !co.m_bOperationsVerifySignature)
    {
	 CREATE_AND_DISPENSE(TakeCard)(true) ;
    }
  */   
	   //Send signature data to TB
	  if(co.fOperationsSendSignatureDataToTB)
      {
	      BSTR pData= NULL;

	      gpSCOTSigCap->Get5991Format(&pData);  // Get 5991 format for either RAP or S-Receipt file
    	  TBSendSigData(CustomerName(cDMLastTrack),(LPCTSTR) pData,(DWORD)(SysStringByteLen(pData)));

          SysFreeString(pData); // Since SMRequestSigBase::DMSigcapEnded frees the BSTR after calling TBSendSigData
                                // we need to do the same here.
      }

		trace(L7, _T("+ra.OnNormalItem, Message=%s"), ps.GetPSText(RA_TENDER_COMPLETE, SCOT_LANGUAGE_PRIMARY));
		ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + 
        ps.GetPSText(RA_TENDER_COMPLETE, SCOT_LANGUAGE_PRIMARY));
		trace(L7, _T("-ra.OnNormalItem"));
		
                EndOfTransactionPrint(); //SR697
	
  	if (co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].bCaptureSignature)
  	{
		  g_bgVerifySig = false;
		  trace(L7, _T("+ra.OnSigCapApproval"));
		  ra.OnSigCapApproval();   // Tell RAP it has been approved. //
		  trace(L7, _T("-ra.OnSigCapApproval"));
		  if (co.m_bOperationsPrintSignatureReceipt)
		  {
			  BSTR pData= NULL;
	      gpSCOTSigCap->Get5991Format(&pData);  // Get 5991 format for either RAP or S-Receipt file
			  SaveSigReceiptData(pData) ;
              SysFreeString(pData); // Since SMRequestSigBase::DMSigcapEnded frees the BSTR after calling SaveSigReceiptData
                              // we need to do the same here.
		  }
	    CREATE_AND_DISPENSE(TakeCard)(true) ; //TAR#132072 -- Proceed SigCap after tendering
	  }

    CREATE_AND_DISPENSE(TakeCard)(true) ;			  // from the StoryBoard end of transaction
}

//////////////////////////////////////////
SMStateBase  *SMSmConfirmSignatureBase::PSButton2(void) // no, sign again
{
	g_bgVerifySig = false;
	trace(L7, _T("+ra.OnSigCapApproval"));
	ra.OnSigCapApproval();  // Tell RAP it's over, redo will be automatic.
	trace(L7, _T("-ra.OnSigCapApproval"));
	RETURNSTATE(RequestSig)
}

//////////////////////////////////////////
SMStateBase *SMSmConfirmSignatureBase::TimedOut(void) // timeout
{
	return SMStateBase::TimedOut() ;
}

//////////////////////////////////////////
SMStateBase  *SMSmConfirmSignatureBase::PSButtonHelp(void)
{
	return SMStateBase::PSButtonHelp();
}


void SMSmConfirmSignatureBase::UnInitialize()
{
	// CAN enter Remote Assist Mode after leaving this state
	//ra.OnSetTransactionState(RA_STATE_ASSISTOK);
    ra.RequestEnableAssistMode(true);   //TAR213249


    ps.SetLanguage(m_customerLanguage);
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmConfirmSignature") );	// RFC 330050
}
///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSmConfirmSignatureBase::OnWtIncreaseNotAllowed()	 
{
	return FinalizationWtIncrease(BES_CONFIRMSIGNATURE);	

}


///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSmConfirmSignatureBase::OnWtDecrease()
{
	if(!g_bIsForgiveUnexpectedDecreaseDuringTendering && !(lBalanceDue < 0.01))
		return FinalizationWtDecreaseNotAllowed(BES_CONFIRMSIGNATURE);
	else
		return STATE_NULL;
}
