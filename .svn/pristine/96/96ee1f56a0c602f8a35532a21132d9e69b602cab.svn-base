//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmAssistMenu.h
//
// TITLE: Class header for Assist menu
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMSmAssistMenuBase
#define _SMSmAssistMenuBase

#include "SMSmAssistMenuBase.h"
#include "SMSmAssistModeBase.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSmAssistMenuBase :  public STATE(StoreModeState)
{
public:
	SMSmAssistMenuBase();
    SMSmAssistMenuBase(CString csMenuContext);
    virtual SMStateBase  *Initialize();
	virtual void		 UnInitialize();
    static bool bReturnToShoppingNow;
    SMStateBase *DMScanner(void);
    SMStateBase *TBParse(MessageElement* me);
    virtual SMStateBase  *TimedOut(void);
    static void SetAssistMessage(PSTEXT textID, PSTEXTWAVE waveID);
protected:

    virtual SMStateBase  *PSNumericKey(const long lKey);
    virtual SMStateBase  *PSAlphaNumericKey(const long);

	virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);
	virtual SMStateBase  *PSButton3(void);
    virtual SMStateBase  *PSButton4(void);
	virtual SMStateBase  *PSButton5(void);
    virtual SMStateBase  *PSButton8(void);
    virtual SMStateBase  *PSRemoteButton1(void);
    virtual SMStateBase  *PSCmdButton(CString csContext, CString csControl, CString csCommand); //RFQ 986
    virtual SMStateBase  *PSSwitchContext(BOOL bRemote, CString csNewContext); //RFQ 986
    virtual SMStateBase  *PSOnRemoteDisconnected(const long lParm);
    virtual SMStateBase *OnRAPUnsolicitedDataReceived(LPCTSTR sData);
    virtual SMStateBase  *OnChangePrice();
    virtual SMStateBase  *OnChangeQuantity();
    virtual SMStateBase  *OnReturnToShopping(void);
    virtual SMStateBase  *OnVoidButton();
	virtual SMStateBase  *OnAssistModeButton();
    virtual SMStateBase  *OnKeyInCode();
    virtual CString ExtractString(CString strData, CString sStringName);
	virtual void SetButtonStates();
    virtual bool IsFullAssistNeeded();
    static bool m_bShowEchoBox;
    static PSTEXT m_PromptTextID;
    static PSTEXTWAVE m_AudioID;    
    static CString m_csMenuContext;
    bool m_bBlockScanData;  //RFQ 986

  DECLARE_DYNCREATE(SMSmAssistMenuBase) // MFC Runtime class/object information
};

#endif
