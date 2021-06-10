//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMSystemMessage4.h
//
// Description: A SMSystemMessage4 is a SMSystemMessageOpPass.  It is
//   like SystemMessage1 (continue but no cancel button), but an
//   operator/password is required to see this one.
//
// Author: Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SYSTEMMESSAGE4STATE
#define _SYSTEMMESSAGE4STATE

#include "SMSystemMessage4Base.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSystemMessage4 : public SMSystemMessage4Base
{
public:
    SMSystemMessage4( LPCTSTR szMessage,  // char message text or NULL
        PSTEXT psMessage,  // message text
        PSTEXT promptText, // prompt text or NULL
        long   promptValue, // prompt area sale value
        long   deviceClass, // device class, -1 for none
        bool   checkHealth,
        bool displayHTML = false);
    SMSystemMessage4();
    SMStateBase *Deliver( LPCTSTR szMessage,  // char message text or NULL
        PSTEXT psMessage,  // message text
        PSTEXT promptText, // prompt text or NULL
        long   promptValue, // prompt area sale value
        long   deviceClass, // device class, -1 for none
        bool   checkHealth,
        bool displayHTML = false);
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual void UnInitialize(void);
    virtual bool PSRemoteLegalForState();

protected:
    virtual CString getWaveFile();
    virtual SMStateBase* stateAfterPasswordVerify(); 

    DECLARE_DYNCREATE(SMSystemMessage4)// MFC Runtime class/object information
};

#endif
