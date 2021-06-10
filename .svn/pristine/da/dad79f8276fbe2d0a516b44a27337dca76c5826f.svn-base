//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMBagAndEAS.CPP
//
// TITLE: Class header for Scan and Bag State
//
// $Header:
//
// AUTHOR:    Tara Duncan
//
//////////////////////////////////////////////////////////////////////////////////////////////////
// POS310244 Work Request:76936 Name: Alex Bronola   Date:Jan 10, 2017
// POS296302 Work Request:76936 Name: Alex Bronola   Date:Dec 15, 2016
// POS143692 Work Request:68237 Name: Kranthi Kumari Cherugondi   Date:March 18, 2015
// POS108118 Work Request:65347 Name: Saiprasad Srihasam Date:August 28, 2014
// POS40504 Work Request:52635 Name:Robert Susanto Date:October 26, 2012

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes
#include "SMState.h"                // MGV base state
#include "SMBagAndEAS.h"           // MGV this state
#include "transbrokerssf.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("BagAndEAS")

IMPLEMENT_DYNCREATE(SMBagAndEAS, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

////////////////////////////
SMBagAndEAS::SMBagAndEAS()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////////////////////////
SMStateBase* SMBagAndEAS::Initialize()
{
    // +POS143692
  //442275+ Coming to this state from SystemMessage and OnItemOK has arrived at SystemMessage
  if (g_bOnItemOK)
  {
	   return setAndCreateAnchorState(BES_SCANANDBAG);
  }
  //442275-
  // -POS143692
  nPreviousState = BES_BAGANDEAS;
	
	//+POS310244 - aab
    SetTBProperty(_T("IsInBagAndEAS"), _T("1"));
	//-POS310244 - aab
  SMStateBase* returnState = SMBagAndEASBase::Initialize();

  //POS40504 - rrs
  bSkipBaggingButtonPressed = false;
	
  // Get the Registry Values of the scale or Emulator, 
  long nScaleDisabled = GetDWORDValue("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\ScotApp\\Controls\\SmartScale", "DisableBagScale");
  // Compare the value to see if the emulator is running or scale is disabled, if so, skip bagging.
  if (nScaleDisabled == 1)  
  {
	delete getRealStatePointer(returnState);
    return SMBagAndEASBase::PSButton1();
  }

  /* TAR 394578 - SSF patch below no longer needed because it causes out of sync in security when using the
                  Use my own bag functionality.

  // If an unexpected increase was queued up, an item was purchased in between and we need to queue it
  // up to return here after it finishes.
  if (returnState != STATE_NULL)
  {
      CString csStateName = returnState->GetRuntimeClass()->m_lpszClassName;
      if (csStateName.Find( _T("SecUnExpectedIncrease") ) != -1)
      {
          trace(L6, _T("Unexpected increase on entering BagAndEAS"));
          ((SMSecUnExpectedIncrease *)returnState)->Deliver(BES_BAGANDEAS, BES_BAGANDEAS); 
      }
  }
  */
  return returnState;
}

////////////////////////////////////////////////////////
// Check the Registry value for the SmartScale in order to determine if the lane is
// a Fast Pay lane. (if the returned data value is 1, the smartscale is disabled.  JYC
long SMBagAndEAS::GetDWORDValue(const CString& key,const CString& value)
{
	long data = 0;
	DWORD iDataLength = sizeof(data);
	DWORD type = REG_DWORD;
	LONG lRC;
	HKEY hKey;
	
	
	lRC = ::RegOpenKeyEx(
		HKEY_LOCAL_MACHINE,
		key,
		0,
		KEY_QUERY_VALUE,
		&hKey);
	
	if (lRC==ERROR_SUCCESS)
	{
		lRC = ::RegQueryValueEx(
			hKey,
			value,
			NULL, 
			&type, 
			(BYTE *)&data,
			&iDataLength);
		
		::RegCloseKey(hKey);
		
		if (lRC!=ERROR_SUCCESS)
		{
			data = 0;
		}
	}
	
	return data;
}

void SMBagAndEAS::UnInitialize()
{
	//-POS310244 - aab
    SetTBProperty(_T("IsInBagAndEAS"), _T("0"));

    SMBagAndEASBase::UnInitialize();

	CString csPOSState = GetTBProperty(_T("SavedLastPosState"));
	trace(L3, _T("*** BagAndEAS::Unitialize - LastTBState: %d PosState = %s bIsItemizationAfterSkipBaggingSent - %d"), m_TBLastState, csPOSState, bIsItemizationAfterSkipBaggingSent);
	
     //7718
    if( !m_bIsNoSale && !bIsItemizationAfterSkipBaggingSent 
			&& !(m_TBLastState == TB_ITEMSOLD && csPOSState.Find(_T("CM_PROMO_LINKEDITEM")) != -1) //SSCOI-55023
			&& (csPOSState.Find(_T("MISS_SCAN_ALERT")) == -1  //CRD 330925
					&& csPOSState.Find(_T("PRODUCT_SWITCH_ALERT")) == -1 	//SSCOI-71317 / CRD 372873
					&& csPOSState.Find(_T("BASKET_LOSS_ALERT")) == -1		//SSCOI-71317 / CRD 372873
				)
		)
    {
		bIsItemizationAfterSkipBaggingSent = true;
		trace(L6, _T("SMBagUnitialize - set TBState to ITEMIZATION_AFTER_SKIPBAGGING"));
			SendTBCommand(TBCMD_SET_TB_STATE,_T("ITEMIZATION_AFTER_SKIPBAGGING"));
    }
}

bool SMBagAndEAS::PSAssistMenuLegalForState()
{
    return SMBagAndEASBase::PSAssistMenuLegalForState();
}

bool SMBagAndEAS::storeMode()
{
    return SMBagAndEASBase::storeMode();
}

SMStateBase * SMBagAndEAS::OnPLAHandInView()
{
    return SMBagAndEASBase::OnPLAHandInView();
}

SMStateBase * SMBagAndEAS::TimedOut(void)
{
    return SMBagAndEASBase::TimedOut();
}

SMStateBase * SMBagAndEAS::PSButton1(void)
{
    return SMBagAndEASBase::PSButton1();
}

SMStateBase * SMBagAndEAS::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
    return SMBagAndEASBase::OnRAPClearSecurityBtn(lpszAuthorizationType);
}

