//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmCashierPasswordBase.CPP 
//
// TITLE: Class implementation for store mode Authenticate state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header
#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMSmCashierPassword.h"     // This state
#include "SMSmMediaStatus.h"      // MGV auth state
#include "SMSmCashManagement.h"
#include "SMSmCashManagementSafePay.h"
#include "SMSmCardManagement.h"


#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmCashierPasswordBase")

IMPLEMENT_DYNCREATE(SMSmCashierPasswordBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SmCashierPassword)

DEFINE_TIMESTAMP

//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
SMSmCashierPasswordBase::SMSmCashierPasswordBase()
: pInputPrompt( PROMPT_CASHIERID )
{
  IMPLEMENT_TIMESTAMP
}

//////////////////////////////////////////////
// Initialize
//////////////////////////////////////////////
SMStateBase  *SMSmCashierPasswordBase::Initialize(void)
{
  trace( L6, _T("+Initialize %d"), pInputPrompt);
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmCashierPassword") );	// RFC 330050

  //If configured to use head cashier context in SCOTOPTS and head cashier already
  //logged in store mode, bypass head cashier login.
  if (!co.csOperationsHeadCashierContext.IsEmpty() &&
      co.csOperationsHeadCashierContext != _T("SmAuthorization") &&
      g_bValidHeadCashier )
  {
        //+SR93 Biometrics
		if (co.m_bOperationsUseFingerPrintDevice && g_bFPTouched) //TAR 395787
		{
			//+TAR394645 
			g_bFPTouched = false; //TAR395787
			_TCHAR  szSoundPath[_MAX_PATH] = _T("%APP_DRIVE%\\scot\\sound");
			_TCHAR szPath[_MAX_PATH];
		
			_tmakepath(szPath,NULL,szSoundPath, ps.GetSoundName(VALIDBIOMETRIC), _T("WAV"));
			CString csSoundFile = szPath;
			GET_PARTITION_PATH(csSoundFile , csSoundFile.GetBuffer(_MAX_PATH));
			csSoundFile.ReleaseBuffer();
			ps.GetPSXObject()->PlaySound( csSoundFile );
			//-TAR394645 
		}
		//-SR93 Biometrics

		// from here, we go to cash management state. upon exit of that state, clear the lists
		SMSmCashManagementBase::m_bfAuthenticated = true;        

		if ( SMSmMediaStatusBase::m_bIsButtonFourPressed )
		{
			RETURNSTATE(SmCardManagement);   //4.2 - Card Dispenser
		}
		else
		{
			if(dm.m_csCashRecyclerModel == _CASHDISPENSER_SAFEPAY)
			{
				RETURNSTATE(SmCashManagementSafePay);
			}

			RETURNSTATE(SmCashManagement);
		}
  }

	ps.SetFrame(_T("SmCashierPassword"));

  if (pInputPrompt == PROMPT_CASHIERID)
  { // do prompt for cashier
    promptForCashierData(PROMPT_CASHIERID);
	}
	else if(pInputPrompt == PROMPT_CASHIERPW)
  { // do prompt for password
    promptForCashierData(PROMPT_CASHIERPW);
	}
  else if( pInputPrompt == PROMPT_CASHIERREENTER )
  {
    promptForCashierData(PROMPT_CASHIERREENTER);
    // reset prompt to prompt_cashierid
    pInputPrompt = PROMPT_CASHIERID;

  }
  trace( L6, _T("-Initialize"));

    return STATE_NULL;              
}



/////////////////////////////////////////////
// PSButton8 - Cancel
/////////////////////////////////////////////
SMStateBase  *SMSmCashierPasswordBase::PSButton8(void)
{
  trace( L6, _T("+PSButton8"));

  if(pInputPrompt == PROMPT_CASHIERPW)
  { 
    //dp185016 tar 308114
    //if we are at the cashier password screen, and <Go Back> button is pressed,
    //then go back to cashier id screen.
    promptForCashierData(PROMPT_CASHIERID);
    return STATE_NULL;
  }
  else if( !dm.m_csCashRecyclerModel.IsEmpty() )
  {
	  if(dm.m_csCashRecyclerModel == _CASHDISPENSER_SAFEPAY)
	  {
		  RETURNSTATE(SmCashManagementSafePay);
	  }
    RETURNSTATE(SmCashManagement); //dp185016 generic cash management
  }
  else
  {
    RETURNSTATE(SmMediaStatus);
  }


  trace( L6, _T("-PSButton8"));

}

