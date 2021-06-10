//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMRAPDataNeededBase.CPP
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
#include "SMRAPDataNeeded.h"  // This state
#include "SMSmAbort.h"              // Store mode abort state
#include "SMSmAuthorization.h"      // Store mode auths state
#include "DMProcedures.h"
#include "SMSmConfirmSignature.h"
#include "SMSmDataEntry.h"
#include "SMSmConfirmSuspendedTransaction.h"
#include "SCOTDataEntry.h"
#include "SMSmAssistMode.h"
#include <math.h>
#include "SMInsertCoupon.h"

CString SMRAPDataNeededBase::m_sView        = _T("");         // String of RAP view to be displayed to RAP operator
CString SMRAPDataNeededBase::m_sDesc        = _T("");         // Prompt to appear above view desciprtion on RAP
CString SMRAPDataNeededBase::m_sViewName    = _T("");         // TAR 233927StringName of RAP view to be displayed to RAP operator
PSTEXT SMRAPDataNeededBase::m_ViewText=0;       // Message seen by customer during this state
int SMRAPDataNeededBase::m_saveDataNeededView = DATANEEDED_NOVIEW; //tar 252987

DLLEXPORT extern DMProcedures dm;   // Global DM object

#define COMP_ID ID_SM               // Base state component
#define T_ID _T("RAPDataNeededBase")

