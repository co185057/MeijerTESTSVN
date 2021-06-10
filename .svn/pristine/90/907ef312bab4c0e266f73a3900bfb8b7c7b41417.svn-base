    //////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // FILE:    MPProcedures.cpp
    //
    // TITLE:   Source file for Main Process thread function.
    //
    //
    // AUTHOR:  Peter Denhoed
    //
    //////////////////////////////////////////////////////////////////////////////////////////////////
    
    #include <stdafx.h>                 // MS MFC always needed first
    #include <afxtempl.h>               // MS MFC template header

    #include "Common.h"                 // MGV includes common to most
    
    #include "afximpl.h"
    
    #include "SMState.h"                // MGV base state catch all
    
    #include "SMOutOfService.h"         // MGV start up state
	#include "SMSystemMessageOpPass.h"
    
    #include "MPProcedures.h"           // MGV MP main process thread
    
    #include "SMWaitForApproval.h" 
    #include "SMFatalError.h"     
    
    #define COMP_ID ID_MP
    #define T_ID _T("MP")

// NewUI
    /*extern PSSimpleInterface CMMessageModal;
    extern PSSimpleInterface CMRetTotal;
    extern PSSimpleInterface CMMessageModal;
    extern PSSimpleInterface CMRetTotal;
    extern PSInterface CMRemoteButton;	*/
