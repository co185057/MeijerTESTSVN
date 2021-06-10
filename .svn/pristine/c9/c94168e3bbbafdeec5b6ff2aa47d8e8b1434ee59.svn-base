//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMAcknowledgeSigBase.cpp
//
// TITLE: Class implimentation for Acknowledge Signature State
//
//
// AUTHOR:    Yafu Wang
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMAcknowledgeSig.h"      // MGV this state
#include "SMRequestSig.h"
#include "SMWaitForApproval.h"
#include "SMTakeCard.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("AcknowledgeSigBase")

DEFINE_TIMESTAMP

IMPLEMENT_DYNCREATE(SMAcknowledgeSigBase, SMCustomerModeState)// MFC Runtime class/object information

//IMPLEMENT_STATE(AcknowledgeSig)

bool SMAcknowledgeSigBase::bAlreadyApproved = false;

//////////////////////////////////////////
SMAcknowledgeSigBase::SMAcknowledgeSigBase()
{
  IMPLEMENT_TIMESTAMP
}

///////////////////////////////////////////
SMStateBase  *SMAcknowledgeSigBase::Initialize(void)
{
   TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("AcknowledgeSig") );
   
   //Remove lines for TAR 374645
   //SMStateBase *sReturnState = CkStateStackAndReturn();
   //if (sReturnState != STATE_NULL)
   //   return sReturnState;
   
	// CANNOT enter Remote Assist Mode at this state
    ra.RequestEnableAssistMode(false);   //TAR213249
	//ra.OnSetTransactionState(RA_STATE_ASSISTNOTOK);

	//Update RAP window with SCOT state
	UpdateStateName(ps.GetPSText(LTT_ACKSIG, SCOT_LANGUAGE_PRIMARY));
// NewUI
	ps.SetCMFrame(_T("AcknowledgeSig")); // we can not use DEFAULT_FRAME, otherwise, SigCap window won't show
    ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);

    if (!bAlreadyApproved)
    {
        ps.CMButtonFlash(_T("CMButton1Med"),true);
        ps.Message12(MSG_SIGN_SIGNATURE, true);
        nTimeOut = 0;				// thp - Turn off Timeout feature.
	    DMSayPhrase(AFTERSIGNING);
    }
    else
    {
        ps.Message12(LTT_CASHPROCESSING, true);
        ps.ButtonState(_T("CMButton1Med"), false, false);
        ps.ButtonState(_T("CMButton2Med"), false, false);
        nTimeOut=2;
    }



// NewUI

	HWND hControlWnd ;
	if (!bAlreadyApproved)
	{
		if (!ps.GetControlHandle(hControlWnd)) //*** NewUI ***//
		{
			//TAR257097+
			SMStateBase * checkState = TurnOnSigCap(hControlWnd); 
			if (checkState != STATE_NULL) //Could be SystemMessage if there is an error inside of TurnOnSigCap()
			{
				return checkState;
			}
			//TAR257097-
		}
	}
	ps.ShowCMFrame(false);		// Disable the help button
	return STATE_NULL;                
}

void SMAcknowledgeSigBase::UnInitialize(void)
{
	TurnOffSigCap() ;
	// CAN enter Remote Assist Mode after leaving this state
    //ra.OnSetTransactionState(RA_STATE_ASSISTOK);
    ra.RequestEnableAssistMode(true);   //TAR213249
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("AcknowledgeSig") );


}



////////////////////////////////////////////////////////////////////////////////////
//
//	FileToSafeArray( COleVariant& vPngFile, CString& csPngFile )
//
//
//	Read a file and place it into a safearray variant 
//
////////////////////////////////////////////////////////////////////////////////////

