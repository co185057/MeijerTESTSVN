//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCustomMessageBase.cpp
//
// TITLE: Class implementation for Custom Message State
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMCustomMessage.h"    // This state
#include "DMProcedures.h"
DLLEXPORT extern DMProcedures dm;   // Global DM object

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("CustomMessageBase")
IMPLEMENT_DYNCREATE(SMCustomMessageBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(CustomMessage)

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMCustomMessageBase::SMCustomMessageBase()
{
  IMPLEMENT_TIMESTAMP
  saMsgFuncPtr = PSProceduresBase::SecurityMessageModal;
}


///////////////////////////////////////
SMStateBase *SMCustomMessageBase::Initialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("CustomMessage") );	// RFC 330050
	//TAR 277900+ Migrated-BM185018
   if(getAnchorState() != BES_CUSTOMMESSAGE)
   {
     nCustomMessageLastState = getAnchorState();
     setAnchorState(BES_CUSTOMMESSAGE);   
   }
   //TAR 277900-
  SMStateBase *sReturnState = CkStateStackAndReturn();
  if (sReturnState != STATE_NULL)
    return sReturnState;
  if (CustomMessageObj.csPlayWave.GetLength())
    DMSayPhrase(CustomMessageObj.csPlayWave);
 	
  ps.SetCMFrame(_T("CustomMessage"));
  ps.CMButtonFlash(_T("CMButton1Med"),true);

  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
  ps.ShowCMTBText(csTBMessage2Scot);	//thp
 
  ps.Message12(CustomMessageObj.csScreenText, true);

  ps.SetCMLeadthruText(CustomMessageObj.csLeadThruText);
 

  // remote approval
  if (CustomMessageObj.bApprovalNeeded) 
  {
    dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
		                DM_TRICOLORLIGHT_BLINK_1HZ,
			            DM_TRICOLORLIGHT_NORMALREQUEST); 
	CString csAnchorStateName = getSCOTStateName();
	CString csDesc, csInstruction;
    csDesc = ps.GetPSText(RA_SCANNED_UNKNOWN_ID, SCOT_LANGUAGE_PRIMARY);
	csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY), csDesc, ps.GetPSText(RA_LANEBTT_RECALLEDITEM, SCOT_LANGUAGE_PRIMARY), csAnchorStateName, co.IsSecurityDisableOn()==true ? ps.GetPSText(RA_SECONDLINE_SECURITYDISABLED, SCOT_LANGUAGE_PRIMARY) : _T(""), ps.GetPSText(RA_THIRDLINE_RECALLEDITEM, SCOT_LANGUAGE_PRIMARY));
	trace(L7, _T("+ra.OnUnknownItem, Message=%s"), csInstruction);
    ra.OnUnknownItem(csInstruction, FALSE); 
	trace(L7, _T("-ra.OnUnknownItem"));
  }
  //+dp185016 
  if (1 == SMStateBase::m_nTBOutOfSyncError) // if out of sync_stop error, then modify custom message
  {
    fStateFreeze = true;// freeze this state
  	trace(L6, _T("This Custom Message displays an Out Of Sync Stop/Need Reboot"));

    ps.Message12(ps.GetPSText(MSG_POS_TB_OUTOFSYNC_STOP), true);
    nTimeOut = 0;

    ra.RequestEnableAssistMode(false); // disable assist mode key in rap.
    m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_FALSE);
    m_pAppPSX->SetConfigProperty(_T("ButtonHelp"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_FALSE);

    ps.ShowCMFrame();

    DMSaySecurity(SEC_LOW, _T("SecurityLow")); // add say security

    if (dm.IsOpen()) //dp185016 tar 284769                
    {
      dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
          DM_TRICOLORLIGHT_BLINK_1HZ,
          DM_TRICOLORLIGHT_NORMALREQUEST);
    }

  	trace(L6, _T("+getDiagFiles"));
    LONG     lRC;
	  HKEY     hKey;
	  _TCHAR  szFilePath[_MAX_PATH+1];
	  DWORD dwDataSize=_MAX_PATH;

	  lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\Load Controller"),
		  0, KEY_QUERY_VALUE, &hKey);
	  ASSERT(lRC==ERROR_SUCCESS);

	  lRC = RegQueryValueEx(hKey, _T("OutOfSyncDiagFile"), NULL, NULL, (LPBYTE)szFilePath, &dwDataSize);

	  ASSERT(lRC==ERROR_SUCCESS ) ;
    Out_of_sync_Trace_Object.CloseTraceFile(); //dp185016 tar 286968
    //+dp185016 tar 287258
    trace(L6, _T("Get Registry Key Value of OutOfSyncDiagFile : %d"), lRC);

    if( ERROR_SUCCESS == lRC )
    {
      //+additional checking since 4.0 build 43 base, patch A build 49 still registers the 
      //autopushdiagfiles.bat in registry, even if it is not part of that patch.
  		_TCHAR  szDiagPath[_MAX_PATH+1];
  		lRC = RegQueryValueEx(hKey, _T("GetDiagFiles"), NULL, NULL, (LPBYTE)szDiagPath, &dwDataSize);
      
      // we check if GetDiagFiles key has autopushdiagfiles.bat, then autopush getdiag feature
      // installed, else, we use the getdiagfiles.exe
      trace(L6, _T("Value of GetDiagFiles key is : %s"), szDiagPath);

      if( _tcsstr( szDiagPath, _T("AutoPushDiagFiles" ) ) == NULL )
	  {
        _tcscpy( szFilePath, OUTOFSYNCTEMPORARYPARAMETER );
			ExpandPartitionStrings(szFilePath, szFilePath);
	  }		
      //-
    }
    else
	{
      _tcscpy( szFilePath, OUTOFSYNCTEMPORARYPARAMETER );
		ExpandPartitionStrings(szFilePath, szFilePath);
	}

    //-dp185016
	  bool rc = LaunchProgram(szFilePath, 0, false);
  	trace(L6, _T("-getDiagFiles LaunchProgram return code=%d"), rc);
  }
  else
  //-dp185016

  //when the user goes from SMAuthorization, then MSR is swiped, 
  // we could still handle the pending intervention & return it to SMAuthorization
  bStoreModeGoBack = false; //jrparro added TAR#326691

  ps.ShowCMFrame();
  
  //Tar298312
  if (CustomMessageObj.bGoBackScanAndBag)
    {
        bEASReEnableScanner = true;
    }
  return STATE_NULL;                
}


void SMCustomMessageBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("CustomMessage") );	// RFC 330050
	// don't need to do this. It creats problems for the derived classes  
	// ps.CMButtonFlash(IDC_BUTTON1,false);
}


//////////////////////////////////////
SMStateBase *SMCustomMessageBase::PSButton1(void) // OK Button
{
  
  if(CustomMessageObj.bGoBackScanAndBag)
  {
	  CustomMessageObj.Reset();     // Reset data object
	  //+TAR 322945
	  if(nCustomMessageLastState == BES_START)
	  {
		  return TBStart();
	  }
	  //-TAR 322945
	  return setAndCreateAnchorState(BES_SCANANDBAG);
  
  }
  CustomMessageObj.Reset();     // Reset data object
  
  setAnchorState((BEST) nCustomMessageLastState);   //TAR277900
  return STATE_RETURN;

}

////////////////////////////////////////
SMStateBase *SMCustomMessageBase::PSEnterKey(void) // OK for scanned items
{
  DMSayPhrase(_T("KeyPadValid"));
  return PSButton1();          
}
