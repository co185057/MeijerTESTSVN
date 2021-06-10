//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmUtilityBase.CPP
//
// TITLE: Class implementation for store mode Utility state
//
//
// AUTHOR:  Feng Zhang
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header
#include "USSFCommon.h"


// Needed for GetGUIThreadInfo API //
#include <winable.h>

#include "Common.h"                 // MGV common includes

#include "SMState.h"
#include "SMSmUtility.h"
#include "SMSmSystemFunctions.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmUtilityBase")

bool SMSmUtilityBase::m_ButtonDisable = false;

IMPLEMENT_DYNCREATE(SMSmUtilityBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////
SMSmUtilityBase::SMSmUtilityBase()
{
  m_nButtonPressed = 0;
  IMPLEMENT_TIMESTAMP
}

///////////////////////////////////////////
SMStateBase  *SMSmUtilityBase::Initialize(void)
{
	trace(L6, _T("SMSmUtilityBase::Initialize"));
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmUtility") );	// RFC 330050

  // New UI
    ps.SetFrame(_T("SmUtility"));
    ps.SetTitle(MSG_UTILITY, PS_BLANK);

	ps.Echo(PS_BLANK);
    
	if(!m_ButtonDisable)
	{   
		//Enable buttons
		ps.Button(_T("SMButton1"), MSG_EXIT_FASTLANE, !ps.RemoteMode()); 
		ps.Button(_T("SMButton2"), MSG_GET_DIAG_FILE, !ps.RemoteMode()); 
		ps.Button(_T("SMButton3"), MSG_REBOOT_SYSTEM, true); 
		ps.Button(_T("SMButton4"), MSG_SHUTDOWN_SYSTEM,true);
		ps.Button(_T("SMButton5"), MSG_VOLUMN_CONTROL,true);
        ps.Button(_T("SMButton6"), MSG_MULTIMEDIA_TRAINING_TOOL, co.fStateTrainingProgramButtonVisible);
        COleVariant v = (co.fStateTrainingProgramButtonVisible ? VARIANT_TRUE : VARIANT_FALSE);
        m_pAppPSX->SetConfigProperty(_T("SMButton6"), _T("SmUtility"), UI::PROPERTYVISIBLE, v);
		ps.Button(_T("SMButton8"), BTT_GOBACK, true);
		m_nButtonPressed = 0;
	}
	else
	{
		//Disable buttons
		ps.Button(_T("SMButton1"), MSG_EXIT_FASTLANE, false); 
		ps.Button(_T("SMButton2"), MSG_GET_DIAG_FILE, false); 
		ps.Button(_T("SMButton3"), MSG_REBOOT_SYSTEM, false); 
		ps.Button(_T("SMButton4"), MSG_SHUTDOWN_SYSTEM,false);
		ps.Button(_T("SMButton5"), MSG_VOLUMN_CONTROL,false);
		if(co.fStateTrainingProgramButtonVisible)   //TAR 275786 disable only if button is visible 
		{
		        ps.Button(_T("SMButton6"), MSG_MULTIMEDIA_TRAINING_TOOL, false);
		}
		ps.Button(_T("SMButton8"), BTT_GOBACK, false);
		m_ButtonDisable = false;
	}

    // TAR 236945 - Needed for Remote, this api will check option whether to display or not.
    ps.Weight(lDMScaleWeight);
	ps.ShowTrainingModeOnScreen(TrainingModeObj.IsTMOn());  //Tar 228689, While lane in training, RAP store mode did not show in training
    ps.SetLeadthruText(LTT_SELECT_UTILITY, PS_BLANK);  // TAR 228095 
	ps.ShowSMTotal(true);
    ps.ShowSMTBText(csTBMessage2Scot);	
    ps.SMShowItemReceipt(true);
    ps.ShowFrame();



  return STATE_NULL;							  // stay in SmUtilityBase state
}

///////////////////////////////////////////
// Button 1 Exit FastLane
///////////////////////////////////////////
SMStateBase  *SMSmUtilityBase::PSButton1(void)
{
	trace(L6, _T("SMSmUtilityBase::PSButton1"));

  if(co.csUtilityExitPassword != _T("") && co.csUtilityExitID != _T(""))  
  {
	  //Need Get Password Approval
	  m_nButtonPressed = 1;
	  return STATE(OperatorPasswordState)::Initialize();
  }
  else
  {
	//Don't need password
	m_ButtonDisable = true;
	Initialize();     //Disable all buttons
	UtilityExitFastLane();
	Initialize();     //Reenable all buttons
	return STATE_NULL;
  }
}


///////////////////////////////////////////
// Button 2 Get Diag Files
///////////////////////////////////////////
SMStateBase  *SMSmUtilityBase::PSButton2(void) // 
{
   trace(L6, _T("SMSmUtilityBase::PSButton2"));

  if(co.csUtilityDiagPassword != _T("") && co.csUtilityDiagID != _T(""))  
  {
	  //Need Get Password Approval
	  m_nButtonPressed = 2;
	  inputMode = PROMPTING_FOR_OPERATOR;

	  return STATE(OperatorPasswordState)::Initialize();
  }
  else
  {
	//Don't need password
	m_ButtonDisable = true;
	Initialize();     //Disable all buttons
	UtilityGetDiagFiles();
	Initialize();     //Reenable all buttons	
	return STATE_NULL;
  }
}

///////////////////////////////////////////
// Button 3 Reboot
///////////////////////////////////////////
SMStateBase  *SMSmUtilityBase::PSButton3(void) // 
{
   trace(L6, _T("SMSmUtilityBase::PSButton3"));

  if(co.csUtilityRebootPassword != _T("") && co.csUtilityRebootID != _T(""))  
  {
	  //Need Get Password Approval
	  m_nButtonPressed = 3;
	  inputMode = PROMPTING_FOR_OPERATOR;

	  return STATE(OperatorPasswordState)::Initialize();
  }
  else
  {
	//Don't need password
	m_ButtonDisable = true;
	Initialize();     //Disable all buttons
	UtilityRebootEntireSystem();
	Initialize();     //Reenable all buttons
	return STATE_NULL;
  }
}

///////////////////////////////////////////
// Button 4 Shutdown
///////////////////////////////////////////
SMStateBase  *SMSmUtilityBase::PSButton4(void) // 
{
   trace(L6, _T("SMSmUtilityBase::PSButton4"));

  if(co.csUtilityShutdownID != _T("") && co.csUtilityShutdownPassword != _T(""))  
  {
	  //Need Get Password Approval
	  m_nButtonPressed = 4;
	  inputMode = PROMPTING_FOR_OPERATOR;

	  return STATE(OperatorPasswordState)::Initialize();
  }
  else
  {
	//Don't need password
	m_ButtonDisable = true;
	Initialize();
	UtilityShutdownEntireSystem();
	Initialize();
	return STATE_NULL;
  }

}

///////////////////////////////////////////
// Button 5 Volume Control
///////////////////////////////////////////
SMStateBase  *SMSmUtilityBase::PSButton5(void) // 
{
   trace(L6, _T("SMSmUtilityBase::PSButton5"));

  if(co.csUtilityVolumeID != _T("") && co.csUtilityVolumePassword != _T(""))  
  {
	  //Need Get Password Approval
	  m_nButtonPressed = 5;
	  inputMode = PROMPTING_FOR_OPERATOR;

	  return STATE(OperatorPasswordState)::Initialize();
  }
  else
  {
	//Don't need password
	m_ButtonDisable = true;
	Initialize();
	UtilityVolumeControl();
	Initialize();
	return STATE_NULL;
  }

}

///////////////////////////////////////////
// Button 6 Multimedia training tool
///////////////////////////////////////////
SMStateBase  *SMSmUtilityBase::PSButton6(void) // 
{
    trace(L6, _T("SMSmUtilityBase::PSButton6"));
    
    // Don't need password for this feature
    m_ButtonDisable = true;
    Initialize();
    UtilityMultimediaTrainingProgram();
    Initialize();
    return STATE_NULL;
}

///////////////////////////////////////////
// Button 7 - Blank
///////////////////////////////////////////

///////////////////////////////////////////
// Button 8 - Return to Shopping
///////////////////////////////////////////
SMStateBase  *SMSmUtilityBase::PSButton8(void)
{
    RETURNSTATE(SmSystemFunctions);
}


SMStateBase * SMSmUtilityBase::PSButtonGoBack()
{
	return Initialize();
}


SMStateBase * SMSmUtilityBase::TimedOut(void)
{
	return Initialize();
}


////////////////////////////////////////
// NewUI
SMStateBase  *SMSmUtilityBase::PSReceiptUp(void)
{
  ps.ScrollUp(_T("SMReceipt"));
  return STATE_NULL;
}

SMStateBase  *SMSmUtilityBase::PSReceiptDown(void)
{
  ps.ScrollDown(_T("SMReceipt"));
  return STATE_NULL;
}
// End NewUI


//Enter USER ID and PASSWORD
/////////////////////////////////////////////////////
bool SMSmUtilityBase::getOperatorPassword()
{
  if (inputMode == PROMPTING_FOR_OPERATOR || inputMode == SHOW_WAIT_FOR_APPROVAL)
  {
    ps.GetInput(csOperator);
	if (csOperator != _T(""))
    // Operator entered, now prompt for password
    {
      promptForPassword();
    }
    else
    // Nothing entered - leave and continue to prompt for operator
	DMSayPhrase(KEYPADINVALID);
    return false;
  }

  else if (inputMode == PROMPTING_FOR_PASSWORD)
  {
    ps.GetInput(csPassword);
    // If password entered, validate operator and password
	if (csPassword != _T(""))
    {

      // read 4690 operator file
		if (UtilityValidateOperator(csOperator,  // ... operator ID, ASCII 1-8 digits
                               csPassword)) // ... password, ASCII 1-8 digits
		{
	        // Valid operator/password, can leave prompting mode
			fInValidOperatorPassword = false;
		    inputMode = OPERATOR_PASSWORD_ACCEPTED;
            if (dm.IsOpen())     //TAR#125161 don't use dm if it's not open b/c of an error
            {
	            // (*) SR898 ====================
                DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                                   DM_TRICOLORLIGHT_OFF,
                                   DM_TRICOLORLIGHT_NORMALREQUEST);
            }

	        // Reshow screen, noting that password has been verified
	        showScreen(true);
			trace(L7, _T("+rp.m_bTransAssisted"));
            rp.SetInterventionTerminalNumber(rp.GetTerminalNumber());
			rp.m_bTransAssisted = true;
			trace(L7, _T("-rp.m_bTransAssisted<%d>"),rp.m_bTransAssisted);
        
				return true;
		}
		else  // No invalid
		{
			fInValidOperatorPassword = true;
              displayInvalidMessage();

			return false;
		}
    }

    // If we haven't returned yet, there was no password or it was invalid.
    // Start over and ask for operator again
    DMSayPhrase(KEYPADINVALID);
    promptForOperator();
    return false;

  } // else if prompting for password

  return false;
}


