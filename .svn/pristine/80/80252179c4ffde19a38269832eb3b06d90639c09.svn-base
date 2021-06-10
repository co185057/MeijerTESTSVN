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
//               // MS MFC template header

#ifndef _CPPUNIT
#include "Common.h"                 // MGV common includes

#include "SMSmAuthorization.h"
#include "SMSmSelectModeOfOperation.h"
#include "SMSmSystemFunctions.h"
#include "SMSmmEditAdd.h"
#include "SMOutOfService.h"
#include "SMSmUtility.h"
#include "RemoteMessageAPI.h"		// External library ...\platform\RemoteMessage
#include "DMProcedures.h"		
#else
#include "SMSmSelectModeOfOperationBase.h"
#endif //_CPPUNIT

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmSelectModeOfOperationBase")

#ifndef _CPPUNIT
static int nEncryptorKey = 0;
extern CString csGLOBALAddThe128Prefix; // CM/NN 090502 TAR 214029
#endif //_CPPUNIT

IMPLEMENT_DYNCREATE(SMSmSelectModeOfOperationBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SmSelectModeOfOperation)


DEFINE_TIMESTAMP

#ifndef _CPPUNIT
//////////////////////////////////////////
SMSmSelectModeOfOperationBase::SMSmSelectModeOfOperationBase()
: m_bButtonEnabledSet(false) // TAR 313180
{
  IMPLEMENT_TIMESTAMP
	 bCashDeviceErrorToDispaly = true;  //415172
}
#include "SMSmSelectModeOfOperationBase.h"

//////////////////////////////////////////////
// Initialize
//////////////////////////////////////////////
SMStateBase  *SMSmSelectModeOfOperationBase::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmSelectModeOfOperation") );	// RFC 330050
  ra.RequestEnableAssistMode(false); 
  
  //+TAR 435176 setting button4 visibility into false since it is minihardware, it does not accept any cash payment at all.
  trace(L6, _T("SMSelectModeOfOperation:%d "), dm.GetDegradedModeType());
  if( IsMINIHardware() || IsSUPERMINI() )	//added check for SuperMini HW type.
  {
	  m_pAppPSX->SetConfigProperty(_T("SMButton4"), 
		  _T("SmSelectModeOfOperation"),UI::PROPERTYVISIBLE, VARIANT_FALSE);
		  
	  m_pAppPSX->SetConfigProperty( _T("SMButton3"), _T("SmSelectModeOfOperation"), 
          UI::PROPERTYSTATE, (long) UI::Disabled);
  }
  //-TAR 435176

   //Tar 450827
	if ( co.IsDegradedModeOn() && dm.GetDegradedModeType() == DEGRADEDMODE_OFF )
    {
		if (dm.IsDeviceConfiguredNotLoaded(DMCLASS_CASHACCEPTOR,0))
		{
			dm.ReloadCashAcceptor();	
		}
		
		if (dm.IsDeviceConfiguredNotLoaded(DMCLASS_COINACCEPTOR,0))
		{
			dm.ReloadCoinAcceptor();
		}

		if (dm.IsDeviceConfiguredNotLoaded(DMCLASS_CASHCHANGER,0))
		{
			dm.ReloadCashChanger();
		}

        if (IsCashDeviceConfigured())	//SSCOP-3937
        {
            SMStateBase  * pRetState = DisplayCashDeviceError();
            if (bCashDeviceErrorToDispaly && (pRetState != STATE_NULL) )  //415172
            {
                return pRetState;
            }
        }
    }
   
  showScreen(to.m_bTraceOn);
  SAWLDBScannerDisable();
  return STATE_NULL;
}
#endif
/////////////////////////////////////////////////////////
// showScreen
/////////////////////////////////////////////////////////
void SMSmSelectModeOfOperationBase::showScreen(const bool traceOn)
{
#ifndef _CPPUNIT
    ps.SetFrame(_T("SmSelectModeOfOperation"));
    ps.Echo(PS_BLANK);
    //+ SSCOP-2255
	//ps.SetLeadthruText(MSG_SELECTMODE_OPERATIONS, PS_BLANK);
    //- SSCOP-2255
#endif
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
    else
    {
         ps.Button(_T("SMButton1"), _T(""), false);
    }
#ifndef _CPPUNIT
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

	if ( co.IsDegradedModeOn() && !STATE(StoreModeState)::Is5XMiniHardware() && IsCashDeviceConfigured())	//SSCOP-3937
	{
		if ( dm.GetDegradedModeType() == DEGRADEDMODE_OFF && !STATE(StoreModeState)::Is5XMiniHardware()) 	//SSCOP-762
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
			
			// +SSCOP-762
            if(!STATE(StoreModeState)::Is5XMiniHardware())	//TAR 457891
            {
                ps.SMButton(_T("SMButton4"), csButton4, (dm.GetDegradedModeType()==DEGRADEDMODE_CASH_ONLY || dm.GetDegradedModeType()==DEGRADEDMODE_OFF)); 
            }
            // -SSCOP-762

		}
		//m_pAppPSX->SetConfigProperty(_T("Title1"), _T("SmSelectModeOfOperation"), UI::PROPERTYTEXTFORMAT, _T(""));  //SSCOP-2255
	}
	else
	{
		ps.SMButton(_T("SMButton3"), BTT_ACCEPT_CARDONLY_MODE, false);
		ps.SMButton(_T("SMButton4"), BTT_ACCEPT_CASHONLY_MODE, false);
	}
    //ps.SetTitle(MSG_MODE_OPERATION, PS_BLANK);	//tar 409031  //SSCOP-2255
    ps.SMButton(_T("SMButton8"), BTT_BUYORCANCEL, true);
    ps.ShowSMTotal(true);
    ps.ShowSMTBText(csTBMessage2Scot);	
    ps.SMShowItemReceipt(true);
    ps.ShowFrame();
