//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmSystemFunctionsBase.CPP
//
// TITLE: Class implementation for store mode System Functions state
//
// AUTHOR:  Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#ifndef _CPPUNIT
#include "Common.h"                 // MGV common includes

#include "SMSmAuthorization.h"
#include "SMSmSystemFunctions.h"
#include "SMSmmEditAdd.h"
#include "SMOutOfService.h"
#include "SMSmUtility.h"
#include "SMSmSelectModeOfOperation.h"	// SR742
#include "RemoteMessageAPI.h"		// External library ...\platform\RemoteMessage
#else
#include "SMSmSystemFunctionsBase.h"
#endif //_CPPUNIT

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmSystemFunctionsBase")

#ifndef _CPPUNIT
static int nEncryptorKey = 0;
extern CString csGLOBALAddThe128Prefix; // CM/NN 090502 TAR 214029
#endif //_CPPUNIT

IMPLEMENT_DYNCREATE(SMSmSystemFunctionsBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SmSystemFunctions)


DEFINE_TIMESTAMP

#ifndef _CPPUNIT
//////////////////////////////////////////
SMSmSystemFunctionsBase::SMSmSystemFunctionsBase()
: bButtonEnabledSet(false) // TAR 313180
{
  IMPLEMENT_TIMESTAMP
}

//////////////////////////////////////////////
// Initialize
//////////////////////////////////////////////
SMStateBase  *SMSmSystemFunctionsBase::Initialize(void)
{
  nStoreModeMenuState = BES_SMSYSTEMFUNCTIONS;
  printerUtils.ResetReceiptPrinter();
  if(!fInMultiSelectPickList)
  {
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmSystemFunctions") );	// RFC 330050
  }
  ra.RequestEnableAssistMode(false); 
  showScreen(to.m_bTraceOn);
  SAWLDBScannerDisable();
  return STATE_NULL;
}
#endif
/////////////////////////////////////////////////////////
// showScreen
/////////////////////////////////////////////////////////
void SMSmSystemFunctionsBase::showScreen(const bool traceOn)
{
    ps.SetFrame(_T("SmSystemFunctions"));

    ShowStoreModeMenuButtons();

    ps.Echo(PS_BLANK);
    ps.SetLeadthruText(LTT_SYSFUNCSELECT, PS_BLANK);  // TAR 228095 
	ps.ShowSMTotal(true);
    ps.ShowSMTBText(csTBMessage2Scot);	
    //ps.SMShowItemReceipt(true); //obsolete
    ps.ShowFrame();
}
#ifndef _CPPUNIT
/////////////////////////////////////////////
// Button 1 - Print operator barcode
/////////////////////////////////////////////
SMStateBase  *SMSmSystemFunctionsBase::PSButton1(void)
{
    return OnPrintBarcodeButton();
}

/////////////////////////////////////////////
// Button 2 Security Maintenance
/////////////////////////////////////////////
SMStateBase  *SMSmSystemFunctionsBase::PSButton2(void)
{
    return OnSecurityMaintButton();
}


/////////////////////////////////////////////
// Button 3 Training
/////////////////////////////////////////////
SMStateBase  *SMSmSystemFunctionsBase::PSButton3(void)
{
    return OnModeOfOperationButton();
}
#endif

/////////////////////////////////////////////
// Button 4 Load Options
/////////////////////////////////////////////
SMStateBase  *SMSmSystemFunctionsBase::PSButton4(void)
{
    SMStateBase *pRetState = OnLoadOptionsButton();
    //call showScreen again to refresh display
    showScreen(to.m_bTraceOn);
    return pRetState;
}
#ifndef _CPPUNIT
/////////////////////////////////////////////
// Button 5 Edit Pick List
/////////////////////////////////////////////
SMStateBase  *SMSmSystemFunctionsBase::PSButton5(void)
{
    return OnEditPickListButton();
}

/////////////////////////////////////////////
// Button 6 Utility
/////////////////////////////////////////////
SMStateBase  *SMSmSystemFunctionsBase::PSButton6(void)
{
    return OnUtilityButton();
}
#endif //_CPPUNIT