IMPLEMENT_DYNCREATE(SMRAPDataNeededBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(RAPDataNeeded)

DEFINE_TIMESTAMP

// This object needs to be accessible by both RAPDataNeededBase and SmDataEntryBase
CSCOTDataEntry SMRAPDataNeededBase::sm_pDataNeededEntry;

////////////////////////////////////////////////////
SMRAPDataNeededBase::SMRAPDataNeededBase(LPCTSTR sDesc, int View)
: m_origView(-1)   // TAR 265143
{
  IMPLEMENT_TIMESTAMP
  bMoveSAMsgDown = true;
  m_sDesc = sDesc;
  m_View = View;
  fStoreMode = false;		//TAR 213678 Set the default value

}

////////////////////////////////////////
SMRAPDataNeededBase::SMRAPDataNeededBase()
{
  IMPLEMENT_TIMESTAMP
  if(m_saveDataNeededView > DATANEEDED_NOVIEW) //tar 252987
  {
      trace(L6, _T("SMRAPDataNeededBase::SMRAPDataNeededBase() Using Saved View (%d)"),m_saveDataNeededView);
      m_View = m_saveDataNeededView;
      m_origView = m_saveDataNeededView;   // TAR 265143
  }
  m_saveDataNeededView = DATANEEDED_NOVIEW; //tar 252987
  fStoreMode = false;		//TAR 213678 Set the default value
}

////////////////////////////////////////
SMStateBase  *SMRAPDataNeededBase::Deliver(LPCTSTR sDesc, int View)
{
  m_sDesc = sDesc;
  m_View = View;
  m_origView = View;   // TAR 265143
  bMoveSAMsgDown = true;
  fStoreMode = false;		//TAR 213678 Set the default value
  SMState::g_bBackToBackDataNeeded = true; // TAR 265143
  return this;
}

////////////////////////////////////////
SMStateBase  *SMRAPDataNeededBase::Deliver(LPCTSTR sDesc, int View, LPCTSTR pPtr, const bool storeMode)
{
  m_sDesc = sDesc;
  m_View = View;
  m_origView = View;   // TAR 265143
  fStoreMode = storeMode;
  // This will setup the object properties and members with
  // the data provided by the TB
  sm_pDataNeededEntry.FromString(pPtr);
  SMState::g_bBackToBackDataNeeded = true; // TAR 265143
  return this;
}
/////////////////////////////////////////
bool SMRAPDataNeededBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{

    // There should be two or four parameters
    if (dlParameterList.GetDescriptorCount() == 2)
    {
        trace(L6, _T("+SMRAPDataNeededBase::DeliverDescriptorList() - Parameter Count of 2"));
        Deliver((LPCTSTR)DLParamCString(dlParameterList),    // sDesc
                (int)DLParamWORD(dlParameterList));          // View
        return true;
    }
    else if (dlParameterList.GetDescriptorCount() == 4)
    {
        trace(L6, _T("+SMRAPDataNeededBase::DeliverDescriptorList() - Parameter Count of 4"));
        Deliver((LPCTSTR)DLParamCString(dlParameterList),    // sDesc
                (int)DLParamDWORD(dlParameterList),           // View
                (LPCTSTR)DLParamCString(dlParameterList),    // pPtr
                (bool)DLParamWORD(dlParameterList));         // storeMode
        return true;
    }
    else
    {
        trace(L2, _T("+SMRAPDataNeededBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}

/////////////////////////////////////////
SMStateBase  *SMRAPDataNeededBase::TimedOut(void)
{
  if (nSaidCount<10)
  {
    DMSayPhrase(WAITFORASSISTANCE);
    nSaidCount++;
  }

  return STATE_NULL;
}

///////////////////////////////////////////
SMStateBase  *SMRAPDataNeededBase::Initialize(void)
{
  if(!fInMultiSelectPickList)//SR679
  {
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("RAPDataNeeded") );
  }
  dm.saveTriColorLightStatus();	//TAR209439 LPM071102 reset the lane lite
  // RFC 197844, during testing I found that these strings need to be initialized each time
  // and SetupViewData has to be done earlier before running FromConfigData
  m_sButton.Empty();
  m_sInput.Empty();
  // start of TAR 265143
  SMState::g_bBackToBackDataNeeded = false;
  if( m_View == -1)
  {
    m_View = m_origView;
  }
  // end of TAR 265143

  // Determine customer text and RAP view text to be used
  // in this state -- we need it to paint this frame.
  SetupViewData();

  // If this is from TBGetDataNeededView, we do not want to read the ini file
  // TB is passing us the ini parameters directly within a string
  if ( (m_View != DATANEEDED_MOREDATA) && (m_View < DATANEEDED_LASTVIEW) ) // TAR 296066
      sm_pDataNeededEntry.FromConfigData(co.csDataEntryINIFile, m_sView);     // RFC 197844

   SMStateBase *pNextState = STATE_NULL;

  bFinish = true;   // to tell rap to go to suspend right away

  ra.OnSetTransactionState(RA_STATE_DATA_NEEDED);

  nTimeOut = co.getTimeOutHelp();   // ring alarm at timeout

  // If there is a previous view, we never appeared to leave this state
  // from the user's perspective, so there is no reason to repeat the phrase.
  if (ra.GetRAPPreviousView() != DATANEEDED_NOVIEW)
  {
//	  DMSayPhrase(WAITFORASSISTANCE);
      ra.RequestEnableAssistMode(true);//tar 245596
	  nSaidCount = 1;
  }

  // RFC 212328 RJF 082602 if already in store mode and wanting
  // TB Data Needed screen,don't need to login again
  if(storeMode() && (m_View == DATANEEDED_MOREDATA))
  {
	  if (!m_sDesc.IsEmpty() && !m_sView.IsEmpty())
		  pNextState = BeginRAPView();
      return stateAfterPasswordVerify();
  }

  pNextState = STATE(OperatorPasswordState)::Initialize();

  // Only try to paint RAP View if this state initialized properly.
  if (pNextState == STATE_NULL)
	  if (!m_sDesc.IsEmpty() && !m_sView.IsEmpty())
		  pNextState = BeginRAPView();

  return pNextState;
}

void SMRAPDataNeededBase::UnInitialize()
{
  if(!(storeMode() && (m_View == DATANEEDED_MOREDATA)))
	  STATE(OperatorPasswordState)::UnInitialize(); // disabled the scanner which is enabled in Initialize
	// TAR209439 Can't blanketly set the light off!  must use the existing logic LPM071102
	//TD 06/11/02 TAR 198931 Moved ra.OnNoRAPDataNeeded();

  // start of TAR 265143
  if( g_bClearOccurred && !g_bBackToBackDataNeeded)
  {
    int iView = m_View;
    SAClearSecurityCondition();
    m_View = iView;  // Handle another overriding condition
  }
  g_bClearOccurred = false;
  // end of TAR 265143
  if(!fInMultiSelectPickList)//SR679
  {
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("RAPDataNeeded") );
  }
}

/////////////////////////////////////////////////////////
void SMRAPDataNeededBase::showScreen(bool passwordVerified)
{

  saMsgFuncPtr = PSProceduresBase::SecurityMessageModal;
  int nNumApprovals = 0;

  // This state does not need to reshow if the password
  // was verified, since we'll be leaving now
  if (passwordVerified)
  {
    return;
  }

  CString csWork;


     if (inputMode == PROMPTING_FOR_OPERATOR)
     {
        saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;
 
		STATE(OperatorPasswordState)::ShowEnterID();  //SR93 Biometrics support

		ps.SetCMLeadthruText( TXT_ENTER_ID );
		ps.AllowInput( MAX_DIGITS_OPERATOR, false, false );
		ps.ShowCMFrame( false );

		return;
     }
     else
     {
		// TK - TAR 199024
        // Disabling scanner when cashier assistance needed screen is displayed.
		DMScannerDisable();
		// TAR 199024
		// Display Wait for Assistance....
        // NewUI
        ps.SetCMFrame(_T("RAPDataNeeded"));
        // End NewUI

		// TK - TAR 197541
		// Playing assistance needed wave file
		DMSaySecurity(SEC_NONE, ps.GetSoundName(ASSISTANCENEEDED));
     }

  //RFQ 2506 - Add Dataneeded override of Leadthru text
  csWork = _T("");
  csWork = sm_pDataNeededEntry.GetRDNLaneLeadthru();
  if (!csWork.GetLength())
	  ps.SetCMLeadthruText(LTT_WAITFORASSISTANCE);
  else
	  ps.SetCMLeadthruText(csWork);

  if(m_View == DATANEEDED_ENDORSEDOCUMENT)
	ps.SetCMGobackButtonVisible(false);
  else
  {
	ps.CMButton(_T("ButtonStoreLogIn"), BTT_STORELOGIN, true);
	//T.D 05/02/02 TAR 2032663 only need store-login button
	//ps.SetCMGobackButtonVisible(true);
  }

  ps.CMButton(_T("ButtonHelp"), BTT_HELP,false);  //Tar 143857 Disable the help button.
  // NewUI

  //RFQ 2506 - Add Dataneeded override of message text
  csWork = _T("");
  csWork = sm_pDataNeededEntry.GetRDNLaneMessage();
  if (!csWork.GetLength())
  {
	  csWork = ps.GetPSText(MSG_WAITFORASSISTANCE);
	  csWork += _T("\n* ") + ps.GetPSText(m_ViewText);
  }
  ps.Message12(csWork, true);
  ps.ShowCMFrame();
}


//////////////////////////////////////////////////////
SMStateBase * SMRAPDataNeededBase::stateAfterPasswordVerify()
{
  //RETURNSTATE(SmAuthorization)
  //Begin RFC 356020: Support Check payment on lane using printer 7167
  if (SMStateBase::nTenderType == TB_TENDER_CHECK_LANE)
  {
	RETURNSTATE(SmAssistMode);
  }
  else
	RETURNSTATE(SmDataEntry)
  //End RFC 356020
}

//////////////////////////////////////////////////////
SMStateBase *SMRAPDataNeededBase::HandleRAPData()
   {
	SMStateBase *pNextState = STATE_NULL;

	switch (m_View)
      {
      case DATANEEDED_APPROVEVOIDEDITEM:
         pNextState = HandleApproveVoidedItem();
         break;
      case DATANEEDED_CONFIRMSUSPEND:
         pNextState = HandleConfirmSuspend();
         break;
      case DATANEEDED_PRICEPROMPT:
         pNextState = HandlePricePrompt();
         break;
      case DATANEEDED_ASSISTEDTENDER:    // Tender requiring RAP assistance.
         pNextState = HandleAssistedTender();
         break;
      case DATANEEDED_ENDORSEDOCUMENT:
         pNextState = HandleAssistedEndorsement();
         break;
      case DATANEEDED_MOREDATA:         // RFC 197844
		 //USSF START
		 USSFHOOK(_T("HandleDataNeeded"), pNextState = HandleDataNeeded(), ;);
         //pNextState = HandleDataNeeded();   // called by multiple states so resides in SMStateBase
		 //USSF END
         break;
	  case DATANEEDED_STARTTRAININGTRANS:
        pNextState = HandleStartTrainingTrans();
        break;
	  //+SR678
	  case DATANEEDED_MAXCOUPONLIMIT:			
		pNextState = HandleMaxCouponLimit();
		break;
	  case DATANEEDED_MAXCOUPONLIMITFINALIZATION:
		pNextState = HandleMaxCouponLimitFinalization();
		break;
	  //-SR678
      default:
         // This is an unknown View.  All views handled by this state
         // must be present in this switch, otherwise they are passed to the
         // base class.
         trace(L0, _T("SMRAPDataNeededBase::HandleRAPData() - Default case hit - 0x%08.8x is an unknown view, calling base."), m_View);
         pNextState = SMStateBase::HandleRAPData();
      }
	  g_bFromRAP = false;  // TAR 265143
    return pNextState;
   }
/******cm150000: End Merge 2******/

SMStateBase *SMRAPDataNeededBase::BeginRAPView()
   {
   SMStateBase *pNextState = STATE_NULL;

   ASSERT(!m_sDesc.IsEmpty());
   ASSERT(!m_sView.IsEmpty());
//TAR 215932
	// Tar 211874 RJF 3/19/03 don't need to check for just price-needed
	// all cases coming thru rapdataneeded require red to blink
   dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
                       DM_TRICOLORLIGHT_BLINK_1HZ,
                       DM_TRICOLORLIGHT_NORMALREQUEST);

//   ASSERT(!ra.GetRAPDataPendingCount());

//   if (!ra.GetRAPDataPendingCount())
//      {
         // Begin TAR 194764
	     //cDataEntry.FromConfigData(csDataEntryINIFile, m_sView); //now done in deliver or initialize

         CString csLaneButton = sm_pDataNeededEntry.GetSecondaryButtonText();
         if ( (m_View != DATANEEDED_MOREDATA) && (m_View < DATANEEDED_LASTVIEW))
         {
             //always use the viewname as lane button text for fastlane-defined requests
             csLaneButton = m_sViewName;
         }
         else if (csLaneButton == _T(""))    //lane button text not defined
         {
             csLaneButton = m_sViewName;
         }
         
         CString csLaneButtonText;
         csLaneButtonText.Empty();
         csLaneButtonText = sm_pDataNeededEntry.GetSecondaryButtonText();
         
         CString csLaneStateOne;
         csLaneStateOne.Empty();
         csLaneStateOne = sm_pDataNeededEntry.GetLaneStateOneText();
         if(csLaneStateOne.IsEmpty())
             csLaneStateOne.Format(ps.GetPSText(TXT_NEEDINFORMATION, SCOT_LANGUAGE_PRIMARY));
         
         CString csLaneStateTwo;
         csLaneStateTwo.Empty();
         csLaneStateTwo = sm_pDataNeededEntry.GetLaneStateTwoText();
         
         CString csLaneStateThree;
         csLaneStateThree.Empty();
         csLaneStateThree = sm_pDataNeededEntry.GetLaneStateThreeText();

	     CString csSecInfo;
		 csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONWITH_DNTYPEINFO, SCOT_LANGUAGE_PRIMARY),
			              m_sDesc,
						  csLaneButton, //TAR 233927 replaced m_sView w/m_sViewName will display Msg file Name for translation reasons
						  csLaneStateOne, 
						  csLaneStateTwo, 
						  csLaneStateThree,
                          m_View
						 );
		// + RFC 256154 - prevent it from sending two RAPDataNeeded events
		if(DATANEEDED_MULTIPACKITEM != m_View)
		{
            CString csReportCategoryName = sm_pDataNeededEntry.GetReportCategoryName();

			ra.OnRAPDataNeeded(csSecInfo, 
                               sm_pDataNeededEntry.ToString(), 
                               m_View,
                               sm_pDataNeededEntry.GetReportCategoryName());
		}
        // End TAR 194764
//      }
   return pNextState;
   }


