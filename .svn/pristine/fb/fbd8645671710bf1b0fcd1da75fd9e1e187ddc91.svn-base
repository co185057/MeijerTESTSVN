//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMThankYou.cpp
//
// TITLE: Class impliementation for Thank You State
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMThankYou.h"             // MGV this state
#include "SMTakeReceipt.h"
#include "transbrokerssf.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("ThankYou")

IMPLEMENT_DYNCREATE(SMThankYou, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMThankYou::SMThankYou()
{
	IMPLEMENT_TIMESTAMP
}

#ifndef _TRADESHOWDEMO
SMStateBase * SMThankYou::Initialize(void)
{
/*
//This logic below will add the TLOG with Override ID=46 at the end of every transaction.
if( co.IsAttendantModeOn() )
{
SendTBCommand(TBCMD_TLOG_OVERRIDE,
co.GetAttendantID(),
DATANEEDED_STARTTRANSACTIONAM,
1,
0,
_ttol(rp.GetTerminalNumber()),
0);//SSCOI-47169 POS304531
}
	*/
	if( GetTBProperty(_T("ReleaseReceipt")) == _T("1") )
	{
		RETURNSTATE(TakeReceipt);
	}
	
	m_bIsNewTrxSelected = false;
	m_bIsSuspendFromRap = false;
	
	nPreviousState = BES_LASTSTATE; //SSCOI-45180;
	
	// start CRD 125377 don't display thank you if transaction canceled
	if 	(((m_bShopAndDropEnabled) && (m_bSNDmPerksCancel))	||
		((m_bShopAndDropEnabled) && (m_bSNDxCancelTrans)))
    {
		ps.SetSkipFrameDisplay(true);
	}
	// end CRD 125377
	SMStateBase  *pRetState = SMThankYouBase::Initialize();

	ps.ShowChangeDueAmountPaid(lChangeDue, lTotalPaid, false); //CRD181094
	
	CString strTBPropName = _T("IsPriceInquiryState");
	CString IsPriceInquiryState;
	SendTBCommand(TBCMD_GET_TBPROPERTY, &strTBPropName, &IsPriceInquiryState);
	
	//Meijer's no sale feature totally circumvent security.  
    //the no sale trxn starts outside of security and security remains
    //in the idle start unaware the a trxn is going on
    //when the no sale completes the tB return tb_close
    //the FL hangs in the thankyou state be cause security will
    //not return Transaction Complete because on has not started.
    CString csNoSaleInProgress = GetTBProperty(_T("NoSaleInProgress"));
    if(csNoSaleInProgress == _T("1"))
    {
        ps.Message12(SSF_MSG_THANKYOUFORSHOPPINGMEIJER, true);
		bTransactionComplete = true;
		nTimeOut = 1;
        SetTBProperty(_T("NoSaleInProgress"), _T("0"));
    }
	if(IsPriceInquiryState == _T("1"))
	{
		strTBPropName = _T("IsPriceInquiryState");
		IsPriceInquiryState = _T("0");
		SendTBCommand(TBCMD_SET_TBPROPERTY, &strTBPropName, &IsPriceInquiryState);
	}
	
    //POS53022 - Display the custom ThankYouCancelTrx context
    if (fInVoidTrans || (lVoidedItems > 0 && lItems <= 0))
    {  	 
        ps.SetCMFrame(_T("ThankYouCancelTrx"));
        ps.Message12(MSG_SETALLASIDE,true);  
        ps.ShowCMFrame(false);
        
    }
    //e - POS53022
	
	// Cancel press on mPerks and need to just go back to Attract without waiting for ThankYouTimerNoBagScale timeout.
	trace(L6, _T("m_bShopAndDropEnabled = %d m_bSNDmPerksCancel = %d m_bSNDxCancelTrans = %d bTransactionComplete = %d m_bPrintReceiptExpire = %d"),m_bShopAndDropEnabled, m_bSNDmPerksCancel, m_bSNDxCancelTrans, bTransactionComplete, m_bPrintReceiptExpire);
	if (((m_bShopAndDropEnabled) && (m_bSNDmPerksCancel)) ||
		((m_bShopAndDropEnabled) && (m_bSNDxCancelTrans)))
	{
		ps.SetSkipFrameDisplay(false);
		
		fSAInTransaction = false;
		ClearStateStackEOT();
		
		trace(L7, _T("+ra.OnTransactionEnd"));
		ra.OnTransactionEnd();
		trace(L7, _T("-ra.OnTransactionEnd"));  
		
		nTimeAttractIdleMsec = 0; 
		
		ms_bThankYouTimerNoBagScaleTimeOutFired = false;
        setAnchorState(BES_START);
        return createAnchorState();
	}
	return pRetState;
    //return SMThankYouBase::Initialize();
}
#else
#include "..\TradeShowDemo\SMThankYouTSDemo.cpp"
#endif //_TRADESHOWDEMO

void SMThankYou::UnInitialize(void)
{
	SMState::nVoidCounts = 0;  //VSTS 131568
	SMState::m_bCustomVoidItemMessage = false; //VSTS 131568
	ps.SetSkipFrameDisplay(false);
    SMThankYouBase::UnInitialize();
}

SMStateBase * SMThankYou::TimedOut(void)
{
	
	
	if (bTransactionComplete)
	{			
		CString strGiftPropName = _T("PrintGiftReceipt");
		CString strPrintGiftReceipt = _T("0");
		SendTBCommand(TBCMD_SET_HOOKPROPERTY, &strGiftPropName, &strPrintGiftReceipt);
		
		CString strRebatePropName = _T("RebateReceiptRequested");
		CString strPrintRebateReceipt = _T("0");
		SendTBCommand(TBCMD_SET_HOOKPROPERTY, &strRebatePropName, &strPrintRebateReceipt);
        
		CString strTBPropValue = _T("0");
		CString strTBPropName = _T("IsFromSeparateMediaState");
		SendTBCommand(TBCMD_SET_TBPROPERTY, &strTBPropName, &strTBPropValue);
		
		//POS190541
		//CString csPOSState;
		//csPOSState = GetHookProperty(_T("__POSState"));
		CString csPOSState = GetTBProperty(_T("SavedLastPosState"));
		trace(L6, _T("Current POSState at ThankYou(%s), Timeout(%d)"), csPOSState,nTimeOut);
		
		//POS207384/214026 - Detect the Transaction Menu also
		if( csPOSState.Find( _T("ITEMMENU") ) == -1 && csPOSState.Find(_T("TRXMENU_DK1")) == -1 &&
			/*CRD185924 */csPOSState.Find(_T("SOFTWARE_AVAILABLE")) == -1 )
		{
			SendTBCommand(TBCMD_CHECK_FORCE_LOAD, NULL); //SSCOI-43034 / VSTS 172859 move here to ensure that security is finished before doing a store close request
			
			// wait for 1 sec before displaying the please wait
			Sleep(1000);
			ps.Message12(TXT_STOREAPPNEEDED,true);  
			ps.ShowCMFrame(false);  
			
			return STATE_NULL;
		}
		
		ps.Message12(SSF_MSG_THANKYOUFORSHOPPINGMEIJER, true);
		//e - POS190541
	}
	// start CRD 125377
	// Cancel press on mPerks and need to just go back to Attract without waiting for ThankYouTimerNoBagScale timeout.
	trace(L6, _T("m_bShopAndDropEnabled = %d m_bSNDmPerksCancel = %d m_bSNDxCancelTrans = %d bTransactionComplete = %d m_bPrintReceiptExpire = %d"),m_bShopAndDropEnabled, m_bSNDmPerksCancel, m_bSNDxCancelTrans, bTransactionComplete, m_bPrintReceiptExpire);
	
	if (((m_bShopAndDropEnabled) && (m_bSNDmPerksCancel) && (bTransactionComplete) && (m_bPrintReceiptExpire)) ||
		((m_bShopAndDropEnabled) && (m_bSNDxCancelTrans)))
	{
		m_bNormalExit = true;
		if (!g_bInterventionApproval )
		{
			bTransactionComplete = false;
		}
		
		ms_bEscalationTimeOutFired = false;
		if(!fInSignedOn && !fInSignedOff)
		{
			fSAInTransaction = false;
			ClearStateStackEOT();
			if (g_bInterventionApproval)
			{   
				trace (L5, _T("Stay at ThankYou until WMRA_SEND_INTERVENTION_APPROVED received"));
				return STATE_NULL;
			}
			else
			{
				trace(L7, _T("+ra.OnTransactionEnd"));
				ra.OnTransactionEnd();
				trace(L7, _T("-ra.OnTransactionEnd"));  
				
				nTimeAttractIdleMsec = 0; 
				
				setAnchorState(BES_START);
				return createAnchorState();
			}
		}
	}
	//  end CRD 125377
	//	 SMStateBase *returnState = SMThankYouBase::TimedOut();
	

	// +CRD127058 ThankYouBase::TimedOut
	if (ms_bThankYouTimerNoBagScaleTimeOutFired)
	{
		ms_bThankYouTimerNoBagScaleTimeOutFired = false;
		setAnchorState(BES_START);
		return createAnchorState();
	}
	
	if (bTransactionComplete)
	{
		if ( m_bPrintReceiptExpire ) //SR832
		{
#ifndef _CPPUNIT        
			m_bNormalExit = true;
			//TAR 379317+
			if (!g_bInterventionApproval )
			{
				bTransactionComplete = false;
			}
			//TAR 379317+
			sayThankYou();
			
			if (dm.IsCashDrawerAvailable())	// Cashdrawer/ExitGate is available check. awk.1
			{								// if it is available wait until security give go ahead then open.
				dm.CashDrawerOpen();	//Cashdrawer/ExitGate open
			}
			
			ms_bEscalationTimeOutFired = false;
#endif
			if(!fInSignedOn && !fInSignedOff)
			{
#ifndef _CPPUNIT
				fSAInTransaction = false;
				ClearStateStackEOT();
				//TAR 371932 & tar 371918+
#endif
				if (g_bInterventionApproval)
				{   
					trace (L5, _T("Stay at ThankYou until WMRA_SEND_INTERVENTION_APPROVED received"));
					return STATE_NULL;
				}
				else
				{
#ifndef _CPPUNIT
					//TAR390688+
					trace(L7, _T("+ra.OnTransactionEnd"));
					ra.OnTransactionEnd();
					trace(L7, _T("-ra.OnTransactionEnd"));  
					//TAR390688-
					
					nTimeAttractIdleMsec = 0; //sscop-5591
#endif
					
					if(co.IsBagScaleDisabled() == 1 && co.getTimeOutThankYouTimerNoBagScale() > 0 
						&& !ms_bThankYouTimerNoBagScaleTimeOutFired)
					{
						ms_bThankYouTimerNoBagScaleTimeOutFired = true;
						nTimeOut = co.getTimeOutThankYouTimerNoBagScale();
						return STATE_NULL;
					}
					setAnchorState(BES_START);
					return createAnchorState();
					
				}
				//TAR 371932 & tar 371918-
				
			}
#ifndef _CPPUNIT
			else
				return TBFinish();
#endif
		}
		else
		{
			return STATE_NULL;	
		}
	}
#ifndef _CPPUNIT
	else	  //else give reminder messages to remove items from smartscale
	{
		//+RFC 276161	
	

		if ( fInVoidTrans || fInSuspendTrans)
		{
			//POS53022 - Display the custom ThankYouCancelTrx context
			if (fInVoidTrans || (lVoidedItems > 0 && lItems <= 0))
			{  
				ps.SetCMFrame(_T("ThankYouCancelTrx"));
				ps.Message12(MSG_SETALLASIDE,true);  
				ps.ShowCMFrame(false);        
			}
			//e POS53022 - Display the custom ThankYouCancelTrx context
			else
			{
				ps.Message12( MSG_TRANSUSPENDED,true );
				DMSayPhrase( SUSPENDTRANS_SETALLITEMSASIDE );
			}

		}
		else
			//-RFC 276161	
		{
			ps.Message12(MSG_PLEASE_TAKE_YOUR_ITEMS, true);

			trace(L6, _T("SMThankYou::Initialize m_csTransactionMode: %s"), m_csTransactionMode);
			if(m_csTransactionMode == TRANSMODE_SND)	//127058 if Transaction Mode is SnD
			{
				DMSaySecurity( SEC_NONE, ps.GetSoundName(SEC_INCORRECTITEMONBELT));	//112 SNDThankYou
			}
			else		
			{
				DMSaySecurity( SEC_NONE, ps.GetSoundName(SEC_PLSTAKEYOURITEMS));
			}
		}
		
		// Even if we already played the Thank You audio once, we'll need it
		// again after we get back out of this condition.
		fThankYouAudioPlayed = false;
		nTimeOut = co.getTimeOutRecurringTakeYourItemAudio();
		return STATE_NULL;
	}
	// -CRD127058
	// -ThankYouBase::TimedOut
	
	//POS53022 - Display the custom ThankYouCancelTrx context
	//if (fInVoidTrans || (lVoidedItems > 0 && lItems <= 0))
	//{  
	//	ps.SetCMFrame(_T("ThankYouCancelTrx"));
	//	ps.Message12(MSG_SETALLASIDE,true);  
	//	ps.ShowCMFrame(false);        
	//}
	//e POS53022 - Display the custom ThankYouCancelTrx context
	
/*	return returnState;*/

    //return SMThankYouBase::TimedOut();
}
#endif

SMStateBase * SMThankYou::OnWtDecrease(void)
{
    return SMThankYouBase::OnWtDecrease();
}

SMStateBase * SMThankYou::OnWtIncreaseNotAllowed(void)
{
    return SMThankYouBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMThankYou::OnBackToLGW(void)
{
    return SMThankYouBase::OnBackToLGW();
}

SMStateBase * SMThankYou::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
    return SMThankYouBase::OnRAPClearSecurityBtn( lpszAuthorizationType);
}

SMStateBase * SMThankYou::OnTransactionComplete(void)
{
    return SMThankYouBase::OnTransactionComplete();
}

SMStateBase * SMThankYou::RAParse(MessageElement *me)
{
    return SMThankYouBase::RAParse(me);
}

void SMThankYou::sayThankYou()
{
	if (!fThankYouAudioPlayed && m_csTransactionMode == TRANSMODE_SND)
    {
		DMSaySecurity( SEC_NONE, ps.GetSoundName(SEC_INCORRECTITEMONBELT));	//112 SNDThankYou
	}
	else
	{
		SMThankYouBase::sayThankYou();
	}
}

void SMThankYou::Escalation2RAP(void)
{
    SMThankYouBase::Escalation2RAP();
}

SMStateBase * SMThankYou::PSButton1(void)
{
    return SMThankYouBase::PSButton1();
}

SMStateBase *SMThankYou::OnRAPUnsolicitedDataReceived(LPCTSTR sData)
{
	CString csData = sData;
	trace (L6, _T("+SMThankYou::OnRAPUnsolicitedDataReceived  ."));
	// Don't accept RAP Initiate Transaction requests from this state
	if (csData.Find(_T("RAPInitiateTransaction")) != -1)
	{
		trace (L6, _T("+SMThankYou::OnRAPUnsolicitedDataReceived IGNORED RAPInitiateTransaction"));
		return STATE_NULL;
	}
	trace (L6, _T("+SMThankYou::OnRAPUnsolicitedDataReceived ."));
	return SMThankYouBase::OnRAPUnsolicitedDataReceived(sData);
}

