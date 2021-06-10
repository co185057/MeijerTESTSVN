//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMSystemMessage5.h
//
// Description: A SMSystemMessage5 is a SMSystemMessageOpPass.  It is
//   like SystemMessage2 (cancel but no continue button), but an
//   operator/password is required to see this one.
//
// Author: Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SYSTEMMESSAGE5STATE
#define _SYSTEMMESSAGE5STATE

#include "SMSystemMessage5Base.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSystemMessage5 : public SMSystemMessage5Base
{
public:
  SMSystemMessage5( LPCTSTR szMessage,  // char message text or NULL
		                PSTEXT psMessage,  // message text
		                PSTEXT promptText, // prompt text or NULL
		                long   promptValue, // prompt area sale value
                    long   deviceClass, // device class, -1 for none
                    bool   checkHealth);// DMCheckHealth after msg?
	SMSystemMessage5();
	SMStateBase *Deliver( LPCTSTR szMessage,  // char message text or NULL
		                PSTEXT psMessage,  // message text
		                PSTEXT promptText, // prompt text or NULL
		                long   promptValue, // prompt area sale value
                    long   deviceClass, // device class, -1 for none
                    bool   checkHealth);

  DECLARE_DYNCREATE(SMSystemMessage5)// MFC Runtime class/object information
};

#endif
