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
//               // MS MFC template header
#ifndef _CPPUNIT
#include "Common.h"                 // Common includes
#include "DeviceErrorLookUpAPI.h"   //SR672
#include "NCRCashServicesSOEx.h"

#include "SMTakeChange.h"
#include "SMTakeReceipt.h"
#include "SMSecUnExpectedIncrease.h"
#include "MPProcedures.h"
//#include "SMInSAProgress.h"         // this state
#include "SMState.h"                // Base state
#include "smsmassistmode.h"			//RFC275377 Migrated BM185018
#include "GloryOpos.h"				// Need CHAN_STATUS_JAM definition.

DLLEXPORT extern HWND hw;           
DLLEXPORT extern MPProcedures mp; 

#else
#include "SMTakeChangeBase.h"
#endif
 
#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("TakeChangeBase")



DEFINE_TIMESTAMP
IMPLEMENT_DYNCREATE(SMTakeChangeBase, CObject)// MFC Runtime class/object information

  
PSTEXTWAVE szwavFileToPlay;
////////////////////////////
SMTakeChangeBase::SMTakeChangeBase(const bool fEOT)
#ifndef _CPPUNIT
: fEndOfTrans(fEOT),	// Coin Acceptor Modifications
  fTimedOutOnce(false),
  m_bMotionEscalationFired(false),
  m_fBillsDispensed(false),  //TAR 391559
  saveChangeDue(0),
  csCurContext(EMPTY_STRING)
#endif
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMTakeChangeBase::SMTakeChangeBase()
#ifndef _CPPUNIT
: csCurContext(EMPTY_STRING)
#endif
{
#ifndef _CPPUNIT
  fEndOfTrans = false;	// Coin Acceptor Modifications
  fTimedOutOnce = false;
  m_fBillsDispensed = false;  //TAR 391559
  saveChangeDue = 0;
#endif
  IMPLEMENT_TIMESTAMP
}
#ifndef _CPPUNIT