//To validate userid and password for each utility button
bool SMSmUtilityBase::UtilityValidateOperator(CString csOperID, CString csPassWD)
{
	bool bReturn = false;

	trace(L6, _T("SMSmUtiltiy::UtiltityValidateOperator(csOpeartor= %s, csPassWD = %s), Button Pressed is %d "), csOperID, csPassWD, m_nButtonPressed);
	switch(m_nButtonPressed)
	{
		case 1:
		{
			if(csOperID == co.csUtilityExitID && csPassWD == co.csUtilityExitPassword)
			{
				bReturn = true;
			}
			break;
		}
		case 2:
		{
			if(csOperID == co.csUtilityDiagID && csPassWD == co.csUtilityDiagPassword)
			{
				bReturn = true;
			}
			break;
		}
		case 3:
		{
			if(csOperID == co.csUtilityRebootID && csPassWD == co.csUtilityRebootPassword)
			{
				bReturn = true;
			}
			break;
		}
		case 4:
		{
			if(csOperID == co.csUtilityShutdownID && csPassWD == co.csUtilityShutdownPassword)
			{
				bReturn = true;
			}
			break;
		}
		case 5:
		{
			if(csOperID == co.csUtilityVolumeID && csPassWD == co.csUtilityVolumePassword)
			{
				bReturn = true;
			}
			break;
		}
		default:
			trace(L6, _T("SMSmUtiltiy::UtiltityValidateOperator. Should not be here "));
	}
	return bReturn;
}

