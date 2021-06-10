//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStateSABase.h
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMSTATESABASE
#define _SMSTATESABASE

#define SMM_LISTWIDTH  42;

#ifndef _CPPUNIT
#include "SMStateSAConstants.h"
#else
#include "FakeSMStateSAClasses.h"
#endif //_CPPUNIT

public:
#ifndef _CPPUNIT

class InitializeParms
	  : public CKeyValueHandler<InitializeParms>
  {
	  DECLARE_KV_HANDLER_MAP();

	  DECLARE_KV_HANDLER(DeviceName);
	  DECLARE_KV_HANDLER(MsgTextIndex);
  	  DECLARE_KV_HANDLER(FailureText);
  	  DECLARE_KV_HANDLER(ErrorFilename);
   	  DECLARE_KV_HANDLER(ErrorCode);
   	  DECLARE_KV_HANDLER(MissingFilename);
	  DECLARE_KV_HANDLER(ApplicationState);

public:
	  void TraceThis(const _TCHAR *pPairs) throw();
	  CString m_csDeviceName;
	  int m_nMsgTextIndx;
	  CString m_csFailureText;
	  CString m_csErrorFilename;
	  long m_lErrorCode;
	  CString m_csMissingFilename;
	  CString m_csApplicationState;

  };
class SecMgrEventParms
	  : public CKeyValueHandler<SecMgrEventParms>
  {
	  DECLARE_KV_HANDLER_MAP();

	  DECLARE_KV_HANDLER(ItemId);
	  DECLARE_KV_HANDLER(ExceptionId);
      DECLARE_KV_HANDLER(DeltaWeight);
      DECLARE_KV_HANDLER(ActualWeight);
	  DECLARE_KV_HANDLER(ActualWeightNotConverted);		//TAR 423666 
	  DECLARE_KV_HANDLER(ExpectedWeightNotConverted);	//TAR 423666
	  DECLARE_KV_HANDLER(ExpectedWeight);
	  DECLARE_KV_HANDLER(Weight2x);
	  DECLARE_KV_HANDLER(Tolerance);
  	  DECLARE_KV_HANDLER(ToleranceType);
   	  DECLARE_KV_HANDLER(Disposition);
	  DECLARE_KV_HANDLER(ExceptionType);
	  DECLARE_KV_HANDLER(ExceptionMsgIndex);
	  DECLARE_KV_HANDLER(TraceDescription);
	  DECLARE_KV_HANDLER(ItemDescription);
	  DECLARE_KV_HANDLER(ThresholdMax);
	  DECLARE_KV_HANDLER(MultiPackItem);
	  DECLARE_KV_HANDLER(ApplicationState);
	  DECLARE_KV_HANDLER(SourcePath);

public:
      SecMgrEventParms(){};

      SecMgrEventParms(SecDelayIntervention *secDelayIntervention)
          :nSecInterventionIndex(0),
          m_secDataNeededIntervention(NULL),
      m_secDelayIntervention(secDelayIntervention){};

      SecMgrEventParms(SecDataNeededIntervention *secDataNeededIntervention)
          :nSecInterventionIndex(0),
          m_secDelayIntervention(NULL),
      m_secDataNeededIntervention(secDataNeededIntervention){};

	  HRESULT ProcessRecord();
  	  void TraceThis(const _TCHAR *pPairs) throw();
	  long m_lItemId;
	  long m_lExceptionId;
      CString m_csDeltaWeight;
      CString m_csActualWeight;
	  CString m_csActualWeightNotConverted;				//TAR 423666
      CString m_csExpectedWeightNotConverted;			//TAR 423666	
      CString m_csExpectedWeight;
      long m_lTolerance;
	  CString m_csToleranceType;
      CString m_csDisposition;
      CString m_csExceptionType;
      int m_nExceptionMsgIndex;
      CString m_csTraceDescription;
	  int nSecInterventionIndex;
	  CString m_csItemDescription;
	  bool m_bWeight2x;
	  bool m_bMultiPackItem;    // migrating TAR 256154
	 int m_nThresholdMax;
	  SecDelayIntervention *m_secDelayIntervention;
	  CString m_csApplicationState;
      CString m_csSourcePath;
	  SecDataNeededIntervention *m_secDataNeededIntervention;

      void Clear();
      void Copy(SecDataNeededIntervention);
  };

