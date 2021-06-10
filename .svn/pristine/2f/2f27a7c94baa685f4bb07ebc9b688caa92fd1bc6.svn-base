//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmSelectModeOfOperationBase.CPP
//
// TITLE: Class implementation for store mode Select Mode of Operation state
//
// AUTHOR:  Renato Taer
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMSmAuthorization.h"
#include "SMSmSelectModeOfOperation.h"
#include "SMSmSystemFunctions.h"
#include "SMSmmEditAdd.h"
#include "SMOutOfService.h"
#include "SMSmUtility.h"
#include "RemoteMessageAPI.h"		// External library ...\platform\RemoteMessage
#include "DMProcedures.h"		

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmSelectModeOfOperationBase")

static int nEncryptorKey = 0;
extern CString csGLOBALAddThe128Prefix; // CM/NN 090502 TAR 214029

IMPLEMENT_DYNCREATE(SMSmSelectModeOfOperationBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SmSelectModeOfOperation)


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSmSelectModeOfOperationBase::SMSmSelectModeOfOperationBase()
: m_bButtonEnabledSet(false) // TAR 313180
{
  IMPLEMENT_TIMESTAMP
}
#include "SMSmSelectModeOfOperationBase.h"

//////////////////////////////////////////////
// Initialize
//////////////////////////////////////////////
SMStateBase  *SMSmSelectModeOfOperationBase::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmSelectModeOfOperation") );	// RFC 330050
  ra.RequestEnableAssistMode(false); 
  showScreen(to.m_bTraceOn);
  SAWLDBScannerDisable();
  return STATE_NULL;
}

/////////////////////////////////////////////////////////
// showScreen
/////////////////////////////////////////////////////////
void SMSmSelectModeOfOperationBase::showScreen(const bool traceOn)
{
    ps.SetFrame(_T("SmSelectModeOfOperation"));
    ps.Echo(PS_BLANK);

	ps.SetLeadthruText(MSG_SELECTMODE_OPERATIONS, PS_BLANK);

    //Button 1
	if (TrainingModeObj.IsAllowed())
    {
        CString csButton1;
        bool bEnableTMButton = true;
        bEnableTMButton = !(fInSignedOn || fInSignedOff || fDoOptions); // do not allow training mode inside a transaction

        if (TrainingModeObj.IsTMOn())
        {
            // right now in training mode, set button text to leave training mode
            csButton1 = ps.GetPSText(MSG_LEAVE_TRAINING_MODE);
			ps.ShowTrainingModeOnScreen(TrainingModeObj.IsTMOn()); //tar 227613 AH 02/13/03
        }
        else
        {
            // right now SCOT is in non-training mode, set button text to enter training mode
            csButton1 = ps.GetPSText(MSG_ENTER_TRAINING_MODE);
        }
        ps.Button(_T("SMButton1"), csButton1, bEnableTMButton);
    }
    
    //Button 2
    if (co.fStateAttndModeAllowed)
    {
        if (co.IsAttendantModeOn())
        {
            ps.SMButton(_T("SMButton2"), MSG_LEAVE_ATTENDANT_MODE, true);
            UpdateStateName(ps.GetPSText(MSG_ATTENDANT, SCOT_LANGUAGE_PRIMARY));
        }
        else
        {
            ps.SMButton(_T("SMButton2"), MSG_ENTER_ATTENDANT_MODE, true);
            UpdateStateName(ps.GetPSText(RA_LANEBTT_STOREMODE, SCOT_LANGUAGE_PRIMARY));
        }
    }
    else
    {
        ps.SMButton(_T("SMButton2"), MSG_ENTER_ATTENDANT_MODE, false);
        UpdateStateName(ps.GetPSText(RA_LANEBTT_STOREMODE, SCOT_LANGUAGE_PRIMARY));
    }


	if ( co.IsDegradedModeOn() )
	{
		if ( dm.GetDegradedModeType() == DEGRADEDMODE_OFF )
		{
			ps.SMButton(_T("SMButton3"), BTT_ACCEPT_CARDONLY_MODE, true);
			ps.SMButton(_T("SMButton4"), BTT_ACCEPT_CASHONLY_MODE, true);
		}
		else
		{
			CString csButton3 = ps.GetPSText(BTT_ACCEPT_CARDONLY_MODE);
			CString csButton4 = ps.GetPSText(BTT_ACCEPT_CASHONLY_MODE);

			if ( dm.GetDegradedModeType() == DEGRADEDMODE_CARD_ONLY )
			{
				csButton3 = ps.GetPSText(BTT_EXIT_CARDONLY_MODE);
			}
			else if ( dm.GetDegradedModeType() == DEGRADEDMODE_CASH_ONLY )
			{
				csButton4 = ps.GetPSText(BTT_EXIT_CASHONLY_MODE);
			}
			else
			{
				trace(L6, _T("SMSelectModeOfOperation:%d is invalid degraded mode type."), dm.GetDegradedModeType());	
			}

			ps.SMButton(_T("SMButton3"), csButton3, (dm.GetDegradedModeType()==DEGRADEDMODE_CARD_ONLY || dm.GetDegradedModeType()==DEGRADEDMODE_OFF )); 	
			ps.SMButton(_T("SMButton4"), csButton4, (dm.GetDegradedModeType()==DEGRADEDMODE_CASH_ONLY || dm.GetDegradedModeType()==DEGRADEDMODE_OFF)); 

		}
		m_pAppPSX->SetConfigProperty(_T("Title1"), _T("SmSelectModeOfOperation"), UI::PROPERTYTEXTFORMAT, _T(""));
	}
	else
	{
		ExitDegradedMode();
		ps.SMButton(_T("SMButton3"), BTT_ACCEPT_CARDONLY_MODE, false);
		ps.SMButton(_T("SMButton4"), BTT_ACCEPT_CASHONLY_MODE, false);
	}
    ps.SetTitle(MSG_MODE_OPERATION, PS_BLANK);	//tar 409031
    ps.SMButton(_T("SMButton8"), BTT_BUYORCANCEL, true);
    ps.ShowSMTotal(true);
    ps.ShowSMTBText(csTBMessage2Scot);	
    ps.SMShowItemReceipt(true);
    ps.ShowFrame();
}