#endif
}
#ifndef _CPPUNIT

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
#endif // _CPPUNIT

/////////////////////////////////////////////
// Button 2 - Attendant Mode
/////////////////////////////////////////////
SMStateBase  *SMSmSelectModeOfOperationBase::PSButton2(void)
{
    trace(L6, _T("+PSButton2():Attendant mode button pressed"));
    if (co.GetfStateAttndModeAllowed())
    {
#ifndef _CPPUNIT
        if (!fSAInTransaction)
        {
            rp.TransactionStart(rp.m_csBusinessDate);
        }
#endif // _CPPUNIT
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
            SetAttendantModeStateInSM(false); //SSCOADK-544
            SetNextGenUIMode();

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
			TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmAssistMode") );
            co.SetAttendantID(csOperator);
            co.SetAttendantMode(true);
            SetAttendantModeStateInSM(true); //SSCOADK-544
            SetNextGenUIMode();

			rp.SetOperator(csOperator);
			rp.ModeChange(rp.m_lAttendantEntryID, ATTENDANT, ENTER);
            DMTakeawayBeltNotify(tabEventAssistModeStart);
        }
#ifndef _CPPUNIT
		rp.m_lAttendantEntryID += 1;
        if (!fSAInTransaction)
        {
            rp.TransactionEnd();
        }
        // ShowTrainingModeOnScreen will update training/attendant text 
        // and show/hide mode indicators. 
        ps.ShowTrainingModeOnScreen(TrainingModeObj.IsTMOn());
        showScreen(to.m_bTraceOn);
#endif // _CPPUNIT
    }
	trace(L6, _T("-PSButton2()"));
#ifndef _CPPUNIT
    //set lane closed flag to make SMAuthorization return to shopping
    m_bIsAttendantModeChanged = true; //TAR371918 & 371932
    //m_bIsFromLaneClosed = true; //TAR371918 & 371932 remove
