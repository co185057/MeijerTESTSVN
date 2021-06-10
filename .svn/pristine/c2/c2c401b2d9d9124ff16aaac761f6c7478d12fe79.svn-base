//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmmAddWeightBase.CPP
//
// TITLE: Class implementation for store mode Security Maintenance Mode Add Weight
//
// AUTHOR:    Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // common includes
#include "SMState.h"                // Base state
#include "SMSmmAddWeight.h"		// This state
#include "SMSmmEditAdd.h"		// Prev state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmmAddWeightBase")

IMPLEMENT_DYNCREATE(SMSmmAddWeightBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SmmAddWeight)

DEFINE_TIMESTAMP

//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
SMSmmAddWeightBase::SMSmmAddWeightBase()
{
  IMPLEMENT_TIMESTAMP
}

//////////////////////////////////////////////
// Initialize
//////////////////////////////////////////////
SMStateBase  *SMSmmAddWeightBase::Initialize(void)
{

  DMScannerDisable();
  // NewUI
  ps.SetFrame(_T("SmmAddWeight"));
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmmAddWeight") );

  // End NewUI
  //TD 05/29/02 TAR 205490 Begin
	if (dm.fStateTakeawayBelt)
		ps.SetLeadthruText(TXT_SMM_ITEMWTBELT_INSTR, PS_BLANK);//TD 05/29/02 TAR 205490
	else 
		ps.SetLeadthruText(TXT_SMM_ITEMWT_INSTR, PS_BLANK);
//TD 05/29/02 TAR 205490  End
  ps.ShowFrame();
  return STATE_NULL;                // stay in SMSmmAddWeightBase state
}


/////////////////////////////////////////////
// Bag Scale event called from SAParse
/////////////////////////////////////////////
SMStateBase  *SMSmmAddWeightBase::SASMMBagScaleEvent()
{
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
  long lrc = SecMgr.SecurityMaintenance(lSecMgrCookie, bstrParmList);
  // Need to decide where we want to go EditAdd state or staty here ???
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
	// if not lWTorPPWU equals to less than or equal to
	if(!(lWTorPPWU <= 0))
	{
		long lOverrideTolWt = 1000000 + lSMMglWtTol; // the 1000000 lets the Client know we're overriding (see clientdbmgr) LPM/CM090402
		// Dual-use the lAppr to send in Tolerance during SMM Add/Update  LPM/CM090402 - TAR 213762 - 
		// override the lAppr field (was SMM_SEC_CALC) to secretly pass in the 'correct' default tolerance
		bool bRes;

		bool bRes = dbMgr.UpdateSMMWLDBwt(SMM_ADD, csSMMScannedUPC, SMM_SEC_CALC, lWTorPPWU, lOverrideTolWt, SMM_SEC_CALC, SMM_SEC_CALC, SMM_SEC_CALC, io.IsEmbeddedPrice(csSMMScannedUPC));
		if (!bRes)
		{
			bSMMFailureActionCancel = true;
			return SASMMSecOffline();
		}
	}
	CalcObtainAndTraceSMMWtTol(false);	// Get new tol based on the weight, don't change exception info - Std Dev LPM121702
	bIsSMMItemAdded = true;
	RETURNSTATE(SmmEditAdd)
  }
  //if bag scale weight tally not the first, then it's greater, never 0
  if (dm.fStateTakeawayBelt)//TAR 205490
	ps.SetLeadthruText(MSG_SMM_DUPBELTWEIGHT, PS_BLANK);
  else
	ps.SetLeadthruText(MSG_SMM_DUPWEIGHT, PS_BLANK);	
#endif

  return STATE_NULL;		// stay in SMSmmAddWeightBase state
}

/////////////////////////////////////////////
// PSButton8 - Go Back
/////////////////////////////////////////////
SMStateBase  *SMSmmAddWeightBase::PSButton8(void)
{
  // Tell the security sub-system that we are back to Security Maintenance main state
  CKeyValueParmList parmList;
  CKeyValue<const _TCHAR*>::Put(parmList, KEY_SMM_OPERATION, KEY_SMM_OPERATION_CANCEL);
  _bstr_t bstrParmList((const _TCHAR *)parmList);
  SecMgr.SecurityMaintenance(lSecMgrCookie, bstrParmList);  
   
  bIsSMMItemAdded = false; //jrparro added TAR#327528
  RETURNSTATE(SmmEditAdd)
}

void SMSmmAddWeightBase::UnInitialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmmAddWeight") );

}
