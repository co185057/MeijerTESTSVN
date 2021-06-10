//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStoreModeMenuBase.cpp
//
// TITLE: Store mode menu functionality
// NOTES: The StoreModeMenu state is intended to be used as the parent state
//        for store mode menu states such as SMSystemFunctions and SMAuthorization.
//        
//
// AUTHOR:  Barbara Dawkins
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first

#include "Common.h"
#include "SMStoreModeMenu.h"

//USSF START
#include "SMUSSFManager.h"
#include "USSFCommon.h"
//USSF END
#include "SMInProgress.h"
#include "SMLaneClosed.h"
#include "SMSmRestrictedItems.h"
#include "SMSmVoidedItems.h"
#include "SMSmCouponItems.h"
#include "SMSmVisualItems.h"
#include "SMSmConfirmSuspendedTransaction.h"
#include "SMSmAuthorization.h"
#include "SMSmmEditAdd.h"
#include "SMSmAssistMode.h"
#include "SMSmAssistMenu.h"
#include "SMSmReportsMenu.h"
#include "SMSmMediaStatus.h"
#include "SMSmSystemFunctions.h"
#include "SMSmSelectModeOfOperation.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("StoreModeMenuBase")


extern CString csGLOBALAddThe128Prefix; // CM/NN 090502 TAR 214029
BEST SMStoreModeMenuBase::nStoreModeMenuState = BES_STATERETURN;  // Current store mode menu state


