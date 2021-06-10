//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmmEditAddBase.CPP
//
// TITLE: Class implementation for store mode Security Maintenance Mode Edit main screen
//
// AUTHOR:    Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // common includes
#include "SMState.h"				// Base state
#include "SMSmmEditAdd.h"			// This state
#include "SMSmmAddWeight.h"			// Possible next state
#include "SMSmmEditWtExp.h"			// Possible next state
#include "SMSmmEditWtSubCk.h"		// Possible next state
#include "SMSmmEditWtTolerance.h"	// Possible next state
#include "SMSmSystemFunctions.h"

#include "DMProcedures.h"
DLLEXPORT extern DMProcedures dm;   // Global DM object

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmmEditAddBase")

IMPLEMENT_DYNCREATE(SMSmmEditAddBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SmmEditAdd)

DEFINE_TIMESTAMP

//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
SMSmmEditAddBase::SMSmmEditAddBase()
{
  IMPLEMENT_TIMESTAMP
}

//////////////////////////////////////////////
// Initialize
//////////////////////////////////////////////
SMStateBase  *SMSmmEditAddBase::Initialize(void)
{

  bool bNeedList = false;
  bOkToDisplayBagScaleInfo = true;
  ps.SetFrame(_T("SmmEditAdd"));

  ps.ShowSMTotal(false);
  DMScannerEnable();

  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmmEditAdd") );	// RFC 330050
  ps.Prompt(PS_NOTEXT); // TAR 227482
  if (bSMMFailure)		// Failure returned from the Security Maintenance Mode event sent to security
  {
      DMScannerDisable();
  	  // if the failure occurred while updating something, then the msg should reflect that
	  if (bSMMFailureActionCancel)
      {
          ps.SetLeadthruText(MSG_SMM_OFFLINE_CANCELLED, PS_BLANK); 
      }
      else
      {
          ps.SetLeadthruText(MSG_SMM_OFFLINE, PS_BLANK);
      }
      ps.Button(_T("SMButton1"),BTT_SMM_ADDWT,false);
      ps.Button(_T("SMButton2"),BTT_SMM_ADDZEROWT,false);
      ps.Button(_T("SMButton3"),BTT_SMM_DELWT,false);
      ps.Button(_T("SMButton4"),BTT_SMM_EDIT_WTEXP,false);
      ps.Button(_T("SMButton5"),BTT_SMM_EDIT_WTSUBCK,false);
      ps.Button(_T("SMButton6"),BTT_SMM_EDIT_WTTOL,false);
      ps.ResetSMMItemList();
  }
  else
  {
	if (bSMMPreScanState )
	{
        // NewUI
		ps.Button(_T("SMButton1"),BTT_SMM_ADDWT,false);
		ps.Button(_T("SMButton2"),BTT_SMM_ADDZEROWT,false);
		ps.Button(_T("SMButton3"),BTT_SMM_DELWT,false);
		ps.Button(_T("SMButton4"),BTT_SMM_EDIT_WTEXP,false);
		ps.Button(_T("SMButton5"),BTT_SMM_EDIT_WTSUBCK,false);
		ps.Button(_T("SMButton6"),BTT_SMM_EDIT_WTTOL,false);
        // End NewUI
		ps.SetLeadthruText(LTT_SMMEDITADD_INSTR, PS_BLANK);   
	}
	else  
	{
		if (bIsSMMItemAdded)
		{
			ps.SetLeadthruText(MSG_SMM_ITEMADDED, PS_BLANK);
            // NewUI
			//+ TAR#327528 -- jrparro commented
			//ps.Button(_T("SMButton1"),BTT_SMM_ADDWT,false);
			//ps.Button(_T("SMButton2"),BTT_SMM_ADDZEROWT,false);
			//ps.Button(_T("SMButton3"),BTT_SMM_DELWT,true);
			//- TAR#327528 -- jrparro commented
            // End NewUI
			//SMSmmEditAddBase::DMScanner();

			//+jrparro TAR#327528 -- to enable the multiple add weight
			ps.Button(_T("SMButton1"),BTT_SMM_ADDWT,true);
			ps.Button(_T("SMButton2"),BTT_SMM_ADDZEROWT,true);
			//-jrparro TAR#327528			
		}
		else if (bIsSMMItemDeleted)
		{
			ps.SetLeadthruText(MSG_SMM_ITEMDELETED, PS_BLANK);	
            // NewUI
			ps.Button(_T("SMButton1"),BTT_SMM_ADDWT,true);
			ps.Button(_T("SMButton2"),BTT_SMM_ADDZEROWT,true);
            // End NewUI
		}
		else if (0 == lSMMWeightCount)  
		{
			ps.SetLeadthruText(TXT_SMM_EDITADD_INSTR, PS_BLANK);  
            // NewUI
			ps.Button(_T("SMButton1"),BTT_SMM_ADDWT,true);
			ps.Button(_T("SMButton2"),BTT_SMM_ADDZEROWT,true);
            // End NewUI
		}
		else
		{
			ps.SetLeadthruText(TXT_SMM_EDITADD_INSTR, PS_BLANK);  
			ps.SetLTTinto2(MSG_SMM_DELETEZERO);//TD RFC 203307 instructional txt  
            // NewUI
			ps.Button(_T("SMButton1"),BTT_SMM_ADDWT,true);
			ps.Button(_T("SMButton2"),BTT_SMM_ADDZEROWT,true);
			ps.Button(_T("SMButton3"),BTT_SMM_DELWT,true);
            // End NewUI
		}
		ps.Prompt(TXT_SMM_EDITADD_PROMPT);  
        // NewUI
		ps.Button(_T("SMButton4"),BTT_SMM_EDIT_WTEXP,true); // changed the numbering of the buttons 4,5,6
		ps.Button(_T("SMButton5"),BTT_SMM_EDIT_WTSUBCK,true);//was missing the delete button T.d 01/28/02
		ps.Button(_T("SMButton6"),BTT_SMM_EDIT_WTTOL,true);
        // end NewUI
		bNeedList = true;
	}  
  }  
  if (bWLDBNotActive)
  {
    // NewUI
	ps.Button(_T("SMButton1"),BTT_SMM_ADDWT,false);
	ps.Button(_T("SMButton2"),BTT_SMM_ADDZEROWT,false);
    // End NewUI
  }
  ps.SetTitle(LTL_SMM_EDITADD); 
  //ps.SetButtonText(6, PS_NOTEXT); T.d & CM sometimes have a 6th button
  ps.AllowInput(MAX_DIGITS,false,false);	// allow the EchoInput() func to accept input

  if (bNeedList)
  {
	ps.SMShowSMMItemList();			// show the list in case get here from edit wt exp or edit wt sub ck
  }
  ps.ShowFrame();
  return STATE_NULL;                // stay in SMSmmEditAddBase state
}