void  SMSmUtilityBase::showScreen(bool passwordVerified)
{

	if(!passwordVerified)
	{
	    if (inputMode == PROMPTING_FOR_OPERATOR || inputMode == SHOW_WAIT_FOR_APPROVAL)
		{
			//saMsgFuncPtr = ps.SecurityMessageNonmodal;
        
			//Tar 393757
			STATE(OperatorPasswordState)::ShowEnterID();  //SR93 Biometrics
			ps.SetCMLeadthruText( TXT_ENTER_ID ); //Tar 395105
			ps.AllowInput( co.GetnOperationsOperatorIDMaxDigit(), false, false );
			ps.ShowCMFrame( false );   
				
			return;
		}
		
	}

	ps.SetCMFrame(_T("SmUtility"));
	
	return;
}


void SMSmUtilityBase::UtilityExitFastLane(void)
{
   trace(L6, _T("SMSmUtilityBase::UtilityExitFastLane"));
   mo.PostGP(GP_STOP);

	return;
}

void SMSmUtilityBase::UtilityGetDiagFiles(void)
{
    trace(L6, _T("+SMSmUtilityBase::UtilityGetDiagFiles"));

	USSFutil.DumpSymbolsByElementID();

    //Find GetDiagFile.exe path
	LONG     lRC;
	HKEY     hKey;
	_TCHAR  szFilePath[_MAX_PATH+1];
	DWORD dwDataSize=_MAX_PATH;

	lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\Load Controller"),
			 0, KEY_QUERY_VALUE, &hKey);
	ASSERT(lRC==ERROR_SUCCESS);

    CString cDiagFile;

	// Get the file path
	lRC = RegQueryValueEx(hKey, _T("GetDiagFiles"), NULL, NULL,
			  (LPBYTE)szFilePath, &dwDataSize);
	ASSERT(lRC==ERROR_SUCCESS ) ;

	cDiagFile = szFilePath;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));
	ZeroMemory(&si, sizeof(si));

	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;


	si.dwFlags |= STARTF_USEPOSITION;
	si.dwFlags |= STARTF_USESIZE;

	LPTSTR lString = cDiagFile.GetBuffer(cDiagFile.GetLength());

	int rc = CreateProcess(NULL, lString , NULL, NULL, false, 0, NULL, NULL, &si, &pi);
	DWORD exitCode;

	while (1)
	{
		DWORD rc =	WaitForSingleObject(pi.hProcess, INFINITE);
		if (rc == WAIT_TIMEOUT)
		{
		}
		GetExitCodeProcess(pi.hProcess, &exitCode);
		if (exitCode != STILL_ACTIVE)
		{
			break;
		}
	}
	cDiagFile.ReleaseBuffer();
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
  	//mo.PostGP(WMU_SCOT_GETDIAGFILES);
   trace(L6, _T("-SMSmUtilityBase::UtilityGetDiagFiles"));


	return;
}

