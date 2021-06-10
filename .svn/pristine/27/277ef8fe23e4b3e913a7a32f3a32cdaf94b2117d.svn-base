//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMSystemMessage.h
//
// Description: Base class for system message classes.  Differences between
//   subclasses are what combination of Continue and Cancel buttons they have.
//
// Author: Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SYSTEMMESSAGE
#define _SYSTEMMESSAGE

#include "SMSystemMessageBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif  
  SMSystemMessage : public SMSystemMessageBase
{ 
public:
           SMSystemMessage(BMPS   bitmap,
                           PSTEXT title,
                           bool   continueButton,
                           bool   cancelButton,
		                       LPCTSTR szMessage,  // char message text or NULL
		                       PSTEXT psMessage,  // message text
		                       PSTEXT promptText, // prompt text or NULL
		                       long   promptValue,
                           long   deviceClass,
                           bool   checkHealth,
                           bool showTotal = true,
                           bool showRetractedTotal = false,
                           bool okButton = false,
                            bool flashOkButton = true);
	SMSystemMessage();
	SMStateBase *Initialize();
	SMStateBase *Deliver(BMPS   bitmap,
                           PSTEXT title,
                           bool   continueButton,
                           bool   cancelButton,
		                       LPCTSTR szMessage,  // char message text or NULL
		                       PSTEXT psMessage,  // message text
		                       PSTEXT promptText, // prompt text or NULL
		                       long   promptValue,
                           long   deviceClass,
                           bool   checkHealth,
                           bool showTotal = true,
                           bool showRetractedTotal = false,
                           bool okButton = false,
                            bool flashOkButton = true);
  DECLARE_DYNCREATE(SMSystemMessage)// MFC Runtime class/object information
};

#endif
