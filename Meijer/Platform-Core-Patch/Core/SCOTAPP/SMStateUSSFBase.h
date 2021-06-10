
public:
  virtual SMStateBase *StateEntry();
  virtual void StateExit();
  //virtual bool HookEntry(CString csEventName, SMStateBase *&nextState, long &lHookState);
  virtual SMStateBase *HookEntry(CString csEventName, long &lHookState, bool &bDone, SMStateBase *newState);
  virtual void SetActiveState(CString){return;}
  virtual void SetActiveContext(CString){return;}
  static CString GetActiveState(void);
  static CString GetActiveContext(void);
  virtual SMStateBase *ValidateContext(CString csContextName);

public:
  virtual SMStateBase *SMStateBase::InitializeUSSF(void);
  static void SMStateBase::UnInitializeUSSF(void);
  static SMStateBase *pUSSFthis;
  static SMStateBase *pUSSFNative;
  virtual bool getUSSFDefaultContext(CString&);
  virtual long isHandled(CString, CString altContext = _T(""), bool bUSSFMode = false){return 0;}
  virtual SMStateBase *MakeNextMove(CString thisContext = _T("")){return STATE_NULL;}
  virtual SMStateBase *ProcessEvent(CString thisEvent = _T(""), CString thisContext = _T("")){return STATE_NULL;}
  virtual SMStateBase *ProcessEventComplete(CString thisEvent = _T(""), CString thisContext = _T("")){return STATE_NULL;}
  virtual SMStateBase *ProcessEventPartial(CString thisEvent = _T(""), CString thisContext = _T("")){return STATE_NULL;}
  virtual bool isUSSFMode(void){return false;}  
  virtual CString GetPendingNextStep(void);
  virtual void SetPendingNextStep(CString);
  //virtual bool HookEntry(CString csEventName, SMStateBase *nextState, long &lHookState, long &lHookDisposition);

  static SMStateBase *MakeUSSFState(void);

public:
  virtual long StoreUSSFData(CString name, COleVariant value = _T(""));
  virtual long StoreAPPLData(CString name, COleVariant value = _T(""), CString prefix = _T(""));
  virtual void SetOperationalState(SMStateBase *);

protected:
  virtual int DoTranslationFromWCToMB( char *ch, const wchar_t *wChar, size_t count,
								 bool bIgnoreNull );
  SMStateBase *pOperationalState;

public:
  virtual SMStateBase *PerformAction(CString thisAction, bool &bActionTaken){return STATE_NULL;}
  virtual long ParseAction(CString thisAction){return 0;}
  virtual long ShowPOSDisplay(CString csLine1, CString csLine2);
  virtual long ClearPOSDisplay(void);
  virtual long SendKeySequence(CString, int iSleep = 0);
  virtual long SendKeySequence(unsigned char, int iSleep = 0);

  virtual SMStateBase *PureMethod(void){return STATE_NULL;}
  virtual long SendPOSData(CString csSource);
  virtual CString getStateName(const SMStateBase * const pState);

  //CString csQuantity, csPrice, csWeight, csTare;
  static bool fItemOverride;

public:
  static void SetPendingEvent(CString);

protected:
  virtual SMStateBase *RunScript(CString thisScript){return STATE_NULL;}
  virtual SMStateBase *ManageAction(CString thisAction, bool &bActionTaken){return STATE_NULL;}
  virtual void MarkActivation(CString reference = _T(""));
  //virtual SMStateBase *MakeNextMove(CString thisContext = _T("")){return STATE_NULL;}
  virtual SMStateBase *MakePreviousMove(void){return STATE_NULL;}
  virtual SMStateBase *MakeReferenceMove(CString reference = _T("")){return STATE_NULL;}
  virtual void SetActivationState(CString state, CString context);
  virtual void SetActivationState(void);
  virtual void ClearActivationDB(void);
  virtual void SaveActivationData(void);

  static long lUnapprovedUSSFItems;
  //static long lMasterEventSerialNumber;
  //static bool bTransitionLock;
  static long lRecursionDepth;
  static long lRecursionLimit;
  static bool bIgnoreUnknownAction;

  static CString csExitName;
  static CString csExitVersion;
  static HMODULE hUSSFExit;

  CStringArray vcs_ContextList;

  static struct ActivationRecord
  {
	long lSerial;
	bool bInScope;
	CString csTimeStamp;
    CString CurrentState;
    CString CurrentContext;
    CString ReferenceKey;
	long lLinkage;
  } thisActivation;

  static vector<ActivationRecord> ActivationDB;
  static long lActivationSerial;
  static long lHookSerial;

  static CCriticalSection USSFEventCriticalSection;
  CString csHookTraceBuffer;

//////////////////////////////
// In-Line Public Setters
//////////////////////////////
public:

	inline void USSFset_csItemSent(CString csValue){csItemSent = csValue;}
