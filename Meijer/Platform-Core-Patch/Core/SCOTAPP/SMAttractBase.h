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

#include "SMCustomerModeState.h"


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
SMAttractBase : public SMCustomerModeState
{
public:
  SMAttractBase();                      // constructor
  virtual SMStateBase  *Initialize(void);        // required override
  virtual void UnInitialize(void);          // optional
  virtual SMStateBase  *SAEndOfTransaction(void);   //TAR#118659	LPM092899  coded to use the SA 16100 event
  virtual SMStateBase  *OnWtDecrease(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(); //aiqin
  static int ms_nNumberOfAutoRunTrans; 
  static long ms_nAutoRunTransactionTally;
  long m_TimerResponse;						//RFC 276235
protected:
  virtual SMStateBase  *TimedOut(void);          // optional
  virtual SMStateBase  *PSButton1(void);       
  virtual SMStateBase  *PSButton2(void);       
  virtual SMStateBase  *PSButton3(void);         
  virtual SMStateBase  *PSButton4(void);         
  virtual SMStateBase  *PSButton5(void);         
  virtual SMStateBase  *PSButton6(void);         
  virtual SMStateBase  *PSButton8(void); 
  virtual SMStateBase  *DMScanner(void);         // override for DM scanner
  virtual SMStateBase  *DMCardReader(void);      // override for card reader
  virtual SMStateBase  *DMScale(void);
  virtual SMStateBase  *PSButtonHelp(void);       // NewUI
  virtual SMStateBase  *DMTakeawayBagAreaBackup(bool isBackedUp);  // takeaway belt event
  virtual void PaintScanAndBagScreen();  // TAR 262131
  static VOID CALLBACK ScanScaleAttractTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);	// RFC276235
  void ScanScaleAttract(void);//RFC 276235
  CString csCurContext;    // Tar 238987
  static VOID CALLBACK AttendantModeTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
  static SMAttractBase* m_pAttract;
  virtual void HandleAttendantModeTimedOut();
  //+4.2 provide subtle on-screen volume control
  virtual SMStateBase  *PSButton7(void); 
  virtual void DisplayHopperSubstitution(void);
  static VOID CALLBACK VolumeControlTimedOut ( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
  static VOID CALLBACK BackToPrimaryLanguageTimedOut ( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
  void HandleVolumeControlTimedOut();
  void HandleBackToPrimaryLanguageTimedOut();
  //-4.2 provide subtle on-screen volume control 	

//private:
  bool	bGoingToInProgress;   // going to this state next?
  bool  bInitializeToTimeOut;   //From Attract::Initialize() to TimeOut()
  bool  m_bLangChange;         //Last language was not the system default - TAR 262131
  bool  m_bLanguageButtonClicked;

  virtual SMStateBase * transition(TRANSITION_EVENT event, SCOT_LANGUAGE_TYPE languageUsed);  // where to proceed from here
	
  DECLARE_DYNCREATE(SMAttractBase)// MFC Runtime class/object information
};

#endif
