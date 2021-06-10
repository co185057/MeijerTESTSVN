//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMSystemMessage.h
//
// Description: Base class for system message classes.  Differences between
//   subclasses are what combination of Continue and Cancel buttons they have.
//
// Author: Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SYSTEMMESSAGE
#define _SYSTEMMESSAGE

#include "SMSystemMessageBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif  
  SMSystemMessage : public SMSystemMessageBase
{ 
public:
    SMSystemMessage(BMPS   bitmap,
        PSTEXT title,
        bool   continueButton,
        bool   cancelButton,
        LPCTSTR szMessage,
        PSTEXT psMessage,
        PSTEXT promptText,
        long   promptValue,
        long   deviceClass,
        bool   checkHealth,
        bool showTotal = true,
        bool showRetractedTotal = false,
        bool okButton = false,
        bool flashOkButton = true,
        bool fHTML = false);
    SMSystemMessage();
    SMStateBase *Deliver(BMPS   bitmap,
        PSTEXT title,
        bool   continueButton,
        bool   cancelButton,
        LPCTSTR szMessage,
        PSTEXT psMessage, 
        PSTEXT promptText,
        long   promptValue,
        long   deviceClass,
        bool   checkHealth,
        bool showTotal = true,
        bool showRetractedTotal = false,
        bool okButton = false,
        bool flashOkButton = true,
        bool displayHTML=true);
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual SMStateBase * Initialize(void);
    virtual void UnInitialize();
    virtual SMStateBase  *OnMatchedWt(void);
    virtual bool PSRemoteLegalForState(); 
    virtual long GetDeviceClass();
    virtual SMStateBase* CkStateStackAndReturn(void);
    virtual SMStateBase * ShowDeviceErrorNotFixedScreen(void);
    virtual bool IsMediaStatusError(void);
    virtual int      getTimeout();
    virtual void     showScreen(bool showInfo = true);
    virtual SMStateBase * PSButton1(void);
    virtual SMStateBase * PSButton2(void);
    virtual SMStateBase * PSButton3(void);
    virtual SMStateBase * PSCancelKey(void);

protected:
    virtual void     showRemoteScreen(bool showInfo);
    virtual CString  getWaveFile();
    virtual SMStateBase * PSButton8(void);
    virtual SMStateBase * OnBackToLGW(void);
    virtual void    LogDeviceError(void);
    virtual bool getInSystemMessage(void);
    virtual void setInSystemMessage(bool b);

  DECLARE_DYNCREATE(SMSystemMessage)// MFC Runtime class/object information
};

#endif