void SMRAPDataNeededBase::SetupViewData()
   {
   switch (m_View)
      {
      case DATANEEDED_APPROVEVOIDEDITEM:
         m_sView        = _T("ApproveVoidedItem");
         m_sViewName    = ps.GetPSText(LTT_APPROVE_VOIDED_ITEM, SCOT_LANGUAGE_PRIMARY); //TAR 233927
         m_ViewText     = TXT_VOIDEDITEMAPPROVAL;
         break;
      case DATANEEDED_CONFIRMSUSPEND:
         m_sView        = _T("Suspend");
         m_sViewName    = ps.GetPSText(MSG_CONFIRMSUSPEND, SCOT_LANGUAGE_PRIMARY);   //TAR 233927
         m_ViewText     = TXT_SUSPENDCONFIRMATION;
         break;
      case DATANEEDED_PRICEPROMPT:
         m_sView        = _T("PricePrompt");
         m_sViewName    = ps.GetPSText(MSG_PRICEPRMT, SCOT_LANGUAGE_PRIMARY);       //TAR 233927
         m_ViewText     = MSG_PRICEPRMT;
         break;
     case DATANEEDED_ASSISTEDTENDER:    // Tender requiring RAP assistance.
        m_sView         = _T("AssistedTender");
        m_sViewName     = ps.GetPSText(TXT_JUSTASSISTEDTENDER, SCOT_LANGUAGE_PRIMARY);   //TAR 233927
        m_ViewText      = TXT_RAPASSISTEDTENDER;
        break;
	  case DATANEEDED_ENDORSEDOCUMENT:
         m_sView        = _T("AssistedEndorsement");
         m_sViewName    = ps.GetPSText(MSG_DATANEEDED_ENDORSEDOCUMENT, SCOT_LANGUAGE_PRIMARY);  //TAR 233927
         m_ViewText     = MSG_ENDORSEMENTNEEDED;
         break;
      case DATANEEDED_MOREDATA:         // RFC 197844
         m_sView        = _T("NeedInfo");
         m_sViewName    = ps.GetPSText(MSG_DATANEEDED_MOREDATA, SCOT_LANGUAGE_PRIMARY);         //TAR 233927
         m_ViewText     = TXT_NEEDINFORMATION;
         break;
	  case DATANEEDED_MULTIPACKITEM:  //TAR 256154
         m_sView        = _T("MultiPackItem");
         m_sViewName    = ps.GetPSText(MSG_RA_MULTIPACKITEM, SCOT_LANGUAGE_PRIMARY);       //TAR 233927
         m_ViewText     = MSG_RA_MULTIPACKITEM;
         break;
      // + TAR 352409
	  case DATANEEDED_SUSPENSION_DECLINE:
		  m_sView		= _T("ApproveSuspensionDecline");
		  m_sViewName	= ps.GetPSText(MSG_RA_CAT_SUSPENSIONDECLINED, SCOT_LANGUAGE_PRIMARY);
		  m_ViewText	= MSG_RA_CAT_SUSPENSIONDECLINED;
		 break;
	  // - TAR 352409
	  case DATANEEDED_STARTTRAININGTRANS:  // cs185088
        m_sView        = _T("StartTrainingModeTrans");
        m_sViewName    = ps.GetPSText(MSG_TM_TRANSACTION, SCOT_LANGUAGE_PRIMARY);  
        m_ViewText     = MSG_TRAININGMODE_TRANS;
        break;
	  //+SR678
	  case DATANEEDED_MAXCOUPONLIMIT:		
		m_sView         = _T("MaxCouponApproval");
		m_sViewName		= ps.GetPSText(MSG_MAXCOUPONLIMIT,  SCOT_LANGUAGE_PRIMARY);
		m_ViewText      = MSG_COUPONTOOHIGH;
		break;
	  case DATANEEDED_MAXCOUPONLIMITFINALIZATION:	
		m_sView         = _T("CouponWarningAtFinalization");
		m_sViewName		= ps.GetPSText(MSG_MAXCOUPONLIMIT, SCOT_LANGUAGE_PRIMARY);
		m_ViewText      = MSG_COUPONTOOHIGH;
		break;
	  //-SR678
     default:
         ASSERT(!m_sView.IsEmpty());
         if (m_sView.IsEmpty())
            {
            m_sView     = _T("Unknown");
            m_ViewText  = 0;
            m_sViewName = ps.GetPSText(MSG_RA_CAT_OTHER, SCOT_LANGUAGE_PRIMARY);                //TAR 233927
            trace(L0, _T("SMRAPDataNeededBase::SetupViewData() - Default case hit - 0x%08.8x is an unknown view!"));
            }
         else
            {
            trace(L6, _T("SMRAPDataNeededBase::SetupViewData() - Assuming %s is a valid view."), m_sView);
            }
      }
   }


