#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMSecUnexpectedDecrease.h"      

#include "transbrokerssf.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SecUnexpectedDecrease")


IMPLEMENT_DYNCREATE(SMSecUnexpectedDecrease, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP



//////////////////////////////////////////
SMSecUnexpectedDecrease::SMSecUnexpectedDecrease()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase  *SMSecUnexpectedDecrease::Initialize(void)
{
    return SMSecUnexpectedDecreaseBase::Initialize();
}

void SMSecUnexpectedDecrease::UnInitialize(void)
{
    SMSecUnexpectedDecreaseBase::UnInitialize();
}

bool SMSecUnexpectedDecrease::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMSecUnexpectedDecreaseBase::DeliverDescriptorList(dlParameterList);
}

void SMSecUnexpectedDecrease::showScreen(bool passwordVerified)
{
    nTimeOut = co.getTimeOutPeriodicSecurityAudio();		// RFC 233322

	if (passwordVerified)
		return;
	

		if (inputMode == PROMPTING_FOR_OPERATOR)
		{
            // NewUI
			STATE(SecOperatorPWState)::ShowEnterID();   //SR93 Biometrics support			
			
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
		{  // Display unexpected decrease screen....   
			//CHU 072502 RFC 208874+
			//DMSaySecurity(SEC_NONE, ps.GetSoundName(SEC_REMOVEDITEM));	// play wave here so it doesn't replay on the key pad screen
			// TAR 253626 changed the check from (m_PrevState == BES_BAGANDEAS) to use bPrevStateBagAndEAS flag
			if ( bPrevStateBagAndEAS && co.fOperationsShowItemRemovedButtonOnRemoveItemViolationScreen)
			{
				 if (!co.fOperationsSuppressAudioInItemRemoveViolation ) 
				 {
					DMSaySecurity(SEC_NONE, ps.GetSoundName(SEC_ITEMREMOVEDONREMOVEVIOLATION));
				 }
			}
			else
				DMSaySecurity(SEC_NONE, ps.GetSoundName(SEC_REMOVEDITEM));	
			//CHU 072502 RFC 208874-
            // NewUI
			ps.SetCMFrame(_T("SecUnexpectedDecrease"));
			COleVariant v = VARIANT_FALSE;
			if(nEscalationRAPTimeOut == 0)			// RFC 233322
			{
				v = VARIANT_TRUE;				// show the store mode button
			}
			m_pAppPSX->SetConfigProperty(_T("ButtonStoreLogIn"), csCurContext, UI::PROPERTYVISIBLE, v);

			//CHU 072502 RFC 208874+
			// TAR 253626 changed the check from (m_PrevState == BES_BAGANDEAS) to use bPrevStateBagAndEAS flag
			if ( bPrevStateBagAndEAS  && co.fOperationsShowItemRemovedButtonOnRemoveItemViolationScreen)
			{	v = VARIANT_TRUE;
				ps.CMButton(_T("CMButton1Med"), BTT_ITEMREMOVED_ONITEMREMOVEVIOLATION, true);
			}
			else
			{
				v = VARIANT_FALSE;
			}
			//CHU 072502 RFC 208874-
			m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), csCurContext, UI::PROPERTYVISIBLE, v);
            // End NewUI
		}


	if (dm.fStateTakeawayBelt)
		ps.SetCMLeadthruText(MSG_ATTENTION_ALERT);
	else
		ps.SetCMLeadthruText(MSG_BAGGING_ALERT);
    // End NewUI
	SAWLDBScannerDisable();			// Disable the scanner
	ps.ShowCMTBText(csTBMessage2Scot);	//thp
	//CHU 072502 RFC 208874+
	//ps.Message12(MSG_OVERALL_WEIGHT_DECREASE, true);
	// TAR 253626 changed the check from (m_PrevState == BES_BAGANDEAS) to use bPrevStateBagAndEAS flag
	if ( bPrevStateBagAndEAS && co.fOperationsShowItemRemovedButtonOnRemoveItemViolationScreen)
		ps.Message12(MSG_RETURNITEMS_TO_BAGGING_ONITEMREMOVEVIOLATION, true);
	else
		ps.Message12(MSG_OVERALL_WEIGHT_DECREASE, true);
    //CHU 072502 RFC 208874-
	trace(L7, _T("+ra.OnTransactionState, Itemization State"));
	ra.OnSetTransactionState(RA_STATE_SECURITY_VIOLATION);
	trace(L7, _T("-ra.OnTransactionState"));
	ps.ShowCMFrame(false);
}

