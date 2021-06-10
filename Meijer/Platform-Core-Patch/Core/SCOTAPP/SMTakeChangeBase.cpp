//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTakeChangeBase.cpp
//
// TITLE: Class implementation for Take Change State
//
// This state represents a successful transaction paid for in cash with
// change due back.
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMTakeChange.h"
#include "SMTakeReceipt.h"
#include "SMSecUnExpectedIncrease.h"
#include "MPProcedures.h"
//#include "SMInSAProgress.h"         // this state
#include "SMState.h"                // Base state
#include "smsmassistmode.h"			//RFC275377 Migrated BM185018
#include "GloryOpos.h"				// Need CHAN_STATUS_JAM definition.


#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("TakeChangeBase")

DLLEXPORT extern HWND hw;           
DLLEXPORT extern MPProcedures mp;  
 


DEFINE_TIMESTAMP
IMPLEMENT_DYNCREATE(SMTakeChangeBase, CObject)// MFC Runtime class/object information

  
PSTEXTWAVE szwavFileToPlay;
////////////////////////////
SMTakeChangeBase::SMTakeChangeBase(const bool fEOT)
: fEndOfTrans(fEOT),	// Coin Acceptor Modifications
  fPaperMoneyReturned(false),
  fTimedOutOnce(false),
  fIs3xHardware(false), //merged TAR232798 to 3.1.1 - fixed by Chen
  m_bMotionEscalationFired(false),
  m_fBillsDispensed(false),  //TAR 391559
  saveChangeDue(0),
  csCurContext(EMPTY_STRING)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMTakeChangeBase::SMTakeChangeBase()