////////////////////////////////////////
SMStateBase  *SMTakeChangeBase::Deliver(const bool fEOT)
{
  fEndOfTrans = fEOT;	// Coin Acceptor Modifications
  fTimedOutOnce = false;
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
#endif // _CPPUNIT
///////////////////////////////////////
SMStateBase  *SMTakeChangeBase::Initialize(void)
{

  BMPS		eBMP;
  PSTEXT	eLeadthru;
  PSTEXT    eMsg;
  PSTEXT	eEcho;
  long		drc = 0;				// Store result code of dispense.
  csCurContext = _T("TakeChange");  // Tar 238987


  //+317732 
  if(SMStateBase::fCashOutSensorBlocked) //if true, we can say we've been here(TakeChange state) but must have been interrupted by some other state
  {
	  //Turn timer back ON, for cases when TakeChange was interrupted/exited with cash still pending removal
	  trace(L7, _T("SMTakeChangeBase::Initialize() Turning Escalate2RAP timer ON"));
      nTimeOut = co.getTimeOutCashOutSensorAudioTimer();
      nEscalationRAPTimeOut = co.getTimeOutDelaySecurityNotification();
#ifndef _CPPUNIT
      if(nEscalationRAPTimeOut != 0)
      { 
		  SetTimer(hw, RAP_ESCALATION, nEscalationRAPTimeOut, &EscalationTimedOut);
      }
      else if (nEscalationRAPTimeOut == 0) // if 0, immediate escalation 
      {
		  //we won't reach here. If it was immediate, it would have been handled 
		  //on first entry into TakeChange, so no need to implement here      
      }
#endif // _CPPUNIT
  }
  //-317732
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("TakeChange") );	// RFC 330050
  //Tar 237024, lost receipt after cash dispenser error
  SMStateBase *sReturnState = CkStateStackAndReturn();
  if (sReturnState != STATE_NULL)
	  return sReturnState;

  bDoNotResetTimeOutOnDMEvent = true;  //Tar324574

  // CANNOT enter Remote Assist Mode at this state
  //ra.OnSetTransactionState(RA_STATE_ASSISTNOTOK);
  ra.RequestEnableAssistMode(false);   //TAR 213249
  //Update RAP window with SCOT state
  UpdateStateName(ps.GetPSText(LTT_TAKECHANGE, SCOT_LANGUAGE_PRIMARY));

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
  
  ps.SetCMFrame(csCurContext);

  //To ensure store login button is not visable
  m_pAppPSX->SetConfigProperty(_T("ButtonStoreLogIn"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);

  // Display this here because letting the MPProcedures::ProcessNextState()
  // do it causes it not to be displayed until AFTER we dispense the cash,
  // which is a long operation.
  DisplayAuxScreen(AUX_SCREEN_DEFAULT, true);

  //411233
  if (( co.IsDegradedModeOn() && dm.GetDegradedModeType() == DEGRADEDMODE_CARD_ONLY && lChangeDue == 0 && nDMCashInserted == 0 ) || //436794
      !IsCashDeviceConfigured())
  {
	  //lChangeDue = 0; //436794
	  RETURNSTATE(TakeReceipt);  //414907
  }

  //+SSCOADK-836
  //Add in any cash inserted but not sent to TB, so we can give it back now
  if( nDMCashInserted)
  {
	  lChangeDue += nDMCashInserted;
	  nDMCashInserted = 0; 
  }

  if (lChangeDue==0)   //tar 272209
      return STATE_NULL; // TAR 05/13/02 TAR 204027: Avoid receiving coupon change back twice on same transaction

  CString csTemp = EMPTY_STRING;
  csTemp.Format(ps.GetPSText(MSG_RA_CHANGEDISPENSED, SCOT_LANGUAGE_PRIMARY), ps.FmtDollar(lChangeDue));
  trace(L7, _T("+ra.OnNormalItem, Message=%s"), csTemp);
  ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY+ csTemp);
  trace(L7, _T("-ra.OnNormalItem"));
  //-SSCOADK-836

  saveChangeDue = lChangeDue;
  bool bDispense = true;
  if (TrainingModeObj.IsTMOn() && !co.GetfOperationsInterventionAtStartOfTrainingModeTransaction()) // in training mode and option set to not dispense
  {
    bDispense = false;
    nTimeOut = 3; // stay some time on take change even in training mode
    lChangeDue = 0;
    return STATE_NULL; // OK, wait for timeout
  }

  CString csCashCountsStatusCheck;
  long rc = DMDispenseChange(lChangeDue, csCashCountsStatusCheck, &drc);

  if (rc>0) 
  {
      lChangeDue -= rc;
  }
  
  //+SSCOP-3518 - moved here from above.
  /*
  if (lBalanceDue>0)
  {
	  // ShowAmountRefundedAmountPaid() param 1 changed 
	  // to saveChangeDue from lChangeDue.
	  ps.ShowAmountRefundedAmountPaid(saveChangeDue, lTotalPaid );
  }
  else 	
  {
	  // if lChangeDue > 0 during takechange then a posible jam case, we have to make
	  // sure to display the updated change due amount.
	  ps.ShowChangeDueAmountPaid(lChangeDue >  0 ? lChangeDue : saveChangeDue, lTotalPaid);
  }
  */
  
  if (lChangeDue == 0 && co.GetfOperationsHopperSubstitutionIntervention() && !fCancelPayment)
      nHopperTrxCount = nHopperTrxCount + 1;

  m_fBillsDispensed = WereBillsDispensed();  //TAR 391559
  // +TAR 388086 
  // SSCOADK-1060 - Added saveChangeDue==0, if no refundable amount then don't wait for timeout.
  if (lChangeDue == 0 && !m_fBillsDispensed && co.GetfOperationsTakeChangeBypassedIfNoBills())  //TAR 391559
  {
      // Performance enhancement.  If no bills were dispensed, then don't 
      // wait for timeout.
      trace(L6, _T("SMTakeChangeBase::Initialize() - No bills dispensed" ));
      return DMCashRemoved();
  }
  // -TAR 388086

  ps.ShowCMFrame();
  //-SSCOP-3518
  ps.SetCMLeadthruText(eLeadthru);  //sscoadk-6740 moved this after ShowCMFrame where it sets the language
  ps.Message12(eMsg, true);         //sscoadk-6740 moved this after ShowCMFrame where it sets the language
  //sscoadk-6740+ move it here after language is set in ShowCMFrame()
  if (lBalanceDue>0)
  {
	  // ShowAmountRefundedAmountPaid() param 1 changed 
	  // to saveChangeDue from lChangeDue.
	  ps.ShowAmountRefundedAmountPaid(saveChangeDue, lTotalPaid );
  }
  else 	
  {
	  // if lChangeDue > 0 during takechange then a posible jam case, we have to make
	  // sure to display the updated change due amount.
	  ps.ShowChangeDueAmountPaid(lChangeDue >  0 ? lChangeDue : saveChangeDue, lTotalPaid);
  }
  //sscoadk-6740-

  // Begin Dynamic Bill Detection
  //  SSCOADK-2844 - Refactor SMTakeChange
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
  DMSaySecurity(SEC_NONE, ps.GetSoundName(szwavFileToPlay));  //SSCOP-3579
  nTimeOut = co.getTimeOutTakeCoins();  //default is 3 seconds. Also fPaperMoneyReturned is not used for 3x.
  
  // TAR 353244 - Initialize csWork.
  CString csWork /*= _T("DeviceErrorCashRecycler.xml:DEVICEERRORUNKNOWN")*/;
  
  bool bEmpty = false;
  bool bChuteJam = false;
  IsHopperSubstitutionUsed(bEmpty, bChuteJam);

  if (lChangeDue==0)
      return STATE_NULL; // OK, wait for timeout

  if (bChuteJam)
	  return STATE_NULL;  //DM parse will take to system message of chute jam
	
  // Error condition of we didn't give all change due...
  // Need to distinguish coin or bill error.   TINA
  trace(L6, _T("cash changer error: %s"), csCashCountsStatusCheck);

  //SR672
  long lCoinStatus = 0;
  long lBillStatus = 0;
  dm.CashChangerGetStatusValues(&lCoinStatus, &lBillStatus);

  CString sParms;
  long lcode = 0;
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
		 bool fCheckHealth = false;
		 //SR672
		 if(lBillErrorCode)
		  {
			 if(dm.m_csCashRecyclerModel.Find(_CASHDISPENSER_GLORY) != -1)
			 {
				csTempErrorCode.Format(_T("%d"),lBillErrorCode);
				csTempErrorCode = csTempErrorCode.Left(2); // xml file only use first 2 digits
				//csWork = GetDeviceErrorMnemonic(nDMLastDev, _BILLRECYCLER, 0, _ttol(csTempErrorCode), nDeviceErrorContext );
				CString sSubModel = _T("Bill");
				CString sModelName = dm.GetDeviceModelName(DMCLASS_CASHRECYCLER, 0);
				csWork = DMGetDeviceErrorHTML(DMCLASS_CASHRECYCLER, sModelName, sSubModel, 0, _ttol(csTempErrorCode), 0, NULL, &fCheckHealth, nDeviceErrorContext, co.csPrimaryLanguageCode );
			 }
			m_nCashRecyclerHardwareErrorCode = lBillErrorCode;
			sParms.Format(_T("%d"), m_nCashRecyclerHardwareErrorCode);

		  }
		  else 
		  {
			  lCoinErrorCode = dm.CashRecyclerGetErrorCode(0x01);
			  if(lCoinErrorCode)
			  {
				  if(dm.m_csCashRecyclerModel.Find(_CASHDISPENSER_GLORY) != -1)
				  {					  
					  csTempErrorCode.Format(_T("%d"),lCoinErrorCode);
					  csTempErrorCode = csTempErrorCode.Left(2); // xml file only use first 2 digits
					  //csWork = GetDeviceErrorMnemonic(nDMLastDev, _COINRECYCLER, 0,  _ttol(csTempErrorCode), nDeviceErrorContext );
					  CString sSubModel = _T("Coin");
					  CString sModelName = dm.GetDeviceModelName(DMCLASS_CASHRECYCLER, 0);
					  csWork = DMGetDeviceErrorHTML(DMCLASS_CASHRECYCLER, sModelName, sSubModel, 0, _ttol(csTempErrorCode), 0, NULL, &fCheckHealth, nDeviceErrorContext, co.csPrimaryLanguageCode );
				  }
				m_nCashRecyclerHardwareErrorCode = lCoinErrorCode;
				sParms.Format(_T("%d"), m_nCashRecyclerHardwareErrorCode);
			  }
		  	 
		  }
	 
		 // Handle SafePay dispense error.
		 // The SO always seems to return 201 which is "UNKNOWN SAFEPAY ERROR".
		 // Not very useful information.
		 if(dm.m_csCashRecyclerModel.Find(_CASHDISPENSER_SAFEPAY) != -1)		 
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
				 /*
                 csWork = GetDeviceErrorMnemonic(DMCLASS_CASHRECYCLER,
                                                 _CASHDISPENSER_SAFEPAY, 
                                                 CHAN_STATUS_JAM, 
                                                 drc, 2);
												 */
				 bool fCheckHealth = false;
				 CString sSubModel = _T("");
				 CString sModelName = dm.GetDeviceModelName(DMCLASS_CASHRECYCLER, 0);
				 csWork = DMGetDeviceErrorHTML(DMCLASS_CASHRECYCLER, sModelName, sSubModel, CHAN_STATUS_JAM, drc, 0, NULL, &fCheckHealth, 2, co.csPrimaryLanguageCode );
				 sParms.Format(_T("%s"), m_SafePayLastErrorCode);
			 }

			 // If drc is not set, then we don't have much to go on.
			 // Let the normal cash changer error handling proceed.
			 // cswork should have the value of DEVICEERRORUNKNOWN in this case.
		 }

		 if ( !sParms.IsEmpty() && !csWork.IsEmpty() && csWork.Find(_T("DeviceErrorUnknown"))== -1  )
		  {
	            TCHAR sHTML2[_MAX_PATH];
		        _tcscpy(sHTML2, csWork.GetBuffer(_MAX_PATH));
		        ParseDeviceErrorHTML(sHTML2, sParms.GetBuffer(_MAX_PATH), _T("%s"));
		        csWork.ReleaseBuffer();
		        csWork = sHTML2;
		        sParms.ReleaseBuffer();
		  }

  }
  // +TAR 353945  Dispenser is not a cash recycler
  else
  {
      // Handles cash changer error. 
      csWork = GetCashChangerError();
  }
  

 trace(L6, _T("csWork : %s."), csWork);

 if(csWork.Find(_T("DeviceErrorUnknown")) != -1 || csWork.Find(_T("Ignore")) !=-1 || csWork.IsEmpty()) // Did not find the extended result code for this error
		csWork = CashChangerErrorMessage(lChangeDue, csCashCountsStatusCheck);

  trace(L6, _T("Going to report this cash changer error: %s"), csWork);
	
  // Added for TAR 191990

	CString csTitle1;
	if (lCoinStatus)
	  csTitle1 = ps.GetPSText(LTT_COINDISPENSERERROR,  SCOT_LANGUAGE_PRIMARY);
	if (lBillStatus)
	  csTitle1 = ps.GetPSText(LTT_BILLDISPENSERERROR,  SCOT_LANGUAGE_PRIMARY);
    if (lCoinStatus && lBillStatus)     
      csTitle1 = ps.GetPSText(MSG_COIN_BILL_DISPENSER_ERROR,  SCOT_LANGUAGE_PRIMARY);

	ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + csTitle1);

	// save the previous view
	m_PrevView = m_View;

	// send a DataNeeded Approval
  cDataEntry.FromConfigData(co.GetcsDataEntryINIFile(), _T("DeviceFailure"));
  m_View = DATANEEDED_DEVICEERROR;
  CString csSecInfo;



 	if(csTitle1.IsEmpty())
	{
		csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY),csWork,ps.GetPSText(BUTTON_RA_DEVICE_ERROR, SCOT_LANGUAGE_PRIMARY), _T(""), _T(""), _T(""), _T(""));
	}
	else
	{
		csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY),csTitle1 /*+ _T("\n") + csTitle2 */,ps.GetPSText(BUTTON_RA_DEVICE_ERROR, SCOT_LANGUAGE_PRIMARY), _T(""), _T(""), _T(""), _T(""));
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
                              true,            // T/F need to issue CheckHealth after operator clears problem
							  true);   //display html
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
	if (TrainingModeObj.IsTMOn() && !co.GetnOperationsAllowDispenseInTrainingMode()) // in training mode and option set to not dispense
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
bool SMTakeChangeBase::WereBillsDispensed()
{

    //sscoadk-2959+
    //For Glory, it has sensors for both coin and bill. SSCO should just stay at TakeChange until the change (coin or bill) is taken
    if(dm.m_csCashRecyclerModel.Find(_CASHDISPENSER_GLORY) != -1)
	{
        trace(L6, _T("WereBillsDispensed: It is Glory, wait on sensor from either coin or bill"));
        return true;
    }
    //sscoadk-2959-

	bool bStatus = false;
    
#ifndef _CPPUNIT
    long lAmtDispensed = 0;

    CString sList = dm.CashChangerGetDispensedCashList();
	int nSemiColon = sList.Find(_T(';'));
	int nLength = sList.GetLength();

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
#endif //_CPPUNIT
	return bStatus;
}
// End of Dynamic Bill Detection
#ifndef _CPPUNIT
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
        
        // (*) SR898 ====================
        DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
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
#endif

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
#ifndef _CPPUNIT
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
#endif
        if (m_bCancellingCashTender) 
        { 
            nTenderType = TB_TENDER_NONE; 
            m_bCancellingCashTender = false; // resets the global variable 
            RETURNSTATE(SmAssistMode); 
         } 
        else
        {
            trace(L7, _T("-SMTakeChangeBase::OnNoMotionCashOutSensor"));
            //RETURNSTATE(TakeReceipt);//tar 268473
            return createAnchorState();//Let SMFinish decide if tranx is over
        }

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
    KillTimer(hw,RAP_ESCALATION); 
    mo.PostGP(GP_ESCALATE_TO_RAP);
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
    return;  //Handle this in GPNotify instead
}

