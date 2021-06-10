//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMSystemMessage3.h
//
// Description: A SMSystemMessage3 is a SMSystemMessage, whose constructor
//   is called with flags to create a Continue button *and* a Cancel
//   button - a distinguishing feature of a SMSystemMessage3.
//
//
// Author:  Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SYSTEMMESSAGE3STATE
#define _SYSTEMMESSAGE3STATE

#include "SMSystemMessage3Base.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSystemMessage3 : public SMSystemMessage3Base
{
public:
         SMSystemMessage3(LPCTSTR szMessage, // ...char message text or NULL
		                      PSTEXT psMessage, // ...message text
		                      PSTEXT promptText, // ...prompt text or NULL
		                      long   promptValue, // prompt area sale value
                          long   deviceClass, // device class, -1 for none
                          bool   checkHealth);// DMCheckHealth after msg?
	SMSystemMessage3();
	SMStateBase *Deliver(LPCTSTR szMessage, // ...char message text or NULL
		                      PSTEXT psMessage, // ...message text
		                      PSTEXT promptText, // ...prompt text or NULL
		                      long   promptValue, // prompt area sale value
                          long   deviceClass, // device class, -1 for none
                          bool   checkHealth);
	
	
  DECLARE_DYNCREATE(SMSystemMessage3)// MFC Runtime class/object information
};

#endif

