//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMRemoteScannerConnect.cpp
//
// Description: See h file
//
// Author:  Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMRemoteScannerConnect.h"       // MGV sys msg 3 state

#include "transbrokerssf.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SystemMessage6")

IMPLEMENT_DYNCREATE(SMRemoteScannerConnect, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

/////////////////////////////////////////////////////
// Constructor
/////////////////////////////////////////////////////
SMRemoteScannerConnect::SMRemoteScannerConnect
                (bool bConnect) // DMCheckHealth after msg?
// Construct our parent, most things are handled there
: SMRemoteScannerConnectBase(bConnect)

{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMRemoteScannerConnect::SMRemoteScannerConnect()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMRemoteScannerConnect::Deliver(bool bConnect)
{
  SMRemoteScannerConnectBase::Deliver(bConnect);


  return this;
}

SMStateBase *SMRemoteScannerConnect::Initialize(void)
{
    return SMRemoteScannerConnectBase::Initialize();
}

void SMRemoteScannerConnect::UnInitialize(void)
{
    SMRemoteScannerConnectBase::UnInitialize();
}

SMStateBase *SMRemoteScannerConnect::PSButton1(void)
{
    return SMRemoteScannerConnectBase::PSButton1();
}

SMStateBase *SMRemoteScannerConnect::PSButton2(void)
{
    return SMRemoteScannerConnectBase::PSButton2();
}

SMStateBase *SMRemoteScannerConnect::PSButton3(void)
{
    return SMRemoteScannerConnectBase::PSButton3();
}

SMStateBase *SMRemoteScannerConnect::PSButton8(void)
{
    return SMRemoteScannerConnectBase::PSButton8();
}

SMStateBase *SMRemoteScannerConnect::stateAfterPasswordVerify(void)
{
    return SMRemoteScannerConnectBase::stateAfterPasswordVerify();
}

bool SMRemoteScannerConnect::PSRemoteLegalForState()
{
    return SMRemoteScannerConnectBase::PSRemoteLegalForState();
}

bool SMRemoteScannerConnect::storeMode()
{
    return SMRemoteScannerConnectBase::storeMode();
}

void SMRemoteScannerConnect::showScreen(bool passwordVerified)
{
    trace(L6, _T("SMRemoteScannerConnect::showScreen(%d)"), passwordVerified);
    SAWLDBScannerEnable();		// enable the scanner
    
    saMsgFuncPtr = ps.SecurityMessageModal;
    
    if (!passwordVerified)
    {
        if (inputMode == PROMPTING_FOR_OPERATOR)
        {
            saMsgFuncPtr = ps.SecurityMessageNonmodal;
            
            ps.SetCMFrame( _T("EnterID") );
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
            // Leadthru text message set
			
            ps.AllowInput( co.GetnOperationsOperatorIDMaxDigit(), false, false );
            ps.ShowCMFrame( false );   
            
            return;
        }
        // NewUI
    }
    else
    {
        if (ps.RemoteMode() || co.IsInAssistMenus() || storeMode()) 
        {
            // while in remote mode, got remote scanner connect
            showRemoteScreen();
        }
        else
        {
            showLocalScreen();
        }
    } 
    
    if (!passwordVerified) 
    {
        ps.SetCMFrame(_T("SystemMessageOpPass"));
        ps.LeadThruText(LTT_WAITFORASSISTANCE);
        
        CString msgText;
        msgText = ps.GetPSText(MSG_WAITFORMESSAGE);
        msgText += "\n* ";
        msgText += ps.GetPSText(MSG_REMOTESCANNER_CONNECT);
        
        ps.Message12(msgText, true);
        SAWLDBScannerDisable();		// disable the scanner	
        
    }
    ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
    ps.ShowCMTBText(csTBMessage2Scot);	//thp
    ps.ShowCMFrame(false); 
}

SMStateBase *SMRemoteScannerConnect::DMRemoteScannerConnect(bool bConnect)
{
    return SMRemoteScannerConnectBase::DMRemoteScannerConnect(bConnect);
}

void SMRemoteScannerConnect::showRemoteScreen(void)
{
	SMRemoteScannerConnectBase::showRemoteScreen();
}

void SMRemoteScannerConnect::showLocalScreen(void)
{
    SMRemoteScannerConnectBase::showLocalScreen();
}