IMPLEMENT_DYNCREATE(SMStoreModeMenuBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(StoreModeMenu)


DEFINE_TIMESTAMP

////////////////////////////////////
SMStoreModeMenuBase::SMStoreModeMenuBase()
{
  IMPLEMENT_TIMESTAMP
}

///////////////////////////////////////////
//  OnCloseLaneButton
// 
// Purpose: Handle "Close Lane" button press, execute TBLaneClosed function
//          and handle return state transition.
// Parameters: None
///////////////////////////////////////////
SMStateBase  *SMStoreModeMenuBase::OnCloseLaneButton(void)
{
	SynchronizeStoreloginWithLaunchPad(_T("operation=set-signed-off")); // TAR 301706

	//+RFC 986
	if (co.fStateStartupWaitForTB)
    {
        setAnchorState(BES_START);
        TBLaneClosed(true);
		CREATE_AND_DISPENSE(InProgress)(storeMode(), helpMode(), co.getTimeOutItemTransactionResult());
    }
    else
    { //-RFC 986
        RETURNSTATE(LaneClosed)
    }
}

///////////////////////////////////////////
// OnSuspendButton
// 
// Purpose: Handle "Suspend" button press, execute TBSuspendTransaction function
//          and handle return state transition.
// Parameters: None
///////////////////////////////////////////
SMStateBase  *SMStoreModeMenuBase::OnSuspendButton(void) //
{
	//TAR 364905 Begin
	if (co.fOperationsForceDelayedApprovalBeforeSuspend)
	{
		bForceApprovalBeforeSuspend = true;
		if (isRestrictedItemApprovalRequired())
          RETURNSTATE(SmRestrictedItems)

		if (isCouponRatioApprovalRequired())
          RETURNSTATE(SmCouponItems)

		if (isVisualItemApprovalRequired())
          RETURNSTATE(SmVisualItems)
    }//TAR 364905 End

    if(co.fStateConfirmationForSuspendingTransaction)
    {
        // TAR 196159 If you suspend twice in a row, you're stuck is store mode
        // This change prevents the original sellstate from being overwritten
        // on the second attempt
        if (!fCancelSuspend)
        {
            nSellState = getAnchorState();
            setAnchorState(BES_STOREAUTHORIZATION);
        }
        RETURNSTATE(SmConfirmSuspendedTransaction)
    }
    else
    {
        bSuspendFailure = false; //Tar 126382
		SynchronizeStoreloginWithLaunchPad(_T("operation=set-signed-off")); // TAR 301706

        //Before suspend transaction, need void coupons and  paid cash
        if( lCoupons && co.fOperationsVoidCouponBeforeSuspend)
            VoidAllCoupons(); // Void all coupons before suspend transaction
	// TAR 236567 Let TB handle refund cash
	//if(nDMCashInDrawer)   // void already paid cash
            //TBEnterTender(TB_TENDER_CASH, -1*nDMCashInDrawer);
	// End TAR 236567

        //Tar 120428
        //  DMSayPhrase("TransactionSuspended");
        return TBSuspendTransaction();
    }
}

/////////////////////////////////////////////
// OnPrintBarcodeButton
// 
// Purpose: Handle "Print Operator Barcode" button press, execute DMReceiptPrintBarcode function
// Parameters: None
/////////////////////////////////////////////
SMStateBase  *SMStoreModeMenuBase::OnPrintBarcodeButton(void)
{
    if (ps.RemoteMode())
    {
        return STATE_NULL;
    }
//fiscal printer start
	if(co.fOperationsTBControlsPrinter)
		return STATE_NULL;
//fiscal printer end
	// Even though ID and password is valid
	// Barcode can only have 9 digits for ID and Password
	// NN 080202 RFC 187993 Also, check if the barcode mask match with the actual operator id and password 
    CString csTemp;  //FZ20020730
    
    //RFC 240738 allow option for code 128 barcodes to contain leading zeros
    bool fAllowLeadingZeros = ((co.nOperationsOperatorLoginBarcodeType == 2)  && co.fOperationsAllowLeadingZerosInCode128LoginBarCode); 
    
	if ( csOperator.GetLength() > nOperatorStoreLoginBarcodeUseridLength )

	{
        csTemp.Format(ps.GetPSText(MSG_ERR_BARCODE_ID_LENGTH), nOperatorStoreLoginBarcodeUseridLength);
        ps.SetLeadthruText(MSG_ERR_NO_BARCODE, csTemp);
		return STATE_NULL;
	}
        else if ( csPassword.GetLength() > nOperatorStoreLoginBarcodePasswordLength )

    {
        csTemp.Format(ps.GetPSText(MSG_ERR_BARCODE_PSWD_LENGTH), nOperatorStoreLoginBarcodePasswordLength);
        ps.SetLeadthruText(MSG_ERR_NO_BARCODE, csTemp);
		return STATE_NULL;
    }

	if(!fAllowLeadingZeros)//RFC 240738
    {
        //Tar 121045
        if((csOperator.GetLength() > 1 && csOperator.Left(1) == _T("0")) 
            || (csPassword.GetLength() > 1 && csPassword.Left(1) == _T("0"))) 
        // Could not print barcode if it is zero leading OP or zero leading Password
        {
            ps.SetLeadthruText(MSG_ERR_NO_BARCODE, MSG_ZERO_LEADING_OP_PASSWORD);
            return STATE_NULL;
        }
    }
    CPSXUserInputLock userInputLock(_T("SMStoreModeMenuBase::OnPrintBarcodeButton"));
	// Scanned manager ID has the following format
	//   F411ooooppppmx
	// where 
	//   F	-- Scanner added indicating type of barcode
	//	412	-- new SCOT unique indicator for manager ids (used to be 411, changed to accomodate new pswd encoding algorithm)
	//   o	-- operator number
	//   p	-- encoded password  XOR'ed with decimal 3456
	//   m	-- mod 10 check digit calculated 
	//   x	-- actual JAN 13 check digit -- not sure of algorithm

	CString csBarcode = csOperatorStoreLoginBarcodeSignature;
	CString csOp = _T("");

	for (int i=0; i< ( nOperatorStoreLoginBarcodeUseridLength - csOperator.GetLength()); i++)
	{
        csOp += !fAllowLeadingZeros ? _T('0') : _T(' ');//RFC 240738
	}
	csOp += csOperator;
	CString csPass = _T("");
	for ( i=0; i< ( nOperatorStoreLoginBarcodePasswordLength - csPassword.GetLength()); i++)
	{
		csPass += !fAllowLeadingZeros ? _T('0') : _T(' ');//RFC 240738
	}
	csPass += csPassword;


    int nDigit;
    for ( i=0; i < nOperatorStoreLoginBarcodePasswordLength; i++)

    {
		//Encode the alphanumberic password 
		if(csPass[i]<_T('0'))     //Ascii from 0-47
		{
			nDigit = (csPass[i]-_T(' ')) + (i+3);
			nDigit = nDigit % 48;
			csPass.SetAt(i, nDigit + _T(' '));
		}
		else if(csPass[i]>_T('9'))  //Ascii from 58-255
		{
			nDigit = (csPass[i]-_T(':')) + (i+3);
			nDigit = nDigit % 198;
			csPass.SetAt(i, nDigit + _T(':'));
		}
		else // 0-9  Ascii from 48-57
		{
            nDigit = (csPass[i]-_T('0')) + (i+3);
            nDigit = nDigit % 10;
            csPass.SetAt(i, nDigit + _T('0'));
		}
    }
    
	csBarcode += csOp;
	csBarcode += csPass;
	if(co.nOperationsOperatorLoginBarcodeType == 1)
	{

	long lCheckDigit = 0;
	
		for (int index = 0; index < csBarcode.GetLength(); index++)

		{
			lCheckDigit += (csBarcode[index] - _T('0'));
		}
		lCheckDigit = ((10 - (lCheckDigit % 10)) % 10) + _T('0');
		csBarcode += (TCHAR)lCheckDigit;
    	DMReceiptPrintBarcode(csBarcode, DM_BCS_JAN13);
	}
	else if(co.nOperationsOperatorLoginBarcodeType == 2)
	{
		// -----------------------------------------------------------------
		// CM/NN090502 TAR 214029 - start
		// known problem on K590 workaround 090502, where it does not work
		// with a {A , {B, nor a {C.  Mike Barlow says just to print it
		// without those prefixes.  This optional registry entry will allow
		// us to NOT add that prefix.  Note: that the default prefix
		// will be {C (if the reg entry does NOT exist).
		// -----------------------------------------------------------------

		if (csGLOBALAddThe128Prefix.GetLength() != 0)  // see if prefix exist
		{
			// add start characters for code 128 type barcode.  
			// note: "{A" includes all numbers, characters and special characters 
			DMReceiptPrintBarcode(csGLOBALAddThe128Prefix+csBarcode, DM_BCS_CODE128);
		}
		else
		{
			// allows workaround of current K590 limitation (see above) 090502
			DMReceiptPrintBarcode(csBarcode, DM_BCS_CODE128);
		}
		//"{A" indicates the start character for code 128 type barcode.  We decided 
		//to use "{A" since this includes all numbers, characters and special characters 
		//	DMReceiptPrintBarcode("{A"+csBarcode, DM_BCS_CODE128);
	}
	else
	{
		trace(L2, _T("SMSmsystemFunctionsBase::PSButton2() Wrong Barcode Type  <%d>"), co.nOperationsOperatorLoginBarcodeType);
	}

	// Add printer codes 
	// CENTER
	// BOLD
	// DOUBLE HIGH and DOUBLE WIDE
	//
	// codes retrieved from 
	// DOC:				OLE for Retail POS Application Guide Rel. 1.2
	// Chap:			POS Printer
	// Section:		Data Characters and Escape Sequences
	CString csOpText;
	if (co.fOperationsOperatorTwoLineText)
		csOpText = _T("\n\x1B|cA\x1B|bC\x1B|4COPERATOR\n");  //Tar 330860
	else
		csOpText = _T("\n\x1B|cA\x1B|bC\x1B|4COPERATOR ");  

	//csOpText += csOp.Right(4);
	csOpText += csOp;

	// reset back to normal
	csOpText += _T("\x1B|N\n");
	DMReceiptLine(csOpText);
	DMReceiptCut();

    return STATE_NULL;
}

/////////////////////////////////////////////
// OnModeOfOperationButton
// 
// Purpose: Handle "Mode of Operation" button press and return state transition
// Parameters: None
/////////////////////////////////////////////
SMStateBase  *SMStoreModeMenuBase::OnModeOfOperationButton(void)
{
    if (ps.RemoteMode())
    {
        return STATE_NULL;
    }
    RETURNSTATE(SmSelectModeOfOperation)
}

/////////////////////////////////////////////
// OnLoadOptionsButton
// 
// Purpose: Handle "Load Options" button press, execute ReloadOptions function
// Parameters: None
/////////////////////////////////////////////
SMStateBase  *SMStoreModeMenuBase::OnLoadOptionsButton(void)
{
    if (ps.RemoteMode())
    {
        return STATE_NULL;
    }

    if (fInSignedOn || fInSignedOff || fSAInTransaction)
    {
        // You are in the middle of a transaction, initialize all at the end of it
        trace(L0,_T("Options load requested"));
        fDoOptions = true;
        ps.Prompt(OPTIONS_LOAD);
        ps.Echo(BTT_OK);
        return STATE_NULL;
    }
    else
    {
        // outside of a transaction, no active transaction in progress
        // do initialize of all right now
        // fDoOptions = false;
        // ps.Prompt(OPTIONS_LOAD);
        // CREATE_AND_DISPENSE(OutOfService)(2);
        CPSXUserInputLock userInputLock(_T("SMStoreModeMenuBase::OnLoadOptionsButton"));  
		m_bReloadingOptions = true;  //tar 352145
        SMStateBase* pStartState = ReloadOptions();
		m_bReloadingOptions = false; 
        if (pStartState)
        {
            return pStartState;
        }
        else
        {
            ASSERT(0);
            return STATE_RESTART;
        }
    }
}

/////////////////////////////////////////////
// OnEditPickListButton
// 
// Purpose: Handle "Edit Pick List" button press, load Picklist Editor tool
// Parameters: None
/////////////////////////////////////////////
SMStateBase  *SMStoreModeMenuBase::OnEditPickListButton(void)
{
    if (ps.RemoteMode())
    {
        return STATE_NULL;
    }
    // Prepare FastLane UI for a window to be on top of it //
    COleVariant v;
    HWND hAppWnd = NULL;
    BOOL bAppWasTopmost = TRUE;
    if( PSX_SUCCESS == m_pAppPSX->GetControlProperty( UI::CONTROLDISPLAY, UI::PROPERTYHWND, v ) )
    {
        hAppWnd = (HWND)v.lVal;
        bAppWasTopmost = ( WS_EX_TOPMOST == ( GetWindowLong( hAppWnd, GWL_EXSTYLE ) & WS_EX_TOPMOST ) );
        if( bAppWasTopmost )
        {
            SetWindowPos( hAppWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
        }
    }
    else
    {
        trace( L2, _T("SMStoreModeMenuBase::OnEditPickListButton() Failed to get FastLane UI HWND!") );
        return STATE_NULL;
    }
	CPSXUserInputLock userInputLock(_T("SMStoreModeMenuBase::OnEditPickListButton"));  //TAR 371980 fix
        
    // Launch PLE process //
    STARTUPINFO si = { 0 };
    si.cb = sizeof( si );
	PROCESS_INFORMATION pi = { 0 };
    	
	TCHAR szPath[MAX_PATH] = _T("");
	_tmakepath( szPath, NULL, _T("%APP_DRIVE%\\scot\\bin"), PICKLISTEDITOR, _T("EXE") );
	GET_PARTITION_PATH(szPath, szPath);

	_tcscat( szPath, _T(" ") );
	_tcscat( szPath, rp.GetServerSharePoint() );
	_tcscat( szPath, XML_PICKLISTFILE );
	if( CreateProcess( NULL, szPath, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi ) )
	{
		CPSXUserInputLock userInputLock(_T("SMStoreModeMenuBase::OnEditPickListButton"));  
		CloseHandle( pi.hThread );	
		WaitForSingleObject( pi.hProcess, INFINITE );
		CloseHandle( pi.hProcess );        
	}
	else
	{
		trace( L2, _T("SMStoreModeMenuBase::OnEditPickListButton() Failed to CreateProcess( %s )"), szPath );
	}

    
    if( bAppWasTopmost )
    {
        SetWindowPos( hAppWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
    }
	
	return STATE_NULL;
}

/////////////////////////////////////////////
// OnUtilityButton
// 
// Purpose: Handle "Utility" button press, launch LaunchPad application.
// Parameters: None
/////////////////////////////////////////////
SMStateBase  *SMStoreModeMenuBase::OnUtilityButton(void)
{
    if (ps.RemoteMode())
    {
        return STATE_NULL;
    }
	SynchronizeStoreloginWithLaunchPad(_T("operation=launchpad-activate")); // TAR 301706
	return STATE_NULL;
}


///////////////////////////////////////////
// OnAssistModeButton
// 
// Purpose: Handle "Assist Mode" button press and return state transition.
// Parameters: None
///////////////////////////////////////////
SMStateBase  *SMStoreModeMenuBase::OnAssistModeButton(void)
{
    g_csAssistModeStateName.Empty(); // TAR 244640
	if (co.fStateAssistMenuFromStoreModeAllowed)
	{
		RETURNSTATE(SmAssistMenu)
	}
	else
	{
		RETURNSTATE(SmAssistMode)
	}
}

/////////////////////////////////////////////
// OnReportingButton
// 
// Purpose: Handle "Reporting" button press and return state transition.
// Parameters: None
/////////////////////////////////////////////
SMStateBase  *SMStoreModeMenuBase::OnReportingButton(void)
{
    RETURNSTATE(SmReportsMenu);
}

/////////////////////////////////////////////
// OnMediaStatusButton
// 
// Purpose: Handle "Mediat Status" button press and return state transition.
// Parameters: None
/////////////////////////////////////////////
SMStateBase  *SMStoreModeMenuBase::OnMediaStatusButton(void)
{
    if (ps.RemoteMode())
    {
        return STATE_NULL;
    }
    RETURNSTATE(SmMediaStatus);
}

/////////////////////////////////////////////
// OnSystemFunctionsButton
// 
// Purpose: Handle "System Functions" button press and return state transition.
// Parameters: None
/////////////////////////////////////////////
SMStateBase  *SMStoreModeMenuBase::OnSystemFunctionsButton(void)
{
    if (ps.RemoteMode())
    {
        return STATE_NULL;
    }
    RETURNSTATE(SmSystemFunctions);
}


/////////////////////////////////////////////
// OnGoBackButton
//
// Purpose: Does nothing, overrides must handle return state transition 
// Parameters: None
/////////////////////////////////////////////
SMStateBase  *SMStoreModeMenuBase::OnGoBackButton(void)
{
    return STATE_NULL;
}

///////////////////////////////////////
// OnSecurityMaintButton
// 
// Purpose: Handle "Security Maintenance" button press. System will
//          enter security maintenance mode.  
// Parameters: None
///////////////////////////////////////
SMStateBase  *SMStoreModeMenuBase::OnSecurityMaintButton(void)
{
    if (ps.RemoteMode())
    {
        return STATE_NULL;
    }
    SASendSCOTInput( MAINTENANCE_MODE );
    bSMMPreScanState = true;
	bSMMFailure = false;				// 228239 - have to reset both failure flags LPM021803 
    bSMMFailureActionCancel = false;	// this wasn't being reset until the app was restarted LPM102102 3.1 StdDev

	// Tell the security sub-system that we are in Security Maintenance mode
	CKeyValueParmList parmList;
	CKeyValue<const _TCHAR*>::Put(parmList, KEY_SMM_OPERATION, KEY_SMM_OPERATION_BEGIN);
	CKeyValue<const _TCHAR*>::Put(parmList, KEY_SMM_TOLERANCE_TYPE, KEY_SMM_TOLERANCE_TYPE_GLOBAL);
	_bstr_t bstrParmList((const _TCHAR *)parmList);
	long lrc = SecMgr.SecurityMaintenance(lSecMgrCookie, bstrParmList);
    bOkToDisplayBagScaleInfo = false;
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmAuthorization") );
    RETURNSTATE(SmmEditAdd)
}

///////////////////////////////////////
// ShowStoreModeMenuButtons
// 
// Purpose: To call functions to enable or disable buttons based on
//          configuration options and current system state.
// Parameters: None
///////////////////////////////////////
void SMStoreModeMenuBase::ShowStoreModeMenuButtons()
{
    ShowPrintBarcodeButton();
    ShowSecurityMaintButton();
    ShowModeOfOperationButton();
    ShowLoadOptionsButton();
    ShowUtilityButton();
    ShowCloseLaneButton();
    ShowSuspendButton();
    ShowReportingButton();
    ShowSystemFunctionsButton();
    ShowMediaStatusButton();
    ShowAssistModeButton();
    ShowGoBackButton();
}

///////////////////////////////////////
// ShowPrintBarcodeButton
// 
// Purpose: Enable "Print Operator Barcode" button Fastlane controls printer and
//          if there is a valid user mask configured,
// Parameters: None
///////////////////////////////////////
void SMStoreModeMenuBase::ShowPrintBarcodeButton()
{
    if (co.IsInAssistMenus())
    {
        ps.ButtonState(_T("CmdBtn_PrintBarcode"), false, false);
        return;
    }
    if(co.fOperationsTBControlsPrinter || !nOperatorStoreLoginBarcodeUseridLength || !nOperatorStoreLoginBarcodePasswordLength)  
        ps.Button(_T("CmdBtn_PrintBarcode"),BTT_BARCODE,false);
    else
        ps.SetButtonText(_T("CmdBtn_PrintBarcode"), BTT_BARCODE);
}

///////////////////////////////////////
// ShowSecurityMaintButton
// 
// Purpose: Enable "Security Maintenance" button if not in attendant mode.
// Parameters: None
///////////////////////////////////////
void SMStoreModeMenuBase::ShowSecurityMaintButton()
{
    if (co.IsInAssistMenus())
    {
        ps.ButtonState(_T("CmdBtn_SecurityMaint"), false, false);
        return;
    }
    if (co.IsAttendantModeOn())
    {
        ps.Button(_T("CmdBtn_SecurityMaint"), BTT_SECURITYMAINTENANCE, false);
    }
    else
    {
        ps.Button(_T("CmdBtn_SecurityMaint"), BTT_SECURITYMAINTENANCE, true);
    }
}

///////////////////////////////////////
// ShowModeOfOperationButton
// 
// Purpose: Enable "Mode Of Operation" button
// Parameters: None
///////////////////////////////////////
void SMStoreModeMenuBase::ShowModeOfOperationButton()
{
    if (co.IsInAssistMenus())
    {
        ps.ButtonState(_T("CmdBtn_ModeOfOperation"), false, false);
        return;
    }
    
    ps.SMButton(_T("CmdBtn_ModeOfOperation"),MSG_MODE_OPERATION,true);	//SR742
}

///////////////////////////////////////
// ShowLoadOptionsButton
// 
// Purpose: Enable "Load Options" button if outside of a transaction and
//          there is no LoadOptions request pending.
// Parameters: None
///////////////////////////////////////
void SMStoreModeMenuBase::ShowLoadOptionsButton()
{
    if (co.IsInAssistMenus())
    {
        ps.ButtonState(_T("CmdBtn_LoadOptions"), false, false);
        return;
    }
    //during transaction or OPTION RELOAD was requested but pending
    if (fInSignedOn || fInSignedOff || fDoOptions)
    {
        ps.SMButton(_T("CmdBtn_LoadOptions"),BTT_LOADOPTS,false); 
        trace(L6, _T("SMStoreModeMenuBase::ShowLoadOptionsButton() Dont display CmdBtn_LoadOptions On = <%d> Off = <%d> Opt = <%d>"), fInSignedOn, fInSignedOff, fDoOptions);
        
    }
    else
    {
        ps.SMButton(_T("CmdBtn_LoadOptions"),BTT_LOADOPTS,true); 
    }
}

///////////////////////////////////////
// ShowUtilityButton
// 
// Purpose: Show "Utility" button
// Parameters: None
///////////////////////////////////////
void SMStoreModeMenuBase::ShowUtilityButton()
{
    if (co.IsInAssistMenus())
    {
        ps.ButtonState(_T("CmdBtn_Utility"), false, false);
        return;
    }

    bool bShow = co.fUtilityExitButtonVisible;

    if (bShow)
    {
		 ps.SMButton(_T("CmdBtn_Utility"),MSG_UTILITY,true); 
    }
    
    ps.ButtonState(_T("CmdBtn_Utility"), bShow, bShow);
}

///////////////////////////////////////
// ShowCloseLaneButton
// 
// Purpose: Show "Close Lane" button
// Parameters: None
///////////////////////////////////////
void SMStoreModeMenuBase::ShowCloseLaneButton()
{
    ps.Button(_T("CmdBtn_CloseLane"), BTT_CLOSELANE, (ps.SalesReceiptEmpty() && !bIsTransactionStarted));
}

///////////////////////////////////////
// ShowSuspendButton
// 
// Purpose: Show "Suspend" button if allowed by Scotopts configuration
// Parameters: None
///////////////////////////////////////
void SMStoreModeMenuBase::ShowSuspendButton()
{
    if (co.IsInAssistMenus())
    {
        ps.ButtonState(_T("Cmdbtn_Suspend"), false, false);
        return;
    }

    ps.SetButtonText(_T("CmdBtn_Suspend"), co.fOperationsSuspend? BTT_SUSPEND : PS_NOTEXT); // if suspend is not configured, do not display button
	// +TAR 299534 - added tracing to see why form attract suspend button was enabled
	bool bStatus = isAnySoldVoidedItems();
    trace(L6, _T("SMStoreModeMenuBase::ShowSuspendButton isAnySoldVoidedItems =%d fInSignedOn= %d lTotalPaid= %d nDMCashInDrawer = %d"), bStatus, fInSignedOn, lTotalPaid, nDMCashInDrawer);
	// - TAR 299534
    //+RFC267820
    if (    (!isAnySoldVoidedItems() && co.fOperationsSuspend) //Nothing Sold
         || !fInSignedOn //TB not in tranx
         || ( (short(lTotalPaid) != nDMCashInDrawer) && !co.fOperationsAllowSuspendIfPartiallyTendered) )  //Tranx partially tendered, but config option not set to allow suspend if tranx is partially tendered
    {
        ps.Button(_T("CmdBtn_Suspend"),BTT_SUSPEND,false); // disable suspend button
    }//-RFC267820
    // RFC 263291.  Ask the TB if the Suspend button is allowed, only if the app already has it enabled
    else if (!TBIsAllowed(TB_FT_SUSPEND))
    {
        ps.Button(_T("CmdBtn_Suspend"),BTT_SUSPEND,false);			// disable suspend button
    }

}

///////////////////////////////////////
// ShowReportingButton
// 
// Purpose: Show "Reporting" button
// Parameters: None
///////////////////////////////////////
void SMStoreModeMenuBase::ShowReportingButton()
{
    if (co.IsInAssistMenus())
    {
        ps.ButtonState(_T("CmdBtn_Reporting"), false, false);
        return;
    }
    ps.SetButtonText(_T("CmdBtn_Reporting"), BTT_REPORTING); //Reports Menu
	//403469: either AssistMode or AssistMenuFromStoreModeAllowed is configured
    if ( ( co.fStateAssistMode || co.fStateAssistMenuFromStoreModeAllowed ) && fInSignedOn && co.fOperationsTBControlsPrinter)
    //if (co.fStateAssistMode && fInSignedOn && co.fOperationsTBControlsPrinter)
        ps.Button(_T("CmdBtn_Reporting"), BTT_REPORTING, false); 
}

///////////////////////////////////////
// ShowSystemFunctionsButton
// 
// Purpose: Show "System Functions" button
// Parameters: None
///////////////////////////////////////
void SMStoreModeMenuBase::ShowSystemFunctionsButton()
{
    if (!co.IsInAssistMenus())
    {   
        ps.Button(_T("CmdBtn_SystemFunctions"), BTT_SYSTEMFUNCTIONS, !ps.RemoteMode());
    }
    else
    {
        ps.Button(_T("CmdBtn_SystemFunctions"), PS_NOTEXT, false); //System Functions Screen 
    }
}

///////////////////////////////////////
// ShowMediaStatusButton
// 
// Purpose: Show "Media Status" button
// Parameters: None
///////////////////////////////////////
void SMStoreModeMenuBase::ShowMediaStatusButton()
{
    if (co.IsInAssistMenus())
    {
        ps.ButtonState(_T("Cmdbtn_MediaStatus"), false, false);
        return;
    }

    ps.Button(_T("CmdBtn_MediaStatus"), BTT_LANESTATUS, !ps.RemoteMode()); //Media Status Screen
}

///////////////////////////////////////
// ShowAssistModeButton
// 
// Purpose: Show "Assist Mode" button
// Parameters: None
///////////////////////////////////////
void SMStoreModeMenuBase::ShowAssistModeButton()
{
    if (co.IsInAssistMenus())
    {
        ps.ButtonState(_T("CmdBtn_AssistMode"), false, false);
        return;
    }
    ps.Button(_T("CmdBtn_AssistMode"), BTT_ASSISTMODE, ( co.fStateAssistMode || co.fStateAssistMenuFromStoreModeAllowed ) && fInSignedOn && !ps.RemoteMode());
}

///////////////////////////////////////
// ShowGoBackButton
// 
// Purpose: Show "GoBack" button
// Parameters: None
///////////////////////////////////////
void SMStoreModeMenuBase::ShowGoBackButton()
{
    if (co.IsInAssistMenus())
    {
        ps.ButtonState(_T("CmdBtn_GoBack"), false, false);
        return;
    }
    ps.Button(_T("CmdBtn_GoBack"), BTT_GOBACK, true);
}

///////////////////////////////////////
// CreateStoreModeMenu
// 
// Purpose: Create store mode menu state
// Parameters: None
///////////////////////////////////////
SMStateBase* SMStoreModeMenuBase::CreateStoreModeMenu()
{
    if (nStoreModeMenuState == BES_STATERETURN)
        return createState(BES_STOREAUTHORIZATION); //SMSmAuthorization is the default
    else
        return createState(nStoreModeMenuState);  
}

/////////////////////////////////////
// PSCmdButton
// 
// Purpose: Handle "Command" button press.  This method is called
//          when a click occurs on a button or button list named "CmdBtn_X..."
//          where any sequence of characters can follow the "_"
//          For the StoreModeMenu version,  we are checking the button name
//          and calling the appropriate button handler function.
// Parameters
//   csContext - name of the context 
//   csControl - name of the control
//   csCommand - contents of the control "Data", defined either in a button's "Data"
//               property or a button list's "List" property
//        		<Control Name="CmdBtn_Button1" Type="Button">
//                  <Data>control string</Data>
//              </Control>
//              <Control Name="CmdBtn_List1" Type="ButtonList">
//                  <List>
//                     <Button>Label1,-,control string 1</Button>
//                     <Button>Label1,-,control string 2</Button>
//                  </List>
//              </Control>
///////////////////////////////////////
SMStateBase *SMStoreModeMenuBase::PSCmdButton(CString csContext, CString csControl, CString csCommand)
{
    trace(L6,_T("+SMStoreModeMenuBase::PSCmdButton(%s, %s, %s)"), csContext, csControl, csCommand);
    
    SMStateBase *retState = STATE_NULL;
	if (csControl.CompareNoCase(_T("CmdBtn_CloseLane")) == 0)
        retState = OnCloseLaneButton();
    else if (csControl.CompareNoCase(_T("CmdBtn_Suspend")) == 0)
        retState = OnSuspendButton();
    else if (csControl.CompareNoCase(_T("CmdBtn_Reporting")) == 0)
        retState = OnReportingButton();
    else if (csControl.CompareNoCase(_T("CmdBtn_MediaStatus")) == 0)
        retState = OnMediaStatusButton();
    else if (csControl.CompareNoCase(_T("CmdBtn_SystemFunctions")) == 0)
        retState = OnSystemFunctionsButton();
    else if (csControl.CompareNoCase(_T("CmdBtn_AssistMode")) == 0)
        retState = OnAssistModeButton();
    else if (csControl.CompareNoCase(_T("CmdBtn_PrintBarcode")) == 0)
        retState = OnPrintBarcodeButton();
    else if (csControl.CompareNoCase(_T("CmdBtn_ModeOfOperation")) == 0)
        retState = OnModeOfOperationButton();
    else if (csControl.CompareNoCase(_T("CmdBtn_LoadOptions")) == 0)
        retState = OnLoadOptionsButton();
    else if (csControl.CompareNoCase(_T("CmdBtn_EditPickList")) == 0)
        retState = OnEditPickListButton();
    else if (csControl.CompareNoCase(_T("CmdBtn_Utility")) == 0)
        retState = OnUtilityButton();
    else if (csControl.CompareNoCase(_T("CmdBtn_SecurityMaint"))==0)
        retState = OnSecurityMaintButton();
    else if (csControl.CompareNoCase(_T("CmdBtn_GoBack")) == 0)
        retState = OnGoBackButton();
    else 
    {
        retState = STATE(State)::PSCmdButton(csContext, csControl, csCommand);
    }


    trace(L6, _T("-SMSmAssistMenuBase::PSCmdButton()"));
	return retState;
}
