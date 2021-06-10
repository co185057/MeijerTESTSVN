//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSecOperatorPWStateBase.h
//
// Description: Derived from SMHelpModeState, all states that need to prompt
//        for an operator and password derive from this state.
//
// AUTHOR: Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMSECOPERATORPWSTATEBASE
#define _SMSECOPERATORPWSTATEBASE

#include "Common.h"

//#include "SMHelpModeState.h"
#include "SMOperatorPasswordState.h"


//////////////////////////////////////
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSecOperatorPWStateBase : public STATE(OperatorPasswordState)
{
	
public:
  SMSecOperatorPWStateBase();
  virtual SMStateBase * Initialize();
  virtual void UnInitialize();

  virtual SMStateBase * createLevel1Message( LPCTSTR szMsg,
		                                    PSTEXT pstMsg,
		                                    PSTEXT prompt,
		                                    long   value,
		                                    long   device,
		                                    bool   checkHealth );
  virtual SMStateBase * createLevel2Message( LPCTSTR szMsg,
		                                    PSTEXT pstMsg,
		                                    PSTEXT prompt,
		                                    long   value,
		                                    long   device,
		                                    bool   checkHealth );
  virtual SMStateBase * createLevel3Message( LPCTSTR szMsg,
		                                    PSTEXT pstMsg,
		                                    PSTEXT prompt,
		                                    long   value,
		                                    long   device,
		                                    bool   checkHealth );

  // All subclasses must tell where to go once a valid operator
  // and password have been entered
  virtual SMStateBase * stateAfterPasswordVerify(){ return NULL;}

protected:

//USSF Start
  friend SMUSSFManagerBase;
  friend SMUSSFManager;
//USSF End

  // Note: no subclasses should accept button cancel, pinpad cancel, or
  // pinpad OK (enter) unless this class says it's OK!
  // The presentation changes when prompting for the password, and we must
  // have control of button 2 (Cancel) and pinpad Cancel over subclasses.
  // In this context, this state is the parent class.
  bool fInValidOperatorPassword;
  virtual bool parentClassCurrentlyHandlingButton2()
  { return (inputMode == PROMPTING_FOR_PASSWORD); }
  virtual bool parentClassCurrentlyHandlingCancelKey()
  { return (inputMode == PROMPTING_FOR_PASSWORD); }

  // All subclasses must tell how to display themselves
  virtual void     showScreen(bool passwordVerified = false){;}
  virtual SMStateBase * DMScanner(void);  //to scan supervisor bar codes
  // Handle PS entry keys for operator/password entry
  virtual SMStateBase * PSEnterKey();
  virtual SMStateBase * PSNumericKey(const long);
  virtual SMStateBase * PSClearKey();
  virtual SMStateBase * PSButton1();
  virtual SMStateBase * PSButton2();
  virtual SMStateBase * PSCancelKey();
  virtual SMStateBase * PSButtonGoBack();
  virtual SMStateBase * displayInvalidMessage();

  virtual SMStateBase * OnWtIncreaseNotAllowed(void);	// Smart Scale reported an unmatched wt increase
  virtual SMStateBase * OnWtDecrease(void);
  virtual SMStateBase * TareWeightedItem(void);
  virtual SMStateBase * TareOnly(void);
  virtual SMStateBase * TareWLDBItem(void);
  virtual SMStateBase * TareNone(void);
  virtual SMStateBase * TareOnlyAtAttract(void);//SR085
//private:
  INPUT_MODE inputMode;

  BEST m_PrevState, m_NextState;


   virtual void promptForOperator();
 //  virtual void invalidOperatorID();
   virtual void promptForPassword();
   virtual bool getOperatorPassword();
   virtual void ShowEnterID();  //SR93 Biometrics support

  DECLARE_DYNCREATE(SMSecOperatorPWStateBase)// MFC Runtime class/object information
	
};

#endif