SMStateBase  *SMSecUnexpectedDecrease::stateAfterPasswordVerify()
{
    return SMSecUnexpectedDecreaseBase::stateAfterPasswordVerify();
}

SMStateBase  *SMSecUnexpectedDecrease::OnBackToLGW(void)
{
    //SOTF 7004
    bool bNeedApprovalState = false;
	
    if ((!isAnyApprovalRequired()) && 
        (m_PrevState == BES_WAITFORAPPROVAL) &&         
        (g_bgVoidTrx))
	{
        bNeedApprovalState = true;
		
    }
    SMStateBase * pRet = SMSecUnexpectedDecreaseBase::OnBackToLGW();
	
    if(bNeedApprovalState)
    {
        
        delete getRealStatePointer( pRet); 
        return createState(BES_WAITFORAPPROVAL);
    }
    return pRet;
	
    //sotf 7004
}

SMStateBase  *SMSecUnexpectedDecrease::TimedOut(void)
{
    return SMSecUnexpectedDecreaseBase::TimedOut();
}

SMStateBase  *SMSecUnexpectedDecrease::PSButtonGoBack(void)
{
    return SMSecUnexpectedDecreaseBase::PSButtonGoBack();
}

SMStateBase  *SMSecUnexpectedDecrease::PSButton1(void)
{
    DMSaySecurity( SEC_NONE, _T("blank.wav") );			//to interrupt the wave file if it's playing 153796
    
	if(fInValidOperatorPassword)
	{
		return STATE(SecOperatorPWState)::PSButton1();
	}
	
	//Set prevstate is BES_PICKINGUPITEMS and anchor state to ScanAndBag, so we go back to ScanAndBag
	//if m_PrevState is BES_SCANANDBBAG, it means that OnItemOK has been received 
	if (m_PrevState != BES_SCANANDBAG ) //407358
	{
		m_PrevState = BES_PICKINGUPITEMS;
	}
	trace(L6, _T("m_PrevState is %d"), m_PrevState);//407358
    setAnchorState(BES_SCANANDBAG);
    
    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_SKIP_BAGGING);
    _bstr_t bstrParmList((const _TCHAR *)parmList);
    BSTR bstrResult;
    bstrResult = NULL;
    SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
    SysFreeString(bstrResult);
    // + TAR 311901
    if ((secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_UNEX_DECREASE) && 
        (secEventParms.m_csDisposition == KEY_OSE_DISPOSITION_OUTSTANDING) )
    {
        OverrideSecurityException(_T("None"), KEY_EXCEPTION_OPERATION_CLEAR);
        
    }
    return STATE_NULL; // SM will drive us out of state
}

SMStateBase  *SMSecUnexpectedDecrease::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
    return SMSecUnexpectedDecreaseBase::OnRAPClearSecurityBtn(lpszAuthorizationType);
}

SMStateBase  *SMSecUnexpectedDecrease::OnMatchedWt(void)
{
    return SMSecUnexpectedDecreaseBase::OnMatchedWt();
}

SMStateBase  *SMSecUnexpectedDecrease::DMdf_EASNoMotion()
{
    return SMSecUnexpectedDecreaseBase::DMdf_EASNoMotion();
}

SMStateBase  *SMSecUnexpectedDecrease::OnWtIncreaseNotAllowed(void)
{
    return SMSecUnexpectedDecreaseBase::OnWtIncreaseNotAllowed();
}

SMStateBase  *SMSecUnexpectedDecrease::OnWtDecrease(void)
{
    return SMSecUnexpectedDecreaseBase::OnWtDecrease();
}

SMStateBase  *SMSecUnexpectedDecrease::OnTBNeedData(void)
{
    return SMSecUnexpectedDecreaseBase::OnTBNeedData();
}

void SMSecUnexpectedDecrease::Escalation2RAP(void)
{
    SMSecUnexpectedDecreaseBase::Escalation2RAP();
}