SMStateBase *SMRAPDataNeededBase::HandleApproveVoidedItem()
   {
   if (m_sButton == _T("Approve"))
      {
	   // This is approved, we are finished.
       return createAnchorState();
      }
   else if (m_sButton == _T("Suspend"))
      {
      m_View = DATANEEDED_CONFIRMSUSPEND;
      SetupViewData();
	  //TD 04/25/02 TAR 202804
      m_sDesc = ps.GetPSText(TXT_RA_VOIDED_ITEM_SUSP,SCOT_LANGUAGE_PRIMARY);
      ra.OnRAPDataNeeded(m_sDesc, m_sView, m_View);
      //showScreen(false);
	  if(co.fStateConfirmationForSuspendingTransaction)
	  {
		  nSellState = getAnchorState();
		  setAnchorState(BES_SMSYSTEMFUNCTIONS);
		  RETURNSTATE(SmConfirmSuspendedTransaction)
	  }
	  else
	  {
		bSuspendFailure = false; //Tar 126382

		//Before suspend transaction, need void coupons and  paid cash
		if( lCoupons && co.fOperationsVoidCouponBeforeSuspend)
		  VoidAllCoupons();   // Void all coupons before suspend transaction
		// TAR 236567 Let TB handle refund cash
		//if(nDMCashInDrawer)   // void already paid cash
		  //TBEnterTender(TB_TENDER_CASH, -1*nDMCashInDrawer);
		// End TAR 236567

	  //Tar 120428
	  //  DMSayPhrase("TransactionSuspended");
		return TBSuspendTransaction();
	  }
      //return STATE_NULL;
      }
   else
      // Unexpected button command returned from this view!.
      ASSERT(FALSE);

   return createAnchorState();
   }