// NewUI
    
	// AKR    DLLEXPORT  extern CTransBroker tb;
	DLLEXPORT  extern HWND hw;
	DLLEXPORT  extern CWnd *pDialogWindow;				  // access to dialog
				  // RFC 209075 RJF 073102
	
	// TAR179195 don't allow certain screen states to be interrupted by other states
	bool bRAPApprovalMsg = false;
	bool bTBMessage = false;    
	SMStateBase* pgState2Push = NULL;

	static long lGLOBALStackCnt = 0;
    extern bool bInSystemMessage;

	static CString csLastPushedState = _T("");

    void CheckStatePointer(SMStateBase  *State)
    {
      if (State==NULL         ||
        State==STATE_STOP     ||
        State==STATE_RETURN   ||
        State==STATE_RESUME   ||
        State==STATE_RESTART  ||
        State==STATE_ENDED    ||
        State==STATE_ERROR    ||
        State==STATE_NULL)
      {
        ScotError(FATAL,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_STATE_POINT,_T("%d"),(int)State);

        AfxGetApp()->ExitInstance();
        //AfxAbort(); 
        exit(0); //tar 215050
      }
    }
    
    ///////////////////////////////////
    MPProcedures::MPProcedures()
        : m_eventStartScot (TRUE, TRUE,_T("NCRSCOTLoading")),
		  m_eventEndScot (TRUE, TRUE,_T("NCRSCOTUnloading"))

    {
      m_TransLogName = _T("%DATA_DRIVE%\\scot\\logs\\scotapp.stf");
	  GET_PARTITION_PATH(m_TransLogName, m_TransLogName.GetBuffer(_MAX_PATH));
	  m_TransLogName.ReleaseBuffer();

      m_MaxTransLogSize = 1048576;  // 1MB
      m_pTransLogFile = NULL;
      m_eventEndScot.ResetEvent() ;
      m_eventStartScot.ResetEvent();
	  //tar 209906
		try  //CM/CH082302 - add try catch
		{
			lGLOBALStackCnt = 0;
			helpStateStack.deleteAll();
			trace(L2,_T("HelpStack.deleteALL 1 - number states stacked = %d"), lGLOBALStackCnt);
		}
		catch ( CMemoryException *e)
		{
			 trace(L2,_T("HelpStack.deleteALL 1 mem exception Caught - number states stacked = %d"), lGLOBALStackCnt);
			 e->Delete();
		}//CM/CH082302 end - add try catch
	  //tar 209906
    }

    //////////////////////////////////////////
    long MPProcedures::Initialize(void)
    {
      _TCHAR szFile1[_MAX_PATH+1];
      _TCHAR szFile2[_MAX_PATH+1];
	  
      trace(L0,_T(__TIMESTAMP__));
    	
      trace(L2,_T("Initialize"));

      tb.SetLogObjects(&to, &eo);	// allow to trace and log i/o and errors in TB API calls

	  OpenTransLogFile();
      // Save off the current day to use latter in automatic EOD
	  // operation
	  CTime ctCurrentTime = CTime::GetCurrentTime();
	  SMStateBase::nLastDay = ctCurrentTime.GetDay();
	  SMStateBase::nPreviousDay = ctCurrentTime.GetDay();

      // we have to create a current state, so that if initialization
      // causes events there will be a state to catch them, even
      // though we can't initialize it yet
      CREATE_AND_ASSIGN(OutOfService, sCurr)(1);    // show SCOT starting...
      CheckStatePointer(sCurr);
    	
      sCurr->csLastNonHelpStateName = _T("none");
      sCurr->csLastNonHelpStateBMP  = _T("none");
      sCurr->nLastNonHelpStateTimeOut = 0;
      sCurr->fLastNonHelpStateStoreMode = false;
    	
      // just initialize PS for now to present the OutOfService screen
      // here we init with the existing SCOTMSGS file on the local
      // drive, later we will re-init with the copy from the 4690

   //Move data files form SCOT directory to SCOT/CNOFIG
   //  _makepath(szFile1,SCOTDRIVE,SCOTPATH,SCOTBMPS,"DAT");
   _tmakepath(szFile1, NULL, co.csConfigFilePath, co.csDataBitmapFile, _T("DAT")); 
   
   //  _makepath(szFile2,SCOTDRIVE,SCOTPATH,SCOTMSGS,"DAT");
   _tmakepath(szFile2,NULL,co.csConfigFilePath,co.csDataMessageFile, _T("DAT"));

      if (ps.Initialize(szFile1,szFile2,NULL,NULL,0,0,false,false)<0)
      {
        AfxGetApp()->ExitInstance();
        AfxAbort();
      }
    	
      sNext = sCurr;
      CheckStatePointer(sNext);
    	
	  //USSF Start
	  //Initialize of USSF deferred until SMOutOfServiceBase on introduction of EnableUSSF (scotopts.dat) option.
	  //sNext -> InitializeUSSF();
	  //USSF End

      long rc = ProcessNextState();     // force init of OutOfService
      
    	
      return rc;
    }
    
    ////////////////////////////////////////////////////

   long MPProcedures::Process(MessageElement *me)
   {
	trace(L8,_T("Got msg in state %s"),getStateName(sCurr));
	// have a message in 'me'
	// the parse() performs the work of this state and
	// returns STATE_NULL to stay in this state or optionally
	// returns a new next state object pointer
	// Parse() need not have any access to soCurr
	// Parse() does not change the soCurr itself!
	// Parse() returns a STATE_NULL or a new next state
	//
	// parse has this form:
	//
	//  StateObject *Parse(...)
	//  {
	//        ...
	//        return STATE_NULL;
	//  or
	//        return new soScanAndBag;  // new instance
	//  } 

   // TAR 194101 - TAR 176732 causes an unhandled exception when
   // an exception is caught in CSCOTAPPDlg::OnUserMessage() since it passes 
   // NULL to  this function.  I reorganized this to handle the shutdown case 
   // properly.

   if (me==NULL)
      {
      ScotError(RETURN,SM_CAT_STOP_ABORT, SCOTAPP_RESTART);
      sNext = STATE_RESTART;				  // forced restart from abend in SCOTAPPDLG
      }
   else
      {
	   //176732 have to know if the event was from TB LPM082201
      if ( me->id == ID_TRANSBROKER )
         bTBMessage = true;
      else
         bTBMessage = false;

      //176732 have to know if the event was from RAP LPM082201
      if ( (me->id == ID_RA) && (me->raInfo.approvalCode == 1) )
	  {
         bRAPApprovalMsg = true;
		  //TAR195100+ CHU022502 If RAP clears the Approval message and 
		  //We are having g_pDelay_WaitForApprovalStateChange waiting,
		  //we clear g_pDelay_WaitForApprovalStateChange.
		 /*if (SMStateBase::g_pDelay_WaitForApprovalStateChange != NULL )
		 {
			CString csStateName = SMStateBase::g_pDelay_WaitForApprovalStateChange->GetRuntimeClass()->m_lpszClassName;
   			delete SMStateBase::g_pDelay_WaitForApprovalStateChange;
			SMStateBase::g_pDelay_WaitForApprovalStateChange = NULL;
			trace(L6,"Delete the state saved in g_pDelay_WaitForApprovalStateChange since RAP has approved",csStateName);
   		 }*/  //LPM don't need this
		 //TAR195100-
	  }
      else
         bRAPApprovalMsg = false;

      if ( (sCurr < 0) || (sCurr > STATE_ERROR) ) // TAR 363002 Add safety
         {
         sNext = sCurr->Parse(me);
         }
      else
         {
         trace(L2,_T("Non-pointer sCurr value encountered in MPProcedures::Process()"));
         return 0;
         }
      }
   return ProcessNextState();
   }
    
    //////////////////////////////////////////////
    long MPProcedures::IdleTimer(const long lmSec)
    {
      // during shut down the timer may pop one more time
      // ignore it
      if (sCurr==STATE_NULL)
        return 0;
    	
   	trace(L9,_T("Timeout in state %s"),getStateName(sCurr));
 
    	
      // timed out, do housekeeping NOTE: if the current state you are in
      // does not have a idle timer function the base state handles it.
      sNext = sCurr->IdleTimer(lmSec);  // tell idle msec too
    	
      return ProcessNextState();
    }
    
    /////////////////////////////////////
    long MPProcedures::UnInitialize(void)
    {
      trace(L2,_T("UnInitialize"));
    //Begin_EventLog
      //eo.Uninitialize();
    //End_EventLog

	  //USSF Start
	  if (co.fOperationsEnableUSSF)
			SMStateBase::UnInitializeUSSF();
	  //USSF End

      m_eventEndScot.SetEvent();

      CloseTransLogFile();
     
      trace(L2,_T("UnInitialize OK"));
    	
      return 0;
    }
    
    /////////////////////////////////////////
    long MPProcedures::ProcessNextState(void)
    {
      static BOOL fRestart = false;
      static CString csLastName = _T("");
	  static CString csNextStateName = _T("");
      static long CustModeOrStoreMode = -9999;	// track the SA major mode
      static CString csPreScreenState = EMPTY_STRING; //save previous screen state - fix screen blinking	
	  
      bool bIsPreviousStateStoreMode = false;   // TAR 449476 - SSCOB-438
      bool bIsAttendantModeChanged = false;     // TAR 449476 - SSCOB-438
      // TAR 223705: Variable for tracking whether a system message was popped
      //             Off the system message stack.
      BOOL bPoppedSystemMessage = false;
	  // for the Security Agent addition
      while ( sNext != STATE_NULL )
      {
          // TAR 449476 - SSCOB-438 Determine if current state is a store mode state before we move into a new state.
        if(sCurr->storeMode())
		{
           bIsPreviousStateStoreMode = true;
		}
        // TAR 449476 - SSCOB-438 Determine if there is a change in the attendant mode of operation before we move into a new state.
        if(sCurr->m_bIsAttendantModeChanged)
		{
           bIsAttendantModeChanged = true;
		}
        // TAR 449476 - SSCOB-438 end

        // do not change state while state is frozen unless going to help
        if (StateFrozen())
        {
            return 0;
        }
		csLastName = getStateName(sCurr);

		//fix screen blinking
		// TAR 205214 TK - fix screen blinking again
		// If we scan an item and go quickly place on the scale, we will by pass
		// Scan and bag screen and go directly into Bag & EAS. 
        VideoStateCheck(csLastName);

        //+dp185016 tar 280966
        if (SMStateBase::m_bClearStack)
        {
          SMStateBase::m_bClearStack = false;
          trace(L6,_T("+Empty stack of help states"));

		      try  
		      {
			      lGLOBALStackCnt = 0; // do our own counting just to be sure
			      helpStateStack.deleteAll();
			      trace(L2,_T("HelpStack.deleteAll 2 - number states stacked = %d"), lGLOBALStackCnt);
		      }
		      catch ( CMemoryException *e)
		      {
			       trace(L2,_T("HelpStack.deleteAll 2 mem exception Caught - number states stacked = %d"), lGLOBALStackCnt);
			       e->Delete();
		      } 
		      trace(L6,_T("-Empty stack of help states"));
        }
        //-dp185016
		
		// begin TAR179195/176732 don't allow certain screen states to be interrupted by other states LPM091901
		SMStateBase  *sTemp = 0; // TAR 303487
        if ( sNext == STATE_RETURN || sNext==STATE_RESUME)
        {
          // only the HelpMode states save a return state so only they should use STATE_RETURN
          // if any other state tries to use STATE_RETURN return to anchor state
  		  sTemp = SMStateBase::createAnchorState();
 		  csNextStateName = getStateName(sTemp);
		}
		else
		{
          if (sNext != NULL)    // TAR 198401
          {
		     sTemp = sNext;
 		     csNextStateName = getStateName(sTemp);
          } 
		}

	    if (DelayOrIgnoreStateChangeCheck(bPoppedSystemMessage, csLastName, csNextStateName, sTemp))
        {
            return 0;
        }

		csLastPushedState = _T("");
		// end TAR 179195  LPM091901

        trace(L6,_T("Changing state from %s"),csLastName);
        // save the last non-help state's name and timeout for display in
        // SMContextHelp when running in DEBUG mode
        if ( ! sCurr->helpMode() )
        {
          sCurr->csLastNonHelpStateName = csLastName;
          sCurr->csLastNonHelpStateBMP  = ps.Background();
          sCurr->nLastNonHelpStateTimeOut = sCurr->nTimeOut;
          sCurr->fLastNonHelpStateStoreMode = sCurr->storeMode();
        }

        // run the current state's uninitialize
		//USSF Start
        //sCurr->UnInitialize();
		sCurr->StateExit();
		//USSF End
		
		trace(L6,_T("Returned from UnInitialize()"));

		//WE want to go to saved WaitForApproval after InProgress:Uninitialize
        ProcessDelayedApproval(csLastName, csNextStateName, sTemp);
         
        // Process ended, restart, stop or error special codes
        int iExceptionReturn = ProcessExceptionStatePointers(fRestart);
        if (iExceptionReturn != 0)
        {
            return iExceptionReturn;
        }
 
        if ( sNext == STATE_RETURN || sNext==STATE_RESUME)
        {
            ProcessReturnResumeStates(sTemp);
        }
        else if (sCurr!=sNext)          // about to change state, not the same state
        {
            ProcessNewState(bPoppedSystemMessage, csLastName);
        }

        sNext = STATE_NULL;             // STATE_NULL next state again
    		
       trace(L4,_T("Changing state to %s"),getStateName(sCurr));
       if ( csLastName == getStateName(sCurr) )
       {
        trace(L4,_T("Changing to same state <%s>"),getStateName(sCurr));
       }
       else
       {
         LogTransition(getStateName(sCurr), sCurr->nTBLastRealState);
       }
       QueryForStateOverride(csLastName, getStateName(sCurr));	
       if (co.fTracingPSAPITraceStateInit)
       {
          trace(L6, _T("Before %s::Initialize"), getStateName(sCurr));
          traceMem(L6);
       }

       RemoteStateProcessing();  // Determine if state is remote and process accordingly
       
		if (sCurr->storeMode())
        {
           // All store mode screens should be in the primary language.  If there
           // is an exception, it can set itself appropriately in the initialize of 
           // the state.
           ps.SetLanguage(SCOT_LANGUAGE_PRIMARY);

        }
		//CHU080102+
		else
		{

			//TAR210823: Set back to customerLanuguage for the states which are not in store mode
			if (co.fStateDualLanguage)
				ps.SetLanguage(sCurr->m_customerLanguage);  
		}
		//CHU080102-

		trace(L4,_T("Right before initialize() is called.")); //CHU&Dale: just adding more trace
        
		//USSF Start
		//sNext = sCurr->Initialize();
		sNext = sCurr->StateEntry();
		//USSF End

		sCurr->ProcessPendingActions(); //SR697 gc185038
		
		trace(L4,_T("Right after initialize() was called.")); //CHU&Dale: just adding more trace

		if ((sNext == STATE_STOP) || (sNext == STATE_RESTART))
		{
			trace(L0, _T("Severe error, SCOT state flow abort"));
            if(sNext == STATE_STOP)//tar 286744 (Need a way to abort application for severe errors)
            {
                AfxGetApp()->ExitInstance();
                exit(0); 
            }
			return 0;
		}

		// TK - TAR 197064 - took out the re-enabling of buttons from here
		// this allows multiple button presses that causes erroroneous submenus.
		// Buttons are re-enabled in PSProcedures.cpp ShowFrame function.
        // TAR 153940.
 //       PSEnableNotification(hDC, true, PS_ETYPE_TOUCH);
		// TAR 197064

        trace(L7, _T("ProcessNextState(): Returned from  %s::Initialize"), getStateName(sCurr));
        if (co.fTracingPSAPITraceStateInit)
        {
          trace(L6, _T("After %s::Initialize"), getStateName(sCurr));
          traceMem(L6);
        }

        // If we are staying in this state after Init, and we
        // haven't displayed a screen on the Aux Display yet, then
        // let's display the default screen.  If someone shows
        // displays one in their ::Initialize() function, then the 
        // m_bNeedToDisplayAuxScreen variable should be set to false
        // and we won't display here.
        if ((sNext == STATE_NULL) && (sCurr->m_bNeedToDisplayAuxScreen))
          sCurr->DisplayAuxScreen(AUX_SCREEN_DEFAULT);
    		
        // if new state is store mode and if not currently store mode
        // set store mode
        if ( (sCurr->storeMode()) && (CustModeOrStoreMode != STORE_MODE) && (!sCurr->bSecMaintMode))
        {
          sCurr->SASendSCOTInput( STORE_MODE );
          CustModeOrStoreMode = STORE_MODE;
        }
    		
        // if new state is not store mode, not help mode and if currently in store mode
        // set customer mode
		CString csCurrName = getStateName(sCurr);

        if (!sCurr->storeMode() && (!sCurr->helpMode() || (csCurrName.Find(_T("WaitForApproval")) != -1)) && (CustModeOrStoreMode != CUSTOMER_MODE))
        {
          sCurr->SASendSCOTInput( CUSTOMER_MODE );
          CustModeOrStoreMode = CUSTOMER_MODE;
        }

		// set ra into correct mode tar 121634 - previously done in smStateSABase
        if (   !co.IsInAssistMenus()  //Do not record entry/exit if in assist menus.
			&& !co.IsAttendantModeOn() || bIsAttendantModeChanged ) //TAR426337-SSCOB-438 Don't record if in attendant mode
        {
         // TAR 449476 - Add the bIsPreviousStateStoreMode condition so we don't send needless Enter Store Mode messages.
        if (sCurr->storeMode() && (!sCurr->bSecMaintMode) && !bIsPreviousStateStoreMode)
        {
			trace(L7, _T("+ra.OnEnterStoreMode"));
			ra.OnEnterStoreMode(sCurr->GetOperator(), sCurr->GetPassword());
			trace(L7, _T("-ra.OnEnterStoreMode"));
        }
         if (!sCurr->storeMode())
        {
			//TAR 279092+
			if(SMState::bInApprovalNeeded && sCurr->helpMode() && (csCurrName.Find(_T("SystemMessage")) != -1))
			{
				trace(L7, _T("ra.OnExitStoreMode will not be executed this time"));				
			}		
			//TAR 279092-
            // TAR 449476 SSCOB-438 - Add the bIsPreviousStateStoreMode condition so we don't send needless Exit Store Mode messages.
			else if(bIsPreviousStateStoreMode)
			{
				trace(L7, _T("+ra.OnExitStoreMode"));
				ra.OnExitStoreMode();
				trace(L7, _T("-ra.OnExitStoreMode"));
			}			
        }
        }
		

      }
    	
      return 0;
    }
    
    
    
    
    /////////////////////////////////////////////////////////////////////////////
    // Called by ErrorObject to tell main process of an error
    // here we could ingnore the error since it has already been
    // written to the error log but we choose to log the
    // error to the server
    void MainErrorLogger(const long lCompId,	  // component ID, long
                     LPCTSTR szTrcId,			  // trace ID
                     const int nLine,		  // line number in source file
                     const long nLevel,	  // error level C_FATAL...C_INFO
                     CString* pErrorText)          // variable data
    {
       CString csCompId;
       switch (lCompId)
      {
      case ID_MP: csCompId = _T("MP"); break;	// MP Main process
      case ID_SM: csCompId = _T("SM"); break;	// All state classes
      case ID_HC: csCompId = _T("HC"); break;	// HC Emulator components
      case ID_PS: csCompId = _T("PS"); break;	// PS Interface components
      case ID_RP: csCompId = _T("RP"); break;	// RP components
      case ID_DM: csCompId = _T("DM"); break;	// DM Interface components
      case ID_SA: csCompId = _T("SA"); break;	// SA Interface components
      case ID_TB: csCompId = _T("TB"); break;	// TS Interface components
      case ID_TO: csCompId = _T("TO"); break;	// Trace service
      case ID_EO: csCompId = _T("EO"); break;	// Error service
      case ID_MO: csCompId = _T("MO"); break;	// Message service
      case ID_CO: csCompId = _T("CO"); break;	// Configuration object
      case ID_UE: csCompId = _T("UE"); break;	// User Exits
      case ID_GP: csCompId = _T("GP"); break;	// File Utilities
      case ID_TI: csCompId = _T("TI"); break;	// TB interface
      }

 	
      UCHAR  nSeverity = 0;
  	
      switch (nLevel)
      {
    	case C_FATAL:
    		nSeverity = 1;								  // sev 1
    		break;
  	  case C_RESTART:
  		nSeverity = 1;								  // sev 1
    		break;
    	case C_RETURN:
    		nSeverity = 2;								  // sev 2
    		break;
    	case C_INFO:
    		nSeverity = 3;								  // sev 3
    		break;
    	default:
    		nSeverity = 4;								  // sev 4

 		  break;

      }

 	

/* ts.LogErrorTo4690(AppName,        // application name
 		'B',													  // B for smkt
 		nMsgNum,											  // msg number B200
 		nSeverity,										  // 1 high... 5 low
 		nEvent,												  // 0-255, 64-81 cause alerts on NPDA
 		10,														  // length of unique
 		szUnique);										  // 10 data bytes any format
 */

	try	// Sometimes this is throwing an exception itself.  
	{
		
		if(tb.IsTBInitialized() == TB_SUCCESS) // only if TB is currently initialized, call LogErrorToServer
		{
			TBRC rc = tb.LogErrorToServer((LPCTSTR) *pErrorText, nSeverity, lCompId, csCompId, szTrcId, nLine);
		  ASSERT(rc == TB_SUCCESS);
		}
	} 
	catch(...)
	{
		// We can't log an exception, that would cause a recursive call right back here.  
		// This has in the past, caused recursive exceptions in the app, crashing it.   
	}
	return;
}
 