/////////////////////////////////////////////
// Button 1 - Training Mode
/////////////////////////////////////////////
SMStateBase  *SMSmSelectModeOfOperationBase::PSButton1(void)
{
	trace(L6, _T("training mode button pressed"));
    ASSERT(TrainingModeObj.IsAllowed());
    CString csButton4;
    bool bTMState;
    if (TrainingModeObj.IsTMOn())
    {
        // user wants to leave training mode
        bTMState = false;


    }
    else
    {
        // user wants to enter training mode
        bTMState = true;;
    }
    DisableButtons();  // TAR 313180
    SMStateBase *pRetVal = TBSetTrainingModeState(bTMState);
    ReenableButtons(); // TAR 313180

    return pRetVal;
}

/////////////////////////////////////////////
// Button 2 - Attendant Mode
/////////////////////////////////////////////
SMStateBase  *SMSmSelectModeOfOperationBase::PSButton2(void)
{
    trace(L6, _T("+PSButton2():Attendant mode button pressed"));
    if (co.fStateAttndModeAllowed)
    {
        if (!fSAInTransaction)
        {
            rp.TransactionStart(rp.m_csBusinessDate);
        }
        if (co.IsAttendantModeOn())
        {
            trace(L6, _T("Exiting Attendant mode"));
            if (co.IsRemoteScannerConnected())
            {
                dm.ScannerRemoteAccess(false);
                co.SetRemoteScannerConnected(false);
            }
            ps.SMButton(_T("SMButton7"), MSG_LEAVE_ATTENDANT_MODE, true);
			rp.SetOperator(csOperator);
            co.SetAttendantID(EMPTY_STRING);
            co.SetAttendantMode(false);

			// TAR 336337:  4.0E3_244 - RAP hangs after voiding item then go back to quad view.
			//	  Migrated fix for 310488/287456 from 4.0 D.
			// TAR 310488/287456 - removed the code that was here where it issued stop-assist-mode
			// to security manager. Since when TellApplicationStateToSecurity is called 
			// later issues stop-assist-mode. The two stop-assist-mode calls back to back
			// does not allow the Takeaway belt to go back to reset state.
			if (fSAInTransaction)
			{
				SASendSCOTInput( TRANSACTION_CONTINUE );
			}
			TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmAssistMode") );
			rp.ModeChange(rp.m_lAttendantEntryID, ATTENDANT, EXIT);
            DMTakeawayBeltNotify(tabEventAssistModeStop);
        }
        else
        {
            trace(L6, _T("Entering Attendant mode"));
            ps.SMButton(_T("SMButton7"), MSG_ENTER_ATTENDANT_MODE, true);
			// +TAR 310488/287456 - need to issue this before going to assist mode
			if(!fSAInTransaction)
			{
				SASendSCOTInput( ITEMIZATION );
			}
			// -TAR 310488/287456
			TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmAssistMode") );
            co.SetAttendantID(csOperator);
            co.SetAttendantMode(true);
			rp.SetOperator(csOperator);
			rp.ModeChange(rp.m_lAttendantEntryID, ATTENDANT, ENTER);
            DMTakeawayBeltNotify(tabEventAssistModeStart);
        }
		rp.m_lAttendantEntryID += 1;
        if (!fSAInTransaction)
        {
            rp.TransactionEnd();
        }
        // ShowTrainingModeOnScreen will update training/attendant text 
        // and show/hide mode indicators. 
        ps.ShowTrainingModeOnScreen(TrainingModeObj.IsTMOn());
        showScreen(to.m_bTraceOn);
    }
	trace(L6, _T("-PSButton2()"));

    //set lane closed flag to make SMAuthorization return to shopping
    m_bIsAttendantModeChanged = true; //TAR371918 & 371932
    //m_bIsFromLaneClosed = true; //TAR371918 & 371932 remove
    RETURNSTATE(SmAuthorization)
}