SMStateBase * SMBagAndEAS::OnWtIncreaseNotAllowed(void)
{
    return SMBagAndEASBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMBagAndEAS::OnWtDecrease(void)
{
    return SMBagAndEASBase::OnWtDecrease();
}

SMStateBase * SMBagAndEAS::OnMatchedWt(void)
{
    return SMBagAndEASBase::OnMatchedWt();
}

SMStateBase * SMBagAndEAS::DMdf_EASNoMotion()
{
    return SMBagAndEASBase::DMdf_EASNoMotion();
}

SMStateBase * SMBagAndEAS::OnBackToLGW(void)
{
    return SMBagAndEASBase::OnBackToLGW();
}

SMStateBase * SMBagAndEAS::PSButtonGoBack(void)
{
    return SMBagAndEASBase::PSButtonGoBack();
}

void SMBagAndEAS::showScreen(bool passwordVerified)
{
    COleVariant v;
    if (passwordVerified)
        return;
    
    if (inputMode == PROMPTING_FOR_OPERATOR)
    {
        ps.SetCMFrame(_T("EnterID"));
        ps.SetCMLeadthruText( TXT_ENTER_ID );

        // Determine the leadthru text to show
        int iMessage = MSG_ENTERID_PRESSENTER;
        COleVariant v = VARIANT_TRUE;
        bool bEmplid = (SendTBCommand(TBCMD_IS_EMPLID_ENABLED) == TB_SUCCESS_IMMEDIATE);
        if( bEmplid )
        {
            bool bAllowHandKey = (SendTBCommand(TBCMD_IS_EMPLID_ALLOWHANDKEY) == TB_SUCCESS_IMMEDIATE);
            iMessage = bAllowHandKey ? MSG_ENTERID_SCANIDORKEY : MSG_ENTERID_SCANIDONLY;
            v = bAllowHandKey ? VARIANT_TRUE : VARIANT_FALSE;

//(+)POS108118
            if(!bAllowHandKey)
            {
                CString csNewPosition;
                csNewPosition = _T("400,262,520,65");
                m_pAppPSX->SetConfigProperty(_T("LeadthruText"), _T("EnterID"), UI::PROPERTYPOSITION, csNewPosition);

                csNewPosition = _T("265,396,510,250");
                m_pAppPSX->SetConfigProperty(_T("MessageBoxNoEcho"), _T("EnterID"), UI::PROPERTYPOSITION, csNewPosition);

                m_pAppPSX->SetConfigProperty(_T("LeadthruText"),  _T("EnterID"),UI::PROPERTYTEXTALIGNMENT, _T("0"));
                m_pAppPSX->SetConfigProperty(_T("MessageBoxNoEcho"),  _T("EnterID"),UI::PROPERTYTEXTALIGNMENT, _T("0"));
            }
//(-)POS108118
        }

        m_pAppPSX->SetConfigProperty(_T("NumericP1"), _T("EnterID"), UI::PROPERTYVISIBLE, v);
        m_pAppPSX->SetConfigProperty(_T("NumericP2"), _T("EnterID"), UI::PROPERTYVISIBLE, v);
        m_pAppPSX->SetConfigProperty(_T("NumericP3"), _T("EnterID"), UI::PROPERTYVISIBLE, v);
        m_pAppPSX->SetConfigProperty(_T("NumericP4"), _T("EnterID"), UI::PROPERTYVISIBLE, v);

//(+)POS108118
        m_pAppPSX->SetConfigProperty(_T("Echo"), _T("EnterID"), UI::PROPERTYVISIBLE, v);
        m_pAppPSX->SetConfigProperty(_T("EchoField"), _T("EnterID"), UI::PROPERTYVISIBLE, v);
//(-)POS108118
        
        ps.Message12(iMessage, false);
        ps.AllowInput(co.GetnOperationsOperatorIDMaxDigit(),false,false);
        ps.ShowCMFrame(false);
        return;
    }
    else
    {  
        // TAR 200470 - this problem was discovered as a side effect of the show being slow
        ps.SetCMFrame(_T("BagAndEAS"));
        // the wave and background changes depending on if Belt is installed or not
        if (dm.fStateTakeawayBelt)
            ps.SetCMBackground(BMP_PLACEITEMONBELT);
        else if (dm.EASDeactivatorPresent() && dm.fStateMotionSensor) 
            ps.SetCMBackground(BMP_BAGITEMANDEAS);
        else
            ps.SetCMBackground(BMP_BAGITEM);			// background bitmap for Non EAS
        // BZ 06/02/02 RFC 203538: Show the Store Login button 			
        //			ps.CMButton(_T("ButtonStoreLogIn"),BTT_STORELOGIN, true);
        v = VARIANT_TRUE;
        m_pAppPSX->SetConfigProperty(_T("ButtonStoreLogIn"), csCurContext, UI::PROPERTYVISIBLE, v);
    }
    
    m_bLoginProcessing = false;  // TAR 274406
    
	if (co.fOperationsAllowSkipBaggingBtn &&  co.getTimeOutEnableSkipBaggingButtonTimer() == 0 || 
		(CString(_T("SMProduceFavorites")) == this->csLastNonHelpStateName ) && co.fOperationsAllowMultiPick &&
		co.fOperationsAllowSkipBaggingBtn || co.getTimeOutEnableSkipBaggingButtonTimer()> 0 && co.fOperationsAllowSkipBaggingBtn && 
		g_bSkipBaggingButtonTimerExpires) //BZ RFC 203538
    {
        v = VARIANT_TRUE;
        if (dm.fStateTakeawayBelt)
            ps.SetButtonText(_T("CMButton1Med"),BTT_BELT_SKIPBELT);     // skip belt button
        else
            ps.SetButtonText(_T("CMButton1Med"),BTT_SKIP_BAGGING);      // skip bagging button
    }
    else
        v = VARIANT_FALSE;
    m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), csCurContext, UI::PROPERTYVISIBLE, v);
    
    CPSXRedrawLock  csRedrawLock(_T("Display"), _T("SMBagAndEASBase::ShowScreen"));
    if (dm.fStateTakeawayBelt)
    {
        ps.SetCMLeadthruText(LTT_BELT_PLACEONBELT); // title for TAB
        ps.Message12(MSG_BELT_PLACE_ITEM_ON_BELT, true); // instructions for TAB
    }
    else if(dm.EASDeactivatorPresent() && dm.fStateMotionSensor) 
    {
        ps.SetCMLeadthruText(MSG_BAGITEM);			// instructions for EAS
        ps.Message12(LTT_BAGGING_AREA,true);
    }
    else
    {
        ps.SetCMLeadthruText(MSG_BAGITEM);			// instructions for Non EAS
        ps.Message12(MSG_PLACEITEM_IN_BAGGING, true);
    }
    
    ps.ShowCMTBText(csTBMessage2Scot);	//thp
    
    DisplayAuxScreen(AUX_SCREEN_DEFAULT, true); //tar 193987
    ps.ShowCMFrame(false);
}

