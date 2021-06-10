//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSystemMessageOpPass.h
//
// Description: A base state for system messages that require an operator and
//   password to see them.  Note that this state is essentially both a
//   SMSystemMessage state and a SMOperatorPassword state.  Multiple inheritance
//   was not used because both those states inherit from SMHelpModeState,
//   leading to ambiguities, etc.  Thus a SMSystemMessageOpPass state inherits
//   from SMOperatorPassword state and has a SMSystemMessage.
//
// Author: Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SYSTEMMESSAGEOPPASS
#define _SYSTEMMESSAGEOPPASS

#include "SMSystemMessageOpPassBase.h"


#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSystemMessageOpPass : public SMSystemMessageOpPassBase
{
public:
    SMSystemMessageOpPass( BMPS   bitmap,
        PSTEXT title,
        bool   continueButton,
        bool   cancelButton,
        LPCTSTR szMessage,
        PSTEXT psMessage,
        PSTEXT promptText,
        long   promptValue,
        long   deviceClass,
        bool   checkHealth,
        bool displayHTML = false);
    SMSystemMessageOpPass();
    SMStateBase *Deliver( BMPS   bitmap,
        PSTEXT title,
        bool   continueButton,
        bool   cancelButton,
        LPCTSTR szMessage,
        PSTEXT psMessage,
        PSTEXT promptText,
        long   promptValue,
        long   deviceClass,
        bool   checkHealth,
        bool displayHTML = false);
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual SMStateBase * Initialize();
    virtual void UnInitialize();
    virtual SMStateBase  *OnMatchedWt(void);
    void InitSystemMessage( BMPS   bitmap,
        PSTEXT title,
        bool   continueButton,
        bool   cancelButton,
        LPCTSTR szMessage,
        PSTEXT psMessage,
        PSTEXT promptText,
        long   promptValue,
        long   deviceClass,
        bool   checkHealth,
        bool displayHTML = false);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void); 
    virtual SMStateBase  *OnWtDecrease(void);
    virtual long GetDeviceClass();

protected:
    virtual SMStateBase * stateAfterPasswordVerify(); 
    virtual void     showScreen(bool passwordVerified);
    virtual CString  getWaveFile();
    virtual SMStateBase * PSButton1(void);
    virtual SMStateBase * PSButton2(void);
    virtual SMStateBase * PSButton3(void);
    virtual SMStateBase * PSCancelKey(void);
    virtual SMStateBase * OnBackToLGW(void);

    DECLARE_DYNCREATE(SMSystemMessageOpPass)// MFC Runtime class/object information
};

#endif
