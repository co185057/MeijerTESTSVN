//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMSystemMessageOpPass.cpp
//
// Description: see h file
//
// Author: Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMSystemMessageOpPass.h"
#include "transbrokerssf.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SystemMessageOpPass")

IMPLEMENT_DYNCREATE(SMSystemMessageOpPass, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////////////////
// Constructor
////////////////////////////////////////////
SMSystemMessageOpPass::SMSystemMessageOpPass(BMPS   bitmap,
                                             PSTEXT title,       // PSTEXT title number or PS_NOTEXT
                                             bool   continueButton,
                                             bool   cancelButton,
                                             LPCTSTR szMessage,   // string text message or NULL
                                             PSTEXT psMessage,   // PSTEXT message number or PS_NOTEXT
                                             PSTEXT promptText,  // PSTEXT prompt number or PS_NOTEXT
		                                         long   promptValue, // prompt area sale value
                                             long   deviceClass, // device class, -1 for none
                                             bool   checkHealth,
											 bool displayHTML) // DMCheckHealth after msg?
// Create a SMSystemMessage from the parms given us.
: SMSystemMessageOpPassBase(bitmap,
                   title,
                   continueButton,
                   cancelButton,
                   szMessage,
                   psMessage,
                   promptText,
                   promptValue,
                   deviceClass,
                   checkHealth,
				   displayHTML)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMSystemMessageOpPass::SMSystemMessageOpPass()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMSystemMessageOpPass::Deliver(BMPS   bitmap,
                                             PSTEXT title,       // PSTEXT title number or PS_NOTEXT
                                             bool   continueButton,
                                             bool   cancelButton,
                                             LPCTSTR szMessage,   // string text message or NULL
                                             PSTEXT psMessage,   // PSTEXT message number or PS_NOTEXT
                                             PSTEXT promptText,  // PSTEXT prompt number or PS_NOTEXT
		                                         long   promptValue, // prompt area sale value
                                             long   deviceClass, // device class, -1 for none
                                             bool   checkHealth,
											 bool displayHTML)
{
  SMSystemMessageOpPassBase::Deliver(bitmap,
               title,
               continueButton,
               cancelButton,
               szMessage,
               psMessage,
               promptText,
               promptValue,
               deviceClass,
               checkHealth,
			   displayHTML);

  return this;
}

SMStateBase *SMSystemMessageOpPass::Initialize(void)
{
	SendTBCommand(TBCMD_NOTIFY_STOREMODE_PROMPT);	//CRD 338206
    return SMSystemMessageOpPassBase::Initialize();
}

void SMSystemMessageOpPass::UnInitialize(void)
{
    SMSystemMessageOpPassBase::UnInitialize();
}

bool SMSystemMessageOpPass::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMSystemMessageOpPassBase::DeliverDescriptorList(dlParameterList);
}

SMStateBase *SMSystemMessageOpPass::OnMatchedWt(void)
{
    return SMSystemMessageOpPassBase::OnMatchedWt();
}

void SMSystemMessageOpPass::InitSystemMessage( BMPS   bitmap,
                                              PSTEXT title,
                                              bool   continueButton,
                                              bool   cancelButton,
                                              LPCTSTR szMessage,
                                              PSTEXT psMessage,
                                              PSTEXT promptText,
                                              long   promptValue,
                                              long   deviceClass,
                                              bool   checkHealth,
                                              bool displayHTML)
{
    SMSystemMessageOpPassBase::InitSystemMessage(bitmap, title, continueButton, cancelButton, szMessage,
        psMessage, promptText, promptValue, deviceClass,  checkHealth, displayHTML);
}

SMStateBase *SMSystemMessageOpPass::OnWtIncreaseNotAllowed(void)
{
    return SMSystemMessageOpPassBase::OnWtIncreaseNotAllowed();
}

SMStateBase *SMSystemMessageOpPass::OnWtDecrease(void)
{
    return SMSystemMessageOpPassBase::OnWtDecrease();
}