#endif // _CPPUNIT
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
        dm.EnableCashDevices(false);
	}
	else //exit cards only mode
	{
		ExitDegradedMode();
		co.fTenderCashAllowed = true;
		//Tar 450827
		bool bReloadedCashAcceptor=false;
		bool bReloadedCoinAccptor = false;
		bool bReloadedChanger = false;
		long rc = 0;
		int nInError = 0;
	
		ps.ButtonEnable(_T("SMButton1"), false);
		ps.ButtonEnable(_T("SMButton2"), false);
		ps.ButtonEnable(_T("SMButton3"), false);
		ps.ButtonEnable(_T("SMButton8"), false);
		

#ifndef _CPPUNIT

        //+SSCOB-15416
		dm.CheckHealth( DMCLASS_CASHACCEPTOR,  0, false ); // level 2 check health
		long m_nStartDeviceErrorResultCodeExtended = dm.GetResultCodeExtended(DMCLASS_CASHACCEPTOR, 0, false );
		
		if(m_nStartDeviceErrorResultCodeExtended > 0)  //There is error
            nInError++;

		if ( nInError && co.IsDegradedModeOn() )
			dm.m_lDeviceClassThatWasInError.insert(DMCLASS_CASHACCEPTOR);
        //-SSCOB-15416

		//+SR742  
    
		 for (dm.m_lDeviceClassThatWasInErrorIterator = dm.m_lDeviceClassThatWasInError.begin(); 
		     dm.m_lDeviceClassThatWasInErrorIterator != dm.m_lDeviceClassThatWasInError.end(); 
			 dm.m_lDeviceClassThatWasInErrorIterator++)
		{
			switch( *(dm.m_lDeviceClassThatWasInErrorIterator) )
			{
			case DMCLASS_CASHCHANGER:
				{
					bReloadedChanger = true;
					dm.ReloadCashChanger();
					
				}break;
			case DMCLASS_CASHACCEPTOR:
				{
					bReloadedCashAcceptor = true;
					//+SSCOB-15416
					rc = dm.ReloadCashAcceptor();
					if(rc != 0)
					{
						long resultCodeEx = 0;
						resultCodeEx = dm.GetResultCodeExtended(DMCLASS_CASHACCEPTOR,0,true);

						mo.PostDM(DM_ERROR, DMCLASS_CASHACCEPTOR, 0, resultCodeEx, NULL);
						return STATE_NULL;
					}
					//-SSCOB-15416					
				}break;
			case DMCLASS_COINACCEPTOR:
				{
						bReloadedCoinAccptor=true;
						dm.ReloadCoinAcceptor();
					
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

		if (bReloadedCashAcceptor)
		{
			bReloadedCashAcceptor=false;
			SetDeviceEnabled(DMCLASS_CASHACCEPTOR, true, false);
		}
		else
			SetDeviceEnabled(DMCLASS_CASHACCEPTOR, true, true);

		if (bReloadedCoinAccptor)
		{
			bReloadedCoinAccptor = false;
			SetDeviceEnabled(DMCLASS_COINACCEPTOR, true, false);
		}
		else
			SetDeviceEnabled(DMCLASS_COINACCEPTOR, true, true);


		if (bReloadedChanger) 
		{
			bReloadedChanger = false;
			SetDeviceEnabled(DMCLASS_CASHCHANGER, true, false);
		}
		else
			SetDeviceEnabled(DMCLASS_CASHCHANGER, true, true);
#endif //CPPUNIT



		
	}

	RETURNSTATE(SmSelectModeOfOperation);
}

#ifndef _CPPUNIT
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

        //+SSCOADK-1252
        //If there is a track data, it needs to be deleted since we are in cash only mode.
        if ( cDMLastTrack )
        {
            delete [] cDMLastTrack; 
            cDMLastTrack = NULL;
            nDMLastTrackLen = 0;
            csDMEncryptedPin = EMPTY_STRING;
            waitingForCardRemoval = false; 
        } 
        //-SSCOADK-1252
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

        // Do not enable MSR here. It will be enabled based on each state.
        //DMMSREnable();
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


void SMSmSelectModeOfOperationBase::SetDeviceEnabled(long nDeviceClass, bool fEnabled, bool fCheckHealth)
{
	trace(L6,_T("+SMSmSelectModeOfOperationBase::SetDeviceEnabled()"));

	dm.Enable(nDeviceClass, 0, fEnabled);
	if (fCheckHealth) 
			dm.CheckHealth(nDeviceClass, 0);


	trace(L6,_T("-SMSmSelectModeOfOperationBase::SetDeviceEnabled()"));
	return;
}
#endif // _CPPUNIT