//////////////////////////////////////////////////////////////////////////
 CString MPProcedures::getStateName(const SMStateBase * const pState)
 {
   int classType = (int)pState;
   switch (classType)
   {
   case (int)STATE_NULL:
       return _T("STATE_NULL");
       break;
   case (int)STATE_STOP:
       return _T("STATE_STOP");
       break;
   case (int)STATE_RETURN:
       return _T("STATE_RETURN");
       break;
   case (int)STATE_RESUME:
       return _T("STATE_RESUME");
       break;
   case (int)STATE_RESTART:
       return _T("STATE_RESTART");
       break;
   case (int)STATE_ENDED:
       return _T("STATE_ENDED");
       break;
   case (int)STATE_ERROR:
       return _T("STATE_ERROR");
       break;
   default:
       return pState->GetRuntimeClass()->m_lpszClassName;
       break;
   }
 } 

 
 BOOL MPProcedures::OpenTransLogFile()
 { 
   BOOL rc = FALSE;

   m_pTransLogFile = _tfopen(m_TransLogName, _T("a"));
   if (m_pTransLogFile)
   {
     rc = TRUE;
   }
   else
   {
	 trace(L2,_T("OpenTransLogFile - failed to open %s"), m_TransLogName);
	 // Found case where upcon copy occurring as we try to open can cause us to fail, Retry
     m_pTransLogFile = _tfopen(m_TransLogName, _T("a"));
    if (m_pTransLogFile)
    {
       rc = TRUE;
    }
    else
    {
       trace(L2,_T("OpenTransLogFile - failed to open %s twice"), m_TransLogName);
    }
   } 

   return rc;
 }

 void MPProcedures::CloseTransLogFile()
 {
   if (m_pTransLogFile)
   {
     fclose(m_pTransLogFile);
     m_pTransLogFile = NULL;
   }
 }

 void MPProcedures::ArchiveTransLogFile()
 {
  CString strBakFileName;

  CloseTransLogFile();

  strBakFileName = m_TransLogName;
  strBakFileName += _T(".bak");

  WIN32_FIND_DATA fd;    
  HANDLE hFindFile;
  if ((hFindFile=FindFirstFile(strBakFileName, &fd)) != INVALID_HANDLE_VALUE) 
  {
	  FindClose(hFindFile); //must close handle!
	  //only remove it if it exists
	  CFile::Remove(strBakFileName);   
  }

  _trename(m_TransLogName, strBakFileName);

  OpenTransLogFile();
 }

 void MPProcedures::LogTransition(CString& stateName, TBSTATE lastTBState)
 {
   if (m_pTransLogFile)
   {
     // Now we will ignore some states we don't want to see as they just
     // clutter things up

     if (stateName.Compare(_T("SMInProgress")) == 0)
       return;

     time_t timeNow = time(NULL);

     CString outputLine;
     outputLine.Format(_T("%d,%s,%d\n"), timeNow, stateName, (int) lastTBState);
     _fputts(outputLine, m_pTransLogFile);

     fflush(m_pTransLogFile);

     if (ftell(m_pTransLogFile) > m_MaxTransLogSize)
       ArchiveTransLogFile();
   }

 }