//Price entered at both RAP & ScotApp stored mode are handled by this method.
SMStateBase *SMRAPDataNeededBase::HandlePricePrompt()
{
	SAClearSecurityCondition();	// TAR209439 LPM071102 reset the lane lite
	//218558 remove ra code. it is done in SAClear above
	//218558 set scanner flag to re-enabled only in state that isn't handling EAS automatically
	trace(L2, _T("bEASReEnableScanner is set to true."));
	bEASReEnableScanner = true;


	if ((m_sButton == _T("cReturn"))||(co.IsImmedInterventionOn() && (m_sButton == _T("cGoBack"))))
	{
		if (io.csItemCode.GetLength()>0) //tar 183854
		{
			// Tar 120648
			SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
		}
		m_sButton = _T(""); //TD TAR 202158 04/19/02
		return setAndCreateAnchorState(BES_SCANANDBAG);     //TAR#126112 return to Scan&Bag LPM111699

	}
    setAnchorState(BES_SCANANDBAG);   //TAR 193566

	//TAR 356929 problems with extraneous decimals.  Ignore digits after second decimal.

	CString csDecimal = co.csLocaleDecimalSymbol;
	int iFind = m_sInput.Find(csDecimal);
	if (iFind == -1)
	{
		csDecimal = _T(".");
		iFind = m_sInput.Find(csDecimal);
	}


	if (iFind < m_sInput.GetLength() - 1)
	{
		int iFind2 = m_sInput.Find(csDecimal, iFind + 1);
		if (iFind2 != -1)
		{
			// Remove the decimal character (".") from the string
			m_sInput = m_sInput.Left(iFind2);
		}
	}

	int nMultiplyer = 1;  //Multiplyer to account for missing decimal digits TAR 356929

   if (iFind != -1)
   {
	   //TAR 356929  - fix for TAR 228430 not adequate.  Enhance for different decimal settings.
	   //TAR 228430 BZ 02/19/03+: Append a 0 if there is only one digit after the decimal point
	   //This prevents the price data received from RAP gets mishandled, for example, price

	   int nDecimalDigits = m_sInput.GetLength() - iFind - 1;
	   if (nDecimalDigits > co.nLocaleNoOfDigitsAfterDecimal) //cut off extra decimal digits
		   m_sInput = m_sInput.Left(iFind + co.nLocaleNoOfDigitsAfterDecimal + 1);
	   int nMissingDecDigits = co.nLocaleNoOfDigitsAfterDecimal - (m_sInput.GetLength() - iFind -1);
	   nMultiplyer = pow(10, nMissingDecDigits);
	   //if ((m_sInput.Right(m_sInput.GetLength() - iFind -1)).GetLength() == 1)
	   //		m_sInput = m_sInput + _T("0");
	   //TAR 228430 -

         // Remove the decimal character (".") from the string
	   m_sInput = m_sInput.Left(iFind) +
				       m_sInput.Right
			                   (m_sInput.GetLength() - iFind -1);
   }

	// if there is no leading ";" character, csEntryData was input at ScotApp store
	// mode & the string does NOT contain a decimal value. so convert as is
	io.lPriceEntered = _ttoi(m_sInput) * nMultiplyer;
	m_sButton = _T(""); //TD TAR 202158 04/19/02
	return TBEnterItem();
}