/////////////////////////////////////////////
// PSClearKey - Clear all input (on the pinpad)
//////////////////////////////////////////////
SMStateBase * SMSmCashierPasswordBase::PSClearKey()
{
  trace( L6, _T("+PSClearKey"));

  PSClearAllInput();

  trace( L6, _T("-PSClearKey"));

  return STATE_NULL;
}

//Tar 417254
SMStateBase* SMSmCashierPasswordBase::HandleScannerData(const MessageElement* message)
{
	DMScannerDisable();
	
	bool bIsValidOperator = isBarcodeValidOperatorPassword(cDMLastData);
	
    if (bIsValidOperator)      
    { 
		
			//+TAR 343053 
			bool bMakeXMLNow = false;
			if (!rp.IsTransactionInProgress() )
			{
				bMakeXMLNow = true;
				rp.TransactionStart(rp.m_csBusinessDate);
			}
			rp.SetOperator(csOperator);
			rp.OperatorLogin(rp.m_lOperatorLoginID, ATTIN, SCOTAPP);
			rp.m_lOperatorLoginID += 1;
			if (bMakeXMLNow)
			{
				rp.TransactionEnd();
			}
			//-TAR 343053 
			
				
				//DMScannerEnable(); 
				
				USSFDATA(_T("DMLASTBARCODE"), csDMLastBarCode);
				USSFDATA(_T("DMLASTBARLABEL"), csDMLastBarLabel);
				USSFDATA(_T("DMLASTBARTYPE"), csDMLastBarType);
				USSFHOOK(_T("DMXOperator"), nextState = DMScanner(), return nextState);
	
    }
    pInputPrompt = PROMPT_CASHIERREENTER;	
	Initialize();
    return STATE_NULL;
}


