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

#include "SMStoreModeState.h"
#include "SMRAPDataNeeded.h"
//USSF START
#include "SMUSSFManager.h"
//USSF END

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSmDataEntryBase : public STATE(RAPDataNeeded)
{
public:
          SMSmDataEntryBase();		     // constructor
  virtual SMStateBase * Initialize(void);        // required override
  virtual void UnInitialize(void);
  virtual bool PSRemoteLegalForState();
  virtual bool storeMode(); // mm185096 TAR332574
protected:
//USSF Start
  friend SMUSSFManagerBase;
  friend SMUSSFManager;
//USSF End
  virtual SMStateBase  *PSButton1(void);         // override for button 1
  virtual SMStateBase  *PSButton2(void);         // override for button 2
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
  virtual SMStateBase  *PSShiftKey(void);
  virtual SMStateBase  *PSAlphaKey(const long lKey);
  virtual SMStateBase  *PSAlphaNumericKey(const long lKey);
 //virtual SMStateBase  *PSCmdList(CString, CString, const long);  //RFQ 2605
  virtual SMStateBase  *DMScanner();             // scanned item
  virtual SMStateBase  *DMCardReader(void);                       //RFQ 2605
  virtual SMStateBase  *OnTBNeedData(void);
  virtual SMStateBase  *HandleRAPData(void);
  static bool savebEASReEnableScanner;			 // Tar 210186 RF 071902
  //*** NewUI ***//
  virtual SMStateBase *PSReceiptUp(void);   
  virtual SMStateBase *PSReceiptDown(void);
  //*** end of NewUI ***//
  virtual void DisplayUserButtons(void);
  virtual void SetUserButton(int nIndex, int nPageOffset);
  virtual CString GetPagedButtonCommand(int index);
  virtual CString GetButtonCommand(int nButtonIndex);
  CString csCurContext;    // Tar 238987
  int m_nBtnPageIndex;
  int m_nNumButtons;
  bool m_bPageButtons;
  bool m_bShiftOn;
  bool m_bCapsInput;
  virtual SMStateBase *PSOnRemoteDisconnected(const long lParm);
protected:
  virtual SMStateBase *PSCmdButton(CString, CString, CString);

  DECLARE_DYNCREATE(SMSmDataEntryBase)// MFC Runtime class/object information
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