BOOL SMAcknowledgeSigBase::FileToSafeArray( COleVariant& vPngFile, CString& csTempFile )
{
	BOOL bResult = FALSE; 
	CString debug;

	vPngFile.Clear();
	
	HANDLE hFile = CreateFile( ( LPCTSTR ) csTempFile, GENERIC_READ, FILE_SHARE_READ,
											NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

	if	( hFile != INVALID_HANDLE_VALUE )
	{
		void HUGEP *pData = NULL;

		DWORD dwHS = 0, fileSize = GetFileSize( hFile, &dwHS );

		if	( fileSize > 0 )
		{
			SAFEARRAY *psaArray = SafeArrayCreateVector( VT_I1, 0, fileSize );

			if	( psaArray ) 
			{
				dwHS = 0;

				HRESULT hr = SafeArrayAccessData( psaArray, &pData );

				if	( SUCCEEDED( hr ) )
				{
					void * pBuffer = ( void * ) pData;

					BOOL bRead = ReadFile( hFile, pBuffer, fileSize, &dwHS, NULL );

					SafeArrayUnaccessData( psaArray );

					if	( ( bRead ) && ( fileSize == dwHS ) )
					{
						debug.Format( _T( "FileToSafeArray: Wrote [%d] bytes of Sig Cap png file [%s] to SafeArray" ),
											fileSize, ( LPCTSTR ) csTempFile );
						trace( L2, ( LPCTSTR ) debug );

						vPngFile.parray = psaArray;
						vPngFile.vt = VT_ARRAY | VT_UI1;
						bResult = TRUE;
					}
					else
					{
						debug.Format( _T( "FileToSafeArray: Unable to read file [%s] [%d] bytes" ),
												( LPCTSTR ) csTempFile, fileSize );
						trace( L2, ( LPCTSTR ) debug );
					}

				}
				else
				{
					debug.Format( _T( "FileToSafeArray: SafeArrayAccessData failed for [%s]" ),
											( LPCTSTR ) csTempFile );
					trace( L2, ( LPCTSTR ) debug );
				}

			}
			else
			{
				debug.Format( _T( "FileToSafeArray: Unable to create sig cap safe array size [%d]" ),
											fileSize );
				trace( L2, ( LPCTSTR ) debug );
			}

		}
		else
		{
			debug.Format( _T( "FileToSafeArray: Sig Cap png file empty [%s]" ),
								( LPCTSTR ) csTempFile );
	        trace( L2, ( LPCTSTR ) debug );
		}
		

		CloseHandle( hFile );
	}
	else
	{
		debug.Format( _T( "FileToSafeArray: Unable to open Sig Cap file [%s]" ),
								( LPCTSTR ) csTempFile );
		
        trace( L2, ( LPCTSTR ) debug );
	}
	
	return bResult;
}



//////////////////////////////////////////
SMStateBase  *SMAcknowledgeSigBase::PSButton1(void) // Yes, 
{
	BSTR pData= NULL;
    COleVariant vReturn;
    TCHAR* fnTemp = NULL;
    CString csTempFile;
    bAlreadyApproved = false;
	SYSTEMTIME sTime;
	GetLocalTime( &sTime );
	CString csTime;

    
    /* Create a temporary filename: */
	csTime.Format(_T("SCPIC%.2d%.2d%.2d"), 
		         sTime.wHour,
				 sTime.wMinute,
				 sTime.wSecond);
    fnTemp= _ttempnam( NULL, (LPCTSTR)csTime);
	
    if( fnTemp != NULL )
    {
        csTempFile.Format(_T("%s.PNG"), fnTemp);
        SaveSignatureDataToBitmap(csTempFile);
        vReturn.boolVal = VARIANT_TRUE;
        if (vReturn.boolVal == VARIANT_TRUE)
        {
			COleVariant vPngFile;
			if	( FileToSafeArray( vPngFile, csTempFile ) )
			{
				m_pAppPSX->SetConfigProperty( _T( "SigCap" ), _T( "" ), UI::PROPERTYPNGFILE, vPngFile );
			}
			else
			{
				CString debug;
				debug.Format( _T("Unable to send sigcap image to Rap [%s]"), 
											( LPCTSTR ) csTempFile );
				
	            trace(L2, ( LPCTSTR ) debug );
                m_pAppPSX->SetConfigProperty( _T( "SigCap" ), _T( "" ), UI::PROPERTYPNGFILE, _T( "" ) );
			}
			STATE(State)::SCOTDeleteFile( ( LPCTSTR ) csTempFile );   //TAR 348946
			SafeArrayDestroy( vPngFile.parray );
			vPngFile.parray = NULL;
			vPngFile.vt = VT_EMPTY;
			vPngFile.Clear();
        }
        else
        {
            trace(L6,_T("Unable to save sigcap image"));
        }
        delete fnTemp;
    }
    else
    {
        trace(L6, _T("Cannot create a unique filename for sig cap\n") );
    }

	TurnOffSigCap() ;
    gpSCOTSigCap->Get5991Format(&pData);
  
	if (co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].bVerifySignature
				  || (co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].nVerifySignatureOverAmount != -1 && co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].nVerifySignatureOverAmount  <= lLastTender) ) // if verify, saving signature and printing receipt will be delayed to confirmSig
	{
		//Tar 203306 AH 05/14/02 
		CString csInstruction;
		if (cDMLastTrack==NULL)
		{
			  //The second field in the following  string format should not be NULL  
			  csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), 
			    _T(" "),
		        ps.GetPSText(RA_LANEBTT_SIGCAPTURE, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(LTT_SIGNCONFIRM, SCOT_LANGUAGE_PRIMARY), co.IsSecurityDisableOn()==true || co.GetSecurityDisableSubCheckOn ? ps.GetPSText(RA_SECONDLINE_SECURITYDISABLED, SCOT_LANGUAGE_PRIMARY) : _T(""), 
			    ps.GetPSText(RA_THIRDLINE_SIGCAPTURE, SCOT_LANGUAGE_PRIMARY));
		}
		else
		{
			  csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), 
				CustomerName(cDMLastTrack),
				ps.GetPSText(RA_LANEBTT_SIGCAPTURE, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(LTT_SIGNCONFIRM, SCOT_LANGUAGE_PRIMARY), co.IsSecurityDisableOn()==true || co.GetSecurityDisableSubCheckOn ? ps.GetPSText(RA_SECONDLINE_SECURITYDISABLED, SCOT_LANGUAGE_PRIMARY) : _T(""), 
				ps.GetPSText(RA_THIRDLINE_SIGCAPTURE, SCOT_LANGUAGE_PRIMARY));
		}		  
		trace(L7, _T("+ra.OnSigCapData"));
	    ra.OnSigCapData(csInstruction,(LPCTSTR) pData,(DWORD)(SysStringByteLen(pData)));
		//Tar 203306 end
		trace(L7, _T("-ra.OnSigCapData"));
		SysFreeString(pData);
		//DMMSREject();  //TAR#128476 moved to wait for approval, should not eject card until after store personal has logged in LPM122199
		CREATE_AND_DISPENSE(WaitForApproval) (false,false, true) ; //(bVerifyTrans, bVoidTrans, bVerifySig)  
	}
	else  // if do not verify, print receipt and take card
	{
  
	  //Send signature data to TB
	  if(co.fOperationsSendSignatureDataToTB)
		  TBSendSigData(CustomerName(cDMLastTrack),(LPCTSTR) pData,(DWORD)(SysStringByteLen(pData)));

	  if (lBalanceDue < 0.01)
	  {
		  trace(L7, _T("+ra.OnNormalItem, Message=%s"), ps.GetPSText(RA_TENDER_COMPLETE, SCOT_LANGUAGE_PRIMARY));
		  ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + 
                           ps.GetPSText(RA_TENDER_COMPLETE, SCOT_LANGUAGE_PRIMARY));
		  trace(L7, _T("-ra.OnNormalItem"));
	  }
		  
	  EndOfTransactionPrint(); //SR697

	  // +TAR 390646
      if (co.m_bOperationsPrintSignatureReceipt)
      {
          SaveSigReceiptData(pData);
      }
	  // -TAR 390646
  
      SysFreeString(pData);
  	  CREATE_AND_DISPENSE(TakeCard)(true);  //TAR#132072 -- Proceed SigCap after tendering
	}
}

