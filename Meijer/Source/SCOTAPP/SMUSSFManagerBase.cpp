//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMUSSFManagerBase.CPP
//
// TITLE: Class implementation for this state
//
// AUTHOR:	Christopher Swift
//		   
//            
//////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"                 // MS MFC always needed first
//                     // MS MFC template header
#include <afxpriv.h>                // for USES_CONVERSION
#include <afxdisp.h>

#include "Common.h"                       // MGV common includes
#include "PSIntList.h"                    // MGV common includes

#include "SMState.h"                      // MGV base state
#include "SMUSSFManager.h"    // MGV this state

/////////////////////////////////////////////////////////////////////////
// Need to include all the states to which control may be transitioned.
/////////////////////////////////////////////////////////////////////////
#include "SMFinish.h"
#include "SMScanAndBag.h"
#include "SMTakeCard.h"
#include "SMTakeReceipt.h"
#include "SMCardMisRead.h"
#include "SMContinueTrans.h"
#include "SMSmAssistMode.h"
#include "SMWaitForApproval.h"
// TAR #429050 START
#include "USSFCommon.h"
// TAR #429050 STOP
//////////////////////////////////////////
// EVENTUALLY MOVE THIS INTO H-FILE!
//////////////////////////////////////////
#ifndef STATE_USSF_ENDSCRIPT
#define STATE_USSF_ENDSCRIPT	((SMStateBase *)(998))
#endif
#ifndef STATE_USSF_GOTO
#define STATE_USSF_GOTO	((SMStateBase *)(997))
#endif
#ifndef STATE_USSF_FLUSHEVENT
#define STATE_USSF_FLUSHEVENT	((SMStateBase *)(996))
#endif
//////////////////////////////////////////
//////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LPTSTR SMUSSFManagerBase::SwipeData = NULL;
int SMUSSFManagerBase::SwipeDataLen = 0;
vector<SMUSSFManagerBase::ActivationRecord> SMUSSFManagerBase::ActivationDB;
SMUSSFManagerBase::ActivationRecord SMUSSFManagerBase::thisActivation;
//long SMUSSFManagerBase::lMasterEventSerialNumber = 0;
//bool SMUSSFManagerBase::bTransitionLock = false;
long SMUSSFManagerBase::lRecursionDepth = 0;
long SMUSSFManagerBase::lRecursionLimit = 10;
long SMUSSFManagerBase::lConfigurationCheckProgress = 0;
long SMUSSFManagerBase::lConfigurationCheckHWM = 0;
//CString SMUSSFManagerBase::csQuantity = _T("");
//CString SMUSSFManagerBase::csPrice = _T("");

const CString csUSSFDefaultContext = _T("USSFDefaults");
const CString csUSSFManagerContext = _T("USSFManager");
const CString csUSSFSelectionTimeOut = _T("USSFSelectionTimeOut");
const CString csUSSFMasterTimeOut = _T("USSFMasterTimeOut");

extern CPSX*  m_pGAPSX;          // NewUI - pointer to PSX object

#define Clear() Clear()
#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SMUSSFManagerBase")

