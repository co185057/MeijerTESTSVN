//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmAssistModeBase.h
//
// TITLE: Class header for Assist mode keyboard entry mode
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMSmAssistModeBase
#define _SMSmAssistModeBase

#include "SMStoreModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSmAssistModeBase : public STATE(StoreModeState)
{
public:
	SMSmAssistModeBase();
   ~SMSmAssistModeBase();
  virtual SMStateBase  *Initialize();
  virtual void     UnInitialize();
  virtual bool PSRemoteLegalForState();
  virtual SMStateBase  *TimedOut(void);
  virtual bool	 assistMode()	  { return true; }
  virtual SMStateBase  *DMRemoteScannerConnect(bool bConnect);
	
protected:
  virtual SMStateBase  *PSButton1(void);				  // override for button 1
  virtual SMStateBase  *PSButton2(void);				  // override for button 2
  virtual SMStateBase  *PSAssistKey(const long); // override for 50 keys
  virtual SMStateBase  *PSNumericKey(const long);// a key 0-9 was pressed
  virtual SMStateBase  *PSEnterKey();            // override for enter key
  virtual SMStateBase  *PSClearKey();            // override for clear key
  virtual SMStateBase  *PSCancelKey();           // override for cancel key
  virtual SMStateBase  *DMScanner();             // scanned item
  virtual SMStateBase  *DMCardReader(void);      // MSR input
  virtual SMStateBase  *DMScale();				 // scanned item
  virtual SMStateBase  *SAEndOfTransaction();
  virtual SMStateBase  *DMAcceptor(void) { return STATE_NULL;} ;	
//  virtual SMStateBase *PSOnRemoteDisconnected(const long lParm);
   virtual SMStateBase* DMMICR(DWORD dwEvent);
   virtual SMStateBase* OnNeedMICR();
   virtual SMStateBase* DMSlipPrinter(DWORD dwEvent);
   virtual SMStateBase* OnEndorseDocument(void);
   virtual SMStateBase* OnNewCashDrawerState(void);
   virtual SMStateBase *PSKeystroke(const long wParam, const long lParam);
   virtual TBRC  TBKeyPress(const USHORT uKey);
   virtual SMStateBase *PSAlphaKey(const long lKeystroke);
   virtual SMStateBase *OnAssistModeStatusChanged(void);
   virtual SMStateBase *PSButtonToggle(void);
   virtual SMStateBase *PSButtonLaneStatus(long lLaneIndex);
   virtual SMStateBase *OnRAPUnsolicitedDataReceived(LPCTSTR sData);
   virtual SMStateBase *PSRemoteButton1(void);
   virtual SMStateBase *DMTakeawayAMExitDone(void);
   virtual void TABExitingAssistMode(void);	 // Tar 218330 TAB in Assist Mode
   virtual SMStateBase *HandleNewContext();  // Rel 3.2 Requirement 4.5.1 - Single Mode RAP
   virtual SMStateBase *AutoRunTimeOut(void); 
   virtual SMStateBase *TBEndOfTransaction(void);
   virtual CString ExtractString(CString strData, CString sStringName);
   virtual SMStateBase *PSConnectRemote(CString&, CString&);				// Tar 328198 Mb185050
   static const int ASSISTMODE_RETURN_TIMEOUT;

   CString csNumData;
   long lSlashData;
   long lTareCode;
   long lWeightEntered;
   long lQuantityEntered;
   long lUnitPriceEntered;
   long lExtPriceEntered;       //Tar 137876 add extended price
   bool fVoided;
   bool fWaitingForEOT;
   bool fWaitingToExit;
   bool fItemizing;
   bool fTendering;
   bool bSetMonitorFalse;
   bool fWaitingOnBelt;
   bool m_bToggled; //272625


   virtual long     DoKeyCode(UCHAR uKey);
   virtual SMStateBase  *IssueChange(void);

protected:
   LPTSTR m_szSlipDocumentDescription;
   LPTSTR m_szSlipDocumentFront;
   LPTSTR m_szSlipDocumentBack;
   BOOL m_bWaitingForMICRInsertion;
   bool m_bMultipleEndorsements;
   long m_nEndorsementsPending;
   long m_nMICREvents;            // MICR requests can change endorsement document handling.
   SMStateBase* sReturnState;   // Tar 213624 CHU 041503 
   CString csCurContext;    // Tar 238987
   bool m_bPartialAssistEnabled;

   DECLARE_COPY_INHIBIT(SMSmAssistModeBase);
   DECLARE_DYNCREATE(SMSmAssistModeBase)// MFC Runtime class/object information

private:
    //private:
  int  retryReceiptAttempts;
};

//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSmAssistMode(void);
// or
//    DLLEXPORT SMStateBase *makeSMSmAssistModeBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSmAssistMode objects.  This function is called by
// RETURNSTATE(SmAssistMode) instead of calling new STATE(SmAssistMode) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSmAssistModeBase.cpp and SMSmAssistMode.cpp
// by using the macro IMPLEMENT_STATE(SmAssistMode)


//DECLARE_STATE(SmAssistMode)

#endif
