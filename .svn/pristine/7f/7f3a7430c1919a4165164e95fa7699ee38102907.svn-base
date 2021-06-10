//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMInSAProgressBase.cpp
//
// Description: The InSAProgress state is used when the transition from 
//			TB_ITEMSOLD to the next Security event, which is the SASERVER WLBD
//			lookup return, and might take some time.  If App doesn't wait long 
//			enough, the DBDClient will return local database info
//
// Author:  Leila Parker
///
//////////////////////////////////////////////////////////////////////////////////////////////////
 
#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header
#include "Common.h"                 // common includes
#include "SMInSAProgress.h"         // this state
#include "SMBagAndEAS.h"			// bag and EAS state
#include "SMSecMisMatchWeight.h"
#include "SMSecUnexpectedDecrease.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("InSAProgressBase")

bool bNeedToCallTimedOutFunction = false;

IMPLEMENT_DYNCREATE(SMInSAProgressBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////////////////////
SMInSAProgressBase::SMInSAProgressBase(	const bool storeMode,
										const bool helpMode)
										: fStoreMode(storeMode),
										  fHelpMode(helpMode)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMInSAProgressBase::Deliver(	const bool storeMode,
											const bool helpMode)
{
  fStoreMode = storeMode;
  fHelpMode = helpMode;

  return this;
}

////////////////////////////////////////////////
SMInSAProgressBase::SMInSAProgressBase()
{
  IMPLEMENT_TIMESTAMP
  nTimeOut = co.getTimeOutSAInProgress();
}

//////////////////////////////////////
SMStateBase  *SMInSAProgressBase::Initialize(void)
{
	if (!g_bNeedLastChanceLookup)	// if the server lookup has returned, go back to bag item and call dept lookup LPM032802
		return createState(BES_BAGANDEAS);

	trace(L4, _T("Initialize. Waiting for WLDB Lookup."));
	bNeedToCallTimedOutFunction = true;
	m_bNeedToDisplayAuxScreen = false; // DON'T display aux screens in this state!
	return STATE_NULL;
}

///////////////////////////////////
void SMInSAProgressBase::UnInitialize(void)
{
	if (bNeedToCallTimedOutFunction)
		TimedOut();
}

////////////////////////////////////
SMStateBase  *SMInSAProgressBase::TimedOut(void) // timeout
{
  trace(L4, _T("+TimedOut"));
  if (g_bNeedLastChanceLookup)	// if the server lookup hasn't returned, then call the local lookup
  {
	trace(L4, _T("Calling LastChanceLookup, WLDB server entries didnt return in %d seconds."), co.getTimeOutSAInProgress());
	g_bNeedLastChanceLookup = false;
	CallLastChanceLookup();
	// TAR 217216 LPM/RJF 11/04/02
	// this only ocurs when the SAserver is down, and TB sells the linked item.
	// the only way this will work is if the linked item is in the same department or 
	// if there are no department overrides.
	if (g_bTBReturnedITEMSOLDforSA && io.d.bLinkedItem)
	{
	    trace(L4, _T("Assuming linked item info arrived before SAserver data, calling dept lookup"));
		SecurityPLUInfo();
	}
  }  
  else
  {
	trace(L4, _T("WLDB Server lookup succeeded. Calling ::SecurityPLUInfo."));
	g_bTBReturnedITEMSOLDforSA = false;	// dont' want to return to InSAProgress for this item
	// tell client database mgr OCX to lookup the department execeptions
	// if the item is not linked
	if( ((!io.d.bLinkedItem)  || (bNeedToCallTimedOutFunction)) && 
		((!io.fVoid) && (!io.d.fCoupon))	 )
	{
		SecurityPLUInfo();
	}
	else
	{
		CString csExplain;
		if (io.fVoid) { csExplain = _T("void item "); }
		if (io.d.fCoupon) { csExplain += _T("coupon "); }
		if (io.d.bLinkedItem) { csExplain += _T("linked item"); }
		trace(L4, _T("TimedOut - Skipping Dept Lookup - because TBs Item Object = %s."), csExplain);
	}
  }

  //ss.SetHoldWeight(false);  //removed on Tom E suggestion since Smart scale is already setting this	// set hold wt to false in InSAProgress instead of SABase TAR198471 Aiqin LPM
  bNeedToCallTimedOutFunction = false;
  trace(L4, _T("-TimedOut"));
  return createState(BES_BAGANDEAS);
}

//////////////////////////////////
SMStateBase  *SMInSAProgressBase::DMScanner(void) // scanned an item
{
  trace(L3, _T("Probable race condition, scanned code: %s, Previous input sent to Sec: %d, fItemIsWaiting: %d"), csDMLastBarCode, nSAPrevSCOTInputID, fItemIsWaiting); 
  if (!fItemIsWaiting)
  {
	csItemSent = csDMLastBarCode;
	fItemIsWaiting = true;            // to know if the item was sold from the attract screen or not.  sell item after signon in S&B
	if (nSAPrevSCOTInputID != UPC_SCANNED) 
	{
	  trace(L3, _T("Sending Security event ID: 1 to fix race condition."));		// 
	  SASendSCOTInput( UPC_SCANNED, csDMLastBarLabel); //security needs to know we scanned an item quickly.  //TAR#121479
	}
  }
  return STATE_NULL;
}

////////////////////////////////////////
// trap this message just so base doesn't give an error msg,
// nothing else to do here
SMStateBase  *SMInSAProgressBase::DMCardReader(void)
{
  return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMInSAProgressBase::OnBackToLGW(void)
{
	// TAB re-enable scanner and clear Wait For Approval State
	TABReEnableScannerAndClearWaitForApprovalState();
	return STATE_NULL;
}

///////////////////////////////////////////
// EAS Motion Sensor detected motion ended
///////////////////////////////////////////
SMStateBase  *SMInSAProgressBase::DMdf_EASNoMotion(void) 
{
	// The sensor has been detected, not waiting EAS any more
	SASendSCOTInput(PRODUCT_TAG_DEACTIVATED, io.csItemCode, 0, 0, 0, 0, 0);
	// The sensor has been detected, remember it 
	m_bEASHappened = true; //tar 224210 - fixed by Ruby
	return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMInSAProgressBase::OnWtIncreaseNotAllowed()	 
{
	DMSaySecurity( SEC_NONE, _T("blank.wav") );

	//The text here is just the name of the section in scotdataentry.ini.  Don't need to be in SCOTMSGE.DAT
	cDataEntry.FromConfigData(DATAENTRYINIFILE, _T("IncorrectItemWeight"));
	m_View = DATANEEDED_MISMATCHWEIGHT;
	CString csInstruction, csDesc;
	CString csExpectedWts = GetExpectedWts();
	csDesc.Format(ps.GetPSText(RA_DESCRIPTIONINFO, SCOT_LANGUAGE_PRIMARY), io.d.csDescription, csScaleUnits, FmtWtToString(g_lWtDelta), csScaleUnits, csExpectedWts);
	csInstruction.Format(ps.GetPSText(RA_WEIGHTINFO, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_SECURITY_EVENT, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(RA_WTMISMATCH, SCOT_LANGUAGE_PRIMARY), csExpectedWts, csScaleUnits, FmtWtToString(g_lWtDelta), csScaleUnits);
	trace(L6, _T("+ra.OnRAPDataNeeded %s"), csInstruction);
	ra.OnRAPDataNeeded(csInstruction, cDataEntry.ToString(), m_View);
				  //MisMatch ItemExceptions for Reporting 3.1 
				  trace(L7, _T("+rp.ItemException"));
			      CString csItemSku = io.csOrigCode;
				  long lPrice = io.lExtPriceEntered;//lUnitPrice;
				  CString csMisMatchDesc = io.csOrgItemDescription;//Always use the orginal item description 
				  CString csObservedWts = FmtWtToString(g_lWtDelta);
				  rp.ItemException(csMisMatchDesc,csItemSku,lPrice, csObservedWts, csExpectedWts);
				  trace(L7, _T("-rp.ItemException"));
				  //MisMatch ItemExceptions for Reporting 3.1 
	//NN 080602 RFC 210177 Log WLDB mismatch
	if (co.fOperationsLogWLDBmismatch)
	{
		long lWeight;
		lWeight = g_lWtDelta;  //NN 090502 TAR 213930 Get current mismatch  Wt. 
		LogWLDBWeightMismatch(lWeight);
	}
	//NN 080902 RFC 210177 end

	trace(L6, _T("-ra.OnRAPDataNeeded"));

	//unmatched weights while on the InSAProgress mean a mismatched wt so return to BagAndEAS, else if cleared, return to s&b
	CREATE_AND_DISPENSE(SecMisMatchWeight)(BES_BAGANDEAS, getNextStateForGoodItem());
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMInSAProgressBase::OnWtDecrease()
{
	DMSaySecurity( SEC_NONE, _T("blank.wav") );
	//unexpected decreases while on the InSAProgress not allowed, return to BagAndEAS if customer fixes, else if cleared, return to s&b
	CREATE_AND_DISPENSE(SecUnexpectedDecrease)(BES_BAGANDEAS, getNextStateForGoodItem());
}

//229084 created a Base function for getNextStateForGoodItem LPM022503

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMInSAProgressBase::OnDataBaseInfoComplete()
{
	return TimedOut();	
}
// TAR 246429
SMStateBase *SMInSAProgressBase::PSButtonHelp()
{
    trace(L6,_T("SMInSAProgressBase::PSButtonHelp - ignore help button press"));
	return STATE_NULL;    
}
// TAR 246429