//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmDataEntryBase.h
//
// TITLE: Class header for store mode DataEntry state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmDataEntryBaseSTATE
#define _SmDataEntryBaseSTATE

#ifndef _CPPUNIT
#include "SMStoreModeState.h"
#include "SMRAPDataNeeded.h"

//USSF START
#include "SMUSSFManager.h"
//USSF END

#else
#include "FakeSMStateBase.h"
#include "FakeSMOperatorPasswordStateBase.h"
#include "FakeSMRAPDataNeededBase.h"
#include "FakeCommon.h"

#endif //_CPPUNIT

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif

  SMSmDataEntryBase : public  STATE(RAPDataNeeded)

{
public:
  SMSmDataEntryBase();		     // constructor
  virtual SMStateBase * Initialize(void);        // required override
  virtual void UnInitialize(void);
#ifndef _CPPUNIT  
  virtual bool PSRemoteLegalForState();
  virtual bool storeMode(); // mm185096 TAR332574

protected:
//USSF Start
  friend SMUSSFManagerBase;
  friend SMUSSFManager;
//USSF End
#endif //_CPPUNIT 
  virtual SMStateBase  *PSButton1(void);         // override for button 1
  virtual SMStateBase  *PSButton2(void);         // override for button 2
#ifndef _CPPUNIT 
  virtual SMStateBase  *PSButton3(void);         // override for button 3
  virtual SMStateBase  *PSButton4(void);         // override for button 4
  virtual SMStateBase  *PSButton5(void);         // override for button 5
  virtual SMStateBase  *PSButton6(void);         // override for button 6
  virtual SMStateBase  *PSButton7(void);         // override for button 7
  virtual SMStateBase  *PSButton8(void);         //
  virtual SMStateBase  *PSNumericKey(const long);	// a key 0-9 was pressed
  virtual SMStateBase  *PSEnterKey(void);        // override for enter key
  virtual SMStateBase  *PSClearKey(void);        // override for clear key
  virtual SMStateBase  *PSDecimalKey(const long ch);
  virtual SMStateBase  *PSBackSpaceKey(void);
  virtual SMStateBase  *PSSpaceKey(void);
#endif //_CPPUNIT 
  virtual SMStateBase  *PSShiftKey(void);
  virtual SMStateBase  *PSAltGrKey(void);
  virtual SMStateBase  *PSHotKey(CString &);
  virtual SMStateBase  *PSCharKey(TCHAR c);

#ifndef _CPPUNIT 
  virtual SMStateBase  *PSAlphaKey(const long lKey);
  virtual SMStateBase  *PSAlphaNumericKey(const long lKey);
#endif //_CPPUNIT
  virtual SMStateBase  *DMScanner();             // scanned item
  virtual SMStateBase  *DMScannerEncrypted();   // SR 93.4
#ifndef _CPPUNIT 
  virtual SMStateBase  *DMCardReader(void);                       //RFQ 2605
  virtual SMStateBase  *OnTBNeedData(void);
  virtual SMStateBase  *HandleRAPData(void);
  //*** NewUI ***//
  virtual SMStateBase *PSReceiptUp(void);   
  virtual SMStateBase *PSReceiptDown(void);
  //*** end of NewUI ***//
  virtual void DisplayUserButtons(void);
  virtual void SetUserButton(int nIndex, int nPageOffset);
#endif //_CPPUNIT
  virtual CString GetPagedButtonCommand(int index);
  virtual CString GetButtonCommand(int nButtonIndex);
  CString csCurContext;    // Tar 238987
  int m_nBtnPageIndex;
  int m_nNumButtons;
  bool m_bPageButtons;
  enum IPSProcedures::HKStatus m_HKStatus;
  bool m_bShiftOn;
  bool m_bAltGrOn; //SR828
  bool m_bCapsInput;
#ifndef _CPPUNIT 
  virtual SMStateBase *PSOnRemoteDisconnected(const long lParm);
#endif

    virtual SMStateBase  *PSButtonGoBack(void);        // override for enter key

protected:
  virtual SMStateBase *PSCmdButton(CString, CString, CString);
#ifndef _CPPUNIT 
  virtual SMStateBase *OnRAPUnsolicitedDataReceived(LPCTSTR sData); //SSCOP-3803

  
  virtual SMStateBase  *stateAfterPasswordVerify(); //CRD 321191

private:
	static bool ms_bRAPHasScanned; //SSCOP-3803
#endif //_CPPUNIT
    static bool m_bPickListAssistHandler; //SSCOADK-1000
#ifndef _CPPUNIT 
  DECLARE_DYNCREATE(SMSmDataEntryBase)// MFC Runtime class/object information
#endif //_CPPUNIT
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSmDataEntry(void);
// or
//    DLLEXPORT SMStateBase *makeSMSmDataEntryBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSmDataEntry objects.  This function is called by
// RETURNSTATE(SmDataEntry) instead of calling new STATE(SmDataEntry) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSmDataEntryBase.cpp and SMSmDataEntry.cpp
// by using the macro IMPLEMENT_STATE(SmDataEntry)

//DECLARE_STATE(SmDataEntry)

#endif