IMPLEMENT_DYNCREATE(SMUSSFManagerBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMUSSFManagerBase::SMUSSFManagerBase()
{
  IMPLEMENT_TIMESTAMP

  SetOperationalState((SMStateBase *)this);
}

/////////////////////////////////////////////////
void SMUSSFManagerBase::UnInitialize(void)
{
  trace(L7, _T("SMUSSFManagerBase::UnInitialize()"));
  // TAR #429050 START
  USSFdisp.InvalidateDisplayElements();
  // TAR #429050 STOP
}

/////////////////////////////////////////////////
SMStateBase *SMUSSFManagerBase::Initialize(void)
{
  trace(L7, _T("SMUSSFManagerBase::Initialize(void)"));

  // Reset Timer to ensure full time-out interval as between Fastlane states.
  nTimeIdleMsec = 0;
  // TAR #429050 START
  USSFdisp.InvalidateDisplayElements();
  // TAR #429050 STOP
  CString currentContext = _T(""), pendingAction = _T("");
  SMStateBase *returnState = STATE_NULL;

  if (returnState == STATE_NULL)
  {
    returnState = PrepareContext(currentContext);
  }

  // Initial-Action
  if (returnState == STATE_NULL)
  {
    //CString initialAction = _T("");

    if (USSFutil.TestPSX())
    {
	  CString csInitialAction;
	  GETUSSFCONTEXTDATA(currentContext, _T(""), _T("InitialAction"), csInitialAction);
      returnState = RunScript(csInitialAction);
	  if (returnState == STATE_USSF_ENDSCRIPT)
	  {
		returnState = STATE_NULL;
	  }
	  else if (returnState == STATE_USSF_GOTO)
	  {
		// NOT VALID HERE!
		returnState = STATE_NULL;
	  }
    }
  }

  // Check for Actions of the form <XActioni></XActioni>.

  long lCount = 1;
  CString csTempstr;
  CString thisAction;
  bool bEndFlag = false;
  while((returnState == STATE_NULL) && (lCount < 100) && (!bEndFlag))
  {
	csTempstr.Format(_T("InitialAction%d"), lCount++);
	//GETUSSFEVENTDATA(thisControl, currentContext, altContext, csTempstr, thisAction);
	GETUSSFCONTEXTDATA(currentContext, _T(""), csTempstr, thisAction);
	if (thisAction.GetLength() > 0)
	{
	  trace(L5, _T("Performing %s=<%s>."), csTempstr, thisAction);
      returnState = RunScript(thisAction);
	  if (returnState == STATE_USSF_ENDSCRIPT)
	  {
		returnState = STATE_NULL;
		bEndFlag = true;
	  }
	  else if (returnState == STATE_USSF_GOTO)
	  {
		returnState = STATE_NULL;
		CString csTempstr;
		USSFutil.GetTransactionVariable(_T("USSF_GOTO_TARGET"), csTempstr);
		long lTemp = _ttol(csTempstr);
		if (lTemp > 0)
		{
			lCount = lTemp;
		}
		else
		{
			// Invalid Line Number; Ignore!
			trace(L6, _T("Invalid Line Target=<%s>; ignored."), csTempstr);
		}
	  }
	}
	else
	{
	  bEndFlag = true;
	}
  }

  // Do Context-Dependent Config!
  DoContextConfig(currentContext, _T("USSFDefaults"));

  // Post-Action
  if (returnState == STATE_NULL)
  {
    pendingAction = GetPendingPostAction();
    if (pendingAction != _T(""))
    {
      trace(L5, _T("POST-ACTION=<%s>."), pendingAction);
      returnState = RunScript(pendingAction);
	  if (returnState == STATE_USSF_ENDSCRIPT)
	  {
		returnState = STATE_NULL;
	  }
	  else if (returnState == STATE_USSF_GOTO)
	  {
		// NOT VALID HERE!
		returnState = STATE_NULL;
	  }
    }
    else
    {
      trace(L7, _T("No Post-Action Pending."));
    }
  }

  // Always do this while transitioning (after PostAction before InitialAction)!
  // Should save any needed data prior to transition (ie. PriorityAction).
  //READYACTION
  if (returnState == STATE_NULL)
  {
	CString csInTransition;
	GETUSSFCONTEXTDATA(currentContext, _T(""), _T("ReadyAction"), csInTransition);
    returnState = RunScript(csInTransition);
	if (returnState == STATE_USSF_ENDSCRIPT)
	{
		returnState = STATE_NULL;
	}
	else if (returnState == STATE_USSF_GOTO)
	{
		// NOT VALID HERE!
		returnState = STATE_NULL;
	}
  }

  // Check for Actions of the form <XActioni></XActioni>.

  lCount = 1;
  //CString csTempstr;
  //CString thisAction;
  bEndFlag = false;
  while((returnState == STATE_NULL) && (lCount < 100) && (!bEndFlag))
  {
	csTempstr.Format(_T("ReadyAction%d"), lCount++);
	//GETUSSFEVENTDATA(thisControl, currentContext, altContext, csTempstr, thisAction);
	GETUSSFCONTEXTDATA(currentContext, _T(""), csTempstr, thisAction);
	if (thisAction.GetLength() > 0)
	{
	  trace(L5, _T("Performing %s=<%s>."), csTempstr, thisAction);
      returnState = RunScript(thisAction);
	  if (returnState == STATE_USSF_ENDSCRIPT)
	  {
		returnState = STATE_NULL;
		bEndFlag = true;
	  }
	  else if (returnState == STATE_USSF_GOTO)
	  {
		returnState = STATE_NULL;
		CString csTempstr;
		USSFutil.GetTransactionVariable(_T("USSF_GOTO_TARGET"), csTempstr);
		long lTemp = _ttol(csTempstr);
		if (lTemp > 0)
		{
			lCount = lTemp;
		}
		else
		{
			// Invalid Line Number; Ignore!
			trace(L6, _T("Invalid Line Target=<%s>; ignored."), csTempstr);
		}
	  }
	}
	else
	{
	  bEndFlag = true;
	}
  }

  trace(L7, _T("Clearing Pending Post-Action."));
  SetPendingPostAction(_T(""));

  trace(L7, _T("Clearing Pre-Action."));
  SetPendingPreAction(_T(""));

  SetActivationState();
  SaveActivationData();

  bIgnoreUnknownAction = false;

  return returnState;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// PSX Input-Handling
/////////////////////////////////////////////////////////////////////////////////////////////////////

SMStateBase  *SMUSSFManagerBase::PSButtonGoBack(void)
{
  trace(L7, _T("SMUSSFManagerBase::PSButtonGoBack(void)"));

  SMStateBase *returnState = STATE_NULL;
  CString currentContext = _T("");

  if (!USSFutil.TestPSX()) return STATE_NULL;
  //m_pGAPSX -> GetContext(currentContext);

  //SetPendingEvent(_T("GoBack"));

  if (returnState == STATE_NULL)
  {
    // Determine next move!
    //returnState = MakeNextMove();
    returnState = ProcessEvent(_T("GoBack"));
  }

  return returnState;
}

SMStateBase  *SMUSSFManagerBase::PSButtonHelp(void)
{
  trace(L7, _T("SMUSSFManagerBase::PSButtonHelp(void)"));

  // May need some special attention here!
  return STATE(State)::PSButtonHelp();
}

//
// Event Handler for 8 PSX Customer Mode Buttons.
//
SMStateBase *SMUSSFManagerBase::PSButton1(void)
{
  trace(L7, _T("SMUSSFManagerBase::PSButton1(void)"));
  // TAR #429050 START
  CString csPendingEvent;
  long rc = USSFdisp.HandleControlButton(_T("PSButton1"), csPendingEvent);
  return ProcessEvent(csPendingEvent);
  // TAR #429050 STOP
}

SMStateBase *SMUSSFManagerBase::PSButton2(void)
{
  trace(L7, _T("SMUSSFManagerBase::PSButton2(void)"));
  // TAR #429050 START
  CString csPendingEvent;
  long rc = USSFdisp.HandleControlButton(_T("PSButton2"), csPendingEvent);
  return ProcessEvent(csPendingEvent);
  // TAR #429050 STOP
}

SMStateBase *SMUSSFManagerBase::PSButton3(void)
{
  trace(L7, _T("SMUSSFManagerBase::PSButton3(void)"));
  // TAR #429050 START
  CString csPendingEvent;
  long rc = USSFdisp.HandleControlButton(_T("PSButton3"), csPendingEvent);
  return ProcessEvent(csPendingEvent);
  // TAR #429050 STOP
}

SMStateBase *SMUSSFManagerBase::PSButton4(void)
{
  trace(L7, _T("SMUSSFManagerBase::PSButton4(void)"));
  // TAR #429050 START
  CString csPendingEvent;
  long rc = USSFdisp.HandleControlButton(_T("PSButton4"), csPendingEvent);
  return ProcessEvent(csPendingEvent);
  // TAR #429050 STOP
}

SMStateBase *SMUSSFManagerBase::PSButton5(void)
{
  trace(L7, _T("SMUSSFManagerBase::PSButton5(void)"));
  // TAR #429050 START
  CString csPendingEvent;
  long rc = USSFdisp.HandleControlButton(_T("PSButton5"), csPendingEvent);
  return ProcessEvent(csPendingEvent);
  // TAR #429050 STOP
}

SMStateBase *SMUSSFManagerBase::PSButton6(void)
{
  trace(L7,_T("SMUSSFManagerBase::PSButton6(void)"));
  // TAR #429050 START
  CString csPendingEvent;
  long rc = USSFdisp.HandleControlButton(_T("PSButton6"), csPendingEvent);
  return ProcessEvent(csPendingEvent);
  // TAR #429050 STOP
}

SMStateBase *SMUSSFManagerBase::PSButton7(void)
{
  trace(L7, _T("SMUSSFManagerBase::PSButton7(void)"));
  // TAR #429050 START
  CString csPendingEvent;
  long rc = USSFdisp.HandleControlButton(_T("PSButton7"), csPendingEvent);
  return ProcessEvent(csPendingEvent);
  // TAR #429050 STOP
}

SMStateBase *SMUSSFManagerBase::PSButton8(void)
{
  trace(L7, _T("SMUSSFManagerBase::PSButton8(void)"));
  // TAR #429050 START
  CString csPendingEvent;
  long rc = USSFdisp.HandleControlButton(_T("PSButton8"), csPendingEvent);
  return ProcessEvent(csPendingEvent);
  // TAR #429050 STOP
}

// Timeout Handling.
SMStateBase *SMUSSFManagerBase::TimedOut(void)
{
  trace(L7, _T("SMUSSFManagerBase::TimedOut(void)"));

  SMStateBase *returnState = STATE(State)::TimedOut();
  if (returnState != STATE_NULL)
  {
	trace(L0, _T("Eclipse base request to leave USSF."));
	delete returnState;
	returnState = STATE_NULL;
  }

  //SetPendingEvent(_T("TimedOut"));

  // Determine next move!
  //returnState = MakeNextMove();
  returnState = ProcessEvent(_T("TimedOut"));

  return returnState;
}

void SMUSSFManagerBase::SetChoiceID(CString data)
{
  trace(L7, _T("ChoiceID: %s."), data);

  USSFutil.SetTransactionVariable(_T("USSF_CHOICEID"), data);
}

CString SMUSSFManagerBase::GetPendingEvent(void)
{
  trace(L7, _T("SMUSSFManagerBase::GetPendingEvent(void)"));

  CString csResult = _T("");
  USSFutil.GetTransactionVariable(_T("USSF_PendingEvent"), csResult);
  return csResult;

}

void SMUSSFManagerBase::SetPendingPreAction(CString action)
{
  trace(L7, _T("SMUSSFManagerBase::SetPendingPreAction(CString %s)"), action);

  USSFutil.SetTransactionVariable(_T("USSFPendingPreAction"), action);
}

CString SMUSSFManagerBase::GetPendingPreAction(void)
{
  trace(L7, _T("SMUSSFManagerBase::GetPendingPreAction(void)"));

  CString csResult;
  USSFutil.GetTransactionVariable(_T("USSFPendingPreAction"), csResult);
  return csResult;

}

/*
void SMUSSFManagerBase::SetPendingNextStep(CString step)
{
  trace(L5, _T("SMUSSFManagerBase::SetPendingNextStep(CString step)"));

  if (!USSFutil.TestPSX()) return;
  m_pGAPSX -> SetTransactionVariable(_T("USSF_NEXTSTEP"), step);
}

CString SMUSSFManagerBase::GetPendingNextStep(void)
{
  trace(L5, _T("SMUSSFManagerBase::GetPendingNextStep(void)"));

  USES_CONVERSION;

  COleVariant vValue;
  vValue.Clear();

  if (!USSFutil.TestPSX()) return _T("");
  m_pGAPSX -> GetTransactionVariable(_T("USSF_NEXTSTEP"), vValue);
  vValue.ChangeType(VT_BSTR);
  return OLE2T(vValue.bstrVal);
}
*/

void SMUSSFManagerBase::SetPendingPostAction(CString action)
{
  trace(L7, _T("SMUSSFManagerBase::SetPendingPostAction(CStrng %s)"), action);

  USSFutil.SetTransactionVariable(_T("USSFPendingPostAction"), action);
}

CString SMUSSFManagerBase::GetPendingPostAction(void)
{
  trace(L7, _T("SMUSSFManagerBase::GetPendingPostAction(void)"));

  CString csResult;
  USSFutil.GetTransactionVariable(_T("USSFPendingPostAction"), csResult);
  return csResult;

}

SMStateBase *SMUSSFManagerBase::MakeAnchorState(CString nextState)
{
  trace(L7, _T("SMUSSFManagerBase::MakeAnchorState(CString nextState)"));

  SMStateBase *returnState = STATE_NULL;

  trace(L5,_T("Making New State for %s."), nextState);

  MAKEANCHORSTATE("STATE_RETURN", BES_STATERETURN);
  MAKEANCHORSTATE("STATE_START", BES_START);
  MAKEANCHORSTATE("ScanAndBag", BES_SCANANDBAG);
  MAKEANCHORSTATE("SelectFromList", BES_SELECTFROMLIST);
  MAKEANCHORSTATE("EnterCoupons", BES_ENTERCOUPONS);
  MAKEANCHORSTATE("EmptyState", BES_EMPTY_STATE);
  MAKEANCHORSTATE("LookupItem", BES_LOOKUPITEM);
  MAKEANCHORSTATE("Finish", BES_FINISH);
  MAKEANCHORSTATE("TakeCard", BES_TAKECARD);
  MAKEANCHORSTATE("SignCharge", BES_SIGNCHARGE);
  MAKEANCHORSTATE("SelectCardType", BES_SELECTCARDTYPE);
  MAKEANCHORSTATE("ThankYou", BES_THANKYOU);
  MAKEANCHORSTATE("SelectContainer", BES_SELECTCONTAINER);
  MAKEANCHORSTATE("HelpScreen", BES_HELPSCREEN);
  MAKEANCHORSTATE("CardMisread", BES_CARDMISREAD);
  MAKEANCHORSTATE("SelectTare", BES_SELECTTARE);
  MAKEANCHORSTATE("OutOfService", BES_OUTOFSERVICE);
  MAKEANCHORSTATE("CouponNotAllowed", BES_COUPONNOTALLOWED);
  MAKEANCHORSTATE("KeyInCode", BES_KEYINCODE);
  MAKEANCHORSTATE("RequestSig", BES_REQUESTSIG);
  MAKEANCHORSTATE("EnterQuantity", BES_ENTERQUANTITY);
  MAKEANCHORSTATE("EnterPin", BES_ENTERPIN);
  MAKEANCHORSTATE("ProduceFavorites", BES_PRODUCEFAVORITES);
  MAKEANCHORSTATE("EnterWeight", BES_ENTERWEIGHT);
  MAKEANCHORSTATE("EnterWtForPriceEmbedded", BES_ENTERWTFORPRICEEMBEDDED);
  MAKEANCHORSTATE("CrateableItem", BES_CRATEABLEITEM);
  MAKEANCHORSTATE("SmMediaStatus", BES_SMMEDIASTATUS);
  MAKEANCHORSTATE("SmSystemFunctions", BES_SMSYSTEMFUNCTIONS);
  MAKEANCHORSTATE("CouponTooHigh", BES_COUPONTOOHIGH);
  MAKEANCHORSTATE("CouponNoMatch", BES_COUPONNOMATCH);
  MAKEANCHORSTATE("InsertCoupon", BES_INSERTCOUPON);
  MAKEANCHORSTATE("EnterCouponValue", BES_ENTERCOUPONVALUE);
  MAKEANCHORSTATE("BagAndEAS", BES_BAGANDEAS);
  MAKEANCHORSTATE("SecViolation", BES_SECVIOLATION);
  MAKEANCHORSTATE("PickingUpItems", BES_PICKINGUPITEMS);
  MAKEANCHORSTATE("SecMismatchWeight", BES_SECMISMATCHWEIGHT);
  MAKEANCHORSTATE("SecUnexpectedDecrease", BES_SECUNEXPECTEDDECREASE);
  MAKEANCHORSTATE("VoidApproval", BES_VOIDAPPROVAL);
  MAKEANCHORSTATE("SecUnexpectedIncrease", BES_SECUNEXPECTEDINCREASE);
  MAKEANCHORSTATE("AcknowledgeSig", BES_ACKNOWLEDGESIG);
  MAKEANCHORSTATE("CardProcessing", BES_CARDPROCESSING);
  MAKEANCHORSTATE("ConfirmSignature", BES_CONFIRMSIGNATURE);
  MAKEANCHORSTATE("GetFoodStampState", BES_GETFOODSTAMPSTATE);
  MAKEANCHORSTATE("TakeReceipt", BES_TAKERECEIPT);
  MAKEANCHORSTATE("SetAllItemsAside", BES_SETALLITEMSASIDE);
  MAKEANCHORSTATE("VoidTransApproval", BES_VOIDTRANSAPPROVAL);
  MAKEANCHORSTATE("OutOfService2", BES_OUTOFSERVICE2);
  MAKEANCHORSTATE("HostOffline", BES_HOSTOFFLINE);
  MAKEANCHORSTATE("LaneClosed", BES_LANECLOSED);
  MAKEANCHORSTATE("SecBagBoxThreshold", BES_SECBAGBOXTHRESHOLD);
  MAKEANCHORSTATE("SecItemRemovedThreshold", BES_SECITEMREMOVEDTHRESHOLD);
  MAKEANCHORSTATE("SecNewItemEntry", BES_SECNEWITEMENTRY);
  MAKEANCHORSTATE("SecSkipBaggingThreshold", BES_SECSKIPBAGGINGTHRESHOLD);
  MAKEANCHORSTATE("VoidItem", BES_VOIDITEM);
  MAKEANCHORSTATE("VoidNoMatch", BES_VOIDNOMATCH);
  MAKEANCHORSTATE("ConfirmVoid", BES_CONFIRMVOID);
  MAKEANCHORSTATE("CashPayment", BES_CASHPAYMENT);
  MAKEANCHORSTATE("DepositCoupons", BES_DEPOSITCOUPONS);
  MAKEANCHORSTATE("DropOffCoupons", BES_DROPOFFCOUPONS);
  MAKEANCHORSTATE("TransportItem", BES_TRANSPORTITEM);
  MAKEANCHORSTATE("SecBagAreaBackup", BES_SECBAGAREABACKUP);
  MAKEANCHORSTATE("ConfirmAbort", BES_CONFIRMABORT);
  MAKEANCHORSTATE("WaitForApproval", BES_WAITFORAPPROVAL);
  MAKEANCHORSTATE("WaitForRemoteAssist", BES_WAITFORREMOTEASSIST);
  MAKEANCHORSTATE("SysmsgOpPass", BES_SYSMSGOPPASS);
  MAKEANCHORSTATE("SecurityLevel3", BES_SECURITYLEVEL3);
  MAKEANCHORSTATE("RapDataNeeded", BES_RAPDATANEEDED);
  MAKEANCHORSTATE("EnterAlphaNumericInfo", BES_ENTERALPHANUMERICINFO);
  MAKEANCHORSTATE("UndeactivatedItem", BES_UNDEACTIVATEDITEM);
  MAKEANCHORSTATE("ItemNotForSale", BES_ITEMNOTFORSALE);
  MAKEANCHORSTATE("UnknownItem", BES_UNKNOWNITEM);
  MAKEANCHORSTATE("TimeRestrictedItem", BES_TIMERESTRICTEDITEM);
  MAKEANCHORSTATE("ItemQuantityExceeded", BES_ITEMQUANTITYEXCEEDED);
  MAKEANCHORSTATE("CMDataEntry", BES_CMDATAENTRY);
  MAKEANCHORSTATE("AssistedTender", BES_ASSISTEDTENDER);
  MAKEANCHORSTATE("StoreAuthorization", BES_STOREAUTHORIZATION);
  MAKEANCHORSTATE("SMDataEntry", BES_SMDATAENTRY);
  MAKEANCHORSTATE("VoucherPayment", BES_VOUCHERPAYMENT);
  MAKEANCHORSTATE("TenderBalance", BES_TENDERBALANCE);
  MAKEANCHORSTATE("DepositGiftCard", BES_DEPOSITGIFTCARD);
  MAKEANCHORSTATE("InsertGiftCard", BES_INSERTGIFTCARD);
  MAKEANCHORSTATE("CollectGiftCard", BES_COLLECTGIFTCARD);
  MAKEANCHORSTATE("ExtensionState", BES_EXTENSIONSTATE);
  MAKEANCHORSTATE("CollectGiftCard", BES_COLLECTGIFTCARD);
  MAKEANCHORSTATE("LanguageSelect", BES_LANGUAGESELECT);
  MAKEANCHORSTATE("CustomMessage", BES_CUSTOMMESSAGE);
  MAKEANCHORSTATE("SecurityAnalysis", BES_SECURITYANALYSIS); 
  MAKEANCHORSTATE("InvalidBarcode", BES_INVALIDBARCODE); 
  MAKEANCHORSTATE("USSF", BES_UNIVERSALSSF);

  if (returnState == STATE_NULL)
  {
	//bTransitionLock = false;
    trace(L5,_T("Failed attempt to set anchor state (%s)."), nextState);
	CString csValue;
	csValue.Format(_T("Unable to Set Anchor State=<%s>."), nextState);
	ERRHANDLE(_T("StateError"), _T("Internal Error"), _T("SMUSSFManagerBase::MakeNewState()"), csValue);
  }
  else
  {
    trace(L7,_T("Success setting anchor state (%s)."), nextState);
  }

  return returnState;
}

SMStateBase *SMUSSFManagerBase::MakeNewState(CString nextState)
{
  trace(L7, _T("SMUSSFManagerBase::MakeNewState(CString nextState)"));

  SMStateBase *returnState = STATE_NULL;

  trace(L5,_T("Making New State for %s."), nextState);

  CREATENEWSTATE("STATE_RETURN", BES_STATERETURN);
  CREATENEWSTATE("STATE_START", BES_START);
  CREATENEWSTATE("ScanAndBag", BES_SCANANDBAG);
  CREATENEWSTATE("SelectFromList", BES_SELECTFROMLIST);
  CREATENEWSTATE("EnterCoupons", BES_ENTERCOUPONS);
  CREATENEWSTATE("EmptyState", BES_EMPTY_STATE);
  CREATENEWSTATE("LookupItem", BES_LOOKUPITEM);
  CREATENEWSTATE("Finish", BES_FINISH);
  CREATENEWSTATE("TakeCard", BES_TAKECARD);
  CREATENEWSTATE("SignCharge", BES_SIGNCHARGE);
  CREATENEWSTATE("SelectCardType", BES_SELECTCARDTYPE);
  CREATENEWSTATE("ThankYou", BES_THANKYOU);
  CREATENEWSTATE("SelectContainer", BES_SELECTCONTAINER);
  CREATENEWSTATE("HelpScreen", BES_HELPSCREEN);
  CREATENEWSTATE("CardMisread", BES_CARDMISREAD);
  CREATENEWSTATE("SelectTare", BES_SELECTTARE);
  CREATENEWSTATE("OutOfService", BES_OUTOFSERVICE);
  CREATENEWSTATE("CouponNotAllowed", BES_COUPONNOTALLOWED);
  CREATENEWSTATE("KeyInCode", BES_KEYINCODE);
  CREATENEWSTATE("RequestSig", BES_REQUESTSIG);
  CREATENEWSTATE("EnterQuantity", BES_ENTERQUANTITY);
  CREATENEWSTATE("EnterPin", BES_ENTERPIN);
  CREATENEWSTATE("ProduceFavorites", BES_PRODUCEFAVORITES);
  CREATENEWSTATE("EnterWeight", BES_ENTERWEIGHT);
  CREATENEWSTATE("EnterWtForPriceEmbedded", BES_ENTERWTFORPRICEEMBEDDED);
  CREATENEWSTATE("CrateableItem", BES_CRATEABLEITEM);
  CREATENEWSTATE("SmMediaStatus", BES_SMMEDIASTATUS);
  CREATENEWSTATE("SmSystemFunctions", BES_SMSYSTEMFUNCTIONS);
  CREATENEWSTATE("CouponTooHigh", BES_COUPONTOOHIGH);
  CREATENEWSTATE("CouponNoMatch", BES_COUPONNOMATCH);
  CREATENEWSTATE("InsertCoupon", BES_INSERTCOUPON);
  CREATENEWSTATE("EnterCouponValue", BES_ENTERCOUPONVALUE);
  CREATENEWSTATE("BagAndEAS", BES_BAGANDEAS);
  CREATENEWSTATE("SecViolation", BES_SECVIOLATION);
  CREATENEWSTATE("PickingUpItems", BES_PICKINGUPITEMS);
  CREATENEWSTATE("SecMismatchWeight", BES_SECMISMATCHWEIGHT);
  CREATENEWSTATE("SecUnexpectedDecrease", BES_SECUNEXPECTEDDECREASE);
  CREATENEWSTATE("VoidApproval", BES_VOIDAPPROVAL);
  CREATENEWSTATE("SecUnexpectedIncrease", BES_SECUNEXPECTEDINCREASE);
  CREATENEWSTATE("AcknowledgeSig", BES_ACKNOWLEDGESIG);
  CREATENEWSTATE("CardProcessing", BES_CARDPROCESSING);
  CREATENEWSTATE("ConfirmSignature", BES_CONFIRMSIGNATURE);
  CREATENEWSTATE("GetFoodStampState", BES_GETFOODSTAMPSTATE);
  CREATENEWSTATE("TakeReceipt", BES_TAKERECEIPT);
  CREATENEWSTATE("SetAllItemsAside", BES_SETALLITEMSASIDE);
  CREATENEWSTATE("VoidTransApproval", BES_VOIDTRANSAPPROVAL);
  CREATENEWSTATE("OutOfService2", BES_OUTOFSERVICE2);
  CREATENEWSTATE("HostOffline", BES_HOSTOFFLINE);
  CREATENEWSTATE("LaneClosed", BES_LANECLOSED);
  CREATENEWSTATE("SecBagBoxThreshold", BES_SECBAGBOXTHRESHOLD);
  CREATENEWSTATE("SecItemRemovedThreshold", BES_SECITEMREMOVEDTHRESHOLD);
  CREATENEWSTATE("SecNewItemEntry", BES_SECNEWITEMENTRY);
  CREATENEWSTATE("SecSkipBaggingThreshold", BES_SECSKIPBAGGINGTHRESHOLD);
  CREATENEWSTATE("VoidItem", BES_VOIDITEM);
  CREATENEWSTATE("VoidNoMatch", BES_VOIDNOMATCH);
  CREATENEWSTATE("ConfirmVoid", BES_CONFIRMVOID);
  CREATENEWSTATE("CashPayment", BES_CASHPAYMENT);
  CREATENEWSTATE("DepositCoupons", BES_DEPOSITCOUPONS);
  CREATENEWSTATE("DropOffCoupons", BES_DROPOFFCOUPONS);
  CREATENEWSTATE("TransportItem", BES_TRANSPORTITEM);
  CREATENEWSTATE("SecBagAreaBackup", BES_SECBAGAREABACKUP);
  CREATENEWSTATE("ConfirmAbort", BES_CONFIRMABORT);
  CREATENEWSTATE("WaitForApproval", BES_WAITFORAPPROVAL);
  CREATENEWSTATE("WaitForRemoteAssist", BES_WAITFORREMOTEASSIST);
  CREATENEWSTATE("SysmsgOpPass", BES_SYSMSGOPPASS);
  CREATENEWSTATE("SecurityLevel3", BES_SECURITYLEVEL3);
  CREATENEWSTATE("RapDataNeeded", BES_RAPDATANEEDED);
  CREATENEWSTATE("EnterAlphaNumericInfo", BES_ENTERALPHANUMERICINFO);
  CREATENEWSTATE("UndeactivatedItem", BES_UNDEACTIVATEDITEM);
  CREATENEWSTATE("ItemNotForSale", BES_ITEMNOTFORSALE);
  CREATENEWSTATE("UnknownItem", BES_UNKNOWNITEM);
  CREATENEWSTATE("TimeRestrictedItem", BES_TIMERESTRICTEDITEM);
  CREATENEWSTATE("ItemQuantityExceeded", BES_ITEMQUANTITYEXCEEDED);
  CREATENEWSTATE("CMDataEntry", BES_CMDATAENTRY);
  CREATENEWSTATE("AssistedTender", BES_ASSISTEDTENDER);
  CREATENEWSTATE("StoreAuthorization", BES_STOREAUTHORIZATION);
  CREATENEWSTATE("SMDataEntry", BES_SMDATAENTRY);
  CREATENEWSTATE("VoucherPayment", BES_VOUCHERPAYMENT);
  CREATENEWSTATE("TenderBalance", BES_TENDERBALANCE);
  CREATENEWSTATE("DepositGiftCard", BES_DEPOSITGIFTCARD);
  CREATENEWSTATE("InsertGiftCard", BES_INSERTGIFTCARD);
  CREATENEWSTATE("CollectGiftCard", BES_COLLECTGIFTCARD);
  CREATENEWSTATE("ExtensionState", BES_EXTENSIONSTATE);
  CREATENEWSTATE("CollectGiftCard", BES_COLLECTGIFTCARD);
  CREATENEWSTATE("LanguageSelect", BES_LANGUAGESELECT);
  CREATENEWSTATE("CustomMessage", BES_CUSTOMMESSAGE);
  CREATENEWSTATE("SecurityAnalysis", BES_SECURITYANALYSIS);
  CREATENEWSTATE("InvalidBarcode", BES_INVALIDBARCODE);
  CREATENEWSTATE("USSF", BES_UNIVERSALSSF);

  if (nextState.Find(_T("ContinueTrans")) >= 0)
  {
	trace(L5, _T("Returning State <ContinueTrans>."));
	RETURNSTATE(ContinueTrans);
  }

  if (returnState == STATE_NULL)
  {
	//bTransitionLock = false;
    trace(L5,_T("Failed attempt to make new state (%s)."), nextState);
	CString csValue;
	csValue.Format(_T("Unable to Make Requested State=<%s>."), nextState);
	ERRHANDLE(_T("StateError"), _T("Internal Error"), _T("SMUSSFManagerBase::MakeNewState()"), csValue);
  }
  else
  {
    trace(L7,_T("Success making new state (%s)."), nextState);
  }

  return returnState;
}

// Transition Managers
SMStateBase *SMUSSFManagerBase::ProcessEvent(CString thisEvent, CString thisContext)
{
	trace(L8,_T(">>>SMUSSFManagerBase::ProcessEvent(CString thisEvent<%s>;CString thisContext<%s>.)"), thisEvent, thisContext);
	SMStateBase *returnState = STATE_NULL;
	returnState =  ProcessEventComplete(thisEvent, thisContext);
	trace(L8,_T("<<<SMUSSFManagerBase::ProcessEvent(CString thisEvent<%s>;CString thisContext<%s>.)"), thisEvent, thisContext);
	return returnState;
}
// Partial only executes actions.
// Complete does complete transition.
SMStateBase *SMUSSFManagerBase::ProcessEventPartial(CString thisEvent, CString thisContext)
{
	//trace(L5,_T(">>>SMUSSFManagerBase::ProcessEventPartial(CString thisEvent<%s>;CString thisContext<%s>.)"), thisEvent, thisContext);

	SMStateBase *returnState = STATE_NULL;

	if (thisEvent.GetLength() <= 0)
	{// Nothing to do!
		trace(L7, _T("SMUSSFManagerBase::ProcessEventPartial() - NULL Event; Nothing to Process!"));
		return returnState;
	}

	///////////////////////////////////////////////////////////////////
	// Static Critical Section Start.
	///////////////////////////////////////////////////////////////////
	CSingleLock sl( &USSFEventCriticalSection, false );
	sl.Lock();                        // enter critical section
	trace(L7,_T(">>>SMUSSFManagerBase::ProcessEventPartial(CString thisEvent<%s>;CString thisContext<%s>.)"), thisEvent, thisContext);
	///////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////

	USSFutil.SetTransactionVariable(_T("USSF_PendingEvent"), thisEvent);
	//SetPendingEvent(thisEvent);

	// Part of ::MakeNextMove().
	USES_CONVERSION;

	CString currentContext =_T("");
	//long lCurrentEventSerialNumber = lMasterEventSerialNumber;

	if (!USSFutil.TestPSX()) return STATE_NULL;
	//Remove dependence on PS since context reported while in Remote Assist
	//will not be the actual remote context.
	//m_pGAPSX -> GetContext(currentContext);
	currentContext = GetActiveContext();

	// FIRST, Get the name of the custom data variable representing the control just activated.
	CString thisControl, tempvar;
	thisControl = GetPendingEvent();
	if (thisControl == _T(""))
	{
		trace(L5,_T("<1<SMUSSFManagerBase::ProcessEventPartial(CString thisEvent<%s>;CString thisContext<%s>.)"), thisEvent, thisContext);
		sl.Unlock();
		return returnState;
	}

	trace(L7, _T("Clearing PendingEvent."));
	if (!USSFutil.TestPSX())
	{
		trace(L5,_T("<2<SMUSSFManagerBase::ProcessEventPartial(CString thisEvent<%s>;CString thisContext<%s>.)"), thisEvent, thisContext);
		sl.Unlock();
		return returnState;
	}
	USSFutil.SetTransactionVariable(_T("USSF_PendingEvent"), _T(""));

	trace(L7,_T("Actual Context: %s / Alternate Context: %s / Pending Event: %s"), 
		currentContext, thisContext, thisControl);


	GETUSSFEVENTDATA(thisControl, currentContext, thisContext, _T("NextContext"), nextContext);
	GETUSSFEVENTDATA(thisControl, currentContext, thisContext, _T("NextState"), nextState);
	GETUSSFEVENTDATA(thisControl, currentContext, thisContext, _T("NextStep"), nextStep);
	GETUSSFEVENTDATA(thisControl, currentContext, thisContext, _T("Action"), baseAction);
	GETUSSFEVENTDATA(thisControl, currentContext, thisContext, _T("PriorityAction"), priorityAction);
	GETUSSFEVENTDATA(thisControl, currentContext, thisContext, _T("PreAction"), preAction);
	GETUSSFEVENTDATA(thisControl, currentContext, thisContext, _T("PostAction"), postAction);

	trace(L7,_T("Next Context: %s / Next State: %s / Next Step: %s"), nextContext, nextState, nextStep);
	trace(L7,_T("Base Action: %s / Priority Action: %s / Pre-Action: %s / Post-Action: %s"), baseAction, priorityAction, preAction, postAction);

	if (priorityAction == _T(""))
	{
		priorityAction = baseAction;
	}

	// If we have a priority action, do it NOW!
	CString presentContext = currentContext;
	if (priorityAction !=_T(""))
	{
		trace(L5,_T("*** PRIORITY-ACTION=<%s>."), priorityAction);

		returnState = RunScript(priorityAction);
		presentContext = GetActiveContext();
		if (returnState == STATE_USSF_ENDSCRIPT)
		{
			returnState = STATE_NULL;
		}
		else if (returnState == STATE_USSF_GOTO)
		{
			// NOT VALID HERE!
			returnState = STATE_NULL;
		}
	}
	else
	{
		trace(L7,_T("No Priority-Action Pending."));
	}

	trace(L7, _T("ProcessEventPartial() Terminates After Processing [Priority]Actions"));

	///////////////////////////////////////////////////////////////////
	// Static Critical Section End.
	///////////////////////////////////////////////////////////////////
	trace(L7,_T("<<<SMUSSFManagerBase::ProcessEventPartial(CString thisEvent<%s>;CString thisContext<%s>.)"), thisEvent, thisContext);
	sl.Unlock();
	///////////////////////////////////////////////////////////////////
	//trace(L5,_T("<<<SMUSSFManagerBase::ProcessEventPartial(CString thisEvent<%s>;CString thisContext<%s>.)"), thisEvent, thisContext);

	return returnState;
}
SMStateBase *SMUSSFManagerBase::ProcessEventComplete(CString thisEvent, CString thisContext)
{
	//trace(L5,_T(">>>SMUSSFManagerBase::ProcessEventComplete(CString thisEvent<%s>;CString thisContext<%s>.)"), thisEvent, thisContext);

	SMStateBase *returnState = STATE_NULL;

	if (thisEvent.GetLength() <= 0)
	{// Nothing to do!
		trace(L7, _T("SMUSSFManagerBase::ProcessEventComplete() - NULL Event; Nothing to Process!"));
		return returnState;
	}

	///////////////////////////////////////////////////////////////////
	// Static Critical Section Start.
	///////////////////////////////////////////////////////////////////
	CSingleLock sl( &USSFEventCriticalSection, false );
	sl.Lock();                        // enter critical section
	trace(L7,_T(">>>SMUSSFManagerBase::ProcessEventComplete(CString thisEvent<%s>;CString thisContext<%s>.)"), thisEvent, thisContext);
	///////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////

	USSFutil.SetTransactionVariable(_T("USSF_PendingEvent"), thisEvent);
	//SetPendingEvent(thisEvent);

	returnState = MakeNextMove(thisContext);

	///////////////////////////////////////////////////////////////////
	// Static Critical Section End.
	///////////////////////////////////////////////////////////////////
	trace(L7,_T("<<<SMUSSFManagerBase::ProcessEventComplete(CString thisEvent<%s>;CString thisContext<%s>.)"), thisEvent, thisContext);
	sl.Unlock();
	///////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////
	//trace(L5,_T("<<<SMUSSFManagerBase::ProcessEventComplete(CString thisEvent<%s>;CString thisContext<%s>.)"), thisEvent, thisContext);

	return returnState;
}
SMStateBase *SMUSSFManagerBase::MakeNextMove(CString altContext)
{
  trace(L7,_T("SMUSSFManagerBase::MakeNextMove(CString altContext)"));

  //if (bTransitionLock)
  //{
  //  trace(L5, _T("Transition Already in Progess; Cannot Proceed."));
  //  return STATE_NULL;
  //}

  USES_CONVERSION;

  csHookTraceBuffer.Empty(); //TAR407754

  CString csTempstr;
  CString currentContext =_T("");
  //long lCurrentEventSerialNumber = lMasterEventSerialNumber;

  if (!USSFutil.TestPSX()) return STATE_NULL;
  //Remove dependence on PS since context reported while in Remote Assist
  //will not be the actual remote context.
  //m_pGAPSX -> GetContext(currentContext);
  currentContext = GetActiveContext();

  SMStateBase *returnState = STATE_NULL;

  // FIRST, Get the name of the custom data variable representing the control just activated.
  CString thisControl, tempvar;
  thisControl = GetPendingEvent();
  if (thisControl == _T(""))
  {
	  return STATE_NULL;
  }

  trace(L7, _T("Clearing PendingEvent."));
  if (!USSFutil.TestPSX()) return returnState;

 USSFutil.SetTransactionVariable(_T("USSF_PendingEvent"), _T(""));

  trace(L7,_T("Actual Context: %s / Alternate Context: %s / Pending Event: %s"), 
    currentContext, altContext, thisControl);

  GETUSSFEVENTDATA(thisControl, currentContext, altContext, _T("NextContext"), nextContext);
  GETUSSFEVENTDATA(thisControl, currentContext, altContext, _T("NextState"), nextState);
  GETUSSFEVENTDATA(thisControl, currentContext, altContext, _T("NextStep"), nextStep);
  GETUSSFEVENTDATA(thisControl, currentContext, altContext, _T("Action"), baseAction);
  GETUSSFEVENTDATA(thisControl, currentContext, altContext, _T("PriorityAction"), priorityAction);
  GETUSSFEVENTDATA(thisControl, currentContext, altContext, _T("PreAction"), preAction);
  GETUSSFEVENTDATA(thisControl, currentContext, altContext, _T("PostAction"), postAction);

  trace(L7,_T("Next Context: %s / Next State: %s / Next Step: %s"), nextContext, nextState, nextStep);
  trace(L7,_T("Base Action: %s / Priority Action: %s / Pre-Action: %s / Post-Action: %s"), baseAction, priorityAction, preAction, postAction);

  csTempstr.Format(_T(":NC=[%s]:NS=[%s]:BA=[%s];PR=[%s]:PRE=[%s]:POST=[%s]"), nextContext, nextState, baseAction, priorityAction, preAction, postAction);
  csHookTraceBuffer += csTempstr;

  if (priorityAction == _T(""))
  {
	priorityAction = baseAction;
  }

  SetPendingPreAction(preAction);
  SetPendingPostAction(postAction);
  SetPendingNextStep(nextStep);

//#ifndef STATE_USSF_ENDSCRIPT
//#define STATE_USSF_ENDSCRIPT	((SMStateBase *)(998))
//#endif
//#ifndef STATE_USSF_GOTO
//#define STATE_USSF_GOTO	((SMStateBase *)(997))
//#endif

  // If we have a priority action, do it NOW!
  CString presentContext = currentContext;
  if (priorityAction !=_T(""))
  {
    trace(L5,_T("*** PRIORITY-ACTION=<%s>."), priorityAction);

    returnState = RunScript(priorityAction);
	presentContext = GetActiveContext();
	if (returnState == STATE_USSF_ENDSCRIPT)
	{
		returnState = STATE_NULL;
	}
	else if (returnState == STATE_USSF_GOTO)
	{
		// NOT VALID HERE!
		returnState = STATE_NULL;
	}
  }
  else
  {
    trace(L7,_T("No Priority-Action Pending."));
  }

  //if ((returnState == STATE_NULL) && (lCurrentEventSerialNumber == lMasterEventSerialNumber))
  if ((returnState == STATE_NULL))
  {
    // Pre-Action
    CString pendingAction = GetPendingPreAction();
	if (pendingAction != _T(""))
	{
	  trace(L5, _T("*** PRE-ACTION=<%s>."), pendingAction);

      returnState = RunScript(pendingAction);
	  if (returnState == STATE_USSF_ENDSCRIPT)
	  {
		returnState = STATE_NULL;
	  }
	  else if (returnState == STATE_USSF_GOTO)
	  {
		// NOT VALID HERE!
		returnState = STATE_NULL;
	  }
	}
	else
	{
      trace(L7, _T("No Pre-Action Pending."));
	}
  }

  // Check for Actions of the form <XActioni></XActioni>.

  long lCount = 1;
  //CString csTempstr;
  CString thisAction;
  bool bEndFlag = false;
  while((returnState == STATE_NULL) && (lCount < 100) && (!bEndFlag))
  {
	csTempstr.Format(_T("Action%d"), lCount++);
	GETUSSFEVENTDATA(thisControl, currentContext, altContext, csTempstr, thisAction);
	if (thisAction.GetLength() > 0)
	{
	  trace(L5, _T("Performing %s=<%s>."), csTempstr, thisAction);
	  csTempstr.Format(_T(":BA%d=[%s]"), lCount, thisAction);
	  csHookTraceBuffer += csTempstr;
      returnState = RunScript(thisAction);
	  if (returnState == STATE_USSF_ENDSCRIPT)
	  {
		returnState = STATE_NULL;
		bEndFlag = true;
	  }
	  else if (returnState == STATE_USSF_GOTO)
	  {
		returnState = STATE_NULL;
		CString csTempstr;
		USSFutil.GetTransactionVariable(_T("USSF_GOTO_TARGET"), csTempstr);
		long lTemp = _ttol(csTempstr);
		if (lTemp > 0)
		{
			lCount = lTemp;
		}
		else
		{
			// Invalid Line Number; Ignore!
			trace(L6, _T("Invalid Line Target=<%s>; ignored."), csTempstr);
		}
	  }
	}
	else
	{
	  bEndFlag = true;
	}
  }

  // If we did a priority action and it caused EITHER the state or context to change,
  // or a new event to become pending, then we MUST NOT act on the original event.
  //if ((returnState == STATE_NULL) && (lCurrentEventSerialNumber == lMasterEventSerialNumber))
  if ((returnState == STATE_NULL))
  {
    //bTransitionLock = true;

    // Do we need to change state?
    // MAKE the next state, if applicable.

	bool bDoTransition = true;
	// If nextState and nextContext are NULL, then there is NO transition.
	if ((nextState == _T("")) && (nextContext == _T("")))
	{
		trace(L7, _T("Next Context/State Not Specified; No Transition."));
		bDoTransition = false;
	}
	else if ((nextState == _T("")) && (nextContext != _T("")))
	{
		nextState = GetHostState(nextContext);
		trace(L5, _T("Next Context <%s>; Setting Next State to Host State <%s>."), nextContext, nextState);
	}

    //if ((GetActiveState() == nextState) && (pUSSF != this))
    if ((GetActiveState() == nextState)||
		((GetActiveState() == _T("SMUSSFManager")) &&
		 (nextState == _T("USSF"))))
    {
      trace(L7,_T("USSF States Unchanged; No State Change."));
      nextState = _T("");
    }

	if ((bDoTransition) && (returnState == STATE_NULL))
	{
      returnState = DoTransition(nextState, nextContext);
	}

    //bTransitionLock = false;
  }
  else
  {
    trace(L5, _T("*** Event <%s> Bypassed Due to [Priority]Action Result(s)."), thisControl);
	if (returnState == STATE_NULL)
	{
		trace(L5, _T("*** Calling MakeNextMove() Recursively."));
		if (lRecursionDepth < lRecursionLimit)	// Arbitrary Recursion Depth=10 (hard-coded).
		{
			lRecursionDepth++;
			returnState = MakeNextMove();	// Recurse
			lRecursionDepth--;
		}
		else if (lRecursionDepth == lRecursionLimit)
		{
			trace(L5, _T("*** Recursion Prohibited - Limit <%d> Reached; Error Forced."), lRecursionLimit);
		    ERRHANDLE(_T("RecursionError"), _T("Internal Error"), _T("SMUSSFManagerBase::MakeNextMove()"), _T("Recursion"));
		}
		else
		{
			trace(L5, _T("*** Recursion Prohibited - Limit <%d> Exceeded; Base Case Forced."), lRecursionLimit);
			SetPendingEvent(_T("RecursionError"));
		}
	}
	else
	{
		trace(L5, _T("*** Priority Action Did NOT Return STATE_NULL."));
	}
  }

  return returnState;
}

SMStateBase *SMUSSFManagerBase::RunScript(CString thisScript)
{
  SMStateBase *returnState = STATE_NULL;

  // Need to parse script (thisScript).
  // Allow the following grammar: {X;X;X;X;} where X represents an atomic action.
  // X => X;
  // X => {X}
  // For each X, a call to PerformAction() is made.

  trace(L5, _T("Starting Script -->%s<--"), thisScript);

  CString csAction = _T("");
  bool bDone = false, bActionTaken = false;
  long lNest = 0;

  int i = 0;
  while ((i < thisScript.GetLength()) && (bDone == false))
  {
	switch(thisScript[i])
	{
	case '{':
		lNest++;
		break;
	case '}':
		lNest--;
		break;
	case ';':
		// Process this Action.
		bActionTaken = false;

		returnState = ManageAction(csAction, bActionTaken);
		if (returnState == STATE_USSF_FALSE)
		{	// Conditional Failed, abort remainder of script.
			returnState = STATE_NULL;
			bDone = true;
		}
		else if ((returnState == STATE_NULL) && (bActionTaken == false))
		{
			trace(L5,_T("Specified Action Does Not Exist (%s)."), csAction);
			if (!bIgnoreUnknownAction)
			{
				bDone = true;
				// Throw the error.
				CString csValue;
				csValue.Format(_T("Action Does Not Exist=<%s>."), csAction);
				ERRHANDLE(_T("UnknownActionError"), _T("Internal Error"), _T("SMUSSFManagerBase::RunScript()"), csValue);
			}
		}
		else if ((returnState != STATE_NULL) && (fStateAssist == false))
		{
			bDone = true;
		}
		csAction = _T("");
		break;
	default:
		// Accumulate the Action.
		csAction += thisScript[i];
		break;
	}

	i++;
  }
  if ((bDone == false) && (csAction.GetLength() > 0))
  {
	  // Process Last Action (even without ";").
	  returnState = ManageAction(csAction, bActionTaken);
	  if (returnState == STATE_USSF_FALSE)
	  {	// Conditional Failed, abort remainder of script.
		  returnState = STATE_NULL;
		  bDone = true;
	  }
	  else if ((returnState == STATE_NULL) && (bActionTaken == false))
	  {
		  trace(L5,_T("Specified Action Does Not Exist (%s)."), csAction);
		  if (!bIgnoreUnknownAction)
		  {
			  bDone = true;
			  // Throw the error.
			  CString csValue;
			  csValue.Format(_T("Action Does Not Exist=<%s>."), csAction);
			  ERRHANDLE(_T("UnknownActionError"), _T("Internal Error"), _T("SMUSSFManagerBase::RunScript()"), csValue);
		  }
	  }
  }

  return returnState;
}

SMStateBase *SMUSSFManagerBase::ManageAction(CString thisAction, bool &bActionTaken)
{
  trace(L7,_T("SMUSSFManagerBase::ManageAction(CString thisAction, bool &bActionTaken)"));

  trace(L5, _T("*** PERFORMING ACTION=<%s>."), thisAction);

  CString csTraceStr;
  SMStateBase *returnState = STATE_NULL;

  // ParseAction() must be called from here since SMUSSFAction::PerformAction()
  // gets control BEFORE SMUSSFActionBase::PerformAction().
  long rc = ParseAction(thisAction);

  if (thisAction.GetLength() > 0)	
  {
	CString csResult;
	//Allow for Variable Reference!
	// In the event of a variable reference, call RunScript recursively!
	if (USSFutil.ResolveSymbol(thisAction, csResult) == 0)
	{
		trace(L5, _T("Calling RunScript() Recursively for <%s>."), csResult);
		returnState = RunScript(csResult);
		if (returnState == STATE_USSF_ENDSCRIPT)
		{
			returnState = STATE_NULL;
		}
		else if (returnState == STATE_USSF_GOTO)
		{
			// NOT VALID HERE!
			returnState = STATE_NULL;
		}
	}
    else
	{
      // This method is called by RunScript(). We will call SMUSSFAction::PerformAction first.
      // If no action is taken, then we will check to see if we can handle here.
      returnState = PerformAction(myCommand, bActionTaken);
	}

	if ((bActionTaken == false) && (returnState == STATE_NULL))
	{
		CString thisAction = myCommand;
		// Do the USSF system action if possible.
		// Call USSFutil.USSF_FullReset_Action(pOpState, vParms);
		try
		{
			USSF_LOCAL_ACTION(_T("+Resume"), Resume);
			USSF_LOCAL_ACTION(_T("+Mark"), Mark);
			USSF_LOCAL_ACTION(_T("+Return"), Return);
			USSF_LOCAL_ACTION(_T("+Exit"), Exit);
		}
		catch(...)
		{
			trace(L5, _T("Exception Within USSF System Action(%s)."), thisAction);
		}

	    if (bActionTaken == false)
		{
		  // Action Not Available.
		  trace(L5, _T("Requested Action (%s) Not Available."), thisAction); 
		}
	}
  }

  // Clear Parameters!
  ClearParameters();

  return returnState;
}

void SMUSSFManagerBase::ClearParameters(void)
{
	CString csActionCommand, csVariable;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

	for (int i = 1; i <= lActionParmCount; i++)
	{
		csVariable.Format(_T("ActionParm%02d"), i);
		USSFutil.SetTransactionVariable(csVariable, _T(""));
	}

	USSFutil.SetTransactionVariable(_T("ActionCommand"), _T(""));
	USSFutil.SetTransactionVariable(_T("ActionParmCount"), (long)0);

	return;
}

long SMUSSFManagerBase::ParseAction(CString thisAction)
{
  long rc = 0;

  myArgs.erase(myArgs.begin(), myArgs.end());
  myParms.RemoveAll();
  myParmsLower.RemoveAll();
  myParmsUpper.RemoveAll();
  myCommand = _T("");

  thisAction.TrimLeft();
  thisAction.TrimRight();

  USSFutil.SetTransactionVariable(_T("ActionCommand"), _T(""));
  USSFutil.SetTransactionVariable(_T("ActionParmCount"), (long)0);

  // Parse thisAction into tokens.
  CString tempstr = thisAction;
  long place = tempstr.Find(_T(" "));

  if (place >= 0)
  {
    myCommandLower = myCommandUpper = myCommand = tempstr.Left(place);
    tempstr = tempstr.Mid(place + 1);
  }
  else
  {
    myCommandLower = myCommandUpper = myCommand = tempstr;
    tempstr = _T("");
  }

  myCommandUpper.MakeUpper();
  myCommandLower.MakeLower();
  USSFutil.SetTransactionVariable(_T("ActionCommand"), myCommand);

  long i = 0;
  CString csParmName;
  place = tempstr.Find(_T(" "));
  if (place < 0)
  {
    if (tempstr.GetLength() > 0)
    {
      myParms.Add(tempstr);
	  myArgs.push_back(tempstr);
	  csParmName.Format(_T("ActionParm%02d"), ++i);
	  USSFutil.SetTransactionVariable(csParmName, tempstr);
	  //****************************
	  // Support for Upper/Lower
	  //****************************
	  CString csThisParm = tempstr;
	  csThisParm.MakeUpper();
	  myParmsUpper.Add(csThisParm);
	  csThisParm.MakeLower();
	  myParmsLower.Add(csThisParm);
	  //****************************
    }
  }
  else
  {
    while(place >= 0)
    {
      myParms.Add(tempstr.Left(place));
      myArgs.push_back(tempstr.Left(place));
	  csParmName.Format(_T("ActionParm%02d"), ++i);
	  USSFutil.SetTransactionVariable(csParmName, tempstr.Left(place));
	  //****************************
	  // Support for Upper/Lower
	  //****************************
	  CString csThisParm = tempstr.Left(place);
	  csThisParm.MakeUpper();
	  myParmsUpper.Add(csThisParm);
	  csThisParm.MakeLower();
	  myParmsLower.Add(csThisParm);
	  //****************************

      tempstr = tempstr.Mid(place + 1);
      place = tempstr.Find(_T(" "));
    }
	if (tempstr.GetLength() > 0)
	{
      myParms.Add(tempstr);
	  myArgs.push_back(tempstr);
	  csParmName.Format(_T("ActionParm%02d"), ++i);
	  USSFutil.SetTransactionVariable(csParmName, tempstr);
	  //****************************
	  // Support for Upper/Lower
	  //****************************
	  CString csThisParm = tempstr;
	  csThisParm.MakeUpper();
	  myParmsUpper.Add(csThisParm);
	  csThisParm.MakeLower();
	  myParmsLower.Add(csThisParm);
	  //****************************
	}
  }

  USSFutil.SetTransactionVariable(_T("ActionParmCount"), i);

  return rc;
}

SMStateBase *SMUSSFManagerBase::PerformAction(CString thisAction, bool &bActionTaken)
{
  trace(L7,_T("SMUSSFManagerBase::PerformAction(CString thisAction)"));

  trace(L5, _T("*** PERFORMING ACTION=<%s>."), thisAction);
  bActionTaken = false;

  CString csTraceStr;
  SMStateBase *returnState = STATE_NULL;

  typedef SMStateBase *(*ExitCall) (LPCTSTR, vector<CString>, SMStateBase *, bool&);

  // This is now called externally from SMUSSFManagerBase prior to the 
  // call to PerformAction().
  // See the call: ParseAction(thisAction).

  // FIRST, is this a User Exit Action?
  if (!thisAction.CompareNoCase(_T("Call")))
  {
    trace(L5, _T("Processing Action: %s"), _T("Call"));

    if (myArgs.size() > 0)
    {
		try
		{
			// Check for a method in USSFExit.dll
			//HMODULE hMod = LoadLibrary(_T("C:\\Scot\\bin\\USSFExit.dll"));
			if (hUSSFExit != NULL)
			{
				trace(L5, _T("%s Loaded."), csExitName);
				ExitCall fProc = (ExitCall)GetProcAddress( hUSSFExit, (LPCSTR)"CallDirector" );
				if (fProc != NULL)
				{
					returnState = fProc(myArgs[0], myArgs, pOperationalState, bActionTaken);
					if (returnState == STATE_NULL)
					{
						trace(L5, _T("%s(Initialize) Returned STATE_NULL."), csExitName);
					}
					else
					{
						trace(L5, _T("%s(Initialize) DID NOT Return STATE_NULL."), csExitName);
					}
					bActionTaken = true;
				}
			}
			else
			{
				trace(L5, _T("%s <%s> Not Loaded."), csExitName, csExitVersion);
			}
		}
		catch(...)
		{
			trace(L5, _T("Exception Within %s(CallDirector)"), csExitName);
		}
	}
	else
	{
	  trace(L5, _T("User Exit Requested (via Call) But Not Specified."));
	}

	bActionTaken = true;
  }
  else
  {	// Not a "Call", but try USSFExit as well.
    trace(L5, _T("Processing Action: %s"), thisAction);

    //if (myArgs.size() > 0)
    //{
	//
	try
	{
		// Check for a method in USSFExit.dll
		//HMODULE hMod = LoadLibrary(_T("C:\\Scot\\bin\\USSFExit.dll"));
		if (hUSSFExit != NULL)
		{
			trace(L5, _T("%s Loaded."), csExitName);
			ExitCall fProc = (ExitCall)GetProcAddress( hUSSFExit, (LPCSTR)"CallDirector" );
			if (fProc != NULL)
			{
				returnState = fProc(thisAction, myArgs, pOperationalState, bActionTaken);
				if (returnState == STATE_NULL)
				{
					trace(L5, _T("%s(Initialize) Returned STATE_NULL."), csExitName);
				}
				else
				{
					trace(L5, _T("%s(Initialize) DID NOT Return STATE_NULL."), csExitName);
				}
			}
		}
		else
		{
			//trace(L5, _T("Failed to Load USSFExit.dll"));
			trace(L5, _T("%s <%s> Not Loaded."), csExitName, csExitVersion);
		}
	}
	catch(...)
	{
		trace(L5, _T("Exception Within %s(CallDirector)"), csExitName);
	}
	//}
	//else
	//{
	//  trace(L5, _T("User Exit Requested (via Call) But Not Specified."));
	//}
  }

  // Do the action if possible.
  // Call USSFutil.USSF_FullReset_Action(pOpState, vParms);
  try
  {
	USSF_LOCAL_ACTION(_T("Alert"), Alert);
	USSF_LOCAL_ACTION(_T("AllowInput"), AllowInput);
	USSF_LOCAL_ACTION(_T("AmtDueAmtPaid"), AmtDueAmtPaid);
	USSF_LOCAL_ACTION(_T("Append"), Append);
	USSF_LOCAL_ACTION(_T("Approval"), Intervention);
	USSF_LOCAL_ACTION(_T("AssistAlert"), AssistAlert);
	USSF_LOCAL_ACTION(_T("CancelItem"), CancelItem);
	USSF_LOCAL_ACTION(_T("CancelPendingItem"), CancelItem);
	USSF_LOCAL_ACTION(_T("CancelTransaction"), CancelTransaction);
	USSF_LOCAL_ACTION(_T("CashAcceptorDisable"), CashAcceptorDisable);
	USSF_LOCAL_ACTION(_T("CashAcceptorEnable"), CashAcceptorEnable);
	USSF_LOCAL_ACTION(_T("Clear"), Clear);
	USSF_LOCAL_ACTION(_T("ClearControlButtons"), ClearControlButtons);
	USSF_LOCAL_ACTION(_T("ClearDefinition"), ClearDefinition);
	USSF_LOCAL_ACTION(_T("ClearDisplay"), ClearDisplay);
	USSF_LOCAL_ACTION(_T("ClearInput"), ClearInput);
	USSF_LOCAL_ACTION(_T("ClearItemData"), ClearItemData);
	USSF_LOCAL_ACTION(_T("ClearMetaData"), ClearMetaData);
	USSF_LOCAL_ACTION(_T("ClearTransactionData"), ClearTransactionData);
	USSF_LOCAL_ACTION(_T("ClearUSSFApproval"), ClearUSSFApproval);
	USSF_LOCAL_ACTION(_T("CoinAcceptorDisable"), CoinAcceptorDisable);
	USSF_LOCAL_ACTION(_T("CoinAcceptorEnable"), CoinAcceptorEnable);
	USSF_LOCAL_ACTION(_T("Display"), Display);
	USSF_LOCAL_ACTION(_T("DisplayKeyedElement"), DisplayKeyedElement);
	USSF_LOCAL_ACTION(_T("DisplayKeyedBuffer"), DisplayKeyedBuffer);
	USSF_LOCAL_ACTION(_T("DisplayKeyedData"), DisplayKeyedData);
	USSF_LOCAL_ACTION(_T("DisableButton"), DisableButton);
	USSF_LOCAL_ACTION(_T("DisableCashAcceptor"), CashAcceptorDisable);
	USSF_LOCAL_ACTION(_T("DisableCoinAcceptor"), CoinAcceptorDisable);
	USSF_LOCAL_ACTION(_T("DisableMSR"), DisableMSR);
	USSF_LOCAL_ACTION(_T("DisableScale"), ScaleDisable);
	USSF_LOCAL_ACTION(_T("DisableScanner"), DisableScanner);
	USSF_LOCAL_ACTION(_T("DisplayRAP"), DisplayRAP);
	USSF_LOCAL_ACTION(_T("DisplayReceipt"), DisplayReceipt);
	USSF_LOCAL_ACTION(_T("EchoAmount"), EchoAmount);
	USSF_LOCAL_ACTION(_T("EchoData"), EchoData);
	USSF_LOCAL_ACTION(_T("EchoInput"), EchoInput);
	USSF_LOCAL_ACTION(_T("EnableButton"), EnableButton);
	USSF_LOCAL_ACTION(_T("EnableCashAcceptor"), CashAcceptorEnable);
	USSF_LOCAL_ACTION(_T("EnableCoinAcceptor"), CoinAcceptorEnable);
	USSF_LOCAL_ACTION(_T("EnableMSR"), EnableMSR);
	USSF_LOCAL_ACTION(_T("EnableScale"), ScaleEnable);
	USSF_LOCAL_ACTION(_T("EnableScanner"), EnableScanner);
	USSF_LOCAL_ACTION(_T("EndScript"), EndScript);
	USSF_LOCAL_ACTION(_T("ExitAssist"), ExitAssist);
	USSF_LOCAL_ACTION(_T("FirePOSDisplay"), FireTBDisplay);
	USSF_LOCAL_ACTION(_T("FireTBDisplay"), FireTBDisplay);
	USSF_LOCAL_ACTION(_T("FormatData"), FormatData);
	USSF_LOCAL_ACTION(_T("Freeze"), FreezeState);
	USSF_LOCAL_ACTION(_T("FullReset"), FullReset);
	USSF_LOCAL_ACTION(_T("GetDiagFiles"), GetDiagFiles);
	USSF_LOCAL_ACTION(_T("GetPSXVariable"), GetPSXVariable);
	USSF_LOCAL_ACTION(_T("GetTotalDetails"), GetTotalDetails);
	USSF_LOCAL_ACTION(_T("GoAnchorState"), GoAnchorState);
	USSF_LOCAL_ACTION(_T("GoTo"), GoTo);
	USSF_LOCAL_ACTION(_T("GoFLState"), GoFLState);
	USSF_LOCAL_ACTION(_T("GoUSSF"), GoUSSF);
	USSF_LOCAL_ACTION(_T("HandleUSSFApproval"), HandleUSSFApproval);
	USSF_LOCAL_ACTION(_T("IdentifyApproval"), IdentifyApproval);
	USSF_LOCAL_ACTION(_T("IF"), IF);
	USSF_LOCAL_ACTION(_T("If"), IF);
	USSF_LOCAL_ACTION(_T("IfApprovalPending"), IfApprovalPending);
	USSF_LOCAL_ACTION(_T("IfFrankingPending"), IfFrankingPending);
	USSF_LOCAL_ACTION(_T("IfNeedApproval"), IfApprovalPending);
	USSF_LOCAL_ACTION(_T("IfTxBalance"), IfTxBalance);
	USSF_LOCAL_ACTION(_T("IfTxStart"), IfTxStart);
	USSF_LOCAL_ACTION(_T("IgnoreUnknownAction"), IgnoreUnknownAction);
	USSF_LOCAL_ACTION(_T("InitAssist"), InitAssist);
	USSF_LOCAL_ACTION(_T("Initialize"), Initialize);	// TAR #429050
	USSF_LOCAL_ACTION(_T("Intervene"), Intervention);
	USSF_LOCAL_ACTION(_T("Intervention"), Intervention);
	USSF_LOCAL_ACTION(_T("LoadDefinition"), LoadDefinition);
	USSF_LOCAL_ACTION(_T("NOP"), NOP);
	USSF_LOCAL_ACTION(_T("PartialReset"), PartialReset);
	USSF_LOCAL_ACTION(_T("PressButtonX"), PressButtonX);
	USSF_LOCAL_ACTION(_T("PressGoBack"), PressGoBack);
	USSF_LOCAL_ACTION(_T("SAWLDBScannerDisable"), SAWLDBScannerDisable);
	USSF_LOCAL_ACTION(_T("SAWLDBScannerEnable"), SAWLDBScannerEnable);
	USSF_LOCAL_ACTION(_T("SayAmount"), SayAmount);
	USSF_LOCAL_ACTION(_T("SayPhrase"), SayPhrase);
	USSF_LOCAL_ACTION(_T("SayWave"), SayWave);
	USSF_LOCAL_ACTION(_T("SaveActivationData"), SaveActivationData);	// TAR #429050
	USSF_LOCAL_ACTION(_T("ScaleDisable"), ScaleDisable);
	USSF_LOCAL_ACTION(_T("ScaleEnable"), ScaleEnable);
	USSF_LOCAL_ACTION(_T("Scroll"), Scroll);				// TAR #429050
	USSF_LOCAL_ACTION(_T("ScrollDown"), ScrollDown);			// TAR #429050
	USSF_LOCAL_ACTION(_T("ScrollList"), ScrollList);			// TAR #429050
	USSF_LOCAL_ACTION(_T("ScrollUp"), ScrollUp);				// TAR #429050
	USSF_LOCAL_ACTION(_T("SellDepartmentItem"), SellDepartmentItem);
	USSF_LOCAL_ACTION(_T("SellScannedItem"), SellScannedItem);
	USSF_LOCAL_ACTION(_T("SellKeyedItem"), SellKeyedItem);
	USSF_LOCAL_ACTION(_T("SendPOSMessage"), SendPOSMessage);
	USSF_LOCAL_ACTION(_T("SendRAPMessage"), SendRAPMessage);
	USSF_LOCAL_ACTION(_T("SetAnchorState"), SetAnchorState);
	USSF_LOCAL_ACTION(_T("SetButton"), SetButton);
	USSF_LOCAL_ACTION(_T("SetDeclineMessage"), SetDeclineMessage);
	USSF_LOCAL_ACTION(_T("SetPSXVariable"), SetPSXVariable);
	USSF_LOCAL_ACTION(_T("SetTimeOut"), SetTimeOut);
	USSF_LOCAL_ACTION(_T("ShowLeadthru"), ShowLeadthru);
	USSF_LOCAL_ACTION(_T("Show2x20"), Show2x20);
	USSF_LOCAL_ACTION(_T("Shutdown"), Shutdown);
	USSF_LOCAL_ACTION(_T("Sleep"), Sleep);
	USSF_LOCAL_ACTION(_T("SpoofPOSDisplay"), FireTBDisplay);
	USSF_LOCAL_ACTION(_T("Store"), Store);
	USSF_LOCAL_ACTION(_T("StoreSymbols"), StoreSymbols);
	USSF_LOCAL_ACTION(_T("SuspendTransaction"), SuspendTransaction);
	USSF_LOCAL_ACTION(_T("TBFinish"), TBFinish);
	USSF_LOCAL_ACTION(_T("Thaw"), UnFreezeState);
	USSF_LOCAL_ACTION(_T("Trace"), Trace);
	USSF_LOCAL_ACTION(_T("UnFreeze"), UnFreezeState);
	USSF_LOCAL_ACTION(_T("WaitForApproval"), WaitForApproval);
  }
  catch(...)
  {
	trace(L5, _T("Exception Within Base Action(%s)."), thisAction);
  }

  return returnState;
}

CString SMUSSFManagerBase::GetHostState(CString nextContext)
{
  trace(L7, _T("CString SMUSSFManagerBase::GetHostState(CString nextContext)"));

  SMStateBase *returnState = STATE_NULL;

  // Check next context to see if a host state is defined.
  CString hostState = _T("");

  if (!USSFutil.TestPSX()) return _T("");
  m_pGAPSX -> GetCustomDataVar(_T("HostState"), hostState, _T(""), nextContext);

  if (hostState == _T(""))
  {
    trace(L7, _T("HostState Variable Not Set for Next Context (%s)."), nextContext);

	m_pGAPSX -> GetCustomDataVar(_T("HostState"), hostState, _T(""), csUSSFDefaultContext);    
	if (hostState != _T(""))
	{
	  trace(L5, _T("Using Default Host State = <%s>."), hostState);
	}
  }
  else
  {
    trace(L5, _T("HostState Variable = %s for Next Context (%s)."), hostState, nextContext);
  }

  return hostState;
}

CString SMUSSFManagerBase::SetDisplayContext(CString nextContext)
{
  trace(L7, _T("CString SMUSSFManagerBase::SetDisplayContext(CString nextContext)"));

    SMStateBase *returnState = STATE_NULL;

  // Check next context to see if a host state is defined.
  CString displayContext = _T("");

  if (!USSFutil.TestPSX()) return _T("");
  m_pGAPSX -> GetCustomDataVar(_T("DisplayContext"), displayContext, _T(""), nextContext);

  if (displayContext == _T(""))
  {
    trace(L5, _T("DisplayContext Variable Not Set for Next Context (%s)."), nextContext);

	if (!USSFutil.TestPSX()) return _T("");
	m_pGAPSX -> GetCustomDataVar(_T("DisplayContext"), displayContext, _T(""), csUSSFDefaultContext);    
	if (displayContext == _T(""))
	{
		trace(L5, _T("Using Actual Context."));
		displayContext = nextContext;
	}
	else
	{
		trace(L5, _T("Using Default Display Context = <%s>."), displayContext);
	}
  }
  else
  {
    trace(L5, _T("DisplayContext Variable = %s for Next Context (%s)."), displayContext, nextContext);
  }

  USSFutil.SetTransactionVariable(_T("USSFDisplayContext"), displayContext);


  return displayContext;

}

CString SMUSSFManagerBase::GetDisplayContext(void)
{
  CString csResult;
  USSFutil.GetTransactionVariable(_T("USSFDisplayContext"), csResult);
  return csResult;

}

SMStateBase *SMUSSFManagerBase::MakePreviousMove(void)
{
  trace(L5, _T("SMUSSFManagerBase::MakePreviousMove(void)"));

  SMStateBase *returnState = STATE_NULL;
  long lCount = 0;
  vector<ActivationRecord>::iterator ActivationIterator;
  CString nextState, nextContext;

  // Find most recent Activation Record, Delete it, Transition to it and create new Activation Record.
  for(ActivationIterator = ActivationDB.end() - 1; ActivationIterator != ActivationDB.begin(); ActivationIterator--)
  {
	  if ((*ActivationIterator).bInScope)
	  {
	      (*ActivationIterator).bInScope = false;
		  if (++lCount == 2)
		  {
			  // Ok, this is the one!
			  nextState = (*ActivationIterator).CurrentState;
			  nextContext = (*ActivationIterator).CurrentContext;
			  break;
		  }
	  }
  }

  if (lCount < 2)
  {
	  // Should Not Happen, but handle anyway!
	  trace(L5, _T("Activation DB Empty Unexpectedly; returning STATE_RETURN."));
	  nextState = _T("STATE_RETURN");
  }

  // Now transition (which also takes care of activation record)!
  //if ((GetActiveState() == nextState) && (pUSSF != this))
  if (GetActiveState() == nextState)
  {
      trace(L5, _T("Same State Transition (%s); Changing Only Context (%)."), nextState, nextContext);
      nextState = _T("");
  }

  returnState = DoTransition(nextState, nextContext);

  return returnState;
}

SMStateBase *SMUSSFManagerBase::MakeReferenceMove(CString csReference)
{
  trace(L5, _T("SMUSSFManagerBase::MakeReferenceMove(CString csReference)"));

  SMStateBase *returnState = STATE_NULL;
  bool bFound = false;
  vector<ActivationRecord>::iterator ActivationIterator;
  CString nextState, nextContext;

  // Find most recent Activation Record, Delete it, Transition to it and create new Activation Record.
  for(ActivationIterator = ActivationDB.end() - 1; ActivationIterator != ActivationDB.begin(); ActivationIterator--)
  {
	  if ((*ActivationIterator).bInScope)
	  {
		  if ((*ActivationIterator).ReferenceKey == csReference)
		  {
			  bFound = true;
			  break;
		  }
	  }
  }

  if (bFound)
  { // OK, now really do it!
	for(ActivationIterator = ActivationDB.end() - 1; ActivationIterator != ActivationDB.begin(); ActivationIterator--)
	{
	  if ((*ActivationIterator).bInScope)
	  {
	      (*ActivationIterator).bInScope = false;
		  if ((*ActivationIterator).ReferenceKey == csReference)
		  {
			  // Ok, this is the one!
			  nextState = (*ActivationIterator).CurrentState;
			  nextContext = (*ActivationIterator).CurrentContext;
			  thisActivation.lLinkage = (*ActivationIterator).lSerial;
			  break;
		  }
	  }
	}
  }
  else
  {
	  // Reference NOT Found, do not transition!
	  trace(L5, _T("Not Found in Activation DB; returning STATE_NULL."));
	  nextState = _T("");
  }

  // Now transition (which also takes care of activation record)!
  //if ((GetActiveState() == nextState) && (pUSSF != this))
  if (GetActiveState() == nextState)
  {
      trace(L5, _T("Same State Transition (%s); Changing Only Context (%)."), nextState, nextContext);
      nextState = _T("");
  }

  returnState = DoTransition(nextState, nextContext);

  return returnState;
}

SMStateBase *SMUSSFManagerBase::DoTransition(CString nextState, CString nextContext)
{
  trace(L7, _T("SMUSSFManagerBase::DoTransition(CString nextState, CString nextContext)"));

  SMStateBase *returnState = STATE_NULL;

  returnState = ValidateContext(nextContext);

  if ((nextState != _T("")) && (returnState == STATE_NULL))
  {
    // Need some mechanism of dynamically creating a class from a variable.
    returnState = MakeNewState(nextState);
  }

  // Did we actually make a new state? 
  // IT IS POSSIBLE that we asked for a state that we cannot make OR
  // simply that we do not want to change states.
  if (returnState != STATE_NULL)        // YES, Do we need to switch contexts?
  {
    // Set the ATMActiveState transaction variable.
	if (nextState != _T(""))
	{
		trace(L5, _T("Setting Active State to: %s"), nextState);
		SetActiveState(nextState);
	}

    if (nextContext != _T(""))
    {
      // Normal Fastlane state change process governs the transition.

      // UnInitialize() will be called by MP prior to state change.

      // Set the ATMActiveContext transaction variable.
      trace(L5, _T("Setting Active Context to: %s"), nextContext);
      SetActiveContext(nextContext);

      // Next context will be physically set in Initialize() called by MP after state change.
    }
    //SaveActivationData();
	//bIgnoreUnknownAction = false;
  }
  else                                  // NO, Do we need to switch contexts?
  {
    if (nextContext != _T(""))
    {
      // ATM State Machine governs the transition between contexts.
      // This emulates a Fastlane state transition without physically
      // changing Fastlane states via MPProcedures.

      // UnInitialize() will be called now.
      UnInitialize();

      // Set the ATMActiveContext transaction variable.
      trace(L5, _T("Setting Active Context to: %s"), nextContext);
      SetActiveContext(nextContext);

      //SaveActivationData();

      // Next context will be physically set in Initialize() called below.
      Initialize();
    }
  }

  return returnState;
}

SMStateBase *SMUSSFManagerBase::PrepareContext(CString &currentContext)
{
  trace(L7, _T("SMUSSFManagerBase::PrepareContext(CString &currentContext)"));

  SMStateBase *returnState = STATE_NULL;

  // Get Context.
  currentContext = GetActiveContext();
  if (currentContext == _T(""))
  {
	//SetPendingEvent(_T("Initialize"));
	//return MakeNextMove();
	return ProcessEvent(_T("Initialize"));
  }

  CString displayContext = GetDisplayContext();

  if (displayContext == _T(""))
  {
    // Internal Issue; Re-Start.
    trace(L5, _T("Context Undefined; Re-Initializing."));
    returnState = MakeNewState(_T("USSF"));
  }
  else
  {
    // Set Appropriate Context.
	if (!displayContext.CompareNoCase(_T("NONE")))
	{
		trace(L5, _T("Display Context Set to NONE; Context Unchanged."));
	}
	else
	{
		trace(L5, _T("Setting Display Context to %s."), displayContext);
		ps.SetUSSFFrame(displayContext);
	}

    // Set the Time-Out, if specified.
    long tempnum = GetTimeOut(currentContext);
    if (tempnum >= 0)
    {
      nTimeOut = tempnum;
    }
  }

  return returnState;
}

SMStateBase *SMUSSFManagerBase::ShowContext(void)
{
  trace(L7, _T("SMUSSFManagerBase::ShowContext(void)"));

  SMStateBase *returnState = STATE_NULL;
  CString pendingAction;

  ps.ShowFrame();

  return returnState;
} 

void SMUSSFManagerBase::SetActiveState(CString thisState)
{
  USSFutil.SetTransactionVariable(_T("USSFActiveState"), thisState);
}

void SMUSSFManagerBase::SetActiveContext(CString thisContext)
{
  USSFutil.SetTransactionVariable(_T("USSFActiveContext"), thisContext);

  SetDisplayContext(thisContext);

}

long SMUSSFManagerBase::GetTimeOut(CString context)
{
  trace(L7, _T("SMUSSFManagerBase::GetTimeOut(CString)"));

  long TimeOut = -1;
  CString tempstr;

  if (!USSFutil.TestPSX()) return 0;
  m_pGAPSX -> GetCustomDataVar(_T("TimeOut"), tempstr, _T(""), context);

  if (tempstr != _T(""))
  {
    TimeOut = _ttol(tempstr);
    trace(L5, _T("Configured TimeOut for %s: %ld"), context, TimeOut);
  }
  else
  {
    trace(L5, _T("NO Configured TimeOut for : %s."), context);
  }

  return TimeOut;
}

void SMUSSFManagerBase::GetActivationStatus(CString &state, CString &context)
{
  trace(L5, _T("SMUSSFManagerBase::GetActivationContext(CString &state, CString &context)"));

  state = thisActivation.CurrentState;
  context = thisActivation.CurrentContext;
}

SMStateBase *SMUSSFManagerBase::Resume(void)
{
  SMStateBase *returnState = STATE_NULL;

  CString csNextState = thisActivation.CurrentState;
  CString csNextContext = thisActivation.CurrentContext;

  returnState = DoTransition(csNextState, csNextContext);

  return returnState;
}

SMStateBase *SMUSSFManagerBase::ConfigurationCheck(long lProgress)
{
  trace(L5, _T("SMUSSFManagerBase::ConfigurationCheck()"));

#define MAX_CONFIGURATION_CHECKS 16

  if (lProgress >= 0)
  {
    lConfigurationCheckProgress = (lProgress == 0)?1:lProgress;
    lConfigurationCheckHWM = 0;
  }

  SMStateBase *nextState = STATE_NULL;
  long rc = 0;
  // At Startup, will help to identify customer XML (DisplayShop.xml) trouble spots.
  // ConfigurationWarningX events will be thrown as defined below.

  while(lConfigurationCheckProgress <= MAX_CONFIGURATION_CHECKS)
  {
    switch(lConfigurationCheckProgress)
    {
      // Check that there are base types.
      // ConfigurationWarning5 if none.
    case 1:
      lConfigurationCheckProgress++;
      ERRHANDLE(_T("ConfigurationWarningLevel0"), _T("Starting Configuration Check"), _T("SMUSSFManagerBase::ConfigurationCheck()"), _T("Level 1"));
      lConfigurationCheckHWM = 0;
      break;
    case 2:
      {
        //rc = myDisplayShop.TraverseDisplayShop(ACTION_LIST_BASETYPES);
        //CStringArray& csaQueryResult = myDisplayShop.GetQueryResults();
        lConfigurationCheckProgress++;
        //if (csaQueryResult.GetSize() == 0)
        //{
        //  lConfigurationCheckProgress += 2;
        //  lConfigurationCheckHWM = (lConfigurationCheckHWM < 5) ? 5 : lConfigurationCheckHWM;
        //  ERRHANDLE(_T("ConfigurationWarningLevel5"), _T("No Base Types Specified"), _T("SMUSSFManagerBase::ConfigurationCheck()"), _T("Level 5"));
        //}
      }
      break;

    case 15:
      lConfigurationCheckProgress++;
      ERRHANDLE(_T("ConfigurationCheckComplete"), _T("Configuration Check Complete"), _T("SMUSSFManagerBase::ConfigurationCheck()"), _T("Complete"));
      break;

    case 16:
      {
        switch(lConfigurationCheckHWM)
        {
        case 0:
          ERRHANDLE(_T("ConfigurationCheckSuccess"), _T("Configuration Check Success"), _T("SMUSSFManagerBase::ConfigurationCheck()"), _T("Success"));
          break;
        case 1:
          ERRHANDLE(_T("ConfigurationCheckResult1"), _T("Configuration Check Result 1"), _T("SMUSSFManagerBase::ConfigurationCheck()"), _T("Result1"));
          break;
        case 2:
          ERRHANDLE(_T("ConfigurationCheckResult2"), _T("Configuration Check Result 2"), _T("SMUSSFManagerBase::ConfigurationCheck()"), _T("Result2"));
          break;
        case 3:
          ERRHANDLE(_T("ConfigurationCheckResult3"), _T("Configuration Check Result 3"), _T("SMUSSFManagerBase::ConfigurationCheck()"), _T("Result3"));
          break;
        case 4:
          ERRHANDLE(_T("ConfigurationCheckResult4"), _T("Configuration Check Result 4"), _T("SMUSSFManagerBase::ConfigurationCheck()"), _T("Result4"));
          break;
        case 5:
          ERRHANDLE(_T("ConfigurationCheckResult5"), _T("Configuration Check Result 5"), _T("SMUSSFManagerBase::ConfigurationCheck()"), _T("Result5"));

          break;
        }
      }
      break;

	default:
	  lConfigurationCheckProgress++;
    }
  }

  return nextState;
}

SMStateBase *SMUSSFManagerBase::ShowDetails(CString currentContext)
{
  trace(L7, _T("SMUSSFManagerBase::ShowDetails(CString)"));
  SMStateBase *returnState = STATE_NULL;
  long rc = 0;
  COleVariant v;

  if (!USSFutil.TestPSX()) return STATE_NULL;

  if (!csDisplayType.CompareNoCase(_T("static")))
  {
	return STATE_NULL;
  }

  v = csTitle;
  m_pGAPSX -> SetConfigProperty(_T("Title1"), currentContext, UI::PROPERTYTEXTFORMAT, v);
  v = csTitle2;
  m_pGAPSX -> SetConfigProperty(_T("Title2"), currentContext, UI::PROPERTYTEXTFORMAT, v);
  v = csLeadthru;
  m_pGAPSX -> SetConfigProperty(_T("LeadthruText"), currentContext, UI::PROPERTYTEXTFORMAT, v);

  v = VARIANT_FALSE;
  m_pGAPSX->SetConfigProperty(_T("InstructionTextArea"), currentContext, UI::PROPERTYVISIBLE, v);
  // Is there any instruction text to display?
  if (csInstructionText != _T(""))
  {
	csInstructionText = ps.ConvertToEscapeSequence(csInstructionText, _T("\\n"), _T("\n"));

	// Show TEXT.
	v = csInstructionText;
	m_pGAPSX -> SetConfigProperty(_T("InstructionTextArea"), currentContext, UI::PROPERTYTEXTFORMAT, v);

	v = VARIANT_TRUE;
	m_pGAPSX->SetConfigProperty(_T("InstructionTextArea"), currentContext, UI::PROPERTYVISIBLE, v);
  }

  v = VARIANT_FALSE;
  m_pGAPSX->SetConfigProperty(_T("HelpTextArea"), currentContext, UI::PROPERTYVISIBLE, v);
  m_pGAPSX->SetConfigProperty(_T("HelpVideo"), currentContext, UI::PROPERTYVISIBLE, v);
  m_pGAPSX->SetConfigProperty(_T("HelpImage"), currentContext, UI::PROPERTYVISIBLE, v);
  if (csHelpText != _T(""))
  {
	csHelpText = ps.ConvertToEscapeSequence(csHelpText, _T("\\n"), _T("\n"));
	// Show TEXT.
	v = csHelpText;
	m_pGAPSX -> SetConfigProperty(_T("HelpTextArea"), currentContext, UI::PROPERTYTEXTFORMAT, v);

	v = VARIANT_TRUE;
	m_pGAPSX->SetConfigProperty(_T("HelpTextArea"), currentContext, UI::PROPERTYVISIBLE, v);
  }
  else if (csHelpImage != _T(""))
  {
	// Show IMAGE.
	v = csHelpImage;
	m_pGAPSX->SetConfigProperty(_T("HelpImage"), currentContext, UI::PROPERTYPICTURE, v);
	v = VARIANT_TRUE;
	m_pGAPSX->SetConfigProperty(_T("HelpImage"), currentContext, UI::PROPERTYVISIBLE, v);
  }
  else if (csHelpAVI != _T(""))
  {
	// Show AVI.
	v = csHelpAVI;
	m_pGAPSX->SetConfigProperty(_T("HelpVideo"), currentContext, UI::PROPERTYVIDEO, v);
	v = VARIANT_TRUE;
	m_pGAPSX->SetConfigProperty(_T("HelpVideo"), currentContext, UI::PROPERTYVISIBLE, v);
  }

  csPOSDisplay.MakeUpper();
  // Need to set the property to visible. If the control is on the context,
  // it will then display.
  v= VARIANT_TRUE;
  if (csPOSDisplay == _T("FALSE"))
  {
	v = VARIANT_FALSE;
  }
  // Either make ECHO control visible or invisible.
  m_pGAPSX -> SetConfigProperty(_T("Echo"), currentContext, UI::PROPERTYVISIBLE, v);

  if (csDisplayType.CompareNoCase(_T("dynamic")))
  {
	return STATE_NULL;
  }

  // TAR #429050 START
  rc = USSFdisp.InitializeDisplayElements(currentContext, csDisplayGroup);
  // TAR #429050 STOP

  return returnState;
}

void SMUSSFManagerBase::DoContextConfig(CString currentContext, CString defaultContext)
{
  trace(L7, _T("SMUSSFManagerBase::DoContextConfig(<%s>,<%s>)"), currentContext, defaultContext);

  // PSX Custom Data.
  // FIRST get the default values, if specified.
  trace(L5, _T("SMUSSFManagerBase::DoContextConfig(CString, CString)"));
  trace(L5, _T("Configuring Context = <%s>."), currentContext);
  trace(L5, _T("Default Context = <%s>."), defaultContext);
  CString csTempstr;

  // TAR #429050 START
  GETUSSFCONTEXTDATA(currentContext, _T(""), _T("DisplayGroup"), csDisplayGroup);
  // TAR #429050 STOP
  GETUSSFCONTEXTDATA(currentContext, _T(""), _T("DisplayType"), csDisplayType);
  GETUSSFCONTEXTDATA(currentContext, _T(""), _T("HelpAVI"), csHelpAVI);
  GETUSSFCONTEXTDATA(currentContext, _T(""), _T("HelpImage"), csHelpImage);
  GETUSSFCONTEXTDATA(currentContext, _T(""), _T("DataEntryGroup"), csDataEntryGroup);
  GETUSSFCONTEXTDATA(currentContext, _T(""), _T("DataEntryBuffer"), csDataEntryBuffer);
  GETUSSFCONTEXTDATA(currentContext, _T(""), _T("Scanner"), csScanner);
  csScanner.MakeUpper();

  if (csScanner == _T("TRUE"))
  {
	  DMScannerEnable();
  }
  else if (csScanner == _T("FALSE"))
  {
	  DMScannerDisable();
  }

  GETUSSFCONTEXTDATA(currentContext, _T(""), _T("Scale"), csScale);
  csScale.MakeUpper();

  if (csScale == _T("TRUE"))
  {
	  DMScaleEnable();
  }
  else if (csScale == _T("FALSE"))
  {
	  DMScaleDisable();
  }

  GETUSSFCONTEXTDATA(currentContext, _T(""), _T("Title"), csTitle);
  GETUSSFCONTEXTDATA(currentContext, _T(""), _T("Title2"), csTitle2);
  GETUSSFCONTEXTDATA(currentContext, _T(""), _T("Leadthru"), csLeadthru);
  GETUSSFCONTEXTDATA(currentContext, _T(""), _T("POSDisplay"), csPOSDisplay);
  GETUSSFCONTEXTDATA(currentContext, _T(""), _T("HelpText"), csHelpText);
  GETUSSFCONTEXTDATA(currentContext, _T(""), _T("InstructionText"), csInstructionText);
}

SMStateBase *SMUSSFManagerBase::TBIdleTimer(void)
{
	CString thisEvent = GetPendingEvent();
	if (thisEvent != _T(""))
	{
	  trace(L5, _T("Processing Asynchronous Event=<%s>."), thisEvent);
	  //return MakeNextMove();
	  return ProcessEvent(thisEvent);
	}
	else
	{
	  // Performance Issue - re-visit.
	  //myDisplayShop.ControlManager.Refresh();
	  return STATE(State)::TBIdleTimer();
	}
}

SMStateBase *SMUSSFManagerBase::DMScanner(void)
{
  trace(L5, _T("SMUSSFManagerBase::DMScanner(void)"));
  CString csTempstr;

  SMStateBase *returnState = STATE_NULL;
  CString currentContext = _T("");
  m_pGAPSX -> GetContext(currentContext);

  csItemSent = csDMLastBarCode.Mid(1);  // TTW533
  USSFutil.SetMetaData(csItemSent);

  csTempstr.Format(_T("Scanned Data = <%s>"), csItemSent);
  trace(L5, csTempstr);

  //SetPendingEvent(_T("GoodScan"));

  if (returnState == STATE_NULL)
  {
    // Determine next move!
    //returnState = MakeNextMove();
    returnState = ProcessEvent(_T("GoodScan"));
  }

  return returnState;
}

//***************************************************************
SMStateBase* SMUSSFManagerBase::DMScale()
{
  trace(L5, _T("SMUSSFManagerBase::DMScale()"));
  CString csTempstr;

  // let the base prime the 'virtual' scale
  SMStateBase *returnState = STATE_NULL;
  SMStateBase* sExit = STATE(State)::DMScale();

  if (sExit!=STATE_NULL)
    return sExit;
  
  // TTW1515
  lCurrentWeight = lDMScaleWeight; // weight 1/1000ths from scale
  if (!co.fOperationsScaleMetric) // if not metric
    lCurrentWeight /= 10;        // convert 1/1000ths to 1/100ths in USA
  
  if (lCurrentWeight == 0)
    return STATE_NULL;	// shakey scale, wait for good weight
  
  //SetPendingEvent(_T("GoodWeight"));
  csTempstr.Format(_T("Weight Data = <%d>"), lCurrentWeight);
  trace(L5, csTempstr);

  if (returnState == STATE_NULL)
  {
    // Determine next move!
    //returnState = MakeNextMove();
    returnState = ProcessEvent(_T("GoodWeight"));
  }

  return STATE_NULL;
}


bool SMUSSFManagerBase::PSRemoteLegalForState()
{
	return true;
}

SMStateBase *SMUSSFManagerBase::PSReceiptUp(void)
{
  trace(L5, _T("SMUSSFManagerBase::PSReceiptUp(void)"));

  // TAR #429050 START
  return STATE(State)::PSReceiptUp();
  // TAR #429050 STOP
}

SMStateBase *SMUSSFManagerBase::PSReceiptDown(void)
{
  trace(L5, _T("SMUSSFManagerBase::PSReceiptDown(void)"));

  // TAR #429050 START
  return STATE(State)::PSReceiptDown();
  // TAR #429050 STOP
}

SMStateBase *SMUSSFManagerBase::PSKeystroke(const long wParam, const long lParam)
{
  trace(L5, _T("SMUSSFManagerBase::PSKeystroke(const long wParam, const long lParam)"));

  USES_CONVERSION;

  // First, Discover the Current Context.
  CString currentContext = _T("");
  long rc = 0;

  if (!USSFutil.TestPSX()) return STATE_NULL;
  m_pGAPSX -> GetContext(currentContext);

  SMStateBase *returnState = STATE_NULL;
  
  //Need to for key ID from wParam and lParam!
  CString csID = _T("KEY");
  long lKeycode = (lParam * 1000) + wParam;
  csID.Format(_T("KEY%04d"), lKeycode);

  // Need to get the PendingEvent/POSData from the DisplayShop Object.
  //csSelectGroup/csControlGroup.
  CString csPendingEvent = _T(""), csData = _T("");
  // TAR #429050 START
  rc = USSFdisp.GetButtonInfo(csDataEntryGroup, csID, csPendingEvent, csData);
  // TAR #429050 STOP
  if (rc)
  {
	  //SetPendingEvent(csPendingEvent);
	  USSFutil.SetMetaData(csData);
	  //returnState = MakeNextMove();
	  returnState = ProcessEvent(csPendingEvent);
  }
  else
  {
	  returnState = STATE(State)::PSKeystroke(wParam, lParam);
  }

  return returnState;
}

/////////////////////////////////////////////////
SMStateBase *SMUSSFManagerBase::PSParse(const MessageElement *me)
{
  SMStateBase *nextState;
  // save all the PS parameters
  nPSLastEvt = (PSEVT)me->psInfo.nEvt;
  nPSLastParm = me->psInfo.lParm;
  CString csTabValue;
  static unsigned long lLastKeystroke = 0;
  static unsigned long lLastKeystrokeTime = 0;
  static unsigned long lEventLogged = 0;
  static unsigned long lKeystrokeCount = 0;

  trace(L5,_T("Parse PS %d, wparam:0x%x, lparam:0x%x"),nPSLastEvt,me->psInfo.wParm, nPSLastParm);

  if (nPSLastEvt == PS_UKEVT)
  { // Must be the Button List!

    // Need to reset the shot (timeout) clock!
    nTimeIdleMsec = 0;

    // We need to handle.
	_TCHAR buf[50];
	_itot(me->psInfo.wParm,buf,10);

	// Update USSF_PSXLISTINDEX
	USSFutil.SetTransactionVariable(_T("USSF_PSXLISTINDEX"), buf);

	nextState = PSListLookup(buf);
    return nextState;
  }
  else
  {
      return STATE(State)::PSParse(me);
  }
}

SMStateBase *SMUSSFManagerBase::PSListLookup(CString csButtonIndex)
{
  SMStateBase *returnState = STATE_NULL;
  CString csID, tempstr, csPendingEvent;
  long rc = 0;

  // TAR #429050 START
  rc = USSFdisp.HandleListSelection(csButtonIndex, csPendingEvent);
  return ProcessEvent(csPendingEvent);
  // TAR #429050 STOP
}

//*********************************************
TBSTATE SMUSSFManagerBase::TBProcessMessage(MessageElement* me)
{ 
    TBSTATE eState = STATE(State)::TBProcessMessage(me);
  
  if ((eState == TB_ITEMSOLD) ||
    (eState == TB_ITEMUNKNOWN) ||
    (eState == TB_COUPONNOTMATCH) ||
    (eState == TB_VOIDITEMBEFORECOUPON) ||  // TTW512
    (eState == TB_VOIDNOTMATCH) ||    // TTW533
    (eState == TB_LOYALTYCARD))
  {
    if ((eState == TB_ITEMUNKNOWN) && lastItemSold.IsEmpty())
    {
      // Got item unknown but have not sold an item
      // most likely we are returning from a Bxxx message
      // and the display is the last item sold
      // ignore this so core does not try an item cancel in security
      eState = TB_IGNORE;
    }
    
    bSecurityWaitingForItemSold = false;
    lastItemSold = "";
  }
  
  //A TTW860
  if (eState == TB_ITEMRECALLED) 
  {
    bSecurityWaitingForItemSold = false;
    lastItemSold = "";
  }
  //E TTW860
  
  // The core app upon seeing these states will try to get the item details
  // from TB.   TB will then clear his memory of the item.  Normally the
  // core app passes the item code and new QTY/WEIGHT when it resells the item.
  // Assist mode however, does not keep track of item codes so that info
  // is lost when TB sees the item finally sold
  //
  // So, instead if we see any of these state we will ignore them.
  //
  // There is no automatic processing that should be done by the core anyway
  // until the item is actually sold anyway.
  if ( (eState == TB_NEEDQUANTITY) 
    || (eState == TB_ITEMQTYEXCEEDED)
    || (eState == TB_NEEDWEIGHT)
    || (eState == TB_NEEDPRICE) )
  {
    nTBLastRealState = eState;  // set core variable for future reference
    eState = TB_IGNORE;         // but tell core to ignore this state change

    // update POS scale if scale weight has changed
    if ((lDMScaleWeight > 0) && (fDMScaleChanged))
    {
      DMScale();
    }
  }
  
  return eState;
}

//**************************************
SMStateBase* SMUSSFManagerBase::TBParse(MessageElement* me)
{
	// lUnknownItems Should NOT Change!
	long lTempUnknownItemsCount = lUnknownItems;
	long lTempNotForSaleItemsCount = lNotForSaleItems;
    SMStateBase* returnState = STATE(State)::TBParse(me);
	lUnknownItems = lTempUnknownItemsCount;
	lNotForSaleItems = lTempNotForSaleItemsCount;

	return returnState;
}

long SMUSSFManagerBase::isHandled(CString currentEvent, CString altContext, bool bGAMode)
{
	trace(L7, _T("SMUSSFManagerBase::IsHandled(CString currentEvent <%s>, CString altContext <%s>)"), currentEvent, altContext);
	long rc = 0, lType = 0;	// Not Handled.
	CString csTraceLine, csModeString;

	if (bGAMode)
	{
		csModeString = _T("GAMode");
	}
	else
	{
		csModeString = _T("FLMode");
	}

	// Need to inspect custom data for the active context.
	// Looking for specified event.
	// 
	// Presence of any:
	//		<x>PriorityAction
	//		<x>PreAction
	//		<x>PostAction
	//		<x>NextState
	//		<x>NextContext
	//		
	// Means that the event is handled (in some way).
	//
	// Presence of:
	//		<x>USSFHookOrder
	//
	// Determines the order in which actions/transitions associated with this event
	// will execute when fired as an USSFHOOK. The following element values are supported:
	//
	// Before (rc = 1); DEFAULT
	// After (rc = 2);
	// Instead (rc = 3);

	// If this is Native Mode versus GA Mode, then set AltContext to the value of the state name
	// for all scenarios (StateEntry and StateExit and all other hooks).
	//if (!isGAMode())
	//{
//		CString csStateName;
//		bool bRC = getGADefaultContext(csStateName);
//		altContext = csStateName;
	//}


	if (!USSFutil.TestPSX()) return 0;

	CString currentContext = GetActiveContext();
	CString tempvar, csTempstr;

	//trace(L5, _T("Current Context = <%s>."), currentContext);
	GETUSSFEVENTDATA(currentEvent, currentContext, altContext, _T("NextContext"), nextContext);
	GETUSSFEVENTDATA(currentEvent, currentContext, altContext, _T("NextState"), nextState);
	GETUSSFEVENTDATA(currentEvent, currentContext, altContext, _T("Action"), baseAction);
	GETUSSFEVENTDATA(currentEvent, currentContext, altContext, _T("PriorityAction"), priorityAction);
	GETUSSFEVENTDATA(currentEvent, currentContext, altContext, _T("PreAction"), preAction);
	GETUSSFEVENTDATA(currentEvent, currentContext, altContext, _T("PostAction"), postAction);
	GETUSSFEVENTDATA(currentEvent, currentContext, altContext, _T("USSFHookOrder"), hookOrderEvent);
	CString hookOrder;
	GETUSSFCONTEXTDATA(currentContext, altContext, _T("USSFHookOrder"), hookOrder);

	if (altContext ==_T(""))
	{
		csTraceLine = _T("-->>USSFHOOK:") + csModeString + _T(":") + currentContext + _T(":") + currentEvent + _T(":");
	}
	else
	{
		csTraceLine = _T("-->>USSFHOOK:") + csModeString + _T(":") + altContext + _T(":") + currentEvent + _T(":");
	}

	if (hookOrderEvent != _T(""))
	{
		hookOrder = hookOrderEvent;
	}

	// Now check for the USSFHOOK Order.

	hookOrder.MakeUpper();
	trace(L7, _T("Hook Order = <%s>."), hookOrder);
	if (hookOrder == _T("BEFORE"))
	{
		csTraceLine += _T("BEFORE:");
		lType = 1;
	}
	else if (hookOrder == _T("AFTER"))
	{
		csTraceLine += _T("AFTER:");
		lType = 2;
	}
	else if (hookOrder == _T("INSTEAD"))
	{
		csTraceLine += _T("INSTEAD:");
		lType = 3;
	}
	else
	{
		csTraceLine += _T("DEFAULT:");
		lType = 1;	// Default.
	}
	
	// Is the event handled?
	if ((nextContext.GetLength() > 0)||
		(nextState.GetLength() > 0)||
		(baseAction.GetLength() > 0)||
		(priorityAction.GetLength() > 0)||
		(preAction.GetLength() > 0)||
		(postAction.GetLength() > 0))
	{
		// Log it!
		csTempstr.Format(_T("NC=<%s>:NS=<%s>:BA=<%s>;PR=<%s>:PRE=<%s>:POST=<%s>."), nextContext, nextState, baseAction, priorityAction, preAction, postAction);
		csTraceLine += csTempstr;
	}
	else
	{
		//csTraceLine += _T("UNHANDLED");
		// Last Chance - Check for <XActioni> entries.
		long lCount = 1;
		CString csTempstr, csKey;
		CString thisAction;
		bool bEndFlag = false;
		while((lCount < 100) && (!bEndFlag))
		{
			csKey.Format(_T("Action%d"), lCount++);
			GETUSSFEVENTDATA(currentEvent, currentContext, altContext, csKey, thisAction);
			if (thisAction.GetLength() > 0)
			{
				bEndFlag = true;
				csTempstr.Format(_T("AC%d=<%s>"), lCount - 1, thisAction);
				csTraceLine += (csTempstr + _T(":"));
			}
			else
			{
				lType = 0;		// Truly Unhandled!
				bEndFlag = true;
				csTraceLine += _T("UNHANDLED");
			}
		}
	}

	trace(L7, csTraceLine);
	rc = lType;
	trace(L7, _T("isHandled() Returns <%d>."), rc);

	return rc;
}

// Local Actions
SMStateBase *SMUSSFManagerBase::USSF_Show2x20_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	TBShow2x20();
	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_SendScanData_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	CString csTraceStr;
	csItemSent = csDMLastBarCode;
	csTraceStr.Format(_T("Scanned Data = <%s>"), csItemSent);
	trace(L5, csTraceStr);
	return TBItemSale(false, 0, true);
}

SMStateBase *SMUSSFManagerBase::USSF_SellDepartmentItem_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	SMStateBase *returnState;

	io.Reset();

	CString csPOSData, csValue;
	USSFutil.GetTransactionVariable(_T("USSF_MetaDataParsedBuffer"), csPOSData);
	// Use the right-most character in the string (should only be 1).
	//CString csPOSKeySequenceA, csPOSKeySequenceB, csTempstr;
	CString csTempstr;
	unsigned char ucKey = csPOSData.Right(1)[0]; 

	CString csActionCommand, csActionParmX, csVariable;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

	for (int i = 1; i <= lActionParmCount; i++)
	{
		csVariable.Format(_T("ActionParm%02d"), i);
		USSFutil.GetTransactionVariable(csVariable, csActionParmX);
		CString csParm = csActionParmX;
		CString csParmUpper = csActionParmX;
		csParm.MakeUpper();
		CString csSource = _T("USSF_") + csParm;

		if (!csParm.CompareNoCase(_T("Quantity")))
		{
			USSFutil.GetTransactionVariable(csSource, csValue);
			io.lQuantityEntered = _ttol(csValue);
		}
		else if (!csParm.CompareNoCase(_T("Price")))
		{
			USSFutil.GetTransactionVariable(csSource, csValue);
			io.lPriceEntered = _ttol(csValue);
		}
		else if (!csParm.CompareNoCase(_T("Weight")))
		{
			USSFutil.GetTransactionVariable(csSource, csValue);
			io.lWeightEntered = _ttol(csValue);
		}
		else if (!csParm.CompareNoCase(_T("Tare")))
		{
			USSFutil.GetTransactionVariable(csSource, csValue);
			io.lTareCode = _ttol(csValue);
		}
	}

	// Which Department?
	// for supermarket lookup items
	if (ucKey>=KEY_LOOKUP1 && ucKey<=KEY_LOOKUP40) //TTW56
	{
		// set up for an item sale
		CString ItemTest = _T("");
		//TBGetLookupItemCode(ucKey - KEY_LOOKUP1, ItemTest); // get item code for key 200-239 
      
		if ((ItemTest != _T(" ")) && (ItemTest.GetLength() >= 1))
		{
			csItemSent = ItemTest;

			// Is this a Void? 
			// Check the Void Flag Transaction Variable.
			CString csTempstr;
			CString csTarget = _T("USSF_VOID");
			USSFutil.GetTransactionVariable(csTarget, csTempstr);
			trace(L5, _T("Void Flag = %s."), csTempstr);
			if (!csTempstr.CompareNoCase(_T("TRUE")))
			{	// VOID!
				returnState = TBItemSale(true, 0, false);
			}
			else
			{	// SALE!
				returnState = TBItemSale(false, 0, false);
			}
		}
	}

	USSFutil.SetTransactionVariable(_T("USSF_VOID"), _T(""));
	USSFutil.SetTransactionVariable(_T("USSF_QUANTITY"), _T(""));
	USSFutil.SetTransactionVariable(_T("USSF_PRICE"), _T(""));
	USSFutil.SetTransactionVariable(_T("USSF_WEIGHT"), _T(""));
	USSFutil.SetTransactionVariable(_T("USSF_TARE"), _T(""));

	return returnState;
}
SMStateBase *SMUSSFManagerBase::USSF_SellScannedItem_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	// Optional Positional Parameters: <quantity> <price> <weight> <tare> 
    trace(L5, _T("Processing Action: %s"), _T("SellScannedItem"));
	CString csTraceStr;
	SMStateBase *returnState;

	io.Reset();

	CString csActionCommand, csActionParmX, csVariable;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

	CString csSource, csValue, csQuantity, csPrice, csWeight, csTare;
	//CString csPOSKeySequenceA, csPOSKeySequenceB, csTempstr;
	CString csTempstr;
	for (int i = 1; i <= lActionParmCount; i++)
	{
		csVariable.Format(_T("ActionParm%02d"), i);
		USSFutil.GetTransactionVariable(csVariable, csActionParmX);
		CString csParm = csActionParmX;
		CString csParmUpper = csActionParmX;
		csParmUpper.MakeUpper();
		csSource = _T("USSF_") + csParmUpper;

		if (!csParm.CompareNoCase(_T("Quantity")))
		{
			USSFutil.GetTransactionVariable(csSource, csValue);
			if (_ttol(csValue) != 0)
			{
				io.lQuantityEntered = _ttol(csValue);
				fItemOverride = true;
			}
		}
		else if (!csParm.CompareNoCase(_T("Price")))
		{
			USSFutil.GetTransactionVariable(csSource, csValue);
			if (_ttol(csValue) != 0)
			{
				io.lPriceEntered = _ttol(csValue);
				fItemOverride = true;
			}
		}
		else if (!csParm.CompareNoCase(_T("Weight")))
		{
			USSFutil.GetTransactionVariable(csSource, csValue);
			if (_ttol(csValue) != 0)
			{
				io.lWeightEntered = _ttol(csValue);
				fItemOverride = true;
			}
		}
		else if (!csParm.CompareNoCase(_T("Tare")))
		{
			USSFutil.GetTransactionVariable(csSource, csValue);
			if (_ttol(csValue) != 0)
			{
				io.lTareCode = _ttol(csValue);
				fItemOverride = true;
			}
		}
	}

	csItemSent = csDMLastBarCode.Mid(1);  // TTW533
	//csItemSent = csDMLastBarCode;
	csTraceStr.Format(_T("Scanned Data = <%s>."), csItemSent);
	trace(L5, csTraceStr);

	// Is this a Void? 
	// Check the Void Flag Transaction Variable.
	CString csTarget = _T("USSF_VOID");
	USSFutil.GetTransactionVariable(csTarget, csTempstr);
	trace(L5, _T("Void Flag = %s."), csTempstr);
	if (!csTempstr.CompareNoCase(_T("TRUE")))
	{	// VOID!
		returnState = TBItemSale(true, 0, true);
	}
	else
	{	// SALE!
		returnState = TBItemSale(false, 0, true);
	}

	USSFutil.SetTransactionVariable(_T("USSF_VOID"), (long)0);
	USSFutil.SetTransactionVariable(_T("USSF_QUANTITY"), (long)0);
	USSFutil.SetTransactionVariable(_T("USSF_PRICE"), (long)0);
	USSFutil.SetTransactionVariable(_T("USSF_WEIGHT"), (long)0);
	USSFutil.SetTransactionVariable(_T("USSF_TARE"), (long)0);

	return returnState;
}
SMStateBase *SMUSSFManagerBase::USSF_SellKeyedItem_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    trace(L5, _T("Processing Action: %s"), _T("SellKeyedItem"));
	CString csTraceStr;
	SMStateBase *returnState;

	// Is this really an item sale??
	// Check the Item Flag: 0 - No / 1 - Yes.
	CString csItemSale;
	USSFutil.GetTransactionVariable(_T("USSF_ITEM"), csItemSale);
	if (!csItemSale.CompareNoCase(_T("TRUE")))
	{  //Likely an Item Sale.
		io.Reset();

		CString csPOSData;
		USSFutil.GetTransactionVariable(_T("USSF_METABUFFER"), csPOSData);	

		CString csActionCommand, csActionParmX, csVariable;
		long lActionParmCount;
		USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
		USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

		CString csSource, csValue, csQuantity, csPrice, csWeight, csTare;
		//CString csPOSKeySequenceA, csPOSKeySequenceB, csTempstr;
		CString csTempstr;

		for (int i = 1; i <= lActionParmCount; i++)
		{
			csVariable.Format(_T("ActionParm%02d"), i);
			USSFutil.GetTransactionVariable(csVariable, csActionParmX);
			CString csParm = csActionParmX;
			CString csParmUpper = csActionParmX;
			csParmUpper.MakeUpper();
			csSource = _T("USSF_") + csParmUpper;

			if (!csParm.CompareNoCase(_T("Quantity")))
			{
				USSFutil.GetTransactionVariable(csSource, csValue);
				if (_ttol(csValue) != 0)
				{
					io.lQuantityEntered = _ttol(csValue);
					fItemOverride = true;
				}
			}
			else if (!csParm.CompareNoCase(_T("Price")))
			{
				USSFutil.GetTransactionVariable(csSource, csValue);
				if (_ttol(csValue) != 0)
				{
					io.lPriceEntered = _ttol(csValue);
					fItemOverride = true;
				}
			}
			else if (!csParm.CompareNoCase(_T("Weight")))
			{
				USSFutil.GetTransactionVariable(csSource, csValue);
				if (_ttol(csValue) != 0)
				{
					io.lWeightEntered = _ttol(csValue);
					fItemOverride = true;
				}
			}
			else if (!csParm.CompareNoCase(_T("Tare")))
			{
				USSFutil.GetTransactionVariable(csSource, csValue);
				if (_ttol(csValue) != 0)
				{
					io.lTareCode = _ttol(csValue);
					fItemOverride = true;
				}
			}
		}

		csItemSent = csPOSData;
		CString itemcode = csItemSent;
		csTraceStr.Format(_T("KeyedData Data = <%s>."), csItemSent);
		trace(L5, csTraceStr);

		// Is this a Void? 
		// Check the Void Flag Transaction Variable.
		CString csTarget = _T("USSF_VOID");
		USSFutil.GetTransactionVariable(csTarget, csTempstr);
		trace(L5, _T("Void Flag = %s."), csTempstr);
		if (!csTempstr.CompareNoCase(_T("TRUE")))
		{	// VOID!
			returnState = TBItemSale(true, 0, false);
		}
		else
		{	// SALE!
			returnState = TBItemSale(false, 0, false);
		}
	}
	else
	{
		CString csTempstr, csPOSData;
		USSFutil.GetTransactionVariable(_T("USSF_MetaDataParsedBuffer"), csPOSData);
		csTempstr.Format(_T("Not an Item Sale <%s>."), csPOSData);
		trace(L5, csTempstr);
	}
	USSFutil.SetTransactionVariable(_T("USSF_VOID"), (long)0);
	USSFutil.SetTransactionVariable(_T("USSF_QUANTITY"), (long)0);
	USSFutil.SetTransactionVariable(_T("USSF_PRICE"), (long)0);
	USSFutil.SetTransactionVariable(_T("USSF_WEIGHT"), (long)0);
	USSFutil.SetTransactionVariable(_T("USSF_TARE"), (long)0);

	return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_IgnoreUnknownAction_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    trace(L5, _T("Processing Action: %s"), _T("IgnoreUnknownAction"));
	CString csTraceStr;
	SMStateBase *returnState = STATE_NULL;

	bIgnoreUnknownAction = true;
	return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_Return_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    trace(L5, _T("Processing Action: %s"), _T("Return"));
	CString csTraceStr;
	SMStateBase *returnState = STATE_NULL;

		// Let's get the Action-related Transaction Variables.
	CString csActionCommand, csActionParm;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

	if (lActionParmCount > 0)
	{
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm);
		trace(L5, _T("Performing Reference Move: %s."), csActionParm);
		returnState = MakeReferenceMove(csActionParm);
	}
	else
	{
		trace(L5, _T("Performing Previous Move."));
		returnState = MakePreviousMove();
	}

	return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_Mark_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    trace(L5, _T("Processing Action: %s"), _T("Mark"));
	CString csTraceStr;
	SMStateBase *returnState = STATE_NULL;

	// Let's get the Action-related Transaction Variables.
	CString csActionCommand, csActionParm;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

	if (lActionParmCount > 0)
	{
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm);
		trace(L5, _T("Marking Using Parameter: %s."), csActionParm);
		MarkActivation(csActionParm);
	}
	else
	{
		trace(L5, _T("Cannot Mark; NO Parameter Passed."));
	}

	return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_Resume_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    trace(L5, _T("Processing Action: %s"), _T("Resume"));
	CString csTraceStr;
	SMStateBase *returnState = STATE_NULL;

	returnState = MakePreviousMove();

	return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_Exit_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    trace(L5, _T("Processing Action: %s"), _T("Exit"));
	CString csTraceStr;
	SMStateBase *returnState = STATE_NULL;

	SetActiveContext("");
	ClearActivationDB();

	return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_LoadDefinition_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    trace(L5, _T("Processing Action: %s"), _T("LoadDefinition"));
	CString csTraceStr;
	SMStateBase *returnState = STATE_NULL;

	// TAR #429050 START
	USSFdisp.LoadDefinition();
	// TAR #429050 STOP

	return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_ClearDefinition_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    trace(L5, _T("Processing Action: %s"), _T("ClearDefinition"));
	CString csTraceStr;
	SMStateBase *returnState = STATE_NULL;

	// TAR #429050 START
	USSFdisp.ClearDefinition();
	// TAR #429050 STOP

	return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_StoreSymbols_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    trace(L5, _T("Processing Action: %s"), _T("StoreSymbols"));
	CString csTraceStr;
	SMStateBase *returnState = STATE_NULL;

	// TAR #429050 START
	USSFdisp.StoreSymbols();
	// TAR #429050 STOP

	return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_AmtDueAmtPaid_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);

	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_EnableMSR_Action(SMStateBase *pOpState, vector<CString> vParms)
{
  DMMSREnable();   // enable the MSR, turn light on
  return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_DisableMSR_Action(SMStateBase *pOpState, vector<CString> vParms)
{
  DMMSRDisable();   // enable the MSR, turn light on
  return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_CancelItem_Action(SMStateBase *pOpState, vector<CString> vParms)
{
  if(!TBIsAllowed(TB_FT_QUICKITEMENTRY))
  {
	TBGetItemDetails(io.d);
  }
  io.csItemCode = io.d.csItemCode;
  SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
  return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_FormatData_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	CString csActionCommand, csActionParm1, csActionParm2;
	CString csActionParm3, csActionParm4, csActionParm5, csActionParm6, csActionParm7;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

	// Set the FL State.
	if (lActionParmCount > 1)
	{
		CString csResult, csTempstr;
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm1);
		USSFutil.GetTransactionVariable(_T("ActionParm02"), csActionParm2);
		USSFutil.GetTransactionVariable(_T("ActionParm03"), csActionParm3);
		USSFutil.GetTransactionVariable(_T("ActionParm04"), csActionParm4);
		USSFutil.GetTransactionVariable(_T("ActionParm05"), csActionParm5);
		USSFutil.GetTransactionVariable(_T("ActionParm06"), csActionParm6);
		USSFutil.GetTransactionVariable(_T("ActionParm07"), csActionParm7);
		USSFutil.ResolveSymbol(csActionParm1, csTempstr);
		USSFutil.ResolveSymbol(csActionParm2, csActionParm2);
		USSFutil.ResolveSymbol(csActionParm3, csActionParm3);
		USSFutil.ResolveSymbol(csActionParm4, csActionParm4);
		USSFutil.ResolveSymbol(csActionParm5, csActionParm5);
		USSFutil.ResolveSymbol(csActionParm6, csActionParm6);
		USSFutil.ResolveSymbol(csActionParm7, csActionParm7);
		csResult.Format(csTempstr, csActionParm3, csActionParm4, csActionParm5, csActionParm6, csActionParm7);
		USSFutil.StoreData(csResult, csActionParm2);
	}
	else
	{
		trace(L5, _T("Insufficient Parameters"));
	}
	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_EnableScanner_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	DMScannerEnable();
	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_DisableScanner_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	DMScannerDisable();
	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_IfTxStart_Action(SMStateBase *pOpState, vector<CString> vParms)
{
  SMStateBase *returnState = STATE_NULL;

  bool rc = ((!fInSignedOn) && (!ps.RemoteMode()));
  if (rc == false)
  {
	  returnState = STATE_USSF_FALSE;
  }

  return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_SayAmount_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	CString csActionCommand, csActionParm1;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

	// Set the FL State.
	if (lActionParmCount > 1)
	{
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm1);
		USSFutil.ResolveSymbol(csActionParm1, csActionParm1);
		long lAmount = _ttol(csActionParm1);
		DMSayAmount(lAmount);
	}
	else
	{
		trace(L5, _T("Insufficient Parameters"));
	}
	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_SayPhrase_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	CString csActionCommand, csActionParm1;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

	// Set the FL State.
	if (lActionParmCount > 1)
	{
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm1);
		USSFutil.ResolveSymbol(csActionParm1, csActionParm1);
		long lPhraseNumber = _ttol(csActionParm1);
		DMSayPhrase(csActionParm1);
	}
	else
	{
		trace(L5, _T("Insufficient Parameters"));
	}
	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_SayWave_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	CString csActionCommand, csActionParm1;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

	// Set the FL State.
	if (lActionParmCount > 1)
	{
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm1);
		USSFutil.ResolveSymbol(csActionParm1, csActionParm1);
		DMSayPhrase(csActionParm1);
	}
	else
	{
		trace(L5, _T("Insufficient Parameters"));
	}
	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_SetDeclineMessage_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	CString csActionCommand, csActionParm1;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

	// Set the FL State.
	if (lActionParmCount > 0)
	{
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm1);
		USSFutil.ResolveSymbol(csActionParm1, csActionParm1);
		csErrDescription = csActionParm1;
		//((SMCardMisRead *)pOpState) -> Deliver((LPTSTR)(LPCTSTR)csActionParm1);
		//((SMCardMisRead *)pOpState) -> Initialize();
	}
	else
	{
		trace(L5, _T("Insufficient Parameters"));
	}
	return STATE_NULL;
}

