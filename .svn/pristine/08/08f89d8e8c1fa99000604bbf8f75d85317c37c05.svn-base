//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMSystemMessage6Base.h
//
// Description: A SMSystemMessage6Base is a SMSystemMessageOpPass.  It is
//   like SystemMessage3 (continue and cancel buttons), but an
//   operator/password is required to see this one.
//
// Author: Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SystemMessage6BaseSTATE
#define _SystemMessage6BaseSTATE

#include "SMSystemMessageOpPass.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSystemMessage6Base : public STATE(SystemMessageOpPass)
{
public:
  SMSystemMessage6Base( LPCTSTR szMessage,  // char message text or NULL
		                PSTEXT psMessage,  // message text
		                PSTEXT promptText, // prompt text or NULL
		                long   promptValue, // prompt area sale value
                    long   deviceClass, // device class, -1 for none
                    bool   checkHealth, // DMCheckHealth after msg?
					bool displayHTML = false); //SR672
	SMSystemMessage6Base();
	~SMSystemMessage6Base();
	SMStateBase *Deliver( LPCTSTR szMessage,  // char message text or NULL
		                PSTEXT psMessage,  // message text
		                PSTEXT promptText, // prompt text or NULL
		                long   promptValue, // prompt area sale value
                    long   deviceClass, // device class, -1 for none
                    bool   checkHealth,
					bool displayHTML = false); //SR672

    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize(void);

protected:
  virtual CString getWaveFile() { return _T("SecurityMed"); }
  virtual SMStateBase* stateAfterPasswordVerify(); 

	
  DECLARE_DYNCREATE(SMSystemMessage6Base)// MFC Runtime class/object information
};

//DECLARE_STATE(SystemMessage6)

#endif