bool MPProcedures::SetDelayWaitForApprovalState(CString &csNextStateName, CString &csLastName)
{
    bool bRetVal = false;
    if (  ( csNextStateName.Find(_T("SecBagAreaBackup")) != -1 )
	     ||( csNextStateName.Find(_T("SecBagBoxThreshold")) != -1 )
	     ||( csNextStateName.Find(_T("SecItemRemovedThreshold")) != -1 )
	     ||( csNextStateName.Find(_T("SecMisMatchWeight")) != -1 )
	     ||( csNextStateName.Find(_T("SecNewItemEntry")) != -1 )
	     ||( csNextStateName.Find(_T("SecSkipBaggingThreshold")) != -1 )
	     ||( csNextStateName.Find(_T("SecUnexpectedDecrease")) != -1 )
	     ||( csNextStateName.Find(_T("SecUnExpectedIncrease")) != -1 && !SMStateBase::bQuietNeedSecApproval) //tar 211789 - AH and DE 08/15/02 
	     ||( csNextStateName.Find(_T("PickingUpItems")) != -1 )
	     ||( csNextStateName.Find(_T("UnDeActivatedItemApproval")) != -1 )
		 ||( csNextStateName.Find(_T("SecBagBoxNotValid")) != -1 )) //SR085
    {
	    if ( SMStateBase::g_pDelay_WaitForApprovalStateChange == NULL )
	    {
          if (co.IsSecurityBypassOn())
          {
             trace(L6,_T("We are at %s, Ignoring next state %s when security bypass on."),csLastName, csNextStateName);
          }
          else
          {
		       if ( csNextStateName.Find(_T("SecBagAreaBackup")) != -1 )
			       SMStateBase::g_pDelay_WaitForApprovalStateChange = SMStateBase::createState(BES_SECBAGAREABACKUP);
		       else if ( csNextStateName.Find(_T("SecBagBoxThreshold")) != -1 )
			       SMStateBase::g_pDelay_WaitForApprovalStateChange = SMStateBase::createState(BES_SECBAGBOXTHRESHOLD);
		       else if ( csNextStateName.Find(_T("SecItemRemovedThreshold")) != -1 )
			       SMStateBase::g_pDelay_WaitForApprovalStateChange = SMStateBase::createState(BES_SECITEMREMOVEDTHRESHOLD);
		       else if ( csNextStateName.Find(_T("SecMisMatchWeight")) != -1 )
			       SMStateBase::g_pDelay_WaitForApprovalStateChange = SMStateBase::createState(BES_SECMISMATCHWEIGHT);
		       else if ( csNextStateName.Find(_T("SecNewItemEntry")) != -1 )
			       SMStateBase::g_pDelay_WaitForApprovalStateChange = SMStateBase::createState(BES_SECNEWITEMENTRY);
		       else if ( csNextStateName.Find(_T("SecSkipBaggingThreshold")) != -1 )
			       SMStateBase::g_pDelay_WaitForApprovalStateChange = SMStateBase::createState(BES_SECSKIPBAGGINGTHRESHOLD);
		       else if ( csNextStateName.Find(_T("SecUnexpectedDecrease")) != -1 )
			       SMStateBase::g_pDelay_WaitForApprovalStateChange = SMStateBase::createState(BES_SECUNEXPECTEDDECREASE);
		       else if ( csNextStateName.Find(_T("SecUnExpectedIncrease")) != -1 ) 
			       SMStateBase::g_pDelay_WaitForApprovalStateChange = SMStateBase::createState(BES_SECUNEXPECTEDINCREASE);
		       else if ( csNextStateName.Find(_T("PickingUpItems")) != -1 )  
			       SMStateBase::g_pDelay_WaitForApprovalStateChange = SMStateBase::createState(BES_PICKINGUPITEMS);
			   //SR085 +
			   else if ( csNextStateName.Find(_T("SecBagBoxNotValid")) != -1 )  
			       SMStateBase::g_pDelay_WaitForApprovalStateChange = SMStateBase::createState(BES_SECBAGBOXNOTVALID);	
			   else if ( csNextStateName.Find(_T("UnDeActivatedItemApproval")) != -1 )  
			       SMStateBase::g_pDelay_WaitForApprovalStateChange = SMStateBase::createState(BES_UNDEACTIVATEDITEM);	
			   //SR085 -
		       trace(L6,_T("We are at %s, Saving next state %s in g_pDelay_WaitForApprovalStateChange."),csLastName, csNextStateName);
          }
	    }
	    else
	    {
		    trace(L6,_T("We are at %s, Not saving next state %s in g_pDelay_WaitForApprovalStateChange. Already has saved state"),csLastName, csNextStateName);
	    }
	    //stay at InProgress
	    bRetVal = true; 
    }
    else if ( csNextStateName.Find(_T("WaitForApproval")) != -1 )
    {
      	//+TAR 374645
       if( (csLastName.Find(_T("RequestSig")) !=-1 )	||
           ( csLastName.Find(_T("AcknowledgeSig")) !=-1 ))
       {
			
           if ( SMStateBase::g_pDelay_WaitForApprovalStateChange == NULL )
           {
                SMStateBase::g_pDelay_WaitForApprovalStateChange = SMStateBase::createState(BES_WAITFORAPPROVAL);
                trace(L6,_T("We are at %s, Proceed to next state %s if last state is RequestSig and AcknowledgeSig."),csLastName, csNextStateName);

           }
           else
           {
               //We already have WaitForApproval waiting, ignore this one and proceed to next state
               trace(L6,_T("We are at %s, Not saving next state %s in g_pDelay_WaitForApprovalStateChange. Already has saved state"),csLastName, csNextStateName);
           }
           //don't stay at InProgress
           bRetVal = false;
       } //-TAR 374645
       else
       {
           if ( SMStateBase::g_pDelay_WaitForApprovalStateChange == NULL )
           {
              SMStateBase::g_pDelay_WaitForApprovalStateChange = SMStateBase::createState(BES_WAITFORAPPROVAL);
           }
           else
           {
             //We already have WaitForApproval waiting, ignore this one and stay at InProgress
             trace(L6,_T("We are at %s, Not saving next state %s in g_pDelay_WaitForApprovalStateChange. Already has saved state"),csLastName, csNextStateName);
           }
	    //stay at InProgress
	    bRetVal = true;
       }
    }
    return bRetVal;

}

