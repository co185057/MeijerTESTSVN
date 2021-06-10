    //////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // FILE:  MPProcedures.h
    //
    // TITLE:   Header file for Main Process thread function.
    //
    //
    // AUTHOR:  Peter Denhoed
    //
    //////////////////////////////////////////////////////////////////////////////////////////////////
     
    #ifndef _MPPROCEDURES
    #define _MPPROCEDURES

    #include <stdafx.h>                 // MS MFC always needed first
    ////               // MS MFC template header
    #include <afxmt.h>                  // MS MFC template header
    #include <afxctl.h>
    
    #include "Library.h"
    
    #define MP_STOP 101
    
    ///////////////////////////////////
    //
    // MPProcedures
    //
    ///////////////////////////////////
#include "DllDefine.h"
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
   MPProcedures : public CObject
    {

    public:
      MPProcedures();
      SMStateBase  *sCurr;                   // these need NOT be globals Mgg
    	
      long Initialize(void);            // called by windows proper
      long UnInitialize(void);          // called by w p
      long Process(MessageElement *);	// message processer
      long IdleTimer(const long);       // time out timer, time in msec

      // +SSCOADK-4305: For monitoring mobile project
	  BOOL OpenTransLogFile( BOOL);  // Inserted boolean argument per TAR 419719
      void CloseTransLogFile();
      void ArchiveTransLogFile();
      void LogTransition(CString& stateName, TBSTATE lastTBState);
      // -SSCOADK-4305 

	  bool bTBEventIsInterrupting;		// stack TB events if on WaitForApproval screen
	  static CRuntimeClass* PASCAL FromName(LPCSTR lpszClassName);
	  static CRuntimeClass* PASCAL FromName(LPCWSTR lpszClassName);
    
    private:
	    bool SetDelayWaitForApprovalState(CString &csNextStateName, CString &csLastName);
      SMStateBase  *sNext;									  // not a global, set to STATE_NULL
      PointerStack<SMStateBase *> helpStateStack; // save previous help states
    	
      long    ProcessNextState(void);
      CString getStateName(const SMStateBase * const pState);
      void RemoteStateProcessing();
      void AssistMenuStateProcessing();
      bool StateFrozen();
      void VideoStateCheck(CString csLastStateName);
      bool DelayOrIgnoreStateChangeCheck(BOOL &bPoppedSystemMessage, CString &csLastName, CString &csNextStateName, SMStateBase  *sTemp);
      void ProcessDelayedApproval(CString &csLastName, CString &csNextStateName, SMStateBase  *sTemp);
      int ProcessExceptionStatePointers(BOOL &fRestart);
      void ProcessReturnResumeStates(SMStateBase  *sTemp);
      void ProcessNewState(BOOL &bPoppedSystemMessage, CString &csLastName);
      void QueryForStateOverride(CString &csLastStateName, CString &csNextStateName);


      // Stuff for State Transition Log - 10/06/99
      // Due to time contstraints (i.e. I have to do this tonight!)
      // This will be pretty crude, hard coded name, size, etc.

      CString m_TransLogName;
      ULONGLONG m_MaxTransLogSize; // Changed type per TAR 419719
      CFile m_pTransLogFile; // Changed to a CFile object per TAR 419719
      CString m_sTransitionBackup; // Added per TAR 419719
      CEvent m_eventStartScot;
      CEvent m_eventEndScot;
     
    };
    
    // user exits not called from SMStateBase  derived states
    
    //////////////////////////////////////////////////////////////////////////////////////////////////
    // additional TS SerialMsg Message analysis for user mods or features (before)
    // NOTE: Not part of SMStateBase , runs under TSThread
    long UserExit09(const int lSerMsgIn, // length of serial data being passed in
                    LPCTSTR cSerMsgIn);  // binary serial request message from TS
    
    //////////////////////////////////////////////////////////////////////////////////////////////////
    // additional TS SerialMsg Message analysis for user mods or features (after)
    // NOTE: Not part of SMStateBase , runs under TSThread
    long UserExit10(int  &lSerMsgOut,   // length of serial data being passed back, or 0
                    LPTSTR &cSerMsgOut,  // optional binary serial response message returned or NULL
                    LPTSTR &cEncKeyOut); // optional ascii working key returned or null string ""
    #endif
