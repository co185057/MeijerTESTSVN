//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStateSA.h
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMSTATESA
#define _SMSTATESA

public:
    virtual long SASendSCOTInput(long ID,LPCTSTR=_T(""),long=0,long=0,long=0,long=0,bool=FALSE);
    virtual void SAWLDBScannerEnable(void);
    virtual void SAWLDBScannerDisable(void);
    virtual void SASMMDisplayTolInfo(); 
    virtual SMStateBase *SASMMSecOffline();
    virtual CString SASMMConvertTol();
    virtual long SASMMConvertToPPWU();
    virtual long SALbsToKgs(long Weight);
    virtual CString GetExpectedWts();
    virtual SMStateBase *FinalizationWtIncrease(BEST nextState = BES_STATERETURN);
    virtual void AttractWtDecrease(void);
    virtual SMStateBase *FinalizationWtDecreaseNotAllowed(BEST nextState = BES_STATERETURN);
    virtual void FinalizationWtDecrease(void);
    virtual SMStateBase *VoidingWtIncrease(BEST prevState, BEST nextState);
    virtual SMStateBase *VoidingWtDecrease(void);
    virtual SMStateBase *ThresholdViolRAPClearSecBtn(BEST ReturnState);
    virtual SMStateBase *ThresholdViolWtIncreaseNotAllowed(BEST prevState, BEST ThreshViolationState);
    virtual SMStateBase *ThresholdViolWtDecrease(BEST prevState, BEST ThreshViolationState);
    virtual SMStateBase *ClearedSecViolSetReturnState(BEST PrevState, BEST NextState);
    virtual void LogWLDBWeightMismatch(CString csActualWeight, CString csExpectedWeight, long lTolerance);
    virtual CString BuildNeedMoreDataViewString(CString csView, CString csDescription, CString csLaneStateOneText, 
        CString csLaneStateTwoText, CString csLaneStateThreeText, CString csResourcePath);
    virtual SMStateBase *OnSecMgrNeedData(CString exceptionType, long lItemID);
    virtual bool HandleDataNeededSecureCam(CString &csData);
    virtual void SendSmartAssistExceptionClear(CString csAuthorization, CString csExceptionID);
    virtual void SetAttendantModeStateInSM(bool bAttendantModeState);

    virtual CString FmtWtToString(const long);
    virtual void UpdateScaleUnits(void);
    virtual SMStateBase *DetermineEASDeactivationState(void);
    virtual void ClearSecurityViolationLight(void);
    virtual void CalcObtainAndTraceSMMWtTol(bool bReplaceItemException = false);
    virtual void TellRAPAboutDelaySecurityIntervention();

    virtual bool AnySMDataNeeded();
    virtual SMStateBase *ProcessSMDataNeeded();

    virtual void SendSecureCamConfigToSecMgr(bool);
    virtual bool IsSecurityAnalysisComplete();
protected:
    virtual SMStateBase *SASecMgrParse( const MessageElement*);

    virtual long SecMgrInitialize(void);
    virtual long SecMgrUnInitialize(void);
    virtual void SAEnterSecurityCondition();
    virtual void SAClearSecurityCondition();
    virtual void SecMgrPreInitialize(void);  

    virtual void SecMgrPostInitialize(void); 

    virtual bool CouponTally(long TotalSales, long CouponTotals);
    virtual bool IsDuplicateCoupon();
    virtual SMStateBase *SAEventOccurred(const long);
    virtual SMStateBase *SASMMBagScaleEvent(void);

    virtual void SASMMFormatProductInfo();
    virtual void SASMMFormatToleranceInfo();
    virtual void SASMMFormatWLDBInfo(void);
    virtual CString AdjustBarCode(void);

#endif