class SecMaintenanceEventParms
	  : public CKeyValueHandler<SecMaintenanceEventParms>
  {
	  DECLARE_KV_HANDLER_MAP();

	  DECLARE_KV_HANDLER(Operation);
	  DECLARE_KV_HANDLER(UPC);
      DECLARE_KV_HANDLER(WeightCount);
      DECLARE_KV_HANDLER(Weight);
      DECLARE_KV_HANDLER(Tally);
      DECLARE_KV_HANDLER(Approved);
      DECLARE_KV_HANDLER(Date);
      DECLARE_KV_HANDLER(ToleranceType);
	  DECLARE_KV_HANDLER(Tolerance);
  	  DECLARE_KV_HANDLER(SubstitutionCheck);
	  DECLARE_KV_HANDLER(WeightRequired);
	  DECLARE_KV_HANDLER(Metric);
	  DECLARE_KV_HANDLER(ErrorType);
	  DECLARE_KV_HANDLER(MinTolerance);
	  DECLARE_KV_HANDLER(MaxTolerance);
	  DECLARE_KV_HANDLER(ServerStatus);
  	  DECLARE_KV_HANDLER(ObservedWeight);
	  DECLARE_KV_HANDLER(ApplicationState);
	 
public:
	  SecMaintenanceEventParms(){};
	  SecMaintenanceEventParms(SecMaintenanceRecord **secMaintenanceRecord)
		  :nSecMaintenanceRecordIndex(0),
	  m_secMaintenanceRecord(secMaintenanceRecord){};
	  HRESULT ProcessRecord();
	  void TraceThis(const _TCHAR *pPairs) throw();
	  SecMaintenanceRecord **m_secMaintenanceRecord;
	  int nSecMaintenanceRecordIndex;

	  CString m_csOperation;
	  CString m_csUPC;
	  long m_lWeightCount;
	  CString m_csWeight;
	  long m_lTally;
	  CString m_csApproved;
	  CString m_csDate;
	  long m_lDate;
      CString m_csToleranceType;
      CString m_csTolerance;
	  CString m_csSubstitutionCheck;
	  CString m_csWeightRequired;
	  CString m_csMetric;
	  CString m_csErrorType;
	  CString m_csMinTolerance;
	  CString m_csMaxTolerance;
      CString m_csServerStatus;
	  CString m_csObservedWeight;
	  CString m_csApplicationState;

  };
class SecAppControlEventParms
	  : public CKeyValueHandler<SecAppControlEventParms>
  {
	  DECLARE_KV_HANDLER_MAP();

	  DECLARE_KV_HANDLER(Operation);
  	  DECLARE_KV_HANDLER(Tare);
	  DECLARE_KV_HANDLER(ApplicationState);

	 
public:
	  void TraceThis(const _TCHAR *pPairs) throw();
	  CString m_csOperation;
	  CString m_csTare;
	  CString m_csApplicationState;
  };

class SecExceptionClearedEventParms
	  : public CKeyValueHandler<SecExceptionClearedEventParms>
  {
	  DECLARE_KV_HANDLER_MAP();

	  DECLARE_KV_HANDLER(ExceptionId);
	  DECLARE_KV_HANDLER(ExceptionType);
	  DECLARE_KV_HANDLER(ClearRAPApproval);
	  DECLARE_KV_HANDLER(ApplicationState);
      DECLARE_KV_HANDLER(Disposition);

public:
	  SecExceptionClearedEventParms(){};
	  SecExceptionClearedEventParms(SecImmediateIntervention *secImmediateIntervention){};
	  void TraceThis(const _TCHAR *pPairs) throw();

	  HRESULT ProcessRecord();
	  long m_lExceptionId;
      CString m_csExceptionType;
	  CString m_csApplicationState;
      CString m_csDisposition;
	  bool m_bClearRAPApproval;
  };