long SMSystemMessageOpPass::GetDeviceClass()
{
    return SMSystemMessageOpPassBase::GetDeviceClass();
}

SMStateBase *SMSystemMessageOpPass::stateAfterPasswordVerify()
{
    return SMSystemMessageOpPassBase::stateAfterPasswordVerify();
}

void SMSystemMessageOpPass::showScreen(bool passwordVerified)
{
    SAWLDBScannerEnable();		// enable the scanner
    
    saMsgFuncPtr = PSProceduresBase::SecurityMessageModal;
    
    if ( !passwordVerified)
    {
        if (inputMode == PROMPTING_FOR_OPERATOR)
        {
            saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;
            
            STATE(OperatorPasswordState)::ShowEnterID();  //SR93 Biometrics support		
			
            ps.SetCMLeadthruText( TXT_ENTER_ID );
            
            // Determine the leadthru text to show
            int iMessage = MSG_ENTERID_PRESSENTER;
            COleVariant v = VARIANT_TRUE;
            bool bEmplid = (SendTBCommand(TBCMD_IS_EMPLID_ENABLED) == TB_SUCCESS_IMMEDIATE);
            if( bEmplid )
            {
                // ensure that if the scanner is not loaded due to a system message at startup
                // we show the keypad regardless of the setting
                bool bScannerLoaded = dm.IsDeviceAvailable(DMCLASS_SCANNER);
                bool bAllowHandKey = (SendTBCommand(TBCMD_IS_EMPLID_ALLOWHANDKEY) == TB_SUCCESS_IMMEDIATE);
                iMessage = bAllowHandKey || !bScannerLoaded ? MSG_ENTERID_SCANIDORKEY : MSG_ENTERID_SCANIDONLY;
                v = bAllowHandKey || !bScannerLoaded ? VARIANT_TRUE : VARIANT_FALSE;
				
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
            
			 m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("EnterID"), UI::PROPERTYVISIBLE, v);
            ps.Message12(iMessage, false);
            // Leadthru text message set
            
            ps.AllowInput( co.GetnOperationsOperatorIDMaxDigit(), false, false );
            ps.ShowCMFrame( false );   
            
            return;
        }
        else
        {  // Display System Message
            ps.SetCMFrame(_T("SystemMessageOpPass"));
        }
        // NewUI
    }
    else
    {
        SAWLDBScannerDisable();		// disable the scanner	
        theSystemMessage.showScreen(passwordVerified);
    } 
    
    if (!passwordVerified) 
    {
        ps.LeadThruText(LTT_WAITFORAPP);
        
        CString msgText;
        msgText = ps.GetPSText(MSG_WAITFORMESSAGE);
        msgText += "\n* ";
        msgText += ps.GetPSText(TXT_SYSTEMMESSAGE);
        ps.Message12(msgText, true);
        SAWLDBScannerDisable();		// disable the scanner	
        
    }
    if( lChangeDue > 0 )	// Tar 381865
	{ ps.ShowChangeDueAmountPaid( lChangeDue, lTotalPaid ); }
    else
	{ ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid); }
	
    ps.ShowCMTBText(csTBMessage2Scot);	//thp
    ps.ShowCMFrame(false);
}

CString SMSystemMessageOpPass::getWaveFile()
{
    return SMSystemMessageOpPassBase::getWaveFile();
}

SMStateBase *SMSystemMessageOpPass::PSButton1(void)
{
    return SMSystemMessageOpPassBase::PSButton1();
}

SMStateBase *SMSystemMessageOpPass::PSButton2(void)
{
    return SMSystemMessageOpPassBase::PSButton2();
}

SMStateBase *SMSystemMessageOpPass::PSButton3(void)
{
    return SMSystemMessageOpPassBase::PSButton3();
}

SMStateBase *SMSystemMessageOpPass::PSCancelKey(void)
{
    return SMSystemMessageOpPassBase::PSCancelKey();
}

SMStateBase *SMSystemMessageOpPass::OnBackToLGW(void)
{
    return SMSystemMessageOpPassBase::OnBackToLGW();
}