/////////////////////////////////////////////
// Button 3 - Cards Only Mode
/////////////////////////////////////////////
SMStateBase  *SMSmSelectModeOfOperationBase::PSButton3(void)
{
	trace(L6, _T("SmSelectModeOfOperationBase::PSButton3 button pressed."));

	COleVariant cOleV;
	m_pAppPSX->GetConfigProperty( _T("SMButton3"), _T("SmSelectModeOfOperation"), UI::PROPERTYTEXTFORMAT, cOleV );
	CString sButtonLabel(cOleV.bstrVal);
	trace( L6, _T("SmSelectModeOfOperationBase  Buttontext : %s"), sButtonLabel);

	if ( ps.GetPSText(BTT_ACCEPT_CARDONLY_MODE) == sButtonLabel )
	{
		EnterDegradedMode(DEGRADEDMODE_CARD_ONLY);
		co.fTenderCashAllowed = false;
		SetDeviceEnabled(false);
	}
	else //exit cards only mode
	{
		ExitDegradedMode();
		co.fTenderCashAllowed = true;
		SetDeviceEnabled(true);


	    for (dm.m_lDeviceClassThatWasInErrorIterator = dm.m_lDeviceClassThatWasInError.begin(); 
		     dm.m_lDeviceClassThatWasInErrorIterator != dm.m_lDeviceClassThatWasInError.end(); 
			 dm.m_lDeviceClassThatWasInErrorIterator++)
		{
	
			switch( *(dm.m_lDeviceClassThatWasInErrorIterator) )
			{
			case DMCLASS_CASHCHANGER:
				{
					if ( dm.m_csCashDispenserModel == _CASHDISPENSER_BNR )
					{
						//dm.ReloadDevice(deviceClass);
						dm.ReloadCashAcceptor();
						dm.ReloadCashChanger();
					} 
				
					if ( dm.m_csCashDispenserModel == _COINRECYCLER_BCR )
					{
						//dm.ReloadDevice(deviceClass);
						dm.ReloadCoinAcceptor();
						dm.ReloadCashChanger();
					}
					else
					{
						//dm.ReloadDevice(deviceClass);
						dm.ReloadCashChanger();
					}
				}break;
			case DMCLASS_CASHACCEPTOR:
				{
					if ( dm.m_csCashDispenserModel == _CASHDISPENSER_BNR )
					{
						//dm.ReloadDevice(deviceClass);
						dm.ReloadCashAcceptor();
						dm.ReloadCashChanger();
					}
					else
					{
						//dm.ReloadDevice(deviceClass);
						dm.ReloadCashAcceptor();
					}
				}break;
			case DMCLASS_COINACCEPTOR:
				{
					if ( dm.m_csCoinDispenserModel == _COINRECYCLER_BCR )
					{
						//dm.ReloadDevice(deviceClass);
						dm.ReloadCoinAcceptor();
						dm.ReloadCashChanger();
					}
					else
					{
						//dm.ReloadDevice(deviceClass);
						dm.ReloadCoinAcceptor();
					}
				}break;
			case DMCLASS_CASHRECYCLER:
				{
					 //dm.ReloadDevice(deviceClass);
					 dm.ReloadCoinAcceptor();
					 dm.ReloadCashAcceptor();
					 dm.ReloadCashChanger();

				}break;
			default: 
				break;
			}
		}
		dm.m_lDeviceClassThatWasInError.clear();
	}

	RETURNSTATE(SmSelectModeOfOperation);
}