//////////////////////////////////////////////
// PSEnterKey
//////////////////////////////////////////////
SMStateBase  *SMSmCashierPasswordBase::PSEnterKey(void)
{
  trace( L6, _T("+PSEnterKey"));

  CString csInput;

  //+SR93 Biometrics support -TAR 395474
	if( co.m_bOperationsUseFingerPrintDevice && g_bFPTouched )
	{		
		if ( g_bFPValid )  
		{
			pInputPrompt = PROMPT_CASHIERPW;
			g_bFPValid = false;
		}		
		else
		{
			pInputPrompt = PROMPT_CASHIERREENTER;
			g_bFPTouched = false; 
			FT_Abort();   //call FT_Abort() since FT_getFingerPrint() is already called.
			Initialize();
			return STATE_NULL;
		}
	}
	//-SR93 Biometrics support -TAR 395474
	else
	{  		
		int nEntryDateLength = ps.GetInput(csInput);
		trace( L6, _T("nEntryDateLength %d"), nEntryDateLength);

		if (!nEntryDateLength  )
		{
			//re-enter
			pInputPrompt = PROMPT_CASHIERPW;

			if( co.m_bOperationsUseFingerPrintDevice )   //TAR 395474
			{
				FT_Abort();
			}

			if (!csCashierPw.GetLength())  //Tar 417254
			{
				Initialize();
				return STATE_NULL;
			}
		}
	}
  
  switch( pInputPrompt )
  {
  case PROMPT_CASHIERID :
    csCashierID = csInput;
    SMStateBase::csCashierID = csCashierID; //Part 2
    pInputPrompt = PROMPT_CASHIERPW;
	if( co.m_bOperationsUseFingerPrintDevice )  //TAR 395474
	{
		FT_Abort();
	}
    break;
  case PROMPT_CASHIERPW :
	if (csInput.GetLength())
			csCashierPw = csInput;

	//+TAR 395474
	if (csCashierPw != _T("") || g_csPassword != _T(""))
	{
		if (g_csPassword == _T("")) // keyed in
		{
			g_csOperator = csCashierID;
			g_csPassword = csCashierPw;
		}
		if (csCashierPw == _T("")) //finger print login
		{
			csCashierID = g_csOperator;
			csCashierPw = g_csPassword;
		}
		//-TAR 395474

		// TAR 324096:  Explicitly check for TRUE return from TBValidateHeadCashier()
		if( TBValidateHeadCashier( g_csOperator, g_csPassword ) == TRUE )  //TAR 395474
		{
			//+SR93 Biometrics
			if (co.m_bOperationsUseFingerPrintDevice && g_bFPTouched) //TAR 395787
			{
				//+TAR394645 
				g_bFPTouched = false; //TAR395787
				_TCHAR  szSoundPath[_MAX_PATH] = _T("%APP_DRIVE%\\scot\\sound");
				_TCHAR szPath[_MAX_PATH];
			
				_tmakepath(szPath,NULL,szSoundPath, ps.GetSoundName(VALIDBIOMETRIC), _T("WAV"));
				CString csSoundFile = szPath;
				GET_PARTITION_PATH(csSoundFile , csSoundFile.GetBuffer(_MAX_PATH));
				csSoundFile.ReleaseBuffer();
				ps.GetPSXObject()->PlaySound( csSoundFile );
				//-TAR394645 
			}
			//-SR93 Biometrics

			// from here, we go to cash management state. upon exit of that state, clear the lists
			SMSmCashManagementBase::m_bfAuthenticated = true;        

			if ( SMSmMediaStatusBase::m_bIsButtonFourPressed )
			{
				RETURNSTATE(SmCardManagement);   //4.2 - Card Dispenser
			}
			else
			{
				if(dm.m_csCashRecyclerModel == _CASHDISPENSER_SAFEPAY)
				{
					RETURNSTATE(SmCashManagementSafePay);
				}

				RETURNSTATE(SmCashManagement);
			}
		}
		else
		{
		  pInputPrompt = PROMPT_CASHIERREENTER;	
		}
	}
    break;
  case PROMPT_CASHIERREENTER : // should never go here since this was set in Initialize()
    break;
  }

  Initialize();

  trace( L6, _T("-PSEnterKey"));


  return STATE_NULL;
}

SMStateBase  *SMSmCashierPasswordBase::PSNumericKey(const long lKey)
{
  trace( L6, _T("+PSNumericKey %d"), lKey );

  PSEchoInput(lKey);

  return STATE_NULL;

}

void SMSmCashierPasswordBase::promptForCashierData(PROMPT pInputPrompt)
{
  trace( L6, _T("+promptForCashierData %d"), pInputPrompt );

  ps.ShowSMMNumericKeypad(true);

  switch( pInputPrompt )
  {
    case PROMPT_CASHIERPW :
		{
			//dp185016 tar 308114
			//just be sure that what we received as input, pInputPrompt is also
			//this class' member variable's value
			this->pInputPrompt = PROMPT_CASHIERPW;

			ps.SetTitle(ps.GetPSText( MSG_CASHIER_PASSWORD ), _T("") );
			ps.SetLeadthruText(MSG_KEYIN_PASSWORD); 
			ps.AllowInput(MAX_DIGITS_PASSWORD,true,false);
			if (co.m_bOperationsUseFingerPrintDevice)   //TAR 395474
			{
				FT_Abort();   //call FT_Abort(), disable fp when keyin on pw.
			}                                              
			break;
		}
    
    case PROMPT_CASHIERREENTER :
		{
			DMScannerEnable();  //Tar 417254
			//dp185016 tar 308114
			//just be sure that what we received as input, pInputPrompt is also
			//this class' member variable's value
			this->pInputPrompt = PROMPT_CASHIERREENTER; 

			//Tar 396846
			g_csOperator = _T("");
			g_csPassword = _T("");

			// cashier data not accepted, re-enter text
			ps.SetTitle(ps.GetPSText( MSG_CASHIER_ID ), _T("") );
			if (co.m_bOperationsUseFingerPrintDevice)   //TAR 395474
			{											
				ShowCashierIDSceen();
			}
			else
			{
				ps.SetLeadthruText(MSG_INCORRECTCASHIER);
			}			
			ps.AllowInput(MAX_DIGITS_OPERATOR,false,false);				
			break;
		}

    case PROMPT_CASHIERID :
		{		
			DMScannerEnable();
			//dp185016 tar 308114
			//just be sure that what we received as input, pInputPrompt is also
	        //this class' member variable's value
		    this->pInputPrompt = PROMPT_CASHIERID; 

			//Tar 396846
			g_csOperator = _T("");
			g_csPassword = _T("");

			ps.SetTitle(ps.GetPSText( MSG_CASHIER_ID ), _T("") );
			if (co.m_bOperationsUseFingerPrintDevice)   //TAR 395474
			{								
				ShowCashierIDSceen();
			}
			else
			{
				ps.SetLeadthruText(MSG_KEYIN_ID);
			}
			ps.AllowInput(MAX_DIGITS_OPERATOR,false,false);
			break;
		}

  }


	ps.ShowFrame();

  trace( L6, _T("-promptForCashierData") );

}

