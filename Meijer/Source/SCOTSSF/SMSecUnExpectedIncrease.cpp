//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSecUnexpectedIncrease.cpp
//
// TITLE: Class header for SMSecUnexpectedIncrease
//
//
// AUTHOR:    Thanhhieu Pham
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMSecUnExpectedIncrease.h"

#include "transbrokerssf.h"

DLLIMPORT extern HWND hw;

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SecUnExpectedIncrease")


IMPLEMENT_DYNCREATE(SMSecUnExpectedIncrease, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP



//////////////////////////////////////////
SMSecUnExpectedIncrease::SMSecUnExpectedIncrease()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase  *SMSecUnExpectedIncrease::Initialize(void)
{
    return SMSecUnExpectedIncreaseBase::Initialize();
}

void SMSecUnExpectedIncrease::UnInitialize(void)
{
    SMSecUnExpectedIncreaseBase::UnInitialize();
}

bool SMSecUnExpectedIncrease::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMSecUnExpectedIncreaseBase::DeliverDescriptorList(dlParameterList);
}

bool SMSecUnExpectedIncrease::IgnoreTBMessageInSecurityState()
{
    return SMSecUnExpectedIncreaseBase::IgnoreTBMessageInSecurityState();
}

void SMSecUnExpectedIncrease::showScreen(bool passwordVerified)
{
    nTimeOut = co.getTimeOutPeriodicSecurityAudio();		// RFC 233322
    
    if (passwordVerified)
        return;
    
    
    if (inputMode == PROMPTING_FOR_OPERATOR)
    {
        // NewUI
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
        
        ps.AllowInput(co.GetnOperationsOperatorIDMaxDigit(),false,false);
        ps.ShowCMFrame(false);
        // End NewUI
        return;
    }
    else
    {  // Display Wait for Approval....   
        // TAR 200470 - this problem was discovered as a side effect of the show being slow
        // NewUI
        ps.SetCMFrame(_T("SecUnExpectedIncrease"));
        if (!co.fOperationsSuppressAudioInUnexpectedIncrease)
        {
            // the wave and background changes depending on if Belt is installed or not
            if (dm.fStateTakeawayBelt)
            {
                // TAR 195852 - changed to the play the correct wave file t.k
                
                //TAR379130/379131
                if ( co.getTimeOutDisplayIncreaseMisMatchScreenTimer() !=0 )
                {
                    SetTimer(hw, SHOWSCREEN, co.getTimeOutDisplayIncreaseMisMatchScreenTimer()*1000, &ShowScreenTimedOut);	
                }
                else
                {
                    DMSaySecurity(SEC_NONE, ps.GetSoundName(SEC_UNEXPECTEDITEMONBELT));	// play wave here so it doesn't replay on the key pad screen
                }
            }
            else
            {
                // TAR 195852 - changed to the play the correct wave file t.k
                DMSaySecurity(SEC_NONE, ps.GetSoundName(SEC_UNEXPECTEDITEM));	// play wave here so it doesn't replay on the key pad screen
            }
        }
        else //TAR379130/379131
        {
            if (dm.fStateTakeawayBelt && co.getTimeOutDisplayIncreaseMisMatchScreenTimer() !=0 )
            {
                SetTimer(hw, SHOWSCREEN, co.getTimeOutDisplayIncreaseMisMatchScreenTimer()*1000, &ShowScreenTimedOut);	
            }
        }
        COleVariant v = VARIANT_FALSE;
        if(nEscalationRAPTimeOut == 0)			// RFC 233322
        {
            v = VARIANT_TRUE;				// show the store mode button
        }
        m_pAppPSX->SetConfigProperty(_T("ButtonStoreLogIn"), csCurContext, UI::PROPERTYVISIBLE, v);
        
        // End NewUI
    }
    
    if (dm.fStateTakeawayBelt)
    {
    	// tar 395320 +
		// only show the instruction txt if DisplayIncreaseMisMatchScreenTimer == 0 in takeAwayBelt
		if( co.getTimeOutDisplayIncreaseMisMatchScreenTimer() == 0 )
		{
	    	ps.SetCMLeadthruText(LTT_BELT_ATTENTION);
			ps.Message12(MSG_BELT_UNEXPECTED_ITEM, true);
		}//tar 395320 - 

        // During the new security re-write decided that it does not make sense to display
        // Bag/Box button for the Takeaway belt. Since all items will be moved to the end
        // of the belt to be bagged. Got the PM approval
        m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
        
    }
    else
    {
        ps.SetCMLeadthruText(MSG_BAGGING_ALERT);
        ps.Message12(MSG_UNEXPECTED_WT, true);
        // During the new security re-write decided that it does not make sense to display
        // Bag/Box button for the Takeaway belt. Since all items will be moved to the end
        // of the belt to be bagged. Got the PM approval
        COleVariant v;
		//+Don't prompt for im using my own bag button 
        if(co.fOperationsAllowContainerBtn)
        {
            PSTEXT nBtt1 = BTT_BAGBOX;
	
			if((co.fOperationsDisableUnexpIncContainerBtnAftFirstItem && (lItems <=0 || (lItems==1 && m_bScanOnAttractAtQuietMode))) //litems=1 is allowed if it is scan at quietmode
					|| (!co.fOperationsDisableUnexpIncContainerBtnAftFirstItem))
			{
				v = VARIANT_TRUE;
			}
			else
			{
			    v = VARIANT_FALSE;
			}

            ps.SetButtonText(_T("CMButton1Med"), nBtt1);
        }
        else
        {
            v = VARIANT_FALSE;
		}//-Don't prompt for im using my own bag button

        m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), csCurContext, UI::PROPERTYVISIBLE, v);
    }
    
	if (bIsInFinalization && !co.fOperationsAllowContainerBtnInFinalization) // after F&P do not display bag/box - SR085 allows an option to display the bag/box
        m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
 
	//SR085 +
	// if(m_PrevState != BES_BAGANDEAS) commented for POS96816 and leave the line only that disable it.
	//{
		m_pAppPSX->SetConfigProperty(_T("CMSmallReceipt"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);//show the CMReceipt only if coming from quiet mode violation
/*	}
	else
	{
		m_pAppPSX->SetConfigProperty(_T("CMSmallReceipt"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);	// 408782
	} */
	//SR085 -
    
    // End NewUI
    SAWLDBScannerDisable();			// Disable the scanner
    ps.ShowCMTBText(csTBMessage2Scot);	//thp
    
    DisplayAuxScreen(AUX_SCREEN_DEFAULT, true); //tar 193987
    
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
}