/////////////////////////////////////
// DMScanner - something was scanned
/////////////////////////////////////
SMStateBase  *SMSmmEditAddBase::DMScanner(void)
{

  bOkToDisplayBagScaleInfo = true;
  // +TAR 321239
  io.cScanType = csDMLastBarType; 	
  io.b_gIsThisItemPriceEmbedded = io.IsEmbeddedPrice(csDMLastBarCode);
  // -TAR 321239
  
  // changed bar code label to plain bar code LPM120302
  if (io.HasSupplementalCode(csDMLastBarCode))		//determines if this is a supplemental bar code 
  {
	csSMMScannedUPC = io.csItemCodeSupplementalCodeTruncated;	
  }
  //RFC239992//else if ((! io.IsEmbeddedPrice(csDMLastBarCode)) && (io.b_gHasCustomCodeBeenFound)) 
  else if(io.b_gHasCustomCodeBeenFound)//RFC239992
  {
	csSMMScannedUPC = io.csItemCodeVariablePartZeroed;
  }
  else
  {
	csSMMScannedUPC = csDMLastBarCode;
  }

  csItemSent = csSMMScannedUPC;
  CString csCode = AdjustBarCode();
  csSMMScannedUPC = csCode;    // TAR 236796

  ResetMLBCValues();
  // Tell the security sub-system to query for the item
  CKeyValueParmList parmList;
  CKeyValue<const _TCHAR*>::Put(parmList, KEY_SMM_OPERATION, KEY_SMM_OPERATION_QUERY);
  CKeyValue<const _TCHAR*>::Put(parmList, KEY_UPC,csSMMScannedUPC );
  CKeyValue<bool>::Put(parmList, KEY_SMM_PRICE_EMBEDDED, io.b_gIsThisItemPriceEmbedded);

  _bstr_t bstrParmList((const _TCHAR *)parmList);
  long lrc = SecMgr.SecurityMaintenance(lSecMgrCookie, bstrParmList);

  bIsSMMItemAdded = false;
  bIsSMMItemDeleted = false;
  bSMMPreScanState = false;
  RETURNSTATE(SmmEditAdd)	//to refresh the buttons
}

