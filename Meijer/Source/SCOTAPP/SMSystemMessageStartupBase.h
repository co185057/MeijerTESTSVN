#ifndef _SYSTEM_MESSAGE_STARTUP_BASESTATE
#define _SYSTEM_MESSAGE_STARTUP_BASESTATE

#ifndef _CPPUNIT
#include "SMSystemMessage4.h"
#else
#include "FakeSMSystemMessage4Base.h"
#endif //_CPPUNIT


/**
 * \class SMSystemMessageStartupBase  
 * \brief A system message state that is dedicated to device errors that occur
 *        at lane startup.
 * \note  Added for RFC 445940.
 */

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
SMSystemMessageStartupBase : public STATE(SystemMessage4)
{
public:
#ifndef _CPPUNIT
    SMSystemMessageStartupBase(LPCTSTR szMessage,  // char message text or NULL
                               PSTEXT psMessage,   // message text
                               PSTEXT promptText,  // prompt text or NULL
                               long promptValue,   // prompt area sale value
                               long deviceClass,   // device class, -1 for none
                               bool checkHealth,   // DMCheckHealth after msg?
                               bool displayHTML = false); //SR672
    SMSystemMessageStartupBase();
    virtual ~SMSystemMessageStartupBase();

    SMStateBase *Deliver(      LPCTSTR szMessage,
                               PSTEXT psMessage,
                               PSTEXT promptText,
                               long promptValue,
                               long deviceClass,
                               bool checkHealth,
                               bool displayHTML = false);
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
#endif //_CPPUNIT

protected:
#ifndef _CPPUNIT
    virtual SMStateBase * PSButton1(void);
    virtual SMStateBase * PSButton2(void);         // TAR 452451

    virtual SMStateBase* createLevel1Message( LPCTSTR szMsg,
                                            PSTEXT pstMsg,
                                            PSTEXT prompt,
                                            long   value,
                                            long   device,
                                            bool   checkHealth,
                                            bool displayHTML=false);

    bool m_bDeviceErrorNotFixed;
#endif //_CPPUNIT
    // MFC Runtime class/object information
    DECLARE_DYNCREATE(SMSystemMessageStartupBase)
};
#endif
