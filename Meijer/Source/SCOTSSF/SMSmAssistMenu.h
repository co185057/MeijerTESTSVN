//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmAssistMenu.h
//
// TITLE: Class header for store mode AssistMenu state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMAssistMenuSTATE
#define _SMAssistMenuSTATE

#include "SMSmAssistMenuBase.h"
#include <vector>


#define HH_BUTTON1 1
#define HH_BUTTON2 2
#define HH_BUTTON3 3
#define HH_BUTTON4 4


typedef struct
{
	CString csButtonName;
	CString csKeySeq;
	CString csBtnState;
	
}TBButtonList;

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmAssistMenu : public SMSmAssistMenuBase
{
public:
    SMSmAssistMenu();
    virtual SMStateBase  *Initialize();
    virtual void UnInitialize();
    virtual SMStateBase *DMScanner(void);
    virtual SMStateBase *TBParse(MessageElement* me);
    virtual SMStateBase  *TimedOut(void);
	virtual void SetupHHButtons(long lBtnIndex =0);
	virtual SMStateBase  * SendBtnCmdtoTB(long lBtnIndex);

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
    virtual SMStateBase  *PSCmdButton(CString csContext, CString csControl, CString csCommand);
    virtual SMStateBase  *PSSwitchContext(BOOL bRemote, CString csNewContext);
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
    virtual SMStateBase  *OnWtDecrease(void);
    virtual void ShowAssistModeButton();
    virtual SMStateBase *DepositCouponsIfNecessary();

private:    
	static CString m_csOptionValue;
	static CString m_csBtnStateOptionValue;
	static bool m_bTBDefinesButton;
    CString m_csDefaultBTn;
	bool m_bUsingHandHeldWithSpecialScreen;
	static std::vector<TBButtonList> m_SButtonList;
	bool m_bHHMoreBtnOn;
	long m_lHHBtnIndex;

	

    DECLARE_DYNCREATE(SMSmAssistMenu)// MFC Runtime class/object information
};

#endif