/////////////////////////////////////////////
// PSButton1 - Add Weight Entry
/////////////////////////////////////////////
SMStateBase  *SMSmmEditAddBase::PSButton1(void)
{
  // + TAR 277988
  if (io.IsEmbeddedPrice(csDMLastBarCode) )
  {
	ps.Button(_T("SMButton1"),BTT_SMM_ADDWT,false);
	return STATE_NULL;
  }
  // - TAR 277988
  ps.ClearSMMItemList();
  // Tell the security sub-system to add weight for the item
  CKeyValueParmList parmList;
  CKeyValue<const _TCHAR*>::Put(parmList, KEY_SMM_OPERATION, KEY_SMM_OPERATION_ADD_WEIGHT);
  CKeyValue<const _TCHAR*>::Put(parmList, KEY_UPC,csSMMScannedUPC );
  CKeyValue<bool>::Put(parmList, KEY_SMM_PRICE_EMBEDDED, io.IsEmbeddedPrice(csSMMScannedUPC) ); 
  if (io.IsEmbeddedPrice(csSMMScannedUPC) )
  {
	// get the price per weight unit for price embedded items
	long lPrice = SASMMConvertToPPWU();
	CKeyValue<long>::Put(parmList, KEY_SMM_PPWU, lPrice );
  }
  
  _bstr_t bstrParmList((const _TCHAR *)parmList);
  long lrc =  SecMgr.SecurityMaintenance(lSecMgrCookie, bstrParmList);
  // Need to decide where we want to go, since the security sub-system knows
  // that a weight was placed on the scale???? if wt is not on scale go to
  // addweight state otherwise go to editadd state

#ifndef NewSec
  lSMMWLDBNumOfEntries = 0;

  long lWTorPPWU;
   if (g_lSMMWTTOADD)
  {
	SASMMDisplayTolInfo();
	// embedded price must be divided by the bag scale increase
	if (bIsSMMitemTolPPWU)
	{
		long lPrice = SASMMConvertToPPWU();
		lWTorPPWU = (lPrice  * 1000) / g_lSMMWTTOADD;
	}
	else
	{
		lWTorPPWU = g_lSMMWTTOADD;
	}

	//determine the tolerance here based on the weight received
	// not using the tolerance options here, using Std Dev from the WtInfo object LPM120302
	/*
	if (lWTorPPWU > co.nSmartScaleDefaultHeavyWtLimit)
		lSMMglWtTol = co.nSmartScaleDefaultHeavyWtTol;
	else if( (lWTorPPWU <= co.nSmartScaleDefaultHeavyWtLimit) && (lWTorPPWU >= co.nSmartScaleDefaultMedWtLimit) )
		lSMMglWtTol = co.nSmartScaleDefaultMedWtTol;
	else
		lSMMglWtTol = co.nSmartScaleDefaultLightWtTol;
	*/


	// SMM_SEC_CALC means either we're not using it yet and/or security knows the info already
	// Set approval field to SMM_True to always approve entry when SMM mode
	// Tally field is hard coded to 10
	if (!(lWTorPPWU <= 0))
	{

		if (lSMMglWtTol <= 0) lSMMglWtTol = 20; // CMal/RL 090602 - ensure Zero Wts have a positive tol.
		long lOverrideTolWt = 1000000 + lSMMglWtTol; // the 1000000 lets the Client know we're overriding (see clientdbmgr) LPM/CM090402
		// Dual-use the lAppr to send in Tolerance during SMM Add/Update  LPM/CM090402 - TAR 213762 - 
		// override the lAppr field (was SMM_TRUE) to secretly pass in the 'correct' default tolerance
		 bool bRes;
		bool bRes = dbMgr.UpdateSMMWLDBwt(SMM_ADD, csSMMScannedUPC, SMM_SEC_CALC, lWTorPPWU, lOverrideTolWt, 10, SMM_SEC_CALC, SMM_SEC_CALC, io.IsEmbeddedPrice(csSMMScannedUPC));
		if (!bRes)
		{
			bSMMFailureActionCancel = true;
			return SASMMSecOffline();
		}
	}

	bIsSMMItemAdded = true;
	RETURNSTATE(SmmEditAdd)	//to refresh the buttons
  }
  //bIsSMMItemAdded = true; jrparro commented - TAR#322805

#endif 

  //+jrparro TAR#322805 - need to set bIsSMMItemAdded in here
  bIsSMMItemAdded = true;  
  bIsSMMItemDeleted = false;
  //-jrparro TAR#322805

  RETURNSTATE(SmmAddWeight)

}