// The next two procedures are copied from Visual Studio.Net RUNTIME_CLASS.  When we go
// to visual studio.net, we should use the standard methods and delete these.
#pragma message (_T("MPProcedures::FromName Review this code when changing to visual studio.net"))
#pragma message (_T("  Also review defines in common.h for DECLARE_DYNCREATE and IMPLEMENT_DYNCREATE"))
CRuntimeClass* PASCAL MPProcedures::FromName(LPCSTR lpszClassName)
{
	CRuntimeClass* pClass;

	try
	{

		// search app specific classes
		AFX_MODULE_STATE* pModuleState = AfxGetModuleState();
		AfxLockGlobals(CRIT_RUNTIMECLASSLIST);
		for (pClass = pModuleState->m_classList; pClass != NULL;
			pClass = pClass->m_pNextClass)
		{
			if (lstrcmpA(lpszClassName, pClass->m_lpszClassName) == 0)
			{
				AfxUnlockGlobals(CRIT_RUNTIMECLASSLIST);
				return pClass;
			}
		}
		AfxUnlockGlobals(CRIT_RUNTIMECLASSLIST);
		// search classes in shared DLLs
		AfxLockGlobals(CRIT_DYNLINKLIST);
		for (CDynLinkLibrary* pDLL = pModuleState->m_libraryList; pDLL != NULL;
			pDLL = pDLL->m_pNextDLL)
		{
			for (pClass = pDLL->m_classList; pClass != NULL;
				pClass = pClass->m_pNextClass)
			{
				if (lstrcmpA(lpszClassName, pClass->m_lpszClassName) == 0)
				{
					AfxUnlockGlobals(CRIT_DYNLINKLIST);
					return pClass;
				}
			}
		}
		AfxUnlockGlobals(CRIT_DYNLINKLIST);
	}
	catch ( CMemoryException e )
	{
		 TCHAR msg[ 512 ];
		 memset( msg, 0, sizeof( msg ) );
		 e.GetErrorMessage( ( LPTSTR ) msg, 512, NULL );
		 trace(L0,_T("MPProcedures::FromName exception caught %s"), msg );
		 e.Delete();
		AfxUnlockGlobals(CRIT_RUNTIMECLASSLIST);
		AfxUnlockGlobals(CRIT_DYNLINKLIST);
	}

	return NULL; // not found
}

CRuntimeClass* PASCAL MPProcedures::FromName(LPCWSTR lpszClassName)
{
	USES_CONVERSION;
	return MPProcedures::FromName(W2A(lpszClassName));
}


void MPProcedures::RemoteStateProcessing()
{
    // If currently connected to RAP and the next state is not legal
    // in remote mode, disconnect from RAP
   if (   ps.RemoteMode() 
       && (false == sCurr->PSRemoteLegalForState()))
   {
		trace(L4,_T("In remote mode, disconnect from RAP"));  //CHU&Dale: adding more trace

        ps.DisconnectFromServer( TRUE );	      
		sCurr->fStateAssist = false;
		// merged TAR 225689 to 3.1.1 - fixed by Beiwei:  Tell TB that we are exiting AssistMode  (gd123860).
		tb.SetMonitorAll( false );
		trace (L6, _T("SMStateBase::PSOnRemoteDisconnected Disconnecting remoted devices."));
		dm.EndRemote();
        // TAR 225689:  End
		SMStateBase::ms_bKeepRemoteDevices = false; // TAR 234566 CMal092203
   }
   if (co.IsInAssistMenus() && !sCurr->PSAssistMenuLegalForState())
   {
	   trace(L4,_T("Exiting Assist menus not-allowed state"));
	   co.SetInAssistMenus(false);
	   if (dm.fStateTakeawayBelt && ps.RemoteMode())
		sCurr->TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmRemoteAssistMode") );
	   else
		sCurr->TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmAssistMode") );
       sCurr->SendTBCommand(TBCMD_ASSISTMENUSET, false);
   }
}

bool MPProcedures::StateFrozen()
{
    //381252+
    CString csNextName = getStateName(sNext);
    //if ( (csNextName.Find(_T("SystemMessage")) != -1 )  && sCurr->fStateAssist ) 
    //{
	//	return false;
    //}
    //381252-

	//TAR 408719+
    if ( (csNextName.Find(_T("SystemMessage")) != -1 ))
    {
        if ( (ps.RemoteMode() && sNext->PSRemoteLegalForState()) ||
			 (!ps.RemoteMode() && sCurr->fStateAssist) )	//TAR417992
            return false;
    }
    //TAR 408719-


    if ( sCurr->fStateFreeze || sCurr->fStateAssist) // if in assist mode
    {
      if ( sNext != STATE_RETURN &&
    			sNext != STATE_RESUME &&
    			sNext != STATE_ERROR &&
    			sNext != STATE_ENDED && 
    			sNext != STATE_STOP &&
    			sNext != STATE_RESTART)
      { 
        CheckStatePointer(sNext);
        // Add for "multi state" assist modes
        // Always go on if currently in assist mode and moving to another
        // assist mode state
        if (!(sCurr->fStateAssist && sNext->assistMode()))
        {
            if (!sNext->helpMode() 
               // Regardless of whether the next state is a help state, 
               // do not go there while in Assist Mode. TAR207811
               || sCurr->fStateAssist)
            {
              delete sNext;						  // ignore state change
              sNext = STATE_NULL;
              return true;
            }
        }
      }
    }
    //+dp185016 
    // TB_OUTOFSYNC_STOP
    // if we are at custommessage, and we are at TB_OUTOFSYNC_STOP, then we should
    // ignore any state change
    // I think this is best if we process this separate from the conditions above :
    //      if ( sCurr->fStateFreeze || sCurr->fStateAssist) // if in assist mode ...
    // there are many paths we have to consider if we include this checking to the above conditions
    CString csCurrName = getStateName(sCurr);
    if ( (csCurrName.Find(_T("CustomMessage")) != -1 ) && 
      1 == SMStateBase::m_nTBOutOfSyncError &&
      (sNext != STATE_STOP && sNext != STATE_RESTART) ) // tar 284768
      {
        // delete the next state
        if ( sNext != STATE_RETURN &&
    			  sNext != STATE_RESUME &&
    			  sNext != STATE_ERROR &&
    			  sNext != STATE_ENDED )
        {
          delete sNext;
          sNext = STATE_NULL;
        }
        return true;
      }
    //-dp185016
    return false;

}

