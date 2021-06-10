//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMOperatorPasswordStateBase.h
//
// Description: Derived from SMHelpModeState, all states that need to prompt
//        for an operator and password derive from this state.
//
// AUTHOR: Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMOperatorPasswordStateBase
#define _SMOperatorPasswordStateBase


#ifndef _CPPUNIT
#include "SMHelpModeState.h"
#include "Common.h"

//USSF START
#include "SMUSSFManager.h"
//USSF END
#else
#include "FakeSMHelpModeStateBase.h"
#endif //_CPPUNIT

DLLEXPORT const enum INPUT_MODE
{
  PROMPTING_FOR_OPERATOR,
  PROMPTING_FOR_PASSWORD,
  OPERATOR_PASSWORD_ACCEPTED,
  SHOW_WAIT_FOR_APPROVAL
};


//////////////////////////////////////
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMOperatorPasswordStateBase : public STATE(HelpModeState)
{
	
public:
  SMOperatorPasswordStateBase();
  virtual SMStateBase * Initialize();
  virtual void UnInitialize();

  virtual SMStateBase * createLevel1Message( LPCTSTR szMsg,
		                                    PSTEXT pstMsg,
		                                    PSTEXT prompt,
		                                    long   value,
		                                    long   device,
		                                    bool   checkHealth,
											bool displayHTML=false); //SR672
  virtual SMStateBase * createLevel2Message( LPCTSTR szMsg,
		                                    PSTEXT pstMsg,
		                                    PSTEXT prompt,
		                                    long   value,
		                                    long   device,
		                                    bool   checkHealth,
											bool displayHTML=false); //SR672
  virtual SMStateBase * createLevel3Message( LPCTSTR szMsg,
		                                    PSTEXT pstMsg,
		                                    PSTEXT prompt,
		                                    long   value,
		                                    long   device,
		                                    bool   checkHealth,
											bool displayHTML=false); //SR672

  // All subclasses must tell where to go once a valid operator
  // and password have been entered
  //Tar 334410
  //virtual SMStateBase * stateAfterPasswordVerify(){ return NULL;}
  virtual SMStateBase * stateAfterPasswordVerify(){ return STATE_NULL;}
  //end Tar 334410
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
#ifndef _CPPUNIT
  virtual bool parentClassCurrentlyHandlingButton2()
  { return (inputMode == PROMPTING_FOR_PASSWORD); }
  virtual bool parentClassCurrentlyHandlingCancelKey()
  { return (inputMode == PROMPTING_FOR_PASSWORD); }
#endif //_CPPUNIT

  // All subclasses must tell how to display themselves
  virtual void     showScreen(bool passwordVerified = false){;}
  virtual SMStateBase * DMScanner(void);  //to scan supervisor bar codes
  // Handle PS entry keys for operator/password entry
  virtual SMStateBase * PSEnterKey();
  virtual SMStateBase * PSNumericKey(const long);
  //New UI
  virtual SMStateBase * PSAlphaNumericKey(const long);
  // NewUI
  virtual SMStateBase * PSClearKey();
  virtual SMStateBase * PSButton1();
  virtual SMStateBase * PSButton2();
  virtual SMStateBase * PSCancelKey();
  virtual SMStateBase * PSButtonGoBack();
  virtual SMStateBase * displayInvalidMessage();
  virtual SMStateBase * OnBackToLGW(void);				// Smart Scale reported that the unmatched weight was removed
  virtual void EndOfTransactionPrint(); //SR697 gc185038

  virtual SMStateBase  *PSShiftKey(void);
  virtual SMStateBase  *PSAltGrKey(void);
  virtual SMStateBase  *PSHotKey(CString &);
  virtual SMStateBase  *PSCharKey(TCHAR c);
  virtual SMStateBase  *PSSpaceKey(void);
  virtual SMStateBase  *PSBackSpaceKey();
  virtual SMStateBase  *PSSymbolKey(void);
  virtual SMStateBase *DMScannerEncrypted(void); //SSCOADK-7272

  enum IPSProcedures::HKStatus m_HKStatus;
  bool m_bShiftOn;
  bool m_bAltGrOn;
  bool m_bSymbolOn;
  CString m_csFrameName;  
  int m_nCharLimit;
  CString m_csItemtSentTmp;



//private:
  INPUT_MODE inputMode;

   virtual void promptForOperator();
 //  virtual void invalidOperatorID();
   virtual void promptForPassword();
   virtual bool getOperatorPassword();
   virtual void ShowEnterID();  //SR93 Biometrics support
   virtual SMStateBase *OnSecMgrNeedData(CString exceptionType, long lItemID);
   virtual SMStateBase *HandleScannerData( const MessageElement* );

   void ShowAlphaNumericKeypad();

    virtual bool GetUserInfo();
    virtual bool OnUserCredentialsAquired();

  DECLARE_DYNCREATE(SMOperatorPasswordStateBase)// MFC Runtime class/object information
	
};

#endif