: csCurContext(EMPTY_STRING)
{
  fEndOfTrans = false;	// Coin Acceptor Modifications
  fPaperMoneyReturned = false;
  fTimedOutOnce = false;
  fIs3xHardware= false;  //merged TAR232798 to 3.1.1 - fixed by Chen
  m_fBillsDispensed = false;  //TAR 391559
  saveChangeDue = 0;
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMTakeChangeBase::Deliver(const bool fEOT)
{
  fEndOfTrans = fEOT;	// Coin Acceptor Modifications
  fPaperMoneyReturned = false;
  fTimedOutOnce = false;
  fIs3xHardware= false;   //merged TAR232798 to 3.1.1 - fixed by Chen
  saveChangeDue = 0;

  return this;
}

/////////////////////////////////////////
bool SMTakeChangeBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    
    // There should be one parameter, a bool passed as word
    if (dlParameterList.GetDescriptorCount() == 1)
    {
        trace(L6, _T("+SMTakeChangeBase::DeliverDescriptorList() - Parameter Count of 1"));
        Deliver((bool)DLParamWORD(dlParameterList));   // fEOT
        return true;
    }
    else
    {
        trace(L2, _T("+SMTakeChangeBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}

///////////////////////////////////////
SMStateBase  *SMTakeChangeBase::Initialize(void)
{

  BMPS		eBMP;
  PSTEXT	eLeadthru;
  PSTEXT    eMsg;
  PSTEXT	eEcho;
  long		drc = 0;				// Store result code of dispense.
  csCurContext = _T("TakeChange");  // Tar 238987

  m_bShowMediaStatusButton = false; //TAR 388019

  //+317732 
  if(SMStateBase::fCashOutSensorBlocked) //if true, we can say we've been here(TakeChange state) but must have been interrupted by some other state
  {
	  //Turn timer back ON, for cases when TakeChange was interrupted/exited with cash still pending removal
	  trace(L7, _T("SMTakeChangeBase::Initialize() Turning Escalate2RAP timer ON"));
      nTimeOut = co.getTimeOutCashOutSensorAudioTimer();
      nEscalationRAPTimeOut = co.getTimeOutDelaySecurityNotification();
      if(nEscalationRAPTimeOut != 0)
      { 
		  SetTimer(hw, RAP_ESCALATION, nEscalationRAPTimeOut, &EscalationTimedOut);
      }
      else if (nEscalationRAPTimeOut == 0) // if 0, immediate escalation 
      {
		  //we won't reach here. If it was immediate, it would have been handled 
		  //on first entry into TakeChange, so no need to implement here      
      }

  }
  //-317732
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("TakeChange") );	// RFC 330050
  //Tar 237024, lost receipt after cash dispenser error
  SMStateBase *sReturnState = CkStateStackAndReturn();
  if (sReturnState != STATE_NULL)
	  return sReturnState;

 bDoNotResetTimeOutOnDMEvent = true;  //Tar324574
 fIs3xHardware = Is3xHardware();   //merged TAR232798 to 3.1.1 - fixed by Chen

  // CANNOT enter Remote Assist Mode at this state
  //ra.OnSetTransactionState(RA_STATE_ASSISTNOTOK);
  ra.RequestEnableAssistMode(false);   //TAR 213249
  //Update RAP window with SCOT state
  UpdateStateName(ps.GetPSText(LTT_TAKECHANGE, SCOT_LANGUAGE_PRIMARY));

  CString csTemp = EMPTY_STRING;
  csTemp.Format(ps.GetPSText(MSG_RA_CHANGEDISPENSED, SCOT_LANGUAGE_PRIMARY), ps.FmtDollar(lChangeDue));
  trace(L7, _T("+ra.OnNormalItem, Message=%s"), csTemp);
  ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY+ csTemp);
  trace(L7, _T("-ra.OnNormalItem"));

  if (nTenderType>TB_TENDER_CASH)
  {
	  // Cash Back from EFT card
	  // Tar 204085 05/17/02 - use a different bitmap for take away belt 
	  if(dm.fStateTakeawayBelt)	// is on a take away belt system
		  eBMP = BMP_TAKECASH_BELT;
	  else
		  eBMP = BMP_TAKECASH;
	  //  mascot_bitmap = BMP_MASCOT_TAKECASH;
	  eLeadthru = LTT_TAKECHANGE;
	  eMsg = MSG_TAKECHANGE;
	  eEcho = TXT_REFUNDED;
  }
  else
  {
	  // All other cash tenders
	  // Tar 204085 05/17/02 - use a different bitmap for take away belt 
	  if(dm.fStateTakeawayBelt)	// is on a take away belt system
		  eBMP = BMP_TAKECHANGE_BELT;
	  else
		  eBMP = BMP_TAKECHANGE;
	  // mascot_bitmap = BMP_MASCOT_TAKECHANGE;
	  eLeadthru = LTT_TAKECHANGE;
	  eMsg = MSG_TAKECHANGE;
	  eEcho = TXT_CHANGEDUE;
  }
  
  //DMSayPhrase(TAKECHANGE); //merged TAR232798 to 3.1.1 - fixed by Chen

  ps.SetCMFrame(csCurContext);

    //To ensure store login button is not visable
  m_pAppPSX->SetConfigProperty(_T("ButtonStoreLogIn"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);

  ps.SetCMLeadthruText(eLeadthru);
    
  ps.Message12(eMsg, true);
  
  if (lBalanceDue>0)
    ps.ShowAmountRefundedAmountPaid(lChangeDue,lTotalPaid);
  else 
	ps.ShowChangeDueAmountPaid(lChangeDue,lTotalPaid);
  ps.ShowCMFrame();


  // Display this here because letting the MPProcedures::ProcessNextState()
  // do it causes it not to be displayed until AFTER we dispense the cash,
  // which is a long operation.

  DisplayAuxScreen(AUX_SCREEN_DEFAULT, true);


  if (lChangeDue==0)   //tar 272209
    return STATE_NULL; // TAR 05/13/02 TAR 204027: Avoid receiving coupon change back twice on same transaction

  //411233
  if ( co.IsDegradedModeOn() && dm.GetDegradedModeType() == DEGRADEDMODE_CARD_ONLY )
  {
	  lChangeDue = 0;
	  RETURNSTATE(TakeReceipt);  //414907
  }

  saveChangeDue = lChangeDue;
  bool bDispense = true;
  if (TrainingModeObj.IsTMOn() && !co.nOperationsAllowDispenseInTrainingMode) // in training mode and option set to not dispense
  {
    bDispense = false;
    fPaperMoneyReturned = false;
    nTimeOut = 3; // stay some time on take change even in training mode
    lChangeDue = 0;
    return STATE_NULL; // OK, wait for timeout
  }

  CString csCashCountsStatusCheck;
  long rc = DMDispenseChange(lChangeDue, csCashCountsStatusCheck, &drc);

  if (rc>0) 
    lChangeDue -= rc;
  
  m_fBillsDispensed = WereBillsDispensed();  //TAR 391559

  // Begin Dynamic Bill Detection
  //merged TAR232798 to 3.1.1 - fixed by Chen
  if ( !fIs3xHardware )        
  {
	  if (m_fBillsDispensed && !DMWasCashTaken()) //note: DMWasCashTaken() is only applied for 1.x hardware CHU 033103
	  { 
		fPaperMoneyReturned = true;
		nTimeOut = co.getTimeOutTakeCash();
	  }
	  else
	  {
        //For 2.x hardware, it always implements this part of code.
		// may want to wait less time if just coin change
		fPaperMoneyReturned = false;
		nTimeOut = co.getTimeOutTakeCoins();
	  }
	  szwavFileToPlay = TAKECHANGE; //TAR232798/205994 CHU moved from top to here. TD Replaced 6/20/03
      DMSayPhrase(szwavFileToPlay);
	  // End Dynamic Bill Detection
  }
  else
  {
	  if (!dm.m_csCashRecyclerModel.IsEmpty() ) // TAR 304956 - if recycler
	  {
          szwavFileToPlay = TAKECHANGE;
	  }
	  else
	  {
		  if (m_fBillsDispensed)  //TAR 391559
		  {
			  szwavFileToPlay = TAKECHANGEBELOWSCANNER;
		  }
		  else
		  {      
			  szwavFileToPlay = TAKECHANGE;
		  }
	  }

      DMSayPhrase(szwavFileToPlay);
      nTimeOut = co.getTimeOutTakeCoins();  //default is 3 seconds. Also fPaperMoneyReturned is not used for 3x.
  }
  //end of TAR232798

  // TAR 353244 - Initialize csWork.
  CString csWork = _T("DeviceErrorCashRecycler.xml:DEVICEERRORUNKNOWN");
  
  bool bEmpty = false;
  bool bChuteJam = false;
  IsHopperSubstitutionUsed(bEmpty, bChuteJam);

  if (lChangeDue==0)
  {
	if (co.fOperationsHopperSubstitutionIntervention)
	{
		  if (!fCancelPayment)
				nHopperTrxCount = nHopperTrxCount + 1;
	}

	// +TAR 388086
	if ( !m_fBillsDispensed )  //TAR 391559
	{
		// Performance enhancement.  If no bills were dispensed, then don't 
		// wait for timeout.
		trace(L6, _T("SMTakeChangeBase::Initialize() - No bills dispensed" ));
		return DMCashRemoved();	
	}
	// -TAR 388086

    return STATE_NULL; // OK, wait for timeout
  }

  if (bChuteJam)
	  return STATE_NULL;  //DM parse will take to system message of chute jam
	
  // Error condition of we didn't give all change due...
  // Need to distinguish coin or bill error.   TINA
  // csWork.Format(ps.GetPSText(MSG_CHANGEERROR),ps.FmtDollar(lChangeDue), csCashCountsStatusCheck);
  trace(L6, _T("cash changer error: %s"), csCashCountsStatusCheck);

  // TAR 353244 - This block only valid for cash recyclers.
  if( ! dm.m_csCashRecyclerModel.IsEmpty() ) 
  {
  //FZ Error Handlin
  //if glory  
		
		// Note:  CashRecyclerGetErrorCode does not utilized the input parameter
		//        if the cash recycler is a SafePay.
		long lBillErrorCode = dm.CashRecyclerGetErrorCode(0x80);
		long lCoinErrorCode = 0;
		 CString csTempErrorCode;
		 if(lBillErrorCode)
		  {
			 if(dm.m_csCashRecyclerModel == _CASHDISPENSER_GLORY)
			 {
				csTempErrorCode.Format(_T("%d"),lBillErrorCode);
				csTempErrorCode = csTempErrorCode.Left(2); // xml file only use first 2 digits
				csWork = GetDeviceErrorMnemonic(DMCLASS_CASHRECYCLER, _BILLRECYCLER, 0, _ttol(csTempErrorCode), nDeviceErrorContext );
			 }
			m_nCashRecyclerHardwareErrorCode = lBillErrorCode;

		  }
		  else 
		  {
			  lCoinErrorCode = dm.CashRecyclerGetErrorCode(0x01);
			  if(lCoinErrorCode)
			  {
				  if(dm.m_csCashRecyclerModel == _CASHDISPENSER_GLORY)
				  {					  
					  csTempErrorCode.Format(_T("%d"),lCoinErrorCode);
					  csTempErrorCode = csTempErrorCode.Left(2); // xml file only use first 2 digits
					  csWork = GetDeviceErrorMnemonic(DMCLASS_CASHRECYCLER, _COINRECYCLER, 0,  _ttol(csTempErrorCode), nDeviceErrorContext );
				  }
				m_nCashRecyclerHardwareErrorCode = lCoinErrorCode;
			  }
		  	  else
				  csWork = _T("DeviceErrorCashRecycler.xml:DEVICEERRORUNKNOWN");
		  }
	 
		 // Handle SafePay dispense error.
		 // The SO always seems to return 201 which is "UNKNOWN SAFEPAY ERROR".
		 // Not very useful information.
		 if(dm.m_csCashRecyclerModel == _CASHDISPENSER_SAFEPAY)		 
		 {
			 // Note drc is initialized from the dispense above.
			 if(drc == CHAN_STATUS_JAM || drc == OPOS_E_EXTENDED)	
			 {
				 // Use error handling mechanism in DMParse.
				 trace(L4, _T("Posting SafePay cash recycler error: %d"), drc);
				 mo.PostDM(DM_ERROR, DMCLASS_CASHRECYCLER, 0, drc, NULL);
	 			 return STATE_NULL;
			 }

			 // Why didn't you use m_nCashRecyclerHardwareErrorCode?  Answer:
			 // because it is not initialized to zero.  Didn't want to change
			 // initialization of this variable because it might break something.
			 drc = lBillErrorCode ? lBillErrorCode : lCoinErrorCode;
			 if(drc)
			 {
				 // In case note cover needs to be removed.
				 m_SafePayLastErrorCode = drc;  

				 // Note that 2 is used as the context argument.  This 
				 // distinguishes this error as a dispense error.  The same
				 // error codes can be returned for both deposit and dispense jams.
                 csWork = GetDeviceErrorMnemonic(DMCLASS_CASHRECYCLER,
                                                 _CASHDISPENSER_SAFEPAY, 
                                                 CHAN_STATUS_JAM, 
                                                 drc, 2);
			 }

			 // If drc is not set, then we don't have much to go on.
			 // Let the normal cash changer error handling proceed.
			 // cswork should have the value of DEVICEERRORUNKNOWN in this case.
		 }
  }
  // +TAR 353945  Dispenser is not a cash recycler
  else
  {
  	  // TAR 353945:  If cash device is not a recycler, then use
	  //              the cash changer XML error.
	  //              Removed if() block error disposition.
       csWork = GetDeviceErrorMnemonic(DMCLASS_CASHCHANGER,
                                       dm.GetDeviceName(DMCLASS_CASHCHANGER, 0),
                                       EVENTID_ANY,
                                       dm.GetResultCodeExtended(DMCLASS_CASHCHANGER, 0),
                                       0); //always show image
	  // -TAR 353945
      //+TAR 398739
	  if(csWork.Find(_T("NCR_E5633_FEED_FAILURE")) != -1)
	   {
           DMCheckCashCounts(false);
		   if(co.fLocaleCashManagementScreen&&dm.GetBillsVeryLow())
           {

			   m_bShowMediaStatusButton = true;
			   csWork=  _T("DeviceErrorCashChanger.xml:NCR_BOTH_CANNOT_DISPENSE_CASH_MANAGEMENT");
		   }
	   }
	   //-TAR 398739

  }
  

 trace(L6, _T("csWork : %s."), csWork);

 if(csWork.Find(_T("DEVICEERRORUNKNOWN")) != -1)  // Did not find the extended result code for this error
		csWork = CashChangerErrorMessage(lChangeDue, csCashCountsStatusCheck);
  trace(L6, _T("Going to report this cash changer error: %s"), csWork);
	
  
  // Added for TAR 191990


	CString csTitle1, csTitle2, csMessage, csMovie;
	BMPS bitmap;
	PSTEXTWAVE nWave;
    ProcessDeviceErrorXMLFile( csWork, csTitle1, csTitle2, csMessage, bitmap, nWave, csMovie);


    //Notify RAP
    trace(L7, _T("+ra.OnRAPDataNeeded, ErrorMessage=%s"), csWork);
    // write the error description on the receipt
     if(csTitle1.IsEmpty())
	{
		ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + csWork);
	}
	else
	{
		ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + csTitle1);
	}

	// save the previous view
	m_PrevView = m_View;

	// send a DataNeeded Approval
  cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("DeviceFailure"));
  m_View = DATANEEDED_DEVICEERROR;
  CString csSecInfo;



 	if(csTitle1.IsEmpty())
	{
		csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY),csWork,ps.GetPSText(BUTTON_RA_DEVICE_ERROR, SCOT_LANGUAGE_PRIMARY), _T(""), _T(""), _T(""), _T(""));
	}
	else
	{
		csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY),csTitle1 + _T("\n") + csTitle2 ,ps.GetPSText(BUTTON_RA_DEVICE_ERROR, SCOT_LANGUAGE_PRIMARY), _T(""), _T(""), _T(""), _T(""));
	}
	
  ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);
  trace(L7, _T("-ra.OnRAPDataNeeded"));
  
  // End for TAR 191990

  // TAR 297897 Begin
  if (lChangeDue < 0)
  {
	  // If the cash dispenser dispensed too much money, make sure that the next time around it is handled properly
	  trace(L6, _T("ERROR: Reset Change Due because of Overdispense"));
	  lChangeDue = 0;
  }
  // TAR 297897 End

  // Prevent queuing of any additional errors that are posted for this device class.
  m_nDMSavedDev =  DMCLASS_CASHCHANGER;
  
  // when this returns, we will return to this state at Initialize()
  return  createLevel1Message(csWork,           // char *message
                              PS_NOTEXT,        // no canned message
                              PS_NOTEXT,        // no prompt
                              0,                // no value
                              DMCLASS_CASHCHANGER,// device reporting the error or -1
                              true);            // T/F need to issue CheckHealth after operator clears problem
}