class SecurityControlParms
	  : public CKeyValueHandler<SecurityControlParms>
  {
	  DECLARE_KV_HANDLER_MAP();

	  DECLARE_KV_HANDLER(ReloadOptionsStatusText);
  	  DECLARE_KV_HANDLER(DeviceName);
  	  DECLARE_KV_HANDLER(DeviceStatus);
  	  DECLARE_KV_HANDLER(ParamKeyValue);
	  DECLARE_KV_HANDLER(ApplicationState);

public:
	  void TraceThis(const _TCHAR *pPairs) throw();
	  CString m_csReloadOptionsStatusText;
	  CString m_csDeviceName;
  	  CString m_csDeviceStatus;
	  CString m_csParamKeyValue;
	  CString m_csApplicationState;

  };

class SecurityDeviceFailureEventParms
	  : public CKeyValueHandler<SecurityDeviceFailureEventParms>
  {
	  DECLARE_KV_HANDLER_MAP();

	  DECLARE_KV_HANDLER(DeviceName);
	  DECLARE_KV_HANDLER(ApplicationState);
	  DECLARE_KV_HANDLER(ErrorCode);
  	  DECLARE_KV_HANDLER(ExtendedErrorCode);

public:
	  void TraceThis(const _TCHAR *pPairs) throw();
	  CString m_csDeviceName;
	  CString m_csApplicationState;
	  long  m_lErrorCode;
	  long	m_lExtendedErrorCode;


  };

class SecurityControlEventParms
	  : public CKeyValueHandler<SecurityControlEventParms>
  {
	  DECLARE_KV_HANDLER_MAP();

	  DECLARE_KV_HANDLER(Operation);
	  DECLARE_KV_HANDLER(WeightObserved);
	  DECLARE_KV_HANDLER(ApplicationState);

public:
	  void TraceThis(const _TCHAR *pPairs) throw();
	  CString m_csOperation;
	  CString m_csWeightObserved;
	  CString m_csApplicationState;

  };
//  static PointerStack<SecImmediateIntervention *> ImmediateInterventionStack;    // save previous exception ids

#endif //_CPPUNIT
  static CTypedPtrList<CPtrList,SecImmediateIntervention *> ImmediateInterventionStack;



  virtual long SASendSCOTInput(long ID,LPCTSTR=_T(""),long=0,long=0,long=0,long=0,bool=FALSE);
#ifndef _CPPUNIT
  virtual void SAWLDBScannerEnable(void);
  virtual void SAWLDBScannerDisable(void);
#endif //_CPPUNIT
  virtual void SASMMDisplayTolInfo(); 
  virtual SMStateBase *SASMMSecOffline();
#ifndef _CPPUNIT
  virtual CString SASMMConvertTol();
  virtual long SASMMConvertToPPWU();
#endif //_CPPUNIT
  static CString csScaleUnits;             // string for 'lb' or 'kg'
#ifndef _CPPUNIT
  static CString csSMMScannedUPC;
#endif //_CPPUNIT
  static bool fSAInTransaction;
  static bool fSAInItemSale;
#ifndef _CPPUNIT
  static bool fSAInItemize;
  static bool fSAInTender;
  static bool fSAInCustMode;
#endif //_CPPUNIT
  static bool fSAInStoreMode;
#ifndef _CPPUNIT
  static bool fSAInTenderStart;
#endif //_CPPUNIT
  static long nSAPrevSCOTInputID;
#ifndef _CPPUNIT
  virtual long SALbsToKgs(long Weight);
  static bool g_bOkToUpdate;						// when store mode approves a wt delta, only update the DB when this is true
  static long g_lWLDBNotApprovedThisTrans;
#endif //_CPPUNIT
  virtual CString GetExpectedWts();					// return expected wt in CString format.
  static long g_lNumberOfWLDBEntriesThisItem;