void SMSmUtilityBase::UtilityRebootEntireSystem(void)
{
   trace(L6, _T("+SMSmUtilityBase::UtilityRebootEntireSystem"));


	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	if (OpenProcessToken(GetCurrentProcess(), 
				   TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hToken))
	{	
		LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,
					   &tkp.Privileges[0].Luid);

		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		AdjustTokenPrivileges(hToken, FALSE,&tkp, 0, (PTOKEN_PRIVILEGES) NULL, 0);
	
		if (GetLastError() == ERROR_SUCCESS)
		{	   	
			if (!InitiateSystemShutdown(NULL, NULL, 0, FALSE, TRUE))
               trace(L6, _T("SMSmUtilityBase::UtilityRebootEntireSystem:System Reboot did not work.\n"));

		}
		else
               trace(L6, _T("SMSmUtilityBase::UtilityRebootEntireSystem:System Reboot did not work.\n"));
	}
	else
		TRACE(_T("CStateDialogMainUtility::RebootSystem(): OpenProcessToken() Failed\n"));

   trace(L6, _T("-SMSmUtilityBase::UtilityRebootEntireSystem"));
	return;
}

void SMSmUtilityBase::UtilityShutdownEntireSystem(void)
{
   trace(L6, _T("+SMSmUtilityBase::UtilityShutdownEntireSystem"));

	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	if (OpenProcessToken(GetCurrentProcess(), 
				   TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hToken))
	{	
		LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,
					   &tkp.Privileges[0].Luid);

		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		AdjustTokenPrivileges(hToken, FALSE,&tkp, 0, (PTOKEN_PRIVILEGES) NULL, 0);
	
		if (GetLastError() == ERROR_SUCCESS)
		{	   	
			if (!InitiateSystemShutdown(NULL, NULL, 0, FALSE, FALSE))
               trace(L6, _T("SMSmUtilityBase::ShutdownSystem:System Reboot did not work.\n"));

		}
		else
               trace(L6, _T("SMSmUtilityBase::ShutdownSystem:System Reboot did not work.\n"));
	}
	else
        trace(L6, _T("SMSmUtilityBase::ShutdownSystem: OpenProcessToken() Failed\n"));

   trace(L6, _T("-SMSmUtilityBase::UtilityShutdownEntireSystem"));

	return;
}