/* Original Version Enhanced
SMStateBase *SMUSSFManagerBase::USSF_Intervention_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	// This will cause a visual verify either immediate or deferred.
	// Parms:
	// 1. now, later, immediate, deferred.
	// 2. type (only visual verify for now).
	// 3-7. Informational Text.
	CString csActionCommand, csActionParm1, csActionParm2;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);
    bool bImmediate = true;
	CString csType = _T("VISUAL");
	CString csText = _T("");

	// Set the FL State.
	if (lActionParmCount > 0)
	{
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm1);
		USSFutil.ResolveSymbol(csActionParm1, csActionParm1);
		if ((!csActionParm1.CompareNoCase(_T("later"))) ||
			(!csActionParm1.CompareNoCase(_T("deferred"))) ||
			(!csActionParm1.CompareNoCase(_T("delayed"))))
		{
			bImmediate = false;
		}

		if (lActionParmCount > 1)
		{
			// Get the Type!
			USSFutil.GetTransactionVariable(_T("ActionParm02"), csActionParm2);
			USSFutil.ResolveSymbol(csActionParm2, csActionParm2);
			csType = csActionParm2;
		}

		if (lActionParmCount > 2)
		{
			// Get the Text!
			CString csTempstr;
			USSFutil.GetTransactionVariable(_T("ActionParm03"), csTempstr);
			USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
			csText = csTempstr + _T(" ");
			USSFutil.GetTransactionVariable(_T("ActionParm04"), csTempstr);
			USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
			csText += csTempstr + _T(" ");
			USSFutil.GetTransactionVariable(_T("ActionParm05"), csTempstr);
			USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
			csText += csTempstr + _T(" ");
			USSFutil.GetTransactionVariable(_T("ActionParm06"), csTempstr);
			USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
			csText += csTempstr + _T(" ");
			USSFutil.GetTransactionVariable(_T("ActionParm07"), csTempstr);
			USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
			csText += csTempstr + _T(" ");
			USSFutil.GetTransactionVariable(_T("ActionParm08"), csTempstr);
			USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
			csText += csTempstr + _T(" ");
			USSFutil.GetTransactionVariable(_T("ActionParm09"), csTempstr);
			USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
			csText += csTempstr + _T(" ");
		}
	}

	if (!csType.CompareNoCase(_T("VISUAL")))
	{
		lUnapprovedVisualVerifyItems++;

		/*
		CString csDesc;
		// csErrDescription may hold an alternate VV description.
		if (io.d.csErrDescription.IsEmpty())
		{
		  csDesc = ps.GetPSText(RA_VISUAL_VALIDATE_ID, SCOT_LANGUAGE_PRIMARY);
		}
		else
		{
		  csDesc = io.d.csErrDescription + io.d.csDescription;
		}
		*/
