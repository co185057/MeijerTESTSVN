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

#ifndef _CPPUNIT
#include "SMSmAssistMenuBase.h"
#include "SMSmAssistModeBase.h"
#include "SMStoreModeMenu.h"   //SR819
#else
#include "FakeSMStoreModeMenuBase.h"
#endif //_CPPUNIT


#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSmAssistMenuBase :  public STATE(StoreModeMenu) //SR819
{
public:
#ifndef _CPPUNIT
	SMSmAssistMenuBase();
    SMSmAssistMenuBase(CString csMenuContext);
#endif
    virtual SMStateBase  *Initialize();
#ifndef _CPPUNIT
	virtual void		 UnInitialize();
    virtual SMStateBase *DMScanner(void);
    virtual SMStateBase *TBParse(MessageElement* me);
#endif
    virtual SMStateBase  *TimedOut(void);
#ifndef _CPPUNIT
    static void SetAssistMessage(PSTEXT textID, PSTEXTWAVE waveID);
#endif
    static bool bScannerEnabledSaved; //sscoadk-3385
    static bool bReturnToShoppingNow;
protected:
#ifndef _CPPUNIT
    virtual SMStateBase  *PSNumericKey(const long lKey);
    virtual SMStateBase  *PSAlphaNumericKey(const long);

	virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);
	virtual SMStateBase  *PSButton3(void);
    virtual SMStateBase  *PSButton4(void);
#endif
    virtual SMStateBase  *PSButton5(void);
    virtual SMStateBase  *OnAssistModeButton();
#ifndef _CPPUNIT
    virtual SMStateBase  *PSButton8(void);
    virtual SMStateBase  *PSRemoteButton1(void);
    virtual SMStateBase  *PSCmdButton(CString csContext, CString csControl, CString csCommand); //RFQ 986
    virtual SMStateBase  *PSSwitchContext(BOOL bRemote, CString csNewContext); //RFQ 986
    virtual SMStateBase  *PSOnRemoteDisconnected(const long lParm);
    virtual SMStateBase *OnRAPUnsolicitedDataReceived(LPCTSTR sData);
    virtual SMStateBase  *OnChangePrice();
    virtual SMStateBase  *OnChangeQuantity();
#endif
    virtual SMStateBase  *OnReturnToShopping(void);
#ifndef _CPPUNIT
    virtual SMStateBase *DepositCouponsIfNecessary();
    virtual SMStateBase  *OnVoidButton();
    virtual SMStateBase  *OnKeyInCode();
    virtual CString ExtractString(CString strData, CString sStringName);
	virtual void SetButtonStates();
    virtual bool IsFullAssistNeeded();
    static bool m_bShowEchoBox;
    static PSTEXT m_PromptTextID;
    static PSTEXTWAVE m_AudioID;    
#endif
    static CString m_csMenuContext;
#ifndef _CPPUNIT
    bool m_bBlockScanData;  //RFQ 986
    virtual SMStateBase  *OnWtDecrease(void);
	virtual void ShowAssistModeButton();	//SSCOADK-667

  DECLARE_DYNCREATE(SMSmAssistMenuBase) // MFC Runtime class/object information
#endif
};

#endif