void MPProcedures::VideoStateCheck(CString csLastName)
{
    CString csPreScreenState = csLastName;	// csLastName is the previous
	// TAR 205214 TK - fix screen blinking again
	// If we scan an item and go quickly place on the scale, we will by pass
	// Scan and bag screen and go directly into Bag & EAS. Here we will set the
	// g_bSnBScreenIsShown to false since we didn't get a chance to show the screen
	if ( (csPreScreenState.Find(_T("Attract"),1) >= 0) 
		&& (!co.fStatePaintScanAndBagOnInitialScan) ) // TAR 276768
		SMStateBase::g_bSnBScreenIsShown = false;
	// TAR 205214 TK - Fix Screen blinking again
	// Added the following if statement so that only go through this condition
	// if not in Bag & EAS since bAlreadyInSnB is set accordingly in the Bag & EAS
	if (csPreScreenState.Find(_T("BagAndEAS"),1) == -1) 
	{ 
		if (	( csPreScreenState.Find(_T("ScanAndBag"),1) == -1 ) 
			&&	( csPreScreenState.Find(_T("InProgress"),1) == -1 ) 
			&&	( csPreScreenState.Find(_T("InSAProgress"),1) == -1 )   )
			SMStateBase::bAlreadyInSnB = false;
		else if (csPreScreenState.Find(_T("InProgress"),1) == -1 )
			SMStateBase::bAlreadyInSnB = true; 
	}
	// TAR 205214
	if ( csPreScreenState.Find(_T("SystemMessage5"),1) != -1)
		SMStateBase::bIsSysMsgScreenState = true; //tar 185972
	//fix screen blinking end
}
    //TAR195100 CHU022502
    //The problem of this tar is that during InProgress state,
    //we go to SystemMessage or WaitForApproval because of any device 
	//failure or Security event 31/30. While we are at SystemMessage or 
	//WaitforApproval state, TB sends A state back which will be pushed
	//onto statck. When we finish the SystemMessage or WaitForApproval, we
	//don't pop the stack at the right state.
    //The fix is that: 
    //We always go to SystemMessage state it doesn't matter what
	//state currently is at.
	//while we are on InProgress state,
	//if the next requested changing state is WaitForApproval, we save this
	//state and stay at InProgress. We will go back to WaitForApproval at 
	//ScanAndBag::Initialize() or DeactivateTheftTag::Initilialize().
    //While we are on SystemMessage state,
	//if the next requested changing state is SystemMessage, 
	//WaitForApproval state, or the state from TB
	//we save the state and will go to the saved state when 
	//ScanAndBag::initialize() is implemented.
	//While we are on DeactivateTheftTag state,
	//We want to stay at this state not interrupt by any coming WaitForApproval.
	//So we save coming WaitForApproval and go to this state at 
	//ScanAndBag::Initialize().
	//But when we get in DeactivateTheftTag, there is a SystemMessage or WaitForApproval
	//is waiting, we are going to that state. When we come back from this SystemMesssage
	//or WaitForApproval, we will go back to DeactivateTheftTag

bool MPProcedures::DelayOrIgnoreStateChangeCheck(BOOL &bPoppedSystemMessage, CString &csLastName, CString &csNextStateName, SMStateBase  *sTemp)
{
	if ( sNext != STATE_STOP ) //CHU TAR187418: Added this line
	  //CHU TAR187418 Added this line

	//TAR195100-

    {
		if (!bRAPApprovalMsg)	// RAP didn't send an approval message
		{
			
			//We are at InProgress
			if( ( csLastName.Find(_T("InProgress")) != -1 )					|| 
				( csLastName.Find(_T("WaitForRemoteAssistance")) != -1 )	||
				( csLastName.Find(_T("ContextHelp")) != -1 )				||
				( csLastName.Find(_T("DataEntry")) != -1 )					||  //tar 252987
				( csLastName.Find(_T("HostOffLine")) != -1 )				||
				( csLastName.Find(_T("RequestSig")) !=-1 )					|| //TAR 374645
				( csLastName.Find(_T("AcknowledgeSig")) !=-1 )				||		//TAR 374645
                ( csLastName.Find(_T("VoidApproval")) != -1 )				||  //tar 410858
				( csLastName.Find(_T("VoidTransApproval")) != -1 )			||  //tar 410858
				( csLastName.Find(_T("LaneClosed")) != -1 )					)

			{
                if ( SetDelayWaitForApprovalState(csNextStateName, csLastName))
                {
                    if ((sTemp > STATE_ERROR) || (sTemp < 0)) // TAR 303487
                    {
                        delete sTemp;
						sTemp = STATE_NULL;
                    }
                    return true;
                }

			}

			// We are at SystemMessage
            if ( csLastName.Find(_T("SystemMessage")) != -1 )	
			{
				//TB event is next
				if (bTBMessage)
				{
					if ( SMStateBase::g_pDelay_TBExpectedStateChange != NULL )
					{  
						CString csStateName = SMStateBase::g_pDelay_TBExpectedStateChange->GetRuntimeClass()->m_lpszClassName;
						delete SMStateBase::g_pDelay_TBExpectedStateChange;
						SMStateBase::g_pDelay_TBExpectedStateChange = NULL;
						trace(L6,_T("Delete the state %s saved in g_pDelay_TBExpectedStateChange"),csStateName);
					}
					SMStateBase::g_pDelay_TBExpectedStateChange = sTemp;
					trace(L6,_T("We are at systemMessage, saving next state from TB %s to g_pDelay_TBExpectedStateChange"),csNextStateName);
					//stay at SystemMessage;
					return true; 			
				}
				//Next state is WaitForApproval or ...
				else if ( SetDelayWaitForApprovalState(csNextStateName, csLastName) && bInSystemMessage)
                {
                    if ((sTemp > STATE_ERROR) || (sTemp < 0)) // TAR 303487
                    {
                        delete sTemp;
						sTemp = STATE_NULL;
                    }
                    return true;
                }
				//Next state is SystemMessage
				else if (csNextStateName.Find(_T("SystemMessage")) != -1)
				{
			 	  if ((sTemp > STATE_ERROR) || (sTemp < 0)) // TAR 303487
				  {
                    // TAR 223705:  Only Push on system message stack if the
                    //              message was not just popped off the stack
                    if ( ! sTemp->GetStatePopped() )
                    {
						// push state onto stack, unless we're in remote AM
						
						if (!ps.RemoteMode())  //Tar229577
						{
							sCurr->PushStateFifoStack(sTemp);	
							trace(L6,_T("Pushing state %s onto stack"),csNextStateName);
						}
						//stay at SystemMessage;
						return true; 			
                    }
                    else
                    {
                        bPoppedSystemMessage = true;
                    }
				  }
			    }
			}			
			sCurr->ResetMLBCValues(); //RFC 241710	

            
			//If the current state is WaitForApproval and the next state is WaitForApproval,
			//ignore the next state and go back to the current
			if (   (csLastName.Find(_T("WaitForApproval")) !=-1 ) 
				&& (csNextStateName.Find(_T("WaitForApproval")) !=-1 ) )
			{
				//On WaitForApproval, ignore this coming WaitForApproval
				trace(L6,_T("We are at %s, Not saving next state WaitForApproval. Already has one"),csLastName);
				//stay at this WaitForApproval
				if ((sTemp > STATE_ERROR) || (sTemp < 0)) // TAR 303487
				{
				    delete sTemp;
					sTemp = STATE_NULL;
				}
				return true; 			
			}
			

			//+TAR 341426 When we are in following states, delay states from the TB, save it in g_pDelay_TBExpectedStateChange
			if(((csLastName.Find(_T("WaitForApproval")) !=-1 ) 
				&& !co.IsInAssistMenus() && !co.IsAttendantModeOn() )
				||(csLastName.Find(_T("SecBagAreaBackup")) !=-1 )
				||(csLastName.Find(_T("SecBagBoxThreshold")) !=-1 )
				||(csLastName.Find(_T("SecItemRemovedThreshold")) !=-1 )
				||(csLastName.Find(_T("SecMisMatchWeight")) !=-1 )
				||(csLastName.Find(_T("SecNewItemEntry")) !=-1 )
				||(csLastName.Find(_T("SecSkipBaggingThreshold")) !=-1 )
				||(csLastName.Find(_T("SecUnexpectedDecrease")) !=-1 )
				||(csLastName.Find(_T("SecUnExpectedIncrease")) !=-1 )
				||(csLastName.Find(_T("PickingUpItems")) !=-1 )	
				||(csLastName.Find(_T("UnDeActivatedItemApproval")) !=-1 )
				||(csLastName.Find(_T("SecBagBoxNotValid")) !=-1 )//SR085
			   )
			{
				//event was from TB
				if (bTBMessage  && sCurr->IgnoreTBMessageInSecurityState())
				{
					if ( ( sTemp > STATE_ERROR) || (sTemp < 0 ) ) 
					{
						if ( SMStateBase::g_pDelay_TBExpectedStateChange != NULL )
						{  
							CString csStateName = SMStateBase::g_pDelay_TBExpectedStateChange->GetRuntimeClass()->m_lpszClassName;
							delete SMStateBase::g_pDelay_TBExpectedStateChange;
							SMStateBase::g_pDelay_TBExpectedStateChange = NULL;
							trace(L6,_T("Delete the state %s saved in g_pDelay_TBExpectedStateChange"),csStateName);
						}
						
						SMStateBase::g_pDelay_TBExpectedStateChange = sTemp;
						trace(L6,_T("We are at %s, Saving next state %s in g_pDelay_TBExpectedStateChange."), csLastName, csNextStateName);
						
					}
					
					else
					{
						trace(L6,_T("Numeric state(%d); not saving in g_pDelay_TBExpectedStateChange."), sTemp);
					}
					//stay at current state
					return true;
				}
			}
			//-TAR 341426
		}

	}
    return false;
}