/*
		CString csAnchorStateName = getSCOTStateName();//Tar 247393
		CString csInstruction;
		csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csText, ps.GetPSText(RA_LANEBTT_VISUALVALIDATIONITEM, SCOT_LANGUAGE_PRIMARY), csAnchorStateName, _T(""), ps.GetPSText(RA_THIRDLINE_VISUALVALIDATIONITEM, SCOT_LANGUAGE_PRIMARY));
		trace(L5, _T("+ra.OnVisualValidationItem, Message=%s"), csInstruction);
		ra.OnVisualValidationItem(csInstruction, FALSE);
		trace(L5, _T("-ra.OnVisualValidationItem"));

		// Save Text into USSF_VISUAL_APPROVAL
		USSFutil.SetTransactionVariable(_T("USSF_VISUAL_APPROVAL"), csText);
	}
	else	// CUSTOM!
	{
		lUnapprovedUSSFItems++;

		// This could change depending on desired behaviour of HHRAP.
		CString csAnchorStateName = getSCOTStateName();//Tar 247393
		CString csInstruction;
		csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csText, ps.GetPSText(RA_LANEBTT_VISUALVALIDATIONITEM, SCOT_LANGUAGE_PRIMARY), csAnchorStateName, _T(""), ps.GetPSText(RA_THIRDLINE_VISUALVALIDATIONITEM, SCOT_LANGUAGE_PRIMARY));
		trace(L5, _T("+ra.OnVisualValidationItem, Message=%s"), csInstruction);
		ra.OnVisualValidationItem(csInstruction, FALSE);
		trace(L5, _T("-ra.OnVisualValidationItem"));

		// Save Text into USSF_VISUAL_APPROVAL
		USSFutil.SetTransactionVariable(_T("USSF_CUSTOM_APPROVAL"), csText);
		USSFutil.SetTransactionVariable(_T("USSF_APPROVAL_TYPE"), csType);
	}

	if (bImmediate)
	{
		CREATE_AND_DISPENSE(WaitForApproval)(false,false);
	}

	return STATE_NULL;
}
*/

