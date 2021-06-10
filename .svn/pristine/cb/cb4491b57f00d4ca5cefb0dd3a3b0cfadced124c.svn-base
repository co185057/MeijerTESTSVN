//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMSystemMessage2Base.h
//
// Description: A SMSystemMessage2Base is a SMSystemMessage, whose constructor
//   is called with flags to create a Cancel button but not a Continue
//   button - a distinguishing feature of a SMSystemMessage2Base.
//
//
// Author:  Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SystemMessage2BaseSTATE
#define _SystemMessage2BaseSTATE

#include "SMSystemMessage.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSystemMessage2Base : public STATE(SystemMessage)
{
public:
           SMSystemMessage2Base(LPCTSTR szMessage,  // char message text or NULL
		                        PSTEXT psMessage,  // message text
		                        PSTEXT promptText, // prompt text or NULL
		                        long   promptValue, // prompt area sale value
                            long   deviceClass, // device class, -1 for none
                            bool   checkHealth);// DMCheckHealth after msg?
	SMSystemMessage2Base();
	SMStateBase *Deliver(LPCTSTR szMessage,  // char message text or NULL
		                        PSTEXT psMessage,  // message text
		                        PSTEXT promptText, // prompt text or NULL
		                        long   promptValue, // prompt area sale value
                            long   deviceClass, // device class, -1 for none
                            bool   checkHealth);
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);

  virtual SMStateBase  *Initialize(void);
	
protected:
  virtual CString getWaveFile() { return _T("SecurityMed"); }
	
  DECLARE_DYNCREATE(SMSystemMessage2Base)// MFC Runtime class/object information
};

//DECLARE_STATE(SystemMessage2)

#endif
