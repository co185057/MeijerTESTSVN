//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCMOperatorPWState.h
//
// Description: Derived from SMOperatorPasswordState.
//               This state handles athentication in Cash Management.
//
// AUTHOR: Jeffrey Yu
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMCMOPERATORPWSTATE
#define _SMCMOPERATORPWSTATE


#include "SMCMOperatorPWStateBase.h"

//////////////////////////////////////
#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMCMOperatorPWState : public SMCMOperatorPWStateBase
{
	
public:
    SMCMOperatorPWState();
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

    virtual bool OnUserCredentialsAquired();
    DECLARE_DYNCREATE(SMCMOperatorPWState)// MFC Runtime class/object information

};

#endif
