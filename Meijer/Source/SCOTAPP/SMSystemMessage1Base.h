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

#ifndef _CPPUNIT
#include "SMSystemMessage.h"
#else
#include "FakeSMSystemMessageBase.h"
#endif //_CPPUNIT

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSystemMessage1Base : public STATE(SystemMessage)
{
public:
#ifndef _CPPUNIT
           SMSystemMessage1Base(LPCTSTR szMessage,   // char message text or NULL
		                        PSTEXT psMessage,   // message text
		                        PSTEXT promptText,  // prompt text or NULL
		                        long   promptValue, // prompt area sale value
                            long   deviceClass, // device class, -1 for none
                            bool   checkHealth, // DMCheckHealth after msg?
							bool displayHTML = false); //SR672
	SMSystemMessage1Base();
	SMStateBase *Deliver(LPCTSTR szMessage,   // char message text or NULL
		                        PSTEXT psMessage,   // message text
		                        PSTEXT promptText,  // prompt text or NULL
		                        long   promptValue, // prompt area sale value
                            long   deviceClass, // device class, -1 for none
                            bool   checkHealth,
							bool displayHTML = false); //SR672
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
#endif //_CPPUNIT
    virtual bool	 storeMode();
	
protected:
#ifndef _CPPUNIT
  virtual CString getWaveFile() { return _T("SecurityLow"); }

  DECLARE_DYNCREATE(SMSystemMessage1Base)// MFC Runtime class/object information
#endif //_CPPUNIT

};

//DECLARE_STATE(SystemMessage1)

#endif
