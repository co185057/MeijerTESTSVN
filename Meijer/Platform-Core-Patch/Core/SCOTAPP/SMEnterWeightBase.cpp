//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMEnterWeightBase.CPP
//
// TITLE: Class implementation for Enter Weight state
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMEnterWeight.h"      
#include "SMSelectContainer.h"      // Select a tare
#include "SMScaleBroken.h"          // Reset scale
#include "SMLookUpItem.h"           
#include "SMSelectTare.h"			// Tare state	RFC 152490
//begin TAR#118659	LPM092899
#include "DMProcedures.h"
DLLEXPORT extern DMProcedures dm;   // Global DM object
// end 118659

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("EnterWeightBase")

IMPLEMENT_DYNCREATE(SMEnterWeightBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(EnterWeight)


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMEnterWeightBase::SMEnterWeightBase()
: csCurContext(EMPTY_STRING)
{
  IMPLEMENT_TIMESTAMP
}

///////////////////////////////////////
SMStateBase  *SMEnterWeightBase::Initialize(void)
{
  if(!fInMultiSelectPickList)//SR679
  {
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("EnterWeight") );
  }

  ra.RequestEnableAssistMode(false);  //TAR 297347

  //Update RAP window with SCOT state
  UpdateStateName(ps.GetPSText(LTT_PUTONSCALE, SCOT_LANGUAGE_PRIMARY));
  setAnchorState(BES_ENTERWEIGHT);  //TAR#154147 return to this state from help on way	
  // if we have the weight aleady because this is a
  // void and we have the original weight from the
  // receipt we can just use that weight
  bIsScaleBroken = false; //tar 183854
  if (io.fVoid && lVoidWgt>0)
  {
    io.lWeightEntered = lVoidWgt;
    lVoidWgt = 0;
    return TBEnterItem();
  }

  if(!io.bBizerbaScale)
	DMScaleEnable();
	
  bool weightchanged = fDMScaleChanged;
  //if there is something on the scale now, use that weight -- but
  // only if we know item on the scale has changed
  fDMScaleChanged = 0;
  if (lDMScaleWeight > 0 && weightchanged)
  {
    // if this is a manually entered item that needs a weight
    // and we didn't select a tare yet (i.e.==0) and we must select a
    // tare, then goto select a tare state
    // note: the tare is in the ranges:
    //  -1    no tare
    //  0     no tare
    //  1-10  1-10 for Smkt tares 0-9
    // pd980722 removed check for NOT a scanned item as per NCR
    io.lWeightEntered = lDMScaleWeight;
		
	//if (io.lTareCode==0 && io.d.fTareRequired)
	if(io.lTareCode ==0 && co.fOperationsSelectTareOnWeight)
	{
	  //Tar 152490 start
	  if((!co.fOperationsJumpToTarePickList)&&(!co.IsInAssistMenus()))
			RETURNSTATE(SelectContainer)
	  else
		    RETURNSTATE(SelectTare)
	  //Tar 152490 end
    }
    
    //TAR#124125 use automatic tare code option
    if (!co.fOperationsSelectTareOnWeight)
    {
      io.lTareCode = co.nOperationsAutoTareCode;
    }
		
    // just press enter, SMKT will poll the scale
    // for the weight and get the weight we
    // prime the virtual scale with.
    return TBEnterItem();
  }
	
  //+TAR#293994
  if(!io.bBizerbaScale)
  {
	DMSayPhrase(PLACEITEMONSCALE);
  }
  //-TAR#293994

  ps.SetCMFrame(_T("EnterWeight"));
  if (dm.fStateTakeawayBelt)
  {
	ps.SetCMBackground(BMP_PUTONSCALE_BELT);
  }
  else if(!io.bBizerbaScale)
  {
	ps.SetCMBackground(BMP_PUTONSCALE);
  }
  else
  {
	ps.SetCMBackground(BMP_BIZERBAPUTONSCALE);
  }
  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
  ps.ShowCMTBText(csTBMessage2Scot);	//thp
  csCurContext = _T("EnterWeight");  //  Tar 238987
  COleVariant v;
  if(!io.bBizerbaScale)
  {
	v=VARIANT_TRUE;
  }
  else
  {
  	v=VARIANT_FALSE;
	ps.CMButton(_T("ButtonHelp"),BTT_HELP,false); 
    nTimeOut = 0;  //No time out
  }
  m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), csCurContext, UI::PROPERTYVISIBLE, v);
  //RFC 236397 050703
  ps.Message12(io.csOrgItemDescription, true);
  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid); //*** NewUI ***//
  ps.ShowCMFrame();
  return STATE_NULL;                // stay in EnterWeightBase state
}

