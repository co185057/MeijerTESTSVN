//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMAttractBase.h
//
// TITLE: Class header for AttractBase State
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ATTRACTBASESTATE
#define _ATTRACTBASESTATE

#ifdef _CPPUNIT
   #include "FakeSMCustomerModeState.h"
   #include "DMTriColorLightConstants.h"
#else
   #include "SMCustomerModeState.h"
#endif // _CPPUNIT


 enum TRANSITION_EVENT
{
  START_BUTTON,
  SCAN_ITEM,
  SWIPE_CARD
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMAttract(void);
// or
//    DLLEXPORT SMStateBase *makeSMAttractBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMAttract objects.  This function is called by
// RETURNSTATE(Attract) instead of calling new STATE(Attract) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMAttractBase.cpp and SMAttract.cpp
// by using the macro IMPLEMENT_STATE(Attract)

//DECLARE_STATE(Attract)



#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
SMAttractBase : public SMCUSTOMERMODESTATE
{
public:
  SMAttractBase();                      // constructor


  virtual SMStateBase  *Initialize(void);        // required override
#ifndef _CPPUNIT
  virtual void UnInitialize(void);          // optional
  virtual SMStateBase  *SAEndOfTransaction(void);   //TAR#118659    LPM092899  coded to use the SA 16100 event
  virtual SMStateBase  *OnWtDecrease(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(); //aiqin
  virtual SMStateBase *RAParse(MessageElement* ); //429111
  virtual SMStateBase *OnLookupComplete (long byteCount);
  virtual bool LoyaltyCheck(CString csLookupType, CString csLookupData);  //Make this virtual in the Base, please!
  static VOID CALLBACK LoyaltyCheckTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
  virtual void HandleLoyaltyCheckTimedOut();
#endif // _CPPUNIT
  virtual SMStateBase* ReloadOptions(void);

  static int ms_nNumberOfAutoRunTrans; 
  static long ms_nAutoRunTransactionTally;
  long m_TimerResponse;                     //RFC 276235
  static bool  m_bLanguageButtonClicked;  //Allow this to be set from SelectLanguage
  static bool m_bGiveChange;  //SSCOADK-1554
  static bool ms_bTransactionCancelled;


protected:
  virtual SMStateBase  *TimedOut(void);          // optional
  virtual SMStateBase  *PSButton1(void);
#ifndef _CPPUNIT       
  virtual SMStateBase  *PSButton2(void);       
  virtual SMStateBase  *PSButton3(void);         
  virtual SMStateBase  *PSButton4(void);         
  virtual SMStateBase  *PSButton5(void);         
  virtual SMStateBase  *PSButton6(void);         
  virtual SMStateBase  *PSButton8(void); 
  virtual SMStateBase  *DMScanner(void);         // override for DM scanner
  virtual SMStateBase  *DMCardReader(void);      // override for card reader
#endif //_CPPUNIT
  virtual SMStateBase  *DMScale(void);
#ifndef _CPPUNIT
  virtual SMStateBase  *PSButtonHelp(void);       // NewUI
  virtual SMStateBase  *DMTakeawayBagAreaBackup(bool isBackedUp);  // takeaway belt event
#endif // _CPPUNIT
  virtual void PaintScanAndBagScreen();  // TAR 262131
#ifndef _CPPUNIT
  static VOID CALLBACK ScanScaleAttractTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime); // RFC276235
  void ScanScaleAttract(void);//RFC 276235
#endif // _CPPUNIT
  
  virtual SMStateBase *OnMobileTransactionStart(CString csTransactionInfo);
  virtual SMStateBase *CUParse(MessageElement *me);

  CString csCurContext;    // Tar 238987

#ifndef _CPPUNIT
  static VOID CALLBACK AttendantModeTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
#endif // _CPPUNIT

  static SMAttractBase* m_pAttract;

  virtual void HandleAttendantModeTimedOut();
  //+4.2 provide subtle on-screen volume control
  virtual SMStateBase  *PSButton7(void); 
#ifndef _CPPUNIT
  virtual void DisplayHopperSubstitution(void);
  static VOID CALLBACK VolumeControlTimedOut ( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
  static VOID CALLBACK BackToPrimaryLanguageTimedOut ( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
  void HandleVolumeControlTimedOut();
  virtual void HandleBackToPrimaryLanguageTimedOut();//SSCOP-963
  //-4.2 provide subtle on-screen volume control    
  virtual void ReloadThemes(); // SSCOP-1158

      /**
     * Check device abnormal balance status.
     * An alert info to RAP is shown if a discrepancy 
     * error is detected.
     * \param[in] void 
     * \note Added for Glory CM Support
     */
    virtual void CheckCashDiscrepancyStatus(void);
#endif // _CPPUNIT

  virtual SMStateBase *PSCmdButton(CString, CString, CString);  //SSCOP-1136

//private:
  bool  bGoingToInProgress;   // going to this state next?
  bool  bInitializeToTimeOut;   //From Attract::Initialize() to TimeOut()
  bool  m_bLangChange;         //Last language was not the system default - TAR 262131

  static bool ms_bInitalizeIncomplete;
  static CString m_csLoyaltyCheckType;

  virtual SMStateBase * transition(TRANSITION_EVENT event, SCOT_LANGUAGE_TYPE languageUsed);  // where to proceed from here
// (+) SR898 ====================
public:
  virtual void DMTurnOffTriColorLight();
// (-) SR898 ====================


  DECLARE_DYNCREATE(SMAttractBase)// MFC Runtime class/object information
};

#endif
