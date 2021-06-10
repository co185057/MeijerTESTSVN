//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMSystemMessage2.h
//
// Description: A SMSystemMessage2 is a SMSystemMessage, whose constructor
//   is called with flags to create a Cancel button but not a Continue
//   button - a distinguishing feature of a SMSystemMessage2.
//
//
// Author:  Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SYSTEMMESSAGE2STATE
#define _SYSTEMMESSAGE2STATE

#include "SMSystemMessage2Base.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSystemMessage2 : public SMSystemMessage2Base
{
public:
           SMSystemMessage2(LPCTSTR szMessage,  // char message text or NULL
		                        PSTEXT psMessage,  // message text
		                        PSTEXT promptText, // prompt text or NULL
		                        long   promptValue, // prompt area sale value
                            long   deviceClass, // device class, -1 for none
                            bool   checkHealth);// DMCheckHealth after msg?
	SMSystemMessage2();
	SMStateBase *Deliver(LPCTSTR szMessage,  // char message text or NULL
		                        PSTEXT psMessage,  // message text
		                        PSTEXT promptText, // prompt text or NULL
		                        long   promptValue, // prompt area sale value
                            long   deviceClass, // device class, -1 for none
                            bool   checkHealth);

	
  DECLARE_DYNCREATE(SMSystemMessage2)// MFC Runtime class/object information
};

#endif