#ifndef _CPPUNIT
  virtual SMStateBase *FinalizationWtIncrease(BEST nextState = BES_STATERETURN);	// determine if wt increase allowed on finalization screens
  virtual void AttractWtDecrease(void); // TAR 257709
  virtual SMStateBase *FinalizationWtDecreaseNotAllowed(BEST nextState = BES_STATERETURN);
  virtual void FinalizationWtDecrease(void);	
  virtual SMStateBase *VoidingWtIncrease(BEST prevState, BEST nextState);	// determine if wt increase allowed on Voiding screens
  virtual SMStateBase *VoidingWtDecrease(void);			// determine if wt decrease allowed on Voiding screens
  virtual SMStateBase *ThresholdViolRAPClearSecBtn(BEST ReturnState);  // RAP sending Clear Security on threshold sec vio
  virtual SMStateBase *ThresholdViolWtIncreaseNotAllowed(BEST prevState, BEST ThreshViolationState);	 
  virtual SMStateBase *ThresholdViolWtDecrease(BEST prevState, BEST ThreshViolationState);	 
  virtual SMStateBase *ClearedSecViolSetReturnState(BEST PrevState, BEST NextState);	 
  virtual void LogWLDBWeightMismatch(CString csActualWeight, CString csExpectedWeight, long lTolerance); //NN 080602 RFC 210177
  static bool g_bAllowRemove;
#endif // _CPPUNIT
  static bool g_bWtExpected;		// only update if we're expecting a weight

  virtual CString BuildNeedMoreDataViewString(CString csView, CString csDescription, CString csLaneStateOneText, 
      CString csLaneStateTwoText, CString csLaneStateThreeText, CString csResourcePath);
  virtual SMStateBase *OnSecMgrNeedData(CString exceptionType, long lItemID);
  virtual bool HandleDataNeededSecureCam(CString &csData);
  virtual void SendSmartAssistExceptionClear(CString csAuthorization, CString csExceptionID);
  virtual void SetAttendantModeStateInSM(bool bAttendantModeState); //SSCOADK-544



protected:

  // Security Manager
  virtual SMStateBase *SASecMgrParse( const MessageElement*);	// same parse for all
#ifndef _CPPUNIT
  virtual long SecMgrInitialize(void);    // called by MP at start up
  virtual long SecMgrUnInitialize(void);    // called by MP at start up
#endif //_CPPUNIT
  virtual void SAEnterSecurityCondition();
  virtual void SAClearSecurityCondition();
#ifndef _CPPUNIT
  /**
   * Perform setup operations prior to loading security.
   * TAR 452531.
   */
  virtual void SecMgrPreInitialize(void);  

  /**
   * Perform setup operations after loading security.
   * TAR 452531.
   */
  virtual void SecMgrPostInitialize(void); 

  virtual bool CouponTally(long TotalSales, long CouponTotals);
  virtual bool IsDuplicateCoupon();
  static long g_lCTW;					// global Current Total Weight
  static long g_lLRW;
  static long g_lBeginningTransLGW;
  static BEST nStateBeforeSecVio;	// global the state before go to Sec Vio Screens
  static long g_lPreLRW; //previous LRW - tar 197339

  // handlers for DM device data
  virtual SMStateBase *SAEventOccurred(const long);
  virtual SMStateBase *SASMMBagScaleEvent(void);
#endif //_CPPUNIT
  static long lSALoggedMsgs;        // SA generated messages logged in total
#ifndef _CPPUNIT
  static long lSAYellowNeedingApproval;	// SA generated messages logged that caused yellow light
  static bool lSARedNeedingApproval;  // TAR 197015

  long (PSProceduresBase::*saMsgFuncPtr)(LPCTSTR szText);
  bool bMoveSAMsgDown;

//private:

  virtual void SASMMFormatProductInfo();		// format the 1st line in the display
  virtual void SASMMFormatToleranceInfo();		// format the tolerance line in the display
#endif //_CPPUNIT
  virtual void SASMMFormatWLDBInfo(void);		// format the WLDB line in the display
#ifndef _CPPUNIT
  static bool g_lRAPApprovedUpdate;
  virtual CString AdjustBarCode(void);					// Moved RFC 212888  to it's own function - code was hard to read.  LPM120202
  static long	m_lRememberVirtualZero;    // TAR 257709 - moved it form SMAttractBase to here                                                  // TAR 235796 - AdjustBarcode() should return a CString