void SMSmUtilityBase::UtilityVolumeControl(void)
{
  trace(L6, _T("+SMSmUtilityBase::UtilityVolumeControl"));

  // start of TAR 266679
  CString sCmdLine;
  sCmdLine = _T("sndvol32.exe");
  LPTSTR psCmdLine = sCmdLine.GetBuffer(sCmdLine.GetLength());

  STARTUPINFO si = { 0 };
  si.cb = sizeof( si );
  si.dwFlags = STARTF_USESHOWWINDOW;
  si.wShowWindow = SW_MAXIMIZE;
  PROCESS_INFORMATION pi = { 0 };
  if( CreateProcess( NULL, psCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi ) )
  {
    WaitForInputIdle( pi.hProcess, INFINITE );
    GUITHREADINFO gui = { 0 };
    gui.cbSize = sizeof( GUITHREADINFO );
    if( GetGUIThreadInfo( pi.dwThreadId, &gui ) )
    {
      ::ShowWindow( gui.hwndActive, SW_SHOWMAXIMIZED);
      ::SetWindowPos( gui.hwndActive, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_ASYNCWINDOWPOS );
    }

    DWORD dwResult = WAIT_TIMEOUT;
    while( WAIT_TIMEOUT == dwResult)
    {
      dwResult = WaitForSingleObject( pi.hProcess, 200 );
      if(  GetGUIThreadInfo( pi.dwThreadId, &gui ) )
      {
        if( ::IsIconic( gui.hwndActive))
        {
          ::OpenIcon( gui.hwndActive);
        }

        ::ShowWindow( gui.hwndActive, SW_SHOWMAXIMIZED);
        ::SetWindowPos( gui.hwndActive, HWND_TOPMOST, 0, 0, 0, 0, /*SWP_NOMOVE |*/ SWP_NOSIZE | SWP_SHOWWINDOW | SWP_ASYNCWINDOWPOS );
        ::SetFocus( gui.hwndActive);
        ::SetForegroundWindow( gui.hwndActive);
      }
    }
    CloseHandle( pi.hThread );
    // end of TAR 266679
    CloseHandle( pi.hProcess );
  }
  else
  {
    trace(L6, _T("SMSmUtilityBase::UtilityVolumeControl - failed to create process( %s )!"), psCmdLine);
  }

  sCmdLine.ReleaseBuffer();

  trace(L6, _T("-SMSmUtilityBase::UtilityVolumeControl"));

  return;
}