SMStateBase *SMRAPDataNeededBase::HandleConfirmSuspend()
   {
   // Always return to anchor state -- if RAP suspended us, we will
   // hit the logic at the end of the transaction.  Otherwise, everything
   // will continue as normal -- we don't care here.
   return createAnchorState();
   }

/////////////////////////////////////////////
/*SMStateBase  *SMRAPDataNeededBase::PSButton1(void)
{
  promptForOperator();
  return STATE_NULL;
}*/



SMStateBase *SMRAPDataNeededBase::HandleAssistedTender()
   {
   trace(L6, _T("+SMRAPDataNeededBase::HandleAssistedTender()"));
   SMStateBase *pNextState = createAnchorState();

   if (m_sButton == _T("cAssistMode"))
      {
      // Sit tight and wait for RAP to throw us into Assist Mode...
      pNextState = STATE_NULL;
      }
   else if (m_sButton != _T("cReturn"))
      {
      ASSERT(FALSE);
      trace(L0, _T("SMRAPDataNeededBase::HandleAssistedTender() -- Button data was not valid.  Returning to anchor state."));
      }

   trace(L6, _T("-SMRAPDataNeededBase::HandleAssistedTender()"));
   return pNextState;
   }

SMStateBase *SMRAPDataNeededBase::HandleAssistedEndorsement()
   {
   trace(L6, _T("+SMRAPDataNeededBase::HandleAssistedEndorsement()"));
   SMStateBase *pNextState = createAnchorState();

   if (m_sButton == _T("cAssistMode"))
      {
      // Sit tight and wait for RAP to throw us into Assist Mode...
      pNextState = STATE_NULL;
      }
   else
      {
      trace(L0, _T("SMRAPDataNeededBase::HandleAssistedEndorsement() -- Button go back pressed.  Returning to data needed again."));
      SetupRAPLocalView(m_sDesc, m_sView, m_View);
      showScreen(false);
      pNextState = STATE_NULL;
      }
   trace(L6, _T("-SMRAPDataNeededBase::HandleAssistedEndorsement()"));
   return pNextState;
   }


