//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmAssistMode.h
//
// TITLE: Class header for Assist mode keyboard entry mode
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMSMHANDHELDASSISTMODE
#define _SMSMHANDHELDASSISTMODE

#include "SMSmAssistMode.h"

#define MAX_DYNA_SOFTKEY 8
#define MAX_HH_SOFTKEY 2

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmHandHeldAssistMode : public SMSmAssistMode
{
public:
	SMSmHandHeldAssistMode();
    SMStateBase  *Initialize();
    virtual void     UnInitialize();
	virtual SMStateBase *TBParse(MessageElement *me);
    virtual SMStateBase  *TimedOut(void);
    virtual bool PSAssistMenuLegalForState() { return true; };
    virtual bool PSRemoteLegalForState() { return true; };
    virtual void ShowProcssingScreen(void);
	virtual bool IsOperatorGuidance(void);
protected:
	virtual SMStateBase  *PSAssistKey(const long nKey);
    virtual SMStateBase  *PSNumericKey(const long lKey);
    virtual SMStateBase  *PSClearKey(void);
    virtual SMStateBase  *PSButton1(void);         // go back key
    virtual SMStateBase  *PSButton2(void);         // toogle key
    virtual SMStateBase  *PSButton5(void);         // override for button 5    
    virtual SMStateBase  *PSButton8(void);         //   more key  
    virtual SMStateBase  *PSEnterKey(void);        // override for enter key
	virtual SMStateBase  *PSBackSpaceKey(void);
	virtual SMStateBase  *PSAlphaNumericKey(const long lKey);
    virtual SMStateBase  *PSReceiptUp(void); 
    virtual SMStateBase  *PSReceiptDown(void);
	
    virtual void TBShowDisplay(bool bRefresh = false);    
    virtual void EnableDisableAllKeys(bool bFlag);
    void EnableClearKey(void);    
    void EnableAllowedSoftKeys(void);
    void SetOperatorGuidance();
    void SetupScreenContext();
    void DisableSoftKeys();
	void SetupGraphicalScreen();
	void SetScreenText();

    void SetDynakeys();
    bool AllowAssistmodeExit(void);
	
    

private:
	
	bool m_bTransactionEnding;
    int m_iBtnIndex;
    bool m_bHideInput;
    CString m_csButtonText[9];
    bool m_bButtonEnable[9];
    bool m_bButtonIconOn[9];
    bool m_bAssistModeExitAllowed;
	CString m_csScreenContext;

  DECLARE_DYNCREATE(SMSmHandHeldAssistMode) // MFC Runtime class/object information
};

#endif