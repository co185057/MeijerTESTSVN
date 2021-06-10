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
#ifndef _CPPUNIT
#include "SMSystemMessageOpPass.h"
#else
#include "FakeSMSystemMessageOpPassBase.h"
#endif //_CPPUNIT

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSystemMessage4Base : public STATE(SystemMessageOpPass)
{
public:
#ifndef _CPPUNIT
  SMSystemMessage4Base( LPCTSTR szMessage,  // char message text or NULL
		                PSTEXT psMessage,  // message text
		                PSTEXT promptText, // prompt text or NULL
		                long   promptValue, // prompt area sale value
                    long   deviceClass, // device class, -1 for none
                    bool   checkHealth, // DMCheckHealth after msg?
					bool displayHTML = false); //SR672
	SMSystemMessage4Base();
	~SMSystemMessage4Base();
	SMStateBase *Deliver( LPCTSTR szMessage,  // char message text or NULL
		                PSTEXT psMessage,  // message text
		                PSTEXT promptText, // prompt text or NULL
		                long   promptValue, // prompt area sale value
                    long   deviceClass, // device class, -1 for none
                    bool   checkHealth,
					bool displayHTML = false); //SR672
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual void UnInitialize(void);
   virtual bool PSRemoteLegalForState(); //TAR408719
#endif //_CPPUNIT

    virtual bool	 storeMode();


protected:
#ifndef _CPPUNIT

  virtual CString getWaveFile() { return _T("SecurityLow"); }
  virtual SMStateBase* stateAfterPasswordVerify(); 
  virtual SMStateBase  *SASecMgrParse( const MessageElement*);
#endif //_CPPUNIT
	
  DECLARE_DYNCREATE(SMSystemMessage4Base)// MFC Runtime class/object information
};

//DECLARE_STATE(SystemMessage4)

#endif