SMStateBase * SMBagAndEAS::stateAfterPasswordVerify()
{
    return SMBagAndEASBase::stateAfterPasswordVerify();
}

SMStateBase * SMBagAndEAS::DMScanner(void)
{
    return SMBagAndEASBase::DMScanner();
}

void SMBagAndEAS::Escalation2RAP(void)
{
    SMBagAndEASBase::Escalation2RAP();
}

SMStateBase *SMBagAndEAS::GPNotify(const GPEVT gpEvt)
{
    return SMBagAndEASBase::GPNotify(gpEvt);
}

void SMBagAndEAS::ShowSkipBaggingOnEReceipt(void)
{
    trace(L6, _T("+SMBagAndEAS::ShowSkipBaggingOnEReceipt"));
	
    //POS40504 - don't write to e-receipt if the button is pressed again
    if( !bSkipBaggingButtonPressed )
    {
        //POS40504 - disable the button after pressing it - rrs
        bSkipBaggingButtonPressed = true;
		
        SMBagAndEASBase::ShowSkipBaggingOnEReceipt();
    }
	
    trace(L6, _T("-SMBagAndEAS::ShowSkipBaggingOnEReceipt"));
}

SMStateBase * SMBagAndEAS::DMTakeawayReadyForItem(void)
{
    return SMBagAndEASBase::DMTakeawayReadyForItem();
}

void SMBagAndEAS::HandleSkipBaggingButtonTimedOut()
{
    SMBagAndEASBase::HandleSkipBaggingButtonTimedOut();
}

void SMBagAndEAS::StartTimerForSkipBaggingBtn()
{
    SMBagAndEASBase::StartTimerForSkipBaggingBtn();
}

SMStateBase * SMBagAndEAS::SASecMgrParse( const MessageElement* me)
{
    return SMBagAndEASBase::SASecMgrParse(me);
}

SMStateBase * SMBagAndEAS::DMTakeawayReadyForNotRequiredBaggedItem()
{
    return SMBagAndEASBase::DMTakeawayReadyForNotRequiredBaggedItem();
}
