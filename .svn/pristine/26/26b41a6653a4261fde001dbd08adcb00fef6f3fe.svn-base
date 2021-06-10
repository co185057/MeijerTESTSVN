//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMScanAndBagBase.h
//
// TITLE: Class header for Scan and Bag State
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SCANANDBAGBASESTATE
#define _SCANANDBAGBASESTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
SMScanAndBagBase : public SMCustomerModeState
{
public:
	SMScanAndBagBase();
  virtual SMStateBase *Initialize(void);
  virtual void     UnInitialize(void);
  virtual SMStateBase  *OnWtDecrease(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);	// RFC 213876
  virtual SMStateBase  *OnMatchedWt(void);				// Smart Scale reported a matched weight
  static int ms_nNumberOfItems; 
  virtual bool PSAssistMenuLegalForState() { return true; };
  long m_TimerResponse;						//RFC 276235

protected:
  virtual SMStateBase *PSButton1(void);
  virtual SMStateBase *PSButton2(void);
  virtual SMStateBase *PSButton3(void);
  virtual SMStateBase *PSButton4(void);
  virtual SMStateBase *PSButton5(void);
  virtual SMStateBase *DMScanner(void);
  virtual SMStateBase *DMCardReader(void);
  virtual SMStateBase *DMAcceptor(void);
  virtual SMStateBase *DMScale(void);
  virtual SMStateBase  *PSButtonHelp(void);

  virtual SMStateBase* DMTakeawayBagAreaBackup(bool isBackedUp);  // takeaway belt event

  // allow user to key in item code in this screen
  virtual SMStateBase *PSNumericKey(const long);
  virtual SMStateBase *PSEnterKey(void);
  virtual SMStateBase *PSClearKey(void);
  virtual void InitializeBasedOnOption();  // Set configured button and window frame 
  virtual void FlashButtonsOnWeight(bool);      // Start or stop flashing Look Up Item and/or Key In code button
  virtual SMStateBase *TimedOut(void);     
  static CString csCurContext;    // Tar 238987
  static bool bXmlBtnVisibility; //TAR 279197
  DECLARE_DYNCREATE(SMScanAndBagBase)// MFC Runtime class/object information
  static VOID CALLBACK ScanScaleSBTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);	// RFC276235
  void ScanScaleSB(void);//RFC 276235
  virtual void ChangeLanguage(SCOT_LANGUAGE_TYPE langNew);
  virtual SMStateBase *OnLookupComplete (long byteCount);
  virtual SMStateBase  *PSButton7(void); //4.2 provide subtle on-screen volume control
  virtual SMStateBase  *PSListLookup(CString);

  // +TAR 399985
  virtual long DMSayPhrase(const int nMsg);
  virtual long DMSaySecurity(const long nMsg, LPCTSTR szWave, HANDLE hWaitEvent =0);
  // -TAR 399985
  
  virtual void TBGetItemDetails(ItemDetails&); //407959

private:
	bool fSayPurProd;
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMScanAndBag(void);
// or
//    DLLEXPORT SMStateBase *makeSMScanAndBagBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMScanAndBag objects.  This function is called by
// RETURNSTATE(ScanAndBag) instead of calling new STATE(ScanAndBag) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMScanAndBagBase.cpp and SMScanAndBag.cpp
// by using the macro IMPLEMENT_STATE(ScanAndBag)

//DECLARE_STATE(ScanAndBag)

#endif
