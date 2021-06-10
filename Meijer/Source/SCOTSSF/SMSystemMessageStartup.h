#ifndef _SYSTEM_MESSAGE_STARTUP_STATE
#define _SYSTEM_MESSAGE_STARTUP_STATE

#include "SMSystemMessageStartupBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSystemMessageStartup : public SMSystemMessageStartupBase
{
public:
    SMSystemMessageStartup( LPCTSTR szMessage,  // char message text or NULL
                            PSTEXT psMessage,  // message text
                            PSTEXT promptText, // prompt text or NULL
                            long   promptValue, // prompt area sale value
                            long   deviceClass, // device class, -1 for none
                            bool   checkHealth,
                            bool displayHTML = false);
    SMSystemMessageStartup();
    SMStateBase *Deliver(   LPCTSTR szMessage,  // char message text or NULL
                            PSTEXT psMessage,  // message text
                            PSTEXT promptText, // prompt text or NULL
                            long   promptValue, // prompt area sale value
                            long   deviceClass, // device class, -1 for none
                            bool   checkHealth,
                            bool displayHTML = false);
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);

protected:
    virtual SMStateBase * PSButton1(void);
    virtual SMStateBase * PSButton2(void);

    virtual SMStateBase* createLevel1Message( LPCTSTR szMsg,
                                            PSTEXT pstMsg,
                                            PSTEXT prompt,
                                            long   value,
                                            long   device,
                                            bool   checkHealth,
                                            bool displayHTML=false);
protected:
    virtual CString getWaveFile();

    // MFC Runtime class/object information
    DECLARE_DYNCREATE(SMSystemMessageStartup)
};

#endif