/////////////////////////////////////////////////////
SMStateBase *SMTakeChangeBase::GPNotify(const GPEVT gpEvt)
{
   trace(L6, _T("+SMTakeChangeBase::GPNotify(%d)"), gpEvt);
    SMStateBase* retState = STATE_NULL;
    if (gpEvt == GP_ESCALATE_TO_RAP)
    {
        if(SMStateBase::fCashOutSensorBlocked) //if Cash Out Sensor is blocked
        {
            m_bMotionEscalationFired = true;
            
            // (*) SR898 ====================
            DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
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
            retState = STATE(SecOperatorPWState)::Initialize();			
        } 
    }
    trace(L6, _T("-SMTakeChangeBase::GPNotify"));
    return retState;
}

#ifndef _CPPUNIT
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
					if (co.GetfStateDualLanguage())
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
    ps.SetPSText(PS_WORK1,ps.GetPSText(BTT_STORELOGIN,SCOT_LANGUAGE_PRIMARY));
    ps.SetButtonText(_T("ButtonStoreLogIn"),PS_WORK1);
	COleVariant v;
	if (passwordVerified)
		return;
	

		if (inputMode == PROMPTING_FOR_OPERATOR)
		{
			STATE(SecOperatorPWState)::ShowEnterID();  //SR93 Biometrics support
			ps.SetCMLeadthruText( TXT_ENTER_ID ); //Tar 395105
			ps.AllowInput(co.GetnOperationsOperatorIDMaxDigit(),false,false);
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
#endif //_CPPUNIT



