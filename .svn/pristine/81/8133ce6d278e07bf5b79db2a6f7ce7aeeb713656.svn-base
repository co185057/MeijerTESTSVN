//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	SMBagAndEasBase.h
//
// TITLE:	Class header for BagAndEas state
//
// AUTHOR:  Tara Duncan
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _BAGANDEASBASESTATE
#define _BAGANDEASBASESTATE
//BZ 06/06/02 RFC 203538 Begin: Change the base class to SMSecOperatorPWState
#ifdef _CPPUNIT
    #include "FakeSMSecOperatorPWStateBase.h"
#else
    #include "SMSecOperatorPWState.h"
#endif // _CPPUNIT
//RFC 203538 End

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMBagAndEASBase : public STATE(SecOperatorPWState)  //BZ 06/07/02 RFC 203538: Change base class to SMSecOperatorPWState
{
public:
#ifndef _CPPUNIT
  SMBagAndEASBase();
  virtual SMStateBase  *Initialize(void);
#endif
  virtual void UnInitialize(void);          // optional
#ifndef _CPPUNIT
  virtual bool PSAssistMenuLegalForState() { return true; };
  virtual bool storeMode(); //412874
  
  virtual SMStateBase *OnPLAHandInView();
#endif //_CPPUNIT
protected:
#ifndef _CPPUNIT
  virtual SMStateBase  *TimedOut(void);         // optional
#endif //_CPPUNIT
  static VOID CALLBACK EscalationTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
#ifndef _CPPUNIT
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
  virtual SMStateBase  *OnWtDecrease(void);
  virtual SMStateBase  *OnMatchedWt(void);				// Smart Scale reported a matched weight
  virtual SMStateBase  *DMdf_EASNoMotion();
  virtual SMStateBase  *OnBackToLGW(void);				// Smart Scale reported that the unmatched weight was removed
  //BZ 06/07/02 RFC 203538 Begin: Override following functions for Bag&EAS state
  virtual SMStateBase  *PSButtonGoBack(void);
  virtual void showScreen(bool passwordVerified);
  virtual SMStateBase  *stateAfterPasswordVerify();
  //RFC 203538 End
  virtual SMStateBase *DMScanner(void);	// TAR 274406
#endif //_CPPUNIT
  virtual void Escalation2RAP(void);
  virtual SMStateBase *GPNotify(const GPEVT gpEvt);
  virtual void ShowSkipBaggingOnEReceipt(void);	// RFC 252473
#ifndef _CPPUNIT
  virtual SMStateBase *DMTakeawayReadyForItem(void); // TAR 372391
#endif //_CPPUNIT

  CString m_sButton;       // String of button clicked by RAP operator
  int nCountForPeriodicBagItemAudio;
  CString csCurContext;    // Tar 238987
  bool m_bNeedToClearApproval; //TAR245215
  bool m_bLoginProcessing;     //TAR 274406
  static CString m_csExpectedWts;
  //229084 created a Base function for getNextStateForGoodItem LPM022503

#ifndef _CPPUNIT    
  //+SR778
  static VOID CALLBACK SkipBaggingButtonTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime); 
  virtual void HandleSkipBaggingButtonTimedOut(); 
  static SMBagAndEASBase* m_pBagAndEAS;		
  virtual void StartTimerForSkipBaggingBtn(); 
  //-SR778

  virtual SMStateBase  *SASecMgrParse( const MessageElement*);	//SR078
  virtual SMStateBase  *DMTakeawayReadyForNotRequiredBaggedItem(); //SR078
#endif //_CPPUNIT
  DECLARE_DYNCREATE(SMBagAndEASBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMDeactivateTheftTag(void);
// or
//    DLLEXPORT SMStateBase *makeSMDeactivateTheftTagBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMDeactivateTheftTag objects.  This function is called by
// RETURNSTATE(DeactivateTheftTag) instead of calling new STATE(DeactivateTheftTag) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMDeactivateTheftTagBase.cpp and SMDeactivateTheftTag.cpp
// by using the macro IMPLEMENT_STATE(DeactivateTheftTag)

//DECLARE_STATE(DeactivateTheftTag)

#endif
