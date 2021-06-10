//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMSystemMessage3Base.h
//
// Description: A SMSystemMessage3Base is a SMSystemMessage, whose constructor
//   is called with flags to create a Continue button *and* a Cancel
//   button - a distinguishing feature of a SMSystemMessage3Base.
//
//
// Author:  Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SystemMessage3BaseSTATE
#define _SystemMessage3BaseSTATE

#include "SMSystemMessage.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSystemMessage3Base : public STATE(SystemMessage)
{
public:
         SMSystemMessage3Base(LPCTSTR szMessage, // ...char message text or NULL
		                      PSTEXT psMessage, // ...message text
		                      PSTEXT promptText, // ...prompt text or NULL
		                      long   promptValue, // prompt area sale value
                          long   deviceClass, // device class, -1 for none
                          bool   checkHealth, // DMCheckHealth after msg?
						  bool displayHTML = false); //SR672
	SMSystemMessage3Base();
	SMStateBase *Deliver(LPCTSTR szMessage, // ...char message text or NULL
		                      PSTEXT psMessage, // ...message text
		                      PSTEXT promptText, // ...prompt text or NULL
		                      long   promptValue, // prompt area sale value
                          long   deviceClass, // device class, -1 for none
                          bool   checkHealth,
						  bool displayHTML = false); //SR672
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
virtual SMStateBase  *Initialize(void);
	
	
protected:
  virtual CString getWaveFile() { return _T("SecurityMed"); }
	
//private:
  bool  fHealth;
  long  lDevice;  // DM device causing error msg or -1
	
  DECLARE_DYNCREATE(SMSystemMessage3Base)// MFC Runtime class/object information
};

//DECLARE_STATE(SystemMessage3)

#endif