#endif //_CPPUNIT
public:
#ifndef _CPPUNIT
  virtual CString FmtWtToString(const long);
#endif //_CPPUNIT
  virtual void UpdateScaleUnits(void);
#ifndef _CPPUNIT
  virtual SMStateBase *DetermineEASDeactivationState(void);
#endif //_CPPUNIT
  static bool bIsInFinalization; //in finalization - tar 194770
#ifndef _CPPUNIT  
  static bool g_bGotDBLookupSuccess;	// set to true if DB Client fires DBM_DBLOOKUPSUCCESS event
  static bool g_bGotDeptInfoSuccess;	// set to true if DB Client fires DBM_DEPTINFOSUCCESS event
  static bool g_bTBReturnedITEMSOLDforSA;	// for sa to determine when BagAndEAS should go to InSAProgress LPM030602
  static bool g_bMatchedReceivedForTAB;	// for TAB when sensor B is tripped by a hand 1976010 LPM041802
  static long g_lTABMisMatchApprWt;		// RAP approved a TAB 'wrong' weight. Now have to get it off the belt without any more mismatched wts. 200247 LPM042502
#endif //_CPPUNIT
  virtual void ClearSecurityViolationLight(void);  //Clear red light, restore the prev light
  //TAR 216262 Move this global variable to public section so we can access it outside.
  static long g_lLGW;					// global Last Good Weight
  virtual void CalcObtainAndTraceSMMWtTol(bool bReplaceItemException = false);	// Std Dev SMM needs to access the tolerance data in Wt Info, don't always want to replace the item exception data LPM120403
#ifndef _CPPUNIT
  //End of Fixing the problem with learning light wt item when already existing wt entry in WLDB
  static bool m_bLastChanceLookupCalled;	//228759 - don't update the WLDB if LastChanceLookup was Called LPM022303
  static bool g_bWtIncAtContextHelpComingFromAttract; //TAR237079 051303 CHU/TP
#endif //_CPPUNIT
  static long lSecMgrCookie; // NewSec - returned cookie
#ifndef _CPPUNIT
  static CKeyValueParmList secKeyValues; // NewSec - Key Value 
#endif //_CPPUNIT
  static SecMgrEventParms secEventParms;
  static SecAppControlEventParms secAppControlEventParms;
  static SecMaintenanceRecord *psecMaintenanceRecordArray;
  static long lSMMWeightCount;
  static CString csSMMOperation;
  static CString csSMMTolerance;
  static CString csSMMSubstitutionCheck;
  static CString csSMMToleranceType;
  static CString csSMMWeightRequired;
  static CString csSMMErrorType;
  static CString csSMMMinTolerance;
  static CString csSMMMaxTolerance;
  static CString csSMMServerStatus;
  static CString csSMMObservedWeight;
  static bool g_bLastImmediateInterventionCleared;
#ifndef _CPPUNIT
  static CString g_csInitMissingFilename;
  static CString g_csInitErrorFilename;

#endif //_CPPUNIT
  static bool bTransactionComplete;
  virtual void TellRAPAboutDelaySecurityIntervention();
  static bool g_bSubstCkPriceEmbeddedWeights;
  static bool g_bApproveFromRAP;    // TAR 269648
  static CString g_csActualWeight;

  static CString g_csExpectedWeight;
  static CString g_csDeltaWeight;   // SR682

  static long g_lTolerance;
  static bool g_bIsSecurityTransactionPolicyReady;

#ifndef _CPPUNIT
  virtual bool AnySMDataNeeded();
  virtual SMStateBase *ProcessSMDataNeeded();
#endif //_CPPUNIT

  virtual void SendSecureCamConfigToSecMgr(bool);
  static bool g_bIsSecureCamEnable;
  static bool g_bIsSmartAssistImmediate;
  static bool g_bReportTrustLevel;

  static int g_nSecurityAnalysisTimer;
  static bool g_bSecurityAnalysisTimerExpires; 
  static bool m_bSecurityAnalysisComplete;
  virtual bool IsSecurityAnalysisComplete() { return m_bSecurityAnalysisComplete; } 
  static bool g_bSLServerStatus; // false - offline; true - online;
#endif
