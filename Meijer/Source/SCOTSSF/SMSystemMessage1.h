//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMSystemMessage1.h
//
// Description: A SMSystemMessage1 is a SMSystemMessage, whose constructor
//   is called with flags to create a Continue button but not a Cancel
//   button - a distinguishing feature of a SMSystemMessage1.
//
//
// Author:  Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SYSTEMMESSAGE1STATE
#define _SYSTEMMESSAGE1STATE

#include "SMSystemMessage1Base.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSystemMessage1 : public SMSystemMessage1Base
{
public:
    SMSystemMessage1(LPCTSTR szMessage,   // char message text or NULL
        PSTEXT psMessage,   // message text
        PSTEXT promptText,  // prompt text or NULL
        long   promptValue, // prompt area sale value
        long   deviceClass, // device class, -1 for none
        bool   checkHealth,
        bool displayHTML=false);// DMCheckHealth after msg?
    SMSystemMessage1();
    SMStateBase *Deliver(LPCTSTR szMessage,   // char message text or NULL
        PSTEXT psMessage,   // message text
        PSTEXT promptText,  // prompt text or NULL
        long   promptValue, // prompt area sale value
        long   deviceClass, // device class, -1 for none
        bool   checkHealth,
        bool displayHTML=false);

    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual bool	 storeMode();

protected:
    virtual CString getWaveFile();

    DECLARE_DYNCREATE(SMSystemMessage1)// MFC Runtime class/object information
};

#endif