///////////////////////////////////////////
// Button 7 Attendant Mode button
///////////////////////////////////////////
SMStateBase  *SMSmSystemFunctionsBase::PSButton7(void) // 
{
	trace(L6, _T("+PSButton7():Attendant mode button pressed"));
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
			// +TAR 310488/287456 - need to issue this before going to assist mode
			if(!fSAInTransaction)
			{
				SASendSCOTInput( ITEMIZATION );
			}
			// -TAR 310488/287456
			TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmAssistMode") );
            co.SetAttendantID(csOperator);
            co.SetAttendantMode(true);
            SetAttendantModeStateInSM(true); //SSCOADK-544
            SetNextGenUIMode();

			rp.SetOperator(csOperator);
			rp.ModeChange(rp.m_lAttendantEntryID, ATTENDANT, ENTER);
            DMTakeawayBeltNotify(tabEventAssistModeStart);
        }
		rp.m_lAttendantEntryID += 1;
#ifndef _CPPUNIT
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
	trace(L6, _T("-PSButton7()"));
#ifndef _CPPUNIT
    //set lane closed flag to make SMAuthorization return to shopping
    m_bIsAttendantModeChanged = true; //TAR371918 & 371932
    //m_bIsFromLaneClosed = true; //TAR371918 & 371932 remove
#endif // _CPPUNIT
    RETURNSTATE(SmAuthorization)
}

#ifndef _CPPUNIT
/////////////////////////////////////////////
// Button 8 Go Back
/////////////////////////////////////////////
SMStateBase  *SMSmSystemFunctionsBase::PSButton8(void)
{
    return OnGoBackButton();
}

/////////////////////////////////////////////
// OnGoBackButton
//
// Purpose: Handle return state transition
// Parameters:
//   None
/////////////////////////////////////////////
SMStateBase  *SMSmSystemFunctionsBase::OnGoBackButton(void)
{
    RETURNSTATE(SmAuthorization)
}

//////////////////////////////////////////////
// CopyLogsToServer
//////////////////////////////////////////////
long SMSmSystemFunctionsBase::CopyLogsToServer(void)
{
  _TCHAR szFile[_MAX_PATH];
    
  trace(L0,_T("Log files to server requested"));
    
  ps.Prompt(COPY_TO_SERVER);


  // we must force a close of the trace file to copy it
  // if the trace file is open, force a close first
  CString csSourceFileName = co.csConfigFilePath +_T("\\TRACES.CPY");
  ps.Echo(to.m_strTraceFileName);
  to.CloseTraceFile();
  STATE(StoreModeMenu)::SCOTDeleteFile(csSourceFileName);   //TAR 348946 - make sure you have an old traces.log somehow
  CopyFile(to.m_strTraceFileName, csSourceFileName,false);
  _tcscpy(szFile,_T("TRACES.LOG"));
  CopyFileToServer(csSourceFileName,szFile);
  STATE(StoreModeMenu)::SCOTDeleteFile(csSourceFileName);   //TAR 348946
    
  // only copy the NT event logs on demand
  HANDLE eLog = NULL;
  // copy the system event log
  ps.Echo(_T("System Event Log"));
  eLog = OpenEventLog(_T(""),_T("System"));
  if (eLog)
  {
	csSourceFileName = co.csConfigFilePath +_T("\\NTSYSTEM.CPY");
	STATE(StoreModeMenu)::SCOTDeleteFile(csSourceFileName);   //TAR 348946 - make sure you have an old system event log somehow
    ClearEventLog(eLog, csSourceFileName);
    _tcscpy(szFile,_T("NTSYSTEM.LOG"));
    CopyFileToServer(csSourceFileName,szFile);
	STATE(StoreModeMenu)::SCOTDeleteFile(csSourceFileName);   //TAR 348946
  }
    
  // copy the application event log
  ps.Echo(_T("Application Event Log"));
  eLog = OpenEventLog(_T(""),_T("Application"));
  if (eLog)
  {
	csSourceFileName = co.csConfigFilePath +_T("\\NTAPPLIC.CPY");
	STATE(StoreModeMenu)::SCOTDeleteFile(csSourceFileName);   //TAR 348946 - make sure you have an old application event log somehow
    ClearEventLog(eLog, csSourceFileName);
    _tcscpy(szFile,_T("NTAPPLIC.LOG"));
    CopyFileToServer(csSourceFileName,szFile);
	STATE(StoreModeMenu)::SCOTDeleteFile(csSourceFileName);   //TAR 348946
  }
    
  // all done
  ps.Prompt(COPY_COMPLETE);
  ps.Echo(BTT_OK);
    
  return 0;
}