/////////////////////////////////////////////
// PSButton2 - Add Zero Weight Entry
/////////////////////////////////////////////
SMStateBase  *SMSmmEditAddBase::PSButton2(void)
{
  ps.ClearSMMItemList();
  // Tell the security sub-system to add weight for the item
  CKeyValueParmList parmList;
  CKeyValue<const _TCHAR*>::Put(parmList, KEY_SMM_OPERATION, KEY_SMM_OPERATION_ADD_ZERO);
  CKeyValue<const _TCHAR*>::Put(parmList, KEY_UPC,csSMMScannedUPC );
  CKeyValue<bool>::Put(parmList, KEY_SMM_PRICE_EMBEDDED, io.IsEmbeddedPrice(csSMMScannedUPC) ); 
  long lPrice = 0;
  CKeyValue<long>::Put(parmList, KEY_SMM_PPWU, lPrice );
  _bstr_t bstrParmList((const _TCHAR *)parmList);
  long lrc =  SecMgr.SecurityMaintenance(lSecMgrCookie, bstrParmList);
 
  //+added also by Taraneh - TAR#322805
  bIsSMMItemDeleted = false;
  bIsSMMItemAdded = false;
  //-added also by Taraneh - TAR#322805

  RETURNSTATE(SmmEditAdd);
}

/////////////////////////////////////////////
// PSButton3 - Delete Weight Entries
/////////////////////////////////////////////
SMStateBase  *SMSmmEditAddBase::PSButton3(void)
{
  ps.ClearSMMItemList();
    // Tell the security sub-system to add weight for the item
  CKeyValueParmList parmList;
  CKeyValue<const _TCHAR*>::Put(parmList, KEY_SMM_OPERATION, KEY_SMM_OPERATION_DELETE_ALL_WEIGHTS);
  CKeyValue<const _TCHAR*>::Put(parmList, KEY_UPC,csSMMScannedUPC );
  CKeyValue<bool>::Put(parmList, KEY_SMM_PRICE_EMBEDDED, io.IsEmbeddedPrice(csSMMScannedUPC) ); 
  _bstr_t bstrParmList((const _TCHAR *)parmList);
  long lrc =  SecMgr.SecurityMaintenance(lSecMgrCookie, bstrParmList);

  bIsSMMItemDeleted = true;
  bIsSMMItemAdded = false;

  RETURNSTATE(SmmEditAdd)	//to refresh the buttons
}

