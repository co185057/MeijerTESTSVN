//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStateSA.h
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMSTATERA
#define _SMSTATERA

protected:
    virtual long RAInitialize(void);
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
    virtual SMStateBase *HandleStartTrainingTrans();
    virtual void HandleHopperError(CString csBttID);
    virtual void EnableHandheldAssistMode(bool bEnable);
    virtual void ClearHopperSubstitution(void);
    virtual CString ExtractString(CString strData, CString sStringName);
    virtual SMStateBase *HandleMaxCouponLimit();
    virtual SMStateBase *HandleMaxCouponLimitFinalization();
    virtual SMStateBase *HandleInvalidBarcode();
    virtual SMStateBase *HandleRemoteGenericDelayed(CString csData);

#endif