///////////////////////////////////////////////
bool SMSmSystemFunctionsBase::inEncryptorMode(void)
{
  if (nEncryptorKey!=10)
    return false;
    
//  ps.Prompt("Use ATM keys to enter Master Encryptor Key");  //Too long for store mode prompt
  return true;
}

///////////////////////////////////////////////
SMStateBase  *SMSmSystemFunctionsBase::PSNumericKey(const long lKey)
{
  // expect input from ATM keypad only as master key is keyed
  if (inEncryptorMode())
  {
    PSEchoInput(lKey);
  }
    
  return STATE_NULL;
}

///////////////////////////////////////////////
SMStateBase  *SMSmSystemFunctionsBase::DMEncryptorCompleted(void)// master key complete was Done *** MGG
{
  ps.Button(_T("SMButton1"), BTT_ENCRYPTOR, true);
  nEncryptorKey = 0;
  ps.Echo(BTT_OK);
    
  ps.AllowInput(0,false,false);
    
  return STATE_NULL;
}
////////////////////////////////////////
// NewUI
SMStateBase  *SMSmSystemFunctionsBase::PSReceiptUp(void)
{
  ps.ScrollUp(_T("SMReceipt"));
  return STATE_NULL;

}
SMStateBase  *SMSmSystemFunctionsBase::PSReceiptDown(void)
{
  ps.ScrollDown(_T("SMReceipt"));
  return STATE_NULL;

}
// NewUI

// Function added for TAR 313180
void SMSmSystemFunctionsBase::DisableButtons()
{
   CString csCtxt = _T("SmSystemFunctions");
   COleVariant vType( _T("") );
   CString csTemp = EMPTY_STRING;
   if (!bButtonEnabledSet)
   {
      for (int i = 1; i <= 8; i++)
      {
         bButtonEnabled[i-1] = false;
         csTemp.Format(_T("SMButton%d"), i);
         if (PSX_SUCCESS == m_pAppPSX->GetConfigProperty(csTemp, csCtxt, UI::PROPERTYSTATE, vType))
         {
            if (vType.lVal == UI::Normal)
            {
               bButtonEnabled[i-1] = true;
               vType = (long) UI::Disabled;
               m_pAppPSX->SetConfigProperty(csTemp, csCtxt, UI::PROPERTYSTATE, vType);
            }
         }
      }
      bButtonEnabledSet = true;
   }

}

// Function added for TAR 313180
void SMSmSystemFunctionsBase::ReenableButtons()
{
   if (!bButtonEnabledSet)
   {
      return;
   }
   CString csCtxt = _T("SmSystemFunctions");
   COleVariant vType = (long) UI::Normal;
   CString csTemp = EMPTY_STRING;
   for (int i = 1; i <= 8; i++)
   {
      if (bButtonEnabled[i-1])
      {
         csTemp.Format(_T("SMButton%d"), i);
         m_pAppPSX->SetConfigProperty(csTemp, csCtxt, UI::PROPERTYSTATE, vType);
      }
   }
   bButtonEnabledSet = false;
}
// + RFC 330050
void SMSmSystemFunctionsBase::UnInitialize(void)
{
    if(!fInMultiSelectPickList)
    {
	    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmSystemFunctions") );	
    }
}
// - RFC 330050

/////////////////////////////////////
// ShowPrintBarcodeButton 
// 
// Purpose: Display the command button - CmdBtn_PrintBarcode 
//          or the fixed button - SMButon1
// Parameters:
//   None
/////////////////////////////////////
void SMSmSystemFunctionsBase::ShowPrintBarcodeButton()
{
    STATE(StoreModeMenu)::ShowPrintBarcodeButton();
    //Override the display of the command button - CmdBtn_PrintBarcode 
    //to still be able to use the fixed button - SMButon1
    if (co.IsInAssistMenus())
    {
        ps.ButtonState(_T("SMButton1"), false, false);
        return;
    }
    //FZ20020730 and NN 080402 Disable the print barcode button if the mask contain anything other than 'u' or 'p' for the ID and PW
    if(co.fOperationsTBControlsPrinter || !nOperatorStoreLoginBarcodeUseridLength || !nOperatorStoreLoginBarcodePasswordLength)  
        ps.Button(_T("SMButton1"),BTT_BARCODE,false);
    else
        ps.SetButtonText(_T("SMButton1"), BTT_BARCODE);
}