/////////////////////////////////////////////
// Button 4 -  Cash Only Mode
/////////////////////////////////////////////
SMStateBase  *SMSmSelectModeOfOperationBase::PSButton4(void)
{
	trace(L6, _T("SmSelectModeOfOperationBase::PSButton4 button pressed."));

	COleVariant cOleV;
	m_pAppPSX->GetConfigProperty( _T("SMButton4"), _T("SmSelectModeOfOperation"), UI::PROPERTYTEXTFORMAT, cOleV );
	CString sButtonLabel(cOleV.bstrVal);
	trace( L6, _T("SmSelectModeOfOperationBase  Buttontext : %s"), sButtonLabel);
	
	if ( ps.GetPSText(BTT_ACCEPT_CASHONLY_MODE) == sButtonLabel )
	{
		EnterDegradedMode(DEGRADEDMODE_CASH_ONLY);
		co.fTenderCashAllowed = true;
		
		DMMSRDisable();	
	}
	else //exit cash only mode
	{
		ExitDegradedMode();
		co.fTenderCashAllowed = true;

		for (dm.m_lDeviceClassThatWasInErrorIterator = dm.m_lDeviceClassThatWasInError.begin(); 
		     dm.m_lDeviceClassThatWasInErrorIterator != dm.m_lDeviceClassThatWasInError.end(); 
			 dm.m_lDeviceClassThatWasInErrorIterator++)
		{

			switch ( *(dm.m_lDeviceClassThatWasInErrorIterator)  )
			{
			case DMCLASS_MSR:
				{
					//Tar 418070
					long lReloadRet = dm.ReloadMSR();
					if (lReloadRet != OPOS_SUCCESS)
					{
						if (!dm.IsDeviceConfigured(DMCLASS_MSR,0))
						{
							  CString csWork;
							  csWork = ps.GetPSText(MSG_DEVICESERROR); // code from below replaced here
							  csWork += DMGetDevicesInError(); 
							  return createLevel1Message(csWork,            // char *message
														PS_NOTEXT,         // no canned message
														PS_NOTEXT,         // no prompt
														0,                 // no prompt value
														DMCLASS_MSR,		  // -1 = no device
														true);        // whether DM check health needed

						}
					}
				}break;
			default: 
				break;
			}	
		}
		dm.m_lDeviceClassThatWasInError.clear();

		DMMSREnable();
	}
	RETURNSTATE(SmSelectModeOfOperation);
}