////////////////////////////////////
SMStateBase  *SMTakeChangeBase::TimedOut(void)
{   
  //if the cash out sensor is blocked then the customer hasn't removed their cash yet
  //tell them to take cash (only applies to dollar bills) and escalate to rap  		
	trace(L7, _T("SMTakeChangeBase::TimedOut() fCashOutSensorBlocked %d"),SMStateBase::fCashOutSensorBlocked ); 

  if((SMStateBase::fCashOutSensorBlocked) && m_fBillsDispensed)  //TAR 391559
  {

      DMSayPhrase(szwavFileToPlay);
     // SetTimer(hw, RAP_ESCALATION, nEscalationRAPTimeOut, &EscalationTimedOut);
      return STATE_NULL;
  }

  // Don't bother waiting if there is no paper change
  if (!fPaperMoneyReturned)
  {
	trace(L7, _T("SMTakeChangeBase::TimedOut() fPaperMoneyReturned %d"),fPaperMoneyReturned );
    return DMCashRemoved();	// Coin Acceptor Modifications
  }


  if (fTimedOutOnce)
  {
    // this is the second timeout in this state
    // suck in the bills if no one has removed them
    DMCashChangerPurgeBills();
	ScotError(INFO, SM_CAT_INTERNAL_ERROR, SCOTAPP_BILL_PURGED);
    return DMCashRemoved();	// Coin Acceptor Modifications
  }

  fTimedOutOnce = true;
// NewUI
  ps.SetCMFrame(_T("TakeChange"));
// NewUI
    
  ps.Message12(MSG_TAKECHANGE, true);

  ps.ShowChangeDueAmountPaid(saveChangeDue,lTotalPaid);

  if (fPaperMoneyReturned==true)
  {
	DMSayPhrase(SECURITYMED);
    //merged TAR232798 to 3.1.1 - fixed by Chen
	if ( fIs3xHardware )
	{
	  if (!dm.m_csCashRecyclerModel.IsEmpty() ) // TAR 304956 - if recycler
	  {
          DMSayPhrase(TAKECHANGE);
	  }
	  else
	  {
		 DMSayPhrase(TAKECHANGEBELOWSCANNER);
	  }
	}
	else
    {
		DMSayPhrase(TAKECHANGE);
    }		
	
  }
  ps.ShowCMFrame();
  return STATE_NULL;
}