void MPProcedures::ProcessDelayedApproval(CString &csLastName, CString &csNextStateName, SMStateBase  *sTemp)
{

	if(   ( csLastName.Find(_T("InProgress")) !=-1 )
		&& ( SMStateBase::bIsInFinalization || SMStateBase::bQuietNeedSecApproval)
		&& ( csNextStateName.Find(_T("InProgress")) == -1 ) 
		&& SMStateBase::g_pDelay_WaitForApprovalStateChange != NULL
		//tar 207510  
		&& (sTemp != STATE_STOP && sTemp != STATE_RESTART))
		//tar 207510
	{
		trace(L6,_T("We want to go to saved WaitForApproval after InProgress:UnInitialize"));
		//save sNext onto pgDelay_TBExpectedStateChange
		SMStateBase::g_pDelay_TBExpectedStateChange = sTemp;
		//go to WaitForApproval
		sNext = SMStateBase::g_pDelay_WaitForApprovalStateChange;
		SMStateBase::g_pDelay_WaitForApprovalStateChange = NULL;
		
	}
	
}

int MPProcedures::ProcessExceptionStatePointers(BOOL &fRestart)
{
    if ( sNext == STATE_ENDED )
    {
      ScotError(INFO,SM_CAT_STOP_ABORT, SCOTAPP_EXIT);

      delete sCurr;
      sCurr = STATE_NULL;

	  try  //CM/CH082302 - add try catch
	  {
		 lGLOBALStackCnt = 0;  // do our own counting just to be sure
		 helpStateStack.deleteAll();
		 trace(L2,_T("HelpStack.deleteAll - number states stacked = %d"), lGLOBALStackCnt);
	  }
	  catch ( CMemoryException *e)
	  {
		 trace(L2,_T("HelpStack.deleteAll mem exception Caught - number states stacked = %d"), lGLOBALStackCnt);
		 e->Delete();
	  } //CM/CH082302 end - add try catch

      
      sCurr = STATE_NULL;

      if (fRestart)
        return -2;
      return -1;
    }
     
    if ( sNext == STATE_ERROR )
    {
      ScotError(INFO,SM_CAT_CONFIGURATION, SCOTAPP_STATE);

      CREATE_AND_ASSIGN(OutOfService, sNext)(3);
      CheckStatePointer(sNext);
    }
     
    if ( sNext == STATE_STOP )
    {
      ScotError(INFO,SM_CAT_CONFIGURATION, SCOTAPP_STOP_REQUEST);

      CREATE_AND_ASSIGN(OutOfService, sNext)(4);
      CheckStatePointer(sNext);
    }
     
    if ( sNext == STATE_RESTART )
    {


      ScotError(INFO,SM_CAT_STARTUP_INIT, SCOTAPP_START_REQUEST);

      CREATE_AND_ASSIGN(OutOfService, sNext)(5);
      CheckStatePointer(sNext);
      fRestart = true;
    }
    return 0;
         
}

