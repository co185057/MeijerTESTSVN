//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmUtility.CPP
//
// TITLE: Class implementation for store mode System Functions state
//
// AUTHOR:  Feng Zhang
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMSmUtility.h"
#include "transbrokerssf.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmUtility")


IMPLEMENT_DYNCREATE(SMSmUtility, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSmUtility::SMSmUtility()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSmUtility::Initialize(void)
{
    return SMSmUtilityBase::Initialize();
}

void SMSmUtility::UnInitialize(void)
{
    SMSmUtilityBase::UnInitialize();
}

bool SMSmUtility::storeMode(void)
{
    return SMSmUtilityBase::storeMode();
}

SMStateBase * SMSmUtility::PSButton1(void)
{
    return SMSmUtilityBase::PSButton1();
}

SMStateBase * SMSmUtility::PSButton2(void)
{
    return SMSmUtilityBase::PSButton2();
}

SMStateBase * SMSmUtility::PSButton3(void)
{
    return SMSmUtilityBase::PSButton3();
}

SMStateBase * SMSmUtility::PSButton4(void)
{
    return SMSmUtilityBase::PSButton4();
}

SMStateBase * SMSmUtility::PSButton5(void)
{
    return SMSmUtilityBase::PSButton5();
}

SMStateBase * SMSmUtility::PSButton6(void)
{
    return SMSmUtilityBase::PSButton6();
}

SMStateBase * SMSmUtility::PSButton8(void)
{
    return SMSmUtilityBase::PSButton8();
}

SMStateBase * SMSmUtility::PSButtonGoBack()
{
    return SMSmUtilityBase::PSButtonGoBack();
}

SMStateBase * SMSmUtility::TimedOut(void)
{
    return SMSmUtilityBase::TimedOut();
}

SMStateBase * SMSmUtility::PSReceiptUp(void)
{
    return SMSmUtilityBase::PSReceiptUp();
}

SMStateBase * SMSmUtility::PSReceiptDown(void)
{
    return SMSmUtilityBase::PSReceiptDown();
}

bool SMSmUtility::getOperatorPassword()
{
    return SMSmUtilityBase::getOperatorPassword();
}

void SMSmUtility::showScreen(bool passwordVerified)
{
    if(!passwordVerified)
    {
        if (inputMode == PROMPTING_FOR_OPERATOR || inputMode == SHOW_WAIT_FOR_APPROVAL)
        {
            //saMsgFuncPtr = ps.SecurityMessageNonmodal;
            
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
        
    }
    
    ps.SetCMFrame(_T("SmUtility"));
    
    return;
}

void SMSmUtility::UtilityExitFastLane(void)
{
    SMSmUtilityBase::UtilityExitFastLane();
}

void SMSmUtility::UtilityGetDiagFiles(void)
{
    SMSmUtilityBase::UtilityGetDiagFiles();
}

void SMSmUtility::UtilityRebootEntireSystem(void)
{
    SMSmUtilityBase::UtilityRebootEntireSystem();
}

void SMSmUtility::UtilityShutdownEntireSystem(void)
{
    SMSmUtilityBase::UtilityShutdownEntireSystem();
}

void SMSmUtility::UtilityVolumeControl(void)
{
    SMSmUtilityBase::UtilityVolumeControl();
}

void SMSmUtility::UtilityMultimediaTrainingProgram(void)
{
    SMSmUtilityBase::UtilityMultimediaTrainingProgram();
}

bool SMSmUtility::UtilityValidateOperator(CString csOperator, CString csPassword)
{
    return SMSmUtilityBase::UtilityValidateOperator(csOperator, csPassword);
}

SMStateBase * SMSmUtility::stateAfterPasswordVerify(void)
{
    return SMSmUtilityBase::stateAfterPasswordVerify();
}
