//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStatePS.h
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMSTATEUSSF
#define _SMSTATEUSSF


public:
    virtual SMStateBase *StateEntry();
    virtual void StateExit();
    virtual SMStateBase *HookEntry(CString csEventName, long &lHookState, bool &bDone, SMStateBase *newState);
    virtual void SetActiveState(CString);
    virtual void SetActiveContext(CString);
    virtual SMStateBase *ValidateContext(CString csContextName);

    virtual SMStateBase *InitializeUSSF(void);
    virtual bool getUSSFDefaultContext(CString&);
    virtual long isHandled(CString, CString altContext = _T(""), bool bUSSFMode = false);
    virtual SMStateBase *MakeNextMove(CString thisContext = _T(""));
    virtual SMStateBase *ProcessEvent(CString thisEvent = _T(""), CString thisContext = _T(""));
    virtual SMStateBase *ProcessEventComplete(CString thisEvent = _T(""), CString thisContext = _T(""));
    virtual SMStateBase *ProcessEventPartial(CString thisEvent = _T(""), CString thisContext = _T(""));
    virtual bool isUSSFMode(void);
    virtual CString GetPendingNextStep(void);
    virtual void SetPendingNextStep(CString);

    virtual long StoreUSSFData(CString name, COleVariant value = _T(""));
    virtual long StoreAPPLData(CString name, COleVariant value = _T(""), CString prefix = _T(""));
    virtual void SetOperationalState(SMStateBase *);
    virtual SMStateBase *PerformAction(CString thisAction, bool &bActionTaken);
    virtual long ParseAction(CString thisAction);
    virtual long ShowPOSDisplay(CString csLine1, CString csLine2);
    virtual long ClearPOSDisplay(void);
    virtual long SendKeySequence(CString, int iSleep = 0);
    virtual long SendKeySequence(unsigned char, int iSleep = 0);

    virtual SMStateBase *PureMethod(void);
    virtual long SendPOSData(CString csSource);
    virtual CString getStateName(const SMStateBase * const pState);

protected:
    virtual int DoTranslationFromWCToMB( char *ch, const wchar_t *wChar, size_t count,bool bIgnoreNull );

    virtual SMStateBase *RunScript(CString thisScript);
    virtual SMStateBase *ManageAction(CString thisAction, bool &bActionTaken);
    virtual void MarkActivation(CString reference = _T(""));
    virtual SMStateBase *MakePreviousMove(void);
    virtual SMStateBase *MakeReferenceMove(CString reference = _T(""));
    virtual void SetActivationState(CString state, CString context);
    virtual void SetActivationState(void);
    virtual void ClearActivationDB(void);
    virtual void SaveActivationData(void);

#endif