//////////////////////////////
SMStateBase * SMTakeChangeBase::DMCashRemoved(void)
{
	bool bDispense = true;
	//+RFC 275377
	if (true == m_bCancellingCashTender) 
    {
		nTenderType = TB_TENDER_NONE;
        m_bCancellingCashTender = false; // resets the global variable
		RETURNSTATE(SmAssistMode);
    } 
	//-RFC 275377
	if (TrainingModeObj.IsTMOn() && !co.nOperationsAllowDispenseInTrainingMode) // in training mode and option set to not dispense
	{
		bDispense = false;
	}
    if (bDispense)
	{
		DMSetCashTaken();
	}
	// Begin Coin Acceptor Modifications
	// bReadyForTakeReceipt = true;
	if (fEndOfTrans)
	{	//Tar 124345
		// if(bTrxClosed) //Tar 126024

		RETURNSTATE(TakeReceipt)	// end this transaction
		// return STATE_NULL;
	}
	else
	{
		return createAnchorState();		// remain in the transaction
	}
	// End Coin Acceptor Modifications
}

///////////////////////////////////////////////////////////////////////////////
//
// WereBillsDispensed()
//
// Local function that determines if any bills where presented during the
// last cash dispense.  
//
///////////////////////////////////////////////////////////////////////////////
// Begin Dynamic Bill Detection
bool WereBillsDispensed()
{
	bool bStatus = false;
    long lAmtDispensed = 0;

    CString sList = dm.CashChangerGetDispensedCashList();
	int nSemiColon = sList.Find(_T(';'));
	int nLength = sList.GetLength();
	
	//Begin TAR114422
	//if (nSemiColon != (nLength - 1)) //this worked only for the emulators LPM071699 
    //{
	//	bStatus = true;
	//}

	if ((nSemiColon >= 0) && ((nLength - 1) > nSemiColon))
	{
    	/*
    	CString CashChangerDispensedCashList(); Description:
    	Returns a string specifying the amount dispensed both in bills and coins
    	ie  25:0,5:0,1:0;100:0,500:0,1000:0,2000:0 for the last dispense operation.
		*/

    	sList += _T(",");		 // append one last comma so the loop below finds the last pair
    	CString csWorkingList = sList.Right((nLength) - nSemiColon);
    	while (true)
    	{
			 int i = csWorkingList.Find(_T(','));
			 if (i<0) break;
			 CString csTrimmedList = csWorkingList.Left(i);  //everything left of the 1st comma
			 csWorkingList = csWorkingList.Mid(i+1);	  //reset the working list to exclude the group just processed
			 i = csTrimmedList.Find(_T(':'));						
			 if (i>0)					//if there's no colon, the list is finished
			 {
    			int lVal = _ttoi(csTrimmedList.Left(i));	  
    			int lNum = _ttoi(csTrimmedList.Mid(i+1));
    			lAmtDispensed += (lVal * lNum);	//multiply to find the monetary amt
			 }
    	}

		if (lAmtDispensed > 0)
		{
			bStatus = true;
		}
	}
	//End TAR114422
	return bStatus;
}
// End of Dynamic Bill Detection

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMTakeChangeBase::OnWtDecrease()
{
	if(!g_bIsForgiveUnexpectedDecreaseDuringTendering && !(lBalanceDue < 0.01))
		return FinalizationWtDecreaseNotAllowed(BES_STATERETURN);
	else
		return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMTakeChangeBase::OnWtIncreaseNotAllowed()	 
{
	return FinalizationWtIncrease(BES_STATERETURN);
}
/////////////////////////////////////////////////////////////////////////////////////
//
// Method:		void SMTakeChangeBase::OnMotionCashOutSensor
//
// Purpose:		handles OnMotionCashOutSensor event 
//
// Parameters:	none
//
// Returns:		void
//
/////////////////////////////////////////////////////////////////////////////////////
SMStateBase  *SMTakeChangeBase::OnMotionCashOutSensor(void) 
{
  trace(L7, _T("+SMTakeChangeBase::OnMotionCashOutSensor"));
  if(m_fBillsDispensed)  //TAR 391559
  {
      SMStateBase::fCashOutSensorBlocked = true;  

      trace(L7, _T("CashOutSensorBlocked<%d>"), SMStateBase::fCashOutSensorBlocked);
      nTimeOut = co.getTimeOutCashOutSensorAudioTimer();
      nEscalationRAPTimeOut = co.getTimeOutDelaySecurityNotification();
      if(nEscalationRAPTimeOut != 0)
        { 
             SetTimer(hw, RAP_ESCALATION, nEscalationRAPTimeOut, &EscalationTimedOut);
        }
      else if (nEscalationRAPTimeOut == 0) // if 0, immediate escalation 
      {
        m_bMotionEscalationFired = true;
        dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
        DM_TRICOLORLIGHT_BLINK_1HZ,
        DM_TRICOLORLIGHT_NORMALREQUEST);
	    cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("TakeChangeTimerExpires"));
		m_View = DATANEEDED_TAKECHANGETIMEREXPIRED;
		CString csSecInfo ;
        csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), _T(" "), 
            ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(LTT_TAKECHANGE, 
            SCOT_LANGUAGE_PRIMARY), _T(""), ps.GetPSText(RA_TIMEREXPIRED, SCOT_LANGUAGE_PRIMARY));    
        ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View); 
		ra.RequestEnableAssistMode(true); 
        STATE(SecOperatorPWState)::Initialize();
      }
  }
  trace(L7, _T("-SMTakeChangeBase::OnMotionCashOutSensor"));
  return STATE_NULL;
}
/////////////////////////////////////////////////////////////////////////////////////
//
// Method:		void SMTakeChangeBase::OnNoMotionCashOutSensor
//
// Purpose:		handles OnNoMotionCashOutSensore event 
//
// Parameters:	none
//
// Returns:		void
//
/////////////////////////////////////////////////////////////////////////////////////
SMStateBase  *SMTakeChangeBase::OnNoMotionCashOutSensor(void) 
{    
	if(m_fBillsDispensed)  //TAR 391559
    {
        trace(L7, _T("+SMTakeChangeBase::OnNoMotionCashOutSensor"));
        SMStateBase::fCashOutSensorBlocked = false;            // Sensor is broken cash has been removed

        trace(L7, _T("CashOutSensorBlocked<%d>"), SMStateBase::fCashOutSensorBlocked);
        if (m_bMotionEscalationFired) 
        {
	        // resets lane light, flushes belt, sets re-enable scanner flag, and tells RAP no data is needed
            // TAR 249233 Don't call SAClearSecurityCondition because it makes the takeaway belt 
            // takeaway belt jump
            ClearSecurityViolationLight();//End of Tar 209034 & 209045

	        //Clear RAP Approval 
	        trace(L2, _T("Calling ra.OnNoRAPDataNeeded()"));
	        ra.OnNoRAPDataNeeded();  
        }
        trace(L7, _T("-SMTakeChangeBase::OnNoMotionCashOutSensor"));
        //RETURNSTATE(TakeReceipt);//tar 268473
        return createAnchorState();//Let SMFinish decide if tranx is over

    }
    else
        return STATE_NULL;
}
/////////////////////////////////////////////////////////////////////////////////////
//
// Method:		void SMTakeChangeBase::EscalationTimedOut
//
// Purpose:		handles passing timeout event to Escalation2RAP using the current state pointer
//
// Parameters:	none
//
// Returns:		void
//
/////////////////////////////////////////////////////////////////////////////////////
VOID CALLBACK SMTakeChangeBase::EscalationTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
    trace(L7, _T("+SMTakeChangeBase::EscalationTimedOut"));
	mp.sCurr->Escalation2RAP();
    trace(L7, _T("-SMTakeChangeBase::EscalationTimedOut"));

}
/////////////////////////////////////////////////////////////////////////////////////
//
// Method:		void SMTakeChangeBase::Escalation2RAP
//
// Purpose:		2nd timeout routine handling the delayed security notification
//
// Parameters:	none
//
// Returns:		void
//
/////////////////////////////////////////////////////////////////////////////////////
void SMTakeChangeBase::Escalation2RAP()
{ 
    trace(L7, _T("+SMTakeChangeBase::Escalation2RAP"));
    KillTimer(hw,RAP_ESCALATION); 
    if(SMStateBase::fCashOutSensorBlocked) //if Cash Out Sensor is blocked
    {
        m_bMotionEscalationFired = true;
        dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
        DM_TRICOLORLIGHT_BLINK_1HZ,
        DM_TRICOLORLIGHT_NORMALREQUEST);
	    cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("TakeChangeTimerExpires"));
		m_View = DATANEEDED_TAKECHANGETIMEREXPIRED;
		CString csSecInfo ;
        csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), _T(" "), 
            ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(LTT_TAKECHANGE, SCOT_LANGUAGE_PRIMARY), 
            _T(""), ps.GetPSText(RA_TIMEREXPIRED, SCOT_LANGUAGE_PRIMARY));    
        ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View); 
		ra.RequestEnableAssistMode(true); 
		STATE(SecOperatorPWState)::Initialize();			
    } 
    trace(L7, _T("-SMTakeChangeBase::Escalation2RAP"));
 }
