//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMEnterWeightBase.CPP
//
// TITLE: Class implementation for Enter Weight state
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header
#ifndef _CPPUNIT
#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
     
#include "SMSelectContainer.h"      // Select a tare
#include "SMScaleBroken.h"          // Reset scale
#include "SMLookUpItem.h"           
#include "SMSelectTare.h"			// Tare state	RFC 152490
#include "SMRemoveItemFromProduceScale.h"
//begin TAR#118659	LPM092899
#include "DMProcedures.h"
DLLEXPORT extern DMProcedures dm;   // Global DM object
// end 118659
#else  // _CPPUNIT
#endif // _CPPUNIT

#include "SMEnterWeight.h" 

bool SMEnterWeightBase::m_bInTimeOut = false;//SR814.2
DLLEXPORT extern HWND hw;           //SR814.2


#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("EnterWeightBase")

IMPLEMENT_DYNCREATE(SMEnterWeightBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(EnterWeight)


DEFINE_TIMESTAMP

SMEnterWeightBase *SMEnterWeightBase::m_pEnterWeight = NULL;//SR814.2
//////////////////////////////////////////
SMEnterWeightBase::SMEnterWeightBase()
: csCurContext(EMPTY_STRING)
{
  IMPLEMENT_TIMESTAMP
  m_pEnterWeight = this;//SR814.2
}

///////////////////////////////////////
SMStateBase  *SMEnterWeightBase::Initialize(void)
{
#ifndef _CPPUNIT
  m_pEnterWeight = this;
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
  bool weightChangeDetected = false;//SR814.2
  //if there is something on the scale now, use that weight -- but
  // only if we know item on the scale has changed
  fDMScaleChanged = 0;
  if (lDMScaleWeight > 0 && weightchanged)
  {
       weightChangeDetected = true;//SR814.2
  }
	
  //+TAR#293994
  if((!io.bBizerbaScale)
      && (   co.fStateAcceptWeightBtnEnable //+SSCOP-675 from SSCOB-1495
         || (co.getTimeOutEnterWeightTimer() > 0)
         || (lDMScaleWeight <= 0)  ) ) //-SSCOP-675 from SSCOB-1495
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
#endif //_CPPUNIT
  csCurContext = _T("EnterWeight");  //  Tar 238987
  COleVariant v;
  if(!io.bBizerbaScale && !g_bInRecallMode)
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
#ifndef _CPPUNIT
  // (+) SR899 ====================
  CString csWork;//sscop-297
  csWork= io.csOrgItemDescription;//sscop-297
  SetNextGenUICurrentItem(io.csOrigCode,csWork);
  if ( co.IsInAssistMenus() )
  {
    csCurContext = ps.GetCurrentContext();

	v = VARIANT_FALSE;

	if (lDMScaleWeight > 0 && weightchanged)
	{
	  if(co.fStateAcceptWeightBtnEnable)
	  {
	    v=VARIANT_TRUE;
	  }	
	}

	ps.SetButtonText(_T("CMButton1Med"), BTT_ACCEPTWEIGHT);
	m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), csCurContext, UI::PROPERTYVISIBLE, v);
	m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
	//sscop-297	
	csWork += _T("\n") + ps.GetPSText(MSG_ENTERWEIGHTASSIST);
  }
  else{
	csWork += _T("\n") + ps.GetPSText(MSG_ENTERWEIGHT);
  }//sscop-297

  // (-) SR899 ====================
  
  //+SR814.2
  if(!ShowAcceptWeightBtnAndStartEnterWeightTimer(weightChangeDetected))
  {
	return ProcessWeightOnScale();
  }
  //-SR814.2
  //RFC 236397 050703
  //CString csWork;
  //csWork= io.csOrgItemDescription;
  //csWork += _T("\n") + ps.GetPSText(MSG_ENTERWEIGHTASSIST);
  ps.Message12(csWork, true);
  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid); //*** NewUI ***//
  ps.ShowCMFrame();
#endif //_CPPUNIT
  return STATE_NULL;                // stay in EnterWeightBase state
}
#ifndef _CPPUNIT
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
	
  //+SR814.2
  if(!ShowAcceptWeightBtnAndStartEnterWeightTimer())
  {
	return ProcessWeightOnScale();
  }
	
  if (lDMScaleWeight>=0)
  {
    return STATE_NULL;	//wait for timeout until good weight accepted	
  }
  else
  {
    setAnchorState(BES_SCANANDBAG);
    RETURNSTATE(ScaleBroken)
  }
  //-SR814.2

}

////////////////////////////////////////
SMStateBase  *SMEnterWeightBase::PSCancelKey(void) // cancel
{
  DMSayPhrase(KEYPADVALID);
  return PSButton3();
}

