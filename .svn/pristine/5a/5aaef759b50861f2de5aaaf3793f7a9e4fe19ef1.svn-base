//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMLaneClosed.cpp
//
// TITLE: Class implementation for lane closed state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMLaneClosed.h"
#include "transbrokerssf.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("LaneClosed")

IMPLEMENT_DYNCREATE(SMLaneClosed, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

////////////////////////////
SMLaneClosed::SMLaneClosed()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase *SMLaneClosed::Initialize(void)
{
    SMStateBase * pState = SMLaneClosedBase::Initialize();
	
    nTimeOut = 30;

	//Loading Meijer logo on to the SCO printer
	if (!SMState::bSCOPrinterBitmapStatus)
	{
		if (SMState::csMeijerLogoFilePath.IsEmpty())
		{
			CString csBitmapPathOption = _T("BitmapPath");
			CString csBitmapFullPath;
			SendTBCommand(TBCMD_GET_OPTIONS, &csBitmapPathOption, &csBitmapFullPath);
			trace(L6, _T("Bitmap path in ACSOPTS = %s"), csBitmapFullPath);
			if (csBitmapFullPath.IsEmpty())
			{
				//either the option is undefined or failed to read. Assume default path
				SMState::csMeijerLogoFilePath = "C:\\ACS\\Data\\PAMLOGO10.DAT";
			}
			else
				SMState::csMeijerLogoFilePath = csBitmapFullPath;
		}
		SMState::bSCOPrinterBitmapStatus = LoadReceiptLogo(SMState::csMeijerLogoFilePath);
	}
	    
    return pState;
}

void SMLaneClosed::UnInitialize(void)
{
    long rc = TBGetCurrentBusinessDate(rp.m_csBusinessDate);
	
	if (rc != TB_SUCCESS)
    {
		trace(L1, _T("Error TBGetCurrentBusiness not implemented, please be advised!"));
	}  
	
	 SMLaneClosedBase::UnInitialize();
}

SMStateBase *SMLaneClosed::PSButtonHelp(void)
{
    return SMLaneClosedBase::PSButtonHelp();
}

SMStateBase *SMLaneClosed::PSButton1(void)
{
    return SMLaneClosedBase::PSButton1();
}

SMStateBase *SMLaneClosed::PSButtonGoBack()
{
    if (inputMode == PROMPTING_FOR_PASSWORD)
    {
        promptForOperator();
    }
    else
    {
        inputMode = SHOW_WAIT_FOR_APPROVAL;
        if (co.fStateDualLanguage)
			ps.SetLanguage(m_customerLanguage);  //TAR 196315: Set back to customerLanuguage for dual language
        showScreen(false);
    }
	
    return STATE_NULL;
}

SMStateBase *SMLaneClosed::PSCancelKey(void)
{
    return SMLaneClosedBase::PSCancelKey();
}

SMStateBase *SMLaneClosed::stateAfterPasswordVerify(void)
{
	SendTBCommand(TBCMD_NOTIFY_SCOLOADCONFIRMATION);
    return SMLaneClosedBase::stateAfterPasswordVerify();
}

void SMLaneClosed::showScreen(bool passwordVerified )
{
    saMsgFuncPtr = PSProceduresBase::SecurityMessageModal;
    
    // This state does not need to reshow if the password
    // was verified, since we'll be leaving now
    if (passwordVerified)
    {
        TBLaneClosed(false);   //TAR# 143172 tell TB to leave lane closed LPM061300
        trace(L6, _T("On NO RAP DATA NEEDED, CLEAR LANE CLOSED VIEW"));
        ra.OnNoRAPDataNeeded();
        SynchronizeTime();
        return;
    }
    
    if (inputMode == PROMPTING_FOR_OPERATOR)
    { 
		trace(L6, _T("*_* inputMode = PROMPTING_FOR_OPERATOR"));
        saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;
        STATE(OperatorPasswordState)::ShowEnterID();
        ps.SetCMLeadthruText(TXT_ENTER_ID);
		
        // Determine the leadthru text to show
        int iMessage = MSG_ENTERID_PRESSENTER;
        COleVariant v = VARIANT_TRUE;
        bool bEmplid = (SendTBCommand(TBCMD_IS_EMPLID_ENABLED) == TB_SUCCESS_IMMEDIATE);
        if( bEmplid )
        {
			trace(L6, _T("*_* EmplID enabled"));
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
        // Leadthru text message set
		
        ps.AllowInput(co.GetnOperationsOperatorIDMaxDigit(),false,false);
        ps.ShowCMTBText(csTBMessage2Scot);	//thp
        ps.ShowCMFrame(false);
		return ;
    }
    else
    {  
		trace(L6, _T("*_* Displaying Lane close screen"));
		// Display initial Lane Closed screen.
        // NewUI
        ps.SetCMFrame(_T("LaneClosed"));
        ps.Echo(_T(""));	//TAR # 247620
        ps.SetButtonText(_T("CMButton1StoreLogIn"), BTT_STORELOGIN);
        // End NewUI
        //tar 174918 turn red light off to stop page resend timer
        dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
            DM_TRICOLORLIGHT_OFF,
            DM_TRICOLORLIGHT_NORMALREQUEST);
        //then turn red light back on
        dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
            DM_TRICOLORLIGHT_ON,
            DM_TRICOLORLIGHT_NORMALREQUEST);
    }
    
    ps.SetCMLeadthruText(LTT_LANECLOSED);
    SAWLDBScannerDisable();	
    ps.ShowCMTBText(csTBMessage2Scot);	//thp
    ps.ShowCMFrame(false); 
    // End NewUI
    
    // grey out help button
    //ps.CMButton(IDC_BUTTONHELP,BTT_HELP,false);
	//SMLaneClosedBase::showScreen();
}

SMStateBase *SMLaneClosed::OnWtIncreaseNotAllowed(void)
{
    return SMLaneClosedBase::OnWtIncreaseNotAllowed();
}

SMStateBase *SMLaneClosed::OnWtDecrease(void)
{
    return SMLaneClosedBase::OnWtDecrease();
}

SMStateBase *SMLaneClosed::DMScanner(void)
{
    return SMLaneClosedBase::DMScanner();
}

SMStateBase  *SMLaneClosed::TimedOut(void)
{
    // return STATE_NULL instead of returning TimedOut() 
    // otherwise it will keep ringing on RAP	
    return STATE_NULL; 
}
