//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMSystemMessage1Base.h
//
// Description: A SMSystemMessage1Base is a SMSystemMessage, whose constructor
//   is called with flags to create a Continue button but not a Cancel
//   button - a distinguishing feature of a SMSystemMessage1Base.
//
//
// Author:  Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SystemMessage1BaseSTATE
#define _SystemMessage1BaseSTATE

#include "SMSystemMessage.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSystemMessage1Base : public STATE(SystemMessage)
{
public:
           SMSystemMessage1Base(LPCTSTR szMessage,   // char message text or NULL
		                        PSTEXT psMessage,   // message text
		                        PSTEXT promptText,  // prompt text or NULL
		                        long   promptValue, // prompt area sale value
                            long   deviceClass, // device class, -1 for none
                            bool   checkHealth);// DMCheckHealth after msg?
	SMSystemMessage1Base();
	SMStateBase *Deliver(LPCTSTR szMessage,   // char message text or NULL
		                        PSTEXT psMessage,   // message text
		                        PSTEXT promptText,  // prompt text or NULL
		                        long   promptValue, // prompt area sale value
                            long   deviceClass, // device class, -1 for none
                            bool   checkHealth);
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
	
protected:
  virtual CString getWaveFile() { return _T("SecurityLow"); }

  DECLARE_DYNCREATE(SMSystemMessage1Base)// MFC Runtime class/object information
};

//DECLARE_STATE(SystemMessage1)

#endif