void MPProcedures::ProcessReturnResumeStates(SMStateBase  *sTemp)
{
    // only the HelpMode states save a return state so only
    // they should use STATE_RETURN or STATE_RESUME
    // if any other state tries to use STATE_RETURN
    // return to base state
    trace(L6,_T("STATE_RETURN or STATE_RESUME "));

    SMStateBase  *sTemp2;
	sTemp2 = STATE_NULL;

    //CM/CH082302 - pop only if we say to						
    trace(L2,_T("HelpStack.pop 1 BEFORE - number states stacked = %d"), lGLOBALStackCnt);
    try
    {
		if (lGLOBALStackCnt > 0)
		{
			sTemp2 = helpStateStack.pop();
			lGLOBALStackCnt--;
			trace(L2,_T("HelpStack.pop 1 AFTER  - number states stacked = %d"), lGLOBALStackCnt);
		}
		else
		{
			trace(L2,_T("HelpStack.pop 1 AFTER  - NOTHING TO POP - number states stacked = %d"), lGLOBALStackCnt);
		
		//if (lGLOBALStackCnt  // orig (sTemp2 = helpStateStack.pop()) == 0)
		//
				//trace(L6,"HelpStateStack Empty");
			 sTemp2 = SMStateBase ::createAnchorState(); // default to anchor state
		}
	}
	catch ( CMemoryException *e)
	{
		 trace(L2,_T("HelpStack.pop 1 mem exception Caught - number states stacked = %d"), lGLOBALStackCnt);
		 e->Delete();
	} //CM/CH082302 end - add try catch
   
    // check state
    CheckStatePointer(sTemp2);

    // if RETURNing and this is the InProgress state
    // pop another state 
	// ------------------------------------------------------
	// In other words - throw away the InProgress CM/CH082302
	// DOESN'T THIS MEAN THAT WE SHOULD THROW AWAY SAInProgress
	// and other like states????
	// ------------------------------------------------------
    if ( sNext==STATE_RETURN && sTemp2->inProgress() )
    {
		trace(L6,_T("STATE_RETURN and this is the InProgress state"));

        delete sTemp2;
		sTemp2 = STATE_NULL;

		 //CM/CH082302 - pop only if we say to						
		 try
		 {
			 trace(L2,_T("HelpStack.pop 2 BEFORE - number states stacked = %d"), lGLOBALStackCnt);
			 if (lGLOBALStackCnt > 0)
			{
				sTemp2 = helpStateStack.pop();
				lGLOBALStackCnt--;
				trace(L2,_T("HelpStack.pop 2 AFTER  - number states stacked = %d"), lGLOBALStackCnt);
			}
			else
			{
				trace(L2,_T("HelpStack.pop 2 AFTER  - NOTHING TO POP - number states stacked = %d"), lGLOBALStackCnt);
				sTemp2 = SMStateBase ::createAnchorState(); // default to anchor state
			}
		}
	 	catch ( CMemoryException *e)
		{
			 trace(L2,_T("HelpStack.pop 2 mem exception Caught - number states stacked = %d"), lGLOBALStackCnt);
			 e->Delete();
		} //CM/CH082302 end - add try catch
		 /* - orig // CM/CH082302
       if ((sTemp2 = helpStateStack.pop()) == 0)
       {
							trace(L6,"HelpStateStack Empty");
          sTemp2 = SMStateBase ::createAnchorState();        // default to anchor state
       } - */
       // check state
       CheckStatePointer(sTemp2);
    }

	// START: TAR:242837
	if ((sTemp > STATE_ERROR) || (sTemp < 0)) // TAR 303487
	{
		delete sTemp;
		sTemp = STATE_NULL;
	}
	// END: TAR:242837
    // delete old state and get pop a previous state off the stack
    delete sCurr;
    sCurr = sTemp2;
}

void MPProcedures::ProcessNewState(BOOL &bPoppedSystemMessage, CString &csLastName)
{
    trace(L6,_T("About to change state, not the same state"));
    CheckStatePointer(sNext);
    if ( sNext->helpMode() )
    {
        // going into help, stack up current state
		//trace(L6,"Going into help, stack up current state %s",csLastName);
							
		//CM/CH082302 - pop only if we say to						
        // TAR 223705: Do not push this state if the next state was
        //             popped off system message stack
        if ( ! bPoppedSystemMessage )
        {
            trace(L2,_T("HelpStack.push 1 BEFORE - number states stacked = %d"), lGLOBALStackCnt);
            helpStateStack.push(sCurr);
            lGLOBALStackCnt++;
            trace(L2,_T("HelpStack.push 1 AFTER  - number states stacked = %d"), lGLOBALStackCnt);
        }
    }
    else
    {
        // out of help mode so empty stack of help states
        try
        {
            delete sCurr;
			sCurr = STATE_NULL;
        }
        catch( CMemoryException *e) //was  (...)  - only s/b MemExcept CM/CH082302
        {
            trace(L0,_T("Problem while deleting:%s state pointer"), csLastName);
            e->Delete();
        }
        trace(L6,_T("+Empty stack of help states"));

		try  //CM/CH082302 - add try catch
		{
			lGLOBALStackCnt = 0; // do our own counting just to be sure
			helpStateStack.deleteAll();
			trace(L2,_T("HelpStack.deleteAll 2 - number states stacked = %d"), lGLOBALStackCnt);
		}
		catch ( CMemoryException *e)
		{
			 trace(L2,_T("HelpStack.deleteAll 2 mem exception Caught - number states stacked = %d"), lGLOBALStackCnt);
			 e->Delete();
		} //CM/CH082302 end - add try catch
        //      helpStateStack.deleteAll();
		trace(L6,_T("-Empty stack of help states"));
        sCurr = STATE_NULL;
    }

    CheckStatePointer(sNext);
    sCurr = sNext;                // make next state current state
    CheckStatePointer(sCurr);
       trace(L4,_T("Changing state to %s, assistMenus=%d, attendantMode=%d"),
           getStateName(sCurr), co.IsInAssistMenus(), co.IsAttendantModeOn());

}

void MPProcedures::QueryForStateOverride(CString &csLastStateName, CString &csNextStateName)
{
    bool bOverride = false;
    LPTSTR lpOverrideStateName;
    CDescriptorList dlParameterList;
    CString csOverrideStateName;
    CObject *pOverrideState;
    bool bDeliveredOK = false;
    long lReturnCode;
    CRuntimeClass *pRuntimeClass = NULL;

	// if the state is one of these, we can't override 
	if ( sCurr==NULL        ||
        sCurr==STATE_STOP     ||
        sCurr==STATE_RETURN   ||
        sCurr==STATE_RESUME   ||
        sCurr==STATE_RESTART  ||
        sCurr==STATE_ENDED    ||
        sCurr==STATE_ERROR    ||
        sCurr==STATE_NULL)
    {
		return;
	}

	try
	{
		trace(L7,_T("MPProcedures::QueryForStateOverride 1") );

		lReturnCode = sCurr->TBGetNextState((LPCTSTR) csLastStateName, 
							(LPCTSTR) csNextStateName, 
							&bOverride,
							&lpOverrideStateName,
							dlParameterList);
		trace(L7,_T("MPProcedures::QueryForStateOverride 2") );
		// If TB Overriding state and call returned successfully
		if ((bOverride) && (!(lReturnCode < 0)))
		{
			csOverrideStateName = lpOverrideStateName;
			trace(L2,_T("State Overridden LS=[%s] NS=[%s] OS=[%s] PC=[%d]"), (LPCTSTR)csLastStateName, (LPCTSTR)csNextStateName, (LPCTSTR)csOverrideStateName, dlParameterList.GetDescriptorCount());
			trace(L7,_T("MPProcedures::QueryForStateOverride 3") );
			pRuntimeClass = FromName(lpOverrideStateName);
			trace(L7,_T("MPProcedures::QueryForStateOverride 4") );
			if (NULL != pRuntimeClass)
			{
				pOverrideState = pRuntimeClass->CreateObject();
				trace(L7,_T("MPProcedures::QueryForStateOverride 5") );
				bDeliveredOK = ((SMStateBase *)pOverrideState)->DeliverDescriptorList(dlParameterList);
				trace(L7,_T("MPProcedures::QueryForStateOverride 6") );
				if (!bDeliveredOK)
				{
					trace(L2, _T("FATAL ERROR!!! Error returned on Deliver."));
				}
			}
			else
			{
				trace(L2, _T("FATAL ERROR!!! Unable to instantiate override state"));
			}
			if (!bDeliveredOK)
			{
				trace(L7,_T("MPProcedures::QueryForStateOverride 7") );
				CREATE_AND_ASSIGN(FatalError, pOverrideState)();
				trace(L7,_T("MPProcedures::QueryForStateOverride 8") );
			}
			delete sCurr;
			sCurr = (SMStateBase *)pOverrideState;
		}
	}
	catch( ... )
	{
		// We saw an exception in this function twice during scotapp shutdown.
		// logging at this point could introduce an exception itself.  
	}
}