void SMRAPDataNeededBase::SetupRAPLocalView(PSTEXT nDescID, CString csViewDetails, int nView)
{
    m_sDesc = ps.GetPSText(nDescID);
    SetupRAPLocalView(m_sDesc, csViewDetails, nView);
}


void SMRAPDataNeededBase::SetupRAPLocalView(CString csDesc, CString csViewDetails, int nView)
{
    sm_pDataNeededEntry.FromConfigData(co.csDataEntryINIFile, m_sView);
    CString csSecInfo;
    //TAR 233927 replaced m_sView with m_sViewName
    csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO), m_sDesc, m_sViewName, _T(""), _T(""), _T(""));
    ra.OnRAPDataNeeded(csSecInfo, sm_pDataNeededEntry.ToString(), m_View);
}


SMStateBase  *SMRAPDataNeededBase::PSButtonGoBack(void) // Store Sign In
{
	// TAR196321 Added 193505 to this state LPM022202
	//+TAR193505: At EnterID screen, "go back" should be back to SMSecUnExpectedIncreaseBase screen

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
					//SR93 Biometrics support

					inputMode = SHOW_WAIT_FOR_APPROVAL;
					if (co.fStateDualLanguage)
						ps.SetLanguage(m_customerLanguage);  //TAR 196315: Set back to customerLanuguage for dual language
					showScreen(false);
					return STATE_NULL;  
				}
		}
	//-TAR193505

    promptForOperator();
	return STATE_NULL;
}

