//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMScaleBrokenBase.cpp
//
// TITLE: Class implimentation for Scale Broken State
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                

#include "SMScaleBroken.h"     
#include "SMContinueTrans.h"


#define COMP_ID ID_SM               // base state component
#define T_ID _T("ScaleBrokenBase")

IMPLEMENT_DYNCREATE(SMScaleBrokenBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(ScaleBroken)


DEFINE_TIMESTAMP

////////////////////////////////
SMScaleBrokenBase::SMScaleBrokenBase()
{
  IMPLEMENT_TIMESTAMP
}

///////////////////////////////////////
SMStateBase  *SMScaleBrokenBase::Initialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("ScaleBroken") );	// RFC 330050
    // NewUI
	ps.SetCMFrame(_T("ScaleBroken"));

	ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
	ps.ShowCMTBText(csTBMessage2Scot);	//thp

	if (!Is5XHardware())
		ps.Message12(MSG_RESETSCALE, true);
	else
		ps.Message12(MSG_RESETSCALE_5X, true);

    DMScaleEnable();

    // End NewUI
    DMSayPhrase(SCALENOTWORKING);
	ps.ShowCMFrame();
    return STATE_NULL;                // stay in ScaleBrokenBase state
}

//////////////////////////////////////
SMStateBase  *SMScaleBrokenBase::TimedOut(void)
{
  // scale broken, user resets it to zero, check whether he did it so
	bool bScaleError = false;
	switch (lDMScaleWeight)
	{
		case DM_ERROR_GENERALFAILURE:     // -1
		case DM_ERROR_SCALE_UNDERZERO:    // -2
		case DM_ERROR_SCALE_OVERWEIGHT:   // -3
			bScaleError = true;
			break;
		default:
			bScaleError = false;
			break;
	}

  if ((lDMScaleWeight == 0) && !bScaleError)
  {
	if ( GetISIPTimeOut() ) 
	{  
		SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1); 
		if (fInMultiSelectPickList)
		{
			g_lNumberOfItemsNotBagInMultiPick--;
			return setAndCreateAnchorState( BES_PRODUCEFAVORITES );
		}
	} 
    return STATE_RETURN;
  }

  // If we got to this state from Attract, don't go to continue trans - 
  // if it times out, it "cancels" a transaction and prints a receipt. then
  // it will go back to attract, where this will start all over again.  The net effect
  // of this is that if scale is broken all night you print a receipt every minute until
  // they fix the scale!  Bogus! - TAR 135402

  if (getAnchorState() == BES_START)
    return STATE_NULL;
  else
  {
    // Fix for TAR 153465
	if (SCOTIsMinimized())
      return STATE_NULL;
    else
	  RETURNSTATE(ContinueTrans)
  }
}


////////////////////////////////////
SMStateBase  *SMScaleBrokenBase::DMScale(void) //
{
  // let the base prime the 'virtual' scale
  SMStateBase  *sExit = SMStateBase ::DMScale();
  if (sExit != STATE_NULL)
	return sExit;

  bIsScaleBroken = true; //tar 183854
  // scale broken, user resets it to zero, check whether he did it so
  if (lDMScaleWeight == 0) 
  {
	fInWLDBProcess = false; //tar 183854
	if ( GetISIPTimeOut() )         
    //If the product sale is in progress, we cancel it and set bEASReEnableScanner to true
	//inside of SASendScotInput()
	{  
		SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1); 
		if (fInMultiSelectPickList)
		{
			g_lNumberOfItemsNotBagInMultiPick--;
			return setAndCreateAnchorState( BES_PRODUCEFAVORITES );
		}
	}
	else
	{
		//TAR209230 CHU07/10/02: If the product sale is not in progress,
        //the scanner needs to be enabled for the next state.
		bEASReEnableScanner = true; 
		trace(L2, _T("bEASReEnableScanner is set to true in SMScaleBrokenBase::DMScale()"));

	}

	return STATE_RETURN; 
  }

  return STATE_NULL;
}


///////////////////////////////////
SMStateBase *SMScaleBrokenBase::PSButtonGoBack(void) 
{
  switch (lDMScaleWeight)
  {
	case DM_ERROR_GENERALFAILURE:     // -1
    lDMScaleWeight = 0;               // clear our error indicatior, wait for next error
    // intentional fall thru to DMCheckHealth();

	case DM_ERROR_SCALE_UNDERZERO:    // -2
	case DM_ERROR_SCALE_OVERWEIGHT:   // -3
	  DMCheckHealth(DMCLASS_SCALE);
	  break;
  }

  // Always going to Scan&Bag is bad since we can come here from attract.  If we come
  // from attract and you press GoBack, then we go to scan&bag and never run TBStart()
  // which lets TB know a transaction is starting (and also resets tender details)
  // Therefore we will go to the anchor state, which for Attract is Attract,
  // and for all other places where ScaleBroken is called, anchor state will be scan&bag
  // TAR# 136406 - 03/23/2000 - ncr_barn
  if (io.csItemCode.GetLength()>0) //tar 183854
  {
	SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
	bIsScaleBroken = false; //tar 183854
  }

  if (fInMultiSelectPickList)
  {
	g_lNumberOfItemsNotBagInMultiPick--;
	return setAndCreateAnchorState( BES_PRODUCEFAVORITES );
  }
  else
  {
	return createAnchorState();
  }
}

// + RFC 330050
void SMScaleBrokenBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScaleBroken") );	
}
// - RFC 330050
