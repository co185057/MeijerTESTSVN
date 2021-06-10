#ifndef _SM_RECALLING_ITEMS_BASE_
#define _SM_RECALLING_ITEMS_BASE_

#ifndef _CPPUNIT
#include "SMInProgress.h"
#else 
#include "FakeSMInProgressBase.h"
#endif 

#include <queue>
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
SMRecallingItemsBase : public STATE(InProgress)
{
public:
    SMRecallingItemsBase();

    virtual SMStateBase * Initialize();
    virtual void UnInitialize(void);  
    virtual SMStateBase * PSButton1();
    virtual bool inProgress() {return false;} // Return here after timeout.
    virtual bool storeMode()  {return false;}
    virtual bool helpMode()   {return false;}
    //+SSCO Mobile
    virtual SMStateBase  *OnSkipBaggingAllowed(void);
    virtual SMStateBase  *OnBackToLGW(void); //430778   
    virtual SMStateBase* OnMatchedWt(void); // Smart Scale reported a matched weight
    virtual void FinishRecall();

    static long g_nTimeoutRecallingWaitForDone;
    //-SSCO Mobile

protected:
    virtual SMStateBase  *TimedOut();
    virtual SMStateBase *SASecMgrParse( const MessageElement*);   
    static VOID CALLBACK EscalationTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
    virtual void ShowRecallingScreen();
    void InitLanguageInfo(void);

    static bool g_bWaitingForScale;
    static bool g_bPressedDone;
    static CString m_Context;


DECLARE_DYNCREATE(SMRecallingItemsBase)
};

#endif //_SM_RECALLING_ITEMS_BASE_