/////////////////////////////////////////////
// PSButton4 - Edit Weight Expectation
/////////////////////////////////////////////
SMStateBase  *SMSmmEditAddBase::PSButton4(void)
{
  ps.ClearSMMItemList();
  RETURNSTATE(SmmEditWtExp)
}

/////////////////////////////////////////////
// PSButton5 - Edit Weight Substitution Check
/////////////////////////////////////////////
SMStateBase  *SMSmmEditAddBase::PSButton5(void)
{
  ps.ClearSMMItemList();
  RETURNSTATE(SmmEditWtSubCk)
}

/////////////////////////////////////////////
// PSButton6 - Edit Weight Tolerance
/////////////////////////////////////////////
SMStateBase  *SMSmmEditAddBase::PSButton6(void)
{
  ps.ClearSMMItemList();
  RETURNSTATE(SmmEditWtTolerance)
}

/////////////////////////////////////////////
// PSButton8 - Go Back
/////////////////////////////////////////////
SMStateBase  *SMSmmEditAddBase::PSButton8(void)
{
  ps.ResetSMMItemList();
  // Tell the security sub-system that we are exiting Security Maintenance mode
  CKeyValueParmList parmList;
  // TAR 342411 start - send cancel in case DB operations haven't completed
  CKeyValue<const _TCHAR*>::Put(parmList, KEY_SMM_OPERATION, KEY_SMM_OPERATION_CANCEL);
  _bstr_t bstrParmListCancel((const _TCHAR *)parmList);
  SecMgr.SecurityMaintenance(lSecMgrCookie, bstrParmListCancel);  
  parmList.Clear();
  // TAR 342411 end
  CKeyValue<const _TCHAR*>::Put(parmList, KEY_SMM_OPERATION, KEY_SMM_OPERATION_END);
  _bstr_t bstrParmList((const _TCHAR *)parmList);
  SecMgr.SecurityMaintenance(lSecMgrCookie, bstrParmList);  

  SASendSCOTInput( STORE_MODE );
  bSecMaintMode = false;	// must set this to false AFTER going into Store Mode by calling SASendSCOTInput
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmAuthorization") );  

  //Return to current store mode menu state instead of SmSystemFunctions
  return STATE(StoreModeMenu)::CreateStoreModeMenu();
}

////////////////////////////////////////////
// UnInitialize 
////////////////////////////////////////////
void SMSmmEditAddBase::UnInitialize()
{
	ps.Prompt(PS_NOTEXT);
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmmEditAdd") );	// RFC 330050
	SMStoreModeStateBase::UnInitialize();
}

////////////////////////////////////////////
// *** NewUI  - aiqin *** //
// PSReceiptUp
////////////////////////////////////////////
SMStateBase  *SMSmmEditAddBase::PSReceiptUp(void)
{
  COleVariant vReturn, vScrollMode;
  vScrollMode = (long) UI::PageUp;
  m_pAppPSX->SendCommand(_T("SMMItemList"), UI::COMMANDSCROLL, vReturn, 1, vScrollMode);
  return STATE_NULL;
}

////////////////////////////////////////////
// PSReceiptDown
////////////////////////////////////////////
SMStateBase  *SMSmmEditAddBase::PSReceiptDown(void)
{
  COleVariant vReturn, vScrollMode;
  vScrollMode = (long) UI::PageDown;
  m_pAppPSX->SendCommand(_T("SMMItemList"), UI::COMMANDSCROLL, vReturn, 1, vScrollMode);  return STATE_NULL;

}// *** end of NewUI *** //