SMStateBase  *SMSecUnExpectedIncrease::PSButton1(void)
{
    return SMSecUnExpectedIncreaseBase::PSButton1();
}

SMStateBase  *SMSecUnExpectedIncrease::stateAfterPasswordVerify()
{
    return SMSecUnExpectedIncreaseBase::stateAfterPasswordVerify();
}

SMStateBase  *SMSecUnExpectedIncrease::OnBackToLGW(void)
{
    //SOTF 7004
    bool bNeedApprovalState = false;
	
    if ((!isAnyApprovalRequired()) && 
        (m_PrevState == BES_WAITFORAPPROVAL) && 
        (!g_bgVerifySig) && 
        (g_bgVoidTrx))
	{
        bNeedApprovalState = true;
		
    }
	
    //sotf 7004
	
	
    // +TAR 394633
    SMStateBase * pRet = SMSecUnExpectedIncreaseBase::OnBackToLGW();
    if( (m_PrevState == BES_PICKINGUPITEMS) && (m_NextState == BES_SCANANDBAG) && pRet == STATE_NULL )
    {
        delete getRealStatePointer( pRet); 
        return setAndCreateAnchorState(BES_SCANANDBAG);
    }else if(bNeedApprovalState)
    {
        //sotf 7004
        delete getRealStatePointer( pRet); 
        return createState(BES_WAITFORAPPROVAL);
    }
    return pRet;
    // -TAR 394633
}

SMStateBase  *SMSecUnExpectedIncrease::TimedOut(void)
{
    return SMSecUnExpectedIncreaseBase::TimedOut();
}

SMStateBase  *SMSecUnExpectedIncrease::PSButtonGoBack(void)
{
    return SMSecUnExpectedIncreaseBase::PSButtonGoBack();
}

SMStateBase  *SMSecUnExpectedIncrease::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
    return SMSecUnExpectedIncreaseBase::OnRAPClearSecurityBtn(lpszAuthorizationType);
}

SMStateBase  *SMSecUnExpectedIncrease::OnWtIncreaseNotAllowed(void)
{
    return SMSecUnExpectedIncreaseBase::OnWtIncreaseNotAllowed();
}

SMStateBase  *SMSecUnExpectedIncrease::DMTakeawaySendScaleStatus()
{
    return SMSecUnExpectedIncreaseBase::DMTakeawaySendScaleStatus();
}

SMStateBase  *SMSecUnExpectedIncrease::OnWtDecrease(void)
{
    return SMSecUnExpectedIncreaseBase::OnWtDecrease();
}

SMStateBase  *SMSecUnExpectedIncrease::DMdf_EASNoMotion()
{
    return SMSecUnExpectedIncreaseBase::DMdf_EASNoMotion();
}

SMStateBase  *SMSecUnExpectedIncrease::OnTBNeedData(void)
{
    return SMSecUnExpectedIncreaseBase::OnTBNeedData();
}

void SMSecUnExpectedIncrease::Escalation2RAP(void)
{
    SMSecUnExpectedIncreaseBase::Escalation2RAP();
}

//TAR395214
SMStateBase *SMSecUnExpectedIncrease::OnNoMotionCashOutSensor(void) 
{
	
	
    if(bIsInFinalization)
    {
        trace(L6, _T("SMSecUnExpectedIncrease::OnNoMotionCashOutSensor"));
		SetDMCashTakenInSecState(true);
        SMStateBase::fCashOutSensorBlocked = false;
		
    }
	return STATE_NULL;
}