////////////////////////////////////////////      /
// Button 8 Go Back
/////////////////////////////////////////////
SMStateBase  *SMSmSelectModeOfOperationBase::PSButton8(void)
{
    //Return to current store mode menu state instead of SmSystemFunctions
    return STATE(StoreModeMenu)::CreateStoreModeMenu();
}


////////////////////////////////////////
// NewUI
SMStateBase  *SMSmSelectModeOfOperationBase::PSReceiptUp(void)
{
  ps.ScrollUp(_T("SMReceipt"));
  return STATE_NULL;

}
SMStateBase  *SMSmSelectModeOfOperationBase::PSReceiptDown(void)
{
  ps.ScrollDown(_T("SMReceipt"));
  return STATE_NULL;

}
// NewUI

// Function added for TAR 313180
void SMSmSelectModeOfOperationBase::DisableButtons()
{
   CString csCtxt = _T("SmSelectModeOfOperation");
   COleVariant vType( _T("") );
   CString csTemp = EMPTY_STRING;
   if (!m_bButtonEnabledSet)
   {
      for (int i = 1; i <= 8; i++)
      {
         m_bButtonEnabled[i-1] = false;
         csTemp.Format(_T("SMButton%d"), i);
         if (PSX_SUCCESS == m_pAppPSX->GetConfigProperty(csTemp, csCtxt, UI::PROPERTYSTATE, vType))
         {
            if (vType.lVal == UI::Normal)
            {
               m_bButtonEnabled[i-1] = true;
               vType = (long) UI::Disabled;
               m_pAppPSX->SetConfigProperty(csTemp, csCtxt, UI::PROPERTYSTATE, vType);
            }
         }
      }
      m_bButtonEnabledSet = true;
   }

}

// Function added for TAR 313180
void SMSmSelectModeOfOperationBase::ReenableButtons()
{
   if (!m_bButtonEnabledSet)
   {
      return;
   }
   CString csCtxt = _T("SmSelectModeOfOperation");
   COleVariant vType = (long) UI::Normal;
   CString csTemp = EMPTY_STRING;
   for (int i = 1; i <= 8; i++)
   {
      if (m_bButtonEnabled[i-1])
      {
         csTemp.Format(_T("SMButton%d"), i);
         m_pAppPSX->SetConfigProperty(csTemp, csCtxt, UI::PROPERTYSTATE, vType);
      }
   }
   m_bButtonEnabledSet = false;
}
// + RFC 330050
void SMSmSelectModeOfOperationBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmSelectModeOfOperation") );	
}
// - RFC 330050


void SMSmSelectModeOfOperationBase::SetDeviceEnabled(bool fEnabled)
{
	trace(L6,_T("+SMSmSelectModeOfOperationBase::SetDeviceEnabled()"));
	if( !dm.m_csCashRecyclerModel.IsEmpty() )
	{
		dm.Enable(DMCLASS_CASHRECYCLER, 0, fEnabled);				
	}
	else
	{
		if ( dm.m_csCashDispenserModel == _CASHDISPENSER_BNR )
		{
			dm.Enable(DMCLASS_CASHACCEPTOR, 0, fEnabled);
		}
		

		if ( dm.m_csCoinDispenserModel == _COINRECYCLER_BCR )
		{
			dm.Enable(DMCLASS_COINACCEPTOR, 0, fEnabled);
		}
		else
		{
			dm.Enable(DMCLASS_CASHACCEPTOR, 0, fEnabled);
			dm.Enable(DMCLASS_COINACCEPTOR, 0, fEnabled);
		}
	dm.Enable(DMCLASS_CASHCHANGER, 0, fEnabled); //remove it here.it calls out for all the cases
	}
	trace(L6,_T("-SMSmSelectModeOfOperationBase::SetDeviceEnabled()"));
	return;
}