void SMSmUtilityBase::UtilityMultimediaTrainingProgram(void)
{
  trace(L6, _T("+SMSmUtilityBase::UtilityMultimediaTrainingProgram"));

  CString sCmdLine = co.csUtilityInternetExplorerEXE;
  CString sTrainingDocument = co.csUtilityTrainingDocument;

  CString sWholeCommand = sCmdLine + " " + sTrainingDocument;
  LPTSTR strWholeCommand = sWholeCommand.GetBuffer(sWholeCommand.GetLength());

  STARTUPINFO si = { 0 };
  si.cb = sizeof( si );
  si.dwFlags = STARTF_USESHOWWINDOW;
  si.wShowWindow = SW_MAXIMIZE;
  PROCESS_INFORMATION pi = { 0 };

  if( CreateProcess( NULL, strWholeCommand, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi ) )
  {
    WaitForInputIdle( pi.hProcess, INFINITE );
    GUITHREADINFO gui = { 0 };
    gui.cbSize = sizeof( GUITHREADINFO );
    if( GetGUIThreadInfo( pi.dwThreadId, &gui ) )
    {
      ::ShowWindow( gui.hwndActive, SW_SHOWMAXIMIZED);
      ::SetWindowPos( gui.hwndActive, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_ASYNCWINDOWPOS );
    }

    DWORD dwResult = WAIT_TIMEOUT;
    while( WAIT_TIMEOUT == dwResult)
    {
      dwResult = WaitForSingleObject( pi.hProcess, 200 );
      if(  GetGUIThreadInfo( pi.dwThreadId, &gui ) )
      {
        if( ::IsIconic( gui.hwndActive))
        {
          ::OpenIcon( gui.hwndActive);
        }

        ::ShowWindow( gui.hwndActive, SW_SHOWMAXIMIZED);
        ::SetWindowPos( gui.hwndActive, HWND_TOPMOST, 0, 0, 0, 0, /*SWP_NOMOVE |*/ SWP_NOSIZE | SWP_SHOWWINDOW | SWP_ASYNCWINDOWPOS );
        ::SetFocus( gui.hwndActive);
        ::SetForegroundWindow( gui.hwndActive);
      }
    }
    CloseHandle( pi.hThread );
    CloseHandle( pi.hProcess );
  }
  else
  {
    int i = GetLastError();
    trace(L6, _T("SMSmUtilityBase::UtilityMultimediaTrainingProgram - failed to create process( %s ) (%d)!"), strWholeCommand, i);
  }

  sCmdLine.ReleaseBuffer();

  trace(L6, _T("-SMSmUtilityBase::UtilityMultimediaTrainingProgram"));

  return;
}

  
SMStateBase * SMSmUtilityBase::stateAfterPasswordVerify(void)
  {
		trace(L6, _T("SMSmUtilityBase::stateAfterPasswordVerify, m_nButtonPressed= %d"), m_nButtonPressed);
		//Disable all buttons
		m_ButtonDisable = true;

		if( 1 != m_nButtonPressed ) //dp185016 tar 286753
  		  {Initialize();}
	   
		//Perform Action for each button pressed
		switch(m_nButtonPressed)   
		{
		case 1:
			{
				UtilityExitFastLane();
				return STATE_NULL;  //dp185016 tar 286753
				break;
			} 
		case 2:
			{
				UtilityGetDiagFiles();
				break;
			}
		case 3:
			{
				UtilityRebootEntireSystem();
				break;
			}
		case 4:
			{
				UtilityShutdownEntireSystem();
				break;
			}
		case 5:
			{
				UtilityVolumeControl();
				break;
			}
		default:
			trace(L6, _T("SMSmUtiltiy::UtiltityValidateOperator. Should not be here "));
		}
	//Enable all buttons	
	return Initialize();
  }

 // + RFC 330050
void SMSmUtilityBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmUtility") );	
}
// - RFC 330050
