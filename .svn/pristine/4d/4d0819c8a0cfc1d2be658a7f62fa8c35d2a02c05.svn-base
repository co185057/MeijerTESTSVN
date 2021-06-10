//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStateRABase.h
//
//
// AUTHOR:    Ying Xie
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMSTATERABASE
#define _SMSTATERABASE

protected:
#ifndef _CPPUNIT
  // start up
  virtual long RAInitialize(void);
  // shut down
  virtual long RAUnInitialize(void);

  virtual SMStateBase *RedoSigCap();
  virtual SMStateBase *ApproveSigCap();
#endif
  virtual SMStateBase *RAParse(MessageElement *me);
 
#ifndef _CPPUNIT
  virtual SMStateBase *OnRAPDataReceived(LPCTSTR sData);
  virtual SMStateBase *HandleRAPData();
#endif //_CPPUNIT
  virtual SMStateBase *OnRAPUnsolicitedDataReceived(LPCTSTR sData);
#ifndef _CPPUNIT
  virtual SMStateBase *HandleApproveVoidedItem();
  virtual SMStateBase *HandleConfirmSuspend();
  virtual SMStateBase *HandlePricePrompt();
  virtual SMStateBase *HandleCancelTrasaction(CString csBttID);
#endif //_CPPUNIT
  virtual SMStateBase *ReturnToWaitForApprovalState();
#ifndef _CPPUNIT
  virtual SMStateBase *HandleStartTransaction();
#endif //_CPPUNIT
  virtual SMStateBase *HandleStartTrainingTrans(); // Handle Training Mode Transaction intervention
#ifndef _CPPUNIT
  virtual void HandleHopperError(CString csBttID);
  virtual void EnableHandheldAssistMode(bool bEnable);
  virtual void ClearHopperSubstitution(void);
  virtual SMStateBase *HandleMaxCouponLimit();			//SR678
  virtual SMStateBase *HandleMaxCouponLimitFinalization();		//SR678
  virtual SMStateBase *HandleInvalidBarcode(); //SR879
#endif
  virtual CString ExtractString(CString strData, CString sStringName);
  virtual SMStateBase *HandleRemoteGenericDelayed(CString csData);

public:
  //+dp185016 
  static bool m_bClearStack;
  //-dp185016
  bool m_bEncryptedBarcodeAtSignOn; //SR93.4


#endif