/////////////////////////////////////
// ShowSecurityMaintButton 
// 
// Purpose: Display the command button - CmdBtn_SecurityMaint 
//          or the fixed button - SMButon2
// Parameters:
//   None
/////////////////////////////////////
void SMSmSystemFunctionsBase::ShowSecurityMaintButton()
{
    STATE(StoreModeMenu)::ShowSecurityMaintButton();
    //Override the display of the command button - CmdBtn_SecurityMaint 
    //to still be able to use the fixed button - SMButon2
    if (co.IsInAssistMenus())
    {
        ps.ButtonState(_T("SMButton2"), false, false);
        return;
    }
    if (co.IsAttendantModeOn())
    {
        ps.Button(_T("SMButton2"), BTT_SECURITYMAINTENANCE, false);
    }
    else
    {
        ps.Button(_T("SMButton2"), BTT_SECURITYMAINTENANCE, true);
    }
}

/////////////////////////////////////
// ShowModeOfOperationButton 
// 
// Purpose: Display the command button - CmdBtn_ModeOfOperation 
//          or the fixed button - SMButton3
// Parameters:
//   None
/////////////////////////////////////
void SMSmSystemFunctionsBase::ShowModeOfOperationButton()
{
    STATE(StoreModeMenu)::ShowModeOfOperationButton();
    //Override the display of the command button - CmdBtn_ModeOfOperation 
    //to still be able to use the fixed button - SMButton3
    if (co.IsInAssistMenus())
    {
        ps.ButtonState(_T("SMButton3"), false, false);
        return;
    }
    ps.SMButton(_T("SMButton3"),MSG_MODE_OPERATION,true); 
}

/////////////////////////////////////
// ShowLoadOptionsButton 
// 
// Purpose: Display the command button - CmdBtn_LoadOptions 
//          or the fixed button - SMButton4
// Parameters:
//   None
/////////////////////////////////////
void SMSmSystemFunctionsBase::ShowLoadOptionsButton()
{
    STATE(StoreModeMenu)::ShowLoadOptionsButton();
    //Override the display of the command button - CmdBtn_LoadOptions 
    //to still be able to use the fixed button - SMButton4
    if (co.IsInAssistMenus())
    {
        ps.ButtonState(_T("SMButton4"), false, false);
        return;
    }
    //during transaction or OPTION RELOAD was requested but pending
    if (fInSignedOn || fInSignedOff || fDoOptions)
    {
        ps.SMButton(_T("SMButton4"),BTT_LOADOPTS,false); 
        trace(L6, _T("SMSmSystemFunctionsBase::ShowLoadOptionsButton() Dont display button 4 On = <%d> Off = <%d> Opt = <%d>"), fInSignedOn, fInSignedOff, fDoOptions);
        
    }
    else
    {
        ps.SMButton(_T("SMButton4"),BTT_LOADOPTS,true); 
    }
}

/////////////////////////////////////
// ShowUtilityButton 
// 
// Purpose: Display the command button - CmdBtn_Utility 
//          or the fixed button - SMButton6
// Parameters:
//   None
/////////////////////////////////////
void SMSmSystemFunctionsBase::ShowUtilityButton()
{
    STATE(StoreModeMenu)::ShowUtilityButton();
    //Override the display of the command button - CmdBtn_Utility 
    //to still be able to use the fixed button - SMButton6
    if (co.IsInAssistMenus())
    {
        ps.ButtonState(_T("SMButton6"), false, false);
        return;
    }
    bool bShow = co.fUtilityExitButtonVisible;
    if (bShow)
    {
		 ps.SMButton(_T("SMButton6"),MSG_UTILITY,true); 
    }

    ps.ButtonState(_T("SMButton6"), bShow, bShow);
}
#endif // _CPPUNIT