SMStateBase *SMUSSFManagerBase::USSF_FireTBDisplay_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	// This will cause a TB Message to be sent as a 2x20 PRIMARYDISPLAY to the TB.
	// Sometimes these are referred to as Fake POS Messages since they do not
	// originate at the POS.
	// Parms:
	// 1-8 POS Display Data.
	CString csActionCommand, csActionParm1, csActionParm2;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);
    bool bImmediate = true;
	CString csType = _T("VISUAL");
	CString csText = _T("");

	// Set the FL State.
	if (lActionParmCount > 0)
	{
		// Get the Text!
		CString csTempstr;
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		csText = csTempstr + _T(" ");
		USSFutil.GetTransactionVariable(_T("ActionParm02"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		csText += csTempstr + _T(" ");
		USSFutil.GetTransactionVariable(_T("ActionParm03"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		csText += csTempstr + _T(" ");
		USSFutil.GetTransactionVariable(_T("ActionParm04"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		csText += csTempstr + _T(" ");
		USSFutil.GetTransactionVariable(_T("ActionParm05"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		csText += csTempstr + _T(" ");
		USSFutil.GetTransactionVariable(_T("ActionParm06"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		csText += csTempstr + _T(" ");
		USSFutil.GetTransactionVariable(_T("ActionParm07"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		csText += csTempstr + _T(" ");
		USSFutil.GetTransactionVariable(_T("ActionParm08"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		csText += csTempstr + _T(" ");
	}

	USSFutil.SendScotMsg(TB_DEV_DISPLAYPRIMARY, csText.GetLength(), (void *) (LPCTSTR) csText);

	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_Alert_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	// This will cause a TB Message to be sent as a 2x20 PRIMARYDISPLAY to the TB.
	// Sometimes these are referred to as Fake POS Messages since they do not
	// originate at the POS.
	// Parms:
	// 1-8 POS Display Data.
	CString csActionCommand, csActionParm1, csActionParm2;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);
    bool bImmediate = true;
	CString csType = _T("VISUAL");
	CString csText = _T("");

	// Set the FL State.
	if (lActionParmCount > 0)
	{
		// Get the Text!
		CString csTempstr;
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		csText = csTempstr + _T(" ");
		USSFutil.GetTransactionVariable(_T("ActionParm02"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		csText += csTempstr + _T(" ");
		USSFutil.GetTransactionVariable(_T("ActionParm03"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		csText += csTempstr + _T(" ");
		USSFutil.GetTransactionVariable(_T("ActionParm04"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		csText += csTempstr + _T(" ");
		USSFutil.GetTransactionVariable(_T("ActionParm05"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		csText += csTempstr + _T(" ");
		USSFutil.GetTransactionVariable(_T("ActionParm06"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		csText += csTempstr + _T(" ");
		USSFutil.GetTransactionVariable(_T("ActionParm07"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		csText += csTempstr + _T(" ");
		USSFutil.GetTransactionVariable(_T("ActionParm08"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		csText += csTempstr + _T(" ");
	}

	USSFutil.SetTransactionVariable(_T("USSF_ALERT"), csText);

	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_AssistAlert_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	// This will cause a TB Message to be sent as a 2x20 PRIMARYDISPLAY to the TB.
	// Sometimes these are referred to as Fake POS Messages since they do not
	// originate at the POS.
	// Parms:
	// 1-8 POS Display Data.
	CString csActionCommand, csActionParm1, csActionParm2;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);
    bool bImmediate = true;
	CString csType = _T("VISUAL");
	CString csText = _T("");

	// Set the FL State.
	if (lActionParmCount > 0)
	{
		// Get the Text!
		CString csTempstr;
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		csText = csTempstr + _T(" ");
		USSFutil.GetTransactionVariable(_T("ActionParm02"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		csText += csTempstr + _T(" ");
		USSFutil.GetTransactionVariable(_T("ActionParm03"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		csText += csTempstr + _T(" ");
		USSFutil.GetTransactionVariable(_T("ActionParm04"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		csText += csTempstr + _T(" ");
		USSFutil.GetTransactionVariable(_T("ActionParm05"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		csText += csTempstr + _T(" ");
		USSFutil.GetTransactionVariable(_T("ActionParm06"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		csText += csTempstr + _T(" ");
		USSFutil.GetTransactionVariable(_T("ActionParm07"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		csText += csTempstr + _T(" ");
		USSFutil.GetTransactionVariable(_T("ActionParm08"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		csText += csTempstr + _T(" ");
	}

	ra.OnNormalItem(csText);

	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_SetTimeOut_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	// This will cause a TB Message to be sent as a 2x20 PRIMARYDISPLAY to the TB.
	// Sometimes these are referred to as Fake POS Messages since they do not
	// originate at the POS.
	// Parms:
	// 1-8 POS Display Data.
	CString csActionCommand, csActionParm1, csActionParm2;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

	// Set the FL State.
	if (lActionParmCount > 0)
	{
		// Get the Text!
		CString csTempstr;
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));

		pOpState -> nTimeOut = _ttol(csTempstr);

	}
	else
	{
		trace(L5, _T("No TimeOut Value Specified."));
	}

	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_Trace_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	// This will cause a TB Message to be sent as a 2x20 PRIMARYDISPLAY to the TB.
	// Sometimes these are referred to as Fake POS Messages since they do not
	// originate at the POS.
	// Parms:
	// 1-8 POS Display Data.
	CString csActionCommand, csActionParm1, csActionParm2;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);
	CString csText;

	if (lActionParmCount > 0)
	{
		// Get the Text!
		CString csTempstr;
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		csText = csTempstr + _T(" ");
		USSFutil.GetTransactionVariable(_T("ActionParm02"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		csText += csTempstr + _T(" ");
		USSFutil.GetTransactionVariable(_T("ActionParm03"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		csText += csTempstr + _T(" ");
		USSFutil.GetTransactionVariable(_T("ActionParm04"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		csText += csTempstr + _T(" ");
		USSFutil.GetTransactionVariable(_T("ActionParm05"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		csText += csTempstr + _T(" ");
		USSFutil.GetTransactionVariable(_T("ActionParm06"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		csText += csTempstr + _T(" ");
		USSFutil.GetTransactionVariable(_T("ActionParm07"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		csText += csTempstr + _T(" ");
		USSFutil.GetTransactionVariable(_T("ActionParm08"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		csText += csTempstr + _T(" ");

		trace(L5, csText);
	}
	else
	{
		trace(L5, _T("No Trace Information Specified."));
	}

	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_IdentifyApproval_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	SMStateBase *returnState = STATE_NULL;

	// Look at USSF_APPROVAL_TYPE and fire the type as an event.
	CString csValue;
	USSFutil.GetTransactionVariable(_T("USSF_APPROVAL_TYPE"), csValue);
	//SetPendingEvent(csValue);

	if (csValue.GetLength() > 0)
	{
		//returnState = MakeNextMove();
		returnState = ProcessEvent(csValue);
	}

	return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_Sleep_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	CString csActionCommand, csActionParm1, csActionParm2;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

	// Set the FL State.
	if (lActionParmCount > 0)
	{
		// Get the Text!
		CString csTempstr;
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));

		if (_ttol(csTempstr) > 0)
		{
			Sleep(_ttol(csTempstr));
		}

	}
	else
	{
		trace(L5, _T("No Sleep Value Specified."));
	}

	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_ClearUSSFApproval_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	lUnapprovedUSSFItems = 0;
	USSFutil.SetTransactionVariable(_T("USSF_APPROVAL_TYPE"), _T(""));
	USSFutil.SetTransactionVariable(_T("USSF_CUSTOM_APPROVAL"), _T(""));

	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_TBFinish_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	return pOpState -> TBFinish();
}

SMStateBase *SMUSSFManagerBase::USSF_ResetTenderButtons_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	ps.LoadTenderList();
	return STATE_NULL;
}

/* BAD
SMStateBase *SMUSSFManagerBase::USSF_SetAnchorState_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	CString csActionCommand, csActionParm1, csActionParm2;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

	// Set the FL State.
	if (lActionParmCount > 0)
	{
		// Get the Text!
		CString csTempstr;
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));

				

	}
	else
	{
		trace(L5, _T("No Anchor State Specified."));
	}

	return STATE_NULL;
}
*/

SMStateBase *SMUSSFManagerBase::USSF_GoAnchorState_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	return createAnchorState();
}

SMStateBase *SMUSSFManagerBase::USSF_IfApprovalPending_Action(SMStateBase *pOpState, vector<CString> vParms)
{
  SMStateBase *returnState = STATE_NULL;

  bool rc = isAnyApprovalRequired();
  if (rc == false)
  {
	  returnState = STATE_USSF_FALSE;
  }

  return returnState;
}

/////////////////////////////////////////////
// ACTIONS PREVIOUSLY IN USSFUtility
/////////////////////////////////////////////
SMStateBase *SMUSSFManagerBase::USSF_FullReset_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	USSFutil.ResetData(_T("FULL"));
	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_PartialReset_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	USSFutil.ResetData(_T("PARTIAL"));
	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_ClearTransactionData_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	USSFutil.ResetData(_T("TRXN"));
	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_ClearItemData_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	USSFutil.ResetData(_T("ITEM"));
	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_ClearMetaData_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	USSFutil.ResetData(_T("META"));
	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_Clear_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	CString csActionCommand, csActionParm1, csActionParm2;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

	if (lActionParmCount > 0)
	{
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm1);
		USSFutil.ClearData(csActionParm1);
	}
	else
	{
	  trace(L7, _T("Target Not Specified."));
	}
	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_Append_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	CString csActionCommand, csActionParm1, csActionParm2;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

	if (lActionParmCount > 1)
	{
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm1);
		USSFutil.GetTransactionVariable(_T("ActionParm02"), csActionParm2);
		USSFutil.ConcatenateData(csActionParm1, csActionParm2);
	}
    else if (lActionParmCount > 0)
    {
	  trace(L5, _T("Target Not Specified."));
	}
	else
	{
	  trace(L5, _T("Source/Target Not Specified."));
	}
	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_Store_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	CString csActionCommand, csActionParm1, csActionParm2;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

	if (lActionParmCount > 1)
	{
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm1);
		USSFutil.GetTransactionVariable(_T("ActionParm02"), csActionParm2);
		USSFutil.StoreData(csActionParm1, csActionParm2);
	}
    else if (lActionParmCount > 0)
    {
	  trace(L5, _T("Target Not Specified."));
	}
	else
	{
	  trace(L5, _T("Source/Target Not Specified."));
	}

	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_Parse_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	CString csActionCommand, csActionParm1, csActionParm2;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

	if (lActionParmCount > 1)
	{
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm1);
		USSFutil.GetTransactionVariable(_T("ActionParm02"), csActionParm2);
		USSFutil.ParseData(csActionParm1, csActionParm2);
	}
    else if (lActionParmCount > 0)
    {
	  trace(L5, _T("Target Not Specified."));
	}
	else
	{
	  trace(L5, _T("Source/Target Not Specified."));
	}

	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_SendPOSMessage_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	CString csActionCommand, csActionParm1, csActionParm2;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

	if (lActionParmCount > 0)
	{
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm1);
		pOpState -> SendPOSData(csActionParm1);
	}
    else if (lActionParmCount > 0)
    {
	  trace(L5, _T("Source Not Specified."));
	}

	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_DisplayKeyedElement_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	CString csPOSData;
	USSFutil.StoreParsedData(_T("@MetaElement"), csPOSData, false);
	pOpState -> ShowPOSDisplay(_T(""), csPOSData.Mid(0,20));
	return STATE_NULL;
}
SMStateBase *SMUSSFManagerBase::USSF_DisplayKeyedBuffer_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	CString csPOSData;
	USSFutil.StoreParsedData(_T("@MetaBuffer"), csPOSData, false);
	pOpState -> ShowPOSDisplay(_T(""), csPOSData.Mid(0,20));
	return STATE_NULL;
}
SMStateBase *SMUSSFManagerBase::USSF_DisplayKeyedData_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	CString csPOSData;
	USSFutil.StoreParsedData(_T("@MetaBuffer"), csPOSData, false);
	pOpState -> ShowPOSDisplay(_T(""), csPOSData.Mid(0,20));
	return STATE_NULL;
}
SMStateBase *SMUSSFManagerBase::USSF_Display_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	CString csActionCommand, csActionParmX;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

	CString csTempstr, csResult, csVariable;
	for (int i = 1; i <= lActionParmCount; i++)
	{
		csVariable.Format(_T("ActionParm%02d"), i);
		USSFutil.GetTransactionVariable(csVariable, csActionParmX);
		USSFutil.ResolveSymbol(csActionParmX, csResult);
		csTempstr += csResult + _T(" ");
	}
	pOpState -> ShowPOSDisplay(_T(""), csTempstr.Mid(0,20));
	return STATE_NULL;
}
SMStateBase *SMUSSFManagerBase::USSF_ClearDisplay_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	pOpState -> ClearPOSDisplay();
	return STATE_NULL;
}
SMStateBase *SMUSSFManagerBase::USSF_InitAssist_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	pOpState -> fStateAssist = true;
	tb.SetMonitorAll(true);  // true will cause Retalix POS to display.
	pOpState -> ClearPOSDisplay();
	return STATE_NULL;
}
SMStateBase *SMUSSFManagerBase::USSF_ExitAssist_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	pOpState -> fStateAssist = false;
	tb.SetMonitorAll(false);
	pOpState -> ClearPOSDisplay();
	USSFutil.SetTransactionVariable(_T("USSFActiveContext"), _T(""));
	USSFutil.SetTransactionVariable(_T("USSFActiveState"), _T(""));
	ra.RequestEnableAssistMode(true);
	return STATE_NULL;
}
SMStateBase *SMUSSFManagerBase::USSF_NOP_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	return STATE_NULL;
}
SMStateBase *SMUSSFManagerBase::USSF_Set_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	CString csTarget, csValue;

	CString csActionCommand, csActionParm1, csActionParm2;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

	if (lActionParmCount > 1)
	{
		CString csVariable;
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm1);
		USSFutil.GetTransactionVariable(_T("ActionParm02"), csActionParm2);
		csActionParm1.MakeUpper();
		USSFutil.ResolveSymbol(csActionParm1, csVariable);
		csTarget = _T("USSF_");
		csTarget += csVariable;
		csValue = csActionParm2;
		USSFutil.SetTransactionVariable(csTarget, csValue);
		trace(L5, _T("Setting <%s> = <%s>."), csTarget, csValue);
	}
	else if (lActionParmCount == 1)
	{
	  USSFutil.GetTransactionVariable(_T("ActionParm01"), csTarget);
	  trace(L5, _T("Value Not Specified for Target <%s>."), csTarget);
	}
	else
	{
	  trace(L5, _T("Target/Value Not Specified."));
	}
	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_PressButtonX_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	SMStateBase *returnState = STATE_NULL;
	
	CString csActionCommand, csActionParm1, csActionParm2;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

    if (lActionParmCount > 0)
    {
	  USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm1);
	  long which = _ttol(csActionParm1);
	  switch(which)
	  {
	  case 1:
		  returnState = pOpState -> PSButton1();
		  break;
	  case 2:
		  returnState = pOpState -> PSButton2();
		  break;
	  case 3:
		  returnState = pOpState -> PSButton3();
		  break;
	  case 4:
		  returnState = pOpState -> PSButton4();
		  break;
	  case 5:
		  returnState = pOpState -> PSButton5();
		  break;
	  case 6:
		  returnState = pOpState -> PSButton6();
		  break;
	  case 7:
		  returnState = pOpState -> PSButton7();
		  break;
	  case 8:
		  returnState = pOpState -> PSButton8();
		  break;
	  }
    }
	else
	{
	  trace(L5, _T("Button Not Specified."));
	}
	return returnState;
}
SMStateBase *SMUSSFManagerBase::USSF_PressGoBack_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	SMStateBase *returnState = STATE_NULL;
	returnState = pOpState -> PSButtonGoBack();
	return returnState;
}
SMStateBase *SMUSSFManagerBase::USSF_GetDiagFiles_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	SMStateBase *returnState = STATE_NULL;
	//CString csValue;
	//GetTransactionVariable(_T("USSF_SYS_Pending"), csValue);
	//if (!csValue.CompareNoCase(_T("InternalError")))
	//{
	//	returnState = STATE_STOP;
		// Get DIAG Files.
	//	bool rc = pOpState -> LaunchProgram(_T("\\scot\\bin\\GetDiagFiles.exe"), 0, false);
	//	SetTransactionVariable(_T("USSF_SYS_Pending"), _T("Shutdown"));
	//}
	bool rc = pOpState -> LaunchProgram(_T("\\scot\\bin\\GetDiagFiles.exe"), 0, false);
	return returnState;
}
SMStateBase *SMUSSFManagerBase::USSF_Shutdown_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	SMStateBase *returnState = STATE_STOP;
	//bool rc = pOpState -> LaunchProgram(_T("\\scot\\bin\\GetDiagFiles.exe"), 0, false);
	return returnState;
}
SMStateBase *SMUSSFManagerBase::USSF_DisplayRAP_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	SMStateBase *returnState = STATE_NULL;

	CString csActionCommand, csActionParm;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

	if (lActionParmCount > 0)
	{
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm);
		ra.OnNormalItem(csActionParm);
	}
	else
	{
		trace(L5, _T("Cannot Display; NO Parameter Passed."));
	}

	return returnState;
}
SMStateBase *SMUSSFManagerBase::USSF_DisplayReceipt_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	SMStateBase *returnState = STATE_NULL;

	CString csActionCommand, csActionParm1, csActionParm2;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

	if (lActionParmCount > 1)
	{
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm1);
		USSFutil.GetTransactionVariable(_T("ActionParm02"), csActionParm2);
		ps.AddMessageToReceipt(csActionParm1, csActionParm2);
	}
	else
	{
		trace(L5, _T("Cannot Display; 2 Parameters Required."));
	}

	return returnState;
}
SMStateBase *SMUSSFManagerBase::USSF_GetText_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	SMStateBase *returnState = STATE_NULL;

	CString csActionCommand, csActionParm1, csActionParm2;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

	if (lActionParmCount > 1)
	{
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm1);
		USSFutil.GetTransactionVariable(_T("ActionParm02"), csActionParm2);
	}
	else
	{
		trace(L5, _T("Cannot Display; 2 Parameters Required."));
	}

	return returnState;
}
// TAR #429050 START
SMStateBase *SMUSSFManagerBase::USSF_Scroll_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	CString csTemp;
	USSFutil.ResolveSymbol(_T("@PSXCOMMAND"), csTemp);
	if (csTemp.Find(_T("SCROLLUP")) >= 0)
	{
		USSFdisp.ScrollUp(csTemp);
	}
	else if (csTemp.Find(_T("SCROLLDOWN")) >= 0)
	{
		USSFdisp.ScrollDown(csTemp);
	}
	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_ScrollUp_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	CString csTemp;
	CString csActionCommand, csActionParm1, csVariable, csResult;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);
    if (lActionParmCount > 0)
    {
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm1);
		USSFutil.ResolveSymbol(csActionParm1, csActionParm1);
		if (csActionParm1.GetLength() > 0)
		{
			csTemp.Format(_T("SCROLLUP:CONTROL=%s"), csActionParm1);
			USSFdisp.ScrollUp(csTemp);
		}
		else
		{
			trace(L5, _T("Parameter Not Usable."));
		}
    }
	else
	{
		trace(L5, _T("Control Not Specified."));
	}
	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_ScrollDown_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	CString csTemp;
	CString csActionCommand, csActionParm1, csVariable, csResult;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);
    if (lActionParmCount > 0)
    {
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm1);
		USSFutil.ResolveSymbol(csActionParm1, csActionParm1);
		if (csActionParm1.GetLength() > 0)
		{
			csTemp.Format(_T("SCROLLDOWN:CONTROL=%s"), csActionParm1);
			USSFdisp.ScrollDown(csTemp);
		}
		else
		{
			trace(L5, _T("Parameter Not Usable."));
		}
    }
	else
	{
		trace(L5, _T("Control Not Specified."));
	}
	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_ScrollList_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	CString csTemp;
	CString csActionCommand, csActionParm1, csVariable, csResult;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);
    if (lActionParmCount > 0)
    {
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm1);
		USSFutil.ResolveSymbol(csActionParm1, csActionParm1);
		if (csActionParm1.GetLength() == 0)
		{
			trace(L5, _T("Parameter Not Usable."));
			return STATE_NULL;
		}
    }
	else
	{
		trace(L5, _T("Control Not Specified."));
		return STATE_NULL;
	}
	// Need to check PSXLISTSCROLL (0 - Up ; 1 - Down).
	long lTemp;
	USSFutil.ResolveSymbol(_T("@PSXLISTSCROLL"), lTemp);
	switch(lTemp)
	{
	case 0:
		csTemp.Format(_T("SCROLLUP:CONTROL=%s"), csActionParm1);
		USSFdisp.ScrollUp(csTemp);
		break;
	default:
		csTemp.Format(_T("SCROLLDOWN:CONTROL=%s"), csActionParm1);
		USSFdisp.ScrollDown(csTemp);
		break;
	}
	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_Initialize_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	return pOpState -> Initialize();
}
SMStateBase *SMUSSFManagerBase::USSF_SaveActivationData_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	trace(L5, _T("SMStateBase::SaveActivationData(void)"));

	// Push existing activation.
	thisActivation.csTimeStamp = _T(__TIMESTAMP__);
	thisActivation.bInScope = true;
	thisActivation.lSerial = lActivationSerial++;
	//thisActivation.lLinkage = -1;
	trace(L6, _T("Saving Activation Record: state = %s, context = %s; ref = %s"), thisActivation.CurrentState,
		        thisActivation.CurrentContext, thisActivation.ReferenceKey);
	ActivationDB.push_back(thisActivation);

	// Finally, Create New Activation Record.
	thisActivation.lLinkage = -1;
	thisActivation.ReferenceKey = _T("");
	SetActivationState();
	return STATE_NULL;
}
// TAR #429050 STOP
SMStateBase *SMUSSFManagerBase::USSF_EchoInput_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	CString csData;
	USSFutil.GetTransactionVariable(_T("USSF_METAELEMENT"), csData);
	for (int i = 0; i < csData.GetLength(); i++)
	{
		ps.EchoInput((long)(TCHAR)csData[i], true);
	}
	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_EchoData_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	CString csActionCommand, csActionParmX, csVariable, csResult;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

	if (lActionParmCount > 0)
	{
		int i;
		for (i = 1; i <= lActionParmCount; i++)
		{
			csVariable.Format(_T("ActionParm%02d"), i);
			USSFutil.GetTransactionVariable(csVariable, csActionParmX);
			USSFutil.ResolveSymbol(csActionParmX, csActionParmX);
			csResult += csActionParmX + _T(" ");
		}
		csResult.TrimLeft();
		csResult.TrimRight();
 
		for (i = 0; i < csResult.GetLength(); i++)
		{
			ps.EchoInput((long)(TCHAR)csResult[i], true);
		}
    }
	else
	{
	  trace(L5, _T("Data Not Specified."));
	}

	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_ShowLeadthru_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	CString csActionCommand, csActionParm1, csActionParm2;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

    if (lActionParmCount > 0)
    {
	  USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm1);
	  USSFutil.ResolveSymbol(csActionParm1, csActionParm1);
	  ps.SetLeadthruText(csActionParm1);
    }
	else
	{
	  trace(L5, _T("Data Not Specified."));
	}

	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_EchoAmount_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	CString csActionCommand, csActionParm1, csActionParm2;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

    if (lActionParmCount > 0)
    {
	  USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm1);
	  USSFutil.ResolveSymbol(csActionParm1, csActionParm1);
	  long lTempAmount = _ttol(csActionParm1);
	  CString csTempstr;
	  csTempstr = ps.FmtDollar(lTempAmount);
	  for (int i = 0; i < csTempstr.GetLength(); i++)
	  {
		ps.EchoInput((long)(TCHAR)csTempstr[i]);
	  }
    }
	else
	{
	  trace(L5, _T("Data Not Specified."));
	}

	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_ClearInput_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	ps.ClearInput();
	USSFutil.ResetData(_T("META"));
	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_EnableButton_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	SMStateBase *returnState = STATE_NULL;
	
	CString csActionCommand, csActionParm1, csActionParm2;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

    if (lActionParmCount > 0)
    {
	  USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm1);
	  ps.ButtonEnable(csActionParm1, true);
    }
	else
	{
	  trace(L5, _T("Button Not Specified."));
	}
	return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_DisableButton_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	SMStateBase *returnState = STATE_NULL;
	
	CString csActionCommand, csActionParm1, csActionParm2;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

    if (lActionParmCount > 0)
    {
	  USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm1);
	  ps.ButtonEnable(csActionParm1, false);
    }
	else
	{
	  trace(L5, _T("Button Not Specified."));
	}
	return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_SetButton_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	SMStateBase *returnState = STATE_NULL;
	
	CString csActionCommand, csButtonText, csButtonFlash, csButtonVisible, csButtonEnabled, csButtonName;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

	bool bVisible = false, bFlash = false, bEnabled = false;
	CString csResult;

    if (lActionParmCount > 4)
    {	// Text
	  CString csTempstr;
	  USSFutil.GetTransactionVariable(_T("ActionParm05"), csTempstr);
	  csButtonText = csTempstr + _T(" ");
	  USSFutil.GetTransactionVariable(_T("ActionParm06"), csTempstr);
	  csButtonText += csTempstr + _T(" ");
	  USSFutil.GetTransactionVariable(_T("ActionParm07"), csTempstr);
	  csButtonText += csTempstr + _T(" ");
	  USSFutil.GetTransactionVariable(_T("ActionParm08"), csTempstr);
	  csButtonText += csTempstr + _T(" ");
	  USSFutil.GetTransactionVariable(_T("ActionParm09"), csTempstr);
	  csButtonText += csTempstr + _T(" ");
	  USSFutil.ResolveSymbol(csButtonText, csResult, _T(""));
    }
    if (lActionParmCount > 3)
    {	// Flash
	  USSFutil.GetTransactionVariable(_T("ActionParm04"), csButtonFlash);
	  csButtonFlash.TrimLeft();
	  csButtonFlash.TrimRight();
	  csButtonFlash.MakeUpper();
	  if (csButtonFlash.Mid(0, 1) == _T("T"))
	  {
		  bFlash = true;
	  }
    }
    if (lActionParmCount > 2)
    {	// Visible
	  USSFutil.GetTransactionVariable(_T("ActionParm03"), csButtonVisible);
	  csButtonVisible.TrimLeft();
	  csButtonVisible.TrimRight();
	  csButtonVisible.MakeUpper();
	  if (csButtonVisible.Mid(0, 1) == _T("T"))
	  {
		  bVisible = true;
	  }
    }
    if (lActionParmCount > 1)
    {	// Enabled
	  USSFutil.GetTransactionVariable(_T("ActionParm02"), csButtonEnabled);
	  csButtonEnabled.TrimLeft();
	  csButtonEnabled.TrimRight();
	  csButtonEnabled.MakeUpper();
	  if (csButtonEnabled.Mid(0, 1) == _T("T"))
	  {
		  bEnabled = true;
	  }
    }
    if (lActionParmCount > 0)
    {	// Name
	  USSFutil.GetTransactionVariable(_T("ActionParm01"), csButtonName);
    }
    if (lActionParmCount <= 0)
	{
	  trace(L5, _T("Information Not Specified."));
	}
	else
	{	// Take Specified Actions.
		if (csResult.GetLength() > 0)
		{
			ps.Button(csButtonName, csResult, bEnabled);
		}

		ps.ButtonState(csButtonName, bEnabled, bVisible);
		ps.ButtonFlash(csButtonName, bFlash);
	}

	return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_ClearControlButtons_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	SMStateBase *returnState = STATE_NULL;
	
	ps.ButtonState(_T("SMButton1"), false, false);
	ps.ButtonState(_T("SMButton2"), false, false);
	ps.ButtonState(_T("SMButton3"), false, false);
	ps.ButtonState(_T("SMButton4"), false, false);
	ps.ButtonState(_T("SMButton5"), false, false);
	ps.ButtonState(_T("SMButton6"), false, false);
	ps.ButtonState(_T("SMButton7"), false, false);
	ps.ButtonState(_T("SMButton8"), false, false);

	return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_AllowInput_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	SMStateBase *returnState = STATE_NULL;
	
	CString csActionCommand, csActionParm1, csActionParm2, csActionParm3;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);
	long lActionParm1 = 20;
	bool bActionParm2 = false;
	bool bActionParm3 = false;

    if (lActionParmCount > 2)
    {
	  USSFutil.GetTransactionVariable(_T("ActionParm03"), csActionParm3);
	  if (!csActionParm3.CompareNoCase(_T("true")))
	  {
		  bActionParm3 = true;
	  }
    }

    if (lActionParmCount > 1)
    {
	  USSFutil.GetTransactionVariable(_T("ActionParm02"), csActionParm2);
	  if (!csActionParm2.CompareNoCase(_T("true")))
	  {
		  bActionParm2 = true;
	  }
    }

    if (lActionParmCount > 0)
    {
	  USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm1);
	  long lTemp = _ttol(csActionParm1);
	  if (lTemp > 0)
	  {
		  lActionParm1 = lTemp;
	  }
    }

	ps.AllowInput(lActionParm1, bActionParm2, bActionParm3);

	return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_SendRAPMessage_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	SMStateBase *returnState = STATE_NULL;

	CString csActionCommand, csActionParmX, csVariable, csResult = RAP_RECEIPT_LINE_SUMMARY;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

	if (lActionParmCount > 0)
	{
		int i;
		for (i = 1; i <= lActionParmCount; i++)
		{
			csVariable.Format(_T("ActionParm%02d"), i);
			USSFutil.GetTransactionVariable(csVariable, csActionParmX);
			USSFutil.ResolveSymbol(csActionParmX, csActionParmX);
			csResult += csActionParmX + _T(" ");
		}
		csResult.TrimLeft();
		csResult.TrimRight();
 
		ra.OnNormalItem(csResult);
    }
	else
	{
	  trace(L5, _T("Data Not Specified."));
	}

	return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_GoUSSF_Action(SMStateBase *pOpState, vector<CString> vParms)
{
  SMStateBase *returnState = STATE_NULL;

  CString csActionCommand, csActionParm1;
  long lActionParmCount;
  USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
  USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

  // Set the FL State.
  if (lActionParmCount > 0)
  {
	USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm1);
	USSFutil.ResolveSymbol(csActionParm1, csActionParm1);
	SetActiveContext(csActionParm1);
	returnState = MakeNewState(_T("USSF"));
  }
  else
  {
	trace(L6, _T("USSF Context Not Specified"));
  }

  return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_IF_Action(SMStateBase *pOpState, vector<CString> vParms)
{
  SMStateBase *returnState = STATE_NULL;

  CString csActionCommand, csActionParm1, csActionParm2, csActionParm3;
  long lActionParmCount;
  USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
  USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

  // Set the FL State.
  if (lActionParmCount > 2)
  {
	USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm1);
	USSFutil.GetTransactionVariable(_T("ActionParm02"), csActionParm2);
	USSFutil.GetTransactionVariable(_T("ActionParm03"), csActionParm3);
	USSFutil.ResolveSymbol(csActionParm1, csActionParm1);
	USSFutil.ResolveSymbol(csActionParm2, csActionParm2);
	USSFutil.ResolveSymbol(csActionParm3, csActionParm3);

	bool rc = false;
	if (csActionParm2 == _T("=") || csActionParm2 == _T("=="))
	{
		if (csActionParm1 == csActionParm3)
		{
			rc = true;
		}
	}
	else if (csActionParm2 == _T("NE") || csActionParm2 == _T("!="))
	{
		if (csActionParm1 == csActionParm3)
		{
			rc = true;
		}
	}
	else if (csActionParm2 == _T("GT") || csActionParm2 == _T(">"))
	{
		long lParm1 = _ttol(csActionParm1);
		long lParm3 = _ttol(csActionParm3);
		if (lParm1 > lParm3)
		{
			rc = true;
		}
	}
	else if (csActionParm2 == _T("GE") || csActionParm2 == _T(">="))
	{
		long lParm1 = _ttol(csActionParm1);
		long lParm3 = _ttol(csActionParm3);
		if (lParm1 >= lParm3)
		{
			rc = true;
		}
	}
	else if (csActionParm2 == _T("LT") || csActionParm2 == _T("<"))
	{
		long lParm1 = _ttol(csActionParm1);
		long lParm3 = _ttol(csActionParm3);
		if (lParm1 < lParm3)
		{
			rc = true;
		}
	}
	else if (csActionParm2 == _T("LE") || csActionParm2 == _T("<="))
	{
		long lParm1 = _ttol(csActionParm1);
		long lParm3 = _ttol(csActionParm3);
		if (lParm1 <= lParm3)
		{
			rc = true;
		}
	}

    if (rc == false)
	{
	  returnState = STATE_USSF_FALSE;
	}
  }
  else
  {
	trace(L6, _T("IF Parameters Not Specified"));
  }

  return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_GoFLState_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	SMStateBase *returnState = STATE_NULL;
	CString csActionCommand, csActionParms, csTempstr;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

	if (lActionParmCount > 0)
	{
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		// Make the State
		returnState = MakeNewState(csTempstr);
	}
	else
	{
		trace(L5, _T("State Not Specified"));
	}
	return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_WaitForApproval_Action(SMStateBase *pOpState, vector<CString> vParms)
{
		CREATE_AND_DISPENSE(WaitForApproval)(false,false);
}

SMStateBase *SMUSSFManagerBase::USSF_SetAnchorState_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	CString csActionCommand, csActionParm1, csActionParm2;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

	// Set the FL State.
	if (lActionParmCount > 0)
	{
		// Get the Text!
		CString csTempstr;
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));

		MakeAnchorState(csTempstr);
	}
	else
	{
		trace(L6, _T("No Anchor State Specified."));
	}

	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_IfTxBalance_Action(SMStateBase *pOpState, vector<CString> vParms)
{
  SMStateBase *returnState = STATE_NULL;

  if (lBalanceDue <= 0)
  {
	  returnState = STATE_USSF_FALSE;
  }

  return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_Intervention_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	// This will cause a visual verify either immediate or deferred.
	// Parms:
	// 1. now, later, immediate, deferred.
	// 2. type (only visual verify for now).
	// 3-7. Informational Text.
	CString csActionCommand, csActionParm1, csActionParm2;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);
    bool bImmediate = true;
	CString csType = _T("VISUAL");
	CString csText = _T("");

	// Set the FL State.
	if (lActionParmCount > 0)
	{
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm1);
		USSFutil.ResolveSymbol(csActionParm1, csActionParm1);
		if ((!csActionParm1.CompareNoCase(_T("later"))) ||
			(!csActionParm1.CompareNoCase(_T("deferred"))) ||
			(!csActionParm1.CompareNoCase(_T("delayed"))))
		{
			bImmediate = false;
		}

		if (lActionParmCount > 1)
		{
			// Get the Type!
			USSFutil.GetTransactionVariable(_T("ActionParm02"), csActionParm2);
			USSFutil.ResolveSymbol(csActionParm2, csActionParm2);
			csType = csActionParm2;
		}

		if (lActionParmCount > 2)
		{
			// Get the Text!
			CString csTempstr;
			USSFutil.GetTransactionVariable(_T("ActionParm03"), csTempstr);
			USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
			csText = csTempstr + _T(" ");
			USSFutil.GetTransactionVariable(_T("ActionParm04"), csTempstr);
			USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
			csText += csTempstr + _T(" ");
			USSFutil.GetTransactionVariable(_T("ActionParm05"), csTempstr);
			USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
			csText += csTempstr + _T(" ");
			USSFutil.GetTransactionVariable(_T("ActionParm06"), csTempstr);
			USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
			csText += csTempstr + _T(" ");
			USSFutil.GetTransactionVariable(_T("ActionParm07"), csTempstr);
			USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
			csText += csTempstr + _T(" ");
			USSFutil.GetTransactionVariable(_T("ActionParm08"), csTempstr);
			USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
			csText += csTempstr + _T(" ");
			USSFutil.GetTransactionVariable(_T("ActionParm09"), csTempstr);
			USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
			csText += csTempstr + _T(" ");
		}
	}

	if (!csType.CompareNoCase(_T("VISUAL")))
	{
		lUnapprovedVisualVerifyItems++;

		/*
		CString csDesc;
		// csErrDescription may hold an alternate VV description.
		if (io.d.csErrDescription.IsEmpty())
		{
		  csDesc = ps.GetPSText(RA_VISUAL_VALIDATE_ID, SCOT_LANGUAGE_PRIMARY);
		}
		else
		{
		  csDesc = io.d.csErrDescription + io.d.csDescription;
		}
		*/

		CString csAnchorStateName = getSCOTStateName();//Tar 247393
		CString csInstruction;
		csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csText, ps.GetPSText(RA_LANEBTT_VISUALVALIDATIONITEM, SCOT_LANGUAGE_PRIMARY), csAnchorStateName, _T(""), ps.GetPSText(RA_THIRDLINE_VISUALVALIDATIONITEM, SCOT_LANGUAGE_PRIMARY));
		trace(L7, _T("+ra.OnVisualValidationItem, Message=%s"), csInstruction);
		ra.OnVisualValidationItem(csInstruction, FALSE);
		trace(L7, _T("-ra.OnVisualValidationItem"));

		// Save Text into USSF_VISUAL_APPROVAL
		USSFutil.SetTransactionVariable(_T("USSF_VISUAL_APPROVAL"), csText);
	}
	else if (!csType.CompareNoCase(_T("FRANKING")))
	{
		USSFutil.SetTransactionVariable(_T("USSF_VISUAL_APPROVAL"), csText);
		// USSF_FRANKINGREQUIRED already set to 1.
		USSFutil.SetTransactionVariable(_T("USSF_FRANKINGREQUIRED"), _T("1"));
	}
	else if (!csType.CompareNoCase(_T("GIFTCARD")))
	{
		USSFutil.SetTransactionVariable(_T("USSF_VISUAL_APPROVAL"), csText);
		// USSF_FRANKINGREQUIRED already set to 1.
		USSFutil.SetTransactionVariable(_T("USSF_GIFTCARDSALE"), _T("1"));
	}
	else if (!csType.CompareNoCase(_T("PROMO")))
	{
		//lUnapprovedVisualVerifyItems++;
		//lUnapprovedUSSFItems++;

		//CString csAnchorStateName = _T("Finalization");//Tar 247393
		//CString csInstruction;
		//csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csText, _T("Promotion"), csAnchorStateName, _T(""), csText);
		//trace(L7, _T("+ra.OnVisualValidationItem, Message=%s"), csInstruction);
		//ra.OnVisualValidationItem(csInstruction, FALSE);
		//trace(L7, _T("-ra.OnVisualValidationItem"));

		// Save Text into USSF_VISUAL_APPROVAL
		USSFutil.SetTransactionVariable(_T("USSF_VISUAL_APPROVAL"), csText);

		USSFutil.SetTransactionVariable(_T("USSF_PROMOFLAG"), _T("ON"));
	}
	else if (!csType.CompareNoCase(_T("GCEXCEPTION")))
	{
		//lUnapprovedVisualVerifyItems++;
		//lUnapprovedUSSFItems++;

		//CString csAnchorStateName = _T("Finalization");//Tar 247393
		//CString csInstruction;
		//csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csText, _T("Promotion"), csAnchorStateName, _T(""), csText);
		//trace(L7, _T("+ra.OnVisualValidationItem, Message=%s"), csInstruction);
		//ra.OnVisualValidationItem(csInstruction, FALSE);
		//trace(L7, _T("-ra.OnVisualValidationItem"));

		// Save Text into USSF_VISUAL_APPROVAL
		USSFutil.SetTransactionVariable(_T("USSF_VISUAL_APPROVAL"), csText);

		USSFutil.SetTransactionVariable(_T("USSF_GCEXCEPTFLAG"), _T("ON"));
	}
	else	// CUSTOM!
	{
		lUnapprovedUSSFItems++;

		// This could change depending on desired behaviour of HHRAP.
		CString csAnchorStateName = getSCOTStateName();//Tar 247393
		CString csInstruction;
		csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csText, ps.GetPSText(RA_LANEBTT_VISUALVALIDATIONITEM, SCOT_LANGUAGE_PRIMARY), csAnchorStateName, _T(""), ps.GetPSText(RA_THIRDLINE_VISUALVALIDATIONITEM, SCOT_LANGUAGE_PRIMARY));
		trace(L7, _T("+ra.OnVisualValidationItem, Message=%s"), csInstruction);
		ra.OnVisualValidationItem(csInstruction, FALSE);
		trace(L7, _T("-ra.OnVisualValidationItem"));

		// Save Text into USSF_VISUAL_APPROVAL
		USSFutil.SetTransactionVariable(_T("USSF_CUSTOM_APPROVAL"), csText);
		USSFutil.SetTransactionVariable(_T("USSF_APPROVAL_TYPE"), csType);
	}

	if (bImmediate)
	{
		CREATE_AND_DISPENSE(WaitForApproval)(false,false);
	}

	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_SetPSXVariable_Action(SMStateBase *pOpState, vector<CString> vParms)
{
  SMStateBase *returnState = STATE_NULL;

  CString csActionCommand, csActionParm1, csActionParm2, csActionParm3;
  long lActionParmCount;
  USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
  USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

  // Set the FL State.
  if (lActionParmCount > 1)
  {
	USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm1);
	USSFutil.GetTransactionVariable(_T("ActionParm02"), csActionParm2);
	USSFutil.ResolveSymbol(csActionParm1, csActionParm1);
	USSFutil.ResolveSymbol(csActionParm2, csActionParm2);

	USSFutil.SetTransactionVariable(csActionParm1, csActionParm2);
  }
  else
  {
	trace(L6, _T("SetPSXVariable Parameters Incomplete"));
  }

  return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_GetPSXVariable_Action(SMStateBase *pOpState, vector<CString> vParms)
{
  SMStateBase *returnState = STATE_NULL;

  CString csActionCommand, csActionParm1, csActionParm2, csActionParm3;
  long lActionParmCount;
  USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
  USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

  // Set the FL State.
  if (lActionParmCount > 1)
  {
	USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm1);
	USSFutil.GetTransactionVariable(_T("ActionParm02"), csActionParm2);
	USSFutil.ResolveSymbol(csActionParm1, csActionParm1);
	USSFutil.ResolveSymbol(csActionParm2, csActionParm2);

	CString csTempstr;
	USSFutil.GetTransactionVariable(csActionParm1, csTempstr);
	USSFutil.StoreData(csTempstr, csActionParm2);
  }
  else
  {
	trace(L6, _T("GetPSXVariable Parameters Incomplete"));
  }

  return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_FreezeState_Action(SMStateBase *pOpState, vector<CString> vParms)
{
  SMStateBase *returnState = STATE_NULL;

  trace(L5, _T(">>> STATE FROZEN"));
  fStateFreeze = true;
  fStateAssist = true;

  return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_UnFreezeState_Action(SMStateBase *pOpState, vector<CString> vParms)
{
  SMStateBase *returnState = STATE_NULL;

  trace(L5, _T("<<< STATE UNFROZEN"));

  fStateFreeze = false;
  fStateAssist = false;

  return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_IfFrankingPending_Action(SMStateBase *pOpState, vector<CString> vParms)
{
  SMStateBase *returnState = STATE_NULL;
  CString csTempstr;

  USSFutil.GetTransactionVariable(_T("USSF_FRANKINGREQUIRED"), csTempstr);

  bool rc = false;
  if (!csTempstr.CompareNoCase(_T("1")))
  {
	  //USSFutil.SetTransactionVariable(_T("USSF_FRANKINGREQUIRED"), _T("0"));

	  CString csAnchorStateName = _T("Finalization");//Tar 247393
	  CString csInstruction;
	  CString csText;
	  USSFutil.GetTransactionVariable(_T("USSF_FRANKINGDATA"), csText);
	  if (csText.GetLength() == 0)
	  {
		USSFutil.GetTransactionVariable(_T("USSF_VISUAL_APPROVAL"), csText);
	  }
	  else
	  {
		USSFutil.SetTransactionVariable(_T("USSF_VISUAL_APPROVAL"), csText);
	  }

	  USSFutil.SetTransactionVariable(_T("USSF_APPROVAL_TYPE"), _T("FRANKING"));
	  //USSFutil.SetTransactionVariable(_T("USSF_APPROVAL_RESULT"), pTDB -> Get(TDB_SAFEWAY_FRANKING_APPROVAL_RESULT));
	  //USSFutil.SetTransactionVariable(_T("USSF_APPROVAL_INSTRUCTION"), pTDB -> Get(TDB_SAFEWAY_FRANKING_APPROVAL_INSTRUCTION));

	  //ra.OnNormalItem(_T("[Alert] Suspend Required to Frank Award Document"));
	  csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csText, _T("Franking"), csAnchorStateName, _T(""), csText);
	  trace(L7, _T("+ra.OnRequestSuspend, Message=%s"), csInstruction);
	  ra.OnVisualValidationItem(csInstruction, FALSE);
	  ra.OnRequestSuspend(csInstruction);
	  trace(L7, _T("-ra.OnRequestSuspend"));

	  lUnapprovedVisualVerifyItems++;
	  bSuspendRequested = true;
	  rc = true;
  }

  if (rc == false)
  {
	  returnState = STATE_USSF_FALSE;
  }

  return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_HandleUSSFApproval_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	SMStateBase *returnState = STATE_NULL;
	CString csTempstr;

	CString csType, csResult, csInstruction;
	USSFutil.GetTransactionVariable(_T("USSF_APPROVAL_TYPE"), csType);
	//USSFutil.GetTransactionVariable(_T("USSF_APPROVAL_RESULT"), csResult);
	//USSFutil.GetTransactionVariable(_T("USSF_APPROVAL_INSTRUCTION"), csInstruction);

	// What should we do with it?
	trace(L5, _T("Checking for USSF Approvals Pending."));
	
	if (csType.GetLength() > 0)
	{
		CString csText;
		csText.Format(_T("$POSEVENT$ %sPENDING"), csType);
		trace(L5, csText);
		USSFutil.SendScotMsg(TB_DEV_DISPLAYPRIMARY, csText.GetLength(), (void *) (LPCTSTR) csText);
	}
	else
	{
		trace(L5, _T("No USSF Approvals Pending."));
	}

	return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_SuspendTransaction_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	SMStateBase *returnState = STATE_NULL;
	lVisualVerifyItems = 0;
	returnState = pOpState -> TBSuspendTransaction();
	return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_CancelTransaction_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	SMStateBase *returnState = STATE_NULL;
	lVisualVerifyItems = 0;
	returnState = pOpState -> TBCancelTransaction();
	return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_GetTotalDetails_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	SMStateBase *returnState = STATE_NULL;
	lVisualVerifyItems = 0;
	returnState = TBGetTotalDetails();
	return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_GoTo_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	SMStateBase *returnState = STATE_NULL;
	CString csActionCommand, csActionParms, csTempstr;
	long lActionParmCount;
	USSFutil.GetTransactionVariable(_T("ActionCommand"), csActionCommand);
	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

	if (lActionParmCount > 0)
	{
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csTempstr);
		USSFutil.ResolveSymbol(csTempstr, csTempstr, _T(""));
		USSFutil.SetTransactionVariable(_T("USSF_GOTO_TARGET"), csTempstr);
		returnState = STATE_USSF_GOTO;
	}
	else
	{
		trace(L5, _T("GoTo Target Not Specified"));
	}
	return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_EndScript_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	SMStateBase *returnState = STATE_USSF_ENDSCRIPT;
	return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_SAWLDBScannerDisable_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	bEASReEnableScanner = false;
	SAWLDBScannerDisable();
	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_SAWLDBScannerEnable_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	bEASReEnableScanner = true;
	SAWLDBScannerEnable();
	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_CashAcceptorEnable_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	DMCashAcceptorEnable(co.csTenderDenominations);
	return STATE_NULL;
}
SMStateBase *SMUSSFManagerBase::USSF_CashAcceptorDisable_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	DMCashAcceptorDisable();
	return STATE_NULL;
}
SMStateBase *SMUSSFManagerBase::USSF_CoinAcceptorEnable_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	DMCoinAcceptorEnable();
	return STATE_NULL;
}
SMStateBase *SMUSSFManagerBase::USSF_CoinAcceptorDisable_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	DMCoinAcceptorDisable();
	return STATE_NULL;
}
SMStateBase *SMUSSFManagerBase::USSF_ScaleEnable_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	DMScaleEnable();
	return STATE_NULL;
}
SMStateBase *SMUSSFManagerBase::USSF_ScaleDisable_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	DMScaleDisable();
	return STATE_NULL;
}

