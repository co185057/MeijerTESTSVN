//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMOperatorPasswordState.h
//
// Description: Derived from SMHelpModeState, all states that need to prompt
//        for an operator and password derive from this state.
//
// AUTHOR: Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMOPERATORPASSWORDSTATE
#define _SMOPERATORPASSWORDSTATE


#include "SMOperatorPasswordStateBase.h"

//////////////////////////////////////
#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMOperatorPasswordState : public SMOperatorPasswordStateBase
{
	
public:
    SMOperatorPasswordState();
    virtual SMStateBase * Initialize();
    virtual void UnInitialize();

    virtual SMStateBase * createLevel1Message( LPCTSTR szMsg,
        PSTEXT pstMsg,
        PSTEXT prompt,
        long   value,
        long   device,
        bool   checkHealth,
        bool displayHTML=false); 
    virtual SMStateBase * createLevel2Message( LPCTSTR szMsg,
        PSTEXT pstMsg,
        PSTEXT prompt,
        long   value,
        long   device,
        bool   checkHealth,
        bool displayHTML=false);
    virtual SMStateBase * createLevel3Message( LPCTSTR szMsg,
        PSTEXT pstMsg,
        PSTEXT prompt,
        long   value,
        long   device,
        bool   checkHealth,
        bool displayHTML=false);
    virtual SMStateBase * stateAfterPasswordVerify();

	// SA250050: inherited from SMState
    virtual inline CString OperPassStateName() { return _T("SMOperatorPasswordState"); }
    virtual inline bool IsPromptingForOperator() { return inputMode == PROMPTING_FOR_OPERATOR; }
    virtual inline bool IsPromptingForPassword() { return inputMode == PROMPTING_FOR_PASSWORD; }
    // SA250050: Store biometric data that may have been provided for authentication
	CString m_sBiometData;

protected:
    virtual bool parentClassCurrentlyHandlingButton2();
    virtual bool parentClassCurrentlyHandlingCancelKey();
    virtual void     showScreen(bool passwordVerified = false);
    virtual SMStateBase * DMScanner(void);
    virtual SMStateBase * PSNumericKey(const long);
    virtual SMStateBase * PSAlphaNumericKey(const long);
    virtual SMStateBase * PSClearKey();
    virtual SMStateBase * PSButton1();
    virtual SMStateBase * PSButton2();
    virtual SMStateBase * PSCancelKey();
    virtual SMStateBase * PSButtonGoBack();
    virtual SMStateBase * displayInvalidMessage();
    virtual SMStateBase * OnBackToLGW(void);
    virtual void EndOfTransactionPrint();
    virtual void promptForOperator();
    virtual void promptForPassword();
    virtual bool getOperatorPassword();
    virtual void ShowEnterID();
    virtual SMStateBase *OnSecMgrNeedData(CString exceptionType, long lItemID);
    virtual SMStateBase *HandleScannerData( const MessageElement* );
	virtual SMStateBase *DMScannerEncrypted(void); //SSCOADK-7272

	//POS81634
	virtual SMStateBase * PSEnterKey();
	virtual SMStateBase * PSButton5();
	//e POS81634

    virtual bool GetUserInfo();
    virtual bool OnUserCredentialsAquired();

    DECLARE_DYNCREATE(SMOperatorPasswordState)// MFC Runtime class/object information
	
};

#endif