/////////////////////////////////////////////////////////////////////////////////////
//
// Method:		void SMTakeChangeBase::OnRAPClearSecurityBtn
//
// Purpose:		Handle the steps in clearing a security violation from RAP
//              Resets lane light, flushes belt, sets re-enable scanner flag, and tells RAP no data is needed
//
// Parameters:	none
//
// Returns:		
//
/////////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMTakeChangeBase::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
	trace(L7, _T("+SMTakeChangeBase::OnRAPClearSecurityBtn"));
	DMSaySecurity( SEC_NONE, _T("blank.wav") );			//to interrupt the wave file if it's playing 
	// resets lane light, flushes belt, sets re-enable scanner flag, and tells RAP no data is needed
        // TAR 249233 Don't call SAClearSecurityCondition because it makes the takeaway belt 
    // takeaway belt jump
    ClearSecurityViolationLight();//End of Tar 209034 & 209045

	//Clear RAP Approval 
	trace(L2, _T("Calling ra.OnNoRAPDataNeeded()"));
	ra.OnNoRAPDataNeeded();  
    // NewSec
    OverrideSecurityException(lpszAuthorizationType, KEY_EXCEPTION_OPERATION_CLEAR);

    //Let's the customer finish the transaction without unblocking the sensor
    trace(L7, _T("-SMTakeChangeBase::OnRAPClearSecurityBtn<Customer permitted to finish transaction without unblocking the sensor>"));
    return OnNoMotionCashOutSensor(); //tar295914
}
/////////////////////////////////////////////////////////////////////////////////////
//
// Method:		void SMTakeChangeBase::PSButtonGoBack
//
// Purpose:		
//
// Parameters:	none
//
// Returns:		void
//
/////////////////////////////////////////////////////////////////////////////////////
SMStateBase  *SMTakeChangeBase::PSButtonGoBack(void) // Store Login button
{
    trace(L7, _T("+SMTakeChangeBase::PSButtonGoBack-(StoreMode)"));

		switch(inputMode)
		{  
			case SHOW_WAIT_FOR_APPROVAL:
			case PROMPTING_FOR_PASSWORD:
				break;
			case PROMPTING_FOR_OPERATOR:
				{
					//+SR93 Biometrics support
					if (co.m_bOperationsUseFingerPrintDevice)
					{
						FT_Abort();
					}
					//-SR93 Biometrics support

					inputMode = SHOW_WAIT_FOR_APPROVAL;
					if (co.fStateDualLanguage)
						ps.SetLanguage(m_customerLanguage);  //Set back to customerLanuguage for dual language
					showScreen(false);
					return STATE_NULL;  
				}
		}

    DMSaySecurity( SEC_NONE, _T("blank.wav") );	//to interrupt the wave file if it's playing 
	promptForOperator();
    trace(L7, _T("-SMTakeChangeBase::PSButtonGoBack-(StoreMode)"));
    return STATE_NULL;	
}
/////////////////////////////////////////////////////////////////////////////////////
//
// Method:		void SMTakeChangeBase::stateAfterPasswordVerify
//
// Purpose:		
//
// Parameters:	none
//
// Returns:		
//
/////////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMTakeChangeBase::stateAfterPasswordVerify()
{
	trace(L2, _T("+SMTakeChangeBase::stateAfterPasswordVerify"));
	trace(L2, _T("-SMTakeChangeBase::stateAfterPasswordVerify"));
	return OnRAPClearSecurityBtn(_T("Lane"));
}
/////////////////////////////////////////////////////////////////////////////////////
//
// Method:		void SMTakeChangeBase::showScreen
//
// Purpose:		
//
// Parameters:	Boolean passwordVerified
//
// Returns:		void
//
/////////////////////////////////////////////////////////////////////////////////////
void SMTakeChangeBase::showScreen(bool passwordVerified)
{
	COleVariant v;
	if (passwordVerified)
		return;
	

		if (inputMode == PROMPTING_FOR_OPERATOR)
		{
			STATE(SecOperatorPWState)::ShowEnterID();  //SR93 Biometrics support
			ps.SetCMLeadthruText( TXT_ENTER_ID ); //Tar 395105
			ps.AllowInput(MAX_DIGITS_OPERATOR,false,false);
			ps.ShowCMFrame(false);
			return;
		}
		else
		{  
			ps.SetCMFrame(_T("TakeChange"));
			v = VARIANT_TRUE;
		    m_pAppPSX->SetConfigProperty(_T("ButtonStoreLogIn"), csCurContext, UI::PROPERTYVISIBLE, v);
		}

    ps.ShowCMTBText(csTBMessage2Scot);
	ps.ShowCMFrame(false);
}