//////////////////////////////////////////
SMStateBase  *SMAcknowledgeSigBase::PSButton2(void) // no, 
{

	RETURNSTATE(RequestSig)
}

SMStateBase *SMAcknowledgeSigBase::TimedOut(void) // timeout
{
	//return SMStateBase::TimedOut() ;
    if (bAlreadyApproved)
        return PSButton1();
    else
        return STATE_NULL;		// thp - turn off Timeout feature
}

SMStateBase  *SMAcknowledgeSigBase::PSEnterKey(void) // OK
{
  DMSayPhrase(KEYPADVALID);
  return PSButton1();
}

SMStateBase  *SMAcknowledgeSigBase::PSButtonHelp(void)
{
	return STATE(State)::PSButtonHelp();
}
///////////////////////////////////////////////////////////////////////////
SMStateBase *SMAcknowledgeSigBase::OnWtIncreaseNotAllowed()	 
{
	return VoidingWtIncrease(BES_ACKNOWLEDGESIG,BES_ACKNOWLEDGESIG); // TAR 374645

}


///////////////////////////////////////////////////////////////////////////
SMStateBase *SMAcknowledgeSigBase::OnWtDecrease()
{
        //Remove lines for TAR 374645
	//if(!g_bIsForgiveUnexpectedDecreaseDuringTendering)
	//	return FinalizationWtDecreaseNotAllowed(BES_ACKNOWLEDGESIG);
	//else
	//	return STATE_NULL;
	return ThresholdViolWtDecrease(BES_ACKNOWLEDGESIG,BES_ACKNOWLEDGESIG);
}

SMStateBase *SMAcknowledgeSigBase::DMSigcapStarted()
{
    if(gpSCOTSigCap->IsOnScreenCapture())
    {
        ps.CMButtonFlash(_T("CMButton1Med"),true);
        ps.Message12(MSG_SIGN_SIGNATURE, true);
        nTimeOut = 0;				// thp - Turn off Timeout feature.
	    DMSayPhrase(AFTERSIGNING);
    }
    return STATE_NULL;
}