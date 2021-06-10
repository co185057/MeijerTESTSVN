//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMSystemMessage6.h
//
// Description: A SMSystemMessage6 is a SMSystemMessageOpPass.  It is
//   like SystemMessage3 (continue and cancel buttons), but an
//   operator/password is required to see this one.
//
// Author: Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SYSTEMMESSAGE6STATE
#define _SYSTEMMESSAGE6STATE

#include "SMSystemMessage6Base.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSystemMessage6 : public SMSystemMessage6Base
{
public:
    SMSystemMessage6( LPCTSTR szMessage,  // char message text or NULL
        PSTEXT psMessage,  // message text
        PSTEXT promptText, // prompt text or NULL
        long   promptValue, // prompt area sale value
        long   deviceClass, // device class, -1 for none
        bool   checkHealth,
        bool displayHTML = false);
    SMSystemMessage6();
    SMStateBase *Deliver( LPCTSTR szMessage,  // char message text or NULL
        PSTEXT psMessage,  // message text
        PSTEXT promptText, // prompt text or NULL
        long   promptValue, // prompt area sale value
        long   deviceClass, // device class, -1 for none
        bool   checkHealth,
        bool displayHTML = false);
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);

protected:
    virtual CString getWaveFile();
    virtual SMStateBase* stateAfterPasswordVerify(); 

    DECLARE_DYNCREATE(SMSystemMessage6)// MFC Runtime class/object information
};

#endif
