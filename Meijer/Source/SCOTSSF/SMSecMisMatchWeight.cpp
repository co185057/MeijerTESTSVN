#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMSecMisMatchWeight.h"      

#include "transbrokerssf.h"
#include "SMSystemMessage1.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SecMisMatchWeight")

DLLIMPORT extern HWND hw;

IMPLEMENT_DYNCREATE(SMSecMisMatchWeight, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP



//////////////////////////////////////////
SMSecMisMatchWeight::SMSecMisMatchWeight()
: m_bPasswordVerified(false)
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase  *SMSecMisMatchWeight::Initialize(void)
{
    if (m_bPasswordVerified)
		return SMSecMisMatchWeightBase::stateAfterPasswordVerify();
	else
		return SMSecMisMatchWeightBase::Initialize();
}

void SMSecMisMatchWeight::UnInitialize(void)
{
    SMSecMisMatchWeightBase::UnInitialize();
}

bool SMSecMisMatchWeight::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMSecMisMatchWeightBase::DeliverDescriptorList(dlParameterList);
}

void SMSecMisMatchWeight::showScreen(bool passwordVerified)
{
    nTimeOut = co.getTimeOutPeriodicSecurityAudio();	// RFC 233322
    
    if (passwordVerified)
        return;
    
    CPSXRedrawLock csRedrawLock(_T("Display"), _T("SMSecMisMatchWeightBase::showScreen"));
    
    if (inputMode == PROMPTING_FOR_OPERATOR)
    {
        STATE(SecOperatorPWState)::ShowEnterID();  //SR93 Biometrics support			
        
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
    else
    {  // Display Wait for Approval....   
        // NewUI
        ps.SetCMFrame(_T("SecMisMatchWeight"));
        
        //RFC232801
        ps.SetCMBackground(BMP_ASSISTANCENEEDEDITEMDETAILS);
        
        //TAR379130/379131
        if (dm.fStateTakeawayBelt && ( co.getTimeOutDisplayIncreaseMisMatchScreenTimer() !=0 ) )
        {
            
            SetTimer(hw, SHOWSCREEN, co.getTimeOutDisplayIncreaseMisMatchScreenTimer()*1000, &ShowScreenTimedOut);	
        }
        else if (!co.fOperationsSuppressAudioInMismatch)				
        {
            DMSaySecurity(SEC_NONE, ps.GetSoundName(ASSISTANCENEEDED));
        }
        
        COleVariant v = VARIANT_FALSE;;		// hide the store login button
        if(nEscalationRAPTimeOut == 0)		// RFC 233322
        {
            v = VARIANT_TRUE;				// show the store mode button
        }
        //TAR 256154
        if (secEventParms.m_bMultiPackItem)
        {
            v = VARIANT_FALSE;
        }
        m_pAppPSX->SetConfigProperty(_T("ButtonStoreLogIn"), csCurContext, UI::PROPERTYVISIBLE, v);
    }
    
	//tar 395320 +
	//only show the instruction if we are about to show the screen
	if (!(dm.fStateTakeawayBelt && (co.getTimeOutDisplayIncreaseMisMatchScreenTimer() !=0 )))
	{
		//RFC 232801 
		CString csWork;
		ps.SetCMLeadthruText(LTT_WAITFORASSISTANCE);
		if ( secEventParms.m_bWeight2x )
		{
			csWork = ps.GetPSText(MSG_CHECKITEMWEIGHTDOUBLE);	
		}
		else
		{
			csWork = ps.GetPSText(MSG_CHECKITEMWEIGHT);	
		}
		ps.Message12(csWork, true);
	}//tar 395320 -
    COleVariant v;
    if (dm.fStateTakeawayBelt)
    {
        v = VARIANT_FALSE;
        m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), csCurContext, UI::PROPERTYVISIBLE, v);
    }
    else
    {
        if(co.fOperationsAllowContainerBtnForMismatching && !secEventParms.m_bMultiPackItem)
        {
            PSTEXT nBtn1 = BTT_BAGBOX;
            v = VARIANT_TRUE;
            ps.SetButtonText(_T("CMButton1Med"), nBtn1);
        }
        else
        {
            v = VARIANT_FALSE;
        }
        m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), csCurContext, UI::PROPERTYVISIBLE, v);
        // End NewUI
    }
    SAWLDBScannerDisable();			// Disable the scanner
    ps.ShowCMTBText(csTBMessage2Scot);	//thp
    trace(L7, _T("+ra.OnTransactionState, Itemization State"));
    ra.OnSetTransactionState(RA_STATE_SECURITY_VIOLATION);
    trace(L7, _T("-ra.OnTransactionState"));
    
    //TAR379130/379131
    if (dm.fStateTakeawayBelt && (co.getTimeOutDisplayIncreaseMisMatchScreenTimer() !=0 ))
    {
        trace(L6, _T("Do not display the screen"));
    }
    else
    {
        ps.ShowCMFrame(false);
    }
    //ps.ShowCMFrame(false);
}

//****************************************************************
//	DG30 Feature:	Show expected & actual weight 
//					after operator logs in.
//****************************************************************
SMStateBase  *SMSecMisMatchWeight::stateAfterPasswordVerify()
{
    m_bPasswordVerified = true;
	
	CString csWork = ps.GetPSText(MSG_DATANEEDED_MISMATCHWEIGHT);
	csWork.Format(csWork + _T("\n\n     Expected Weight: %s"), secEventParms.m_csExpectedWeight);
	csWork.Format(csWork + _T("\n\n     Actual Weight: %s"), secEventParms.m_csActualWeight );
	
	CREATE_AND_DISPENSE(SystemMessage1)(csWork,		//char *message
		PS_NOTEXT,
		PS_NOTEXT,	// no prompt
		0,			// no value
		-1,			// no device
		false);     // no DM check health after
	
	return STATE_NULL;
}

SMStateBase  *SMSecMisMatchWeight::OnBackToLGW(void)
{
    return SMSecMisMatchWeightBase::OnBackToLGW();
}

SMStateBase  *SMSecMisMatchWeight::TimedOut(void)
{
    return SMSecMisMatchWeightBase::TimedOut();
}

//////////////////////////////////////
// PSButton1 - Button Processing
//////////////////////////////////////
SMStateBase  *SMSecMisMatchWeight::PSButton1(void)
{
    // POS29349: Let sales app know that customer has brought their own shopping bag.
	SendTBCommand(TBCMD_NOTIFY_BROUGHT_BAG, true);
	
	// Call base functionality for button processing.
	SMStateBase *pRetState = SMSecMisMatchWeightBase::PSButton1();
	
	return pRetState;
}

SMStateBase  *SMSecMisMatchWeight::PSButtonGoBack(void)
{
    return SMSecMisMatchWeightBase::PSButtonGoBack();
}

SMStateBase  *SMSecMisMatchWeight::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
    return SMSecMisMatchWeightBase::OnRAPClearSecurityBtn(lpszAuthorizationType);
}

SMStateBase  *SMSecMisMatchWeight::OnWtIncreaseNotAllowed(void)
{
    return SMSecMisMatchWeightBase::OnWtIncreaseNotAllowed();
}

SMStateBase  *SMSecMisMatchWeight::OnMatchedWt(void)
{
    return SMSecMisMatchWeightBase::OnMatchedWt();
}

void SMSecMisMatchWeight::Escalation2RAP(void)
{
    SMSecMisMatchWeightBase::Escalation2RAP();
}