//////////////////////////////////////////////////////////////
SMStateBase *SMRAPDataNeededBase::OnWtIncreaseNotAllowed() //tar 197857
{
    m_saveDataNeededView = m_View;
	if (m_saveDataNeededView == DATANEEDED_MULTIPACKITEM)
		return STATE_NULL;
	if((m_saveDataNeededView == DATANEEDED_MOREDATA) && fItemIsWaiting)	// Tar 211054 RJF 081402
		return STATE_NULL;		// ignore weight increase if 1st item scanned during attract
	if(m_saveDataNeededView == DATANEEDED_ASSISTEDTENDER)
		return VoidingWtIncrease(BES_ASSISTEDTENDER, BES_ASSISTEDTENDER);
	else
		return VoidingWtIncrease(BES_RAPDATANEEDED, BES_RAPDATANEEDED);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMRAPDataNeededBase::OnWtDecrease() //tar 197857
{
    m_saveDataNeededView = m_View;
	if (m_saveDataNeededView == DATANEEDED_MULTIPACKITEM)
		return STATE_NULL;
	if (bIsInFinalization)   //We are at finialization stage
	{
		FinalizationWtDecrease();
		return STATE_NULL;
	}
	if(m_saveDataNeededView == DATANEEDED_ASSISTEDTENDER)
		return VoidingWtIncrease(BES_ASSISTEDTENDER, BES_ASSISTEDTENDER);
	else
		return ThresholdViolWtDecrease(BES_RAPDATANEEDED, BES_RAPDATANEEDED);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMRAPDataNeededBase::OnMatchedWt()
{
    m_saveDataNeededView = m_View;
	setAnchorState(BES_SCANANDBAG);
	return STATE_NULL;
}

bool SMRAPDataNeededBase::storeMode()
{
	return fStoreMode;	// RFC 212328
}

/**
 * Method:      HandleMaxCouponLimit
 * Description: Handles the button events 
 * Parameters:  
 * Returns:
 * Notes:       Added for SR678_New intervention if coupon exceeds new option 
 *				for monetary value limit
 */
SMStateBase *SMRAPDataNeededBase::HandleMaxCouponLimit()
{
	trace(L6, _T("+SMRAPDataNeededBase::HandleMaxCouponLimit()"));
	if (m_sButton == _T("cApproveCoupon"))	
	{
		SAClearSecurityCondition();
		
		//+TAR390208
		if( co.IsInAssistMenus() || fStateAssist )
		{	
			
			return createAnchorState();		
		}
		else
		{				
			  //if coupon sensor is configured, go to Insert Coupon screen
			  if (io.d.fCoupon
					&& (!io.d.fVoid)								
					&& IsUsingCouponSensor())
			  {
				 CREATE_AND_DISPENSE(InsertCoupon)(getAnchorState(), BES_SCANANDBAG);			
			  }
		
			  return setAndCreateAnchorState(BES_SCANANDBAG);
		}
		//-TAR390208
	}
	
	else
	{
		return CouponMaxIntervention();
	}
}


/**
 * Method:      HandleMaxCouponLimitFinalization
 * Description: Handles the button events 
 * Parameters:  
 * Returns:
 * Notes:       Added for SR678_New intervention if coupon exceeds new option     	   	    for monetary value limit
 */

SMStateBase *SMRAPDataNeededBase::HandleMaxCouponLimitFinalization()
{
	trace(L6, _T("+SMRAPDataNeededBase::HandleMaxCouponLimitFinalization()"));
	if (m_sButton == _T("cClearSecurity"))
	{
		dm.restoreTriColorLight();
		ra.OnNoRAPDataNeeded(); 
		return createAnchorState();
	}
	else
	{
		return STATE_NULL;
	}
}
