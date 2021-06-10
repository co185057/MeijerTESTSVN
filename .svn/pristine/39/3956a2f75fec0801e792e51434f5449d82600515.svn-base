//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSystemMessageOpPassBase.h
//
// Description: A base state for system messages that require an operator and
//   password to see them.  Note that this state is essentially both a
//   SMSystemMessage state and a SMOperatorPassword state.  Multiple inheritance
//   was not used because both those states inherit from SMHelpModeState,
//   leading to ambiguities, etc.  Thus a SMSystemMessageOpPassBase state inherits
//   from SMOperatorPassword state and has a SMSystemMessage.
//
// Author: Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SystemMessageOpPassBase
#define _SystemMessageOpPassBase
#ifndef _CPPUNIT
#include "SMOperatorPasswordState.h"
#include "SMSystemMessage.h"
#else
#include "FakeSMOperatorPasswordStateBase.h"
#endif //_CPPUNIT


#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSystemMessageOpPassBase : public STATE(OperatorPasswordState)
{
public:
#ifndef _CPPUNIT
  SMSystemMessageOpPassBase( BMPS   bitmap,
                         PSTEXT title,       // specify PS_NOTEXT for default
                         bool   continueButton,
                         bool   cancelButton,
                         LPCTSTR szMessage,   // NULL if psMessage specified
                         PSTEXT psMessage,   // PS_NOTEXT if szMessage specified
                         PSTEXT promptText,  // PS_NOTEXT for no prompt
		                     long   promptValue, // prompt area sale value
                         long   deviceClass, // device class, -1 for none
                         bool   checkHealth, // DMCheckHealth after msg?
						 bool displayHTML = false); //SR672
	SMSystemMessageOpPassBase();
	SMStateBase *Deliver( BMPS   bitmap,
                         PSTEXT title,       // specify PS_NOTEXT for default
                         bool   continueButton,
                         bool   cancelButton,
                         LPCTSTR szMessage,   // NULL if psMessage specified
                         PSTEXT psMessage,   // PS_NOTEXT if szMessage specified
                         PSTEXT promptText,  // PS_NOTEXT for no prompt
		                     long   promptValue, // prompt area sale value
                         long   deviceClass, // device class, -1 for none
                         bool   checkHealth,
						 bool displayHTML = false); //SR672
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual SMStateBase * Initialize();
  virtual void UnInitialize();					// TAR203166
  virtual SMStateBase  *OnMatchedWt(void);		// Smart Scale reported a matched weight TAR203166
  void InitSystemMessage( BMPS   bitmap,
                         PSTEXT title,       // specify PS_NOTEXT for default
                         bool   continueButton,
                         bool   cancelButton,
                         LPCTSTR szMessage,   // NULL if psMessage specified
                         PSTEXT psMessage,   // PS_NOTEXT if szMessage specified
                         PSTEXT promptText,  // PS_NOTEXT for no prompt
		                     long   promptValue, // prompt area sale value
                         long   deviceClass, // device class, -1 for none
                         bool   checkHealth, // DMCheckHealth after msg?
						 bool displayHTML = false); //SR672
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void); 
  virtual SMStateBase  *OnWtDecrease(void);
  
virtual long GetDeviceClass(); //307389

/******cm150000: Added In Merged Code******/
public:
	CString csVerificationState; // "Y" or "N"
/******cm150000: End Merged Code***********/

protected:
  // Stay here after operator/password entered
  virtual SMStateBase * stateAfterPasswordVerify(); 
  virtual void     showScreen(bool passwordVerified);
  virtual CString  getWaveFile() { return _T(""); }

  virtual SMStateBase * PSButton1(void);
  virtual SMStateBase * PSButton2(void);
  virtual SMStateBase * PSButton3(void);
  virtual SMStateBase * PSCancelKey(void);
  virtual SMStateBase * OnBackToLGW(void);				// Smart Scale reported that the unmatched weight was removed
  
//private:
  STATE(SystemMessage) theSystemMessage;
#endif

  DECLARE_DYNCREATE(SMSystemMessageOpPassBase)// MFC Runtime class/object information
};

#endif
