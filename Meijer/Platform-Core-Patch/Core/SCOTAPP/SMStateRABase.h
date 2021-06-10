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
  // start up
  virtual long RAInitialize(void);
  // shut down
  virtual long RAUnInitialize(void);

  virtual SMStateBase *RedoSigCap();
  virtual SMStateBase *ApproveSigCap();

  virtual SMStateBase *RAParse(MessageElement *me);
  virtual SMStateBase *OnRAPDataReceived(LPCTSTR sData);
  virtual SMStateBase *HandleRAPData();
  virtual SMStateBase *OnRAPUnsolicitedDataReceived(LPCTSTR sData);
  virtual SMStateBase *HandleApproveVoidedItem();
  virtual SMStateBase *HandleConfirmSuspend();
  virtual SMStateBase *HandlePricePrompt();
  virtual SMStateBase *HandleCancelTrasaction(CString csBttID);
  virtual SMStateBase *ReturnToWaitForApprovalState();
  virtual SMStateBase *HandleStartTransaction();
  virtual SMStateBase *HandleStartTrainingTrans(); // Handle Training Mode Transaction intervention
  virtual void HandleHopperError(CString csBttID);
  virtual void EnableHandheldAssistMode(bool bEnable);
  virtual void ClearHopperSubstitution(void);
  virtual CString ExtractString(CString strData, CString sStringName);
  virtual SMStateBase *HandleMaxCouponLimit();			//SR678
  virtual SMStateBase *HandleMaxCouponLimitFinalization();		//SR678

public:
  //+dp185016 
  static bool m_bClearStack;
  //-dp185016
#endif
