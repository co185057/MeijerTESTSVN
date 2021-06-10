//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMSystemMessage4Base.h
//
// Description: A SMSystemMessage4Base is a SMSystemMessageOpPass.  It is
//   like SystemMessage1 (continue but no cancel button), but an
//   operator/password is required to see this one.
//
// Author: Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SystemMessage4BaseSTATE
#define _SystemMessage4BaseSTATE

#include "SMSystemMessageOpPass.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSystemMessage4Base : public STATE(SystemMessageOpPass)
{
public:
  SMSystemMessage4Base( LPCTSTR szMessage,  // char message text or NULL
		                PSTEXT psMessage,  // message text
		                PSTEXT promptText, // prompt text or NULL
		                long   promptValue, // prompt area sale value
                    long   deviceClass, // device class, -1 for none
                    bool   checkHealth);// DMCheckHealth after msg?
	SMSystemMessage4Base();
	~SMSystemMessage4Base();
	SMStateBase *Deliver( LPCTSTR szMessage,  // char message text or NULL
		                PSTEXT psMessage,  // message text
		                PSTEXT promptText, // prompt text or NULL
		                long   promptValue, // prompt area sale value
                    long   deviceClass, // device class, -1 for none
                    bool   checkHealth);
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual void UnInitialize(void);
   virtual bool PSRemoteLegalForState(); //TAR408719

protected:
  virtual CString getWaveFile() { return _T("SecurityLow"); }
  virtual SMStateBase* stateAfterPasswordVerify(); 
	
  DECLARE_DYNCREATE(SMSystemMessage4Base)// MFC Runtime class/object information
};

//DECLARE_STATE(SystemMessage4)

#endif
