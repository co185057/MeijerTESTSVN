//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSecOperatorPWState.h
//
// Description: Derived from SMHelpModeState, all states that need to prompt
//        for an operator and password derive from this state.
//
// AUTHOR: Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMSECOPERATORPWSTATE
#define _SMSECOPERATORPWSTATE


#include "SMSecOperatorPWStateBase.h"

//////////////////////////////////////
#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSecOperatorPWState : public SMSecOperatorPWStateBase
{
	
public:
    SMSecOperatorPWState();
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
    virtual inline CString OperPassStateName() { return _T("SMSecOperatorPWState"); }
    // SA250050: same functionality as SMOperatorPasswordState BUT since inputMode is reimplemented
    //           in the subclass this is required to get the value of the specialized class inputMode
    //           attribute
    virtual inline bool IsPromptingForOperator() { return inputMode == PROMPTING_FOR_OPERATOR; }
    virtual inline bool IsPromptingForPassword() { return inputMode == PROMPTING_FOR_PASSWORD; }

protected:
    virtual bool parentClassCurrentlyHandlingButton2();
    virtual bool parentClassCurrentlyHandlingCancelKey();
    virtual void     showScreen(bool passwordVerified = false);
    virtual SMStateBase * DMScanner(void);
    virtual SMStateBase * PSEnterKey();
    virtual SMStateBase * PSNumericKey(const long);
    virtual SMStateBase * PSClearKey();
    virtual SMStateBase * PSButton1();
    virtual SMStateBase * PSButton2();
    virtual SMStateBase * PSCancelKey();
    virtual SMStateBase * PSButtonGoBack();
    virtual SMStateBase * displayInvalidMessage();
    virtual SMStateBase * OnWtIncreaseNotAllowed(void);
    virtual SMStateBase * OnWtDecrease(void);
    virtual SMStateBase * TareWeightedItem(void);
    virtual SMStateBase * TareOnly(void);
    virtual SMStateBase * TareWLDBItem(void);
    virtual SMStateBase * TareNone(void);
    virtual SMStateBase * TareOnlyAtAttract(void);
    virtual void promptForOperator();
    virtual void promptForPassword();
    virtual bool getOperatorPassword();
    virtual void ShowEnterID();
    virtual SMStateBase * DMScannerEncrypted(void); //SSCOADK-7098

    DECLARE_DYNCREATE(SMSecOperatorPWState)// MFC Runtime class/object information

};

#endif