//Tar 417254
SMStateBase * SMSmCashierPasswordBase::DMScanner(void)
{
	csCashierID = csOperator;
	csCashierPw = csPassword;

	return PSEnterKey();
}

void SMSmCashierPasswordBase::UnInitialize()
{
  trace( L6, _T("+UnInitialize()") );
  ps.PSHideSMMNumericKeypad();

  DMScannerDisable();
  if (co.m_bOperationsUseFingerPrintDevice)  //SR93 Biometrics TAR 395474
  {
	FT_Abort();
  }
    

  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmCashierPassword") );	// RFC 330050
  trace( L6, _T("-UnInitialize()") );

}

///////////////////////////////////////////////////////////////////////////
//+SR93 Biometrics -- TAR 395474
void SMSmCashierPasswordBase::ShowCashierIDSceen()
{
	g_csOperator = _T("");
	g_csPassword = _T("");
		
	ps.SetFrame(_T("SmCashierPassword"));

	if (co.m_bOperationsUseFingerPrintDevice)
	{
		if (g_bBiometricACLServer && g_bBiometricACLDevice)
		{			
			BOOL bFTGetFP=FALSE;
			try
			{
				bFTGetFP = FT_GetFingerprint();
				to.Trace(L6, _T("FT_GetFingerprint() with ret= %d"), bFTGetFP );
			}
			catch(CException* e)                                         
			{
				_TCHAR szWork[255];
				memset(szWork, 0, 255);
				e->GetErrorMessage( szWork, sizeof( szWork ) );
				to.Trace(L6, _T("Exception in FT_GetFingerprint(): %s"), szWork);
				bFTGetFP = FALSE;
				e->Delete();
			}
			catch(...)
			{
				long le = GetLastError();   
				to.Trace(L6, _T("Exception in FT_GetFingerprint(): %d"), le);
				bFTGetFP = FALSE;

			}

			if (bFTGetFP && (PROMPT_CASHIERID == pInputPrompt))
			{			
				ps.SetLeadthruText(MSG_FINGERPRINT_LOGIN);				
			}
			else if(bFTGetFP && (PROMPT_CASHIERREENTER == pInputPrompt))
			{
				ps.SetLeadthruText(LTT_INVALID_FINGERPRINT_LOGIN);
			}
			else
			{				
				ps.SetLeadthruText(MSG_KEYIN_ID);
			}
		}
		else
		{			
			to.Trace(L6, _T("Fingerprint Error: variable is set to false. g_bBiometricACLServer= %d, g_bBiometricACLDevice=%d"),g_bBiometricACLServer, g_bBiometricACLDevice );
			to.Trace(L6, _T("Check fingerprint device and ACL server is connected."));
			if(PROMPT_CASHIERREENTER == pInputPrompt)
				ps.SetLeadthruText(MSG_INCORRECTCASHIER);
			else
				ps.SetLeadthruText(MSG_KEYIN_ID);
		}
	}
	else
	{		
		ps.SetLeadthruText(MSG_KEYIN_ID);
	}
}
//-SR93 Biometrics