SMStateBase *SMUSSFManagerBase::USSF_DumpUSSFSymbols_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	SMStateBase *returnState = STATE_NULL;
	bool bMeta = true;
	long lActionParmCount = 0;
	CString csActionParm1;

	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

	if (lActionParmCount > 0)
	{
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm1);
		USSFutil.ResolveSymbol(csActionParm1, csActionParm1);
		if ((!csActionParm1.CompareNoCase(_T("nometa")))||
			(!csActionParm1.CompareNoCase(_T("brief")))||
			(!csActionParm1.CompareNoCase(_T("terse"))))
		{
			bMeta = false;
		}
	}

	USSFutil.DumpSymbols(bMeta);

	return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_DumpUSSFSymbolsBySerial_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	SMStateBase *returnState = STATE_NULL;
	bool bMeta = true;
	long lActionParmCount = 0;
	CString csActionParm1;

	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

	if (lActionParmCount > 0)
	{
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm1);
		USSFutil.ResolveSymbol(csActionParm1, csActionParm1);
		if ((!csActionParm1.CompareNoCase(_T("nometa")))||
			(!csActionParm1.CompareNoCase(_T("brief")))||
			(!csActionParm1.CompareNoCase(_T("terse"))))
		{
			bMeta = false;
		}
	}

	USSFutil.DumpSymbolsBySerial(bMeta);

	return returnState;
}

SMStateBase *SMUSSFManagerBase::USSF_DumpUSSFSymbolsByElementID_Action(SMStateBase *pOpState, vector<CString> vParms)
{
	SMStateBase *returnState = STATE_NULL;
	bool bMeta = true;
	long lActionParmCount = 0;
	CString csActionParm1;

	USSFutil.GetTransactionVariable(_T("ActionParmCount"), lActionParmCount);

	if (lActionParmCount > 0)
	{
		USSFutil.GetTransactionVariable(_T("ActionParm01"), csActionParm1);
		USSFutil.ResolveSymbol(csActionParm1, csActionParm1);
		if ((!csActionParm1.CompareNoCase(_T("nometa")))||
			(!csActionParm1.CompareNoCase(_T("brief")))||
			(!csActionParm1.CompareNoCase(_T("terse"))))
		{
			bMeta = false;
		}
	}

	USSFutil.DumpSymbolsByElementID(bMeta);

	return returnState;
}