#endif //_CPPUNIT
///////////////////////////////////////
void SMEnterWeightBase::UnInitialize(void)
{
  if(!fInMultiSelectPickList)//SR679
  {
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("EnterWeight") );
  }

  m_bInTimeOut = false;
  KillTimer(hw,WEIGHT_ACCEPT);   //SSCOP-3364
}

#ifndef _CPPUNIT
//+SR814.2
SMStateBase  *SMEnterWeightBase::PSButton1(void) 
{
	if(m_bInTimeOut)
	{
		nTimeOut=0; 
		KillTimer(hw,WEIGHT_ACCEPT);           
	}
    return ProcessWeightOnScale();
}
#endif // _CPPUNIT

SMStateBase  *SMEnterWeightBase::ProcessWeightOnScale(void)
{
#ifndef _CPPUNIT
  // if this is a manually entered item that needs a weight
  // and we didn't select a tare yet (i.e.==0) and we must select a
  // tare, then goto select a tare state
  // note: the tare is in the ranges:
  //  -1    no tare
  //  0     no tare
  //  1-10  1-10 for Smkt tares 0-9
  // pd980722 removed check for NOT a scanned item as per NCR
  io.lWeightEntered = lDMScaleWeight;
  
  if(co.fStateAcceptWeightBeepEnable)
  {
      DMSaySecurity( SEC_NONE, ps.GetSoundName(ACCEPTWEIGHTBEEP));     //SSCOADK-1148  
  }
	
  trace(L6,_T("Scale weight accepted= %d"),lDMScaleWeight);
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
	
  
  m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), csCurContext, UI::PROPERTYSTATE, (long)UI::Disabled);  //tar 330491 
#endif // _CPPUNIT

  if(g_bInRecallMode)
  {
      RETURNSTATE(RemoveItemFromProduceScale); 
  }
  return TBEnterItem();
}

VOID CALLBACK SMEnterWeightBase::EnterWeightTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	m_pEnterWeight->HandleEnterWeightTimedOut();
}


void  SMEnterWeightBase::HandleEnterWeightTimedOut()
{   
	//A timeout of 1 second is needed to 
	//transition from EnterWeight to SelectContainer/SelectTare
	//state. co.getTimeOutEnterWeightTimer() is set to 1000ms 
	//as the minimum value.
	nTimeOut = 1;
    trace(L6,_T("+SMEnterWeightBase::HandleEnterWeightButtonTimedOut()"));
    KillTimer(hw,WEIGHT_ACCEPT);            
	
}

#ifndef _CPPUNIT
SMStateBase *SMEnterWeightBase::TimedOut(void)
{
	trace(L6,_T("SMEnterWeightBase Timeout Expires."));
	m_bInTimeOut = false;
    if (lDMScaleWeight)
    {
        return ProcessWeightOnScale();
    }
    else
    {
        return STATE(State)::TimedOut();
    }
}

#endif // _CPPUNIT

bool SMEnterWeightBase::ShowAcceptWeightBtnAndStartEnterWeightTimer(bool bWeightChange)
{
    
    COleVariant v = VARIANT_FALSE;

    if(bWeightChange || lDMScaleWeight>=0)
    {
        int nEnterWeightTimer = co.getTimeOutEnterWeightTimer();
        
        if(nEnterWeightTimer!=0)
        {  
            if(m_bInTimeOut)
            {
                KillTimer(hw,WEIGHT_ACCEPT);           
            }
            trace(L6,_T("+SMEnterWeightBase::StartTimerForEnterWeight()"));
            KillTimer(hw,WEIGHT_ACCEPT);
            if (lDMScaleWeight)  //only restart timer if we got a weight
            {
                m_bInTimeOut = true;
                nTimeOut=0; //disable main state timer 
                int nTimer = (nEnterWeightTimer - 1000) < 0 ? 0 : nEnterWeightTimer - 1000; 
                trace(L6,_T("Start ShowAcceptWeightBtnAndStartEnterWeightTimer nTimer=%d"), nTimer);
                long lTimerResponse = SetTimer(hw, WEIGHT_ACCEPT, nTimer, &EnterWeightTimedOut);
                trace(L6,_T("Start EnterWeightTimer=%d lTimerResponse=%d"), nEnterWeightTimer, lTimerResponse);
            }
            else
            {
                m_bInTimeOut = false;
                //use the main state timer 
                if (storeMode() || co.IsInAssistMenus() || io.bBizerbaScale)
                    nTimeOut = 0;
                else
                    nTimeOut = co.getTimeOutStateDefault();
            }
        }

        if (lDMScaleWeight)
        {
            if(co.fStateAcceptWeightBtnEnable)
            {
                v=VARIANT_TRUE;
            }
            if(!co.fStateAcceptWeightBtnEnable && nEnterWeightTimer==0)
            {
                return false;
            }
        }
        
    }
    
    m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), csCurContext, UI::PROPERTYVISIBLE, v);
 
    return true;

}
//-SR814.2