/////////////////////////////////////////////////////////////////////////
void SMTakeChangeBase::UnInitialize()
{
    KillTimer(hw,RAP_ESCALATION);   //Ensure timer is dead

	m_bCashRecyclerStopDeposit = false;
	dm.CashRecyclerResetCounts();
	m_nDMCashPaid = 0;		// TAR 305659
	bDoNotResetTimeOutOnDMEvent = false;  //Tar324574
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("TakeChange") );	// RFC 330050
}

//merged TAR232798 to 3.1.1 - fixed by Chen
//Read from registry to see if it is 3.x hardware
bool Is3xHardware()
{
	_TCHAR szHardware[6];       //Default setting to No 
	LONG   lRC;
	HKEY   hKey;
	DWORD  dwDataSize = sizeof(szHardware);
	lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\NCR\\SCOT\\Installation\\platform"), 
               0, KEY_QUERY_VALUE, &hKey);

	lRC = RegQueryValueEx(hKey, _T("Configure"), NULL, NULL,(LPBYTE)szHardware, &dwDataSize);
    ::RegCloseKey(hKey);
	
	if ( lRC == ERROR_SUCCESS )
	{
		//TAR 271451
		if( _tcsicmp(szHardware, _T("SCOT3")) && //if not 3.x hardware
			_tcsicmp(szHardware, _T("SCOT4")) && //and not 4.x hardware
			_tcsicmp(szHardware, _T("MINI4")) && //and not 4.x MINI hardware
			_tcsicmp(szHardware, _T("MINI")))    //and not MINI
		{
 		    trace(L6, _T("SMTakeChange::This is not 3x/4x hardware, it is %s"), szHardware);
			return false;		
		}
		else
		{
			trace(L6, _T("SMTakeChange::This is 3x/4x (%s) hardware"), szHardware);
			return true;
		}
	}
	else
		return false;


}
//end of TAR232798


