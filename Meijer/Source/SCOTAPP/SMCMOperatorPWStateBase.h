//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCMOperatorPWStateBase.h
//
// Description: Derived from SMOperatorPasswordState, SMCashManagementEx class derives from 
//              this class - at least for now
//
// AUTHOR: Jeffrey Yu
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMCMOPERATORPWSTATEBASE
#define _SMCMOPERATORPWSTATEBASE

#ifndef _CPPUNIT
#include "Common.h"
#include "SMOperatorPasswordState.h"
#else
#include "FakeSMOperatorPasswordStateBase.h"
#endif

//////////////////////////////////////
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMCMOperatorPWStateBase : public STATE(OperatorPasswordState)
{
    
public:
    SMCMOperatorPWStateBase();
    virtual SMStateBase *PSParse( const MessageElement* );

protected:
   // All subclasses must tell how to display themselves
   virtual void showScreen(bool passwordVerified = false);
   virtual SMStateBase * PSButtonGoBack();
   virtual SMStateBase * PSEnterKey();
   virtual void promptForOperator();
   virtual void promptForPassword();
   virtual bool getOperatorPassword();
   virtual SMStateBase * stateAfterPasswordVerify(){return STATE_NULL;}

   bool m_bAuthenticationOptional;

   virtual bool OnUserCredentialsAquired();

private:
   virtual void ShowGobackButton();
   virtual void HideGoBackButton();

   DECLARE_DYNCREATE(SMCMOperatorPWStateBase)// MFC Runtime class/object information
    
};
#endif