//////////////////////////////////////
SMStateBase  *SMEnterWeightBase::PSButton3(void) // cancel
{
	if (io.csItemCode.GetLength()>0) //tar 183854
	{
/*		//tar +390024
		if (fInMultiSelectPickList && ps.GetPicklistItemsSold()!=0)
		{
			return setAndCreateAnchorState( BES_PRODUCEFAVORITES );
		}
		//tar -390024
		else
		{
			// Tar 120648*/
			SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
	//	}
    }
	if (fInMultiSelectPickList)
	{
  // +TAR 407200
		g_bMultiPickSkipBaggingAllowed = true;
  // -TAR 407200
		g_lNumberOfItemsNotBagInMultiPick--;
		return setAndCreateAnchorState( BES_PRODUCEFAVORITES );
	}
	else
	{
		return setAndCreateAnchorState(BES_SCANANDBAG);     //TAR#126112 return to Scan&Bag LPM111699
	}
}
////////////////////////////////////
//@@@INFOPOINT
SMStateBase  *SMEnterWeightBase::PSButtonGoBack(void) // cancel
{
  return PSButton3();    // previous state
}
//@@@INFOPOINT
////////////////////////////////////
SMStateBase  *SMEnterWeightBase::DMScale(void) 
{
  // let the base prime the 'virtual' scale
  SMStateBase  *sExit = SMStateBase ::DMScale();
	if (sExit!=STATE_NULL)
		return sExit;
	
  if (lDMScaleWeight==0)
    return STATE_NULL;	// shakey scale, wait for good weight
	
  if (lDMScaleWeight<0)
  {
    setAnchorState(BES_SCANANDBAG);
    RETURNSTATE(ScaleBroken)
  }
	
  // if this is a manually entered item that needs a weight
  // and we didn't select a tare yet (i.e.==0) and we must select a
  // tare, then goto select a tare state
  // note: the tare is in the ranges:
  //  -1    no tare
  //  0     no tare
  //  1-10  1-10 for Smkt tares 0-9
  // pd980722 removed check for NOT a scanned item as per NCR
  io.lWeightEntered = lDMScaleWeight;
	
  fDMScaleChanged = 0;
  if(io.lTareCode ==0 && co.fOperationsSelectTareOnWeight)
  {
	  //Tar 152490 start
	  if((!co.fOperationsJumpToTarePickList)&&(!co.IsInAssistMenus()))
			RETURNSTATE(SelectContainer)
	  else
		    RETURNSTATE(SelectTare)
	  //Tar 152490 end
  }

  //TAR#124125 use automatic tare code option
  if (!co.fOperationsSelectTareOnWeight)
  {
    io.lTareCode = co.nOperationsAutoTareCode;
  }
	
  // just press enter, SMKT will poll the scale
  // for the weight and get the weight we
  // prime the virtual scale with.
	
  // scale has already been primed
  m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);  //tar 330491 
  return TBEnterItem();
}

////////////////////////////////////////
SMStateBase  *SMEnterWeightBase::PSCancelKey(void) // cancel
{
  DMSayPhrase(KEYPADVALID);
  return PSButton3();
}

///////////////////////////////////////
void SMEnterWeightBase::UnInitialize(void)
{
  if(!fInMultiSelectPickList)//SR679
  {
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("EnterWeight") );
  }
}
